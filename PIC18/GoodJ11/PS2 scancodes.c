

char scan_code[]={
                  0x1c,0x32,0x21,0x23,0x24,0x2b,0x34,0x33,0x43,0x3b,0x42,0x4b,0x3a,0x31,0x44,0x4d,0x15,0x2d,0x1b,0x2c,0x3c,0x2a,0x1d,0x22,0x35,0x1a, // a-z
                  0x45,0x16,0x1e,0x26, 0x25,0x2e,0x36,0x3d,0x3e,0x46, // 0-9
                  0x0e,0x4e,0x55,0x5d,0x54,0x5b,0x4c,0x52,0x41,0x49,0x4a,0x29 // punctuation and space
                  0x66,0x0d,0x5A,0x76 // control codes
                  };

char normal_code[]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z', // a-z
               '0','1','2','3','4','5','6','7','8','9', // 0-9
               '`','-','=','\\','[',']',';','\'',',','.','/',' ' // punctuation
               8,9 ,13,27 
               };

char shift_code[]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z', // a-z
               ')','!','@','#','$','%','^','&','*','(', // 0-9
               '~','_','+','|','{','}',':','\"','<','>','?',' ' // punctuation
               8,9,13,27 
               };
char control_code[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26
                     0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,
                     8,9,13,27
};

/*
// pause is unique two codes preceded by a single 0xE1: 0x14 0x77
// it sends a matching release code UPON KEYPRESS, there is no true
// release code. 

            case 0x14: // pause MULTIPLE
                     break;

// printscreen is two codes each preceded by 0xE0: 0x12 and 0x7C
            case 0x12: // printscreen MULTIPLE
                     break;

// the following keys are all preceded by 0xE0
            case 0x1f: // L-GUI
                     break;
            case 0x14: // R-CTL
                     break;
            case 0x11: // R- ALT
                     break;
            case 0x27: // R-GUI
                     break;
            case 0x7e: // scroll lock
                     break;
            case 0x4A: // KP '/' MULTIPLE
                     break;
            case 0x5A: // KP enter MULTIPLE
                     break;
            case 0x70: // insert MULTIPLE
                     break;
            case 0x6C: // home MULTIPLE
                     break;
            case 0x7D: // pgup MULTIPLE
                     break;
            case 0x71: // delete MULTIPLE
                     break;
            case 0x69: // end MULTIPLE
                     break;
            case 0x7A: // pgdn MULTIPLE
                     break;
            case 0x75: // up arrow MULTIPLE
                     break;
            case 0x6B: // left arrow MULTIPLE
                     break;
            case 0x72: // down arrow MULTIPLE
                     break;
            case 0x74: // right arrow MULTIPLE
                     break;
*/


         switch(thiskey)
         {
            case 0x1c: // A
                     emit=shifted?'A':'a';
                     break;
            case 0x32: // B
                     emit=shifted?'B':'b';
                     break;
            case 0x21: // C
                     emit=shifted?'C':'c';
                     break;
            case 0x23: // D
                     emit=shifted?'D':'d';
                     break;
            case 0x24: // E
                     emit=shifted?'E':'e';
                     break;
            case 0x2b: // F
                     emit=shifted?'F':'f';
                     break;
            case 0x34: // G
                     emit=shifted?'G':'g';
                     break;
            case 0x33: // H
                     emit=shifted?'H':'h';
                     break;
            case 0x43: // I
                     emit=shifted?'I':'i';
                     break;
            case 0x3B: // K
                     emit=shifted?'J':'j';
                     break;
            case 0x42: // K
                     emit=shifted?'K':'k';
                     break;
            case 0x4B: // L
                     emit=shifted?'L':'l';
                     break;
            case 0x3A: // M
                     emit=shifted?'M':'m';
                     break;
            case 0x31: // N
                     emit=shifted?'N':'n';
                     break;
            case 0x44: // O
                     emit=shifted?'O':'o';
                     break;
            case 0x4D: // P
                     emit=shifted?'P':'p';
                     break;
            case 0x15: // Q
                     emit=shifted?'Q':'q';
                     break;
            case 0x2D: // R
                     emit=shifted?'R':'r';
                     break;
            case 0x1B: // S
                     emit=shifted?'S':'s';
                     break;
            case 0x2C: // T
                     emit=shifted?'T':'t';
                     break;
            case 0x3C: // U
                     emit=shifted?'U':'u';
                     break;
            case 0x2A: // V
                     emit=shifted?'V':'v';
                     break;
            case 0x1D: // W
                     emit=shifted?'W':'w';
                     break;
            case 0x22: // X
                     emit=shifted?'X':'x';
                     break;
            case 0x35: // Y
                     emit=shifted?'Y':'y';
                     break;
            case 0x1A: // Z
                     emit=shifted?'Z':'z';
                     break;
            case 0x45: // 0
                     emit=shifted?')':'0';
                     break;
            case 0x16: // 1
                     emit=shifted?'!':'1';
                     break;
            case 0x1E: // 2
                     emit=shifted?'@':'2';
                     break;
            case 0x26: // 3
                     emit=shifted?'#':'3';
                     break;
            case 0x25: // 4
                     emit=shifted?'$':'4';
                     break;
            case 0x2E: // 5
                     emit=shifted?'%':'5';
                     break;
            case 0x36: // 6
                     emit=shifted?'^':'6';
                     break;
            case 0x3E: // 7
                     emit=shifted?'*':'7';
                     break;
            case 0x3D: // 8
                     emit=shifted?'&':'8';
                     break;
            case 0x46: // 9
                     emit=shifted?'(':'9';
                     break;
            case 0x0E: // `
                     emit=shifted?'~':'`';
                     break;
            case 0x4E: // -
                     emit=shifted?'_':'-';
                     break;
            case 0x55: // =
                     emit=shifted?'+':'=';
                     break;
            case 0x5D: // backslash
                     emit=shifted?'|':'\\';
                     break;
            case 0x66: // backspace
                     BackSpace();
                     break;
            case 0x29: // space
                     emit=' ';
                     break;
            case 0x0D: // tab
                     emit='t'+128;
                     break;
            case 0x58: // caps
                     if(!release) if(shifted) shifted=0; else shifted=1;
                     break;
            case 0x12: // l-shift
                     if(release) shifted=0; else shifted=1;
                     break;
            case 0x14: // l-control
                     if(release) controlled=0; else controlled=1;
                     emit='t'+128;
                     break;
            case 0x59: // r-shift
                     if(release) shifted=0; else shifted=1;
                     break;
            case 0x5A: // enter
                     ScrollUp();
                     break;
            case 0x76: // ESC
                     emit=27;
                     break;
            case 0x05: // F1
                     emit='1'+128;
                     break;
            case 0x06: // F2
                     emit='2'+128;
                     break;
            case 0x04: // F3
                     emit='3'+128;
                     break;
            case 0x0C: // F4
                     emit='4'+128;
                     break;
            case 0x03: // F5
                     emit='5'+128;
                     break;
            case 0x0B: // F6
                     emit='6'+128;
                     break;
            case 0x83: // F7
                     emit='7'+128;
                     break;
            case 0x0A: // F8
                     emit='8'+128;
                     break;
            case 0x01: // F9
                     emit='9'+128;
                     break;
            case 0x09: // F10
                     emit='a'+128;
                     break;
            case 0x78: // F11
                     emit='b'+128;
                     break;
            case 0x07: // F12
                     emit='c'+128;
                     break;
            case 0x54: // "["
                     emit=shifted?'{':'[';
                     break;
            case 0x5B: // "]"
                     emit=shifted?'}':']';
                     break;
            case 0x4C: // ;
                     emit=shifted?':':';';
                     break;
            case 0x52: // '
                     emit=shifted?'\"':'\'';
                     break;
            case 0x41: // ,
                     emit=shifted?'<':',';
                     break;
            case 0x49: // .
                     emit=shifted?'>':'.';
                     break;
            case 0x4A: // '/' 
                     emit=shifted?'?':'/';
                     break;
            case 0x7C: // KP *
                     emit='*';
                     break;
            case 0x7B: // KP -
                     emit='-';
                     break;
            case 0x79: // KP +
                     emit='+';
                     break;
            case 0x71: // KP .
                     emit='.';
                     break;
            case 0x70: // KP 0
                     emit='0';
                     break;
            case 0x69: // KP 1
                     emit='1';
                     break;
            case 0x72: // KP 2
                     emit='2';
                     break;
            case 0x7a: // KP 3
                     emit='3';
                     break;
            case 0x6b: // KP 4
                     emit='4';
                     break;
            case 0x73: // KP 5
                     emit='5';
                     break;
            case 0x74: // KP 6
                     emit='6';
                     break;
            case 0x6c: // KP 7
                     emit='7';
                     break;
            case 0x75: // KP 8
                     emit='8';
                     break;
            case 0x7d: // KP 9
                     emit='9';
                     break;
            case 0x77: // numlock MULTIPLE
                     emit='n'+128;
                     break;
            default:
                     emit=0;
                     break;
         };

