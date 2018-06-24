#include <Arduino.h>
#include "self_drive.h"
#include "common.h"

int Echo = A4;  
int Trig = A5; 

int rightDistance = 0, leftDistance = 0, middleDistance = 0;
int STOPPING_DIST_CM = 50;
int STRAIGHT = 90;
int RIGHT = 0;
int LEFT = 180;

Servo myservo;      // create servo object to control servo


//Ultrasonic distance measurement Sub function
int Distance_test() {
  digitalWrite(Trig, LOW);   
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);   
  float Fdistance = pulseIn(Echo, HIGH);  
  Fdistance= Fdistance / 58;       
  return (int)Fdistance;
}  

int unit_distance(int x) {
  myservo.write(x);
  delay(300);
  int dist = Distance_test();
  return dist;
}

int can_move(int stop_dist) {
  int d = 15;
  if (stop_dist > unit_distance(STRAIGHT))
    return 0;
  if (stop_dist > unit_distance(STRAIGHT + d))
    return 0;
  if (stop_dist > unit_distance(STRAIGHT - d))
    return 0;
  return 1;
}

void setup_self_drive() { 
  myservo.attach(3);  // attach servo on pin 3 to servo object
  pinMode(Echo, INPUT);    
  pinMode(Trig, OUTPUT);
  myservo.write(STRAIGHT);
} 

void self_drive_loop() {

  int move_flag = can_move(STOPPING_DIST_CM);
  Serial.print("move flag ");
  Serial.println(move_flag);
  if(move_flag == 0) {     
    stop();

    delay(500);                         
    myservo.write(RIGHT);          
    delay(1000);      
    rightDistance = Distance_test();

    delay(500);
    myservo.write(LEFT);              
    delay(1000);                                                  
    leftDistance = Distance_test();

    if((rightDistance < STOPPING_DIST_CM) && (leftDistance < STOPPING_DIST_CM)) {
      back();
      delay(1000);
    } 
    else {
      if(rightDistance > leftDistance) {
        turn_right();
      }
      else if(rightDistance < leftDistance) {
        turn_left();
      }
    }

  }  
  else {
    forward();
  }                     

}

void reset_self_drive() {
  myservo.write(STRAIGHT);
}
