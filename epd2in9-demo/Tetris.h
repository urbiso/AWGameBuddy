#ifndef BRICKS
#define BRICKS

const unsigned char SHAPE_CENTER_X_INDEX = 0;
const unsigned char SHAPE_CENTER_Y_INDEX = 1;
const unsigned char ROW_COUNT_INDEX = 2;
const unsigned char COLUMN_COUNT_INDEX = 3;
const unsigned char MATRIX_START_INDEX = 4;

const unsigned char SHAPE_LL = 0;
const unsigned char SHAPE_LR = 1;
const unsigned char SHAPE_ZL = 2;
const unsigned char SHAPE_ZR = 3;
const unsigned char SHAPE_I = 4;
const unsigned char SHAPE_O = 5;
const unsigned char SHAPE_T = 6;

const unsigned char CELL_STATE_EMPTY = 0;
const unsigned char CELL_STATE_FIXED = 1;
const unsigned char CELL_STATE_MOVING = 2;

const unsigned char FIRST_ROTATION = 0;
const unsigned char LAST_ROTATION = 3;

const unsigned char MAX_X = 10;
const unsigned char MAX_Y = 20;


void copyItemToTable(unsigned int itemInserPositionX, unsigned char *itemToInsert, unsigned char *targetTable, unsigned char columnNum);
void clearNewItemField();
unsigned char *toShapeWithRotation(unsigned char shape, unsigned char rotation);
unsigned char random(unsigned char min, unsigned char max);
unsigned char *generateNewItem();
bool isFixedItemCollision(int offsetX, int offsetY, unsigned char shapeCenterX, unsigned char shapeCenterY, unsigned char shape, unsigned char rotation);
void moveShape(short offsetX, short offsetY, unsigned char *shapeCenterX, unsigned char *shapeCenterY, unsigned char currentShape, unsigned char currentRotation);
void rotateShape(unsigned char shapeCenterX, unsigned char shapeCenterY, unsigned char *currentRotation, unsigned char *currentShape);
void moveItemLeft(unsigned char *shapeCenterX, unsigned char *shapeCenterY, unsigned char currentShape, unsigned char currentRotation);
void moveItemRight(unsigned char *shapeCenterX, unsigned char *shapeCenterY, unsigned char currentShape, unsigned char currentRotation);
void moveItemDown(unsigned char *shapeCenterX, unsigned char *shapeCenterY, unsigned char *currentShape, unsigned char *currentRotation);
void changeStateFromMovingToFixed(unsigned char shape, unsigned char rotation);
void increaseScoreForLinesAndSpeed(unsigned char linesCompleted);
void findCompleteLinesRemoveAddCountScore();
void resetGame();


#endif
