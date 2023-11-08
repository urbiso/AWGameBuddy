#include <SPI.h>
#include "epd2in9.h"
#include "epdpaint.h"
#include "imagedata.h"
#include "Bricks.h"
#include "eInkBricksRender.h"



unsigned char image[4736];
Paint paint(image, 0, 0);    // width should be the multiple of 8 
Epd epd;
 
void initDisplay(){


  if (epd.Init(lut_full_update) != 0) {
     Serial.print("e-Paper init failed");
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
  
  renderFrameBuffer();

  delay(2000);

  if (epd.Init(lut_partial_update) != 0) {
      Serial.print("e-Paper init failed");
     return;
  }
  Serial.print("e-Paper - initialized");
  
}

void renderFrameBuffer()
{
  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
}


void refreshStats(unsigned int lines,unsigned long score, unsigned int speed)
{
  char scoreStr[7] = "";
  char linesStr[4] = "";
  char speedStr[3] = ""; 
  
  sprintf(scoreStr, "%l", score);
  sprintf(linesStr, "%u", lines);
  sprintf(speedStr, "%u", speed);
  
  paint.DrawFilledRectangle(45,0,128,12,UNCOLORED);
  paint.DrawFilledRectangle(45,15,92,27,UNCOLORED);
  paint.DrawFilledRectangle(45,31,92,43,UNCOLORED);
  
  paint.DrawStringAt(45, 0,  scoreStr , &Font16, COLORED);
  paint.DrawStringAt(45, 15, linesStr, &Font16, COLORED);
  paint.DrawStringAt(45, 31, speedStr, &Font16, COLORED);
}

void renderNextItemField(unsigned char *nextItemField, unsigned char maxY, unsigned char maxX)
{

  //  for(int i =0; i< 4; i++){
//    if(count > 0)
//      paint.DrawFilledRectangle(95+i*7+2,16+(count/4-1)*7+2,95+i*7+7,16+(count/4-1)*7+7, UNCOLORED);    
//    else
//      paint.DrawFilledRectangle(95+i*7+2,16+4*7+2,95+i*7+7,6+4*7+7, UNCOLORED);
//    paint.DrawFilledRectangle(95+i*7+2,16+count/4*7+2,95+i*7+7,16+count/4*7+7, COLORED);
  for(unsigned char j = 0; j < maxY; j++)
    for(unsigned char i = 0; i < maxX; i++)
    {      
        if(nextItemField[j*maxX+i] == CELL_STATE_EMPTY)
          paint.DrawFilledRectangle(95+i*7+2,16+j*7+2,95+i*7+7,16+j*7+7, UNCOLORED);   
        else
          paint.DrawFilledRectangle(95+i*7+2,16+j*7+2,95+i*7+7,16+j*7+7, COLORED);
    } 
  
}

void renderPlayField(unsigned char *table, unsigned char maxY, unsigned char maxX)
{

  
  for(unsigned char j = 0; j < maxY; j++)
    for(unsigned char i = 0; i < maxX; i++)
    {
      
        if(table[j*maxX+i] == CELL_STATE_EMPTY)
          paint.DrawFilledRectangle(2+i*12+2,51+j*12+2,(2+i*12)+12,(51+j*12)+12, UNCOLORED);
        else
          paint.DrawFilledRectangle(2+i*12+2,51+j*12+2,(2+i*12)+12,(51+j*12)+12, COLORED);
    } 

}

void showGameOver()
{
  char *gameOver = "GAME OVER !!!";

  paint.DrawFilledRectangle(10,150,120,170,UNCOLORED);
 
  paint.DrawStringAt(15, 155,  gameOver , &Font16, COLORED);
 
}



unsigned count = 0;
void testLoop()
{
  
//   
//  for(int i =0; i< 4; i++){
//    if(count > 0)
//      paint.DrawFilledRectangle(95+i*7+2,16+(count/4-1)*7+2,95+i*7+7,16+(count/4-1)*7+7, UNCOLORED);    
//    else
//      paint.DrawFilledRectangle(95+i*7+2,16+4*7+2,95+i*7+7,6+4*7+7, UNCOLORED);
//    paint.DrawFilledRectangle(95+i*7+2,16+count/4*7+2,95+i*7+7,16+count/4*7+7, COLORED);
//  }

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
}
