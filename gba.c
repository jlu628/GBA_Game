#include "gba.h"

volatile unsigned short *videoBuffer = (volatile unsigned short *)0x6000000;
u32 vBlankCounter = 0;

void waitForVBlank(void) {
  // TODO: IMPLEMENT

  // (1)
  // Write a while loop that loops until we're NOT in vBlank anymore:
  // (This prevents counting one VBlank more than once if your app is too fast)
  while (SCANLINECOUNTER > 160);

  // (2)
  // Write a while loop that keeps going until we're in vBlank:
  while (SCANLINECOUNTER < 160);

  // (3)
  // Finally, increment the vBlank counter:
  //vBlankCounter++;
}

static int __qran_seed = 42;
static int qran(void) {
  __qran_seed = 1664525 * __qran_seed + 1013904223;
  return (__qran_seed >> 16) & 0x7FFF;
}

int randint(int min, int max) { return (qran() * (max - min) >> 15) + min; }

void setPixel(int row, int col, u16 color) {
  // TODO: IMPLEMENT
  videoBuffer[OFFSET(row, col, WIDTH)] = color;
}

void drawRectDMA(int row, int col, int width, int height, volatile u16 color) {
  for (int h = 0; h < height; h ++) {
    // TODO: IMPLEMENT
    DMA[DMA_CHANNEL_3].src = &color;
    DMA[DMA_CHANNEL_3].dst = &videoBuffer[OFFSET(row, col, WIDTH)];
    DMA[DMA_CHANNEL_3].cnt = DMA_ON | DMA_SOURCE_FIXED | width;
  }
}

void drawFullScreenImageDMA(const u16 *image) {
  // TODO: IMPLEMENT
  DMA[DMA_CHANNEL_3].src = image;
  DMA[DMA_CHANNEL_3].dst = videoBuffer;
  DMA[DMA_CHANNEL_3].cnt = DMA_ON | DMA_DESTINATION_INCREMENT | WIDTH * HEIGHT;
}

void drawImageDMA(int row, int col, int width, int height, const u16 *image) {
  // TODO: IMPLEMENT
  for (int h = 0; h < height; h ++) {
    DMA[DMA_CHANNEL_3].src = &image[h * width];
    DMA[DMA_CHANNEL_3].dst = &videoBuffer[OFFSET(row + h, col, WIDTH)];
    DMA[DMA_CHANNEL_3].cnt = DMA_ON | DMA_DESTINATION_INCREMENT | width;
  }
}

void fillScreenDMA(volatile u16 color) {
  // TODO: IMPLEMENT
  DMA[DMA_CHANNEL_3].src = &color;
  DMA[DMA_CHANNEL_3].dst = videoBuffer;
  DMA[DMA_CHANNEL_3].cnt = DMA_ON | DMA_SOURCE_FIXED | WIDTH * HEIGHT;
}

void drawChar(int row, int col, char ch, u16 color) {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 8; j++) {
      if (fontdata_6x8[OFFSET(j, i, 6) + ch * 48]) {
        setPixel(row + j, col + i, color);
      }
    }
  }
}

void drawString(int row, int col, char *str, u16 color) {
  while (*str) {
    drawChar(row, col, *str++, color);
    col += 6;
  }
}

void drawCenteredString(int row, int col, int width, int height, char *str,
                        u16 color) {
  u32 len = 0;
  char *strCpy = str;
  while (*strCpy) {
    len++;
    strCpy++;
  }

  u32 strWidth = 6 * len;
  u32 strHeight = 8;

  int new_row = row + ((height - strHeight) >> 1);
  int new_col = col + ((width - strWidth) >> 1);
  drawString(new_row, new_col, str, color);
}

void drawFromTop(const u16 *image) {
  for (int r = 0; r < HEIGHT; r++) {
    DMA[DMA_CHANNEL_3].src = image;
    DMA[DMA_CHANNEL_3].dst = videoBuffer;
    DMA[DMA_CHANNEL_3].cnt = DMA_ON | DMA_DESTINATION_INCREMENT | WIDTH * (r + 1);
    for (int count = 0; count < 20; count++) {
      waitForVBlank();
    }
  }
}