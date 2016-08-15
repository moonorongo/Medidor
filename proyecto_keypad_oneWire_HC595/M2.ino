/* maquina de estados para controlar el stepper */

int estadoM2;

void setupME2() {
  myA4988.enable(1); // logica invertida
  estadoM2 = 0;
  stepsCount = 0;
  mm = 0;

  pinMode(START_SENSOR, INPUT);
  digitalWrite(START_SENSOR, HIGH);
  pinMode(END_SENSOR, INPUT);
  digitalWrite(END_SENSOR, HIGH);
  pinMode(BLOCK_PIN, OUTPUT);
  digitalWrite(BLOCK_PIN, HIGH);
}

void loopME2() {
    if(!digitalRead(END_SENSOR)) {
        stepsCount = 0;
        estadoM2 = 2;
    }
    
    switch(estadoM2) {
        case 0: // espero 
                if(mm > 0) {
                    myA4988.enable(0);
                    delay(200);
                    digitalWrite(BLOCK_PIN, LOW); 
                    myA4988.setDirection(BACK_DIRECTION); 
                    mm = abs(mm) - config.ZERO_POSITION; 
                    stepsCount = mm * config.MOTOR_STEPS / config.CONFIG_STEPS; 
                    estadoM2 = 1; // reset a 0
                    habilitarLecturaKeypad = 0;
                } // mm>0
                break;
        case 1: // reset a posicion 0; 
                myA4988.stepOnce();
                break;
        case 2: // motor activado    
                myA4988.stepOnce();
                stepsCount--;
                     
                if (stepsCount <= 0) {
                    clearDigits();
                    refreshDisplay();
                    digitalWrite(BLOCK_PIN, HIGH); 
                    delay(200);
                    myA4988.enable(1);
                    mm = 0;
                    estadoM2 = 0;
                    estadoM1 = 0;
                    habilitarLecturaKeypad = 1;
                }
                break;
    } // switch
}

