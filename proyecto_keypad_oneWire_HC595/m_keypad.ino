int rowPressed;

char Teclas[16] = {
      '1','2','3', 'A',
      '4','5','6', 'B',
      '7','8','9', 'C',
      '.','0','#', 'D',
    };
    
void setupM3() {
    pulsacion = 0;
    keyState = K_UNPRESSED;
}

int leerRow(unsigned int _pin) {
    int value = analogRead(_pin);
    
    if (value > 230) {
        int vMax = value;
        for(int i=0; i<10; i++) {
            value = analogRead(_pin);
            vMax = (value > vMax)? value : vMax;
        }
        
        if(vMax > 920) { 
            return 1;
        } else if (vMax > 460) {
            return 2;
        } else if (vMax > 307) {
            return 3;
        } else if (vMax > 230) {
            return 4;
        } else { return 0; }
    } else {return 0;}
}

char obtenerTecla() {
    char t = 0;
    for(int i = 0; i<=3; i++ ) {
        if (t == 0) {
            rowPressed = leerRow(i);
            if (rowPressed) {
                t = Teclas[((rowPressed - 1) * 4) + i];
            } else {
                t = 0;
            }
        }
    } // for i

    return t;
}

void loopM3() {
    switch(keyState) {
        case K_UNPRESSED :  pulsacion = obtenerTecla();
                            if (pulsacion != 0) {
                                keyState = K_PRESSED;
                            }
                            break;
        case K_PRESSED   :  // delay(20); // debouncing
                            keyState = K_HOLD;
                            break;
        case K_HOLD      :  pulsacion = obtenerTecla();
                            if(pulsacion == 0) {
                                keyState = K_RELEASED;
                            }
                            break;
        case K_RELEASED   : // delay(20);
                            keyState = K_UNPRESSED;
                            break;

    } // switch
}


