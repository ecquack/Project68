#asm
;            PONG
;      (C) Rickard Gunée 1998
;
;   This is shareware, use it at your own risk.
;
;   You will find more info about the project
;   and more info on video signals here:
;   http://www.rickard.gunee.com/projects
;
;   Outputs composite NTSC video signal using
;   only two resistors!


   include   P16F628A.INC

   __CONFIG 0x3F2A

up1b      equ   3
down1b      equ   2
left1b      equ   4
right1b      equ   5
fire1b      equ   1
up2b      equ   7
down2b      equ   6
left2b      equ   2
right2b      equ   3
fire2b      equ   1
up1p      equ   PORTB
down1p      equ   PORTB
left1p      equ   PORTB
right1p      equ   PORTB
fire1p      equ   PORTB
up2p      equ   PORTB
down2p      equ   PORTB
left2p      equ   PORTA
right2p      equ   PORTA
fire2p      equ   PORTA

pl1_points   equ   0x20
pl2_points   equ   0x21
pl1_ypos   equ   0x22
pl2_ypos   equ   0x23
ball_xpos   equ   0x24
ball_ypos   equ   0x25
counter0   equ   0x26
counter1   equ   0x27
counter2   equ   0x28
videostuff   equ   0x29
line      equ   0x2A
pl         equ   0x2B
counter5   equ   0x2C
delaycnt   equ   0x2D
ball_xpart   equ   0x2E
ball      equ   0x2F
counter3   equ   0x30
counter4   equ   0x31
sound_counter   equ   0x32
sound_stuff   equ   0x33
sound_counter2   equ   0x34
stuff      equ   0x35
strptr      equ   0x36
strptr2      equ   0x37
stringbuff   equ   0x38

x_dir      equ   0
x_speed      equ   1
y_dir      equ   2
y_speed      equ   3
serve      equ   4
wait      equ   5

sound_length   equ   0x3



delay      MACRO
      LOCAL   label
      movwf   delaycnt
label      decfsz   delaycnt,f
      goto label
      ENDM

dnop      MACRO
      LOCAL   label
label      goto   label+1
      ENDM




      org 0x000
      bcf   STATUS,RP0
      bcf   STATUS,RP1
      MOVLW 0x07    ;Turn comparators off and
      MOVWF CMCON   ;enable pins for I/O

      goto menu


table
numbers      addwf   PCL,f
;number  0 
      retlw 0x1C
      retlw 0x36
      retlw 0x63
      retlw 0x6B
      retlw 0x63
      retlw 0x36
      retlw 0x1C
      retlw 0x0
;number  1 
      retlw 0x18
      retlw 0x1C
      retlw 0x18
      retlw 0x18
      retlw 0x18
      retlw 0x18
      retlw 0x7E
      retlw 0x0
;number  2 
      retlw 0x3E
      retlw 0x63
      retlw 0x60
      retlw 0x38
      retlw 0xC
      retlw 0x66
      retlw 0x7F
      retlw 0x0
;number  3 
      retlw 0x3E
      retlw 0x63
      retlw 0x60
      retlw 0x3C
      retlw 0x60
      retlw 0x63
      retlw 0x3E
      retlw 0x0
;number  4 
      retlw 0x38
      retlw 0x3C
      retlw 0x36
      retlw 0x33
      retlw 0x7F
      retlw 0x30
      retlw 0x78
      retlw 0x0
;number  5 
      retlw 0x7F
      retlw 0x3
      retlw 0x3
      retlw 0x3F
      retlw 0x60
      retlw 0x63
      retlw 0x3E
      retlw 0x0
;number  6 
      retlw 0x1C
      retlw 0x6
      retlw 0x3
      retlw 0x3F
      retlw 0x63
      retlw 0x63
      retlw 0x3E
      retlw 0x0
;number  7 
      retlw 0x7F
      retlw 0x63
      retlw 0x30
      retlw 0x18
      retlw 0xC
      retlw 0xC
      retlw 0xC
      retlw 0x0
;number  8 
      retlw 0x3E
      retlw 0x63
      retlw 0x63
      retlw 0x3E
      retlw 0x63
      retlw 0x63
      retlw 0x3E
      retlw 0x0
;number  9 
      retlw 0x3E
      retlw 0x63
      retlw 0x63
      retlw 0x7E
      retlw 0x60
      retlw 0x30
      retlw 0x1E
      retlw 0x0

;P
      retlw 0x3F
      retlw 0x66
      retlw 0x66
      retlw 0x3E
      retlw 0x6
      retlw 0x6
      retlw 0xF
      retlw 0x0
;L
      retlw 0xF
      retlw 0x6
      retlw 0x6
      retlw 0x6
      retlw 0x46
      retlw 0x66
      retlw 0x7F
      retlw 0x0
;. 
      retlw 0x0
      retlw 0x0
      retlw 0x0
      retlw 0x0
      retlw 0x0
      retlw 0x18
      retlw 0x18
      retlw 0x0
; W
      retlw 0x63
      retlw 0x63
      retlw 0x63
      retlw 0x6B
      retlw 0x6B
      retlw 0x7F
      retlw 0x36
      retlw 0x0
; O
      retlw 0x3E
      retlw 0x63
      retlw 0x63
      retlw 0x63
      retlw 0x63
      retlw 0x63
      retlw 0x3E
      retlw 0x0
; N
      retlw 0x63
      retlw 0x67
      retlw 0x6F
      retlw 0x7B
      retlw 0x73
      retlw 0x63
      retlw 0x63
      retlw 0x0
; G
      retlw 0x3C
      retlw 0x66
      retlw 0x3
      retlw 0x3
      retlw 0x73
      retlw 0x66
      retlw 0x5C
      retlw 0x0
 
      ; C
      retlw 0x3C   ;0xC3
      retlw 0x42   ;0x99
      retlw 0x99   ;0xFC
      retlw 0x85   ;0xFC
      retlw 0x85   ;0xFC
      retlw 0x99   ;0x99
      retlw 0x42   ;0xC3
      retlw 0x3C   ;0xFF
;R
      retlw 0x3F
      retlw 0x66
      retlw 0x66
      retlw 0x3E
      retlw 0x36
      retlw 0x66
      retlw 0x67
      retlw 0x0
; U
      retlw 0x63
      retlw 0x63
      retlw 0x63
      retlw 0x63
      retlw 0x63
      retlw 0x63
      retlw 0x3E
      retlw 0x0
; E
      retlw 0x7F
      retlw 0x46
      retlw 0x16
      retlw 0x1E
      retlw 0x16
      retlw 0x46
      retlw 0x7F
      retlw 0x0
; space
      retlw 0x0
      retlw 0x0
      retlw 0x0
      retlw 0x0
      retlw 0x0
      retlw 0x0
      retlw 0x0
      retlw 0x0
; H
      retlw 0x63
      retlw 0x63
      retlw 0x63
      retlw 0x7F
      retlw 0x63
      retlw 0x63
      retlw 0x63
      retlw 0x0
; C
      retlw 0x3C
      retlw 0x66
      retlw 0x3
      retlw 0x3
      retlw 0x3
      retlw 0x66
      retlw 0x3C
      retlw 0x0

l_0   EQU   0x00
l_1   EQU   0x08
l_2   EQU   0x10
l_3   EQU   0x18
l_4   EQU   0x20
l_5   EQU   0x28
l_6   EQU   0x30
l_7   EQU   0x38
l_8   EQU   0x40
l_9   EQU   0x48
l_P   EQU   0x50
l_L   EQU   0x58
l_dot   EQU   0x60
l_W   EQU   0x68
l_O   EQU   0x70
l_N   EQU   0x78
l_G   EQU   0x80
l_CC   EQU   0x88
l_R   EQU   0x90
l_U   EQU   0x98
l_E   EQU   0xA0
l_space   EQU   0xA8
l_H   EQU   0xB0
l_C   EQU   0xB8
      
dosound      btfss   sound_stuff,0
      goto   skipsound
      bsf   PORTA,4
      btfsc   sound_counter,2
      bcf   PORTA,4
      incf   sound_counter,f
      return
skipsound   dnop
      nop
      return

shortsync   movwf   counter1                 // w=6 into counter1
shortsync_l0   bcf   PORTA,0      ;2us sync  // clear sync bit
      bcf   PORTB,0                          // clear data bit
      dnop
      movlw   0x1D      ;30us black          // load loop counter2 with 30
      movwf   counter2
      nop
      bsf   PORTA,0                          // set sync bit
shortsync_l1   decfsz   counter2,f           // loop counter2
      goto   shortsync_l1
      decfsz   counter1,f                    // loop counter1
      goto   shortsync_l0
      retlw   5



vertsync   movlw   5
      btfss   videostuff,0
      movlw   6
      call   shortsync
      incf   videostuff,f

longsync   movwf   counter1
longsync_l0   movlw   0x1D
      movwf   counter2
      bcf   PORTA,0      ;30 us sync
      bcf   PORTB,0   
longsync_l1   decfsz   counter2,f
      goto   longsync_l1
      nop         ;2us black
      bsf   PORTB,0
      nop
      decfsz   counter1,f
      goto   longsync_l0

      movlw   5
      btfss   videostuff,0
      movlw   4
      call   shortsync
      return

start      
      movlw   sound_length
      movwf   sound_counter2
      bcf   sound_stuff,0
      clrf   sound_counter

      clrf   pl1_points
      clrf   pl2_points
      clrf   counter4

      movlw   0x50
      movwf   pl1_ypos
      movlw   0x50
      movwf   pl2_ypos
      movlw   10
      movwf   ball_xpos
      movwf   ball_ypos
      movlw   20
      movwf   ball

      movlw   0xFE
      tris   PORTB

      clrf   videostuff
      bsf   stuff,0
      
main      call   vertsync

      dnop
      bcf   PORTA,0         ;sync
      bcf   PORTB,0

      movfw   pl1_points
      sublw   0xA
      skpnz
      goto   plwon
      movfw   pl2_points
      sublw   0xA
      skpnz
      goto   plwon

      nop
      dnop

      bsf   PORTA,0         ;set black level

      movlw   0x14
      subwf   ball_ypos,w
      skpc
      movlw   0
      movwf   counter0
      movlw   0x82 - 9
      subwf   counter0,w
      skpnc 
      movlw   0x82 - 9
      skpnc
      movwf   counter0
      movfw   counter0
      btfsc   stuff,1   
      movwf   pl1_ypos
      btfsc   stuff,2
      movwf   pl2_ypos
      dnop   

      bsf   PORTA,0         ;set white
      btfsc   ball,wait
      goto   waitfire
      btfsc   ball,x_speed      ;if speed is high
      goto   highxspeed      ;then skip slowtest
      btfsc   videostuff,0      ;slowtest
      goto   skipball
      nop
highxspeedr   btfsc   ball,x_dir      ;check xdir
      goto   goleft
      incf   ball_xpos,f      ;move ball right
      movfw   ball_xpos
      sublw   0x29         ;bounce ?
      skpz
      goto   norbounce
      bsf   ball,x_dir      ;yes, set left motion
      decf   ball_xpos,f      ;if bounce also prevent ball from getting outa here
      bsf   sound_stuff,0
      movfw   ball_ypos
      addlw   0B
      subwf   pl2_ypos,w
      skpnc
      goto   rmiss1
      movfw   pl2_ypos
      addlw   0x35 - 8
      subwf   ball_ypos,w      ;ball_ypos - (pl2_ypos + 50)
      skpnc
      goto   rmiss2
      bsf   ball,x_dir      ;change ball direction
      btfsc   fire2p,fire2b
      goto   nofirer
      bsf   ball,x_speed      ;if fire increase speed
      btfsc   up2p,up2b
      bcf   ball,y_dir
      btfsc   down2p,down2b
      bsf   ball,y_dir
      bsf   ball,y_speed
      dnop
      bsf   sound_stuff,0
goleftr      btfsc   ball,y_dir
      goto   goup
      incf   ball_ypos,f
      movfw   ball_ypos
      sublw   0xBD - 0x23
      skpnz
      bsf   ball,y_dir
      skpnz
      bsf   sound_stuff,0
      skpnz
      decf   ball_ypos,f
      nop
goupr      movlw   0x09
      delay
      nop
skipballr   movlw   2         ;move bars 2 pixels / frame
      movwf   counter2
movebarl   btfss   up1p,up1b      ;player 1 pressed up ?
      decf   pl1_ypos,f      ;yes, move up player 1
      btfss   down1p,down1b      ;player 1 pressed down ?
      incf   pl1_ypos,f      ;yes, move down player 2
      movfw   pl1_ypos      ;get player 1 ypos
      skpnz            ;ypos = 0 ? (oob)
      incf   pl1_ypos,f      ;yes, move down one step
      sublw   0x93 - 0x23 + 9
      skpnz            ;ypos=150 ? (oob)
      decf   pl1_ypos,f      ;yes, move up one step
      btfss   up2p,up2b      ;player 2 pressed up ?
      decf   pl2_ypos,f      ;yes, move up player 1
      btfss   down2p,down2b      ;player 2 pressed down ?
      incf   pl2_ypos,f      ;yes, move down player 2
      movfw   pl2_ypos      ;get player 2 ypos
      skpnz            ;ypos = 0 (oob)
      incf   pl2_ypos,f      ;yes, move down one step
      sublw   0x93 - 0x23 + 9
      skpnz            ;ypos=150 ? (oob)
      decf   pl2_ypos,f      ;yes, move up one step
      decfsz   counter2,f
      goto   movebarl

      btfss   ball,y_speed      ;fast y-speed ?
      goto   yslow         ;yes, skip it
      btfsc   ball,y_dir      ;ball going up ?
      goto   fastup         
      incf   ball_ypos,f      ;ball going down !
      movfw   ball_ypos
      sublw   0xBD - 0x23      ;check for bounce
      skpnz
      bsf   ball,y_dir      ;if bounce, change y-dir
      skpnz
      bsf   sound_stuff,0
      skpnz
      decf   ball_ypos,f      ;and go up one step
      nop
fastr      nop

      btfss   sound_stuff,0      ;sound ?
      goto   nosoundatall
      decfsz   sound_counter2,f      ;decrease timer
nosoundatall   goto   nosoundfin      ;if timer is zero then
      bcf   sound_stuff,0      ;stop sound
      movlw   sound_length      ;setup time for next time
      movwf   sound_counter2
      bsf   PORTA,4         ;set high level to decrease power consumption
nosoundfinr   
      movlw   2
      delay
      
      clrf   line
      movlw   0x32 - 8
      call   emptylines
   
      dnop
      dnop
      dnop
      nop
nocmprr      
      movlw   0xC8 - 0x23   ;200 lines
      movwf   counter1
l6      movfw   line
      subwf   pl1_ypos,w   ;w=ypos-line, if w>0 then c=1
      bcf   PORTA,0      ;start sync
      bcf   PORTB,0
      skpnc
      goto   nolpl      ;if c=1 then line < ypos, don't show it
      movfw   pl1_ypos
      addlw   0x32 - 8
      subwf   line,w      ;w=line-(ypos+50)
      bsf   pl,0      ;if c=0 then line < (ypos+50), show it
      skpnc
nolplr      bcf   pl,0      ;if c=1 then line > (ypos+50), don't show it
      nop
      nop   
      bsf   PORTA,0      ;end of sync, set black
      movfw   line
      subwf   pl2_ypos,w   ;w=ypos-line, if w>0 then c=1
      skpnc
      goto   norpl      ;if c=1 then line < ypos, don't show it
      movfw   pl2_ypos
      addlw   0x32 - 8
      subwf   line,w      ;w=line-(ypos+50)
      bsf   pl,1      ;if c=0 then line < (ypos+50), show it
      skpnc
norplr      bcf   pl,1      ;if c=1 then line > (ypos+50), don't show it
      nop
      movfw   ball_ypos
      subwf   line,w      ;w=ball_ypos - line
      skpc         ;line < ball_ypos
      goto   noball      ;yes, ball is not on line            30
      movfw   ball_ypos
      addlw   0x0A
      subwf   line,w      ;w=line-(ball_ypos+9)
      skpc         ;line < ball_ypos+9 ?
      goto   ballonline   ;yes: ball is on this line            35
noballr      dnop
      dnop
      nop
      btfsc   pl,0
      bsf   PORTB,0
      dnop   
      dnop
      movlw   0x2A-3
      bcf   PORTB,0
      delay
      call   dosound
      nop
      btfsc   pl,1
      bsf   PORTB,0
      dnop
      dnop
      nop
      bcf   PORTB,0
      dnop
      dnop
endofball   incf   line,f
      decfsz   counter1,f
      goto   l6                        ;191

      movlw   0x3
      movwf   counter1
l8      nop
      bcf   PORTA,0
      bcf   PORTB,0
      movlw   3
      delay
      bsf   PORTA,0
      movlw   6
      delay
      bsf   PORTB,0
      movlw   0x33
      delay
      nop
      decfsz   counter1,f
      goto   l8


      dnop
      bcf   PORTA,0
      bcf   PORTB,0
      movlw   3
      delay
      bsf   PORTA,0
      movlw   6
      delay
      bsf   PORTB,0
      movlw   0x2E
      delay

      movfw   pl1_points
      movwf   counter2
      rlf   counter2,f
      rlf   counter2,f
      rlf   counter2,f
      movfw   pl2_points
      movwf   counter0
      rlf   counter0,f
      rlf   counter0,f
      rlf   counter0,f
      dnop

      movlw   0x00
      tris   PORTB

      clrf   counter4
      clrf   line
      movlw   0x10
      movwf   counter1
l9      dnop
      bcf   PORTA,0
      bcf   PORTB,0
      movlw   3
      delay
      nop
      bsf   PORTA,0
      nop
      movlw   3
      delay
      
      call   dosound

      bsf   PORTB,0

      movlw   3
      delay

      movfw   counter2
      addlw   -4         ;?!?!?!?!?!!
      call   numbers
      btfsc   ball,serve
      xorlw   0xFF

      movwf   PORTB
      rrf   PORTB,f
      rrf   PORTB,f
      rrf   PORTB,f
      rrf   PORTB,f
      rrf   PORTB,f
      rrf   PORTB,f
      rrf   PORTB,f
      bsf   PORTB,0
      movlw   0x1C
      delay
      nop
      
      movfw   counter0
      call   numbers
      btfss   ball,serve
      xorlw   0xFF

      movwf   PORTB
      rrf   PORTB,f
      rrf   PORTB,f
      rrf   PORTB,f
      rrf   PORTB,f
      rrf   PORTB,f
      rrf   PORTB,f
      rrf   PORTB,f
      bsf   PORTB,0

      btfsc   counter4,0
      incf   counter2,f
      btfsc   counter4,0
      incf   counter0,f
      incf   counter4,f
      movlw   4
      delay
      decfsz   counter1,f
      goto   l9

      movlw   0xFE
      tris   PORTB
      nop

      bcf   PORTA,0
      bcf   PORTB,0
      movlw   3
      delay
      bsf   PORTA,0
      movlw   6
      delay
      bsf   PORTB,0
      movlw   0x32
      delay
      nop

      movlw   0x1E - 7
      call   emptylines

      goto   main

nolpl      goto   nolplj
nolplj      goto   nolplr

norpl      goto   norplj
norplj      goto   norplr

ballonline   dnop            ;screen starts here
      dnop
      btfsc   pl,0         ;start showing player 1
      bsf   PORTB,0
      movfw   ball_xpos
      sublw   0x29
      movwf   ball_xpart
      nop
      movfw   ball_xpos
      bcf   PORTB,0
      delay
      bsf   PORTB,0   
      dnop
      movfw   ball_xpart
      bcf   PORTB,0
      delay
      btfsc   pl,1
      bsf   PORTB,0
      nop
      dnop
      dnop
      bcf   PORTB,0
      dnop
      goto   endofball

noball      dnop
      goto   noballr

goleft      decf   ball_xpos,f
      skpz
      goto   nolbounce
      bsf   sound_stuff,0
      bcf    ball,x_dir
      incf   ball_xpos,f
      movfw   ball_ypos
      addlw   0x0B
      subwf   pl1_ypos,w
      skpnc
      goto   lmiss1
      movfw   pl1_ypos
      addlw   0x35 - 8
      subwf   ball_ypos,w      ;ball_ypos - (pl2_ypos + 50)
      skpnc
      goto   lmiss2
      bcf   ball,x_dir      ;change ball direction
      btfsc   fire1p,fire1b
      goto   nofirel
      bsf   ball,x_speed
      btfsc   up1p,up1b
      bcf   ball,y_dir
      btfsc   down1p,down1b
      bsf   ball,y_dir
      bsf   ball,y_speed
      bsf   sound_stuff,0
      goto   goleftr


lmiss1      dnop
      dnop
      nop
lmiss2      bsf   ball,wait      ;start waiting
      btfss   ball,serve
      goto   chservel
      incf   pl2_points,f
      bsf   sound_stuff,0
      nop
      dnop
      goto   goleftr

chservel   bsf   ball,serve
      nop
      dnop
      goto   goleftr

rmiss1      dnop
      dnop
      nop
rmiss2      bsf   ball,wait
      btfsc   ball,serve
      goto   chserver
      incf   pl1_points,f
      bsf   sound_stuff,0
      nop
      dnop
      goto   goleftr

chserver   bcf   ball,serve
      nop
      dnop
      goto   goleftr

goup      decf   ball_ypos,f
      skpnz
      bcf   ball,y_dir
      skpnz
      bsf   sound_stuff,0
      skpnz
      incf   ball_ypos,f
      goto   goupr


skipball   movlw   0x16         ;like major wait or something
      delay
      goto   skipballr

highxspeed   goto   highxspeedr      ;small wait or something

nolbounce   movlw   6         ;wait
      delay
      goto   goleftr

norbounce   movlw   6         ;wait
      delay
      nop
      goto   goleftr

waitfire   btfsc   ball,serve      ;whos serve is it ?
      goto   pl2serve
      nop
      movlw   1         ;ball x pos = 1
      movwf   ball_xpos
      movfw   pl1_ypos
      addlw   0x14 
      movwf   ball_ypos      ;ball y pos = pl1 ypos + 20
      movlw   0x20
      movwf   ball         ;setup ball
      btfss   up1p,up1b
      bsf   ball,y_dir      ;is player pressing up ?
      btfss   fire1p,fire1b
      bcf   ball,wait      ;is player pressing fire ?
      btfss   fire1p,fire1b
      bsf   sound_stuff,0      ;is player pressing fire ?
      btfsc   stuff,1
      bcf   ball,wait      ;computerized fire ?
      btfsc   stuff,1
      bsf   sound_stuff,0      ;computerized fire ?
      nop
      movlw   0x0F
      delay   
      goto   skipballr
      
pl2serve   movlw   0x28         ;ball x pos ? 40
      movwf   ball_xpos
      movfw   pl2_ypos
      addlw   0x14
      movwf   ball_ypos      ;ball y pos = pl2 ypos + 20
      movlw   0x31
      movwf   ball         ;setup ball
      btfss   up2p,up2b
      bsf   ball,y_dir      ;is player pressing up ?
      btfss   fire2p,fire2b
      bcf   ball,wait      ;is player pressing fire ?
      btfss   fire2p,fire2b
      bsf   sound_stuff,0      ;is player pressing fire ?
      btfsc   stuff,2
      bcf   ball,wait      ;computerized fire ?
      btfsc   stuff,2
      bsf   sound_stuff,0      ;computerized fire ?
      nop
      movlw   0x0F
      delay
      goto   skipballr

nofirer:   dnop            ;like wait or something
      dnop
      nop
      goto   skipballr

nofirel      dnop            ;like wait or something
      dnop
      nop
      goto   skipballr

fastup      decf   ball_ypos,f      ;move ball up
      skpnz            ;check for bounce
      bcf   ball,y_dir      ;if bounce change y-dir
      skpnz
      bsf   sound_stuff,0
      skpnz
      incf   ball_ypos,f      ;and also move ball down one step
      goto   fastr

yslow      movlw   2         ;like wait or something
      delay
      goto   fastr

nosoundfin   nop
      goto   nosoundfinr      ;wait and stuff

emptycharline   movlw   0x10 - 2      ;17 black lines
      dnop
emptylines   movwf   counter1
      dnop
      nop
ell      bcf   PORTA,0
      bcf   PORTB,0
      movlw   3
      delay
      movlw   0x7
      bsf   PORTA,0
      delay
      btfsc   stuff,0
      bsf   PORTB,0
      call   dosound

      movlw   0x2F  
      delay
      decfsz   counter1,f
      goto   ell
      nop
      bcf   PORTA,0
      goto   syncret

plwon      movlw   0xff
      movwf   PORTB
      movlw   0xFE
      tris   PORTB
plwonw      btfss   fire1p,fire1b
      goto   plwonw
      movlw   0xFF
      delay
      bcf   stuff,0
      bcf   sound_stuff,0
plwonl      movlw   0x00
      tris   PORTB
      call   vertsync      ;do vertical retrace
      movlw   0x8E - 0x19
      call   emptylines
      movlw   pl1won_str 
      btfsc   ball,serve
      movlw   pl2won_str 
      call   printstring
      dnop
      movlw   0x8D - 0x19
      call   emptylines
      movlw   0xff
      movwf   PORTB
      movlw   0xFE
      tris   PORTB
      btfsc   fire1p,fire1b
      goto   plwonl
wfplw      btfss   fire1p,fire1b
      goto   wfplw
      movlw   0xFF
      delay
      btfss   fire1p,fire1b
      goto   wfplw
      goto   menu

menu      bcf   stuff,0
      movlw   pp_str
      movwf   counter4
      movlw   sound_length
      movwf   sound_counter2
      bcf   sound_stuff,0
      clrf   sound_counter
      movlw   0x0E
      tris   PORTA

menul      movlw   0x00
      tris   PORTB
      call   vertsync      ;do vertical retrace
      call   emptycharline
      call   emptycharline
      call   emptycharline
      movlw   pong_str
      dnop
      call   printstring
      call   emptycharline
      dnop
      movlw   crg_str
      call   printstring
      call   emptycharline
      call   emptycharline
      call   emptycharline
      dnop
      movlw   pl12_str
      call   printstring
      call   emptycharline
      movfw   counter4
      dnop
      call   printstring
      dnop
      movlw   0x3E
      call   emptylines
      movlw   0xff
      movwf   PORTB
      movlw   0xFE
      tris   PORTB
      movfw   counter4
      btfss   down1p,down1b
      movlw   pp_str
      btfss   left1p,left1b
      movlw   cc_str
      btfss   right1p,right1b
      movlw   pc_str
      bcf   PORTB,0
      movwf   counter4
      movlw   3
      delay
      bsf   PORTB,0
      movlw   59
      delay

      btfsc   fire1p,fire1b
      goto   menul
wfupgs      btfss   fire1p,fire1b
      goto   wfupgs
      movlw   0xFF
      delay
      btfss   fire1p,fire1b
      goto   wfupgs

      bcf   stuff,1
      bsf   stuff,2
      movfw   counter4
      sublw   pp_str
      skpnz
      bcf   stuff,2
      movfw   counter4
      sublw   cc_str
      skpnz
      bsf   stuff,1
      movlw   0xFF
      delay

      goto   start

printstring   movwf   strptr
      clrf   line
      movlw   0x08
      movwf   counter1
printstrl0   bcf   PORTA,0         ;1 start of sync, set black
      movlw   0x08         ;1 number of characters is
      movwf   counter0      ;1 counter0 shall count 8 times
      movlw   stringbuff      ;1 get pointer to string uffer
      movwf   FSR            ;1 indirect data pointer = string buffer
      movfw   strptr         ;1 get pointer to string
      bsf      STATUS,RP0      ;1 select regbank 1
      movwf   EEADR         ;1 eeprom pointer = string buffer
      bcf      STATUS,RP0      ;1 select regbank 0
      dnop               ;2 delay to make sync 4us
      nop                  ;1 delay to make sync 4us

      bsf   PORTA,0            ;1 end of 4us sync, set black

prnstrl1 bsf   STATUS,RP0      ;1 select regbank 1
      bsf      EECON1,RD      ;1 read data
      movfw   EEDATA         ;1 get result
      incf   EEADR,f         ;1 increase eeprom pointer
      bcf      STATUS,RP0      ;1 select regbank 0

      addwf   line,w         ;1 add line position
      call   table         ;6 get char data
      movwf   INDF         ;1 store data at fsr
      incf   FSR,f         ;1 increase reg pointer
      
      decfsz   counter0,f      ;1(2) loop count
      goto   prnstrl1      ;2

      movlw   0xE
      delay
      incf   line,f
      bcf   PORTA,0

      movlw   0x08
      movwf   counter0
      movlw   stringbuff
      movwf   FSR
      movlw   2
      delay
      bsf   PORTA,0
      movlw   0xD         ;22us black + 8us black col brst
      delay
prnstrl2   movfw   INDF
      movwf   PORTB
      rrf   PORTB,f
      rrf   PORTB,f
      rrf   PORTB,f
      rrf   PORTB,f
      rrf   PORTB,f
      rrf   PORTB,f
      rrf   PORTB,f
      bcf   PORTB,0
      incf   FSR,f
      decfsz   counter0,f
      goto   prnstrl2
      movlw   0x8         ;22us black
      delay
      decfsz   counter1,f
      goto   printstrl0
      nop
      bcf   PORTA,0
      goto   syncret

syncret      movlw   2
      delay
      nop
      bsf   PORTA,0
      movlw   0x38
      delay
      return

      org 0x2100
      
pl1won_str   equ   0x0
      dw   l_P
      dw   l_L
      dw   l_dot
      dw   l_1
      dw   l_space
      dw   l_W
      dw   l_O
      dw   l_N

pl2won_str   equ   0x8
      dw   l_P
      dw   l_L
      dw   l_dot
      dw   l_2
      dw   l_space
      dw   l_W
      dw   l_O
      dw   l_N

pong_str   equ   0x10
      dw   l_space
      dw   l_space
      dw   l_P
      dw   l_O
      dw   l_N
      dw   l_G
      dw   l_space
      dw   l_space

pl12_str   equ   0x18
      dw   l_P
      dw   l_L
      dw   l_1
      dw   l_space
      dw   l_space
      dw   l_P
      dw   l_L
      dw   l_2

pp_str      equ   0x20
      dw   l_space
      dw   l_H
      dw   l_space
      dw   l_space
      dw   l_space
      dw   l_space
      dw   l_H
      dw   l_space

pc_str      equ   0x28
      dw   l_space
      dw   l_H
      dw   l_space
      dw   l_space
      dw   l_space
      dw   l_space
      dw   l_C
      dw   l_space

cc_str      equ   0x30
      dw   l_space
      dw   l_C
      dw   l_space
      dw   l_space
      dw   l_space
      dw   l_space
      dw   l_C
      dw   l_space

crg_str      equ   0x38
      dw   l_CC
      dw   l_R
      dw   l_dot
      dw   l_G
      dw   l_U
      dw   l_N
      dw   l_E
      dw   l_E


      end 

#endasm
