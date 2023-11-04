#ifndef TETRIS_RENDER
#define TETRIS_RENDER
	void renderCell(unsigned char j, unsigned char i);
	void renderTable();

 void initDisplay();
 void renderPlayField(unsigned char *table, unsigned char maxY, unsigned char maxX);
 void renderNextItemField(unsigned char *newItemTable, unsigned char maxY, unsigned char maxX);
 void refreshStats(unsigned int lines,unsigned long score, unsigned int speed);
 void renderFrameBuffer();
 void testLoop();
 void showGameOver();
#endif
