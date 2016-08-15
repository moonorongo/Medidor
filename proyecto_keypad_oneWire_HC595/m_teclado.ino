int TeclaPresionada;

void setupM1() {
    estadoM1 = 0;
    TeclaPresionada = 0;
}


void loopM1() {
    if(TeclaPresionada == 0) {
        if(keyState == K_HOLD) {
            TeclaPresionada = 1;
    
            if (pulsacion != 0) { // bloque deteccion teclas especiales;
                if (pulsacion == '.') { // punto va a estado decimal
                    if (estadoM1 < 4) {
                        estadoM1 = 4;
                        pulsacion = 0;
                    } else {
                        pulsacion = 0;
                    }
                } // '.'
                
                if (pulsacion == 'A') { // A inicia medicion
                    inputString = "#M";
                    for(int i=0; i<5; i++) {
                        inputString += digits[i]; 
                    }
                    stringComplete = true;
                    estadoM1 = 0;
                    clearDigits();
                    pulsacion = 0;
                }

                if (pulsacion == 'B') { // B do nothing
                    pulsacion = 0;
                }
        
                if (pulsacion == 'C') { // C GET CONFIG
                    inputString = "#G";
                    inputString += digits[3]; 
                    stringComplete = true;
                    estadoM1 = 0;
                    clearDigits();
                    pulsacion = 0;
                }

                /* NOTA: para simplificar codigo, la programacion se realiza
                   de la siguiente manera: se introduce el numero normalmente, 
                   y el decimal indica en que parametro se guardará 
                   se envia como #P12345
                                   vvvvp
                 */
                if (pulsacion == 'D') { // D PROGRAM
                    inputString = "#P";
                    for(int i=0; i<5; i++) {
                        inputString += digits[i]; 
                    }
                    stringComplete = true;
                    estadoM1 = 0;
                    clearDigits();
                    pulsacion = 0;
                }
                
                if (pulsacion == '#') { // # do nothing
                    pulsacion = 0;
                }
                
                refreshDisplay();
            }
            
            if (pulsacion != 0) {
                switch(estadoM1) {
                    case 0 :    digits[3] =  pulsacion - '0';
                                estadoM1 = 1;
                                break;
                    case 1 :    digits[2] = digits[3];
                                digits[3] =  pulsacion - '0';
                                estadoM1 = 2;
                                break;
                    case 2 :    digits[1] = digits[2];
                                digits[2] = digits[3];
                                digits[3] =  pulsacion - '0';
                                estadoM1 = 3;
                                break;
                    case 3 :    digits[0] = digits[1];
                                digits[1] = digits[2];
                                digits[2] = digits[3];
                                digits[3] =  pulsacion - '0';
                                estadoM1 = 4;
                                break;
                    case 4 :    digits[4] =  pulsacion - '0';
                                estadoM1 = 5;
                                break;
                } // switch
                
                refreshDisplay();
            } // if pulsacion
        } // IF HOLD
    } // if TeclaPresionada

    if(TeclaPresionada == 1) {
        if(keyState == K_RELEASED) {
            TeclaPresionada = 0;
        }
    }

} // loop
