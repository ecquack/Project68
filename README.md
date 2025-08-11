# Project68

Motorola 68008 single board computer with PIC18F87K22 co-processor running uCLinux 4.4

- MC68008 CPU clocked at 7.16 mHz
- 4 megabytes of SRAM
- Optional overclocking to 14.31818 mHz
- PIC18F87K22 co-processor clocked at 14.31818 mHz (57.27 mHz internal w/4X PLL)
- 80x25 character black and white ASCII video display (NTSC composite video)
- PS/2 keyboard interface
- TTL serial port
- micro-SD card socket
- Audio Speaker for ASCII “bell” sound


## CPU

The CPU is the 8 bit bus version of the MC68000. It comes in the larger 52 pin PLCC package which is socketed. The parts I have are MC68008FN8, rated at 8mHz. In order to synchronize with the NTSC CCIR-601 video clock, we run the processor at ~7.16mHz There are also MC68008FN10 parts available, rated at 10mHz. Either part can (usually) be successfully overclocked to ~14.318mHz, and a jumper is provided for this if you feel the need for speed.

## MEMORY

A single 32 megabit 4Mx8 SRAM chip is used.This fills the entire available address space with SRAM. There is no separate FLASH, EPROM, or ROM memory, no address decoding of any kind, and no memory mapped I/O.

## CO-PROCESSSOR

The PIC 18F87K22 co-processor provides all the processor’s I/O using soft-DMA. When it is time to transfer data to or from the processor, the co-processor requests the bus from the 68008. When the bus is granted, it uses bit-banging DMA to read and write the SRAM. Specific addresses in the memory space are designated as I/O registers and are interpreted as such by the co-processor to provide low level BIOS functions. 

At power up the co-processor asserts _HALT and _RESET and then copies the 64K BIOS/LOADER into the SRAM from the upper half of FLASH memory that’s built into the PIC18. It then de-asserts _HALT and _RESET and the MC68008 does a power on self test and then runs the loader. The LOADER loads the RAM filing system from the SD card and starts Linux. 

## PS/2 INTERFACE

Project68 uses the MSSP (synchronous serial port) on the PIC18 and a tight assembly language interrupt handler to interface to the PS/2 keyboard. 

When characters arrive on the PS/2 keyboard or serial port, the co-processor requests the bus, stores the received characters into the SRAM, and generates a MC68008 interrupt.

Due to hardware limitations of the MSSP certain scan codes on a standard keyboard are not readable or do not work as expected. In particular, the F5 and F7 keys cannot be distinguished from one another. 
 
## VIDEO DISPLAY

The co-processor is also responsible for generating the 80x25 video display. This is done with a tight assembly language interrupt handler which uses a character ROM lookup table with an 8x8 pixel font. One EUSART on the PIC18 is used as a video shift register (nice!), which makes it possible to clock pixel out once per instruction cycle. 

The physical video interface consists of two output pins and a two resistor network that is connected to an RCA jack. One pin is the output of the synchronous serial transmitter, and the other is the software generated sync line, used for both horizontal and vertical sync.

The video display is not interlaced and does not include a colorburst. The 8x8 ROM font consists of the standard printable ASCII characters (codes 32-126), plus a number of special characters (codes 0-31 and 127). The upper 128 characters are reverse video of the lower 128 characters, which is used to generate the cursor.

## ADDRESS SPACE

The address space of the 68008 is filled completely by the 4Mx8 SRAM chip. There is no address decoding and no EPROM, ROM, or FLASH memory on the MC68008 bus.
There is also no memory mapped I/O. Just an unbroken address space full of zero wait state SRAM.

## BIOS

The BIOS only provides a few functions . Get console character, put console character, put console string, get serial character, get serial string, put serial character, put serial string, read SD card block, and write SD card block are the essential functions.

## TIMER INTERRUPT

The vertical blanking signal serves as an periodic timer interrupt (60 hertz) to the 68008. 

