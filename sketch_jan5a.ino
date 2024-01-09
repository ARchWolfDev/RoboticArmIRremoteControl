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

int servoNumb = 0;
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
        pwm.setPWM(1, 0, pulseWidth(90));
        pwm.setPWM(2, 0, pulseWidth(90));
        pwm.setPWM(3, 0, pulseWidth(180));
        pwm.setPWM(4, 0, pulseWidth(180));
        pwm.setPWM(5, 0, pulseWidth(90));
        pwm.setPWM(6, 0, pulseWidth(0));
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





// pwm.setPWM(2, 0, pulseWidth(90));pwm.setPWM(3, 0, pulseWidth(180));pwm.setPWM(4, 0, pulseWidth(180));pwm.setPWM(5, 0, pulseWidth(90));pwm.setPWM(6, 0, pulseWidth(0));
// if (sensorValue > 180) {
//   pwm.setPWM(1, 0, pulseWidth(180));
// } else {
//   pwm.setPWM(1, 0, pulseWidth(sensorValue));
// }
// for (int motor = 1; motor <= 6; motor++) {
//   for (int i = 90; i >= 0; i--) {
//   pwm.setPWM(motor, 0, pulseWidth(i));
//   delay(50);
//   };
// }

// pwm.setPWM(2, 0, pulseWidth(90));pwm.setPWM(3, 0, pulseWidth(180));pwm.setPWM(4, 0, pulseWidth(180));pwm.setPWM(5, 0, pulseWidth(90));pwm.setPWM(6, 0, pulseWidth(0));
// delay(3000);
// // Pozitia 2: Servo-motorul M1 merge spre o pozitie de 135 de grade si M4 indreapta incheietura bratului
// pwm.setPWM(1, 0, pulseWidth(135)); pwm.setPWM(2, 0, pulseWidth(90));pwm.setPWM(3, 0, pulseWidth(180));pwm.setPWM(4, 0, pulseWidth(90));pwm.setPWM(5, 0, pulseWidth(85));pwm.setPWM(6, 0, pulseWidth(0));
// delay(3000);
// // Pozitia 3: Servo-motorul M2 se lasa pe spate pentru a avea prindere,
// pwm.setPWM(1, 0, pulseWidth(135)); pwm.setPWM(2, 0, pulseWidth(95));pwm.setPWM(3, 0, pulseWidth(180));pwm.setPWM(4, 0, pulseWidth(85));pwm.setPWM(5, 0, pulseWidth(85));pwm.setPWM(6, 0, pulseWidth(0));
// delay(3000);
// // Pozitia 4: Servo-motorul M6 apuca obiectul
// pwm.setPWM(1, 0, pulseWidth(135)); pwm.setPWM(2, 0, pulseWidth(95));pwm.setPWM(3, 0, pulseWidth(180));pwm.setPWM(4, 0, pulseWidth(85));pwm.setPWM(5, 0, pulseWidth(85));pwm.setPWM(6, 0, pulseWidth(21));
// delay(3000);
// // Pozitia 5: Servo motorul M2 se deplaseaza cu 5 grade in sus pentru a ridica obiectul
// pwm.setPWM(1, 0, pulseWidth(135)); pwm.setPWM(2, 0, pulseWidth(80));pwm.setPWM(3, 0, pulseWidth(180));pwm.setPWM(4, 0, pulseWidth(85));pwm.setPWM(5, 0, pulseWidth(85));pwm.setPWM(6, 0, pulseWidth(21));
// delay(3000);
// // Pozitia 6: Servo motorul M1 se depleaza la gradul 0, deplaseaza obiectul
// pwm.setPWM(1, 0, pulseWidth(0)); pwm.setPWM(2, 0, pulseWidth(80));pwm.setPWM(3, 0, pulseWidth(180));pwm.setPWM(4, 0, pulseWidth(85));pwm.setPWM(5, 0, pulseWidth(85));pwm.setPWM(6, 0, pulseWidth(21));
// delay(3000);
// // Pozitia 7: Servo motorul M6 deschide degetele pentru a lasa obiectul jos
// pwm.setPWM(1, 0, pulseWidth(0)); pwm.setPWM(2, 0, pulseWidth(90));pwm.setPWM(3, 0, pulseWidth(180));pwm.setPWM(4, 0, pulseWidth(85));pwm.setPWM(5, 0, pulseWidth(85));pwm.setPWM(6, 0, pulseWidth(0));
// delay(3000);

  
}