/*
PIPE OFFSET RANGE
--150 - 220

*/
#include "myLib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bird.h"
#include "pipe.h"
#include "startscreen.h"


int main()
{
	//EXTERAL LOOP FOR (SELECT) BUTTON HANDLING
	while (1)
	{
		//STARTSCREEN
		int breakVar = 0;

		REG_DISPCNT = MODE_i3 | BG2_EN;
		u16 color = SKYBLUE;
		DMA[3].src = &color;
		DMA[3].cnt = 240 * 180 | DMA_ON | DMA_SOURCE_FIXED | DMA_SOURCE_FIXED;
		drawImage3(0, 0, 160, 240, startscreen, 0);
		while(KEY_DOWN_NOW(BUTTON_START)) {}
		while(!KEY_DOWN_NOW(BUTTON_A)) {}
		while(KEY_DOWN_NOW(BUTTON_A)) {}
		drawRect(0, 0, 160, 240, SKYBLUE);

		while(1)
		{
			//INITIALIZE VARS UPON RESTARTING
			int pressed = 0;
			int start = 0;

			int score = 0;
			char str[10];

			int startRow = 0;
			int accel = 1;
			int velocity = 0;

			Pipe p;
			p.pipeCol = 239;
			p.pipeOffset = randint();
				drawRect(0, 0, 160, 240, SKYBLUE);

			//START INITIAL FREEFALL
			while(!breakVar && !KEY_DOWN_NOW(BUTTON_A))
			{
				if (KEY_DOWN_NOW(BUTTON_START))
				{
					breakVar = 1;
				}



				waitForVblank();
				delay(2);

				int saveRow = startRow;

				velocity += accel;
				if (velocity > 20)
				{
					velocity = 20;
				}
				startRow += velocity;

				//BIRD WRAPAROUND FUNCTIONALITY
				if (startRow >= 160)
				{
					startRow = 0;
				}
				else if (startRow < (0 - BIRD_HEIGHT))
				{
					startRow = 160;
				}
				//DRAWBIRD
				drawRect(saveRow, STATIC_COL, BIRD_HEIGHT, BIRD_WIDTH, SKYBLUE);
				drawImage3(startRow, STATIC_COL, BIRD_HEIGHT, BIRD_WIDTH, bird, 0);

				if (!KEY_DOWN_NOW(BUTTON_A))
				{
					pressed = 0;
				}
			}

			if (breakVar == 1)
			{
				break;
			}

			//MAIN GAME LOOP
			while (!breakVar)
			{
				if (KEY_DOWN_NOW(BUTTON_START)) {
					breakVar = 1;
					break;
				}

				//PRINT SCORE
				drawRect(0, 0, 160, PIPE_WIDTH, SKYBLUE);
				sprintf(str, "%d", score);
				drawString(70, 10, str, BLACK);

				//COLLISION DETECTION
				if ((p.pipeCol - (STATIC_COL + BIRD_WIDTH)) + 2 <= 0 && ((p.pipeCol + PIPE_WIDTH) - STATIC_COL) >= 0)
				{
					if (300 - p.pipeOffset < startRow + BIRD_HEIGHT || 245 - p.pipeOffset > startRow)
					{
						break;
					}
				}

				//JUMP
				if (pressed == 0 && KEY_DOWN_NOW(BUTTON_A))
				{
					velocity = JUMP_VELOCITY;
					pressed = 1;
					start = 1;

				}

				waitForVblank();
				delay(1);

				int saveRow = startRow;

				velocity += accel;
				if (velocity > 20)
				{
					velocity = 20;
				}
				startRow += velocity;j

				//BIRD WRAPAROUND FUNCTIONALITY
				if (startRow >= 160)
				{
					startRow = 0;
				}
				else if (startRow < (0 - BIRD_HEIGHT))
				{
					startRow = 160;
				}
				if (start == 1)
				{
					if (p.pipeCol > 0)
					{
						drawImage3(0, p.pipeCol, PIPE_HEIGHT, PIPE_WIDTH, pipe, p.pipeOffset);
						p.pipeCol -= 5;
						drawRect(0, p.pipeCol + PIPE_WIDTH, 160, 5, SKYBLUE);
					}
					else
					{

						score++;

						//ASSIGN RANDOM PIPE HEIGHT
						p.pipeOffset = randint();
						p.pipeCol = 239;
					}

				}
				//DRAWBIRD
				drawRect(saveRow, STATIC_COL, BIRD_HEIGHT, BIRD_WIDTH, SKYBLUE);
				drawImage3(startRow, STATIC_COL, BIRD_HEIGHT, BIRD_WIDTH, bird, 0);

				if (!KEY_DOWN_NOW(BUTTON_A))
				{
					pressed = 0;
				}
			}

			//GAME OVER STATE
			if (!breakVar)
			{
				for (int i = 0; i < 25; ++i)
				{
					if (KEY_DOWN_NOW(BUTTON_START))
					{
						breakVar = 1;
						break;
					}
					delay(1);
				}
				if (breakVar)
				{
					break;
				}
				drawImage3(0, 0, 160, 240, overscreen, 0);
				char message[20] = " Your final score:";
				drawString(120, 50, message, BLACK);
				drawString(120, 160, str, MAGENTA);

				while(KEY_DOWN_NOW(BUTTON_A))
				{
					if (KEY_DOWN_NOW(BUTTON_START))
					{
						breakVar = 1;
						break;
					}
				}
				int tempPressed = 0;
				while(!breakVar)
				{
					if (KEY_DOWN_NOW(BUTTON_START))
					{
						breakVar = 1;
						break;
					}
					if (!tempPressed && KEY_DOWN_NOW(BUTTON_A))
					{
						tempPressed = 1;
					}
					if (tempPressed && !KEY_DOWN_NOW(BUTTON_A))
					{
						break;
					}

				}
			}
		}
	}
	return 0;
}