//*******************************************************************
//*******************************************************************
// registers in common
//*******************************************************************
//*******************************************************************


// status
#byte    STATUS=  getenv("SFR:STATUS")
#byte    BSR=     getenv("SFR:BSR")
#byte    PCLATH=  getenv("SFR:PCLATH")

// indirect addressing
#byte    INDF0=   getenv("SFR:INDF0")
#byte    POSTINC0=getenv("SFR:POSTINC0")
#byte    POSTDEC0=getenv("SFR:POSTDEC0")

#byte    FSR0H=   getenv("SFR:FSR0H")
#byte    FSR0L=   getenv("SFR:FSR0L")

#byte    INDF1=   getenv("SFR:INDF1")
#byte    POSTINC1=getenv("SFR:POSTINC1")
#byte    FSR1H=   getenv("SFR:FSR1H")
#byte    FSR1L=   getenv("SFR:FSR1L")


#byte    INDF2=   getenv("SFR:INDF2")
#byte    POSTINC2=getenv("SFR:POSTINC2")
#byte    FSR2H=   getenv("SFR:FSR2H")
#byte    FSR2L=   getenv("SFR:FSR2L")

#byte    PRODH=   getenv("SFR:PRODH")
#byte    PRODL=   getenv("SFR:PRODL")

// table access to ROM
#byte    TBLPTRU= getenv("SFR:TBLPTRU")
#byte    TBLPTRH= getenv("SFR:TBLPTRH")
#byte    TBLPTRL= getenv("SFR:TBLPTRL")
#byte    TABLAT=  getenv("SFR:TABLAT")


//*******************************************************************
// peripheral registers
//*******************************************************************

#define  SPEN_BIT  7
#define  SPEN      0x80  // serial port enable
#define  CSRC      0x80  // master mode/slave
#define  SYNC      0x20  // synchronous/asynchronous
#define  TXEN_BIT  4
#define  TXEN      0x10  // transmit/receive

#ifdef KCPU
#byte RCREG=      getenv("SFR:RCREG")
#byte TXREG=      getenv("SFR:TXREG")
#byte TXSTA=      getenv("SFR:TXSTA")
#byte RCSTA=      getenv("SFR:RCSTA")

#byte SPBRG=      getenv("SFR:SPBRG") // baud rate
#byte SPBRGH=     getenv("SFR:SPBRGH") // high byte
#byte BAUDCON=    getenv("SFR:BAUDCON")
#byte SSPSTAT=    getenv("SFR:SSPSTAT")
#byte SSPADD =    getenv("SFR:SSPADD")
#byte SSPBUF =    getenv("SFR:SSPBUF")
#byte SSPCON1=    getenv("SFR:SSPCON1")
#byte SSPCON2=    getenv("SFR:SSPCON2")

#else
#byte RCREG=      getenv("SFR:RCREG1")
#byte TXREG=      getenv("SFR:TXREG1")
#byte TXSTA=      getenv("SFR:TXSTA1")
#byte RCSTA=      getenv("SFR:RCSTA1")

#byte SPBRG=      getenv("SFR:SPBRG1") // baud rate
#byte SPBRGH=     getenv("SFR:SPBRGH1") // high byte
#byte BAUDCON=    getenv("SFR:BAUDCON1")

#byte SSPSTAT=    getenv("SFR:SSP1STAT")
#byte SSPADD =    getenv("SFR:SSP1ADD")
#byte SSPBUF =    getenv("SFR:SSP1BUF")
#byte SSPCON1=    getenv("SFR:SSP1CON1")
#byte SSPCON2=    getenv("SFR:SSP1CON2")

#endif
//#warning getenv("SFR:SSPCON1")
// CCP

#define CCP1IF    2
#define CCP2IF    1

#byte CCPR1H=     getenv("SFR:CCPR1H")
#byte CCPR1L=     getenv("SFR:CCPR1L")

#byte CCPR2H=     getenv("SFR:CCPR2H")
#byte CCPR2L=     getenv("SFR:CCPR2L")


// interrupts

#define  INTEDG2  0x10
#define  SSPIF    0x08


#byte PIE1=       getenv("SFR:PIE1")
#byte PIE2=       getenv("SFR:PIE2")
#byte PIR1=       getenv("SFR:PIR1")
#byte PIR2=       getenv("SFR:PIR2") 
#byte INTCON2=    getenv("SFR:INTCON2")
#byte INTCON3=    getenv("SFR:INTCON3")

#define INT2IF    1  // bit 1 of INTCON3 is external interrupt #2
#define INT2MASK (1<<INT2IF)
#define TMR2IF    1  // bit 1 (not bit 0)
#define TMR2IE    1  // bit 1 (not bit 0)

// PORT latches
#byte    LATC=    getenv("SFR:LATC")
#byte    LATB=    getenv("SFR:LATB")



// Slew rate control

#byte    SLRCON=  getenv("SFR:SLRCON")// this PIC allows you to decrease the
#define  PORTC_SLEW  0x04  // slew rate of the output ports under software control
             
// PORT Mapping (JCPU only)

#byte RPOR17=  getenv("SFR:RPOR17")
#byte RPOR18=  getenv("SFR:RPOR18")
#byte RPINR17= getenv("SFR:RPINR17")

// oscillator control

#byte OSCTUNE= getenv("SFR:OSCTUNE")
#define PLLEN  0x40


