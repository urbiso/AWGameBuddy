#include "Bricks.h"


//#define TFT20IN
//#define TFT24IN
//#define TFT35IN
#define EINK29IN

#ifdef EINK29IN
  #include "eInk29inRender.h"
#elif TFT20IN
  #include "TFT20inRenderer.h"
#elif TFT24IN
  #include "TFT24inRenderer.h"
#elif TFT35IN
  #include "TFT35inRenderer.h"
#endif


unsigned long time_start_ms;
unsigned long time_now_s;

void setup() {  
  delay(2000);
  Serial.begin(9600);
  initDisplay();
  resetGame();
  time_start_ms = millis();

  randomSeed(time_start_ms);
}


void loop() {

  moveItemDown();
  renderFrameBuffer();
  
}
