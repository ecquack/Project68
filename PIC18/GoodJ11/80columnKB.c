/*
      Copyright (c) 2010 Trigonetics Instruments, Inc.
      All Rights Reserved
*/

// Crystal is 14.318180 mHz which is 4X NTSC color burst run
// as H4 with 4X PLL clock.
// it gives us ~70ns pixels
/*

hardware details:

connect the video output to PIN_C2 through a 680 ohm resistor
connect the video output to PIN_C7 through a 330 ohm resistor
run the chip at 3.3 volts
connect an LED to PIN_A5
connect a speaker (through an amp or transistor) to PIN_C1

we're using EUSART as our video shift register.
baud rate divisor is 0 so it outputs one pixel per CPU clock (Fosc/4)
which should be 14.318 mHz or ~70ns

we use timer 2 to generate an interrupt at the beginning of each scan line.
We use a divide by 4 prescaler and set the period register to 227. This
gives us 908 cycles per line. We want to have 910 cycles per line. We can disable the
timer for a single clock cycle (one instruction to turn it off, the following instruction
turns it back on) or two or three in order to get our horizontal sweep frequency just right.
It should be 15.75 khz

we are only generating one field- not a true interlaced signal
we are therefore one line short: 524 lines instead of 525.


VT52 commands

ESC-< Enter VT52 mode
ESC-= alternate numeric keypad mode
ESC-> standard numeric keypad mode
ESC-A Cursor Up
ESC-B Cursor Down
ESC-C Cursor Right
ESC-D Cursor Left
ESC-E Clear Home
ESC-F Use graphics font ROM
ESC-G Use normal US/UK font ROM
ESC-H Cursor Home
ESC-I reverse line feed 
ESC-J Clear to end of screen
ESC-K Delete rest of line
ESC-L Insert Line
ESC-M Delete Line
ESC-Y Positioning Cursor (followed by 2 characters: X, Y where code is 32+X 32+Y (always printable)
ESC-Z Identify terminal (correct response is ESC-/-K is VT52, ESC-/-Z is VT100?) or ESC-/-L if there is a printer attached

ESC-[ hold screen mode (batch input?)
ESC-\ exit hold screen mode


ESC-p Reverse video
ESC-q Normal video

CTL-G  7 BEL
CTL-H  8 BACKSPACE
CTL-I  9 TAB (VT52 tab stops are 8 characters 0,8,16,24...)
CTL-J 10 Linefeed
CTL-L 12 FormFeed
CTL-M 13 CARRIAGE return
CTL-N 14 SHIFT OUT: same as escape Y cursor positioning
CTL-Z 26 Document end
arrow keys:

ESC-A
ESC-B
ESC-C
ESC-D

function keys:

ESC-OP   PF1
ESC-OQ   PF2
ESC-OR   PF3
ESC-OS   PF4

numeric keypad alternate keys:

(note: VT100 is ESC-Op, VT52 is ESC-?p)

ESC-?p   0
ESC-?q   1
ESC-?r   2
ESC-?s   3
ESC-?t   4
ESC-?u   5
ESC-?v   6
ESC-?w   7
ESC-?x   8
ESC-?y   9
ESC-Om   - (minus) // not on VT52
ESC-Ol   , (comma) // not on VT52
ESC-?n   . (period
ESC-?M   enter (^M)


graphics mode:

codes 136-176 octal (94-126, 0x5E-0x7E the lower case letters and symbols )
symbols replaced in graphics mode are: "^_`{|}~ a-z"

special characters in graphics mode are:

8 horizontal bars at various heights (0-7)
numeric subscripts 0-9
solid rectangle
1/
3/
5/
7/
degrees
plus or minus
right arrow
ellipsis
divide by
down arrow
paragraph
two blanks
one reserved

*/

#include <18F26K20.H>
#device HIGH_INTS=true
#use     delay(clock=57272720)
#fuses   H4,NOWDT,NOPROTECT,BROWNOUT,PUT,NOLVP

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "vt100font.c"

/*

0x1C
---111--
0x38
--111---

*/

//#define MSSP      // use the MSSP instead of the EUSART for our video shift register
//
// we can't use the MSSP. it takes NINE cycles to send a byte, not eight.
// also, the bit ordering is different. it's LSB first.
//
//

#define  PIXELS_PER_LINE   910
#define  PIXELS_LOW        (PIXELS_PER_LINE%256)
#define  PIXELS_HIGH       (PIXELS_PER_LINE/256)

#define  KBCLK_PIN      PIN_B2
#define  KBDAT_PIN      PIN_B1

#define  SYNC_PIN          PIN_C2
#define  VSYNC_PIN         PIN_B3
#define  LED_PIN           PIN_A5
#define  SOUND_PIN         PIN_C1
#ifdef   MSSP
   #define  PIXEL_PIN         PIN_C5
   #define  CLOCK_PIN         PIN_C3
#else
   #define  PIXEL_PIN         PIN_C7
   #define  CLOCK_PIN         PIN_C6
#endif
const char copyright[]="Copyright (C) 2010 Trigonetics Instruments, Inc.\r\nAll Rights Reserved\r\nVERSION 1";



// console is our 80 column by 25 line array of character values
// we actually store 81 columns per line. a value of zero means
// end of line for the ISR. 
#define CONSOLECOLS   80    
#define CONSOLEROWS   26
#define CONSOLERAM   2082  // why waste 48 bytes of RAM at the end?
#define CONSOLELAST  (80*25+1)
                           // so don't change it.
char console[CONSOLERAM];
#locate console=0x100      // this address is hard coded into HSync() function

int conX=0,conY=25;     // position of cursor
#define CURSOR (32+128) // reverse video space cursor

//
// EUSART
// we're using the EUSART as a high speed shift register to 
// output pixels. Master synchronous transmit mode.
//


#define SPEN      0x80  // serial port enable

#define CSRC      0x80  // master mode/slave
#define SYNC      0x20  // synchronous/asynchronous
#define TXEN      0x10  // transmit/receive


#byte RCSTA=   0xFAB
#byte TXSTA=   0xFAC // transmit control and status
#byte TXREG=   0xFAD
#byte RCREG=   0xFAE
#byte SPBRG=   0xFAF // baud rate
#byte SPBRGH=  0xFB0 // high byte

// synchronous serial port
#define  SMP            0x80
#define  CKE            0x40
#define  SSPEN          0x20
#define  CKP            0x10
#define  SSP_MASTER_F4  0x00
#define  SSP_MASTER_F16 0x01 
#define  SSP_MASTER_F64 0x02
#define  SSP_MASTER_TM2 0x03
#define  SSP_SLAVE_SS   0x04
#define  SSP_SLAVE_NOSS 0x05

#byte SSPSTAT=  0xFC7
#byte SSPADD =  0xFC8
#byte SSPBUF =  0xFC9
#byte SSPCON1 = 0xFC6
#byte SSPCON2=  0xFC5


// PORT C latch
#byte LATC=    0xF8B

// interrupt control
#byte INTCON2= 0xFF1
#define INTEDG2   0x10

// indirect addressing
#byte POSTINC0=0xFEE
#byte POSTINC1=0xFE6
#byte FSR0H=   0xFEA
#byte FSR0L=   0xFE9
#byte FSR1H=   0xFE2
#byte FSR1L=   0xFE1

#byte TBLPTRU= 0xFF8
#byte TBLPTRH= 0xFF7
#byte TBLPTRL= 0xFF6
#byte TABLAT=  0xFF5


// compare mode (PWM)
#byte CCPR1H=  0xFBF
#byte CCPR1L=  0xFBE



// video defines
#DEFINE  HALFLINE  105              // need to get teh sync 
#define  VSYNCLINE   230            // change this to adjust the top margin (overscan area)
#define  ACTIVE      CONSOLEROWS*8  // 200 active lines. We could do more, especially if we supported PAL not just NTSC.
#define  FIELDLINES  262            // yeah- we're off by a half line. We're not really interlaced.

#define  HSyncHChars    8
#define  VSyncHChars    4
#define  VSyncHNeg      105
#define  HBlankHChars   20    // change this to adjust the left margin (overscan area)

long charW=0x0100;
int charL,charH;
#locate charW=0x72
#locate charH=0x73
#locate charL=0x72

int keybits=0;
int keymiss=0;
int keystate=0;
int keycode=0;
int keylast=0;
int keybyte=0;

int seconds=0,minutes=0,hours=0,fields=0;
unsigned int32 ticks=0;

long hline=0;
int hlinel,hlineh;
#locate hline=0x70
#locate hlineh=0x71
#locate hlinel=0x70

//*************** end of globals ***********************

#INT_EXT2 HIGH    // the keyboard take priority over the video scan so we don't lose any bits
void KBInt(void) // function called on falling edge of PS/2 keyboard clock
{
   int hold;
   
   hold=input(KBDAT_PIN);  // grab the data
   
   keybits++;
   switch(keybits)
   {
      case 11: // stop
               if(keystate) keymiss++;
               keylast=keycode;
               keycode=keybyte;
               keybyte=0;
               keystate=1;
               keybits=0;
               
               return;
      case 1:  // start bit
               break;
      case 2:  // d0
      case 3:  // d1
      case 4:  // d2
      case 5:  // d3
      case 6:  // d4
      case 7:  // d5
      case 8:  // d6
      case 9:  // d7
      case 10: // d8?
               keybyte>>=1;
               if(hold) keybyte|=0x80;
               break;
   }   

}

void KBtimeout(void)
{
   keybits=0;
   keybyte=0;
   
}

/*
#INT_EXT2 HIGH    // the keyboard take priority over the video scan so we don't lose any bits
void KBInt(void) // function called on falling edge of PS/2 keyboard clock
{
   int hold;
   
   hold=input(KBDAT_PIN);  // grab the data
   
   keybits++;
   switch(keybits)
   {
      case 1:  // start bit
               break;
      case 2:  // d0
      case 3:  // d1
      case 4:  // d2
      case 5:  // d3
      case 6:  // d4
      case 7:  // d5
      case 8:  // d6
      case 9:  // d7
      case 10: // d8?
               keybyte>>=1;
               if(hold) keybyte|=0x80;
               break;
//      case 10:  // parity
               // we ignore parity for now
            //   break;
      case 11: // stop
               if(keystate) keymiss++;
               keylast=keycode;
               keycode=keybyte;
               keybyte=0;
               keystate=1;
               keybits=0;
               
               return;
               break;
 
   }   

}
*/


#INLINE
void Delay_Half_Chars(int count)   // ~waits count*8 pixel clocks (count character cells)
{
#asm
chardelay:
      NOP            // four instructions per pixel
      DECFSZ   count
      BRA   chardelay
#endasm
}


void SpitLine(void)
{
int loop;

   loop=40;
   if((hlinel%8)!=0)
   {
      charw=charw-80;

   }
   else
   {
   // this is to balance us out, otherwise line 0 is shifted to the left by a few pixels
#asm
      NOP
      NOP
      NOP
      NOP
      NOP
#endasm
   }



#asm
initHline:
/*
      MOVF   hlinel,W
      ANDLW  07
      BZ    noset
      MOVLW  50
      SUBWF  charL,F
      MOVLW  00
      SUBWFB charH,F
      BRA   setloop
noset:
      NOP
      NOP
      NOP
      NOP
      */
setloop:
      MOVLW    40             // characters per line divided by two
      MOVWF    loop
      
      MOVFF    charL,FSR0L
      MOVFF    charH,FSR0H      
      CLRF     TBLPTRU
      MOVLW    0x07
      ANDWF    hlinel,w
      ADDLW    0x20           // assumes font starts at 0x2000, add the line offset
      MOVWF    TBLPTRH
      CLRF     TBLPTRL
  


                              // TBLPTR= 0x2000 + (hlinel%8 * 0x100)
#ifdef MSSP
      MOVLW    SSPEN
      MOVWF    SSPCON1
#else
      MOVLW    SPEN        // MOVFF does not work- we have to do separate instructions
      MOVWF    RCSTA
#endif

overscan1:
      MOVLW    0
#ifdef MSSP
      MOVWF    SSPBUF      // suppress white bars on the left? this makes them thinner
#else
      MOVWF    TXREG
#endif
      NOP
      NOP
          
overscan2:
      NOP
      NOP
      NOP
      NOP
      MOVLW    0
#ifdef MSSP
      MOVWF    SSPBUF
#else
      MOVWF    TXREG
#endif
      NOP
      NOP
/*
// known good
      MOVLW    SPEN        // MOVFF does not work- we have to do separate instructions
      MOVWF    RCSTA

      MOVLW    0
      MOVWF    TXREG      // suppress white bars on the left?
      NOP                     // we need this NOP
      NOP                     // we need this NOP
*/      
  //  18 instruction cycles    
      
pixel:

      MOVFF   POSTINC0,TBLPTRL // move from *FSR0++ to the Table address low byte
      TBLRD    *                   // read our font data
#ifdef MSSP
      MOVFF    TABLAT,SSPBUF
#else
      MOVFF    TABLAT,TXREG
#endif
      DECF     loop,F
      NOP                        // we need this NOP

      MOVFF   POSTINC0,TBLPTRL   // move from *FSR0++ to the Table address low byte
      TBLRD    *                 // read our font data
#ifdef MSSP
//      MOVFF    TABLAT,SSPBUF
      NOP
      NOP
#else
      MOVFF    TABLAT,TXREG
#endif      
      BNZ      pixel             // if we moved a ZERO from POSTINC0 then we're at the end of the line
                                 // otherwise we grab the next pixel byte

#ifdef MSSP
      CLRF     SSPCON1
#else
      CLRF     RCSTA
#endif      

      BCF      LATC,7  // PIXEL_PIN low

      MOVFF    FSR0L,charL  // save our character position
      MOVFF    FSR0H,charH


#endasm
}

void DoSyncPulse(void)
{
   output_low(SYNC_PIN);
   delay_half_chars(HSyncHChars);
   output_high(SYNC_PIN);
}

void DoHalfSync(void)
{
   output_low(SYNC_PIN);
   delay_half_chars(VSyncHChars);
   output_high(SYNC_PIN);
}

void DoHalfSyncNeg(void)
{
   output_high(SYNC_PIN);
   delay_half_chars(VSyncHChars);
   output_low(SYNC_PIN);
}

#INT_TIMER2
void Hsync(void)  // this function gets called 15,750 (15,734?) times per second approximately every 910 instruction clocks
{
   if(hline<ACTIVE) // active video
   {
      DoSyncPulse();
      delay_half_chars(HBlankHChars);
      SpitLine();
   }
   else
   {
      switch(hline)
      {
         case ACTIVE:
               charW=0x100;
               break;
/*
         case FIELDLINES-4:
         case FIELDLINES-3:
         case FIELDLINES-2:
         case FIELDLINES-1:
               DoSyncPulse();
               delay_half_chars(HBlankHChars);
               SpitLine();
               break;
*/

/*
one source says to do 2ms sync pulses in vsync. 30ms on, 2ms off or vice versa
pattern is 6-5-5 (PAL)

Non-interlaced or field I of interlaced:

6x short sync  (30ms on, 2ms off)
5x long  sync (2ms on, 30 ms off)
5x short sync  (30ms on, 2ms off)
(total of 16 double speed syncs or 8 scan lines)
normal sync pulse is 4ms.
back porch is 8ms


for interlaced we do

5:5:4 at the beginning of field II.

full sync
5x short sync
5x long sync
4x short sync
(total of 14 double speed syncs or 7 scan lines)
back to full sync

also need to check our horizontal sync interval. Is it 15.xx kHz or a bit too long/too short?

*/
         case VSYNCLINE-5:
               output_high(VSYNC_PIN);
               DoSyncPulse();
               break;
         case VSYNCLINE:
               DoSyncPulse();
               Delay_Half_Chars(HALFLINE);
               DoHalfSync();
               break;
         case VSYNCLINE+1:
         case VSYNCLINE+2:
               DoHalfSync();
               Delay_Half_Chars(HALFLINE);
               DoHalfSync();
               break;
         case VSYNCLINE+3:
         case VSYNCLINE+4:
               DoHalfSyncNeg();
               Delay_Half_Chars(HALFLINE);
               DoHalfSyncNeg();
               break;
         case VSYNCLINE+5:
               DoHalfSyncNeg();
               Delay_Half_Chars(HALFLINE);
               DoHalfSyncNeg();
               Delay_Half_Chars(2);
               output_high(SYNC_PIN);
               break;
         case VSYNCLINE+6:
         case VSYNCLINE+7:
               DoHalfSync();
               Delay_Half_Chars(HALFLINE);
               DoHalfSync();
               break;
         case VSYNCLINE+8:
               output_low(VSYNC_PIN);
               fields++;
               ticks++;
               if(fields==60)
               {
                  fields=0;
                  seconds++;
                  if(seconds==60)
                  {
                     seconds=0;
                     minutes++;
                     if(minutes==60)
                     {
                        minutes=0;
                        hours++;
                     }
                  }
               }
         default:
               DoSyncPulse(); // ordinary vertical blanking
      };
   }
   
   hline++;
   if(hline>FIELDLINES)
   {
      hline=0;
   }
   
}

void InitSystem(void)
{
   long index;
  
   for(index=0;index<160;index++) console[index]=0;

   for(index=160;index<CONSOLERAM;index+=80)
   {
                                 //123456789-123456789-12345678-123456789-123456789-123456789-123456789-123456789-
      sprintf(&console[index],"%2ld THIS IS LINE %2ld         30        40        50        60        70        80",index/80-1,index/80-1);
//      sprintf(&console[index],"%2ld THIS IS LINE %2ld 123456789-123456789-123456789-123456789-123456789-1234567890",index/80-1,index/80-1);

     //this overwrites our 2000 characters by 1: it adds a null terminator past the end of the thing.
   }

   for(index=0;index<256;index++)
   {
      console[index+160]=index;
   }

   strcpy(&console[116],"EQUACK 80"); // centered at the top of the screen

   strcpy(&console[960],"The quick brown fox jumped over the lazy dogs.");

   SPBRG=0;
   SPBRGH=0;  // baud rate divisor zero (full clock speed)
   TXSTA=CSRC | SYNC | TXEN; // master synchronous transmit mode

   SSPSTAT= SMP | CKE;
   /////   SSPCON1= SSPEN; // this turns on the hose


   //////   RCSTA=SPEN; // this turns on the hose inside our interrupt
     
   output_high(SYNC_PIN); // sync pin goes LOW to drop below IRE 0.3 volts

   setup_timer_2(T2_DIV_BY_4,227,1);


   EXT_INT_EDGE(2, H_TO_L); // set interrupt for falling edge

   enable_interrupts(INT_TIMER2);   // horizontal sync interrupt
   enable_interrupts(INT_EXT2);     // PS/2 keyboard clock line interrupt
   enable_interrupts(GLOBAL);
}

void Beep(void)
{
   long index;
   
   for(index=0;index<250;index++)
   {
      output_high(SOUND_PIN);
      delay_us(500);
      output_low(SOUND_PIN);
      delay_us(500);
   }
}

void BlinkLED(int count)
{
   int index;
   
   output_high(LED_PIN);
   for(index=0;index<count;index++)
   {
      output_low(LED_PIN);
      delay_ms(250);
      output_high(LED_PIN);
      delay_ms(250);
   }

}

void delay_fields(int interval) // delay a number of vertical blank intervals, 16.7 milliseconds each.
{
   int wait;
   
   //
   // note: after 4 billion frames there is a small chance of 
   // error if we do a delay just as the 32bit ticks counter
   // is about to overflow.
   //
   // if the system runs for 2.25 years this function COULD hang.
   //
   wait=fields+interval;
   for(;;)
   {
      if(fields>=wait) return;
   }
}

void ScrollUp(void)
{
   long index;
   
   for(index=80;index<CONSOLERAM-80;index++)
   {
      console[index]=console[index+80];
   }

  for(index=CONSOLELAST;index<CONSOLELAST+CONSOLECOLS;index++) console[index]=32;
  
  conX=0;

}

void ScrollOff(void)
{
   long count;
   long overflow;
   long index;
   long chosen;
   char graph[80];
   count=0;
   overflow=0;
   
   for(;;)
   {
      ScrollUp();

 //    sprintf(&console[CONSOLELAST],"Line: %ld, Random: 0x%04lx",count++,rand());
      chosen=rand();
      for(index=0;index<chosen%10;index++)
      {
         graph[index]='*';
         graph[index+1]=0;
      }
      
      if(overflow)
      {
         sprintf(&console[CONSOLELAST],"Line %ld%04ld Random: %ld Hours Minutes Seconds Fields  %d:%02d:%02d.%02d(%02d)%s",overflow,count++,chosen,hours,minutes,seconds,fields,keybits,graph);
      }
      else
      {
         sprintf(&console[CONSOLELAST],"Line %ld Random: %5ld Hours Minutes Seconds Fields  %d:%02d:%02d.%02d(%02d)%s",count++,chosen,hours,minutes,seconds,fields,keybits,graph);
      }
      if(count==10000)
      {
         count=0;
         overflow++;
         
      }
      delay_ms(10);
//      delay_fields(30); // wait 0.5 seconds
   }
}

void PrintLine(char *line)
{
   ScrollUp();
   strcpy(&console[CONSOLELAST],line);
}

void PrintChar(char symbol)
{
   console[CONSOLELAST+conX]=symbol;
   conX++;
   if(conX<79)
   {
      console[CONSOLELAST+conX+1]=CURSOR;
   }
   else
   {
      ScrollUp();
      conX=0;
      console[CONSOLELAST]=CURSOR;
   }
}

void Backspace(void)
{
   if(conX)
   {
      console[CONSOLELAST+conX]=0;
      console[CONSOLELAST+conX-1]=CURSOR;
      conX--;
   }
}

// scan code, normal, shift, control

//char E0scancode[]={}
#include "ps2.c"

#define  CAPS     0x58
#define  LSHIFT   0x12
#define  LCTRL    0x14

void KeyRead(void)
{
   char emit;
   int thiskey;
   int shifted=0;
   int controlled=0;
   int release;
   int even=0;
   char shing[4];
   char out;
   long scandex;
   
   ScrollUp();
   for(;;)
   {
      if(keystate)
      {
         thiskey=keycode;
         release=keylast;
         keystate=0;
         if(release!=0xF0) release=0;


//         if(!release) // this was a keypress, not a release
         {
//            if(scancodes[thiskey*3+2]==0) // not a control coode
            {
               scandex=thiskey*3;
               out=scancodes[scandex];
               sprintf(shing,"%02X%c",thiskey,out);
               PrintChar(shing[0]+128);
               PrintChar(shing[1]+128);
               PrintChar(shing[2]);
               PrintChar(shing[3]);
//               PrintChar(scancodes[thiskey*3]);
            }
         }
/*
         sprintf(&console[CONSOLELAST+position],"%02X:%c",thiskey,emit);
         position+=5;
         if(position>75)
         {
            position=0;
         }
*/
      }
   }
}

/*

*/

//---------------Main() entry point--------------------------------
void main( void )
{
    char *right;
    int index;
    
//    start=restart_cause(); 
//    if(start==NORMAL_POWER_UP)
   right=copyright; // force compiler to include our Copyright notice in Flash ROM

   Beep();
   BlinkLED(0);   // just turn the damn light on
   
   output_drive(PIXEL_PIN); // this is our video output
   output_drive(CLOCK_PIN); // we could shut this off for RFU.

   output_float(KBCLK_PIN); // clock is usually an input
   output_float(KBDAT_PIN); // data is usually an input

   port_b_pullups(0x06); // B1 and B2 pulled up

   InitSystem();

   //ScrollOff();
   KeyRead();
   for(;;)
   {
    index++;
   }
}


