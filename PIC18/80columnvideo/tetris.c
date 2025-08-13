;*******************************************************************************
;* SX-TETRIS (C) Rickard Gunée, 2001                                           *
;*******************************************************************************
;* This is the classical computer game tetris, outputing a color video signal  *
;* in software using a couple of resistors.                                    *
;* The video signal is not 100% correct, it will not work on all TV:s, so if   *
;* your TV can't lock on the color signal or you get strange colors on the     *
;* screen then your TV probably can't run this game.                           *
;* This is an open source project and you may use this design and software     *
;* anyway you like as long as it is non comercial and you refer to the         *
;* original author with name and link to homepage.                             *
;* Use this at your own risk, don't blame me if you blow up your tv or kill    *
;* yourself or anyone else with it.                                            *
;*                                                                             *
;* For more info about project go to: http://www.rickard.gunee.com/projects    *
;*******************************************************************************

      DEVICE   SX28,TURBO,STACKX_OPTIONX
      RESET   jumpstart         ;goto 'start' on reset
      NOEXPAND

      SYSTEM_PAL   = 1
      SYSTEM_PAL_N   = 2
      SYSTEM_PAL_M   = 3
      SYSTEM_NTSC   = 4

      SYSTEM = SYSTEM_PAL   ;This line selects TV-system timing to use



      IF (SYSTEM = SYSTEM_PAL)

         FREQ   53156550

         TIME_2US4      EQU   128   
         TIME_4US5      EQU   239
         TIME_27US5      EQU   1463   
         TIME_29US6      EQU   1574
         TIME_64US      EQU   3405
         TIME_TOTAL      EQU   TIME_64US
         TIME_PRESYNC      EQU   89
         TIME_SYNC      EQU   250
         TIME_PREBURST      EQU   48
         TIME_BURST      EQU   144
         TIME_POSTBURST      EQU   114

         TIME_LEFTGFX      EQU   80*12
         TIME_RIGHTGFX      EQU   40*12
         LEFTGFX_BASE      EQU   12*10

         TOT_LINES      EQU   304
         PRE_LINES      EQU   35
         POST_LINES      EQU   13

         BRICK_WIDTH      EQU   7

         BLINE_PHASE      EQU   5
         CAP_BASE      EQU   70*12
         CAP_PHASE      EQU   6
         CAP_PHASEDIFF      EQU   -1
         LEFTSCREW_PHASE      EQU   7
         RIGHTSCREW_PHASE   EQU   0
         GAMEFIELD_PHASE      EQU   6


         SCORE_BASE      EQU   12*6
         SCORE_PHASE      EQU   9
         TEXTNEXT_BASE      EQU   12*6
         TEXTNEXT_PHASE      EQU   7
         TEXTSCORE_BASE      EQU   12*2
         TEXTSCORE_PHASE      EQU   1
         NBLOCK_BASE      EQU   12*6
         NBLOCK_PHASE      EQU   6
         GAMEOVER_PHASE      EQU   6
         GAMEOVER_BASE      EQU   17*12

         STR0_BASE      EQU   38*12
         STR0_PHASE      EQU   11
         STR1_BASE      EQU   30*12
         STR1_PHASE      EQU   7


      ENDIF

      IF (SYSTEM = SYSTEM_PAL_M)

         FREQ   42907332

         TIME_2US4      EQU   103   
         TIME_4US5      EQU   193
         TIME_27US5      EQU   1181   
         TIME_29US6      EQU   1271
         TIME_64US      EQU   2749
         TIME_TOTAL      EQU   TIME_64US
         TIME_PRESYNC      EQU   47
         TIME_SYNC      EQU   202
         TIME_PREBURST      EQU   39
         TIME_BURST      EQU   144
         TIME_POSTBURST      EQU   5

         TOT_LINES      EQU   254
         PRE_LINES      EQU   35
         POST_LINES      EQU   13

         TIME_LEFTGFX      EQU   80*12
         TIME_RIGHTGFX      EQU   40*12
         LEFTGFX_BASE      EQU   12*10

         BRICK_WIDTH      EQU   5

         BLINE_PHASE      EQU   3
         CAP_BASE      EQU   70*12
         CAP_PHASE      EQU   4
         CAP_PHASEDIFF      EQU   0
         LEFTSCREW_PHASE      EQU   5
         RIGHTSCREW_PHASE   EQU   9
         GAMEFIELD_PHASE      EQU   3


         SCORE_BASE      EQU   12*6
         SCORE_PHASE      EQU   8
         TEXTNEXT_BASE      EQU   12*6
         TEXTNEXT_PHASE      EQU   6
         TEXTSCORE_BASE      EQU   12*2
         TEXTSCORE_PHASE      EQU   0
         NBLOCK_BASE      EQU   12*6
         NBLOCK_PHASE      EQU   4

         GAMEOVER_PHASE      EQU   7
         GAMEOVER_BASE      EQU   10*12

         STR0_BASE      EQU   20*12
         STR0_PHASE      EQU   10
         STR1_BASE      EQU   13*12
         STR1_PHASE      EQU   5
      ENDIF


      IF (SYSTEM = SYSTEM_PAL_N)

         FREQ   42984672

         TIME_2US4      EQU   103   
         TIME_4US5      EQU   193
         TIME_27US5      EQU   1181   
         TIME_29US6      EQU   1271
         TIME_64US      EQU   2749
         TIME_TOTAL      EQU   TIME_64US
         TIME_PRESYNC      EQU   47
         TIME_SYNC      EQU   202
         TIME_PREBURST      EQU   39
         TIME_BURST      EQU   144
         TIME_POSTBURST      EQU   5

         TOT_LINES      EQU   304
         PRE_LINES      EQU   35
         POST_LINES      EQU   13

         TIME_LEFTGFX      EQU   80*12
         TIME_RIGHTGFX      EQU   40*12
         LEFTGFX_BASE      EQU   12*10

         TOT_LINES      EQU   304
         PRE_LINES      EQU   35
         POST_LINES      EQU   13

         TIME_LEFTGFX      EQU   80*12
         TIME_RIGHTGFX      EQU   40*12
         LEFTGFX_BASE      EQU   12*10

         BRICK_WIDTH      EQU   5

         BLINE_PHASE      EQU   3
         CAP_BASE      EQU   70*12
         CAP_PHASE      EQU   4
         CAP_PHASEDIFF      EQU   0
         LEFTSCREW_PHASE      EQU   5
         RIGHTSCREW_PHASE   EQU   9
         GAMEFIELD_PHASE      EQU   3


         SCORE_BASE      EQU   12*6
         SCORE_PHASE      EQU   8
         TEXTNEXT_BASE      EQU   12*6
         TEXTNEXT_PHASE      EQU   6
         TEXTSCORE_BASE      EQU   12*2
         TEXTSCORE_PHASE      EQU   0
         NBLOCK_BASE      EQU   12*6
         NBLOCK_PHASE      EQU   4

         GAMEOVER_PHASE      EQU   7
         GAMEOVER_BASE      EQU   10*12

         STR0_BASE      EQU   20*12
         STR0_PHASE      EQU   10
         STR1_BASE      EQU   13*12
         STR1_PHASE      EQU   5
      ENDIF


      IF (SYSTEM = SYSTEM_NTSC)

         FREQ   42954545

         TIME_2US4      EQU   103   
         TIME_4US5      EQU   193
         TIME_27US5      EQU   1181   
         TIME_29US6      EQU   1271
         TIME_64US      EQU   2748
         TIME_TOTAL      EQU   TIME_64US
         TIME_PRESYNC      EQU   47
         TIME_SYNC      EQU   202
         TIME_PREBURST      EQU   39
         TIME_BURST      EQU   144
         TIME_POSTBURST      EQU   5

         TOT_LINES      EQU   254
         PRE_LINES      EQU   25
         POST_LINES      EQU   13

         TIME_LEFTGFX      EQU   80*12
         TIME_RIGHTGFX      EQU   40*12
         LEFTGFX_BASE      EQU   12*10

         BRICK_WIDTH      EQU   5

         BLINE_PHASE      EQU   3
         CAP_BASE      EQU   70*12
         CAP_PHASE      EQU   4
         CAP_PHASEDIFF      EQU   0
         LEFTSCREW_PHASE      EQU   5
         RIGHTSCREW_PHASE   EQU   9
         GAMEFIELD_PHASE      EQU   3


         SCORE_BASE      EQU   12*6
         SCORE_PHASE      EQU   8
         TEXTNEXT_BASE      EQU   12*6
         TEXTNEXT_PHASE      EQU   6
         TEXTSCORE_BASE      EQU   12*2
         TEXTSCORE_PHASE      EQU   0
         NBLOCK_BASE      EQU   12*6
         NBLOCK_PHASE      EQU   4

         GAMEOVER_PHASE      EQU   7
         GAMEOVER_BASE      EQU   10*12

         STR0_BASE      EQU   20*12
         STR0_PHASE      EQU   10
         STR1_BASE      EQU   13*12
         STR1_PHASE      EQU   5

      ENDIF




      delaytimer1   equ   08h
      delaytimer2   equ   09h
      temp0      equ   08h
      temp1      equ   09h
      temp2      equ   0Ah
      temp3      equ   0Bh
      temp4      equ   0Ch
      temp5      equ   0Dh
      temp6      equ   0Eh
      temp7      equ   0Fh


      joy1up      equ   RB.7
      joy1down   equ   RC.5

      joy1left   equ   RC.6
      joy1right   equ   RC.7

      joy1button   equ   RB.6

      x      equ   $10
      y      equ   $11
      kind      equ   $12
      angle      equ   $13
      nextkind   equ   $14
      falltimer   equ   $15
      oldjoy      equ   $16
      joytimer   equ   $17
      blockbuff   equ   $18

      mixedbits   equ   $10
      rnd      equ   $11
      gfxcnt      equ   $12
      linecnt      equ   $13
      musictimer   equ   $14
      songpos      equ   $15

      stemp0      equ   $1B
      pos      equ   $1C
      pos_l      equ   $1C
      pos_h      equ   $1D
      sfreq      equ   $1E
      sfreq_l      equ   $1E
      sfreq_h      equ   $1F

      joy1button_old   equ   oldjoy.2

      black      equ   14
      neutral      equ   14

      JTIME      equ   10

      frame      equ   7
      gameoverbit   equ   6

      video      equ   RB               
      audio      equ   RC

      CAP_SEP         EQU   (((BRICK_WIDTH + 2)*8*12) + (13*12))
      TIME_HSYNC      EQU   (TIME_PRESYNC + TIME_SYNC + TIME_PREBURST + TIME_BURST + TIME_POSTBURST)
      TIME_IMAGE      EQU   (TIME_TOTAL - TIME_HSYNC)
      VISILINES      EQU   (TOT_LINES - PRE_LINES - POST_LINES)

      TOP_LINES      EQU   (VISILINES/30)
      STRTOCAP_LINES      EQU   (VISILINES/20)
      BRICK_LINES      EQU   (VISILINES/25)
      PLAYFIELD_LINES      EQU   ((BRICK_LINES+2)*16)

      VIDEO_BUFFER      EQU   $50

      SCORE         EQU   $3C
      NEXTGFX         EQU   $39

      FONT      EQU $0      ;fastmem
      STR0      EQU $100   ;fastmem
      STR0_LEN   EQU 20      ;fastmem
      STR1      EQU $115   ;fastmem
      STR1_LEN   EQU 22      ;fastmem
      STR2      EQU $12c   ;fastmem
      STR2_LEN   EQU 6      ;fastmem
      STR3      EQU $133   ;fastmem
      STR3_LEN   EQU 6      ;fastmem
      STR4      EQU $13a   ;fastmem
      STR4_LEN   EQU 5      ;fastmem
      STR5      EQU $140   ;fastmem
      STR5_LEN   EQU 5      ;fastmem
      NUMBERS      EQU $146   ;fastmem
      SCREW      EQU $0      ;slowmem
      CAP      EQU $40      ;slowmem
      SINTABLE   EQU $b9      ;slowmem
      FREQTBL      EQU $d9      ;slowmem
      MUSIC      EQU $169   ;slowmem



org 2


;*********************** add16 macro **************************
;* This is a macro to add two 16bit numbers, inputs two       *
;* arguments, each pointing at the lsb register followed by   *
;* the msb register at poistion arg+1.                        *
;* Reults is stored in registers referred to by first arg     *
;* arg1 = arg1 + arg2                                         *
;* clocks: 6                                                  *
;**************************************************************

add16      MACRO   2
      add   (\1),(\2)         ;2
      snc            ;1(2)
      inc   (\1) + 1         ;1
      add   (\1) + 1, (\2) + 1      ;2
      ENDM



;*********************** pcall macro **************************
;* This macro does the same as lcall but in 2 words           *
;**************************************************************

pjmp   MACRO   1
   page (\1)
   jmp   (\1)
   ENDM



;*********************** pcall macro **************************
;* This macro does the same as lcall but in 2 words           *
;**************************************************************

pcall   MACRO   1
   page (\1)
   call   (\1)
   ENDM


;************************ vout macro **************************
;* This macro outputs a constant to the video DA              *
;**************************************************************

vout   MACRO   1
   mov   w,#(\1)
   mov   video,w
   ENDM


;*********************** voutr macro **************************
;* This macro outputs data from a register to the video DA    *
;**************************************************************

voutr   MACRO   1
   mov   w,\1
   mov   video,w
   ENDM


;*********************** itext macro **************************
;* macro for showing a line of chars from rom                 *
;* parameters: strpointer,length,base,phase                   *
;**************************************************************

ITEXT      MACRO   4
      mov   temp7,#8            ;2
      mov   temp4,#((gamedata + FONT) & $ff)   ;2
:bots_l      pcall   hsync               ;1+TIME_HSYNC
      delay   (\3) - (\4)
      mov   temp1,#(((\1) + gamedata) >> 8)      ;2
      mov   temp3,#(((\1) + gamedata) & $FF)   ;2
      mov   temp5,#(\2)             ;2
      pcall   strout               ;STR_LEN*8 * 12 * (w-1) + 42 + 1
      inc   temp4               ;1
      delay   TIME_IMAGE-((((\2)-1)*8*12) + 44 + 1) - (\3) + (\4) - (2+2+2+1+4+1) 
      decsz   temp7               ;1(2)
      jmp   :bots_l               ;3
      delay   2
      pcall   hsync
      ENDM

;************************ tnop macro **************************
;* This macro creates a delay of 3 clock cycles only using    *
;* one word of program memory.                                *
;**************************************************************

tnop   MACRO
:tnopf   
      IF (:tnopf & %111000000000) = ((:tnopf+1) & %111000000000)
         jmp :tnopf + 1
      ELSE
         nop
         nop
         nop
      ENDIF
   ENDM

;********************** setphase macro ************************
;* This is a macro for creating delay that depends of the     *
;* contents of w, it adds w to the low part of pc, and adds   *
;* nops after the jmp instruction, the number of nops is      *
;* specified as a parameter to the function                   *
;**************************************************************

setphase   MACRO   1
      jmp   pc+w
      REPT   \1
      nop      
      ENDR
      ENDM

;************************ delay macro *************************
;* This is a macro for creating delays by calling the delay   *
;* functions, it minimizes the number of program words to max *
;* 4 words. For delaytimes less than 1017 and longer than 9   *
;* it uses the short delay functions at the cost of 2-3 words *
;* for shorter delays it uses the fixed delays at a cost of 1 *
;* to 3 words, longer delays are done by a call to the short  *
;* delay functions followed by a long delay call with a total *
;* cost of 4-6 words of program memory. The macro can handle  *
;* delays from 0 to 260k cycles.                              *
;*                                                            *
;* WARNING, no guarantee that this really works correctly for *
;* all delays as it quite complex and I'm too lazy to test it *
;**************************************************************

delay   MACRO   1

:delbase
   IF (:delbase & $E00) = (delay9 & $E00)
     IF ((\1)<6)
       IF ((\1)//3)=1
              nop
            ENDIF
       IF ((\1)//3)=2
         nop
         nop
       ENDIF
       IF ((\1)/3) > 0 
           REPT ((\1)/3)
      TNOP
             ENDR
       ENDIF
     ENDIF

     IF ((\1)>5) AND ((\1)<10)
       call   delay6 - ((\1)-6)
     ENDIF

     IF ((\1) > 9) AND ((\1)<1027)
       mov w,#((\1)-6)>>2
       call delay_short_0 - (((\1)-6)&3)
     ENDIF

     IF (\1) > 1026
       IF (((\1)-12)//1017)<10
              mov w,#(((((\1)-12)//1017)+1017)>>2)
         call delay_short_0 - (((((\1)-12)//1017)+1017)&3)
         mov w,#(((\1)-12)/1017)-1
       ELSE
              mov w,#((((\1)-12)//1017)>>2)
         call delay_short_0 - ((((\1)-12)//1017)&3)
         mov w,#(((\1)-12)/1017)
       ENDIF
         call delay_long
     ENDIF
   ELSE
     IF ((\1)<7)
       IF ((\1)//3)=1
              nop
            ENDIF
       IF ((\1)//3)=2
         nop
         nop
       ENDIF
       IF ((\1)/3) > 0 
           REPT ((\1)/3)
      TNOP
            ENDR
       ENDIF
     ENDIF

     IF ((\1)>6) AND ((\1)<11)
       page delay6
       call delay6 - ((\1)-7)
     ENDIF

     IF ((\1) > 10) AND ((\1)<1028)
       mov w,#((\1)-7)>>2
       page delay_short_0
       call delay_short_0 - (((\1)-7)&3)
     ENDIF

     IF (\1) > 1027
       IF (((\1)-14)//1017)<10
              mov w,#(((((\1)-14)//1017)+1017)>>2)
         page delay_short_0
         call delay_short_0 - (((((\1)-14)//1017)+1017)&3)
         mov w,#(((\1)-14)/1017)-1
       ELSE
              mov w,#((((\1)-14)//1017)>>2)
         page delay_short_0
         call delay_short_0 - ((((\1)-14)//1017)&3)
         mov w,#(((\1)-14)/1017)
       ENDIF
         page delay_long
         call delay_long
     ENDIF
   ENDIF
ENDM


;********************** delay functions ***********************
;* Different delay functions to be able to create long delays *
;* using as few bytes of program memory as possible           *
;* These functions are required by the delay macro            *
;* delays with exact clock count uses no registers            *
;* short delays use temp0                                     *
;* long delays use temp0 and temp1                            *
;**************************************************************

delay9      nop         ;1      entrypoint of delay9 that delays 9 clocks
delay8      nop         ;1      entrypoint of delay8 that delays 8 clocks
delay7      nop         ;1      entrypoint of delay7 that delays 7 clocks
delay6      retp         ;3      entrypoint of delay6 that delays 6 clocks

delay_short_3   nop         ;1      entrypoint of delay_short_3 that delays 4*w + 8
delay_short_2   nop         ;1      entrypoint of delay_short_3 that delays 4*w + 7
delay_short_1   nop         ;1      entrypoint of delay_short_3 that delays 4*w + 6
delay_short_0   mov   temp0,w      ;1      entrypoint of delay_short_3 that delays 4*w + 5
delay_short_m   decsz   temp0      ;1(2)      decrease counter, mainloop of delay short
      jmp   delay_short_m   ;3      keep looping until counnter is zero
      retp         ;3      return back to caller

delay_long   mov   temp1,w      ;1      set long time counter from w
delay_long_l   mov   w,#251      ;1      set time to delay in short delay
      call   delay_short_3   ;1012      time to delay is 251*4+8=1012
      decsz   temp1      ;1(2)      decrease long time counter
      jmp   delay_long_l   ;3      keep looping until counnter is zero
      retp         ;1      return back to caller


;17
readsong   mov   m,#((MUSIC + gamedata) >> 8)      ;1
      mov   w,#(MUSIC + gamedata) & $FF      ;1
      add   w,songpos            ;1
      snc                  ;1(2)
      mov   m,#((MUSIC + gamedata) >> 8) + 1   ;1
      iread                  ;4
      mov   w,m               ;1
      inc   songpos               ;1
      ret                  ;3

;16
readfreqtbl   mov   m,#((FREQTBL + gamedata) >> 8)      ;1
      mov   w,#(FREQTBL + gamedata) & $FF      ;1
      add   w,temp0               ;1
      snc                  ;1(2)
      mov   m,#((FREQTBL + gamedata) >> 8)+1   ;1
      iread                  ;4
      mov   w,m               ;1
      ret                  ;3



;************************ readjoy1 ****************************
;* Reads joy1 bits from RC and RB and combining them to w     *
;* temp register 0 used                                       *
;* clocks: 12 + 1                                             *
;**************************************************************

readjoy1   mov   w,RC      ;1
      and   w,#%11100000   ;1
      mov   temp0,w      ;1
      mov   w,<>RB      ;1
      and   w,#%00001100   ;1
      or   w,temp0      ;1
      retp         ;3



;*********************** memtovideo ***************************
;* outputs data from memory to video output                   *
;* number of clocks: w*12 + 7 + 1                             *
;* temp register 0 used                                       *
;**************************************************************

memtovideo   mov   temp0,w      ;1   set pixelcounter
mtvl0      mov   w,ind      ;1   get lower level byte from mem
      mov   video,w      ;1   send to video output
      setb   fsr.5      ;1   select upper bank
      mov   w,ind      ;1   get upper level byte from mem
      inc   fsr      ;1   point at next pixel
      clrb   fsr.5      ;1   select lower bank
      nop         ;1   
      mov   video,w      ;1   send to video output
      decsz   temp0      ;1(2)   decrease pixel counter
      jmp   mtvl0      ;3   keep looping until all pixels are done
      vout   BLACK      ;2   set black color
      retp         ;3   get outa here


;********************** setgraphics ***************************
;* outputs data from memory to video output                   *
;* number of clocks: w*31 + 5 +1                              *
;* temp0 = bitmap rom-pointer bit 0..7                        *
;* temp1 = bitmap rom-pointer bit 8..11                       *
;* temp2 = palette rom-pointer bit 0..7                       *
;* fsr = pointr to memory where to store graphics             *
;* Note: bits 8..11 of palette pointer is in the constant     *
;* called PALETTE_PAGE, all palettes should be placed within  *
;* this page. fsr,temp0 and temp1 are modyfied                *
;**************************************************************

setgraphics   mov temp3,w         ;1   set pixelcounter
sgl0      mov m,temp1         ;2   set page
      mov w,temp0         ;1   get image pointer
      iread            ;4   read pixeldata from rom
      mov w,m            ;1   get slowmem nibble
      add w,temp2         ;1   select palettte, assuming all palettes within the same page
      mov m,#PALETTE_PAGE      ;1   select page
      iread            ;4   read palette
      mov ind,w         ;1   remember first level
      setb fsr.5         ;1   select second level bank
      and w,#$C0         ;1   mask out two upper bits
      mov ind,w         ;1   store second level two upper bits
      rr ind            ;1   move upper bits into correct position (1/2)
      rr ind            ;1   move upper bits into correct position (2/2)
      mov w,m            ;1   get second level lower nibble
      or ind,w         ;1   stor second level lower nibble
      clrb fsr.5         ;1   get back to first level bank
      inc fsr            ;1   point at next pixel memory position
      inc temp0         ;1   point at next nibble
      snz            ;1(2)
      inc temp1         ;1   if page overflow, go to next page
      decsz temp3         ;1(2)   decrease pixel counter
      jmp sgl0         ;3   keep looping until all pixels are done
      retp            ;3   get outa here


      
;************************* blocks *****************************
;* get compressed brick data                                  *
;* clock cycles: 20                                           *
;**************************************************************

blocks      and   w,#%1111
      add   pc,w         ;3
      retw   $50         ;3
      retw   $44         ;3
      retw   $D0         ;3
      retw   $0C         ;3
      retw   $D0         ;3
      retw   $0C         ;3
      retw   $D0         ;3
      retw   $3C         ;3
      retw   $D0         ;3
      retw   $CC         ;3
      retw   $F4         ;3
      retw   $C0         ;3
      retw   $5C         ;3
      retw   $C0         ;3
      retw   $00         ;3
      retw   $6C         ;3


;*********************** brickposcheck ************************
;* Check if out of bounds, calculate address to brick and     *
;* mask to unwanted nibble                                    *
;* clock cycles: 20                                           *
;* uses temp0..temp3                                          *
;* temp0 = x-position                                         *
;* temp1 = y-position                                         *
;* returns out of bounds as a nonzero value in temp3          *
;* returns bitmask in w                                       *
;**************************************************************

;20 clocks
brickposcheck   mov   w,#$F8         ;1   get illegal x-positions
      and   w,temp0         ;1   mask out illegal x-bits for x-position
      mov   temp3,w         ;1   store illegal bits for later
      mov   w,#$F0         ;1   get illegal y-positions
      and   w,temp1         ;1   mask out illegal y-bits for y-position
      or   temp3,w         ;1   combine iillegal x- and y- bits and store in temp3
      mov   w,<>temp0      ;1   get x-position and swap nibbles
      and   w,#$60         ;1   only keep former bit 1 and 2
      add   w,temp1         ;1   add y-position
      or   w,#$90         ;1   set bit 4 and 7 to get correct address
      mov   fsr,w         ;1   set file select register to calculated pointer
      mov   w,#$F0         ;1   get bitmask for left brick
      snb   temp0.0         ;1(2)   check if x-pos is odd
      mov   w,#$0F         ;1   yes, get bitmask for right brick instead
      ret            ;3   get outa here


;************************* setbrick ***************************
;* Sets a brick on the position temp0, temp1 with color temp3 *
;* clock cycles: 34                                           *
;* uses temp0..temp3                                          *
;* temp0 = x-position                                         *
;* temp1 = y-position                                         *
;* temp2 = color                                              *
;**************************************************************

setbrick   call   brickposcheck      ;20   calc address, check out of bounds and get bitmask
      test   temp3         ;1   out of bounds ? (1/2)
      sz            ;1(2)   out of bounds ? (2/2)
      jmp   delay9         ;3   yes, out of bounds, do delayed return
      and   ind,w         ;1   clear wanted nibble
      mov   w,temp2         ;1   get color
      snb   temp0.0         ;1   check if x is odd
      mov   w,<>temp2      ;1   yes, get color with swapped nibbles instead
      or   ind,w         ;1   set color
      ret            ;3   get outa here


;************************ checkbrick **************************
;* Checks if there is a brick on the position temp0, temp1    *
;* returns nonxzero value of there is a brick and zero if the *
;* position is clear                                          *
;* clock cycles: 33                                           *
;* uses temp0..temp3                                          *
;* temp0 = x-position                                         *
;* temp1 = y-position                                         *
;**************************************************************

checkbrick   call   brickposcheck      ;20
      test   temp3         ;1   out of bounds ? (1/2)
      sz            ;1(2)   out of bounds ? (2/2)
      jmp   delay8         ;3   yes, out of bounds, do delayed return
      not   w         ;1   invert bits to get wanted nibble
      and   w,ind         ;1   get wanted nibble from playfield
      ret            ;3   get outa here


;*********************** checksetblock *************************
;* If bit 0 in temp6 is set then the the block in blockbuff    *
;* is drawn in the playfiled on position x,y with color temp2  *
;* using 221 clocks                                            *
;* If bit 0 in temp6 is clear then the the block in blockbuff  *
;* is checked for collitions on position x,y in the playfield  *
;* returning result in temp7 using 217 clocks                  *
;* tempregs 0..7 are used                                      *
;* The reason of combining these two operations is that they   *
;* are very similar, combining them will save program mem      *
;* checkblock calls checksetblock with temp6.0 cleared (221+1) *
;* setblock calls checksetblock with temp6.0 set (223+1)       *
;***************************************************************

checkblock   clrb   temp6.0         ;1   set checking (clear setting)
      skip            ;2   don't set setting
setblock   setb   temp6.0         ;1   set setting
checksetblock   clr   temp7         ;1   clear result
      mov   temp4,#blockbuff   ;2   point temp4 at block buffer
      mov   temp5,#4      ;2   each block has 4 bricks

setblock_l   mov   fsr,temp4      ;2   set file select register to block buffer pointer
      mov   w,x         ;1   get block base x-position
      add   w,ind         ;1   add relative brick position
      mov   temp0,w         ;1   store brick x-position
      inc   fsr         ;1   point at next buffer x-position
      mov   w,y         ;1   get block base y-position
      add   w,ind         ;1   add relative brick y-position
      mov   temp1,w         ;1   store brick y-position
      sb   temp6.0         ;1(2)   
      call   checkbrick      ;33   
      snb   temp6.0         ;1(2)   
      call   setbrick      ;34   
      or   temp7,w         ;1   store result from check
      add   temp4,#2      ;2   update buffer pointer to next brick
      decsz   temp5         ;1(2)   decrease brick counter
      jmp   setblock_l      ;3   keep loopin until all 4 bris are out

      bank   $00
      retp            ;3   get outa here

jumpstart   pjmp   start



;********************** simplecolorfa *************************
;* outputs w color cycles at (almost) maximum amplitude       *
;* Clocks: w*12 + 11 + 1                                      *
;**************************************************************

simplecolorfa   mov   temp2,w         ;1
      mov   temp0,#63      ;2
      mov   temp1,#black      ;2
      skip            ;2


;*********************** simplecolor **************************
;* outputs w color cycles                                     *
;* Clocks: w*12 + 5 + 1                                       *
;**************************************************************

simplecolor   mov   temp2,w         ;1   set colorcycle counter
simplecolor_l   voutr   temp0         ;2   set first level
      delay   4         ;4   delay to get 12cycle loop
      voutr   temp1         ;2   set second level
      decsz   temp2         ;1(2)   decrease colorcycle counter
      jmp   simplecolor_l      ;3   do all cycles
      retp            ;3   get outa here

;************************ makeblock ***************************
;* Get compressed coordinates from rom and genereate rotated  *
;* uncompressed coordinates in block buffer in ram            *
;* Clocks: 204+1                                              *
;**************************************************************

mbexpand   and   w,#3         ;1   mask out current block
      add   pc,w         ;3   get to correct value
      retw   0         ;3   return value
      retw   1         ;3   return value   
      retw   2         ;3   return value   
      retw   -1         ;3   return value   

makeblock   bank   $00         ;1   set bank 0
      mov   w,<<kind      ;1   relative address = kind*2
      call   blocks         ;9   get block x-data from table
      mov   temp0,w         ;1   store inn x-data temporary register (temp0)
      mov   w,<<kind      ;1
      or   w,#1         ;1   relative address = kind*2 + 1
      call   blocks         ;9   get block y-data from table
      mov   temp1,w         ;1   store inn y-data temporary register (temp1)
      mov   fsr,#blockbuff      ;2   point at block buffer
      mov   w,angle         ;1   get angle
      and   w,#3         ;1   limit angle to 0..3
      mov   temp4,w         ;1   store in local angle
      mov   w,kind         ;1   what kind of block do we have ?
      and   w,#%00000111      ;1   check lower bits of kindword
      snz            ;1(2)   zero ? (non rotatable square)
      clr   temp4         ;1   yes, do not rotate, set angle to zero
      mov   w,>>temp4      ;1   get bit2 of angle
      xor   w,temp4         ;1   xor it with bit2
      mov   temp2,w         ;1   and store result in temp2 (temp2.0 is set for angle 1&2)
      sb   temp4.0         ;1(2)   if (angle = 0) or (angle = 2)
      jmp   mbnoswap      ;3   then don't swap x and y
      mov   w,temp1         ;1   else do swap x and y by xoring
      xor   w,temp0         ;1   .
      xor   temp1,w         ;1   .
      xor   temp0,w         ;1   .
mbnoswapc   mov   temp3,#4      ;2   4 bricks in each block, set counter to 4
      clr   temp4         ;1   a register vcontaining zero is needed for later

makeblock_l   mov   w,temp0         ;1   get x-data
      call   mbexpand      ;10
      snb   temp2.1         ;1   if (angle = 2) or (angle = 3)
      mov   w,temp4 - w      ;1   then mirror angle
      mov   ind,w         ;1   store in buffer
      inc   fsr         ;1   point at next position in buffer
      mov   w,temp1         ;1   get y-data
      call   mbexpand      ;10
      snb   temp2.0         ;1   if (angle = 1) or (angle = 2)
      mov   w,temp4 - w      ;1   then mirror angle
      mov   ind,w         ;1   store in buffer
      inc   fsr         ;1   point at next position in buffer
      rr   temp0         ;1   temp0 = temp0 >> 2
      rr   temp0         ;1   .
      rr   temp1         ;1   temp1 = temp1 >> 2
      rr   temp1         ;1   .
      decsz   temp3         ;1(2)   decrease brick counter
      jmp   makeblock_l      ;3   keep looping until all bricks are done

      bank   $00         ;1   set bank 0
      retp            ;3   get outa here

mbnoswap   jmp   mbnoswapc      ;3   time portal to get 6 clocks if x and y are not swapped

;************************* start ******************************
;* Start sequence, sets up system                             *
;**************************************************************

start      


      clr   fsr
clr_l      setb   fsr.4
      clr   ind
      incsz   fsr
      jmp    clr_l

      bank   $20
      mov   musictimer,#1

      bank   $00
      mov   joytimer,#JTIME   ;2
      mov   x,#4
      mov   y,#2
      mov   kind,#$12
      mov   falltimer,#50

      mode   $F
      mov   !RB,#%11000001
      mov   !RC,#%11100000
      mode   $E
      mov   !RA,#%0000
      mov   !RB,#%00111110
      mov   !RC,#%00011111
      
      bank   NEXTGFX
      mov   NEXTGFX,#$9F
      mov   NEXTGFX+1,#$F9
      
      bank   $20
      pjmp   main

;*********************** updatemusic **************************
;* Music player, called once per frame, playing "karboshka"   *
;* from rom, each position of the tune is stored as a note    *
;* followed by a length. The length is multyplied with the    *
;* gamespeed making it play faster with the speed of the game *
;* Each note is translated to a "frequency" from a table in   *
;* ROM, notes are separated with a short (2 frames) pause     *
;* clocks: 130 + 1                                            *
;**************************************************************

updatemusic   bank   $20      ;1
      decsz   musictimer   ;1   decrease note/pause length timer
      jmp   musicnochnote   ;3   if not zero, don't update note info

      snb mixedbits.gameoverbit   ;1   no music if gameover
      jmp   musicpausep

      bank   $60      ;1
      mov   w,sfreq_l   ;1
      or   w,sfreq_h   ;1   is frequency zero ?

      sz         ;1   yes, don't make a pause
      jmp   musicpause   ;3   no, make a pause (i.e. set freq to zero for two frames)

      bank   $20      ;1
      call   readsong   ;17   get next nibble of the song from rom, the note (i.e position in frequency table)
      bank   $60      ;1
      mov   temp0,w      ;1   temp0 = 1*freqtablepos
      add   temp0,w      ;1   temp0 = 2*freqtablepos
      add   temp0,w      ;1   temp0 = 3*freqtablepos
      call   readfreqtbl   ;16   get bit 0..3 from rom
      mov   sfreq_l,w   ;1   store in high byte of frequency
      inc   temp0      ;1   point at next position in rom
      call   readfreqtbl   ;16   get bit 4..7 from rom
      swap   sfreq_l      ;1   swap nibbles to be ready for next nibble
      or   sfreq_l,w   ;1   or the two nibbles together
      swap   sfreq_l      ;1   swap nibbles to get back correct order of nibbles
      inc   temp0      ;1   point at next position in rom
      call   readfreqtbl   ;16   get bit 8..11 from rom
      mov   sfreq_h,w   ;1   store in high byte of frequency

      bank   $20      ;1
      mov   temp1,#11   ;2   temp1 = 11
      sub   temp1,(SCORE+1)   ;2   temp1 = 11-speed = note baselength
      call   readsong   ;17   get next nibble of the song from rom, the notelength
      mov   temp0,w      ;1   put notelength in temp0 to be able to do tests
      mov   w,>>temp1   ;1   w = (11-speed)/2
      test   temp0      ;1   update flags according to notelength
      clc         ;1   clear carry to prevent a set carrybiit pollution of the speed multyplier
      sz         ;1   check if notelenth is larger than zero
      rl   temp1      ;1   temp1 = (11-speed)*2
      snb   temp0.1      ;2(4)   check if notelength is 1
      add   temp1,w      ;1   temp1 = speed*2 + speed/2 = speed * 2.5
      mov   w,temp1      ;1   w = temp1 = lengthmultiplier (1, 2 or 2.5)
      mov   musictimer,w   ;1   set notelength to (11-speed)*lengthmultiplier
      
      mov   w,songpos   ;1   get song position
      xor   w,#104      ;1   xor with songlength
      snz         ;1   if result is zero then we have reached the end of the song
      clr   songpos      ;1   and should restart the song
      pjmp   main      ;4   get back to main


musicnochnote   delay   130-12      ;   delay to keep timing when no change of frequency
      pjmp   main      ;4   get back to main

musicpausep   bank   $60
      delay   3

musicpause   clr   sfreq_l      ;1   clear low byte of rfrequency
      clr   sfreq_h      ;1   clear high byte of frequency
      clr   pos_l      ;1
      clr   pos_h      ;1

      bank   $20      ;1   
      mov   musictimer,#2   ;2   pause is for two cycles
      delay   130-25      ;   delay to keep timing when setting pause
      pjmp   main      ;4   get back to main

;*********************** vrealsound ***************************
;* vrealsound is called from vsync and calls realsound every  *
;* second vsync cycle as vsync is called at twice the speed   *
;* as sync, so vrealsound is dependent of realsound           *
;* clocks: 39 + 1                                             *
;**************************************************************

vrealsound   sb   temp2.0      ;1(2)
      jmp   realsound   ;35
      delay   35-1-3      
      retp         ;3

;************************ realsound ***************************
;* realsound is called from hsync to output sound data to the *
;* sound DA, the sound is based on a 16bit sin signal in rom. *
;* clocks: 35 + 1                                             *
;**************************************************************

realsound   mov   w,fsr               ;1
      bank   $60               ;1
      mov   stemp0,w            ;1
      mov   m,#((SINTABLE+gamedata) >> 8)      ;1   point at corrent page for sintable
      add16   pos,sfreq            ;6   update sintable position according to speed
      and   pos_h,#31            ;2   keep sample position in range 0..31
      mov   temp0,pos_h            ;2   get high part i wave position
      add   temp0,#((SINTABLE+gamedata )& $FF)   ;2   add low part of pointer to sintable and position
      mov   w,temp0               ;1   the sum, the low pointer should be in w
      iread                  ;4   read from rom
      mov   w,m               ;1   get high nibble, i.e w=sin(pos)
      mov   temp0,w               ;1   temp0 = 7 + sin(pos)
      add   temp0,#7            ;2
      mov   w,<<temp0            ;1   w = (7 + sin(pos))*2
      mov   audio,w               ;1   output value to audio DA
      mov   fsr,stemp0            ;2
      retp                  ;3



PALETTE_BCW   EQU $0      ;word-mem

gamedata2   dw $808,$80f,$816,$81d,$824,$82b,$832,$839,$f3f,$67f,$d7f,$3bf,$abf,$1ff,$8ff,$fff

PALETTE_PAGE   EQU (( gamedata2 + PALETTE_BCW)>>8)

ORG   $200

;************************* vsync ******************************
;* Performas a vertical sync pattern on the video output      *
;* Uses temp0..temp2                                          *
;**************************************************************

vsync      mov   w,#4         ;1   odd, make 5 pulses instead
      call   short_sync      ;5   clocks until sync, make those pulses, 
      mov   temp2,w         ;1   counter0=5
long_sync_l   clr   video         ;1   set video level to sync
      delay   (TIME_27US5 - 1)      ;   30uS long sync pulse
      vout   black         ;2   set video level to black
      call   vsound         ;43
      delay   (TIME_4US5 - 6 - 43)   ;   2us long black pulse
      decsz   temp2         ;1(2)
      jmp   long_sync_l      ;3
      mov   w,#5         ;1   odd, make 4 pulses instead of 5
short_sync   mov   temp2,w         ;1
short_sync_l   clr   video         ;1   set video level to sync
      call   vsound         ;43
      delay   (TIME_2US4 - 43 - 1)      ;2us long sync pulse
      vout   black         ;2   set video level to black
      delay   (TIME_29US6 - 6)      ;   30us long black pulse
      decsz   temp2         ;1(2)
      jmp   short_sync_l      ;3
      retw   5         ;3
vsound      pjmp   vrealsound      ;40         ;3


/*

BLAH
*/

;************************* hsync ******************************
;* performas a horizontal sync pulse and color burst          *
;* uses temp0                                                 *
;**************************************************************

hsync      delay   TIME_PRESYNC-3-1   ;85   
      clr   video         ;1

      call   sound         ;39
      delay   TIME_SYNC-2-39
      vout   neutral         ;2   

      delay   TIME_PREBURST-2      ;44   
      mov   temp0,#12      ;2   
hsyncl      vout   6         ;2   
      delay   4         ;4   
      vout   21         ;2   
      decsz   temp0         ;1(2)   
      jmp   hsyncl         ;3   
      delay   2         ;2
      vout   neutral         ;2   
      delay   time_postburst - 2-3   ;114   
      retp            ;3   
sound      pjmp   realsound      ;36



;*********************** emptylines ***************************
;* Displays w empty lines, 17clocks until hsync when called   *
;* and 12 clocks until next hsync when returned               *
;**************************************************************

emptylines   mov   temp3,w
emptylines_l   delay   13         ;13
      call   hsync         ;
      delay   (TIME_IMAGE-4-13)   ;
      decsz   temp3         ;1(2)
      jmp   emptylines_l      ;3
      ret            ;3


;******************** brickcolortable *************************
;* function to get phase and amplitude of block w             *
;**************************************************************

brickcolortable   add   pc,w         ;3
      retw   (black)<<2      ;3 black
      retw   $D0         ;3 purple-pink
      retw   $A6         ;3 green-cyan
      retw   $A6         ;3 green-cyan
      retw   $C1         ;3 red
      retw   $F2         ;3 orange
      retw   $F3         ;3 yellow
      retw   $F4         ;3 green
      retw   $E7         ;3 blue


;*********************** main loop ****************************
;* This is the game main loop                                 *
;**************************************************************

main      bank   $20
      clr   linecnt
      clr   gfxcnt
      call   vsync      ;   vertical sync, frame starts here
      
   
      snb mixedbits.gameoverbit
      pjmp gameover

      call   hsync      ;643   first line starts here

;------------ Remove block from playfield ------------- 430 clocks

      pcall   makeblock   ;205   create current block in buffer
      clr   temp2      ;1   set color 0
      pcall   setblock   ;224   set black block at current position, to remove block from playfield

;---------------- Handle block falling ---------------- 987 clocks

      decsz   falltimer   ;1(2)   decrease falltimer
      jmp   nofall      ;3   if falltimer hasn't reached zero, then don't fall
      bank   $20      ;1   set bank 1
      inc   rnd      

      mov   temp0,#10   ;2
      sub   temp0,SCORE+1   ;2   
      mov   w,<<temp0   ;1
      bank   $00      ;1   set bank 0
      mov    falltimer,w   ;1   set falltimer to gamespeed
      rl   falltimer   ;1

      inc   y      ;1   move block one step down
      pcall   checkblock   ;222   check if there was anything in the way
      test   temp7      ;1   was fall possible ? (1/2)
      snz         ;1(2)   was fall possible ? (2/2)
      jmp   fallok      ;3   yes, continue

      bank   $00      ;1   set bank 0
      mov   w,kind      ;1   set color of current block
      and   w,#%00000111   ;1
      mov   temp2,w      ;1
      inc   temp2      ;1
      dec   y      ;1   
      pcall   setblock   ;224   place the block on playfield
      mov   temp0,nextkind   ;2   store next kind in a tempreg to be able to transer to store in kind later
      bank   $20      ;1   set bank1

      xor   w,rnd      ;1   compare nextkind with rnd
      snz         ;1   equal ?
      inc   rnd      ;1   yes, boring, increase rnd to avoid same block twice

      mov   w,rnd      ;1   get a new nextblock
      and   w,#7      ;1
      
      bank   $00      ;1   set bank0
      mov   nextkind,w   ;1   

      mov   w,temp0      ;1   get old nextkind
      and   w,#%11111000   ;1   clear lower bits just leaving the scrollposition
      or   nextkind,w   ;1   combine it with the new nextkind

      mov   w,#%11111000   ;1
      and   kind,w      ;1
      mov   w,temp0      ;1
      and   w,#%00000111   ;1
      or   kind,w      ;1

      clr   angle      ;1   set angle to zero
      mov   y,#2      ;2   set y-position to 2
      mov   x,#4      ;2   set x-position to 4
      pcall   makeblock   ;205   decompress the new block
      pcall   checkblock   ;222
      
      bank   $20
      test   temp7      ;1
      sz         ;1(2)
      setb   mixedbits.gameoverbit
      pcall   incpoints   ;60
      bank   $00      ;1
      jmp   donefall   ;3   and we are done with handle of fall

nofall      delay   242-4      ;235   there was no fall, delay to get timing correct
fallok      delay   987-242      ;732   fall was ok so we don't need to restore fall, delay to get timing correct
donefall   

;-------------- Handle joystick motion ---------------- 23 clocks

      pcall   readjoy1   ;13
      not   w      ;1
      snz         ;1(2)
      jmp   nojoy      ;3
      xor   w,oldjoy   ;1
      snz         ;1(2)
      jmp   joyok      ;3
      decsz   joytimer   ;1(2)
      jmp   nojoytimeout   ;3
joyok      mov   joytimer,#JTIME   ;2

;----------------- Joystick down ? -------------------- 3 clocks

      mov   w,#1      ;1   prepare end of fall
      sb   joy1down   ;1(2)   joy down ?
      mov   falltimer,w   ;1   set falltimer to end of fall

;----------------- Joystick left ? -------------------- 231 clocks

      snb   joy1left   ;1(2)   check if joystick moved left
      jmp   noleft      ;3   if not, do nothing
      dec   x      ;1   else try to move block one step left
      pcall   checkblock   ;222   check if there was anything in the way
      test   temp7      ;1   was motion possible ? (1/2)
      sz         ;1(2)   was motion possible ? (1/2)
      inc   x      ;1   no, retract move
      jmp   doneleft   ;3   we are done here

noleft      delay   231-4      ;226   no left motion, wait to get timing correct
doneleft

;----------------- Joystick right ? -------------------- 231 clocks

      snb   joy1right   ;1(2)   check if joystick moved right
      jmp   noright      ;3   if not, do nothing
      inc   x      ;1   else try to move block one step right
      pcall   checkblock   ;222   check if there was anything in the way
      test   temp7      ;1   was motion possible ? (1/2)
      sz         ;1(2)   was motion possible ? (1/2)
      dec   x      ;1   no, retract move
      jmp   doneright   ;3   we are done here

noright      delay   231-4      ;226   no right motion, wait to get timing correct
doneright
      

;----------------- Joystick button ? -------------------- 436 clocks

      snb   joy1button   ;1(2)   check if joystick buttton pressed
      jmp   norotate   ;3   if not, do nothing
      inc   angle      ;1   else try to rotate block

      pcall   makeblock   ;205
      pcall   checkblock   ;222   check if there was anything in the way
      test   temp7      ;1   was rotation possible ? (1/2)
      sz         ;1(2)   was rotation possible ? (1/2)
      dec   angle      ;1   no, retract rotation
      jmp   donerotate   ;3   we are done here

nojoy      delay   5
nojoytimeout   delay   433+231+231+3+23-18-(433-4)
norotate   delay   436-4
donerotate
;------------ place block in playfield -------------

      pcall   makeblock   ;205

      delay   TIME_IMAGE-430-987-23-3-231-231-436-205

;*********************** Line 2 ****************************

      call   hsync      ;643


;---------------- Find full line ---------- 1032 clocks

      mov   temp2,#15   ;2      examine 15 lines (all except for the most upper one)
      mov   fsr,#$9F   ;2      start at bottom left corner of playfield
      clr   temp4      ;1      clear emptyline memory
fline0      mov   temp1,#4   ;2      set byte counter to 4 bytes (per line)
      mov   temp0,#8   ;2      set brick counter to 8 bricks (per line)

fline1      mov   w,ind      ;1      get byte from playfield
      and   w,#$F0      ;1      mask out one nibble
      sz         ;1(2)      if nibble is not zero
      dec   temp0      ;1      then decrease brick counter
      mov   w,ind      ;1      get byte from playfield
      and   w,#$0F      ;1      mask out the other nibble
      sz         ;1(2)      if nibble is not zero
      dec   temp0      ;1      then decrease brick counter
      add   fsr,#$20   ;2      point one byte right
      decsz   temp1      ;1      decrease byte counter
      jmp   fline1      ;3      do all (four) bytes

      mov   w,temp2      ;1      get line number
      test   temp0      ;1      how many nibbles was not zero ? (1/2)
      snz         ;1(2)      was all nibbles non zero then line is full
      mov   temp4,w      ;1      then remember this as a full line (store line number)
      add   fsr,#-$81   ;2      point one line up and 4 bytes left
      decsz   temp2      ;1(2)      decrease line counter
      jmp   fline0      ;3      do all (15) lines

      mov   temp1,temp4   ;2      line number of empty line is number of lines to move
      mov   w,--temp4   ;1      get line number of the line above the one to remove
      add   fsr,w      ;1      point at that line
      not   temp4      ;1      temp4 = ~linenumbers
      add   temp4,#15+1   ;2      temp4 = lines left = 15 + (1 + ~line number) = 15 - line number
      test   temp1      ;1      check if there are lines to move
      snz         ;1(2)      if no lines to move

;---------------- Remove full line ----------- 731 clocks

      jmp   nomline      ;3      then don't move any lines

mline0      mov   w,#4      ;1      4 bytes per line
      mov   temp3,w      ;1      4 bytes on this line to be moved
      mov   temp5,w      ;1      4 bytes on upper line to move (this will be used later on in the code)

mline1      mov   w,ind      ;1      get one byte of line
      inc   fsr      ;1      get to next line
      mov   ind,w      ;1      store byte at next line
      add   fsr,#$1F   ;2      point one byte left and one line up
      decsz   temp3      ;1(2)      decrease inne loop counter
      jmp   mline1      ;3      do all four bytes

      add   fsr,#-$81   ;2      point one line up
      decsz   temp1      ;1(2)      decrease outer loop counter
      jmp   mline0      ;3      do all lines

      mov   w,#$20      ;1      $20 steps between bytes to clear
clrul      clr   ind      ;1      clear byte
      add   fsr,w      ;1      next byte
      decsz   temp5      ;1(2)      decrease byte counter
      jmp   clrul      ;3      do all 4 bytes

      pcall   tenpoints   ;56

      jmp   mlinec      ;3      skip delay

nomline      delay   731-653      ;731-(43*15+3+1+2+2)

mlinec      test   temp4      ;1      are there lines to remove ?
      snz         ;1(2)
      jmp   nodline      ;3      

dline0      delay   43-4      ;38      delay 5 to get 9 clocks in loop to get same length as mline1 loop
      decsz   temp4      ;1(2)      decrease delay counter
      jmp   dline0      ;3      do all delay lines
      delay   2      ;2      2 clocks to fast, compensate
nodline

;--------------- create next block graphics ------------

      pcall   makenext   ;184

;-------------- remaining delay to fill line ----------- 244 clocks
      bank   $00         ;1
      mov   w,kind         ;1   set color
      and   w,#%00000111      ;1
      mov   temp2,w         ;1
      inc   temp2         ;1
      
      pcall   readjoy1      ;13
      mov   oldjoy,w      ;1

      pcall   setblock      ;224   put block at current position on playfield
      bank   $00         ;1

      delay   TIME_IMAGE - 731 -1032 - 184 - 244 - 17 - 1

;************************ Lines 3..64 ************************

bfgovr      mov   w,#PRE_LINES  -2 + TOP_LINES       ;1
      call   emptylines            ;   do empty lines at top outside of screen

      delay   12-5
      ITEXT   STR0,STR0_LEN,STR0_BASE,STR0_PHASE   
      delay   TIME_IMAGE - 17-1

      mov   w,#STRTOCAP_LINES
      call   emptylines

      delay   12-6
      
      mov   temp5,#11            ;2
      mov   temp3,#((gamedata + CAP) & $FF)      ;2
      mov   temp4,#((gamedata + CAP) >> 8)      ;2
cap_l      call   hsync               ;643
      delay   (CAP_BASE) - (CAP_PHASE) - 9 - 347 - 7
      mov   temp0,temp3            ;2
      mov   temp1,temp4            ;2
      mov    temp2,#((PALETTE_BCW + gamedata2) & $FF);2
      mov   fsr,#VIDEO_BUFFER         ;2
      mov   w,#11               ;1
      pcall   setgraphics            ;11*31+5+1 = 347
      mov   temp3,temp0            ;2
      mov   temp4,temp1            ;2
      mov   fsr,#VIDEO_BUFFER         ;2
      mov   w,#11               ;1
      pcall   memtovideo            ;140
      delay   CAP_SEP - 140 - 2 -1 + (CAP_PHASEDIFF)
      mov   fsr,#VIDEO_BUFFER         ;2
      mov   w,#11               ;1
      pcall   memtovideo            ;140
      delay   (TIME_IMAGE) - (CAP_BASE) + (CAP_PHASE) - (CAP_SEP) - 140 - 4 - (CAP_PHASEDIFF)
      decsz   temp5               ;1(2)
      jmp   cap_l               ;3

      mov   temp7,#16            ;2
field_l      call   hsync

      pcall   leftgfx

;163
      mov   temp0,#$A0      ;2      set temp0 to bottom left corner of playfield
      sub   temp0,temp7      ;2      subtract linecounter to get top left
      mov   temp1,#VIDEO_BUFFER+8   ;2      set temp1 to playfield buffer
      mov   temp3,#4      ;2      set field read counter to 4
fieldread_l   mov   fsr,temp0      ;2      set fsr to playfield pointer
      mov   temp2,ind      ;2      read two blocks of playfield
      mov   fsr,temp1      ;2      set fsr to video buffer pointer
      mov   w,temp2         ;1      get playfield data
      and   w,#$F         ;1      mask out left block color
      call   brickcolortable      ;9      get phase and amplitude of color
      mov   ind,w         ;1      store phase and color in video buffer
      inc   fsr         ;1      update local pointer to point at next position in video buffer
      mov   w,<>temp2      ;1      get blocks in swapped order
      and   w,#$F         ;1      mask out right block color
      call   brickcolortable      ;9      get phase and amplitude of color
      mov   ind,w         ;1      store phase and color in video buffer
      inc   temp1         ;1      update real video buffer pointer
      inc   temp1         ;1      update real video buffer pointer (again)
      add   temp0,#$20      ;2      move playfield pointer one step right
      decsz   temp3         ;1(2)      decrease field read counter
      jmp   fieldread_l      ;3      loop until entire line is read
      clr   fsr         ;1      as the fsr was manipulated, reset it back first page

      delay   (((BRICK_WIDTH + 2)*8*12)+5+1+ 7+11+11)-163

      pcall   rightgfx

      delay   TIME_IMAGE - TIME_RIGHTGFX - TIME_LEFTGFX - (((BRICK_WIDTH + 2)*8*12)+5+1+ 7+11+11) - 2
      
      mov   temp6,#BRICK_LINES
linel      call   hsync         ;643   11
            
      pcall   leftgfx
      delay   11-GAMEFIELD_PHASE
      bank   $20         ;1
      mov   w,temp7         ;1   get blockline
      and   w,#%1110      ;1   dont care about even or odd
      xor   w,#%1000      ;1   check if line is 6 or 7
      sz            ;1(2)   
      jmp   nogameover      ;3   if not don't show gameover
      page   showgameover      ;1   prepare page for jmp
      snb   mixedbits.gameoverbit   ;1(2)   check if game is over
      jmp   showgameover      ;3   if so, show game over
      skip            ;(2)
nogameover   jmp   nogameover2      ;3   
nogameover2   

      mov   fsr,#VIDEO_BUFFER+8   ;2
      mov   temp0,#BRICK_WIDTH   ;2   
      mov   temp1,#black      ;2   
      mov   w,#8         ;1   
      pcall   outputcol      ;((BRICK_WIDTH + 2)*8*12)+5+1
showgameoverret
      delay   GAMEFIELD_PHASE
      pcall   rightgfx

      delay   TIME_IMAGE - TIME_RIGHTGFX - TIME_LEFTGFX - (((BRICK_WIDTH + 2)*8*12)+5+1+ 7+11+11) - 4



      decsz   temp6      ;1(2)
      jmp   linel      ;3
      delay   2      

      call   hsync
      pcall   leftgfx   

      delay   (((BRICK_WIDTH + 2)*8*12)+5+1+ 7+11+11)
      pcall   rightgfx
      delay   TIME_IMAGE - TIME_RIGHTGFX - TIME_LEFTGFX - (((BRICK_WIDTH + 2)*8*12)+5+1+ 7+11+11)

      call   hsync
      pcall   leftgfx
      delay   (((BRICK_WIDTH + 2)*8*12)+5+1+ 7+11+11)   
      pcall   rightgfx
      delay   TIME_IMAGE - TIME_RIGHTGFX - TIME_LEFTGFX - (((BRICK_WIDTH + 2)*8*12)+5+1+ 7+11+11) - 4 - 1

      page   field_l
      decsz   temp7
      jmp   field_l
      delay   2

      call   hsync

      pjmp   nextmain

;*********************** outputcol ****************************
;* shows w number of colorfields, each BLOCKWIDTH cycles wide *
;* (this is the thing that shows the colors in the playfield) *
;*                                                            *
;* number of clocks: ((temp0 + 2) * w * 12) + 5               *
;* used tempregs: 0..5                                        *
;*                                                            *
;* input:                                                     *
;*   w = number of fields                                     *
;*   temp0 = fieldlength, must be odd (phase lost when even)  *
;*   temp1 = neutral level                                    *
;*   fsr = pointer to field data (contens is destroyed)       *
;* output:                                                    *
;*   none                                                     *
;*                                                            *
;* local use of tempregs:                                     *
;*   temp2 is used as color loop counter                      *
;*   temp3 is used as field loop counter                      *
;*   temp4 is used as temp storage for intensity calculations *
;*   temp5 is used as temp storage for phase                  *
;*                                                            *
;* comments:                                                  *
;*   This routine is optimized to get as short gaps between   *
;*   fields as possible, these optimizations assumes some     *
;*   limitations of the input data to be able to get the gap  *
;*   down to only 2 color cycles (24 clocks)                  *
;*   Field color is stored as bytes, where each byte has      *
;*   bit 0..2 as phase bit and bit 2..5 is intensity, note    *
;*   that this means that there is and overlap of phase and   *
;*   intensity                                                *
;*   Remaining phase = 7-phase = 7+(-phase) = 7+(/phase+1) =  *
;*   -1+/phase+1 = /phase = phase xor 7 (assuming 3 bit calc) *
;*   this calculation (xor by 7) is done in the inner loop as *
;*   it was the only place where there was free clock cycles  *
;*   so to get a correct result the inner loop needs to be    *
;*   executed an odd number of times, which makes the field-  *
;*   length required to be an odd number or else phase will   *
;*   be lost.                                                 *
;*   The x in the Phase comment field is the phase value read *
;*   it can be values 0..6                                    *
;*                                                            *
;**************************************************************

                  ;Clocks   Phase   Comment
outputcol   mov   temp3,w         ;1   9   set field counter
ocolx1      mov   w,ind         ;1   10   get phase and intensity
      and   w,#7         ;1   11   mask out phase
      setphase 7         ;3+7-x   0   set phase
      mov   temp5,w         ;1   10-x   remember phase for later
      mov   temp2,temp0      ;2   11-x   set color loop counter
ocolx0      vout   black         ;2   1-x   set first half to black level
      xor   temp5,#7      ;2   3-x   invert all bits in phase
      mov   w,>>ind         ;1   5-x   get color and phase shifted right one step
      mov   temp4,w         ;1   6-x   store shifted value in a tempreg
      mov   w,>>temp4      ;1   7-x   shift value one more step right   
      mov   video,w         ;1   8-x   and set video output to the intensity
      decsz   temp2         ;1(2)   9-x   decrease color cycle counter
      jmp   ocolx0         ;3   10-x   and loop until all color cycles are done
      voutr   temp1         ;2   11-x   set neutral level
      mov   w,temp5         ;1   1-x   get remaining phase = 7-phase = phase xor 7
      setphase 7         ;3+x   2-x   set remaining phase to return to original phase
      inc   fsr         ;1   5+x-x   point at next field byte
      decsz   temp3         ;1(2)   6   decrease field cycle counter
      jmp   ocolx1         ;3   7   and loop until all fields are done
      retp            ;3   8   return back home


;************************* leftgfx ****************************
;* handle graphics at left side of playfield                  *
;**************************************************************

leftgfx      delay   LEFTGFX_BASE

      bank   $20               ;1   select bank $20 to be able to read linecnt
      mov   w,<>linecnt            ;1   get line number with swapped nibbles
      and   w,#$F               ;1   mask out most significant nibble of linecount to get section number
      jmp   pc+w               ;3   jump to correct section of 16 lines
      jmp   textnext_line            ;3   text "NEXT "
      jmp   nextblock_line            ;3   bricks preview
      jmp   nextblock_line            ;3   bricks preview
      jmp   nextblock_line            ;3   bricks preview
      jmp   black_line            ;3   black lines between preview and points text
      jmp   textscore_line            ;3   text "POINTS"
      jmp   showpoints_line            ;3   display points
      jmp   black_line            ;3   black lines at the bottom
      jmp   black_line            ;3   black lines at the bottom
      jmp   black_line            ;3   black lines at the bottom
      jmp   black_line            ;3   black lines at the bottom
      jmp   black_line            ;3   black lines at the bottom
      jmp   black_line            ;3   black lines at the bottom

black_line   delay   TIME_LEFTGFX - LEFTSCREW_PHASE - LEFTGFX_BASE - 9 - 4 - 3 - 3 - 104

showscrew   mov   fsr,#VIDEO_BUFFER         ;2
      mov   w,#8               ;1
      pcall   memtovideo            ;104   output left screw graphics
      delay   LEFTSCREW_PHASE
      retp


;************************ rightgfx ****************************
;* handle graphics at right side of playfield                 *
;**************************************************************

rightgfx   delay   11-RIGHTSCREW_PHASE
      mov   fsr,#VIDEO_BUFFER         ;2
      mov   w,#8               ;1
      pcall   memtovideo            ;104   output right screw graphics
      bank   $20               ;1
      inc   linecnt               ;1   update linecounter

      mov   temp1,#((gamedata + SCREW) >> 8)   ;2   set page of graphics
      mov   w,<>linecnt            ;1   get linenumber with swapped nibbles (multiplied by 16)
      and   w,#$70               ;1   reboove unwanted bits to get (line%8)*16
      mov   temp0,w               ;1   store in temp0
      clc                  ;1   clear carry
      rr   temp0               ;1   rotate right to get (line%8)*8
      add   temp0,#((gamedata + SCREW) & $FF)   ;2   add graphics base
      snc                  ;1(2)   check for page overflow
      inc   temp1               ;1   point at next page
      mov    temp2,#((PALETTE_BCW + gamedata2) & $FF) ;2   point at correct palette
      mov   fsr,#VIDEO_BUFFER         ;2   point at video buffer position where to store graphics
      mov   w,#8               ;1   graphics is 8 pixels wide
      pcall   setgraphics            ;254   translate graphics into

      delay   TIME_RIGHTGFX + RIGHTSCREW_PHASE -11 - 104 - 21 - 254 - 4 - 3
      retp                  ;3


;************************* strout *****************************
;* output characters from string in rom using a font in rom   *
;* temp0 used as character temp storage                       *
;* temp2 used as character counter                            *
;* temp1:temp3 = pointer to string                            *
;* temp4 = line (0..7) + FONT_BASE                            *
;* temp5 = length                                             *
;* clocks: 8 * 12 * w + 44 + 1                                 *
;**************************************************************

strout_cl


strout_l   vout   black            ;2                  pixel three to seven
      delay   2            ;2   
      rr   temp0            ;1   
      snc               ;1(2)   
      mov   w,#53            ;1   
      mov    video,w
      decsz   temp2            ;1(2)   
      jmp   strout_l         ;3   

strout      mov   m,temp1            ;2   set character page

      vout   black            ;2                  pixel one starts here

      mov   w,temp3            ;1   get pointer to characters
      iread               ;4   read one character
      add   w,temp4            ;1   update according to line and fontbase
      mov   m,#((gamedata + FONT) >> 8)   ;1   set font-page
      iread               ;4   read character pixels from font
      mov   temp0,w            ;1   store character pixels in temp0

      mov   w,#black         ;1                  
      rr   temp0            ;1
      snc               ;1(2)   
      mov   w,#53            ;1   
      inc   temp3            ;1   point at next character
      mov   video,w            ;1   

      snz               ;1(2)   
      inc   temp1            ;1
      delay   2
      vout   black            ;2                  pixel three starts here

      mov   temp2,#5         ;2
      rr   temp0            ;1
      snc               ;1(2)   
      mov   w,#53            ;1
      mov   video,w            ;1   
      decsz   temp5            ;1(2)
      jmp   strout_cl         ;3
      vout   black
      retp               ;3


;************************ incpoints ***************************
;* add one point to score                                     *
;* clocks: 59+1                                               *
;**************************************************************

incpoints   mov   fsr,#SCORE+3         ;2
      mov   temp0,#4         ;2

incpoints_l   inc   ind            ;1
      mov   w,#%1010         ;1
      xor   w,ind            ;1
      sz               ;1(2)
      jmp   nocarry_l         ;3
      clr   ind
      dec   fsr            ;1
      decsz   temp0            ;1(2)
      jmp   incpoints_l         ;3
      delay   7            ;7
      retp               ;3

nocarry_l   delay   7
      decsz   temp0            ;1(2)
      jmp   nocarry_l         ;3
      retp


;************************ tenpoints ***************************
;* add ten points to score                                    *
;* note: this routine requires incpoints                      *
;* clocks: 56                                                 *
;**************************************************************

tenpoints   mov   fsr,#SCORE+2         ;2
      mov   temp0,#3         ;2
      jmp   incpoints_l         ;3


;************************ textlines ***************************
;* routine to output line with text in leftgraphics field     *
;* clocks: 537                                                *
;**************************************************************

textlines   mov   w,linecnt            ;1   get line number
      and   w,#%1100            ;1(2)   check bit 2,3
      sz                  ;1   if bits are zero, don't do next test
      xor   w,#%1100            ;1   toggle bit 2,3 to check if values are %11
      snz                  ;1(2)   if bits are %00 or %11
      jmp   notext               ;3   then lines should be empty

      mov   temp4,#((gamedata + FONT) & $ff) - 4   ;2   set temp4 to fontbase - 4 (compensating line starting at 4)
      mov   w,linecnt            ;1   get linecounter
      and   w,#$F               ;1   get last significant nibble (sectionline)
      add   temp4,w               ;1   update pointer according to line number
      mov   temp5,#6            ;2   always output 6 characters
      jmp   strout               ;525   output text
                        

notext      delay   526               ;523   delay to keep timing correct if no text
      ret                  ;3   get back to left graphics and show screw


;************************ makenext ****************************
;* create graphics for next block to be used in leftgraphics  *
;* clocks: 183 + 1                                            *
;**************************************************************

xsel      and   w,#3         ;1 remove unwanted bits
      jmp   pc+w         ;3 select correct returnvalue
      retw   2         ;3 bit 1 
      retw   4         ;3 bit 2 
      retw   8         ;3 bit 3 
      retw   1         ;3 bit 0 

ysel      and   w,#3
      jmp   pc+w
      retw   (NEXTGFX   & $7F) | $80
      retw   (NEXTGFX+1 & $7F) 
      retw   (NEXTGFX+1 & $7F) | $80
      retw   (NEXTGFX   & $7F) 

makenext   bank   NEXTGFX         ;1
      clr   NEXTGFX         ;1
      clr   NEXTGFX+1      ;1
      bank   $00         ;1
      clc            ;1
      mov   w,<<nextkind      ;1
      pcall   blocks         ;10
      mov   temp0,w         ;1
      mov   w,<<nextkind      ;1
      or   w,#1         ;1
      call   blocks         ;9
      page   makenext_l      ;1
      mov   temp1,w         ;1
      mov   temp3,#4      ;2

makenext_l   mov   w,temp0         ;1
      call   xsel         ;10
      mov   temp2,w         ;1
      mov   w,temp1         ;1
      call   ysel         ;10
      mov   fsr,w         ;1
      snb   fsr.7         ;1(2)
      swap   temp2         ;1
      clrb   fsr.7         ;1
      or   ind,temp2      ;2
      rr   temp0         ;1
      rr   temp0         ;1
      rr   temp1         ;1
      rr   temp1         ;1
      decsz   temp3         ;1(2)
      jmp   makenext_l      ;3
      retp            ;3

;********************** textnext_line *************************
;* handles field with text "next" in leftgraphics             *
;**************************************************************

textnext_line   delay   TEXTNEXT_BASE-TEXTNEXT_PHASE      ;   delay to set phase of text
      mov   temp3,#((STR2 + gamedata) & $FF)   ;2   set lower pointer to "Next" text
      mov   temp1,#((STR2 + gamedata) >> 8)      ;2   set upper pointer to "Next" text
      call   textlines            ;537   output 6 charcters to left field
      delay   TIME_LEFTGFX - LEFTSCREW_PHASE - LEFTGFX_BASE - 9 - 4 - 3 - 3 - 104 - 3 - 544 - TEXTNEXT_BASE + TEXTNEXT_PHASE
      jmp showscrew               ;3   get back to left graphics and show screw


;********************** textscore_line ************************
;* handles field with text "score" in leftgraphics            *
;**************************************************************

textscore_line   delay   TEXTSCORE_BASE-TEXTSCORE_PHASE      ;   delay to set phase of text
      mov   temp3,#((STR3 + gamedata) & $FF)   ;2   set lower pointer to "Score" text
      mov   temp1,#((STR3 + gamedata) >> 8)      ;2   set upper pointer to "Score" text
      call   textlines            ;537   output 6 charcters to left field
      delay   TIME_LEFTGFX - LEFTSCREW_PHASE - LEFTGFX_BASE - 9 - 4 - 3 - 3 - 104 - 3 - 544 - TEXTSCORE_BASE + TEXTSCORE_PHASE
      jmp showscrew               ;3   get back to left graphics and show screw


;********************** showpoints_line ***********************
;* handles field with score in leftgraphics                   *
;**************************************************************

showpoints_line   delay   SCORE_BASE-SCORE_PHASE

      mov   w,linecnt            ;1   get line number
      and   w,#$0F               ;1   get last significant nibble (sectionline)
      mov   temp4,w               ;1   store sectionline in text line register (temp4)

      mov   temp1,#4            ;2   set character counter
      mov   fsr,#SCORE            ;2   point att score
      add   temp4,#((gamedata + NUMBERS) & $FF)   ;2   add pointer to font to linenumber

stroutp_cl   mov   m,#((gamedata + NUMBERS) >> 8)      ;1   set font-page
      mov   w,<>ind               ;1   get digit x 16
      inc   fsr               ;1   point at next digit
      add   w,temp4               ;1   point at correct line in font (according to current sectionline)
      snc                  ;1(2)   detect page overflow
      mov   m,#(((gamedata + NUMBERS) >> 8)+1)   ;1   if overflow set next font-page
      iread                  ;4   read character pixels from font
      mov   temp0,w               ;1   store character pixels in temp0
      mov   temp2,#8            ;2   each character is 7 pixels wide
      delay   7               ;7   delay to keep phase

stroutp_l   vout   black               ;2   set video output to black level
      delay   2               ;2   delay to make colorcycle 12 clocks   
      rr   temp0               ;1   rotate font data
      snc                  ;1(2)   check if bit set, if not keeep black leevel in w
      mov   w,#53               ;1   else if bit set set gigit intensity
      mov   video,w               ;1   output selected level to video output
      decsz   temp2               ;1(2)   decrease pixel counter
      jmp   stroutp_l            ;3   loop until all pixels are done
      vout   black               ;2   set video level to black

      decsz   temp1               ;1(2)   decreasee digit counter
      jmp   stroutp_cl            ;3   loop until all digs are done

      delay   TIME_LEFTGFX - LEFTSCREW_PHASE - LEFTGFX_BASE - 9 - 4 - 3 - 3 - 104 - 3 - 487 - SCORE_BASE+SCORE_PHASE
      jmp showscrew               ;3   get back to left graphics and show screw


;********************** nextblock_line ************************
;* handles fields showing next block                          *
;**************************************************************

nextblock_line   delay   NBLOCK_BASE-NBLOCK_PHASE
      mov   w,gfxcnt            ;1   get gfxccounter
      and   w,#$0C               ;1   check if line is 0..2 of the 12-line brick
      snz                  ;1(2)   if not, get on with the brick drawing
      jmp   nonextg               ;3   if one of the first lines, then it should be black

      mov   temp0,gfxcnt            ;2
      bank   NEXTGFX               ;1

      mov   w,NEXTGFX            ;1   get graphics for next block
      snb   temp0.5               ;1(2)   check if brickline second half
      mov   w,NEXTGFX+1            ;1   yes, it was, get next next graphics
      mov   temp1,w               ;1   store in temp1
      snb   temp0.4               ;1(2)   check if blickline is odd
      swap   temp1               ;1   yes, swap nibbles

      bank   $00               ;1   set bank for next kind
      mov   w,++nextkind            ;1   get next kind of block
      and   w,#%00001111            ;1
      pcall   brickcolortable            ;10   translate kind to phase and amplitude
      page   nextl0                ;1   brickcolortabe destroy page register, restore it
      mov   temp2,w               ;1   store phase and amplitude in temp2
      mov   temp3,#4            ;2   we have 4 brick positions to convert
      mov   fsr,#VIDEO_BUFFER + $28         ;2   point at beginning of buffer

nextl0      mov   w,temp2               ;1   get phase and amplitude of next block
      rr   temp1               ;1   rotate block data to get next bit
      sc                  ;1(2)   check if bit was set
      mov   w,#(black)<<2            ;1   if not set black intensity
      mov   ind,w               ;1   store phase and color in buffer
      inc   fsr               ;1   point at next buffer position
      decsz   temp3               ;1(2)   decrease brick counter
      jmp   nextl0               ;3   keep looping until all bricks are done

      mov   fsr,#VIDEO_BUFFER + $28         ;2   point at beginning of buffer
      mov   temp0,#7            ;2   each brick is 7 cycles 
      mov   temp1,#black            ;2   set black level (between bricks)
      mov   w,#4               ;1   we have 4 brick positions
      pcall   outputcol            ;438   output colors to video output
      jmp   donenextg            ;3

nonextg      delay   512               ;508   empty line, delay to keep timing

donenextg   bank   $20               ;1
      inc   gfxcnt               ;1   increase counter
      mov   w,gfxcnt            ;1   get counter value
      and   w,#3               ;1   check bit 0 and bit 1
      sz                  ;1(2)   if not zero upper part was not increased
      setb   gfxcnt.0            ;1   if zero, jump to one by setting bit 0

      delay   TIME_LEFTGFX - LEFTSCREW_PHASE - LEFTGFX_BASE - 9 - 4 - 3 - 3 - 104 - 3   - 524 - NBLOCK_BASE+NBLOCK_PHASE   ;   delay to make points section 551 cycles
      jmp showscrew               ;3   get back to left graphics and show screw      


;*********************** showgameover *************************
;* show "GAME OVER" text in playfield                         *
;**************************************************************

showgameover   delay   GAMEOVER_BASE-GAMEOVER_PHASE      ;
      mov   temp3,#((STR5 + gamedata) & $FF)   ;2   set lower pointer to "Game" text
      mov   temp1,#((STR4 + gamedata) >> 8)      ;2   set upper pointer to "Over" text
      mov   w,#((STR4 + gamedata) & $FF)      ;1   get lower address to "Over"
      snb   temp7.0               ;1(2)   check if section is odd
      mov   temp3,w               ;1   odd line, set lower pointer to "Over" text
      mov   temp4,#((gamedata + FONT -1 +8) & $ff)    ;2   set temp4 to fontbase - 4 (compensating line starting at 4)
      mov   w,temp6               ;1   get linecounter
      and   w,#7               ;1   get last significant nibble (sectionline)
      snb   temp6.3               ;1(2)   check if linenr 1arger than 7
      jmp   emptygoverl            ;3   if found, skip it

      sub   temp4,w               ;1   update pointer according to line number
      mov   temp5,#5            ;1   always output 4 characters
      call   strout               ;429   output text
      delay   ((BRICK_WIDTH + 2)*8*12)+35-444-26+GAMEOVER_PHASE-GAMEOVER_BASE
      pjmp   showgameoverret            ;4   get back to mainloop

emptygoverl   delay   ((BRICK_WIDTH + 2)*8*12)+35-15-26+GAMEOVER_PHASE-GAMEOVER_BASE
      pjmp   showgameoverret            ;4   get back to mainloop

gameover   pcall   hsync
      bank   $00
      snb   joy1button            ;1(2)   check if joystick buttton pressed
      jmp   nogovb               ;3   if not, do nothing
      page   start               ;1   
      sb   joy1button_old            ;1(2)   
      jmp   start               ;3   
nogovb      
      pcall   readjoy1            ;13
      mov   oldjoy,w            ;1

      delay   TIME_IMAGE-1-20
      pcall   hsync
      delay   TIME_IMAGE-1-4
      pjmp   bfgovr


nextmain   delay   12-BLINE_PHASE
      mov   w,#(TIME_IMAGE-1-12-1-12-1-4) / 12
      pcall   simplecolorfa

      delay   ((TIME_IMAGE-1-12-1-12-1-4) // 12) + BLINE_PHASE
      pcall   hsync

      delay   (TIME_IMAGE - 5)

      ITEXT   STR1,STR1_LEN,STR1_BASE,STR1_PHASE   

      delay   (TIME_IMAGE-1)

      pcall   hsync

      delay   4+12-BLINE_PHASE
      mov   w,#(TIME_IMAGE-1-12-4-1-18-1) / 12
      pcall   simplecolorfa
      delay   ((TIME_IMAGE-1-12-4-1-18-1) // 12) + BLINE_PHASE

      mov   w,#POST_LINES + VISILINES - STRTOCAP_LINES - PLAYFIELD_LINES - 11 - 1 - 9 -1 - 10
      pcall   emptylines

      bank   $20
      inc   rnd

      pjmp   updatemusic      ;131


;************************* gamedata ***************************
;* Game data: graphics, music, wavetables etc...              *
;**************************************************************

ORG   $600

gamedata
dw $000,$000,$200,$300,$300,$400,$300,$000,$000,$000,$200,$47e,$400,$500,$400,$100   ; $000..$00f
dw $000,$200,$400,$700,$800,$a18,$418,$200,$21c,$436,$963,$c7f,$963,$863,$463,$200   ; $010..$01f
dw $27f,$446,$c16,$91e,$616,$646,$47f,$200,$23c,$466,$903,$603,$673,$466,$25c,$100   ; $020..$02f
dw $263,$477,$67f,$67f,$46b,$263,$163,$000,$063,$267,$46f,$47b,$373,$363,$163,$000   ; $030..$03f
dw $03e,$063,$063,$063,$063,$263,$03e,$000,$03f,$066,$066,$03e,$036,$066,$067,$200   ; $040..$04f
dw $43c,$266,$00c,$018,$030,$066,$03c,$000,$07e,$27e,$45a,$a18,$318,$218,$03c,$000   ; $050..$05f
dw $063,$063,$063,$263,$463,$a36,$b1c,$a00,$363,$263,$036,$01c,$036,$263,$463,$a00   ; $060..$06f
dw $c00,$a00,$61e,$430,$33e,$233,$06e,$000,$200,$400,$c3e,$a63,$603,$663,$43e,$300   ; $070..$07f
dw $238,$030,$23e,$433,$a33,$c33,$a6e,$600,$600,$500,$43e,$263,$17f,$203,$43e,$900   ; $080..$08f
dw $a00,$600,$66e,$533,$533,$43e,$230,$11f,$218,$400,$51c,$518,$518,$518,$53c,$400   ; $090..$09f
dw $407,$206,$166,$036,$21e,$536,$567,$400,$400,$400,$437,$27f,$16b,$06b,$06b,$000   ; $0a0..$0af
dw $200,$200,$23b,$266,$266,$166,$166,$000,$000,$000,$13e,$363,$463,$563,$63e,$600   ; $0b0..$0bf
dw $700,$700,$73b,$66e,$606,$506,$40f,$300,$100,$000,$f7e,$d03,$c3e,$b60,$a3f,$a00   ; $0c0..$0cf
dw $90c,$90c,$93f,$a0c,$a0c,$b6c,$c38,$d00,$f00,$100,$833,$033,$133,$933,$06e,$a00   ; $0d0..$0df
dw $900,$000,$d63,$a6b,$06b,$27f,$c36,$000,$a00,$d00,$063,$736,$e1c,$036,$363,$000   ; $0e0..$0ef
dw $130,$018,$03e,$063,$07f,$003,$03e,$000,$03e,$041,$059,$045,$045,$059,$041,$03e   ; $0f0..$0ff
dw $050,$068,$008,$058,$088,$0d0,$0c0,$098,$0c8,$000,$0f8,$000,$048,$010,$028,$0d8   ; $100..$10f
dw $0b0,$0f0,$088,$000,$0e0,$0e0,$0e0,$0e0,$010,$0c0,$098,$078,$0a0,$070,$0c0,$080   ; $110..$11f
dw $010,$090,$0d8,$0b0,$088,$088,$010,$078,$0b8,$0a8,$000,$038,$138,$088,$0e8,$0d0   ; $120..$12f
dw $000,$000,$050,$050,$078,$0b8,$0c0,$088,$000,$028,$028,$318,$030,$020,$400,$040   ; $130..$13f
dw $040,$060,$020,$048,$600,$000,$03c,$07e,$be7,$0e7,$0e7,$de7,$0e7,$0e7,$0e7,$0e7   ; $140..$14f
dw $0e7,$0e7,$0e7,$ee7,$07e,$13c,$160,$078,$07e,$37e,$070,$070,$570,$070,$070,$070   ; $150..$15f
dw $a70,$170,$b70,$d70,$170,$070,$038,$07c,$0e6,$4e6,$1e6,$1e6,$0f0,$270,$070,$378   ; $160..$16f
dw $138,$23c,$01c,$10e,$0fe,$0fe,$23e,$27f,$0e7,$4e7,$1e7,$3e0,$07c,$2fc,$0e0,$1e7   ; $170..$17f
dw $2e7,$2e7,$0e7,$3e7,$17f,$43e,$1f8,$2f8,$1f8,$0fc,$1fc,$0ec,$1ee,$5ee,$2e6,$6e7   ; $180..$18f
dw $0ff,$7ff,$1ff,$6e0,$0e0,$5e0,$0ff,$4ff,$207,$207,$077,$4ff,$1e7,$3e7,$0e0,$2e0   ; $190..$19f
dw $0e7,$1e7,$2e7,$2e7,$07e,$33c,$13c,$47e,$1e7,$2e7,$107,$07f,$1ff,$0e7,$1e7,$5e7   ; $1a0..$1af
dw $2e7,$6e7,$0e7,$7e7,$17e,$63c,$0fe,$5fe,$0e0,$4e0,$2e0,$260,$070,$470,$170,$330   ; $1b0..$1bf
dw $038,$238,$038,$138,$21c,$21c,$03c,$37e,$1e7,$4e7,$1e7,$2e7,$1e7,$07e,$1ff,$0e7   ; $1c0..$1cf
dw $1e7,$0e7,$0e7,$0e7,$07e,$03c,$03c,$07e,$0e7,$0e7,$0e7,$0e7,$0e7,$0e7,$0e7,$0ff   ; $1d0..$1df
dw $0fe,$0e0,$0e7,$0e7,$07e,$03c   ; $1e0..$1e5

