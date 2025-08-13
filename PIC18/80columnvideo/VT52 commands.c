/*
VT52 commands

ESC-< Enter VT52 mode
ESC-= alternate numeric keypad mode
ESC-> standard numeric keypad mode
ESC-A Cursor Up
ESC-B Cursor Down
ESC-C Cursor Right
ESC-D Cursor Left
ESC-E Clear Home
ESC-F Use graphics font ROM
ESC-G Use normal US/UK font ROM
ESC-H Cursor Home
ESC-I reverse line feed 
ESC-J Clear to end of screen
ESC-K Delete rest of line
ESC-L Insert Line
ESC-M Delete Line
ESC-Y Positioning Cursor (followed by 2 characters: X, Y where code is 32+X 32+Y (always printable)
ESC-Z Identify terminal (correct response is ESC-/-K is VT52, ESC-/-Z is VT100?) or ESC-/-L if there is a printer attached

ESC-[ hold screen mode (batch input?)
ESC-\ exit hold screen mode


ESC-p Reverse video
ESC-q Normal video

CTL-G  7 BEL
CTL-H  8 BACKSPACE
CTL-I  9 TAB (VT52 tab stops are 8 characters 0,8,16,24...)
CTL-J 10 Linefeed
CTL-L 12 FormFeed
CTL-M 13 CARRIAGE return
CTL-N 14 SHIFT OUT: same as escape Y cursor positioning
CTL-Z 26 Document end
arrow keys:

ESC-A
ESC-B
ESC-C
ESC-D

function keys:

ESC-OP   PF1
ESC-OQ   PF2
ESC-OR   PF3
ESC-OS   PF4

numeric keypad alternate keys:

(note: VT100 is ESC-Op, VT52 is ESC-?p)

ESC-?p   0
ESC-?q   1
ESC-?r   2
ESC-?s   3
ESC-?t   4
ESC-?u   5
ESC-?v   6
ESC-?w   7
ESC-?x   8
ESC-?y   9
ESC-Om   - (minus) // not on VT52
ESC-Ol   , (comma) // not on VT52
ESC-?n   . (period
ESC-?M   enter (^M)


graphics mode:

codes 136-176 octal (94-126, 0x5E-0x7E the lower case letters and symbols )
symbols replaced in graphics mode are: "^_`{|}~ a-z"

special characters in graphics mode are:

8 horizontal bars at various heights (0-7)
numeric subscripts 0-9
solid rectangle
1/
3/
5/
7/
degrees
plus or minus
right arrow
ellipsis
divide by
down arrow
paragraph
two blanks
one reserved

*/

