
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

