#include <a4988.h>
#include <EEPROM.h>

struct ConfigStructure {
    unsigned int MOTOR_STEPS; // CANTIDAD DE PASOS POR VUELTA
    unsigned int CONFIG_STEPS; // FACTOR STEP - MM 
    unsigned int STEPPER_DELAY; // DELAY PARA NO SATURAR EL STEPPER
    unsigned long ZERO_POSITION; // posicion del sensor de 0. utilizada para restar la posicion.
};

ConfigStructure config; 

#define K_UNPRESSED 0
#define K_PRESSED 1
#define K_HOLD 2
#define K_RELEASED 3

#define dataPin  2  
#define latchPin 3
#define clockPin 4  

#define DIRPIN 5
#define STEPPIN 6 
#define ENABLEPIN 7
#define START_SENSOR 8 
#define END_SENSOR 9 
#define BLOCK_PIN 10

#define BACK_DIRECTION 0
#define GO_DIRECTION 1

#define TERMINADOR ';'

// utilizadas por M2
long stepsCount;
long mm;

byte digits[] = {0, 0, 0, 0, 0};
//              m  c  d  u. decimas

a4988 myA4988;

byte const numbers[10] = {
    0b01011111, 
    0b00000110,
    0b00111011, 
    0b00101111, 
    0b01100110,
    0b01101101,
    0b01111100,
    0b00000111,
    0b01111111,
    0b01100111
};

// utilizadas por M1
int estadoM1; 
String inputString = "";         
boolean stringComplete= false;   

// utilizadas por m_keypad
char pulsacion;
unsigned short keyState;
unsigned short habilitarLecturaKeypad;

void resetA4988() {
    myA4988.initA4988(config.MOTOR_STEPS, DIRPIN, ENABLEPIN, STEPPIN);    
    myA4988.setDelay(config.STEPPER_DELAY); 
}


void clearDigits() {
    digits[0] = 0;    
    digits[1] = 0;    
    digits[2] = 0;    
    digits[3] = 0;    
    digits[4] = 0;    
}

void refreshDisplay()
{
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, numbers[digits[4]]);
    shiftOut(dataPin, clockPin, MSBFIRST, numbers[digits[3]]);
    shiftOut(dataPin, clockPin, MSBFIRST, numbers[digits[2]]);
    shiftOut(dataPin, clockPin, MSBFIRST, numbers[digits[1]]);
    shiftOut(dataPin, clockPin, MSBFIRST, numbers[digits[0]]);
    digitalWrite(latchPin, HIGH);
}

void storeDefaults() {
    config.MOTOR_STEPS = 200;
    config.CONFIG_STEPS = 107;
    config.STEPPER_DELAY = 1000;
    config.ZERO_POSITION = 0;
}

void getConfig() {
    EEPROM.get(0, config);    
}

void saveConfig() {
    EEPROM.put(0, config);    
};

void programConfig() {
    if (inputString[6] == '1') {
        config.MOTOR_STEPS = inputString.substring(2,6).toInt();
    } else if (inputString[6] == '2') {
        config.CONFIG_STEPS = inputString.substring(2,6).toInt();
    } else if (inputString[6] == '3') {
        config.STEPPER_DELAY = inputString.substring(2,6).toInt();
    } else if (inputString[6] == '4') {
        config.ZERO_POSITION = inputString.substring(2,6).toInt();
    } else if (inputString[6] == '9') { // 9: guarda en EEPROM
        saveConfig();
    }

    resetA4988();
}

void toArrayDigits(unsigned long value) {
    inputString = String(value);
    for(int i=0; i<inputString.length(); i++) {
        digits[3-i] = inputString[(inputString.length() - 1) - i] - '0';
    }
    inputString = "";
}

void displayConfig(char opcion) {
    if (opcion == '1') {
        toArrayDigits(config.MOTOR_STEPS);
    }
    if (opcion == '2') {
        toArrayDigits(config.CONFIG_STEPS);
    }
    if (opcion == '3') {
        toArrayDigits(config.STEPPER_DELAY);
    }
    if (opcion == '4') {
        toArrayDigits(config.ZERO_POSITION);
    }
    
    if (opcion == '9') { // carga defaults
        storeDefaults();        
        clearDigits();
    }
    
    refreshDisplay();
}


void setup() {
    getConfig();
    resetA4988();

    setupM1();
    setupME2();
    setupM3();
    
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    refreshDisplay();

    habilitarLecturaKeypad = 1;

    // borrar en hardware
    Serial.begin(9600);
}

void loop() {
      if (habilitarLecturaKeypad) {
          loopM3(); // lee teclas
          loopM1(); // interpreta teclas
      }

    if (stringComplete) {
      if(inputString.startsWith("#M")) { // start medida
          mm = inputString.substring(2).toInt();
          inputString = "";
          stringComplete = false;
      }
      if(inputString.startsWith("#G")) { // get config
          displayConfig(inputString[2]);
          inputString = "";
          stringComplete = false;
      }
      if(inputString.startsWith("#R")) { // RESET
          storeDefaults();
          resetA4988();          
          inputString = "";
          stringComplete = false; 
      }
      if(inputString.startsWith("#P")) { // PROGRAM CONFIGURATION
          programConfig();
          resetA4988();          
          inputString = "";
          stringComplete = false; 
      }
      if(inputString.startsWith("#S")) { // save config
          saveConfig();
          inputString = "";
          stringComplete = false; 
      }
    } // stringComplete
  
  loopME2(); // acciones Motor
}
