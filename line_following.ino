#include <Wire.h>
#include <Adafruit_MotorShield.h> // found in adafruit.com
#include "utility/Adafruit_PWMServoDriver.h"
#include <Servo.h> 

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

int photocellPin = 1;     // analog pin for the photocell
int photocellReading;     // analog reading
int sensorPin = 0;   // analog pin for the light sensor

Servo rightServo; // servo1
Servo leftServo; // servo2

float rawRange = 1024; // 3.3v
float logRange = 5.0; // 3.3v = 10^5 lux

void setup() {
  Serial.begin(9600);           

  AFMS.begin();  // create with the default frequency 1.6KHz

  // the two servo pins on the motor shield
  rightServo.attach(10);
  leftServo.attach(9);  
}


void loop() {
    int rawValue = analogRead(sensorPin);    
    photocellReading = analogRead(photocellPin);
    int value = RawToLux(rawValue);
    
    Serial.print("Photocell = ");
    Serial.print(photocellReading);
    Serial.print(" - Lux = ");
    Serial.println(value);
    
    // no movement: right 92; left 93
    // 0 & 180: fullest in either direction
    if (530 <= photocellReading && value >= 54){ // right
      rightServo.write(90);
      leftServo.write(180); 
    }
    else if (photocellReading <= 380 && value <= 44){ // left
      rightServo.write(50);
      leftServo.write(90);
    }
    else if (photocellReading <= 380 || value >= 54){ //left
      rightServo.write(0);
      leftServo.write(93);
    }
    else if (530 <= photocellReading || value <= 44){ // right
      rightServo.write(92);
      leftServo.write(180);
    }
    else if (380 < photocellReading < 530 || 43 < value < 54){ //middle
      rightServo.write(0);
      leftServo.write(180);
    }
    delay(15);                     
}

float RawToLux(int raw) {
  float logLux = raw * logRange / rawRange;
  return pow(10, logLux);
}

// code adapted from Adafruit's tutorials
