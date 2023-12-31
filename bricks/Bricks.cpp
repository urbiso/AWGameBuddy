#include <iostream>
#include <cstring>
#include "Arduino.h"
#include "Defines.h"
#include "Bricks.h"
#include "Interface.h"



#if defined(EINK29IN)
  #include "eInk29inRenderer.h"
#elif defined(TFT20IN)
  #include "TFT20inRenderer.h"
#elif defined(TFT24IN)
  #include "DisplayDrivers/TFT24in/BricksRenderer.h"
#elif defined(TFT35IN)
  #include "DisplayDrivers/TFT35in/BricksRenderer.h"
#endif


unsigned char table[MAX_Y*MAX_X] = {
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

unsigned char newItemTable[4*4] = {
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0
};

unsigned char tempItemTable[4*4] = {
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0
};

unsigned char itemI[4][8] = {
  2,0,1,4,2,2,2,2,
  0,2,4,1,2,2,2,2,
  2,0,1,4,2,2,2,2,
  0,2,4,1,2,2,2,2
};

unsigned char itemLR[4][10] = {
  0,1,3,2,2,0,2,0,2,2,
  1,1,2,3,0,0,2,2,2,2,
  1,1,3,2,2,2,0,2,0,2,
  1,0,2,3,2,2,2,2,0,0
};

unsigned char itemLL[4][10] = {
  1,1,3,2,0,2,0,2,2,2,
  1,0,2,3,2,2,2,0,0,2,
  0,1,3,2,2,2,2,0,2,0,
  1,1,2,3,2,0,0,2,2,2
};

unsigned char itemZR[4][10] = {
  1,0,2,3,2,2,0,0,2,2,
  0,1,3,2,0,2,2,2,2,0,
  1,1,2,3,2,2,0,0,2,2,
  0,1,3,2,0,2,2,2,2,0
};

unsigned char itemZL[4][10] = {
  1,0,2,3,0,2,2,2,2,0,
  1,0,3,2,2,0,2,2,0,2,
  1,0,2,3,0,2,2,2,2,0,
  0,1,3,2,2,0,2,2,0,2
};

unsigned char itemO[4][8] = {
  0,0,2,2,2,2,2,2,
  0,0,2,2,2,2,2,2,
  0,0,2,2,2,2,2,2,
  0,0,2,2,2,2,2,2
};

unsigned char itemT[4][10] = {
  1,1,2,3,0,2,0,2,2,2,
  1,1,3,2,0,2,2,2,0,2,
  1,0,2,3,2,2,2,0,2,0,
  0,1,3,2,2,0,2,2,2,0
};

unsigned char item = 0;



unsigned long score = 0;
unsigned int lines = 0;
unsigned char level = 0;


unsigned char currentShape = 0;
unsigned char currentRotation = 0;
unsigned char newShape = 0;
unsigned char newRotation = 0;
unsigned char shapeCenterX = 0;
unsigned char shapeCenterY = 0;
bool isGameOver = false;

void renderTableIntoSerial(String message,unsigned char *itemToInsert,int columns)
{  int rows = MAX_Y; 

  if(columns == 4)
    rows = 4;
   Serial.print("----------------------------\n");
   Serial.print(String(message + "\n"));
   Serial.print(String("rows: " + String(rows) + " columns: " + String(columns) +"\n"));
    for(int j = 0; j < rows; j++)
    {
      for(int i = 0; i < columns; i++)
      {
        Serial.print(String(String(itemToInsert[j*columns + i]) + " "));
      }
      Serial.print("\n");
    }
      
}

void renderItemTableIntoSerial(String message,unsigned char *itemToInsert)
{  

   Serial.print("----------------------------\n");
   Serial.print(String(message + "\n"));
   Serial.print(String("rows: " + String(itemToInsert[3]) + " columns: " + String(itemToInsert[2]) +"\n"));
    for(int j = 0; j < itemToInsert[ROW_COUNT_INDEX]; j++)
    {
      for(int i = 0; i < itemToInsert[COLUMN_COUNT_INDEX]; i++)
      {
        Serial.print(String(String(itemToInsert[MATRIX_START_INDEX +j*itemToInsert[COLUMN_COUNT_INDEX] + i]) + " "));
      }
      Serial.print("\n");
    }
      
}



void copyItemToTable(unsigned int itemInserPositionX, unsigned char *itemToInsert, unsigned char *targetTable, unsigned char columnNum)
{
// renderTableIntoSerial("copyItemToTable targetTable before insert",targetTable,columnNum);
  
  for (unsigned char j = 0; j < itemToInsert[ROW_COUNT_INDEX]; j++)
    for (unsigned char i = 0; i < itemToInsert[COLUMN_COUNT_INDEX]; i++)
    {
      if (itemToInsert[MATRIX_START_INDEX + j * itemToInsert[COLUMN_COUNT_INDEX] + i] == CELL_STATE_MOVING)
      {
        targetTable[j*columnNum + itemInserPositionX + i] = CELL_STATE_MOVING;
      }
    }
// renderTableIntoSerial("copyItemToTable targetTable after insert",targetTable,columnNum);
}

void clearNewItemField()
{
    std::memset(newItemTable, 0, sizeof(newItemTable));
}

unsigned char *toShapeWithRotation(unsigned char shape, unsigned char rotation)
{
    switch(shape){
        case SHAPE_I:
            return itemI[rotation];
        case SHAPE_LL:
            return itemLL[rotation];
        case SHAPE_LR:
            return itemLR[rotation];
        case SHAPE_ZL:
            return itemZL[rotation];
        case SHAPE_ZR:
            return itemZR[rotation];
        case SHAPE_O:
            return itemO[rotation];
        case SHAPE_T:
            return itemT[rotation];       
    }
	
}

long randomSeedInit =631;

void setRandomSeed(long randomSeedInitInput)
{
  randomSeedInit = randomSeedInitInput;
}
unsigned char *generateNewItem()
{
    newShape = random(0,7);
    newRotation = random(0,4);

    return toShapeWithRotation(newShape,newRotation);
}

bool isFixedItemCollision(int offsetX, int offsetY, unsigned char shapeCenterX, unsigned char shapeCenterY, unsigned char shape, unsigned char rotation)
{
  unsigned char newCenterItemX = shapeCenterX + offsetX;
  unsigned char newCenterItemY = shapeCenterY + offsetY;
  short newX = 0;
  short newY = 0;
 // Serial.print(String("offsetX: " + String(offsetX) + " offsetY: " + String(offsetY) + " shapeCenterX: " + String(shapeCenterX) + " shapeCenterY: " + String(shapeCenterY) + " newCenterItemX: " + String(newCenterItemX) + " newCenterItemY: " + String(newCenterItemY) + "\n"));
  unsigned char *shapeWithRotation = toShapeWithRotation(shape,rotation);

  for(unsigned char j = 0; j < shapeWithRotation[ROW_COUNT_INDEX]; j++)
  {
     newY = newCenterItemY + (j - shapeWithRotation[SHAPE_CENTER_Y_INDEX]);
    
     //Serial.print(String(" --- newY: " + String(newY) + "\n"));
  
    for(unsigned char i = 0; i < shapeWithRotation[COLUMN_COUNT_INDEX]; i++)
    {       
        if(shapeWithRotation[MATRIX_START_INDEX+j* shapeWithRotation[COLUMN_COUNT_INDEX]+i] == CELL_STATE_MOVING)
        {
            newX = newCenterItemX + (i - shapeWithRotation[SHAPE_CENTER_X_INDEX]);          
        //    Serial.print(String("      newX: " + String(newX) + "\n"));
            if(newX < 0 || newX >= MAX_X || newY >= MAX_Y || table[newY*MAX_X+newX] == CELL_STATE_FIXED )
            {
           //  if( table[newY*MAX_X+newX] == CELL_STATE_FIXED )
              //   Serial.print("Matrix Collission\n"); 
             return true;
             
            }
        }
    }
  }
  return false;
}


void moveShape(short offsetX, short offsetY)
{
  if(isGameOver)
    return;
   char newCenterItemX = shapeCenterX + offsetX;
   char newCenterItemY = shapeCenterY + offsetY;

   //renderTableIntoSerial(" moveShape before change",table,MAX_X);
   unsigned char *shapeWithRotation = toShapeWithRotation(currentShape,currentRotation);

   for(unsigned char j = 0; j < shapeWithRotation[ROW_COUNT_INDEX]; j++)
        for(unsigned char i = 0; i < shapeWithRotation[COLUMN_COUNT_INDEX]; i++)
        {
            if(shapeWithRotation[MATRIX_START_INDEX+j*shapeWithRotation[COLUMN_COUNT_INDEX] + i] == CELL_STATE_MOVING)
                table[(shapeCenterY+(j-shapeWithRotation[SHAPE_CENTER_Y_INDEX])) * MAX_X  + (shapeCenterX + (i-shapeWithRotation[SHAPE_CENTER_X_INDEX]))] = CELL_STATE_EMPTY;
        }
   //  renderTableIntoSerial(" moveShape after delete change",table,MAX_X);
    for(unsigned char j = 0; j < shapeWithRotation[ROW_COUNT_INDEX]; j++)
        for(unsigned char i = 0; i <shapeWithRotation[COLUMN_COUNT_INDEX]; i++)
        {
            if(shapeWithRotation[MATRIX_START_INDEX+j*shapeWithRotation[COLUMN_COUNT_INDEX] + i] == CELL_STATE_MOVING)
                table[(newCenterItemY+(j-shapeWithRotation[SHAPE_CENTER_Y_INDEX])) * MAX_X  + (newCenterItemX + (i-shapeWithRotation[SHAPE_CENTER_X_INDEX]))] = CELL_STATE_MOVING;
        }
    
    shapeCenterX = newCenterItemX;
    shapeCenterY = newCenterItemY;
   //   renderTableIntoSerial(" moveShape after render change",table,MAX_X);
   renderPlayField(table,MAX_Y,MAX_X);
}

void rotateShape()
{
    unsigned char newRotation = currentRotation + 1;
    if(newRotation > LAST_ROTATION)
        newRotation = FIRST_ROTATION;
    
    if(isFixedItemCollision(0,0,shapeCenterX,shapeCenterY,currentShape,newRotation))
        return;
  
	unsigned char *shapeWithRotation = toShapeWithRotation(currentShape, currentRotation);

  //renderItemTableIntoSerial("Before rotation",shapeWithRotation);
	currentRotation = newRotation;
	unsigned char *newShapeWithRotation = toShapeWithRotation(currentShape, currentRotation);
  //renderItemTableIntoSerial("After rotation",newShapeWithRotation);


	for (unsigned char j = 0; j < shapeWithRotation[ROW_COUNT_INDEX]; j++)
		for (unsigned char i = 0; i < shapeWithRotation[COLUMN_COUNT_INDEX]; i++)
		{
			if (shapeWithRotation[MATRIX_START_INDEX + j*shapeWithRotation[COLUMN_COUNT_INDEX]  + i] == CELL_STATE_MOVING)
				table[(shapeCenterY + (j - shapeWithRotation[SHAPE_CENTER_Y_INDEX]))*MAX_X + (shapeCenterX + (i - shapeWithRotation[SHAPE_CENTER_X_INDEX]))] = CELL_STATE_EMPTY;
		}

   
	for (unsigned char j = 0; j < newShapeWithRotation[ROW_COUNT_INDEX]; j++)
		for (unsigned char i = 0; i < newShapeWithRotation[COLUMN_COUNT_INDEX]; i++)
		{
			if (newShapeWithRotation[MATRIX_START_INDEX + j*newShapeWithRotation[COLUMN_COUNT_INDEX] + i] == CELL_STATE_MOVING)
				table[(shapeCenterY + (j - newShapeWithRotation[SHAPE_CENTER_Y_INDEX]))*MAX_X + (shapeCenterX + (i - newShapeWithRotation[SHAPE_CENTER_X_INDEX]))] = CELL_STATE_MOVING;
		}

  renderPlayField(table,MAX_Y,MAX_X);
}

void moveItemLeft()
{
	if (isFixedItemCollision(-1,0, shapeCenterX, shapeCenterY, currentShape, currentRotation))
		return;

	moveShape(-1, 0);
}


void moveItemRight()
{
	if (isFixedItemCollision(1, 0, shapeCenterX, shapeCenterY, currentShape, currentRotation))
		return;

	moveShape(1, 0);
}


void moveItemDown()
{

  if(isGameOver)
    return;

	if (isFixedItemCollision(0, 1, shapeCenterX, shapeCenterY, currentShape, currentRotation))
	{
        changeStateFromMovingToFixed(currentShape,currentRotation);      
     
        clearNewItemField(); 

        unsigned char *nextShape = toShapeWithRotation(newShape,newRotation);
        copyItemToTable(4, nextShape, table, MAX_X );
        currentShape = newShape;
        currentRotation = newRotation;
        shapeCenterX = 4+nextShape[SHAPE_CENTER_X_INDEX];
        shapeCenterY = 0+nextShape[SHAPE_CENTER_Y_INDEX];
        if(isFixedItemCollision(0,1, shapeCenterX, shapeCenterY, currentShape, currentRotation)){
            isGameOver = true;
            showGameOver();
            return;
        }
           
        unsigned char *newShapeWithRotation = generateNewItem();
  
        copyItemToTable(0, newShapeWithRotation, newItemTable, 4 );
        renderPlayField(table,MAX_Y,MAX_X);
        renderNextItemField(newItemTable,4);
        refreshStats(lines,score,level);
        return;
                  
  }else   
    moveShape(0,1);   
    

}


void changeStateFromMovingToFixed(unsigned char shape, unsigned char rotation)
{//renderTableIntoSerial("changeStateFromMovingToFixed before change",table,MAX_X);
	unsigned char *shapeWithRotation = toShapeWithRotation(shape, rotation);
	for (unsigned char j = 0; j < shapeWithRotation[ROW_COUNT_INDEX]; j++)
		for (unsigned char i = 0; i < shapeWithRotation[COLUMN_COUNT_INDEX]; i++)
		{
			if (shapeWithRotation[MATRIX_START_INDEX + j*shapeWithRotation[COLUMN_COUNT_INDEX] + i] == CELL_STATE_MOVING)
				table[(shapeCenterY + (j - shapeWithRotation[SHAPE_CENTER_Y_INDEX]))*MAX_X + (shapeCenterX + (i - shapeWithRotation[SHAPE_CENTER_X_INDEX]))] = CELL_STATE_FIXED;
		}
   findCompleteLinesRemoveAddCountScore() ;
 //  renderTableIntoSerial("changeStateFromMovingToFixed after change",table,MAX_X);
}


void increaseScoreForLinesAndSpeed(unsigned char linesCompleted)
{
	if (linesCompleted <= 0)
		return;
	int scoreIncrease = 0;
	lines = lines + linesCompleted;

	switch (linesCompleted)
	{
	case 1:
		scoreIncrease = 100;
		break;
	case 2:
		scoreIncrease = 300;
		break;
	case 3:
		scoreIncrease = 700;
		break;
	case 4:
		scoreIncrease = 1500;
		break;
	default:
		break;
	}

	score = score + scoreIncrease;
	level = (unsigned char)(score / 10000)+1;
	if (level < 1)
		level = 1; 
}
int getLevel()
{
   return level;  
}
void findCompleteLinesRemoveAddCountScore() 
{ 
  unsigned char targetLineIndex = MAX_Y-1;
	unsigned char tableWithRemovedCompletedLines[200] = {};
	int linesCompleted = 0;
Serial.println("After init");
	for (short j = (MAX_Y - 1); j >= 0; j--)
	{
  Serial.println("j " + String(j));
		bool isLineCompleted = true;
		for (short i = 0; i < MAX_X; i++)
		{
			isLineCompleted = isLineCompleted && (table[j*MAX_X + i] == CELL_STATE_FIXED);
			if (!isLineCompleted)
				break;
		}
			if (isLineCompleted)
				linesCompleted++;
			else
			{				
				for (short k = 0; k < MAX_X; k++)
					tableWithRemovedCompletedLines[targetLineIndex*MAX_X+k] = table[j*MAX_X + k];
				targetLineIndex--;
			}

	}
if(linesCompleted > 0)
   for(short q = 0; q < MAX_Y;q++)
     for(short r =0; r< MAX_X;r++)
     {
      table[q*MAX_X+r] = tableWithRemovedCompletedLines[q*MAX_X+r];
     }
     
//	renderTableIntoSerial("Removed lines table",tableWithRemovedCompletedLines,MAX_X);
// renderTableIntoSerial("table",table,MAX_X);
 //Serial.println("Lines completed " + String(linesCompleted));
	increaseScoreForLinesAndSpeed(linesCompleted);	

}



void resetGame()
{
	memset(table, 0, sizeof(table[0]) * MAX_Y  * MAX_X);
   
	lines = 0;
	score = 0;
	level = 1;

	clearNewItemField();
	unsigned char *newShapeWithRotation = generateNewItem();

	copyItemToTable(4,  newShapeWithRotation, table, MAX_X);

	currentShape = newShape;
	currentRotation = newRotation;
	shapeCenterX = 4 + newShapeWithRotation[SHAPE_CENTER_X_INDEX];
	shapeCenterY = 0 + newShapeWithRotation[SHAPE_CENTER_Y_INDEX];

	newShapeWithRotation = generateNewItem();

  copyItemToTable(0,  newShapeWithRotation, newItemTable, 4);
 
  isGameOver = false;

  initFrameBuffer();
  renderPlayField(table,MAX_Y,MAX_X);
  renderNextItemField(newItemTable,4);
  refreshStats(lines, score, level);
}

void initDisplay()
{
  initFrameBuffer();
}

void renderDisplay()
{
  renderFrameBuffer();
}

void leftButtonClick()
{
  moveItemLeft();
}

void rightButtonClick()
{
  moveItemRight();
}

void downButtonClick()
{
  moveItemDown();
}

void upButtonClick()
{
  //rotateShape();
}

void action1ButtonClick()
{
  rotateShape();
}

void resetButtonClick()
{
  resetGame();  
}

void selectButtonClick()
{}

void action2ButtonClick()
{
  //rotateShape();
}

void leftUpButtonClick()
{
  moveItemLeft();  
}

void leftDownButtonClick()
{
  moveItemLeft();
  moveItemDown();
}
void rightUpButtonClick()
{
  moveItemRight();  
}
void rightDownButtonClick()
{
  moveItemRight();
  moveItemDown();
}

void leftAction1ButtonClick()
{
  moveItemLeft();
  rotateShape();  
}
void leftAction2ButtonClick()
{
  moveItemLeft();
  rotateShape();
}

void rightAction1ButtonClick()
{
  moveItemRight();
  rotateShape();
}

void rightAction2ButtonClick()
{
  moveItemRight();
  rotateShape();
}

void upAction1ButtonClick()
{
  rotateShape();
}

void upAction2ButtonClick()
{
  rotateShape();
}

void downAction1ButtonClick()
{
  moveItemDown();
  rotateShape();
}

void downAction2ButtonClick()
{
  moveItemDown();
  rotateShape();
}















  
