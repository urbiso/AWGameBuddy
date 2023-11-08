
#include "Defines.h"
#include "Bricks.h"
#include "Interface.h"

unsigned long time_start_ms;
unsigned long time_now_s;

void setup() {  
  delay(2000);
  Serial.begin(9600);
  initDisplay();
  resetGame();
  time_start_ms = millis();
  pinMode(A0, INPUT);
  randomSeed(time_start_ms);
}


// ---- Buttons codes ------
#define ZEROBTN  0
#define ONEBTN  1
#define TWOBTN  2
#define THREEBTN 3
#define FOURBTN  4
#define FIVEBTN  5
#define SIXBTN  6
#define SEVENBTN  7
#define EIGHTBTN  8
#define NINEBTN  9

#define LEFTBTN 10
#define RIGHTBTN 11
#define DOWNBTN 12
#define UPBTN 13
#define ACTION1BTN 14
#define ACTION2BTN 15
#define SELECTBTN 16
#define RESETBTN 17

#define UNPRESSEDBTN -1
// -----------------------

#define INITIALPRESSDELAY 120
#define REPEATPRESSDELAY 50

#define MAINLOOPDELAY 10

// ---- Buttons resistance values ------
#define LEFTBTNVALUE 557
#define RIGHTBTNVALUE 365
#define UPBTNVALUE 656
#define DOWNBTNVALUE 450
#define ACTION1BTNVALUE 405
#define ACTION2BTNVALUE 334
#define LEFTA1BTNVALUE 673
#define LEFTA2BTNVALUE 648
#define RIGHTA1BTNVALUE 568
#define RIGHTA2BTNVALUE 525
#define DOWNA1BTNVALUE 610
#define DOWNA2BTNVALUE 575
#define UPA1BTNVALUE 739
#define UPA2BTNVALUE 720
#define LEFTUPBTNVALUE 784
#define LEFTDOWNBTNVALUE 689
#define RIGHTUPBTNVALUE 728
#define RIGHTDOWNBNTVALUE 590

#define SELECTBTNVALUE 300
#define RESETBTNVALUE 955 
// ------------------------------------

#define VALUEINTERVAL 10
#define VALUEINTERVALNARROW 5




long keyChangeTimeStamp = 0;
int previousButton =  UNPRESSEDBTN;


bool isPressEvent(int button)
{
  if(previousButton != button)
  {
    keyChangeTimeStamp = millis();
    previousButton = button;
    return true;
  }
  if(previousButton == button)
  {     
    if((millis() - keyChangeTimeStamp) > INITIALPRESSDELAY)
    {
      keyChangeTimeStamp = millis();
      return true;
    }
  }
  return false;
}

void decodeKeyFromVoltage(int value)
{
  // UnComment for keys resistance troubleshooting via SerialMonitor       
  Serial.println(String(value));
  
  if(value > (LEFTBTNVALUE-VALUEINTERVAL) && value < (LEFTBTNVALUE+2))
  {
     if(isPressEvent(LEFTBTN))
        leftButtonClick();
  }
  else
  if(value > (DOWNBTNVALUE-VALUEINTERVAL) && value < (DOWNBTNVALUE+VALUEINTERVAL))
  {
     if(isPressEvent(DOWNBTN))
        downButtonClick();
  }
  else
  if(value > (RIGHTBTNVALUE-VALUEINTERVAL) && value < (RIGHTBTNVALUE+VALUEINTERVAL))
  {
     if(isPressEvent(RIGHTBTN))
        rightButtonClick();
  }
  else
   if(value > (UPBTNVALUE-VALUEINTERVAL) && value < (UPBTNVALUE+VALUEINTERVAL))
  {
     if(isPressEvent(UPBTN))
        upButtonClick();
  }
  else
  if(value > (ACTION1BTNVALUE-VALUEINTERVAL) && value < (ACTION1BTNVALUE+VALUEINTERVAL))
  {
     if(isPressEvent(ACTION1BTN))
       action1ButtonClick();
  }
  else
  if(value > (ACTION2BTNVALUE-VALUEINTERVAL) && value < (ACTION2BTNVALUE+VALUEINTERVAL))
  {
    if(isPressEvent(ACTION2BTN))
     action2ButtonClick();
  }
  else
   if(value > (RESETBTNVALUE-VALUEINTERVAL) && value < (RESETBTNVALUE+VALUEINTERVAL))
  {
    if(isPressEvent(RESETBTN))
     resetButtonClick();
  }
  else
   if(value > (SELECTBTNVALUE-VALUEINTERVAL) && value < (SELECTBTNVALUE+VALUEINTERVAL)) 
  {
    if(isPressEvent(SELECTBTN))
     selectButtonClick();
  }
  else
   if(value > (LEFTA1BTNVALUE-VALUEINTERVAL) && value < (LEFTA1BTNVALUE+VALUEINTERVAL)) 
  {
    if(isPressEvent(LEFTA1BTNVALUE))
     leftAction1ButtonClick();
  }
  else
   if(value > (LEFTA2BTNVALUE-VALUEINTERVAL) && value < (LEFTA2BTNVALUE+VALUEINTERVAL)) 
  {
    if(isPressEvent(LEFTA2BTNVALUE))
     leftAction2ButtonClick();
  }
  else
   if(value > (RIGHTA1BTNVALUE-2) && value < (RIGHTA1BTNVALUE+VALUEINTERVALNARROW)) 
  {
    if(isPressEvent(RIGHTA1BTNVALUE))
     rightAction1ButtonClick();
  }
  else
   if(value > (RIGHTA2BTNVALUE-VALUEINTERVAL) && value < (RIGHTA2BTNVALUE+VALUEINTERVAL)) 
  {
    if(isPressEvent(RIGHTA2BTNVALUE))
     rightAction2ButtonClick();
  }
  else
  if(value > (DOWNA1BTNVALUE-VALUEINTERVAL) && value < (DOWNA1BTNVALUE+VALUEINTERVAL)) 
  {
    if(isPressEvent(DOWNA1BTNVALUE))
     downAction1ButtonClick();
  }
  else
  if(value > (DOWNA2BTNVALUE-VALUEINTERVALNARROW) && value < (DOWNA2BTNVALUE+VALUEINTERVALNARROW)) 
  {
    if(isPressEvent(DOWNA2BTNVALUE))
     downAction2ButtonClick();
  }
  else
  if(value > (UPA1BTNVALUE-4) && value < (UPA1BTNVALUE+VALUEINTERVAL)) 
  {
    if(isPressEvent(UPA1BTNVALUE))
     upAction1ButtonClick();
  }
  else
  if(value > (UPA2BTNVALUE -VALUEINTERVAL) && value < (UPA2BTNVALUE +VALUEINTERVAL)) 
  {
    if(isPressEvent(DOWNA2BTNVALUE))
     upAction2ButtonClick();
  }
  else
  if(value > (LEFTUPBTNVALUE -VALUEINTERVAL) && value < (LEFTUPBTNVALUE+VALUEINTERVAL)) 
  {
    if(isPressEvent(LEFTUPBTNVALUE))
     leftUpButtonClick();
  }
  else
  if(value > (LEFTDOWNBTNVALUE -VALUEINTERVAL) && value < (LEFTDOWNBTNVALUE +VALUEINTERVAL)) 
  {
    if(isPressEvent(LEFTDOWNBTNVALUE))
     leftDownButtonClick();
  }
  else
  if(value > (RIGHTUPBTNVALUE -3) && value < (RIGHTUPBTNVALUE +VALUEINTERVALNARROW)) 
  {
    if(isPressEvent(RIGHTUPBTNVALUE))
     rightUpButtonClick();
  }
  else
  if(value > (RIGHTDOWNBNTVALUE -VALUEINTERVALNARROW) && value < (RIGHTDOWNBNTVALUE +VALUEINTERVALNARROW)) 
  {
    if(isPressEvent(RIGHTDOWNBNTVALUE))
     rightDownButtonClick();
  }
  else
  if(value < 300)
  {
    previousButton = UNPRESSEDBTN;
    keyChangeTimeStamp = 0;
  }
  
  
}


long previousTimeStamp = millis();;


void loop() {

  if((millis() - previousTimeStamp) > (500/getLevel()))
  {
      moveItemDown();
      previousTimeStamp = millis();
  }
  decodeKeyFromVoltage(analogRead(A0));

  delay(MAINLOOPDELAY);
  
}
