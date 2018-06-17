//www.elegoo.com

#include <IRremote.h>
#include <Servo.h>  //servo library
Servo myservo;      // create servo object to control servo

////////// IR REMOTE CODES //////////
#define F 16736925
#define R 16761405
#define B 16754775
#define L 16720605
#define S 16712445
#define B1 16738455
#define B2 16750695
#define B3 16756815
#define B4 16724175
#define B5 16718055
#define B6 16743045
#define B7 16716015
#define B8 16726215
#define B9 16734885
#define B_STAR 16728765
#define B0 16730805
#define B_HASH 16732845

#define RECV_PIN  12

/*define channel enable output pins*/
#define ENA 5	  // Left  wheel speed
#define ENB 6	  // Right wheel speed
/*define logic control output pins*/
#define IN1 7	  // Left  wheel forward
#define IN2 8	  // Left  wheel reverse
#define IN3 9	  // Right wheel reverse
#define IN4 11	// Right wheel forward
#define carSpeed 150	// initial speed of car >=0 to <=255

IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long val;
unsigned long preMillis;

int Echo = A4;  
int Trig = A5; 
int rightDistance = 0, leftDistance = 0, middleDistance = 0;
int STOPPING_DIST_CM = 50;
int STRAIGHT = 90;
int RIGHT = 0;
int LEFT = 180;

int self_drive = 0;


/**
 * BEGIN DEFINE FUNCTIONS
 */

 void forward(){ 
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  Serial.println("go forward!");
}
void back(){
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  Serial.println("go back!");
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
void right(){
  analogWrite(ENA,carSpeed);
  analogWrite(ENB,carSpeed);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  Serial.println("go right!");
}
void stop(){
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("STOP!");  
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
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  stop();
  irrecv.enableIRIn();  
}

void loop() {
  myservo.write(90);  //setservo position according to scaled value

  if (irrecv.decode(&results)){ 
    preMillis = millis();
    val = results.value;
    irrecv.resume();

    toggle_drive_mode(val);
    if (!self_drive)
      remote_drive_loop(val);
  }
  if (self_drive) {
    self_drive_loop();
  } else{
    if(millis() - preMillis > 500){
      stop();
      preMillis = millis();
    }
  }
} 

void self_drive_loop() {
  Serial.println("self drive loop");
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
    } 
    else {
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
/**
any keypress other than B2, exit self drive mode
**/
void toggle_drive_mode(int val) {

  Serial.println("toggle drive mode");
    Serial.println(val);
    switch(val){
      case F: self_drive = 0; break;
      case R: self_drive = 0; break;
      case B: self_drive = 0; break;
      case L: self_drive = 0; break;
      case S: self_drive = 0; break;
      case B1: self_drive = 0; break;
      case B2: self_drive = 1; break;


      default: break;
    }
    Serial.print("drive mode ");
    Serial.println(self_drive);
}

void remote_drive_loop(int val) {

    Serial.println("remote drive loop");
    Serial.println(val);
    switch(val){
      case F: forward(); break;
      case R: right(); break;
      case B: back(); break;
      case L: left(); break;
      case S: stop(); break;

      default: break;
    }
}

