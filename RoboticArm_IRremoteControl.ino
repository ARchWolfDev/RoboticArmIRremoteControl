#include <IRremote.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#define MIN_PULSE_WIDTH 650
#define MAX_PULSE_WIDTH 2350
#define DEFAULT_PULSE_WIDTH 1500
#define FREQUENCY 50
#define LED_PIN1 10
#define LED_PIN2 11
#define LED_PIN3 9
#define LED_PIN4 6
#define LED_PIN5 5
#define LED_PIN6 3

// Define the IR receiver pin
const int IR_PIN = 7;
// Define the IR receiver object
IRrecv irrecv(IR_PIN);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
uint8_t servonum = 0;

const int rows = 10;
const int columns = 7;
int moveSteps[rows][columns];
int index = 0;

void savePosition(int positionToSave[], int index){
  Serial.println("Index:");
  Serial.println(index);
  for (int i = 0; i < columns; i++){
    moveSteps[index][i] = positionToSave[i];
  }
} 

void executeSaved(int moveSteps[][7]){
  for ( int i = 0; i < index; i++){
    for (int j = 1; j < columns; j++){
      pwm.setPWM(j, 0, pulseWidth(moveSteps[i][j]));
      delay(500);
    }
    delay(1000);
  }
}

int servoNumb = 0;
const int initialState[7] = {0, 90, 90, 180, 180, 90, 0};
int angleValue[7] = {0, 90, 90, 180, 180, 90, 0};
int ledArray[7] = {0, LED_PIN1, LED_PIN2, LED_PIN3, LED_PIN4, LED_PIN5, LED_PIN6};
int pwmValue = angleValue[servoNumb];

void setup() { 
  Serial.begin(9600);
  Serial.println("16 channel Servo test!");
  irrecv.enableIRIn();
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);
  pinMode(LED_PIN4, OUTPUT);
  pinMode(LED_PIN5, OUTPUT);
  pinMode(LED_PIN6, OUTPUT);
  pwm.setPWM(1, 0, pulseWidth(90)); pwm.setPWM(2, 0, pulseWidth(90));pwm.setPWM(3, 0, pulseWidth(180));pwm.setPWM(4, 0, pulseWidth(180));pwm.setPWM(5, 0, pulseWidth(90));pwm.setPWM(6, 0, pulseWidth(0));
}

int pulseWidth(int angle) {
  int pulse_wide, analog_value;
  pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  // Serial.println(analog_value);
  return analog_value;
}

void loop() {
  if (irrecv.decode()){
    Serial.println(irrecv.decodedIRData.decodedRawData);
    switch(irrecv.decodedIRData.decodedRawData){
      case 3910598400:
        // digitalWrite(LED_PIN1, HIGH);
        servoNumb = 1;
        break;
      case 2907897600:
        digitalWrite(LED_PIN1, LOW);
        digitalWrite(LED_PIN2, LOW);
        digitalWrite(LED_PIN3, LOW);
        digitalWrite(LED_PIN4, LOW);
        digitalWrite(LED_PIN5, LOW);
        digitalWrite(LED_PIN6, LOW);
        servoNumb = 0;
        break;
      case 3860463360:
        digitalWrite(LED_PIN2, HIGH);
        servoNumb = 2;
        break;
      case 4061003520:
        // digitalWrite(LED_PIN3, HIGH);
        servoNumb = 3;
        break;
      case 4077715200:
        // digitalWrite(LED_PIN4, HIGH);
        servoNumb = 4;
        break;
      case 3877175040:
        // digitalWrite(LED_PIN5, HIGH);
        servoNumb = 5;
        break;
      case 2707357440:
        // digitalWrite(LED_PIN6, HIGH);
        servoNumb = 6;
        break;
      case 3175284480:
        for (int i = 1; i < 7; i++){
          pwm.setPWM(i, 0, pulseWidth(initialState[i]));
          delay(500);
        }
        break;
      case 3041591040:
      // Save position
        savePosition(angleValue, index);
        index = index + 1;
        for (int i = 1; i < 7; i++){
          digitalWrite(ledArray[i], HIGH);
          delay(100);
        }
        break;
      case 3141861120:
      // execute position saved
        executeSaved(moveSteps);
        break;
    }
    switch(irrecv.decodedIRData.decodedRawData){
      case 3108437760:
        if (servoNumb == 6) {
          if (angleValue[servoNumb] >= 100){
            angleValue[servoNumb] +=0;
          } else {
            angleValue[servoNumb] +=5;
          }
        } else {
          if (angleValue[servoNumb] >= 180){
            angleValue[servoNumb] +=0;
          } else {
            angleValue[servoNumb] +=10;
          }
        }
        break;
      case 3927310080:
        if (servoNumb == 6){
          if (angleValue[servoNumb] <= 0){
            angleValue[servoNumb] -=0;
          } else {
            angleValue[servoNumb] -=5;
          }
        } else {
          if (angleValue[servoNumb] <= 0){
            angleValue[servoNumb] -=0;
          } else {
            angleValue[servoNumb] -=10;
          }
        }
        break;
    }
    // Reset the IR receiver for the next signal
    irrecv.resume();
  }
  Serial.println(servoNumb);
  Serial.println(angleValue[servoNumb]);
  for (int i = 0; i <= 7; i++){
    if (i == servoNumb){
      digitalWrite(ledArray[i], HIGH);
    } else {
      digitalWrite(ledArray[i], LOW);
    }
  }

  pwm.setPWM(servoNumb, 0, pulseWidth(angleValue[servoNumb]));
   
}
