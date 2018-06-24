#include <Arduino.h>
#include <IRremote.h>
#include "common.h"
#include "remote_control.h"

#define RECV_PIN  12

IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long preMillis;


void setup_remote() {
  irrecv.enableIRIn();  
}

long get_remote_value() {
  long val = 0;
  if (irrecv.decode(&results)){ 
    val = results.value;
    irrecv.resume();
  }
  Serial.println(val);
  return val;
}

void remote_move(long val) {
  if (val){ 
    preMillis = millis();
    switch(val){
      case F: forward(); break;
      case R: right(); break;
      case B: back(); break;
      case L: left(); break;
      case S: stop(); break;

      default: break;
    }
  }
  else{
    if(millis() - preMillis > 500){
      stop();
      preMillis = millis();
    }
  }
} 


