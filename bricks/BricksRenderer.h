#if !defined(BRICKS_RENDER)
#define BRICKS_RENDER
 void initFrameBuffer();
 void renderPlayField(unsigned char *table, unsigned char maxY, unsigned char maxX);
 void renderNextItemField(unsigned char *newItemTable, unsigned char maxY);
 void refreshStats(unsigned int linesNew,unsigned long scoreNew, unsigned int levelNew);
 void renderFrameBuffer();
 void showGameOver();

#endif
