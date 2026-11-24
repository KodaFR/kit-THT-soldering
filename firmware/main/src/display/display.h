void initscreen();
void drawBitmapPage(const unsigned char bitmap[1024]);
void drawBitmapPageWithLabel(const unsigned char bitmap[1024], const char* label, int acuteAccentIndex = -1, const char* subtitle = nullptr);
void drawPopupPage(const char* title, const char* message);
void drawRadioScanFrame(unsigned long frame);
void drawEatActivityFrame(const unsigned char petBitmap[1024], unsigned int bites);
void drawTextPage(const char* title, const char* const lines[], unsigned int lineCount);