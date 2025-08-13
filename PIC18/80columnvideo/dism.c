00B2: CLRF     FF7
      ADDLW    C2
      MOVLW    00
      ADDWFC   FF7,F
      TBLRD    *+
      MOVF     FF5,W
00C0: RETURN   0

00EC: CLRF     FF7
      ADDLW    FC
      MOVWF    FF6
      MOVLW    00
      ADDWFC   FF7,F
      TBLRD    *+
      MOVF     FF5,F
      RETURN   0
      
