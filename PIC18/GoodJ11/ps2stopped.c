//
// PS2 keyboard functionality for 80 column display project
//
//

#define  KBCLK_PIN         PIN_B2
#define  KBDAT_PIN         PIN_B1
#define  SPICLK_PIN        PIN_C3
#define  SPIDAT_PIN        PIN_C4

#define NEXTKEYDELAY 4    // minimum time in horizontal scan lines for keyboard scan code
                           // if you make this too long the arrow keys won't release (E0...7X) instead of (E0.F0.7X)


//#define  SOUND_PIN         PIN_C1   // for control-G "BEL"
//#define  LED_PIN           PIN_A5


// compare mode (PWM)

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
#define  SSP_BF         0x01  // buffer full


//extern int conY,conX;

extern void ScrollUp(int CR);
extern void ClearScreen(void);
extern void CursorOn(void);
extern void CursorOff(void);
extern void Beep(void);
//
// these scan codes are pre-mogrified
// for your convenience
//
// please note two limitations of our 7-bit PS/2 interface:
// 1. we can't always register the numeric keypad zero on press (we see it on release).
// 2. we can't tell the difference between F5 and F7.
// 3. we may have trouble detecting the INSERT key
//
// one solution is to switch to scan set 1 (the default is 2) but not all keyboards support
// this because all Microsoft operating systems use set 2. Scan set 1 is the 'AT' command set
// scan set 2 is the PS/2 scan set. There is also a scan set 3 but Microsoft doesn't even
// mention it in their hardware documents.
//
//

const char scancodes[]={
0x00,0x00,0x00,0,          //0x00
0x99,0xA9,0xB9,0xC9,       //0x01 F9
0x00,0x00,0x00,0,          //0x02
0x95,0xA5,0xB5,0xC5,       //0x03 F5 -- F7 is 0x83 which is an alias for F5. If only we had the high bit we could tell them apart.
0x93,'3' ,0x80,0,          //0x04 F3
0x91,0xA1,0xB1,0xC1,       //0x05 F1
0x92,0xA2,0xB2,0xC2,       //0x06 F2
0x9C,0xAC,0xBC,0xCC,       //0x07 F12
0x00,0x00,0x00,0,          //0x08
0x9A,0xAA,0xBA,0xCA,       //0x09 F10
0x98,0xA8,0xB8,0xC8,       //0x0A F8
0x96,0xA6,0xB6,0xC6,       //0x0B F6
0x94,0xA4,0xB4,0xC4,       //0x0C F4
0x00,0x00,0x09,0,          //0x0D
'`' ,'~' ,0x00,0,          //0x0E
0x00,0x00,0x00,0,          //0x0F
0x00,0x00,0x00,0,          //0x10 *MM Web Search
0x00,0x00,0x00,0,          //0x11
0x00,0x00,0x00,0,          //0x12 *PrintScreen first char
0x00,0x00,0x00,0,          //0x13
0x00,0x00,0x00,0,          //0x14 **E1 Pause Break
'q' ,'Q' ,0x11,0,          //0x15 *MM Previous Track
'1' ,'!' ,0x00,0,          //0x16
0x00,0x00,0x00,0,          //0x17
0x00,0x00,0x00,0,          //0x18 *MM Web Favorites
0x00,0x00,0x00,0,          //0x19
'z' ,'Z' ,0x1a,0,          //0x1A
's' ,'S' ,0x13,0,          //0x1B
'a' ,'A' ,0x01,0,          //0x1C
'w' ,'W' ,0x17,0,          //0x1D
'2' ,'@' ,0x00,0,          //0x1E
0x00,0x00,0x00,0,          //0x1F
0x00,0x00,0x00,0,          //0x20 *MM Web Refresh
'c' ,'C' ,0x03,0,          //0x21 *MM VolumeDown
'x' ,'X' ,0x18,0,          //0x22
'd' ,'D' ,0x04,0,          //0x23 *MM Mute
'e' ,'E' ,0x05,0,          //0x24
'4' ,'$' ,0x00,0,          //0x25
'3' ,'#' ,0x00,0,          //0x26
0x00,0x00,0x00,0,          //0x27
0x00,0x00,0x00,0,          //0x28 *MM Web Stop
' ' ,' ' ,0x00,0,          //0x29 SPACE
'v' ,'V' ,0x16,0,          //0x2A
'f' ,'F' ,0x06,0,          //0x2B *MM Calculator
't' ,'T' ,0x14,0,          //0x2C
'r' ,'R' ,0x12,0,          //0x2D
'5' ,'%' ,0x00,0,          //0x2E
0x00,0x00,0x00,0,          //0x2F
0x00,0x00,0x00,0,          //0x30 *MM Web Forward
'n' ,'N' ,0x0e,0,          //0x31
'b' ,'B' ,0x02,0,          //0x32 *MM Volume Up
'h' ,'H' ,0x08,0,          //0x33
'g' ,'G' ,0x07,0,          //0x34 *MM Play/Pause
'y' ,'Y' ,0x19,0,          //0x35
'6' ,'^' ,0x00,0,          //0x36
0x00,0x00,0x00,0,          //0x37 *ACPI Power
0x00,0x00,0x00,0,          //0x38 *MM Web Back
0x00,0x00,0x00,0,          //0x39
'm' ,'M' ,0x0d,0,          //0x3A *MM Web Home
'j' ,'J' ,0x0a,0,          //0x3B *MM Stop
'u' ,'U' ,0x15,0,          //0x3C
'7' ,'&' ,0x00,0,          //0x3D
'8' ,'*' ,0x00,0,          //0x3E
0x00,0x00,0x00,0,          //0x3F *ACPI Sleep
0x00,0x00,0x00,0,          //0x40 *MM My Computer
',' ,'<' ,0x00,0,          //0x41
'k' ,'K' ,0x0b,0,          //0x42
'i' ,'I' ,0x09,0,          //0x43
'o' ,'O' ,0x0f,0,          //0x44
'0' ,')' ,0x00,0,          //0x45
'9' ,'(' ,0x00,0,          //0x46
0x00,0x00,0x00,0,          //0x47
0x00,0x00,0x00,0,          //0x48 **MM EMail
'.' ,'>' ,0x00,0,          //0x49
'/' ,0x00,0x80,0,          //0x4A *KEYPAD SLASH
'l' ,'L' ,0x0c,0,          //0x4B
';' ,':' ,0x00,0,          //0x4C
'p' ,'P' ,0x10,0,          //0x4D *MM Next Track
'-' ,'_' ,0x00,0,          //0x4E
0x00,0x00,0x00,0,          //0x4F
0x00,0x00,0x00,0,          //0x50 **MM Media Select
0x00,0x00,0x00,0,          //0x51
'\'','"' ,0x00,0,          //0x52
0x00,0x00,0x00,0,          //0x53
'[' ,'{' ,0x00,0,          //0x54
'=' ,'+' ,0x00,0,          //0x55
0x00,0x00,0x00,0,          //0x56
0x00,0x00,0x00,0,          //0x57
0x00,0x00,0x00,0,          //0x58
0x00,0x00,0x00,0,          //0x59
0x0D,0x0D,0x0D,0,          //0x5A ENTER *KEYPAD ENTER control-M
']' ,'}' ,0x00,0,          //0x5B
0x00,0x00,0x00,0,          //0x5C
'\\','|' ,0x00,0,          //0x5D
0x00,0x00,0x00,0,          //0x5E *ACPI Wake
0x00,0x00,0x00,0,          //0x5F
0x00,0x00,0x00,0,          //0x60 EXTENDED SCAN CODE
0x00,0x00,0x00,0,          //0x61
0x00,0x00,0x00,0,          //0x62
0x00,0x00,0x00,0,          //0x63
0x00,0x00,0x00,0,          //0x64
0x00,0x00,0x00,0,          //0x65
0x08,0x08,0x08,0,          //0x66 BACKSPACE
0x00,0x00,0x00,0,          //0x67
0x00,0x00,0x00,0,          //0x68
'1' ,0x85,0x00,0x85,       //0x69 Keypad 1 *END alt-ctrl-E
0x00,0x00,0x00,0,          //0x6A
'4' ,0x84,0x00,0x84,       //0x6B Keypad 4 *LEFT ARROW alt-ctrl-D
'7' ,0x86,0x00,0x86,       //0x6C Keypad 7 *HOME alt-ctrl-F
0x00,0x00,0x00,0,          //0x6D
0x00,0x00,0x00,0,          //0x6E
0x00,0x00,0x00,0,          //0x6F
'0' ,0x87,0x00,0x87,       //0x70 Keypad 0 *INSERT alias KEYUP alt-ctrl-G
'.' ,0x88,0x00,0x88,       //0x71 Keypad . *DELETE alt-ctrl-H
'2' ,0x82,0x00,0x82,       //0x72 Keypad 2 *DOWN ARROW alt-ctrl-B
'5' ,'5' ,0x00,0,          //0x73 Keypad 5
'6' ,0x83,0x00,0x83,       //0x74 Keypad 6 *RIGHT ARROW alt-ctrl-C
'8' ,0x81,0x00,0x81,       //0x75 Keypad 8 *UP ARROW alt-ctrl-A
0x1b,0x1b,0x1b,0x1b,       //0x76 ESCAPE
0x00,0x00,0x00,0,          //0x77 NUMLOCK, **E1 Pause Break cont'd
0x9B,0xAB,0xBB,0xCB,       //0x78 F11
'+' ,'+' ,0x00,0,          //0x79 Keypad + 
'3' ,0x8A,0x00,0x8A,       //0x7A Keypad 3 *PgDn alt-ctrl-I
'-' ,'-' ,0x00,0,          //0x7B Keypad -
'*' ,'*' ,0x00,0,          //0x7C Keypad *PrintScreen 2nd char
'9' ,0x8B,0x00,0x8B,       //0x7D Keypad 9 *PgUp alt-ctrl-J
0x00,0x00,0x00,0,          //0x7E ScrollLock, alias for keyboard error 0xFE
0x00,0x00,0x00,0           // 0x7F
};


//
// byte codes:
// 0x8x - arrow keys
// 0x81 UP
// 0x82 DOWN
// 0x83 RIGHT
// 0x84 LEFT
// 0x85 END
// 0x86 HOME
// 0x87 INSERT
// 0x88 DEL
// 0x8A PgDn
// 0x8B PgUp
//
// 0x9x - function keys (0x91=F1)
// 0xAx- shifted function keys
// 0xBx- controlled function keys
// 0xCx- extended function keys (ctrl+shift?)


#define NUMLOCK      0x77
#define SCROLLLOCK  0x7E
#define KEYUP        0x70
#define EXTENDED     0x60
#define LSHIFT       0x12
#define RSHIFT       0x59
#define LCTRL        0x14
#define RCTRL        0x14  // 0xE0
#define LALT         0x11
#define RALT         0x11  // 0xE0
#define CAPSLOCK     0x58
#define LWIN         0x1F  // 0xE0
#define RWIN         0x27  // 0xE0
#define MENU         0x2F  // 0x2F


// keyboard commands
#define KB_RESET  0xFF
#define KB_RESEND 0xFE
#define KB_ACK    0xFA
#define KB_MATIC  0xF3  // set typematic delay. bits 0-4= rate (0x0 fastest, 0x1F slowest) , bits 5-6=delay (0x20 shortest, 0x60 longest)
#define KB_SET    0xF0  // set the scan code table to use (0x01, 0x02, 0x03)
#define KB_ECHO   0xEE  // echo back an 0xEE
#define KB_LEDSET 0xED  // second byte is mapped 0x04-caps, 0x02-num, 0x01-scroll
// key board response codes
//
//

char FlipBits(char inbits)
{
   int obits=0;
   
   if(inbits&0x01) obits|=0x80;
   if(inbits&0x02) obits|=0x40;
   if(inbits&0x04) obits|=0x20;
   if(inbits&0x08) obits|=0x10;
   if(inbits&0x10) obits|=0x08;
   if(inbits&0x20) obits|=0x04;
   if(inbits&0x40) obits|=0x02;
   if(inbits&0x80) obits|=0x01;

   return obits>>1;

}

struct keyboard
{
   int state;  // true if new code
   int code;   // current code
   int last;   // previous code
   int second; // two codes ago
   int tick;   // timeout counter
};

struct keyboard ps2={0,0,0,0,0};


// one of the nice things about the PS/2 keyboard protocol is that we can (almost) safely
// ditch bit 7. There are no scan codes above 0x7F. This means we can capture the 
// start bit and data bits 0-6, shut off the MSSP and lose the bit 7, parity, and stop bits.
//
// the trick is to keep it turned off long enough to skip those three bits but
// not long enough to miss the next character. there does not seem to be a spec
// for the interval between characters but I tried four different keyboards on
// the oscilloscope and they all had long intervals between bytes.
//
// The only problem is the zero key on the numeric keypad. it has the same
// scan code as the 'release' code when the high bit is stripped off.
// the solution? when we see 3 0x70 codes in a row we know it is the numeric
// keypad zero. autorepeat works correctly for this key but if you press it
// once you will not see the key register until you RELEASE the key. It beats
// adding another chip.
//
// high bit codes that can occur:
//
// 0xF0 (normal key release) 0x70 alias (numeric keypad zero)
// 0xE0 (extended key) 0x60 alias (unassigned key)
// 0xE1 (pause key) 0x61 alias (unassigned key)
//
// ACPI and windows multimedia keys?

#INLINE
void KBInt(void) // service the MSSP interrupt
{
   ps2.tick++;
   if(SSPCON1&SSPEN) // are we enabled?
   {
      if(PIR1&SSPIF)
      {
         PIR1^=SSPIF;
         ps2.code=SSPBUF;   // save this code
         SSPCON1=0;     // turn off the SSP
         ps2.second=ps2.last; // save the last two codes 
         ps2.last=ps2.code;
         ps2.state=1;
         ps2.tick=0;
      }
   }
   else // we're not enabled
   {
      if(ps2.tick>NEXTKEYDELAY)///
      {
         SSPCON1=CKP| SSP_SLAVE_NOSS | SSPEN; // wait a decent interval then turn the SSP back on again
         ps2.tick=0;
      }
   }
}

void PrintLine(char *line)
{
   CursorOff();
   conY++;
   if(conY>25)
   {
      ScrollUp(1);
      cony=25;
   }
   strcpy(&console[CONSOLELAST],line);
//   conX=strlen(line);
   CursorOn();
}

void PrintCharRaw(char symbol)
{
   console[(long)conX+(long)conY*CONSOLECOLS]=symbol;
   conX++;
   if(conX>=CONSOLECOLS)
   {
      ScrollUp(1);
   }
}

void PrintChar(char symbol)
{
   CursorOff();

      switch(symbol)
      {
         case 0:
            break;
         case 0x81: // up
            if(conY) conY--;
            break;
         case 0x82: // down 
            if(conY<CONSOLEROWS-1) conY++;
            break;
         case 0x83: // forward
            if(conX<CONSOLECOLS-1) conX++;
            break;
         case 0x84: // backward
            if(conX) conX--;
            break;
         case 12: // Form Feed, control-L
            ClearScreen();
            ConX=0;
            ConY=0;
            break;
         case 9: // Tab, control-I
            conX=(conX/8+1)*8;
            if(conX>=CONSOLECOLS) conX=CONSOLECOLS-1;
            break;
         case 13: // Carriage Return, control-M
            ScrollUp(1);
            break;
         case 7:  // BEL
            Beep();
            break;    
         case 8: // backspace, control-H
            if(conX) // can't backspace to the previous line.
            {
               conX--;
            }
            break;
         default:
            PrintCharRaw(symbol);
            break;
      };

   CursorOn();
}

void PrintString(char *pstring)
{
   int index=0;
   
   while(pstring[index])
   {
      PrintChar(pstring[index++]);
   }
}

void PrintStringRaw(char *pstring)
{
   int index=0;
   
   while(pstring[index])
   {
      PrintCharRaw(pstring[index++]);
   }
}

#INLINE
int WaitLow(void) // needs a timeout
{
   long timeout=0;
   while(input(SPICLK_PIN)) 
   {
      timeout++;
      if(timeout==5000) return 0;
   }
   return 1;
}

#INLINE
int WaitHigh(void) //needs a timeout
{
   long timeout=0;

   while(input(SPICLK_PIN)==0)
   {
      timeout++;
      if(timeout==5000) return 0;
   }

   return 1;
}


void PS2WriteByte(int symbol)
{
   int bit=0;
   int parity=0;
  
   input(SPICLK_PIN);
   WaitLow();
   WaitHigh();
   while(bit<8)
   {
      if(symbol&1)
      {
         output_high(SPIDAT_PIN);
         parity^=1;
      }
      else output_low(SPIDAT_PIN);
      symbol>>=1;
      bit++;
      WaitLow();
      WaitHigh();
   }

   if(parity) output_high(SPIDAT_PIN);
   else output_low(SPIDAT_PIN);
   WaitLow();
   WaitHigh();

   output_high(SPIDAT_PIN); // stop bit
   WaitLow();
   WaitHigh();

}

#define KEYCLK SPICLK_PIN
#define KEYDATA   SPIDAT_PIN

void sendByte(byte b)
{
    byte a=0;
    byte p = 0;
    boolean t = 0;

  //  return;         // THIS IS NOT FULLY WORKING ON ALL KEYBOARDS
                    // SO IT IS COMMENTED OUT
                    // Some kind of timing issue or something. Whatever, they're just LEDs

    disable_interrupts(GLOBAL);
    output_low(KEYCLK);
    delay_us(80);
  //  input(KEYCLK);
    output_low(KEYDATA);
    output_high(KEYCLK);
    input(KEYCLK);

    for(a=0; a<8; a++)
    {
        while(input(KEYCLK) == 1); //wait for 0
        t = bit_test(b,a);
        output_bit(KEYDATA, t);
        if(t)
            p++;
        while(input(KEYCLK) == 0); //wait for 1
    }

    while(input(KEYCLK) == 1); //wait for 0
    t = bit_test(p,0);
        t = ~t;
    while(input(KEYCLK) == 0); //wait for 1
     output_bit(KEYDATA, t);   // input(KEYDATA);
    while(input(KEYCLK) == 1); //wait for 0
  ///  while(input(KEYCLK) == 0); //wait for 1
     output_float(KEYDATA);
    enable_interrupts(GLOBAL);
} 


void PS2Write(int command, int argument)
{

   sendByte(command);
   sendByte(argument);


   return;
   disable_interrupts(GLOBAL);
    
   SSPCON1=0;     // turn off the SSP
   output_drive(SPICLK_PIN);
   output_drive(SPIDAT_PIN);

   PS2WriteByte(command);
//   delay_us(100);
   PS2WriteByte(argument);

   output_float(SPIDAT_PIN);
   output_float(SPICLK_PIN);
   SSPCON1=CKP| SSP_SLAVE_NOSS | SSPEN; // wait a decent interval then turn the SSP back on again
   
   enable_interrupts(GLOBAL);
   

}


#INLINE
void SetPS2LED(int capslocked,int numlocked,int scrolllocked)
{
   int mask=0;
   
   if(capslocked) mask|=0x04;
   if(numlocked) mask|=0x02;
   if(scrolllocked) mask|=0x01;
 
  PS2Write(0xED,mask); // LED ON
  
}




char KeyRead(int mode)
{
   static unsigned int thiskey;
   static unsigned int release;
   static unsigned int prelease;
   static int shifted;
   static int controlled;
   static int capslocked;
   static int numlocked;
   static int scrolllocked;
//   static int windozed; // windows command key
   static int keydebug;

   char shing[8];
   char out;
   long scandex;
   static char escapecode;
  
   if(escapecode)
   {
      if(mode) PrintChar(escapecode);
      return escapecode;
   }
  
      if(ps2.state)// yes, there is a new scancode waiting
      {
      
         prelease=release; // save previous key
         release=thiskey;// save last key
         thiskey=FlipBits(ps2.code); // get the scancode
         ps2.state=0; // clear the flag

         scandex=(long)thiskey*4;

         if(release==EXTENDED) out=scancodes[scandex+3];
         else if(controlled) out=scancodes[scandex+2];
         else if(shifted)
         {
            if(capslocked) out=scancodes[scandex];
            else out=scancodes[scandex+1];
         }
         else // not shifted, controlled, or extended
         {
            if(capslocked) out=scancodes[scandex+1];
            else out=scancodes[scandex];
         }
         
         if(out==27) // ESCAPE key toggles between debug and typewriter mode
         {
            if(release!=KEYUP)
            {
               if(keydebug) keydebug=0;
               else keydebug=1;
            }
            out=0; // lose the code
         }
         
         if(keydebug)
         {
            sprintf(shing,"%02X-%02x",thiskey,out);
            PrintString(shing);
            PrintChar(13);
            if(out==13) PrintChar(13);
         }
         else
         {
            if(release==KEYUP) // this was a release
            {
               if(thiskey==LSHIFT)       shifted^=1;
               else if(thiskey==RSHIFT)  shifted^=2;
               else if(thiskey==LCTRL)  controlled=0;
               else if((release==KEYUP) && (prelease==KEYUP) && (thiskey==KEYUP))// numeric keypad zero
               {
                  PrintChar('0');
                  release=0; // OK this is kind of flaky if you hit the key over and over but it works OK for single presses
                  prelease=0; // Could we be smarter about sniffing it out? you might see a key release at any time (shift, ctrl) so probably not
               }
            }
            else if (thiskey!=KEYUP) // this is an actual scan code
            {
               switch(thiskey)
               {
                  case 0:
                        Beep();
                        break;
                  case CAPSLOCK:
                        if(capslocked) capslocked=0;
                        else capslocked=1;
                        SetPS2LED(capslocked,numlocked,scrolllocked);
                        break;
                  case NUMLOCK:
                        if(numlocked) numlocked=0;
                        else numlocked=1;
                        SetPS2LED(capslocked,numlocked,scrolllocked);
                        break;
                  case SCROLLLOCK:                     
                        if(scrolllocked) scrolllocked=0;
                        else scrolllocked=1;
                        SetPS2LED(capslocked,numlocked,scrolllocked);
                        break;
                  case LSHIFT:
                        shifted|=1;
                        break;
                  case RSHIFT:
                        shifted|=2;
                        break;
                  case LCTRL:
                        controlled=1;
                        break;
                  case 0x0E:     // tilde key prints out cursor position
                        sprintf(shing,"%d,%d ",conX,conY);
                        PrintString(shing);
                        break;                  
                  default:
                     if(mode) PrintChar(out);                  
                  };
               }
            }
         }
         else return 0;
   
   return out;
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


void InitKeyboard(void)
{
// EXT_INT_EDGE(2, H_TO_L); // set interrupt for falling edge

   output_float(SPICLK_PIN);
   output_float(SPIDAT_PIN);
   
#ifdef KCPU
// we have real pullups on the J11 board.
   output_float(KBCLK_PIN);  // tied to KBCLK_PIN w/pullup
   output_float(KBDAT_PIN); // tied to KBDAT_PIN w/pullup
   port_b_pullups(0x06); // B1 and B2 pulled up
#endif

   SSPSTAT=0; // clear the two MSBs
   SSPCON1=CKP| SSP_SLAVE_NOSS;
   SSPCON1|=SSPEN;

//   enable_interrupts(INT_EXT2);     // PS/2 keyboard clock line interrupt
   
   PS2Write(0xFF,0xFF); //reset the keyboard
  
// BlinkLED(0); 

}

  // SSPSTAT= SMP | CKE;
   /////   SSPCON1= SSPEN; // this turns on the hose


void Backspace(void)
{
   if(conX)
   {
      console[CONSOLELAST+(long)conX]=0;
      conX--;
   }
}

void TypeWriter(void)
{

   CursorOn();

   for(;;)
   {
      KeyRead(1);
   }

}
