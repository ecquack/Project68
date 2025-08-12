

#int_global
replace the built in interrupt dispatcher (no specific interrupts supported)
#int_default
what to do if a non-specified interrupt occurs (but we still allow specific interrupts)
#device high_ints=
support high priority interupts
#int_xxx fast
treat xxx as a high priority interrupt


0008:  MOVWF  05           // we store 'w' in 05
000A:  MOVFF  STATUS,06    // we store 'status' in 06
000E:  MOVFF  BSR,07       // we store BSR in 07
0012:  MOVLB  0            // we clear the BSR. why?
0014:  MOVFF  FSR0L,0D
0018:  MOVFF  FSR0H,08
001C:  MOVFF  FSR1L,09
0020:  MOVFF  FSR1H,0A
0024:  MOVFF  FSR2L,0B
0028:  MOVFF  FSR2H,0C
002C:  MOVFF  PRODL,14
0030:  MOVFF  PRODH,15
0034:  MOVFF  PCLATH,16

0038:  MOVFF  00,0F  // WTF? 
003C:  MOVFF  01,10  // what is special about these locations?
0040:  MOVFF  02,11  // the symbol map calls them "scratch"
0044:  MOVFF  03,12  // and "return" (address 1 is "return")
0048:  MOVFF  04,13

004C:  BTFSS  PEI1.1
004E:  GOTO   0058
0052:  BTFSC  PIR1.1
0054:  GOTO   00B8      // PWM interrupt
0058:  BTFSS  INTCON.3
005A:  GOTO   0064      // return from interrupt
005E:  BTFSC  INTCON.0
0060:  GOTO   00AA      // button interrupt (port B change)

0064:  MOVFF  0F,00
0068:  MOVFF  10,01
006C:  MOVFF  11,02
0070:  MOVFF  12,03
0074:  MOVFF  13,04
0078:  MOVFF  0D,FSR0L
007C:  MOVFF  08,FSR0H
0080:  BSF    08.7      // we set the high bit of 08? to show that FSR is valid?
0082:  MOVFF  09,FSR1L
0086:  MOVFF  0A,FSR1H
008A:  MOVFF  0B,FSR2L
008E:  MOVFF  0C,FSR2H
0092:  MOVFF  14,PRODL
0096:  MOVFF  15,PRODH
009A:  MOVFF  16,PCLATH
009E:  MOVF   05,W
00A0:  MOVFF  07,BSR
00A4:  MOVFF  06,STATUS
00A8:  RETFIE 0

00AA: ISR_RB

00B8: ISR_PWM

RAM
005-017 is reserved for use by interrupts


Main()

01A0:  CLRF   TBLPTRU
01A2:  BCF    RCON.7
01A4:  BSF    08.7
01A6:  CLRF   FSR0L
01A8:  CLRF   FSR0H
01AA:  MOVF   ANSEL,W
01AC:  ANDLW  E0
01AE:  MOVWF  ANSEL
01B0:  BCF    ADCON1.4  // analog
01B2:  BCF    ADCON1.5
01B4:  MOVLW  00
01B6:  MOVWF  ANSELH
01B8:  BCF    CM2CON1.5
01BA:  BCF    CM2CON1.4
01BC:  CLRF   CM1CON0   // comparator?
01BE:  CLRF   CM2CON0
01C0:  CLRF   18  // RTC
01C2:  CLRF   19  
01C4:  CLRF   1C  / remaining
01C6:  CLRF   1D

