//
// PS2 keyboard functionality for 80 column display project
//
//

#define  KBCLK_PIN         PIN_B2
#define  KBDAT_PIN         PIN_B1
//#define  SOUND_PIN         PIN_C1   // for control-G "BEL"
//#define  LED_PIN           PIN_A5

#define  CAPS     0x58
#define  LSHIFT   0x12
#define  LCTRL    0x14


// compare mode (PWM)
#byte    CCPR1H=  0xFBF
#byte    CCPR1L=  0xFBE

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

#byte    SSPSTAT=  0xFC7
#byte    SSPADD =  0xFC8
#byte    SSPBUF =  0xFC9
#byte    SSPCON1 = 0xFC6
#byte    SSPCON2=  0xFC5
// interrupt control
#byte    INTCON2= 0xFF1
#define  INTEDG2   0x10

#byte    POSTINC1=0xFE6
#byte    FSR1H=   0xFE2
#byte    FSR1L=   0xFE1

//extern int ConY,ConX;

extern void ScrollUp(int CR);
extern void CursorOn(void);
extern void CursorOff(void);

struct keyboard
{
   int bits;
   int miss;
   int state;
   int code;
   int last;
   int kbyte;
   int tick;
   int parity;
};

struct keyboard ps2={0,0,0,0,0,0,0,0};

const char scancodes[]={
' ',' ',0x00,   // 0x00
'F','9',0x80,   // 0x01
' ',' ',0x00,   // 0x02
'F','5',0x80,   // 0x03
'F','3',0x80,   // 0x04
'F','1',0x80,   // 0x05
'F','2',0x80,   // 0x06
'F','C',0x80,   // 0x07
' ',' ',0x00,   // 0x08
'F','A',0x80,   // 0x09
'F','8',0x80,   // 0x0A
'F','6',0x80,   // 0x0B
'F','4',0x80,   // 0x0C
' ',' ',0x09,   // 0x0D
'`','~',0x00,   // 0x0E
' ',' ',0x00,   // 0x0F
' ',' ',0x00,   // 0x10
' ',' ',0x00,   // 0x11
' ',' ',0x00,   // 0x12
' ',' ',0x00,   // 0x13
' ',' ',0x00,   // 0x14
'q','Q',0x11,   // 0x15
'1','!',0x00,   // 0x16
' ',' ',0x00,   // 0x17
' ',' ',0x00,   // 0x18
' ',' ',0x00,   // 0x19
'z','Z',0x1a,   // 0x1A
's','S',0x13,   // 0x1B
'a','A',0x01,   // 0x1C
'w','W',0x17,   // 0x1D
'2','@',0x00,   // 0x1E
' ',' ',0x00,   // 0x1F
' ',' ',0x00,   // 0x20
'c','C',0x03,   // 0x21
'x','X',0x18,   // 0x22
'd','D',0x04,   // 0x23
'e','E',0x05,   // 0x24
'4','$',0x00,   // 0x25
'3','#',0x00,   // 0x26
' ',' ',0x00,   // 0x27
' ',' ',0x00,   // 0x28
' ',' ',0x00,   // 0x29
'v','V',0x16,   // 0x2A
'f','F',0x06,   // 0x2B
't','T',0x14,   // 0x2C
'r','R',0x12,   // 0x2D
'5','%',0x00,   // 0x2E
' ',' ',0x00,   // 0x2F
' ',' ',0x00,   // 0x30
'n','N',0x0e,   // 0x31
'b','B',0x02,   // 0x32
'h','H',0x08,   // 0x33
'g','G',0x07,   // 0x34
'y','Y',0x19,   // 0x35
'6','^',0x00,   // 0x36
' ',' ',0x00,   // 0x37
' ',' ',0x00,   // 0x38
' ',' ',0x00,   // 0x39
'm','M',0x0d,   // 0x3A
'j','J',0x0a,   // 0x3B
'u','U',0x15,   // 0x3C
'7','&',0x00,   // 0x3D
'8','*',0x00,   // 0x3E
' ',' ',0x00,   // 0x3F
' ',' ',0x00,   // 0x40
',','<',0x00,   // 0x41
'k','K',0x0b,   // 0x42
'i','I',0x09,   // 0x43
'o','O',0x0f,   // 0x44
'0',')',0x00,   // 0x45
'9','(',0x00,   // 0x46
' ',' ',0x00,   // 0x47
' ',' ',0x00,   // 0x48
'.','>',0x00,   // 0x49
'/',' ',0x80,   // 0x4A
'l','L',0x0c,   // 0x4B
';',':',0x00,   // 0x4C
'p','P',0x10,   // 0x4D
'-','_',0x00,   // 0x4E
' ',' ',0x00,   // 0x4F
' ',' ',0x00,   // 0x50
' ',' ',0x00,   // 0x51
'\'','"',0x00,  // 0x52
' ',' ',0x00,   // 0x53
'[','{',0x00,   // 0x54
'=','+',0x00,   // 0x55
' ',' ',0x00,   // 0x56
' ',' ',0x00,   // 0x57
' ',' ',0x00,   // 0x58
' ',' ',0x00,   // 0x59
' ',' ',0x80,   // 0x5A
']','}',0x00,   // 0x5B
' ',' ',0x00,   // 0x5C
'\\','|',0x00,  // 0x5D
' ',' ',0x00,   // 0x5E
' ',' ',0x00,   // 0x5F
' ',' ',0x00,   // 0x60
' ',' ',0x00,   // 0x61
' ',' ',0x00,   // 0x62
' ',' ',0x00,   // 0x63
' ',' ',0x00,   // 0x64
' ',' ',0x00,   // 0x65
' ',' ',0x08,   // 0x66
' ',' ',0x00,   // 0x67
' ',' ',0x00,   // 0x68
'1',' ',0x00,   // 0x69
' ',' ',0x00,   // 0x6A
'4','F',0x00,   // 0x6B
'7',' ',0x00,   // 0x6C
' ',' ',0x00,   // 0x6D
' ',' ',0x00,   // 0x6E
' ',' ',0x00,   // 0x6F
'0',' ',0x00,   // 0x70
'.',' ',0x00,   // 0x71
'2',' ',0x00,   // 0x72
'5','7',0x00,   // 0x73
'6','Ç',0x00,   // 0x74
'8',' ',0x00,   // 0x75
' ',' ',0x1b,   // 0x76
' ',' ',0x00,   // 0x77
'F','B',0x80,   // 0x78
'+',' ',0x00,   // 0x79
'3',' ',0x00,   // 0x7A
'-',' ',0x00,   // 0x7B
'*',' ',0x00,   // 0x7C
'9',' ',0x00,   // 0x7D
' ',' ',0x00,   // 0x7E
' ',' ',0x00    // 0x7F
};


//#INT_EXT2 // we are a lower priority than the video hsync interrupt
void KBInt(void) // function called on falling edge of PS/2 keyboard clock
{
   int hold;

//   ps2.tick++;
   if(++ps2.tick>4)
   {  
         ps2.bits=0;
         ps2.kbyte=0;
   }

   if(INTCON3&INT2MASK)
   {
      INTCON3^=INT2MASK; // clear the interrupt flag  
      hold=input(KBDAT_PIN);  // grab the data

      ps2.tick=0;
      
      // if we go more than 4 scan lines without a clock pulse
      // we do a timeout and restart the bit counter.
      
      if(ps2.bits==10)
      {
//         if(ps2.state) ps2.miss++;
         ps2.last=ps2.code;
         ps2.code=ps2.kbyte;
         ps2.kbyte=0;
         ps2.state=1;
         ps2.bits=0;
      }
      else if(ps2.bits==9)
      {
         ps2.parity=hold;
      // drop the parity bit
      }
      else if(ps2.bits)
      {
         ps2.kbyte>>=1;
         if(hold) ps2.kbyte|=0x80;
      }

      ps2.bits++;

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

void PrintChar(char symbol)
{
   console[CONSOLELAST+conX]=symbol;
   conX++;
   if(conX>=CONSOLECOLS)
   {
      conX=0;
      ScrollUp(1);
   }
}

void PrintString(char *pstring)
{
   int index=0;
   
   while(pstring[index])
   {
      PrintChar(pstring[index++]);
   }
}

char KeyRead(void)
{
   static unsigned int thiskey;
   static unsigned int release;
   char shing[5];
   char out;
   long scandex;
   
   for(;;)
   {
      if(ps2.state)
      {
         thiskey=ps2.code;
         release=ps2.last;
         ps2.state=0;

//            if(scancodes[thiskey*3+2]==0) // not a control coode
         scandex=thiskey*3;
         out=scancodes[scandex];
         sprintf(shing,"%02X%c ",thiskey,out);

         if((release!=0xF0) && (thiskey!=0xF0) ) // this was a keypress, not a release
         {
            PrintChar(out);
            shing[0]+=128;
            shing[1]+=128;
         }

//         PrintString(shing);
      }
   }
   return 0;
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
   EXT_INT_EDGE(2, H_TO_L); // set interrupt for falling edge

   output_float(KBCLK_PIN); // clock is usually an input
   output_float(KBDAT_PIN); // data is usually an input
   
   
   
   port_b_pullups(0x06); // B1 and B2 pulled up

//   enable_interrupts(INT_EXT2);     // PS/2 keyboard clock line interrupt
   

   BlinkLED(0);   // just turn the damn light on
      

}

  // SSPSTAT= SMP | CKE;
   /////   SSPCON1= SSPEN; // this turns on the hose


void Backspace(void)
{
   if(conX)
   {
      console[CONSOLELAST+conX]=0;
      conX--;
   }
}


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
//    start=restart_cause(); 
//    if(start==NORMAL_POWER_UP)

