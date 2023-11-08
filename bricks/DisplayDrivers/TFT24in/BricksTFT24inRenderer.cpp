#include "SPI.h"
#include <Adafruit_GFX.h>
#include "Adafruit_ILI9341.h"
#include "TFT24inRenderer.h"
#include "Bricks.h"

#define TFT_DC 4
#define TFT_CS 0
#define TFT_MOSI 13
#define TFT_CLK 14
#define TFT_RST 5
#define TFT_MISO 12

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

unsigned int linesPrevious;
unsigned long scorePrevious;
unsigned int levelPrevious;
 
unsigned char previousTable[MAX_Y*MAX_X] = {
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0
};

void initFrameBuffer(){

  linesPrevious = 0;
  scorePrevious = 0;
  levelPrevious = 1;
  
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.drawRect(0, 0, 320,160, ILI9341_DARKGREY);

  tft.setRotation(3);
  
  tft.setCursor(165, 10);  
  tft.setTextColor(ILI9341_DARKGREY); 
  tft.setTextSize(2);
  tft.println("Score");



  tft.setCursor(165, 55);  
  tft.setTextColor(ILI9341_DARKGREY); 
  tft.setTextSize(2);
  tft.println("Lines");
  


  tft.drawRect(170, 104, 66,66, ILI9341_DARKGREY);

  tft.setCursor(165, 177);  
  tft.setTextColor(ILI9341_DARKGREY); 
  tft.setTextSize(2);
  tft.println("Level");
  
  refreshStats(linesPrevious,scorePrevious,levelPrevious);

  Serial.print("TFT 2.4in display init - finished");
  
}

void renderFrameBuffer()
{

}



void refreshStats(unsigned int linesNew,unsigned long scoreNew, unsigned int levelNew)
{
  tft.setCursor(165, 33);  
  tft.setTextColor(ILI9341_BLACK); 
  tft.setTextSize(2);
  tft.println(String(scorePrevious));
  
  tft.setCursor(165, 78);  
  tft.setTextColor(ILI9341_BLACK); 
  tft.setTextSize(2);
  tft.println(String(linesPrevious));
  
  tft.setCursor(165, 200);  
  tft.setTextColor(ILI9341_BLACK); 
  tft.setTextSize(2);
  tft.println(String(levelPrevious));
  

  tft.setCursor(165, 33);  
  tft.setTextColor(ILI9341_WHITE); 
  tft.setTextSize(2);
  tft.println(String(scoreNew));
  
  tft.setCursor(165, 78);  
  tft.setTextColor(ILI9341_WHITE); 
  tft.setTextSize(2);
  tft.println(String(linesNew));
  
  tft.setCursor(165, 200);  
  tft.setTextColor(ILI9341_WHITE); 
  tft.setTextSize(2);
  tft.println(String(levelNew));

  linesPrevious = linesNew;
  scorePrevious = scoreNew;
  levelPrevious = levelNew;
}

unsigned int newItemFieldZeroX = 173;
unsigned int newItemFieldZeroY = 107;

unsigned int itemFieldZeroX = 1;
unsigned int itemFieldZeroY = 1;

unsigned char brickSize = 14;
unsigned char space = 2;


void renderNextItemField(unsigned char *nextItemField, unsigned char maxX)
{   
  for(unsigned char j = 0; j < maxX; j++)
    for(unsigned char i = 0; i < maxX; i++)
    {      
        if(nextItemField[j*maxX+i] == CELL_STATE_EMPTY)
            tft.fillRect(newItemFieldZeroX + (brickSize+space)*i, newItemFieldZeroY + (brickSize+space)*j, brickSize,brickSize, ILI9341_BLACK);
        else
            tft.fillRect(newItemFieldZeroX + (brickSize+space)*i, newItemFieldZeroY + (brickSize+space)*j, brickSize,brickSize, ILI9341_WHITE);
    }   
}

void renderPlayField(unsigned char *table, unsigned char maxY, unsigned char maxX)
{  
  //renderTableIntoSerial("Previous table",previousTable,maxX);
  //renderTableIntoSerial("Table",table,maxX);
  
  for(unsigned char j = 0; j < maxY; j++)
    for(unsigned char i = 0; i < maxX; i++)
    {   
        if(table[j*maxX+i] != previousTable[j*maxX+i])
        {  
          tft.fillRect(itemFieldZeroX + (brickSize+space)*i, itemFieldZeroY + (brickSize+space)*j, brickSize,brickSize, ILI9341_BLACK);
          if(table[j*maxX+i] != CELL_STATE_EMPTY)
             tft.fillRect(itemFieldZeroX + (brickSize+space)*i, itemFieldZeroY + (brickSize+space)*j, brickSize,brickSize, ILI9341_WHITE);   
          previousTable[j*maxX+i] = table[j*maxX+i];        
        }
        
    } 
}

void showGameOver()
{
  char *gameOver = "GAME OVER !!!";

  tft.fillRect(1,170, 159,25, ILI9341_BLACK);
  tft.drawRect(1,170, 158,24, ILI9341_DARKGREY);

  tft.setCursor(5, 175);  
  tft.setTextColor(ILI9341_WHITE); 
  tft.setTextSize(2);
  tft.println(gameOver);
 // paint.DrawFilledRectangle(10,150,120,170,UNCOLORED);
 
 // paint.DrawStringAt(15, 155,  gameOver , &Font16, COLORED);
 
}
