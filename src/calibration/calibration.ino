//www.elegoo.com

#include <Servo.h>  //servo library

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11

#define carSpeed 150
#define rotation_delay_factor 21

int full_rotation_time = rotation_delay_factor * carSpeed;

int count = 0;

void right() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Right");
}

void left(){
  analogWrite(ENA,carSpeed);
  analogWrite(ENB,carSpeed);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); 
  Serial.println("go left!");
}


void turn_right() {
  right();
  delay(full_rotation_time / 4);
  stop();
}

void turn_left() {
  left();
  delay(full_rotation_time / 4);
  stop();
}

void stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("Stop!");
} 

void setup() { 
  Serial.begin(9600);     
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  stop();
} 

void loop() { 
  delay(500);
  if (count < 20) {
    if ((count/4)%2 == 0) {
      turn_right();
    } else {
      turn_left();
    }
  }
  count++;
}


