
void FlashWriteTest(void)
{
   char block[64];
   int times;
   int32 dest;
   
   delay_ms(2000);   // give us 2 seconds after the beep to man the stopwatch

   output_low(PIN_A5);  // turn off the LED

   for(times=0;times<8;times++)
   {
      for(dest=0x8000;dest<0xFFFF;dest+=64)
      {
         write_program_memory(dest,block,64);
      }
   }
   
   // this took 20 seconds.
   //
   // each dest loop writes 32K bytes of memory
   // performed 8 times is 128K
   // ~6553 bytes per second write speed.
   // call it 6.4K per second, 102 64 byte blocks per second
   // each 64 byte block takes 10 milliseconds to write.
   //
   // it would take 5 seconds to write the whole 64K bytes of flash memory- not bad
   
   //
   // if we store tokenized BASIC in flash we can write some fairly long programs.
   // 
   // Applesoft used a linked list of lines and got slower as the program got large.
   // never knew that. 
   // it also had a shortcut ? for the PRINT statement, which I never knew (sigh)
   //
   // yeah- but we're running at 14 megahertz and 1 clock per instruction. the 6502 took
   // 2-7 clocks per instruction and ran at 1mhz.
   //
 
   
      output_high(PIN_A5);// turn the LED back on

}
