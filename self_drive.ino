//www.elegoo.com

#include <Servo.h>  //servo library
Servo myservo;      // create servo object to control servo

int Echo = A4;  
int Trig = A5; 

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define carSpeed 150
int rightDistance = 0, leftDistance = 0, middleDistance = 0;
int STOPPING_DIST_CM = 50;
int STRAIGHT = 90;
int RIGHT = 0;
int LEFT = 180;

void forward(){ 
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Forward");
}

void back() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Back");
}

void left() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); 
  Serial.println("Left");
}

void right() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Right");
}

void stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("Stop!");
} 

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

int unit_distance(int x) {
  Serial.print("Unit Distance ");
  Serial.println(x);
  myservo.write(x);
  delay(300);
  int dist = Distance_test();
  Serial.print("dist ");
  Serial.println(dist);
  return dist;
}

void setup() { 
  myservo.attach(3);  // attach servo on pin 3 to servo object
  Serial.begin(9600);     
  pinMode(Echo, INPUT);    
  pinMode(Trig, OUTPUT);  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  stop();
} 

void loop() { 
    myservo.write(90);  //setservo position according to scaled value
    delay(500);

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
      } else {
        if(rightDistance > leftDistance) {
          right();
          delay(360);
        }
        else if(rightDistance < leftDistance) {
          left();
          delay(360);
        }
      }

    }  
    else {
        forward();
    }                     
}

void loop1() {
  
  myservo.write(90);
  Serial.println(90);
  delay(5000);
  
  myservo.write(0);
  Serial.println(0);
  delay(5000);
  
  myservo.write(180);
  Serial.println(180);
  delay(5000);
}

