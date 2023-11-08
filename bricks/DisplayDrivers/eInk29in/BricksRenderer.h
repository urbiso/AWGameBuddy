#if !defined(BRICKS_RENDER)
#define BRICKS_RENDER

 void initFrameBuffer();
 void renderPlayField(unsigned char *table, unsigned char maxY, unsigned char maxX);
 void renderNextItemField(unsigned char *newItemTable, unsigned char maxY, unsigned char maxX);
 void refreshStats(unsigned int lines,unsigned long score, unsigned int speed);
 void renderFrameBuffer();
 void showGameOver();

#endif
