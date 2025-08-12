
void SandCastle(void)
{
   int index;
/*
vsync on, four sandcastles, vsync off, four blanks
repeat
sancastles are not symmetrical
*/
   for(;;)
   {
      output_high(VSYNC_PIN);
      for(index=0;index<4;index++)
      {
         output_low(SYNC_PIN);
         output_low(PIXEL_PIN);
         delay_us(2);
         output_high(SYNC_PIN);
         delay_us(1);
         output_high(PIXEL_PIN);
         delay_us(1); 
         output_low(PIXEL_PIN);
         delay_us(1);
      }
      output_low(VSYNC_PIN);
      delay_us(20);
   }

}

