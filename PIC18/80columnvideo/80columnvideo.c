/*
      40/80 Column Video Display using a PIC18F26K20.

      Copyright (c) 2010 Erik C. Quackenbush
      Non-commercial distribution permitted.

This version works correctly DOUBLE40 and 80 with white stripe along side
of 80. If we change the HPORCH value from 34 to 30 then the white stripe
goes away but we get a color flash every 22 seconds.

We did not have a color flash yesterday.


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

//#define KCPU
#define SAVESCRATCH

#ifdef KCPU
#include <18F26K20.H>
#define  HSPLL H4
#else // J CPU
#include <18F26J11.H>
#endif


//#define CRYSTAL28
//#define CRYSTAL13       // 13.5mHz crystal CCIR-601 square pixels
#define CRYSTAL14     // 14.31818 mHz crystal NTSC pixels (4X color burst)
//#define CRYSTAL16     // 16mHz internal oscillator works but the display
                        // is wavy. You really need an HS crystal.

//#define  DOUBLE40       // define DOUBLE40 if you want a 40 column display
                        // comment it out if you want an 80 column display
                        // 40 columns looks good on standard TV sets.
                        // 80 columns looks good on most flat panel TVs.
                        
                        // 40 columns is implemented as a 640 pixel display
                        // that creates double wide pixels in software for
                        // a 40 column display. This eliminates the gap
                        // between adjacent characters that seems to be a
                        // bug in the EUSART when run at BRG=1 (Tcy/2).
                        // it costs us 512 bytes of RAM for a loopup table
                        // we can't use ROM because the TBLRD instruction
                        // takes two extra clock cycles.
                        

/**************************************************************************/

#ifdef CRYSTAL28
   #use delay(clock=28636360)
   #define SINGLE40     // we do 40 columns differently.
#else
   #define USEPLL
#endif

#ifdef CRYSTAL14
   #use     delay(clock=57272720)
#endif

#ifdef CRYSTAL13
   #use  delay(clock=54000000)
#endif

#ifdef CRYSTAL16
#use  delay(clock=64000000)
#endif


#ifdef USEPLL
   #fuses   HSPLL,NOWDT,NOPROTECT//,PUT,NOLVP
#else
   #fuses   HS,NOWDT,NOPROTECT//,PUT,NOLVP
#endif

//#include <string.h>
//#include <stdio.h>
#include "romfont.c"

#define  SYNC_BIT          2
#define  SYNC_PIN          PIN_C2      // pin 13
#define  CLOCK_PIN         PIN_C6      // pin 17
#define  PIXEL_PIN         PIN_C7      // pin 18
#define  SOUND_PIN         PIN_C1
#ifdef KCPU
#define  LED_PIN           PIN_A5
#else // JCPU
#define  LED_PIN           PIN_A3
#endif

const char copyright[]="Copyright (C) 2010 Erik C. Quackenbush, Inc.\r\nNon-commercial distribution permitted\r\nVERSION 1.1";

#ifdef DOUBLE40
   #define CONSOLECOLS   40
#else
   #ifdef SINGLE40
   #define CONSOLECOLS 40
   #else
   #define CONSOLECOLS   80
   #endif
#endif
#define CONSOLEROWS   25
#define CONSOLERAM   (CONSOLEROWS*CONSOLECOLS+2) //2082  // why waste 48 bytes of RAM at the end?
#define CONSOLELAST  (CONSOLECOLS*(CONSOLEROWS-1) )
                           // so don't change it.


// These definitions depend upon the oscillator frequency.
// Note that while we can use the chip's internal 16mhz oscillator it is not
// stable. the video signal it generates wiggles around a lot. 
// Use a crystal.

#ifdef CRYSTAL28
#define  HLINEPERIOD    113         // period of timer 2 which is pixels per hline divided by 4.
#define  HALFLINE       40         // during VSYNC we emit two pulses per line. this time between them in 4 pixel units
#define  HSyncHChars    4           // duration of HSYNC pulse in 4 pixel units
#define  VSyncHChars    2           // duration of VSYNC pulse in 4 pixel units
#define  HBackPorch     9          // horizontal backporch in 4 pixel units
#endif

#ifdef CRYSTAL14
#define  HLINEPERIOD    227         // period of timer 2 which is pixels per hline divided by 4.
#define  HALFLINE       102         // during VSYNC we emit two pulses per line. this time between them in 4 pixel units
#define  HSyncHChars    8           // duration of HSYNC pulse in 4 pixel units
#define  VSyncHChars    4           // duration of VSYNC pulse in 4 pixel units
   #ifdef DOUBLE40
   #define  HBackPorch     20          // horizontal backporch in 4 pixel units
   #else
   #define  HBackPorch     30
   #endif
#endif                              // change the backporch to adjust the
                                    // horizontal (overscan) position.
#ifdef CRYSTAL13
#define  HLINEPERIOD    215
#define  HALFLINE       96
#define  HSyncHChars    7   
#define  VSyncHChars    4     
#define  HBackPorch     16
#endif

#ifdef CRYSTAL16
#define  HLINEPERIOD    254
#define  HALFLINE       115
#define  HSyncHChars    8  
#define  VSyncHChars    4     
#define  HBackPorch     27    
#endif

#define  VSYNCLINE      220            // change this to adjust the top margin (overscan area)
#define  ACTIVE         CONSOLEROWS*8  // 200 active lines.
#define  FIELDLINES     262            // start at 1, 263=0 We're off by a half line but we're not interlaced.
#define  OVERSCANLINES  10

//
// EUSART
// we're using the EUSART as a high speed shift register to 
// output pixels. Master synchronous transmit mode.
//
#include "registers.c"

/******************** GLOBAL VARIABLES *************************/

unsigned char console[CONSOLERAM];
#locate console=0x100      // this address is hard coded because
                           // it must be at the start of a page (256).
                           // You could change it to 0x200, 0x300, etc.
                           // but not 0x180, 0x317, etc.

int conX=0,conY=0;     // position of cursor
#define CURSOR (32+128) // reverse video space cursor



int seconds=0,minutes=0,hours=0,fields=0,blankscreen=0,even=0;
unsigned int32 ticks=0; // number of horizontal video lines since powerup

#include "ps2.c"      // PS/2 keyboard support coming soon

// this is our offset into console memory

#ifdef DOUBLE40

unsigned char Dubs[256];
unsigned char Dubs2[256];
#locate Dubs=0x500
#locate Dubs2=0x600

long DubW=0x500; // just past the end of the 40 column console RAM
int DubL,DubH;
#locate DubW=0x56
#locate DubH=DubW+1
#locate DubL=DubW

long Dub2W=0x600; // one page past the end of the 40 column console RAM
int Dub2L,Dub2H;
#locate Dub2W=0x58
#locate Dub2H=Dub2W+1
#locate Dub2L=Dub2W

#endif


/*************** end of globals ***********************/

/*************** start of video interrupt code ********/

#INLINE
void UpdateClock(void)
{
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
}

unsigned int safety[0x15]; // this stores our registers during ISR
#locate safety=0x004

#INT_GLOBAL
void HScan(void)
{
   static int hloop,hcount;
   static long hline=0;       // this holds our current scanline
   static int hlinel,hlineh;
   static long charW=console;// this is our pointer into console RAM
   static int charL,charH;
   static int syncpos=0;      // positive sync pulse
   static int syncshort=0;    // two short sync pulses instead of one long one
   static int syncneg=0;
   static int activeline=0;
   #locate hline=0x50
   #locate hlineh=hline+1
   #locate hlinel=hline
   #locate charW=0x52
   #locate charH=charW+1
   #locate charL=charW
// static variables we preserve between interrupts (no need to make them global)
#asm
      // we have not enabled high priority interrupts so we don't need to 
      // pad out interrupt vector with NOPs to get us to 0x018
      
      // we rely on the shadow status,w,&BSR registers to keep track of our
      // state. we don't save them manually

      MOVLB  0x00             // set Bank zero
      BTFSC    syncpos,0      // are we positive sync?
      BCF      LATC,SYNC_BIT  // yes, turn off the sync pin output_low(SYNC_PIN);
      MOVFF    FSR0L,0x0C     // saving the registers takes
      MOVFF    FSR0H,0x07     // 28 instructions
#ifdef DOUBLE40
      MOVFF    FSR1L,0x08     // we only use FSR0 and FSR1 in DOUBLE40 mode
      MOVFF    FSR1H,0x09     
      MOVFF    FSR2L,0x0A
      MOVFF    FSR2H,0x0B
#endif      
#ifdef SINGLE40   
      BTFSC    syncshort,0    // short sync pulse?
      BSF      LATC,SYNC_BIT  // clear the sync pin early
      MOVFF    PCLATH,0x14
      MOVFF    TABLAT,0x15
      BTFSC    syncpos,0      // are we normal sync?
      BSF      LATC,SYNC_BIT  // yes- turn on the sync pin   
#else
      MOVFF    PCLATH,0x14
      BTFSC    syncshort,0    // short sync pulse?
      BSF      LATC,SYNC_BIT  // clear the sync pin early
      MOVFF    TABLAT,0x15
#endif
      MOVFF    TBLPTRL,0x16
      MOVFF    TBLPTRH,0x17
#ifdef SAVESCRATCH
      MOVFF    0x00,0x0E // these scratch registers were used by the old switch statement
      MOVFF    0x01,0x0F // we don't need to save them now.
      MOVFF    0x02,0x10
      MOVFF    0x03,0x11
#endif      
//    MOVFF    PRODL,0x12   // we don't use the multiplier
//    MOVFF    PRODH,0x13
#ifndef SINGLE40
      BTFSC    syncpos,0      // are we normal sync?
      BSF      LATC,SYNC_BIT  // yes- turn on the sync pin
#endif // SINGLE40
EndSync:
      BTFSS    PIE1,TMR2IE    // where should this live? earlier?
      GOTO     SkipInt 
      // no actual interrupt pending? that
                              // should never happen but if it does we 
                              // just go quietly
scanout:      
      BTFSS    activeline,0
      BRA      notactiveline
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

      MOVLW  CONSOLECOLS
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
#ifdef SINGLE40
      MOVLW    20//(CONSOLECOLS)/2    // two per loop in SINGLE40 mode
#else      
      MOVLW    40          // loops per line. we do 2 chars per loop in 80 column mode, 1 per loop in 40 column mode
#endif      
      MOVWF    hloop
      
      MOVFF    charL,FSR0L // load our offset into console character memory
      MOVFF    charH,FSR0H      
#ifdef DOUBLE40
      MOVFF    Dub2H,FSR1H
      MOVFF    DubH,FSR2H
#endif
      MOVLW    0x07
      ANDWF    hlinel,w
      ADDLW    ROMFONT/256        // assumes font starts at 0x2000, add the line offset
      MOVWF    TBLPTRH
      MOVLW    0
      MOVWF    TXREG
      BSF      TXSTA,TXEN_BIT
//      BSF      RCSTA,SPEN_BIT // turn on the shift register
overscan:
      MOVWF   TXREG
#ifdef DOUBLE40
      NOP
      NOP
      NOP
      NOP
      NOP
      NOP
      NOP
      MOVWF    TXREG
      NOP
      NOP
      NOP
      NOP
      NOP
      NOP
      NOP
      MOVWF    TXREG
      NOP
      NOP
      NOP
      NOP
      NOP
      MOVFF    POSTINC0,TBLPTRL
      MOVWF    TXREG
      NOP
//      NOP
pixels:
      TBLRD    *                   // read our font data
      MOVFF    TABLAT,FSR1L
      MOVFF    INDF2,TXREG
      DECF     hloop,F
      NOP                        // we need this NOP

      MOVFF    TABLAT,FSR2L
      MOVFF    POSTINC0,TBLPTRL // move from *FSR0++ to the Table address low byte
      MOVFF    INDF1,TXREG
      BNZ      pixels          // are we done with the line?
endpixels:
      NOP
      NOP
      NOP
      NOP
      MOVFF    INDF2,TXREG
      MOVF     POSTDEC0
      NOP
      NOP
#else
   #ifndef SINGLE40
      NOP
      NOP
      NOP
      NOP
      NOP
      NOP
      NOP
      MOVWF    TXREG
      NOP
   #endif
pixels:
      MOVFF    POSTINC0,TBLPTRL // move from *FSR0++ to the Table address low byte
      TBLRD    *                   // read our font data
//      bsf      LATC,7
//      NOP
      MOVFF    TABLAT,TXREG
      DECF     hloop,F
      NOP                        // we need this NOP
      MOVFF    POSTINC0,TBLPTRL // move from *FSR0++ to the Table address low byte
      TBLRD    *                 // read our font data
//      bcf      LATC,7
      MOVFF    TABLAT,TXREG
      BNZ      pixels
endpixels:
      NOP   // we need these in 80 column mode
      NOP
      NOP
#endif

      MOVFF    FSR0L,charL  // save our character position
      MOVFF    FSR0H,charH

      BCF      TXSTA,TXEN_BIT
//      CLRF     RCSTA       // OK, shut off the shift register now

      BRA      postvideo

//*******************************************************************
// if it's not an active line we still have to generate a back porch
// and perhaps a second sync pulse
//*******************************************************************
notactiveline:

      BTFSS    syncpos,0
      BRA      vsynccode
// do a back porch but do not emit video
topbackporch:
      MOVLW    HBackPorch   // back porch delay
      MOVWF    hcount
topchardelay:
      NOP            // four instructions per half character
      DECFSZ   hcount
      BRA      topchardelay

idletime:
      MOVLW    5        // number of instructions div 3- aligns our sync
      MOVWF    hcount
idleloop:
      DECFSZ   hcount
      BRA      idleloop

#ifdef DOUBLE40
      NOP
      NOP
      NOP
      NOP
#endif
      MOVLW    0
      BSF      TXSTA,TXEN_BIT
//      BSF      RCSTA,SPEN_BIT // turn on the shift register
      MOVWF    TXREG
      NOP
      NOP
      NOP
      NOP
      NOP
      NOP
      NOP
      BCF   TXSTA,TXEN_BIT
//      CLRF  RCSTA
      BRA   postvideo
vsynccode:

halfporch:
      MOVLW    HALFLINE // back porch delay
      MOVWF    hcount
halfdelay:
      NOP            // four instructions per half character
      DECFSZ   hcount
      BRA      halfdelay    

      BTFSS    syncpos,0
      BRA      vsyncold

positivesecond:
      BCF      LATC,SYNC_BIT  // yes, turn off the sync pin output_low(SYNC_PIN);
      MOVLW    VSyncHChars
      MOVWF    hcount
secondsyncpos:
      NOP
      DECFSZ    hcount
      BRA       secondsyncpos
      BSF      LATC,SYNC_BIT  // clear the sync pin early
      BRA      vsyncold


halfsyncstart:
      BSF      LATC,SYNC_BIT
      MOVLW VSyncHChars
      MOVWF hcount
halfsyncdelay:
      NOP            // four instructions per loop == 1/2 character at 80 columns (1 character at 40 columns)
      DECFSZ   hcount
      BRA   halfsyncdelay
      BCF      LATC,SYNC_BIT

vsyncold:

      BTFSS    syncneg,0
      BRA      postvideo

      BSF      LATC,SYNC_BIT
      MOVLW VSyncHChars
      MOVWF hcount
halfsyncnegdelay:
      NOP            // four instructions per loop == 1/2 character at 80 columns (1 character at 40 columns)
      DECFSZ   hcount
      BRA   halfsyncnegdelay
      BCF      LATC,SYNC_BIT

      BTFSS    syncshort,0
      BRA      postvideo
      MOVLW 2
      MOVWF hcount
delay4:
      NOP
      DECFSZ hcount,F
      BRA    delay4
      BSF    LATC,SYNC_BIT

postvideo:
#endasm

   hline++;

   if(hline==FIELDLINES)
   {   
      hline=0;
   }

   // here we set some flags to determine what the next video line will
   // look like. will it have active pixels? will it have negative sync?

   syncpos=1;     // normal sync 
   activeline=0;  // not active video
   syncshort=0;   // full width sync pulse
   syncneg=0;     // end with a negative sync pulse
   
   if(hline<ACTIVE)
   {
      if(blankscreen==0) activeline=1; // the next line is active video
   }
   else if(hlinel==VSYNCLINE+9-256)// we already know it's inactive so we can ignore the high byte
      {

      // the clock is wrong here. we need to add an extra scan line
      // at the beginning of the EVEN fields to avoid losing time.

         fields++;
         ticks++;
         //if(ticks%16==0) console[CONSOLERAM-CONSOLECOLS-2]^=0x80; // blink the cursor
         UpdateClock();
         syncpos=0;
      }
      else
      {
      if(hline==ACTIVE)
      {
         charW=console;// this is 0x100 at the end of each active field we reset the pointer into character memory
      }
      else if(hlinel==VSYNCLINE-256)     syncshort=1;
      else if(hlinel==VSYNCLINE+1-256)   syncshort=1;
      else if(hlinel==VSYNCLINE+2-256)   syncshort=1;
      else if(hlinel==VSYNCLINE+3-256)   { syncpos=0; syncneg=1; }
      else if(hlinel==VSYNCLINE+4-256)   { syncpos=0; syncneg=1; }
      else if(hlinel==VSYNCLINE+5-256)   { syncshort=1; syncneg=1; }
      else if(hlinel==VSYNCLINE+6-256)   syncshort=1;
      else if(hlinel==VSYNCLINE+7-256)   syncshort=1;

   }

   if(hline==ACTIVE)
   {
      if(even)
      {
         even=0;
         hline++; // skip a line on even fields
      }
      else even=1;
   }

   // restore our registers before returning
#asm
      BCF      PIR1,TMR2IF

SkipInt:

#endasm
      KBint();
#asm
      BTFSC  PIR1,TMR2IF
#ifdef SAVESCRATCH // if you use a switch() statement you need this
      MOVFF  0x0E,0x00
      MOVFF  0x0F,0x01
      MOVFF  0x10,0x02
      MOVFF  0x11,0x03
#endif      
      MOVFF  0x0C,FSR0L
      MOVFF  0x07,FSR0H
#ifdef DOUBLE40
      MOVFF  0x08,FSR1L
      MOVFF  0x09,FSR1H
      MOVFF  0x0A,FSR2L
      MOVFF  0x0B,FSR2H
#endif      
//      MOVFF  0x12,PRODL
//      MOVFF  0x13,PRODH
      MOVFF  0x14,PCLATH
      MOVFF  0x15,TABLAT
      MOVFF  0x16,TBLPTRL
      MOVFF  0x17,TBLPTRH
      RETFIE 1             // restore shadow registers

#endasm

}

/**************** end of video generation ***************************/

/************ start of console/ video terminal handling functions***/

void CursorOn()
{
   console[(long)conX+(long)conY*CONSOLECOLS]|=0x80;
}

void CursorOff()
{
   console[(long)conX+(long)conY*CONSOLECOLS]^=0x80;
}

void ClearScreen(void)
{
   long index;
   
   for(index=0;index<CONSOLERAM;index++)
   {
      console[index]=0;
   }
}

void ScrollUp(int CR)
{
   long index;

   if(ConY<CONSOLEROWS-1)
   {
      ConY++;
   }
   else
   {
      for(index=0;index<CONSOLERAM-CONSOLECOLS;index++)
      {
         console[index]=console[index+CONSOLECOLS];
      }
   
     for(index=CONSOLELAST;index<CONSOLELAST+CONSOLECOLS;index++) console[index]=32;
     
   }
     if(CR) conX=0;
}

void ScreenChar(char symbol)
{
   CursorOff();
   switch(symbol)
   {
      case 8: // backspace
         if(conX) conX--;
         break;
      case '\n':// line feed
         ConY++;
         if(ConY==CONSOLEROWS)
         {
            ScrollUp(0);// no carriage return- just a line feed
            ConY--;
         }
         break;
      case '\r': // carriage return
         conX=0;
         break;
      default:
         console[CONSOLECOLS*(long)conY+(long)conX]=symbol;
         conX++;
         if(conX>=CONSOLECOLS)
         {
            ConY++;
            if(conY>CONSOLEROWS)
            {
               conY=CONSOLEROWS;
               ScrollUp(1); 
            }
         }
   }
   CursorOn();
}

void StringWrite(char *dest,char *source) // write a string without writing the terminating NULL
{
  int index=0;
  
  while(source[index]!=0)
  {
   if(dest<CONSOLERAM) *dest++=source[index++];
  }
}

void InitConsoleRAM1(void)
{
   long index;
  
   for(index=0;index<CONSOLERAM;index+=CONSOLECOLS) // fill the screen with a test pattern
   {
                                 //123456789-123456789-12345678-123456789-123456789-123456789-123456789-123456789-
#ifdef DOUBLE40
                              // 123456789-123456xx789-123456789-123456789-
      sprintf(&console[index],"%2ld THIS IS LINE %2ld          30        40",index/CONSOLECOLS,index/CONSOLECOLS);
//      sprintf(safetemp,"%2ld THIS IS LINE %2ld          30        40",index/CONSOLECOLS,index/CONSOLECOLS);
      
#else
   #ifdef SINGLE40
      sprintf(&console[index],"%2ld THIS IS LINE %2ld          30        40",index/CONSOLECOLS,index/CONSOLECOLS); 
   #else
      sprintf(&console[index],"%2ld THIS IS LINE %2ld          30        40        50        60        70        80",index/CONSOLECOLS,index/CONSOLECOLS);
   #endif
#endif      
//      sprintf(&console[index],"%2ld THIS IS LINE %2ld 123456789-123456789-123456789-123456789-123456789-1234567890",index/80-1,index/80-1);

     //this overwrites our 2000 characters by 1: it adds a null terminator past the end of the thing.
   }

   for(index=0;index<256;index++)
   {
      console[index+160]=index;
   }

   sprintf(&console[CONSOLECOLS/2-4],"EQUACK %d",CONSOLECOLS); // centered at the top of the screen
   strcpy(&console[CONSOLECOLS*12],"The quick brown fox jumped over the lazy dogs.");
}

#ifdef DOUBLE40
void Init40Double(void)
{
   long index;
   int dubleft,dubright;
   const unsigned char convert[16]={0x00,0x03,0x0C,0x0F,0x30,0x33,0x3C,0x3F,0xC0,0xC3,0xCC,0xCF,0xF0,0xF3,0xFC,0xFF};


   for(index=0;index<256;index++)
   {
      dubleft=convert[index/16];
      Dubs[index]=dubleft;
   }

   for(index=0;index<256;index++)
   {
      dubright=convert[index%16];
      Dubs2[index]=dubright;
   }
}
#endif //DOUBLE40


void InitConsoleRAM2(void)
{
   long index,line;
   long offset;

   for(line=1;line<CONSOLEROWS;line++)
   {
      for(index=0;index<CONSOLECOLS;index+=4)
      {
         offset=index+(line*CONSOLECOLS);
//         console[offset]='@'+line;
         console[offset]='A';
         console[offset+1]='B';
         console[offset+2]='C';
         console[offset+3]='D';
         
      }
   }
   
   
   strcpy(&console[CONSOLECOLS*10],"This is a test of the emergency broadcasting system.");
   strcpy(&console[CONSOLECOLS*14],"AND NOW FOR SOMETHING COMPLETELY DIFFERENT.");
   
}

void InvertScreen(void)
{
   long index;
   
   for(index=CONSOLECOLS;index<CONSOLERAM;index++)
   {
      console[index]^=0x80;
   }
}

void EraseConsoleRAM(void)
{
   long index;
   
   for(index=0;index<CONSOLERAM;index++)
   {
      console[index]=0;
   }
}

void InitSystem(void)
{
 EraseConsoleRAM();
 
 InitConsoleRAM1();

#ifdef DOUBLE40
   Init40Double();
#endif

   output_float(PIXEL_PIN); // this is our video output
   output_float(CLOCK_PIN); // disable the pixel clock output for RFI

#ifndef KCPU
   RPOR17=0; // unmap the EUSART pins
   RPOR18=0;
#endif

   SLRCON=0;//PORTC_SLEW;      // decrease port C slew rate for CRTs
   BAUDCON=0;
   SPBRG=0;       // we always set the baud rate to 0, one pixel per Tcy.
   SPBRGH=0;      // in 40 column mode we double our pixels
#asm
   MOVLW SPEN
   MOVWF RCSTA
   NOP
   NOP
#endasm

  
   RCSTA=SPEN;
   TXSTA= CSRC | SYNC ; // master synchronous transmit mode
   // | TXEN
   
   output_high(SYNC_PIN); // sync pin goes LOW to drop below IRE 0.3 volts

   setup_timer_2(T2_DIV_BY_4,HLINEPERIOD,1);

/*
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
   setup_ccp2(CCP_COMPARE_RESET_TIMER ); // note that we do not actually enable the interrupt for this.

#define BAUDDIV   1491

   CCPR2H=BAUDDIV/256;
   CCPR2L=BAUDDIV%256;
*/ 
   enable_interrupts(INT_TIMER2);   // horizontal sync interrupt
   enable_interrupts(GLOBAL);
}


void delay_fields(long interval) // delay a number of vertical blank intervals, 16.7 milliseconds each.
{
   int32 wait;
   
   // If the system runs for 2.25 years this function
   // could hang if called at exactly the wrong time.
   //
   // This function it has an inherent error because the first field will be a partial field
   // which could last between 1 and 261 scan lines.
   //
   wait=ticks+interval;
   for(;;)
   {
      if(ticks>=wait) return;
   }
}


void TimeDisplay(void)
{
   int cents;

   for(;;)
   {
      cents=fields/6;

      sprintf(&console[CONSOLELAST],"Elapsed time %d:%02d:%02d.%d",hours,minutes,seconds,cents);
      delay_fields(6); //
   }

}

void ScrollOff(void)
{
   int index;
   
   for(index=0;index<25;index++)
   {
      ScrollUp(1);

#ifdef DOUBLE40
      sprintf(&console[CONSOLELAST],"Elapsed time %d:%02d:%02d.%02d 56789-123456789-",hours,minutes,seconds,fields);
#else
   #ifdef SINGLE40
      sprintf(&console[CONSOLELAST],"Elapsed time %d:%02d:%02d.%02d 56789-123456789-",hours,minutes,seconds,fields);
   #else
      sprintf(&console[CONSOLELAST],"Elapsed time %d:%02d:%02d.%02d 56789-123456789-123456789-12345789-123456789-123456789-",hours,minutes,seconds,fields);
   #endif
#endif
      delay_fields(30); //
   }

}


void Beep(void)
{
   long index;
   
   for(index=0;index<250;index++)
   {
      output_low(SOUND_PIN);
      delay_us(500);
      output_high(SOUND_PIN);
      delay_us(500);
   }
}

//---------------Main() entry point--------------------------------
void main( void )
{
   char *right;
   right=copyright; 

#ifdef USEPLL
   osctune|=PLLEN; // crank it up to 4X PLL
#endif
   output_high(LED_PIN);

   delay_ms(50);
   output_low(LED_PIN);
   delay_ms(50);
   output_high(LED_PIN);
   delay_ms(50);
   output_low(LED_PIN);
   delay_ms(50);
   output_high(LED_PIN);

// Beep();

   InitSystem();
   InitKeyboard();
//   TimeDisplay();
//   ScrollUp(1);
   TypeWriter();
//   ScrollOff();

   for(;;) ; // just wait
}

