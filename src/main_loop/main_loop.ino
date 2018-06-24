#include <IRremote.h>
#include <Servo.h>


#include "common.h"
#include "remote_control.h"
#include "self_drive.h"

int self_drive = 0;

void setup() {
  setup_common();
  setup_remote();
  setup_self_drive();
}

void loop() {
  delay(100);
  long val = get_remote_value();

  toggle_drive_mode(val);

  if (self_drive) {
    self_drive_loop();
  } else {
    reset_self_drive();
    remote_move(val);
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


