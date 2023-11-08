#ifndef BRICKS
#define BRICKS

#define COLORED     0
#define UNCOLORED   1

#define SHAPE_CENTER_X_INDEX  0
#define SHAPE_CENTER_Y_INDEX  1
#define ROW_COUNT_INDEX       2
#define COLUMN_COUNT_INDEX    3
#define MATRIX_START_INDEX    4

#define SHAPE_LL 0
#define SHAPE_LR 1
#define SHAPE_ZL 2
#define SHAPE_ZR 3
#define SHAPE_I  4
#define SHAPE_O  5
#define SHAPE_T  6

#define CELL_STATE_EMPTY   0
#define CELL_STATE_FIXED   1
#define CELL_STATE_MOVING  2

#define FIRST_ROTATION  0
#define LAST_ROTATION   3

#define MAX_X  10
#define MAX_Y  20

void copyItemToTable(unsigned int itemInserPositionX, unsigned char *itemToInsert, unsigned char *targetTable, unsigned char columnNum); 
void clearNewItemField();
unsigned char *toShapeWithRotation(unsigned char shape, unsigned char rotation);
unsigned char *generateNewItem();

bool isFixedItemCollision(int offsetX, int offsetY, unsigned char shapeCenterX, unsigned char shapeCenterY, unsigned char shape, unsigned char rotation);
void moveShape(short offsetX, short offsetY);
void rotateShape();
void moveItemLeft();
void moveItemRight();
void moveItemDown();
void changeStateFromMovingToFixed(unsigned char shape, unsigned char rotation);
void increaseScoreForLinesAndSpeed(unsigned char linesCompleted);
void findCompleteLinesRemoveAddCountScore();
void resetGame();
void setRandomSeed(long randomSeed);
void initDisplay();
void renderDisplay();
void renderTableIntoSerial(String message,unsigned char *itemToInsert,int columns);
int getLevel();




#endif
