// ********************************************************************
// eMotor controller V01
// for Computux 2022
//
// ********************************************************************
#include <SPI.h>
#include <mcp2515.h>
#include "canDevice.h"
#include "Timer.h"

// CAN device ID
#define CAN_SLAVE_ID    0xF6

// normalize accelerator value
#define ACC_MAX             870
#define ACC_MIN             170

// GPIO define
#define BTN_HEADLIGHT       2
#define BTN_TURNLEFT        3
#define BTN_TURNRIGHT       4
#define PIN_ACCELERATOR     A0

#define LEDPIN_TURNRIGHT    7
#define LEDPIN_TURNLEFT     6
#define LEDPIN_HEADLIGHT    5

// temp
// led pin define
#define ledPin              13

// switch status
int HeadlightState = 0;
int TurnrightState = 0;
int TurnleftState = 0;
int TrunRightLightStatus = 0;
int TrunLeftLightStatus = 0;

// accelerator value
int needle_index = 0;

// timer to handle turn light flash
Timer t;
int tickEvent;

// CAN BUS
struct can_frame canMsg1;
MCP2515 mcp2515(10);
// CAN BUS format
// [0] : needle index [0:100]
// [1] : headlight status. 0->off, 1->on
// [2] : turn right light status. 0->off, 1->on
// [3] : turn left light status. 0->off, 1->on

void setup() {
  Serial.begin(115200);
  
  // initialize the pushbutton pin as an input:
  pinMode(BTN_HEADLIGHT, INPUT);
  pinMode(BTN_TURNRIGHT, INPUT);
  pinMode(BTN_TURNLEFT, INPUT);

  pinMode(LEDPIN_HEADLIGHT, OUTPUT);
  pinMode(LEDPIN_TURNRIGHT, OUTPUT);
  pinMode(LEDPIN_TURNLEFT, OUTPUT);

  digitalWrite(LEDPIN_HEADLIGHT, LOW);
  digitalWrite(LEDPIN_TURNRIGHT, LOW);
  digitalWrite(LEDPIN_TURNLEFT, LOW);

  tickEvent = t.every(500, procTimerCallback);
  Serial.print("2 second tick started id=");
  Serial.println(tickEvent);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  canMsg1.can_id  = CAN_SLAVE_ID;
  canMsg1.can_dlc = 4;
}

void loop() {
  needle_index = analogRead(PIN_ACCELERATOR);
  needle_index = AccCal(needle_index);
  
  // read the state of the pushbutton value:
  HeadlightState = digitalRead(BTN_HEADLIGHT);
  TurnrightState = digitalRead(BTN_TURNRIGHT);
  TurnleftState = digitalRead(BTN_TURNLEFT);

#if 1
  if (HeadlightState == LOW) {
    // turn LED off:
    digitalWrite(LEDPIN_HEADLIGHT, LOW);
  } else {
    // turn LED on:
    digitalWrite(LEDPIN_HEADLIGHT, HIGH);
  }
#endif

  
  
  if( TurnrightState == HIGH ) {
    TrunRightLightStatus = 0;
    digitalWrite(LEDPIN_TURNRIGHT, LOW);
  }
  
  if( TurnleftState == HIGH ) {
    TrunLeftLightStatus = 0;
    digitalWrite(LEDPIN_TURNLEFT, LOW);
  }

  canMsg1.data[0] = needle_index;   // needle
  canMsg1.data[1] = HeadlightState;
  canMsg1.data[2] = TrunRightLightStatus;
  canMsg1.data[3] = TrunLeftLightStatus;

  Serial.print(HeadlightState);
  Serial.print(":");
  Serial.println(canMsg1.data[1]);

#if 0
  Serial.print(canMsg1.can_id, HEX);
  Serial.print(":");
  Serial.print(canMsg1.data[0]);
  for(int i=1; i<4; i++) {
     Serial.print(canMsg1.data[i], HEX);
     Serial.print(" ");
  }
  Serial.println();
#endif

  mcp2515.sendMessage(&canMsg1);

  delay(100);
  
  t.update();
}

//
// timer callback procedure
// turn light control and send
//
void procTimerCallback()
{
  if( TurnrightState == LOW ) {
    //*****digitalWrite(ledPin, !digitalRead(ledPin));
    TrunRightLightStatus = !TrunRightLightStatus;
    digitalWrite(LEDPIN_TURNRIGHT, !digitalRead(LEDPIN_TURNRIGHT));
  }
  if( TurnleftState == LOW ) {
    //*****digitalWrite(ledPin, !digitalRead(ledPin));
    TrunLeftLightStatus = !TrunLeftLightStatus;
    digitalWrite(LEDPIN_TURNLEFT, !digitalRead(LEDPIN_TURNLEFT));
  }
}

//
// normallize acceleraror value
//
int AccCal(int val) {
  int v;
  int ret;
  
  if( val < ACC_MIN ) {
    v = ACC_MIN;
  }else if( val > ACC_MAX ) {
    v = ACC_MAX+15;
  } else {
    v = val;
  }

  ret = ((10000 / (ACC_MAX-ACC_MIN)) * (v - ACC_MIN)) / 100;
  if( ret > 100 ) {
    ret = 100;
  }
  return ret;
}
