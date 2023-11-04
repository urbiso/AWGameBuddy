#include <iostream>
#include <cstring>
#include <stdlib.h>

#include "Tetris.h"


unsigned char table[MAX_Y][MAX_X] = {
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

unsigned char newItemTable[4][4] = {
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0
};

unsigned char tempItemTable[4][4] = {
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0
};

/*
   Prestored shapes with its rotation.
   Rotation stored as records in array, first index
   centerX,centerY,rows,columns,xxx,xxx,xx....
   Example: 0,1,1,4,2,2,2,2 - center point of the shape with in the matrix of shape
   at row 0, column 1, shape store in matrix with 1 row and 4 coulmns,
   shape definitions follows.
*/

unsigned char itemI[4][8] = {
	0,1,1,4,2,2,2,2,
	1,0,4,1,2,2,2,2,
	0,1,1,4,2,2,2,2,
	1,0,4,1,2,2,2,2
};

unsigned char itemLR[4][10] = {
	1,0,3,2,2,0,2,0,2,2,
	1,1,2,3,0,0,2,2,2,2,
	1,1,3,2,2,2,0,2,0,2,
	0,1,2,3,2,2,2,2,0,0
};

unsigned char itemLL[4][10] = {
	1,1,3,2,0,2,0,2,2,2,
	0,1,2,3,2,2,2,0,0,2,
	1,0,3,2,2,2,2,0,2,0,
	1,1,2,3,2,0,0,2,2,2
};

unsigned char itemZR[4][10] = {
	0,1,2,3,2,2,0,0,2,2,
	1,0,3,2,0,2,2,2,2,0,
	1,1,2,3,2,2,0,0,2,2,
	1,0,3,2,0,2,2,2,2,0
};

unsigned char itemZL[4][10] = {
	0,1,2,3,0,2,2,2,2,0,
	0,1,3,2,2,0,2,2,0,2,
	0,1,2,3,0,2,2,2,2,0,
	1,0,3,2,2,0,2,2,0,2
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
	0,1,2,3,2,2,2,0,2,0,
	1,0,3,2,2,0,2,2,2,0
};

unsigned char item = 0;


unsigned char level = 1;
unsigned long score = 0;
unsigned int lines = 0;
unsigned char speed = 0;


unsigned char currentShape = 0;
unsigned char currentRotation = 0;
unsigned char newShape = 0;
unsigned char newRotation = 0;
unsigned char shapeCenterX = 0;
unsigned char shapeCenterY = 0;
bool isGameOver = false;



void copyItemToTable(unsigned int itemInserPositionX, unsigned char *itemToInsert, unsigned char *targetTable, unsigned char columnNum) 
{
    for(unsigned char j = 0; j < itemToInsert[ROW_COUNT_INDEX]; j++ )
       for(unsigned char i = 0; i < itemToInsert[COLUMN_COUNT_INDEX]; i++)
       {
           if(itemToInsert[i*j+MATRIX_START_INDEX] == CELL_STATE_MOVING )
           {
                  targetTable[j*columnNum + itemInserPositionX + i] = CELL_STATE_MOVING;
           }
       }
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


unsigned char random(unsigned char min, unsigned char max) 
{
	return  rand() % max;
}

unsigned char *generateNewItem()
{
    newShape = random(0,7);
    newRotation = random(0,4);

    return toShapeWithRotation(newShape,newRotation);
}

void renderCell(unsigned char j, unsigned char i)
{
    return;
}

void renderTable()
{
    for(unsigned char j = 0; j < MAX_Y; j++)
       for(unsigned char i = 0; i < MAX_X; i++)
       {
           renderCell(j,i);
       }
}

bool isFixedItemCollision(int offsetX, int offsetY, unsigned char shapeCenterX, unsigned char shapeCenterY, unsigned char shape, unsigned char rotation)
{
  unsigned char newCenterItemX = shapeCenterX + offsetX;
  unsigned char newCenterItemY = shapeCenterY + offsetY;
  short newX = 0;
  short newY = 0;

  unsigned char *shapeWithRotation = toShapeWithRotation(shape,rotation);

  for(unsigned char j = 0; j < shapeWithRotation[ROW_COUNT_INDEX]; j++)
    for(unsigned char i = 0; i < shapeWithRotation[COLUMN_COUNT_INDEX]; i++)
    {
        if(shapeWithRotation[MATRIX_START_INDEX+j*MAX_X+i] == CELL_STATE_MOVING)
        {
            newX = newCenterItemX + (i - shapeWithRotation[SHAPE_CENTER_Y_INDEX]);
            newY = newCenterItemY + (j - shapeWithRotation[SHAPE_CENTER_X_INDEX]);
            if(newX < 0 || newX >= MAX_X || newY >= MAX_Y || table[newY][newX] == CELL_STATE_FIXED )
               return true;
        }
    }
  return false;
}


void moveShape(short offsetX, short offsetY, unsigned char *shapeCenterX, unsigned char *shapeCenterY, unsigned char currentShape, unsigned char currentRotation)
{
   unsigned char newCenterItemX = *shapeCenterX + offsetX;
   unsigned char newCenterItemY = *shapeCenterY + offsetY;

   if(isFixedItemCollision(offsetX,offsetY,*shapeCenterX,*shapeCenterY,currentShape,currentRotation))
    return;
   
   unsigned char *shapeWithRotation = toShapeWithRotation(currentShape,currentRotation);

   for(unsigned char j = 0; j < shapeWithRotation[COLUMN_COUNT_INDEX]; j++)
        for(unsigned char i = 0; i <shapeWithRotation[ROW_COUNT_INDEX]; i++)
        {
            if(shapeWithRotation[MATRIX_START_INDEX+j*MAX_X + i] == CELL_STATE_MOVING)
                table[*shapeCenterY+(j-shapeWithRotation[SHAPE_CENTER_Y_INDEX])][*shapeCenterX + (i-shapeWithRotation[SHAPE_CENTER_X_INDEX])] = CELL_STATE_EMPTY;
        }
    for(unsigned char j = 0; j < shapeWithRotation[COLUMN_COUNT_INDEX]; j++)
        for(unsigned char i = 0; i <shapeWithRotation[ROW_COUNT_INDEX]; i++)
        {
            if(shapeWithRotation[MATRIX_START_INDEX+j*MAX_X + i] == CELL_STATE_MOVING)
                table[newCenterItemY+(j-shapeWithRotation[SHAPE_CENTER_Y_INDEX])][newCenterItemX + (i-shapeWithRotation[SHAPE_CENTER_X_INDEX])] = CELL_STATE_MOVING;
        }
    
    *shapeCenterX = newCenterItemX;
    *shapeCenterY = newCenterItemY;
}

void rotateShape(unsigned char shapeCenterX, unsigned char shapeCenterY, unsigned char *currentRotation, unsigned char *currentShape)
{
    unsigned char newRotation = *currentRotation + 1;
    if(newRotation > LAST_ROTATION)
        newRotation = FIRST_ROTATION;
    
    if(isFixedItemCollision(0,0,shapeCenterX,shapeCenterY,*currentShape,newRotation))
        return;

	unsigned char *shapeWithRotation = toShapeWithRotation(*currentShape, *currentRotation);
	*currentRotation = newRotation;
	unsigned char *newShapeWithRotation = toShapeWithRotation(*currentShape, *currentRotation);

	for (unsigned char j = 0; j < shapeWithRotation[COLUMN_COUNT_INDEX]; j++)
		for (unsigned char i = 0; i < shapeWithRotation[ROW_COUNT_INDEX]; i++)
		{
			if (shapeWithRotation[MATRIX_START_INDEX + j*MAX_X  + i] == CELL_STATE_MOVING)
				table[shapeCenterY + (j - shapeWithRotation[SHAPE_CENTER_Y_INDEX])][shapeCenterX + (i - shapeWithRotation[SHAPE_CENTER_X_INDEX])] = CELL_STATE_EMPTY;
		}
	for (unsigned char j = 0; j < shapeWithRotation[COLUMN_COUNT_INDEX]; j++)
		for (unsigned char i = 0; i < shapeWithRotation[ROW_COUNT_INDEX]; i++)
		{
			if (shapeWithRotation[MATRIX_START_INDEX + j*MAX_X + i] == CELL_STATE_MOVING)
				table[shapeCenterY + (j - shapeWithRotation[SHAPE_CENTER_Y_INDEX])][shapeCenterX + (i - shapeWithRotation[SHAPE_CENTER_X_INDEX])] = CELL_STATE_MOVING;
		}
}

void moveItemLeft(unsigned char *shapeCenterX, unsigned char *shapeCenterY, unsigned char currentShape, unsigned char currentRotation)
{
	if (isFixedItemCollision(-1,0, *shapeCenterX, *shapeCenterY, currentShape, currentRotation))
		return;

	moveShape(-1, 0, shapeCenterX, shapeCenterY, currentShape, currentRotation);
}


void moveItemRight(unsigned char *shapeCenterX, unsigned char *shapeCenterY, unsigned char currentShape, unsigned char currentRotation)
{
	if (isFixedItemCollision(1, 0, *shapeCenterX, *shapeCenterY, currentShape, currentRotation))
		return;

	moveShape(1, 0, shapeCenterX, shapeCenterY, currentShape, currentRotation);
}


void moveItemDown(unsigned char *shapeCenterX, unsigned char *shapeCenterY, unsigned char *currentShape, unsigned char *currentRotation)
{
	if (isFixedItemCollision(0, 1, *shapeCenterX, *shapeCenterY, *currentShape, *currentRotation))
	{

	}
}


void changeStateFromMovingToFixed(unsigned char shape, unsigned char rotation)
{
	unsigned char *shapeWithRotation = toShapeWithRotation(shape, rotation);
	for (unsigned char j = 0; j < shapeWithRotation[COLUMN_COUNT_INDEX]; j++)
		for (unsigned char i = 0; i < shapeWithRotation[ROW_COUNT_INDEX]; i++)
		{
			if (shapeWithRotation[MATRIX_START_INDEX + j*MAX_X  + i] == CELL_STATE_MOVING)
				table[shapeCenterY + (j - shapeWithRotation[SHAPE_CENTER_Y_INDEX])][shapeCenterX + (i - shapeWithRotation[SHAPE_CENTER_X_INDEX])] = CELL_STATE_FIXED;
		}
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
	speed = (unsigned char)(score / 10000);
	if (speed < 1)
		speed = 1;
}
void findCompleteLinesRemoveAddCountScore() 
{
	unsigned char targetLineIndex = 0;
	unsigned char tableWithRemovedCompletedLines[20][10] = {};
	int linesCompleted = 0;

	for (unsigned char j = MAX_Y - 1; j >= 0; j--)
	{
		bool isLineCompleted = true;
		for (unsigned char i = 0; i < MAX_X; i++)
		{
			isLineCompleted = isLineCompleted && (table[j][i] == CELL_STATE_FIXED);
			if (!isLineCompleted)
				break;
			if (isLineCompleted)
				linesCompleted++;
			else
			{				
				for (unsigned char k = 0; k < MAX_X; k++)
					tableWithRemovedCompletedLines[targetLineIndex][i] = table[j][i];
				targetLineIndex++;
			}

		}
	}
	increaseScoreForLinesAndSpeed(linesCompleted);	

}



void resetGame()
{
	memset(table, 0, sizeof(table[0][0]) * MAX_Y  * MAX_X);

	lines = 0;
	score = 0;
	speed = 0;
	clearNewItemField();
	unsigned char *newShapeWithRotation = generateNewItem();
	copyItemToTable(4, *newItemTable, *table, MAX_X);
	currentShape = newShape;
	currentRotation = newRotation;
	shapeCenterX = 4 + newShapeWithRotation[SHAPE_CENTER_Y_INDEX];
	shapeCenterY = 0 + newShapeWithRotation[SHAPE_CENTER_X_INDEX];

	newShapeWithRotation = generateNewItem();

	for(unsigned int j = 0; j < MAX_Y; j++)
		for (unsigned char i = 0; i < MAX_X; i++)
		{
			tempItemTable[j][i] = newShapeWithRotation[MATRIX_START_INDEX + i * j];
	    } 

	copyItemToTable(0, *tempItemTable, *newItemTable, 4);

	isGameOver = false;


}

