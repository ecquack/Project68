/*
      40/80 Column Video Display using a PIC18F26K20.

      Copyright (c) 2010 Erik C. Quackenbush
      Non-commercial distribution permitted.

CPU Crystal is:
14.318180 mHz which is 4X NTSC colorburst giving 69.84 nanosecond pixels
13.5 mHz which is CCIR-601 square pixels 74.07 nanoseconds
16 mHz which is the maximum clock rate (4X=64mhz) with 62.5 nanosecond pixels

hardware details:

connect the video output to PIN_C2 through a 680 ohm resistor
connect the video output to PIN_C7 through a 330 ohm resistor
run the chip at 3.3 volts
connect an LED to PIN_A5

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

*/
#include <18F26K20.H>
#device HIGH_INTS=true

//#define CRYSTAL13       // 13.5mHz crystal CCIR-601 square pixels

#define CRYSTAL14     // 14.31818 mHz crystal NTSC pixels (4X color burst)

//#define CRYSTAL16     // 16mHz internal oscillator works but the display
                        // is wavy. You really need an HS crystal.

#define  COLUMN40       // define COLUMN40 if you want a 40 column display
                        // comment it out if you want an 80 column display
                        // 40 columns looks good on standard TV sets.
                        // 80 columns looks good on most flat panel TVs.
                        
                        // 40 columns uses 1000 bytes less memory. You can run
                        // a 40 column display on a PIC with only 1536 bytes
                        // of RAM like the PIC18F25K20.
                        // The PIC18F25K22 has two EUSARTS so you would still
                        // have a free serial port and a 40 column display.
                        

/**************************************************************************/

#ifdef CRYSTAL14
   #use     delay(clock=57272720)
#else
   #ifdef CRYSTAL13
      #use  delay(clock=54000000)
   #else // CRYSTAL16
      #use  delay(clock=64000000)
   #endif
#endif

#fuses   H4,NOWDT,NOPROTECT,BROWNOUT,PUT,NOLVP

#include <string.h>
#include <stdio.h>
#include "vt100font.c"  // this is important
//#include "ps2.c"      // PS/2 keyboard support coming soon

#define  SYNC_BIT          2
#define  SYNC_PIN          PIN_C2      // pin 13
#define  CLOCK_PIN         PIN_C6      // pin 17
#define  PIXEL_PIN         PIN_C7      // pin 18
#define  VSYNC_PIN         PIN_B3      // pin 24

const char copyright[]="Copyright (C) 2010 Erik C. Quackenbush, Inc.\r\nNon-commercial distribution permitted\r\nVERSION 1";

// console is our 80 (or 40) column by 25 line array of character values

#ifdef COLUMN40
   #define CONSOLECOLS   40
#else
   #define CONSOLECOLS   80
#endif
#define CONSOLEROWS   26
#define CONSOLERAM   (CONSOLEROWS*CONSOLECOLS+2) //2082  // why waste 48 bytes of RAM at the end?
#define CONSOLELAST  (CONSOLECOLS*25)
                           // so don't change it.

// These definitions depend upon the oscillator frequency.
// Note that while we can use the chip's internal 16mhz oscillator it is not
// stable. the video signal it generates wiggles around a lot. 
// Use a crystal.

#ifdef CRYSTAL14
#define  HLINEPERIOD    227         // period of timer 2 which is pixels per hline divided by 4.
#define  HALFLINE       102         // during VSYNC we emit two pulses per line. this time between them in 4 pixel units
#define  HSyncHChars    8           // duration of HSYNC pulse in 4 pixel units
#define  VSyncHChars    4           // duration of VSYNC pulse in 4 pixel units
#define  HBackPorch     24          // horizontal backporch in 4 pixel units
                                    // change the backporch to adjust the 
                                    // horizontal (overscan) position.
#else
   #ifdef CRYSTAL13
      #define  HLINEPERIOD    215
      #define  HALFLINE       96    
      #define  HSyncHChars    7      
      #define  VSyncHChars    4      
      #define  HBackPorch     23      
   #else
      #define  HLINEPERIOD    254
      #define  HALFLINE       115
      #define  HSyncHChars    8  
      #define  VSyncHChars    4     
      #define  HBackPorch     24    
   #endif
#endif

#define  VSYNCLINE      230            // change this to adjust the top margin (overscan area)
#define  ACTIVE         CONSOLEROWS*8  // 200 active lines.
#define  FIELDLINES     262            // We're off by a half line but we're not interlaced.

//
// EUSART
// we're using the EUSART as a high speed shift register to 
// output pixels. Master synchronous transmit mode.
//

#define  SPEN_BIT  7
#define  SPEN      0x80  // serial port enable
#define  CSRC      0x80  // master mode/slave
#define  SYNC      0x20  // synchronous/asynchronous
#define  TXEN      0x10  // transmit/receive

#byte    RCSTA=   0xFAB
#byte    TXSTA=   0xFAC // transmit control and status
#byte    TXREG=   0xFAD
#byte    RCREG=   0xFAE
#byte    SPBRG=   0xFAF // baud rate
#byte    SPBRGH=  0xFB0 // high byte

// PORT C latch
#byte    LATC=    0xF8B

// indirect addressing
#byte    INDF0=   0xFEF
#byte    POSTINC0=0xFEE
#byte    POSTDEC0=0xFED

#byte    FSR0H=   0xFEA
#byte    FSR0L=   0xFE9

// table access to ROM
#byte    TBLPTRU= 0xFF8
#byte    TBLPTRH= 0xFF7
#byte    TBLPTRL= 0xFF6
#byte    TABLAT=  0xFF5


// global

#byte    STATUS=  0xFD8
#byte    BSR=     0xFE0
#byte    PCLATH=  0xFFA


/******************** GLOBAL VARIABLES *************************/

char console[CONSOLERAM];
#locate console=0x100      // this address is hard coded because
                           // it must be at the start of a page (256).
                           // You could change it to 0x200, 0x300, etc.
                           // but not 0x180, 0x317, etc.

int conX=0,conY=1;     // position of cursor
#define CURSOR (32+128) // reverse video space cursor

int seconds=0,minutes=0,hours=0,fields=0,blankscreen=0;
unsigned int32 ticks=0;

// this holds our current scanline (0-261)
long hline=0;     
int hlinel,hlineh,heven=0;
#locate hline=0x50
#locate hlineh=hline+1
#locate hlinel=hline
#locate heven=0x52

// this is our offset into console memory
long charW=console;// &console[0]==0x100
int charL,charH;
#locate chawW=0x53
#locate charH=charW+1
#locate charL=charW

/*************** end of globals ***********************/


#INLINE
void Delay_4_pixels(int count)   // ~waits count*8 pixel clocks (count character cells)
{
#asm
chardelay:
      NOP            // four instructions per loop == 1/2 character at 80 columns (1 character at 40 columns)
      DECFSZ   count
      BRA   chardelay
#endasm
}

void DoSyncPulse(void)
{
   output_low(SYNC_PIN);
   delay_4_pixels(HSyncHChars);
   output_high(SYNC_PIN);
}

void DoHalfSync(void)
{
   output_low(SYNC_PIN);
   delay_4_pixels(VSyncHChars);
   output_high(SYNC_PIN);
}

void DoHalfSyncNeg(void)
{
   output_high(SYNC_PIN);
   delay_4_pixels(VSyncHChars);
   output_low(SYNC_PIN);
}

void HScan(void)
{
   static int hloop,hcount;
   
   if(hline<ACTIVE) // active video
   {
      if(blankscreen==0)
      {
#asm
      BCF      LATC,SYNC_BIT     // output_low(SYNC_PIN);
      MOVLW    HSyncHChars
      MOVWF    hcount
hsyncpulse:
      NOP            // four instructions per half character
      DECFSZ   hcount
      BRA      hsyncpulse

      BSF      LATC,SYNC_BIT   // output_high(SYNC_PIN);

backporch:
      MOVLW    HBackPorch   // back porch delay
      MOVWF    hcount
chardelay:
      NOP            // four instructions per half character
      DECFSZ   hcount
      BRA      chardelay

      MOVF   hlinel,W
      ANDLW  0x07
      BZ     topofline        // if((hlinel&0x07)!=0) 

#ifdef   COLUMN40
      MOVLW  0x28          // charw=charw-40; 
#else
      MOVLW  0x50         // charw=charw-80;
#endif
      SUBWF  charL,F
      MOVLW  0x00
      SUBWFB charH,F  // this is a 16 bit subtract so undercarry
      BRA    setloop    //else

topofline:
      NOP               // it takes 8 scan lines to draw a character. we subtract
      NOP               // charW to the beginning of the line each time except
      NOP               // when we're on the first line (top of char).
      NOP               // The NOPs are here to ensure the same number of cycles
      NOP               // until the first pixel whether we branch or not.

setloop:
      MOVLW    40          // loops per line. we do 2 chars per loop in 80 column mode
      MOVWF    hloop
      
      MOVFF    charL,FSR0L // load our offset into console character memory
      MOVFF    charH,FSR0H      
      CLRF     TBLPTRU
      MOVLW    0x07
      ANDWF    hlinel,w
      ADDLW    0x20        // assumes font starts at 0x2000, add the line offset
      MOVWF    TBLPTRH
      CLRF     TBLPTRL     // TBLPTR= 0x2000 + (hlinel%8 * 0x100)

      CLRF     TXREG
      BSF      RCSTA,SPEN_BIT // turn on the shift register
overscan:
      CLRF     TXREG
      NOP
      NOP    

#ifdef COLUMN40
pixels:
      MOVFF    POSTINC0,TBLPTRL // move from *FSR0++ to the Table address low byte
      TBLRD    *                   // read our font data
      MOVFF    TABLAT,TXREG
      DECF     hloop,F
      NOP                        // we need this NOP
      NOP
      NOP
      NOP
      NOP
      NOP
      NOP
      BNZ      pixels          // are we done with the line?
endpixels:
      NOP
      NOP
      NOP
      NOP
      NOP
      NOP
      NOP
      NOP
      NOP
      NOP
#else
pixels:
      MOVFF    POSTINC0,TBLPTRL // move from *FSR0++ to the Table address low byte
      TBLRD    *                   // read our font data
      MOVFF    TABLAT,TXREG
      DECF     hloop,F
      NOP                        // we need this NOP
      MOVFF    POSTINC0,TBLPTRL // move from *FSR0++ to the Table address low byte
      TBLRD    *                 // read our font data
      MOVFF    TABLAT,TXREG
      BNZ      pixels
endpixels:
      NOP   // we need these in 80 column mode
      NOP
      NOP
#endif

      MOVFF    FSR0L,charL  // save our character position
      MOVFF    FSR0H,charH

      CLRF     RCSTA       // OK, shut off the shift register now

      BCF      LATC,7  // PIXEL_PIN low

#endasm
      }
      else     // just do the sync pulse because we're blanking the display right now.
      {
#asm
      BCF      LATC,SYNC_BIT     // output_low(SYNC_PIN);
      MOVLW    HSyncHChars
      MOVWF    hcount
hsyncpulseblank:
      NOP            // four instructions per half character
      DECFSZ   hcount
      BRA      hsyncpulseblank

      BSF      LATC,SYNC_BIT   // output_high(SYNC_PIN);
#endasm

#ifdef COLUMN40
         if((hline&0x07)==0) charW+=40;
#else
         if((hline&0x07)==0) charW+=80;
#endif
      }
   }
   else
   {
      switch(hline)
      {
         case ACTIVE:
               charW=console;// this is 0x100 at the end of each active field we reset the pointer into character memory
               break;
         case VSYNCLINE-5:
               // we trigger the VSYNC early because we're using it as a trigger 
               // for our analog oscilloscope. For a "real" VSYNC you want to
               // trigger it later and you might want to reverse the polarity.
               output_high(VSYNC_PIN);
               DoSyncPulse();
               break;
         case VSYNCLINE:
         case VSYNCLINE+1:
               DoHalfSync();
               Delay_4_pixels(HALFLINE);
               DoHalfSync();
               break;
         case VSYNCLINE+2:
               DoHalfSync();
               Delay_4_pixels(HALFLINE);
               if(heven)
               {
                  DoHalfSync();
               }
               else
               {
                  DoHalfSyncNeg();             
               }
               break;
         case VSYNCLINE+3:
         case VSYNCLINE+4:
               DoHalfSyncNeg();
               Delay_4_pixels(HALFLINE);
               DoHalfSyncNeg();
               break;
         case VSYNCLINE+5:
               if(heven)
               {
                  DoHalfSyncNeg();
                  Delay_4_pixels(HALFLINE);
                  DoHalfSyncNeg();
                  Delay_4_pixels(2);
                  output_high(SYNC_PIN);
               }
               break;
         case VSYNCLINE+6:
                  DoHalfSync();
                  Delay_4_pixels(HALFLINE);
                  DoHalfSync();
               break;
         case VSYNCLINE+7:
               if(heven)
               {
                  DoHalfSync();
                  Delay_4_pixels(HALFLINE);
                  DoHalfSync();
               }
               else
               {
                  // disable TIMER2 for 1/2 scan line.
               }
               break;
         case VSYNCLINE+8:                // VSYNC done? let's do some housekeeping
               output_low(VSYNC_PIN);
               fields++;
//               if(heven) heven=0; else heven=1;
               ticks++;
               blankscreen=0;
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
            DoSyncPulse();
      };
   }
   
   hline++;
   if(hline>FIELDLINES)
   {
      hline=0;
   }

}

#INT_TIMER2 FAST
void Hsync(void)  // called @ ~15,750 kHz
{
// we save & restore these registers
   static int int_w,int_bsr,int_status;
   static int int_fsr0l,int_fsr0h;
   static int int_tabl,int_tabh,int_pclh,int_tabat; 

#asm
   // save our registers
   // if we let the compiler do it for us it wastes many cycles saving
   // things that we don't trash like the multiplier results and FSR1 & FSR2
   MOVWF    int_w                // save the Working register
   MOVFF    STATUS,int_status    // save the STATUS register
   MOVFF    BSR,int_bsr          // save the BSR
   MOVLB    0                    // BSR=0
   MOVFF    FSR0L,int_fsr0l      // save FSR0 (we don't trash FSR1 or FSR2 so we don't save them)
   MOVFF    FSR0H,int_fsr0h
   MOVFF    TBLPTRL,int_tabl     // save the table pointer and table latch
   MOVFF    TBLPTRH,int_tabh
   MOVFF    TABLAT,int_tabat
   MOVFF    PCLATH,int_pclh      // save the program counter
//   MOVFF    TBLPTRU,int_tabu   // we only have 64K bytes of ROM so we can ignore the upper address bits
                                 // if we were using a processor with an external memory interface or 
                                 // 128K bytes of ROM we would have to store TBLPTRU.
#endasm
   HScan(); /// draw a scanline on the screen
#asm

   // restore our registers before returning
   MOVFF    int_fsr0l,FSR0L
   MOVFF    int_fsr0h,FSR0H
   MOVFF    int_pclh,PCLATH
   MOVFF    int_tabl,TBLPTRL
   MOVFF    int_tabh,TBLPTRH
//   MOVFF    int_tabu,TBLPTRU
   MOVFF    int_tabat,TABLAT
   MOVFF    int_bsr,BSR
   MOVF     int_w,w
   MOVFF    int_status,STATUS
#endasm
   
}

#ifdef COLUMN40
void Init40Double(void)
{
   long index;
   int dubleft,dubright;
   const char convert[16]={0x00,0x03,0x0C,0x0F,0x30,0x33,0x3C,0x3F,0xC0,0xC3,0xCC,0xCF,0xF0,0xF3,0xFC,0xFF};


   for(index=0;index<256;index++)
   {
      dubleft=convert[index>>4];
      dubright=convert[index&0x0F];
   
      console[1024+index]=dubleft;
      console[1024+256+index]=dubright;  
   }
}
#endif


void InitConsoleRAM(void)
{
   long index;
  
  //
  // note that we actually have a 26 line display but first line is never used.
  // we initialize it to zeroes and then leave it alone.
  // if we don't do it this way some TV sets will show horizontal tearing of the
  // display at the top. It's not enough to adjust the VBLANK interval.
  //
  
   for(index=0;index<CONSOLECOLS*2;index++) console[index]=0; //wipe the first two lines

   for(index=CONSOLECOLS*2;index<CONSOLERAM;index+=CONSOLECOLS) // fill the rest of the screen with a test pattern
   {
                                 //123456789-123456789-12345678-123456789-123456789-123456789-123456789-123456789-
#ifdef COLUMN40
   Init40Double();
                              // 123456789-123456xx789-123456789-123456789-
      sprintf(&console[index],"%2ld THIS IS LINE %2ld          30        40",index/CONSOLECOLS-1,index/CONSOLECOLS-1);
#else
      sprintf(&console[index],"%2ld THIS IS LINE %2ld         30        40        50        60        70        80",index/CONSOLECOLS-1,index/CONSOLECOLS-1);
#endif      
//      sprintf(&console[index],"%2ld THIS IS LINE %2ld 123456789-123456789-123456789-123456789-123456789-1234567890",index/80-1,index/80-1);

     //this overwrites our 2000 characters by 1: it adds a null terminator past the end of the thing.
   }

   for(index=0;index<256;index++)
   {
      console[index+160]=index;
   }

#ifdef COLUMN40
   strcpy(&console[CONSOLECOLS+CONSOLECOLS/2-4],"EQUACK 40"); // centered at the top of the screen
#else
   strcpy(&console[CONSOLECOLS+CONSOLECOLS/2-4],"EQUACK 80"); // centered at the top of the screen
#endif
   strcpy(&console[CONSOLECOLS*12],"The quick brown fox jumped over the lazy dogs.");
}

void InitSystem(void)
{
   InitConsoleRAM();

#ifdef COLUMN40
   SPBRG=1;
#else   
   SPBRG=0;
#endif   
   SPBRGH=0;  // baud rate divisor zero (full clock speed)
   TXSTA=CSRC | SYNC | TXEN; // master synchronous transmit mode

   //////   RCSTA=SPEN; // this turns on the hose inside our interrupt
     
   output_high(SYNC_PIN); // sync pin goes LOW to drop below IRE 0.3 volts

   setup_timer_2(T2_DIV_BY_4,HLINEPERIOD,1);


   EXT_INT_EDGE(2, H_TO_L); // set interrupt for falling edge

   enable_interrupts(INT_TIMER2);   // horizontal sync interrupt
   enable_interrupts(GLOBAL);
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

void CursorOn()
{
//   console[conX+conY*80]|=0x80;

}

void CursorOff()
{
//   console[conX+conY*80]^=0x80;
}


void ScrollUp(void)
{
   long index;
   
   for(index=CONSOLECOLS;index<CONSOLERAM-CONSOLECOLS;index++)
   {
      console[index]=console[index+CONSOLECOLS];
   }

  for(index=CONSOLELAST;index<CONSOLELAST+CONSOLECOLS;index++) console[index]=32;
  
  conX=0;

}


void ScrollOff(void)
{
   for(;;)
   {
      strcpy(&console[CONSOLELAST],"123456789-123456789-123456789-123456789-");
      ScrollUp();
      delay_ms(100);
   }
}

//---------------Main() entry point--------------------------------
void main( void )
{
    char *right;
    int index;
    right=copyright; 

   output_drive(PIXEL_PIN); // this is our video output
   output_drive(CLOCK_PIN); // we could shut this off for RFU.

   InitSystem();

//   ScrollOff();
//   InitKeyboard();
//   KeyRead();
   for(;;)
   {
    index++;
   }
}

