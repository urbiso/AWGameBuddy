
#include <SPI.h>
#include "epd2in9.h"
#include "epdpaint.h"
#include "imagedata.h"
#include "Bricks.h"

#define COLORED     0
#define UNCOLORED   1

/**
  * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
  * In this case, a smaller image buffer is allocated and you have to 
  * update a partial display several times.
  * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
  */
unsigned char image[4736];
Paint paint(image, 0, 0);    // width should be the multiple of 8 
Epd epd;
unsigned long time_start_ms;
unsigned long time_now_s;

void setup() {
  
  delay(2000);
  // put your setup code here, to run once:
  //Serial.begin(9600);
  if (epd.Init(lut_full_update) != 0) {
    //  Serial.print("e-Paper init failed");
      return;
  }

  /** 
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to clear the frame memory twice.
   */
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();

  paint.SetWidth(128);
  paint.SetHeight(296);
  paint.Clear(UNCOLORED);

  //  paint.Clear(UNCOLORED);
  paint.DrawStringAt(2, 1, "Score:", &Font12, COLORED);
  paint.DrawStringAt(45, 0, "100000", &Font16, COLORED);

  paint.DrawStringAt(2, 16, "Lines:", &Font12, COLORED);
  paint.DrawStringAt(45, 15, "100", &Font16, COLORED);

  paint.DrawStringAt(2, 32, "Level:", &Font12, COLORED);
  paint.DrawStringAt(45, 31, "01", &Font16, COLORED);

  paint.DrawRectangle(94,15,126,47, COLORED);


  paint.DrawRectangle(0,49,126,295, COLORED);
  paint.DrawRectangle(1,50,125,294, COLORED);
  
  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());

  epd.DisplayFrame();

  delay(2000);

  if (epd.Init(lut_partial_update) != 0) {
   //   Serial.print("e-Paper init failed");
      return;
  }

  /** 
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to set the frame memory and refresh the display twice.
   */


  time_start_ms = millis();
}


  unsigned int count = 0;
void loop() {


   
  for(int i =0; i< 4; i++){
    if(count > 0)
      paint.DrawFilledRectangle(95+i*7+2,16+(count/4-1)*7+2,95+i*7+7,16+(count/4-1)*7+7, UNCOLORED);    
    else
      paint.DrawFilledRectangle(95+i*7+2,16+4*7+2,95+i*7+7,6+4*7+7, UNCOLORED);
    paint.DrawFilledRectangle(95+i*7+2,16+count/4*7+2,95+i*7+7,16+count/4*7+7, COLORED);
  }

  for(int i =0; i< 10; i++){
    if(count > 0)
      paint.DrawFilledRectangle(2+i*12+2,51+(count-1)*12+2,(2+i*12)+12,(51+(count-1)*12)+12, UNCOLORED);
    else
      paint.DrawFilledRectangle(2+i*12+2,51+(19)*12+2,(2+i*12)+12,(51+(19)*12)+12, UNCOLORED);
     paint.DrawFilledRectangle(2+i*12+2,51+count*12+2,(2+i*12)+12,(51+count*12)+12, COLORED);
    }

  count++;
  if(count >= 20)
    count = 0;
  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
  delay(100);
}
