
#asm
FlashWrite():
      MOVF   INTCON,W
      MOVWF  @03          // store interrupt flags in @03
      BCF    INTCON.GIE   // disable global interrupts

      BSF    EECON1.EEPGD // access flash memory set
      BSF    EECON1.WREN  // flash memory write enable
      MOVLB  F            // BSR= 0xF
      MOVLW  55
      MOVWF  EECON2       // write 0x55 to EECON2 (magic cookie)
      MOVLW  AA
      MOVWF  EECON2       // write 0xAA to EECON2 (magic cookie)

      BSF    EECON1.WR    // start a write/erase cycle
      NOP   
      MOVF   @03,W        
      IORWF  INTCON,F     // restore interrupt flags
      MOVLB  0            // BSR= 0x00
      RETLW  00

writeentry:
      BSF    RCON.SBOREN  // brown out reset enabled
      BCF    EECON1.CFGS  // clear bit for FLASH (versus EEPROM)
      MOVF   TBLPTRL,W    // store @00=TBLPTR% 0x1f 
      ANDLW  1F
      MOVWF  @00          
      MOVLW  E0
      ANDWF  TBLPTRL,F     // TBLPTRL&=0xE0 (mask off the lower bits 0-63)
      TBLRD*-              // TBLPTR--
      MOVFF  @WRITE_PROGRAM_MEMORY.P2,01   // 01= @WRITE_PROGRAM_MEMORY.P2
blah1:
      TSTFSZ @00           // goto blah2 if masked portion of address is zero (we're aligned)
      BRA    maskNZ
maskzero:
      MOVF   TBLPTRL,W
      ADDLW  01            // add one to TBLPTR with mask
      ANDLW  3F
      BNZ    maskNZ        // still not aligned? 

      BSF    EECON1.FREE   // erase the block
      TBLRD*+              // read from table++
      RCALL  FlashWrite()  // do the erase
      TBLRD*-
maskNZ:                    // not zero?
      TSTFSZ @00           // check unmasked
      BRA    unmaskZ

      MOVFF  POSTINC0,TABLAT  // move our data++ to TABLAT
      MOVF   @01,F         // 
      BZ     blah4         // branch if @01 zero

      TBLWT+*              // table write
      DECF   @01,F         // @01--
      BRA    blah5
unmaskZ:
      DECF   @00,F         // next byte
blah4:
      TBLRD+*              // TBLPTR++
      TBLWT*               // table write (pre-increment pair)
blah5:
      MOVLW   1F
      ANDWF   TBLPTRL,W    // are we aligned?
      XORLW   1F
      BNZ     blah1        // are we address 0x1F (done the whole block)
      RCALL   FlashWrite() // OK, write it.
      TSTFSZ  @01          // another block?
      BRA     blah1
      CLRF    TBLPTRU
      
      GOTO    myreturn

Write_Flash():
0322:  CLRF   TBLPTRU
0324:  MOVLW  30
0326:  MOVWF  TBLPTRH
0328:  CLRF   TBLPTRL      // TBLPTR= 0x3000
032A:  CLRF   FSR0H        // FSR0= block
032C:  MOVLW  block        // @WRITE_PROGRAM_MEMORY.P2= 0x40
032E:  MOVWF  FSR0L
0330:  MOVLW  40
0332:  MOVWF  @WRITE_PROGRAM_MEMORY.P2

myreturn:  BRA    writeentry
#endasm
