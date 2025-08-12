# Project68

Project68 is a Motorola MC68xxx 16/32 bit single board computer with an 8 bit PIC18 co-processor running uCLinux 4.4.

It connects directly to a monitor and keyboard and does not require a host computer.

The goal was to create a self contained computer using a minimum number of components and tools.

The project is open source. Binaries are provided, but to build the PIC18 firmware from scratch you will need the commercial CSS PICC compiler. 
Special thanks to *crmaykish*, creator of the Mackerel-08 project.

This project is at an early stage of development.

- MC68008 CPU clocked at 7.16 mHz
- 4 megabytes of SRAM
- Optional overclocking to 14.31818 mHz
- PIC18F87K22 co-processor clocked at 14.31818 mHz (57.27 mHz internal w/4X PLL)
- 80x25 character black and white ASCII video display (NTSC composite video)
- PS/2 keyboard interface
- TTL serial port
- micro-SD card socket
- Audio Speaker for ASCII “bell” sound
- No PAL/GAL/CPLD/FPGA programmable logic required
- Only two TTL parts required.

## REQUIREMENTS

- Monitor with composite video input
- PS/2 keyboard
- 5V power supply
- SD card with uCLinux image

 ## BUILD REQUIREMENTS
 
- PIC programmer for initial programming
- CCS PICC C Compiler (optional if you don't use the pre-built binary)
- 

## CPU

The CPU is the 8 bit external bus version of the MC68000, the MC68008FN8, in the larger 52 pin PLCC package. The CPU is socketed on the board so that you can use an automated assembly service such as JLC PCB or PCBWAY for all the soldering. This is a 16/32 bit CPU internally with a narrow external bus which allows using byte wide external memories.

The parts I have tested are rated at 8mHz. In order to synchronize with the NTSC CCIR-601 video clock, we run the processor at ~7.16mHz There are also MC68008FN10 parts available, rated at 10mHz. Either part can (usually) be successfully overclocked to ~14.318mHz, and a jumper is provided for this if you feel the need for speed.

## MEMORY

The system has four megabytes of memory. It uses a single 32 megabit 4Mx8 55ns SRAM chip. This fills the entire available address space with SRAM. There is no separate FLASH, EPROM, or ROM memory, no address decoding of any kind,  no special address hardware at reset, and no traditional memory mapped I/O. Just an unbroken sea of RAM from 0-0x3FFFFF and a simple _DTACK generator. The SRAM part can be a Renesas RMLV3216AGSA-5S2. You can also use an Alliance AS6C6416-55TIN which is a 64 megabit part, but you will only have access to half the memory. 

The memory runs at 3.3 volts (there is an LDO regulator on the board) but the I/O pins are TTL compatible for interfacing with the PIC18 and MC68008, with a high logic level of 2.4 volts.

## CO-PROCESSSOR

The PIC18F87K22 microcontrollre comes in an 80 pin TQFP package. This part was chosen because it has enough GPIO pins to emulate the entire MC68008 bus. It runs at 5 volts, has 128K of FLASH memory, and has just under 4K of on chip SRAM.

The PIC 18F87K22 co-processor provides all the main processor’s I/O using soft-DMA. When it is time to transfer data to or from the processor, the co-processor requests the bus from the 68008. When the bus is granted, it uses bit-banging DMA to read and write the SRAM. Specific addresses in the memory space are designated as I/O registers and are interpreted as such by the co-processor to provide low level BIOS functions. 

At power up the co-processor asserts _HALT and _RESET and then copies up to 64K of BIOS into the SRAM from the upper half of FLASH memory that’s built into the PIC18. It then de-asserts _HALT and _RESET and the MC68008 does a power on self test, then  the BIOS loads the initial RAM filing system from the SD card and starts uCLinux with busybox.

## PS/2 INTERFACE

The system uses the PIC18 MSSP (synchronous serial port) and a tight assembly language interrupt handler to interface to the PS/2 keyboard. 

When characters arrive on the PS/2 keyboard or serial port, the co-processor requests the bus, stores the received characters into the SRAM, and generates a MC68008 interrupt.

Due to hardware limitations of the MSSP (7 bits instead of 8 due to the start bit) certain scan codes on a standard keyboard are not readable or do not work as expected. In particular, the F5 and F7 keys cannot be distinguished from one another, and the zero/insert key on the numeric keypad is registered only on release.

You can use the TTL serial port for the console if this limitation is an issue for you.

The PS/2 interrupt handler always takes the same number of clock cycles to run, no matter which code path is taken.
 
## VIDEO DISPLAY

The co-processor is also responsible for generating the 80x25 video display. This is done with a *really* tight  horizontal sync interrupt handler written in assembly language. It uses an interleaved character FLASH lookup table with an 8x8 pixel font. One EUSART on the PIC18 is used as a video shift register (nice!), which makes it possible to clock pixels out once per instruction cycle.

Roughly half of the PIC18's SRAM (2K) is dedicated to the video display.

The physical video interface consists of two output pins and a two resistor network that is connected to an RCA jack. One pin is the output of the synchronous serial transmitter, and the other is the software generated sync line, used for both horizontal and vertical sync.

The video display is not interlaced and does not include a colorburst. The 8x8 ROM font consists of the standard printable ASCII characters (codes 32-126), plus a number of special characters (codes 0-31 and 127). The upper 128 characters are reverse video of the lower 128 characters, which is used to generate the cursor. There are no fancy effects like blink, underscore, or dim. There are no code page 437 special characters.

The HSYNC interrupt handler contains a single NOP instruction (for timing purposes) in the 8 instruction inner loop. There are are not enough spare clock cycles to add any more features.

Most of the co-processor's time is spent generating video. There is a short horizontal blanking period at the end of each line, and there is a longer vertical blanking interval at the end of each video field during which the processor is free to perform other tasks. All video generation (and PS/2 keyboard processing) takes place inside the interrupt handler.

## SD CARD

The SD card socket is connected to the PIC18 over the shared SPI bus. 512 bytes of PIC18 SRAM is reserved to store a single block of data to/from the SD card. 

## BIOS

The BIOS provides a few functions to access the console, the serial port, and the SD card. It also loads and starts Linux from the SD card. Additional functions could be added, for example, to copy the entire screen buffer to and from memory, give control over the PWM audio output beyond the fixed "beep", or to support additional SPI devices such as a non-volatile real time clock. 

## TIMER INTERRUPT

The vertical blanking signal serves as a periodic timer interrupt (60 hertz) to the 68008. This is used by the Linux kernel for timekeeping and task switching. 

## EXPANSION PORT

- TTL serial port (RX, TX, GND)
- Power (+5V, +3.3V)
- GPIO digital only (3) 
- GPIO or analog input (5)
- SPI BUS (shared with SD card)

## ICSP PORT

For programming the PIC MCU, this 6 pin connector contains PGD, PGC, _MCLR, +5V, and GND.

## PERFORMANCE NOTES

For higher system performance you could use a second smaller PIC18 (such as a PIC18F27K22) dedicated to generating video. The current system was designed to see how simply a complete 68K computer system could be made, and it uses a minimum number of parts for a system that has its own video and keyboard interfaces. 

A decoded address that causes an interrupt on the PIC18 would eliminate the need for the PIC18 to poll the MC68008's memory. Again, this would increase performance but it would add chips to the design and eat into the memory address space. Two 74HCT30s and a 74HCT00 would suffice, and would eat only 64 bytes from the memory map. 

