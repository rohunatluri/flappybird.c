#include "myLib.h"
#include <stdlib.h>

volatile unsigned short *videoBuffer = (unsigned short*) 0x6000000;

// set the value of a single pixel
void setPixel(int r, int c, unsigned short color)
{
	videoBuffer[OFFSET(r, c, 240)] = color;
}

// draw a single character at (col, row)
void drawChar(int row, int col, char ch, u16 color)
{
	for(int r=0; r<8; r++)
	{
		for(int c=0; c<6; c++)
		{
			if(fontdata_6x8[OFFSET(r, c, 6) + ch*48])
			{
				setPixel(row+r, col+c, color);
			}
		}
	}
}

// draw a string at (col, row)
void drawString(int row, int col, char *str, u16 color)
{
	while(*str)
	{
		drawChar(row, col, *str++, color);
		col += 6;
	}
}
// draw a rectangle at (col, row)
void drawRect(int row, int col, int height, int width, unsigned short color)
{
	int h = row;
	int w = 0;
	for (int i = 0; i < height; ++i)
	{
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[240 * h + col];
		DMA[3].cnt = (width) | DMA_ON | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT;
		h += 1;
		w += width;
	}
}

// wait timer
void delay(int n)
{
	volatile int x = 0;
	for(int i=0; i<n*5000; i++)
	{
		x++;
	}
}

// draw orange portals
void drawPortals()
{
	drawRect(10, STATIC_COL - 1, 4, 20, GREEN);
	drawRect(150, STATIC_COL - 1, 4, 20, GREEN);
}

// wait for drawing completion
void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

// draw an image at (col, row) with row offset
void drawImage3(int row, int col, int height, int width, const unsigned short *img, int rowOffset)
{
	int h = row;
	int w = 0;
	for (int i = 0; i < height && h <= 160; ++i)
	{
		DMA[3].src = &img[(width * rowOffset) + w];
		DMA[3].dst = &videoBuffer[240 * h + col];
		if (width + col >= 240)
		{
			DMA[3].cnt = (240 - col) | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
		}

		else
		{
			DMA[3].cnt = (width) | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
		}

		// DMA[3].cnt = (width) | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;

		h += 1;
		w += width;
	}
}

// generate a random number between 150 and 220 (for pipe offset)
int randint() {
	int n = 70;
  if ((n - 1) == RAND_MAX) {
    return rand();
  } else {
    	long end = RAND_MAX / n;
    	end *= n;

    	int r;
    	while ((r = rand()) >= end);

    	return (r % n) + 150;
 	}
}