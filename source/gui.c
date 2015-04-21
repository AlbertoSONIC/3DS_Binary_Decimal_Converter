#include <3ds.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gui.h"
#include "draw.h"
#include "rendering.h"

//For Background
#include "bottom_bin.h"
#include "top_bin.h"

//For time
#define SECONDS_IN_DAY 86400
#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_MINUTE 60
int time = 0;

char buffer[100];

void guiTop(unsigned long long decimal, unsigned  long long binary, char hex[] )
{
	//Background
	gfxDrawSprite(GFX_TOP, GFX_LEFT, (u8*)top_bin, 240, 400, 0, 0);

    //Prints the statusBar
	drawFillRect(0, 0, 399, 17, 255, 128, 0, screenTopLeft);
	gfxDrawText(GFX_TOP, GFX_LEFT, NULL, "3DS Binary Decimal Converter", 5, 238 - fontDefault.height * 1);
	guiClock();

	//Prints the converted values
	sprintf(buffer, "%llu", decimal);
	gfxDrawText(GFX_TOP, GFX_LEFT, NULL, buffer, 30, 135);
	sprintf(buffer, "%s", hex);
	gfxDrawText(GFX_TOP, GFX_LEFT, NULL, buffer,270, 135);
	sprintf(buffer, "%llu", binary);
	gfxDrawText(GFX_TOP, GFX_LEFT, NULL, buffer, 130, 23);
}

void guiBottom()
{
	//Draw BackGround
	gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)bottom_bin, 240, 320, 0, 0);
	//gfxDrawText(GFX_BOTTOM, GFX_LEFT, NULL, "Developed by AlbertoSONIC", 85, 240 - fontDefault.height * 15);
}

void guiClock()
{
	u64 timeInSeconds = osGetTime() / 1000;
	u64 dayTime = timeInSeconds % SECONDS_IN_DAY;
	sprintf(buffer, "%llu:%llu:%llu", dayTime / SECONDS_IN_HOUR, (dayTime % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE, dayTime % SECONDS_IN_MINUTE);

	gfxDrawText(GFX_TOP, GFX_LEFT, NULL, buffer, 350, 238 - fontDefault.height * 1);
}

void guiPopup(char* title, char* line1, char* line2, char* line3, char* button1, char* button2, bool closeonly)
{
	//Prints a dark grey rectangle!
	drawFillRect(36, 60, 272, 85, 128, 128, 128, screenBottom);
	//Prints a light grey rectangle!
	drawFillRect(36, 85, 272, 189, 160, 160, 160, screenBottom);
	//Prints text
	gfxDrawText(GFX_BOTTOM, GFX_LEFT, NULL, title, 124, 240 - fontDefault.height * 5);
	gfxDrawText(GFX_BOTTOM, GFX_LEFT, NULL, line1, 50, 245 - fontDefault.height * 7);
	gfxDrawText(GFX_BOTTOM, GFX_LEFT, NULL, line2, 50, 245 - fontDefault.height * 8);
	gfxDrawText(GFX_BOTTOM, GFX_LEFT, NULL, line3, 50, 245 - fontDefault.height * 9);
    //Prints the buttons!
	if (closeonly)
	{
        drawFillRect(107, 155, 198, 183, 192, 192, 192, screenBottom);
		gfxDrawText(GFX_BOTTOM, GFX_LEFT, NULL, button1, 130, 240 - fontDefault.height * 11);
	}
	else
	{
		drawFillRect(50, 151, 141, 179, 192, 192, 192, screenBottom);
	    drawFillRect(166, 151, 257, 179, 192, 192, 192, screenBottom);
		gfxDrawText(GFX_BOTTOM, GFX_LEFT, NULL, button1, 60, 240 - fontDefault.height * 11);
		gfxDrawText(GFX_BOTTOM, GFX_LEFT, NULL, button2, 180, 240 - fontDefault.height * 11);
	}	
}