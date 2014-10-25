#include "program.h"
#include "draw.h"

//For ctrulib
#include <3ds\types.h>
#include <3ds\services\hid.h>
#include <3ds\gfx.h>

//For sprintF
#include <stdio.h>

int converted = 0;
int bit = 0;
int state = 0;
int val[8]; //Change this "8" before compiling in order to change binary number lenght
int valtoprint;
int position = 1;
char* A = "1";
char* B = "0";
u32 input;
u8* screenBottom = 0;

void variablereset()
{
	position = 1;
	state = 0;
	bit = 0;
		converted = 0;
		int ii;
		for (ii = 0; ii<8; ii++)
		{
			val[ii] = 0;
		}
		input = 0;
}


void program()
{
	hidScanInput();
	input = hidKeysDown();

	//If first boot or time to reset
	if (state == 0 || (state == 2 && input & KEY_UP))
	{

		screenBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);
		clearScreen(screenBottom, GFX_BOTTOM);
		variablereset();
		maintitle();
		screenrender();
		maintitle();
		screenrender();
		state = 1;
	}

	//If normal state, take input
	if (bit < 8)
	{
		if (input & KEY_A)
		{
			valtoprint = 1;
			val[bit] = 1;
			bit++;
			renderinput();
		}
		if (input & KEY_B)
		{
			valtoprint = 0;
			val[bit] = 0;
			bit++;
			renderinput();
		}
		
	}

	//If bit limit reached, then it's time to convert
	if (bit == 8 && state!=2)
	{
        conversion();
		state = 2;
	}

	//Press DOWN to go home!!!!!!
	if (state==2 && input & KEY_DOWN)
	{
		aptReturnToMenu();
	}

	gfxFlushBuffers();
	gfxSwapBuffers();
}




//Screen render
void screenrender()
{
	gfxFlushBuffers();
	gfxSwapBuffers();
	screenBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);
}

//Main screen
void maintitle()
{
	clearScreen(screenBottom, GFX_BOTTOM);

	char buffer[100];
	sprintf(buffer, "***************************************");
	drawString(buffer, 1, 1, 255, 255, 255, screenBottom, GFX_BOTTOM);

	sprintf(buffer, "*                                     *");
	drawString(buffer, 1, 11, 255, 255, 255, screenBottom, GFX_BOTTOM);

	sprintf(buffer, "*     Binary -> Decimal Converter     *");
	drawString(buffer, 1, 21, 255, 255, 255, screenBottom, GFX_BOTTOM);

    sprintf(buffer, "*                                     *");
	drawString(buffer, 1, 31, 255, 255, 255, screenBottom, GFX_BOTTOM);

	sprintf(buffer, "*****  by AlbertoSONIC and Relys ******");
	drawString(buffer, 1, 41, 255, 255, 255, screenBottom, GFX_BOTTOM);

	sprintf(buffer, "Insert a binary number that you want to");
	drawString(buffer, 1, 61, 255, 255, 255, screenBottom, GFX_BOTTOM);

	sprintf(buffer, "convert (A=1, B=0, max 8bit)");
	drawString(buffer, 1, 71, 255, 255, 255, screenBottom, GFX_BOTTOM);

}


//Input rendering
void renderinput()
{
	printinput();
	screenrender();
	printinput();
	screenrender();
	position = position + 8;
}

//Input printing
void printinput()
{
	if (valtoprint == 1)
	{
		drawString(A, position, 81, 255, 255, 255, screenBottom, GFX_BOTTOM);
	}
	if (valtoprint == 0)
	{
		drawString(B, position, 81, 255, 255, 255, screenBottom, GFX_BOTTOM);	
	}
	
}

//Conversion
void conversion()
{

	//New conversion method: uses a for in order to accept any binary number lenght
	int result;
	int k = bit - 1;
	int i;
	for (i = 0; i<bit; i++)
	{
		result = val[k] << i;
		converted = converted + result;
		k = k - 1;
	}

	renderconversion();
}

void printconversion()
{
	char buffer[100];
	
	sprintf(buffer, "The result is %d", converted);
	drawString(buffer, 1, 111, 255, 255, 255, screenBottom, GFX_BOTTOM);

	sprintf(buffer, "Thank you for using this homebrew!");
	drawString(buffer, 1, 131, 255, 255, 255, screenBottom, GFX_BOTTOM);

	sprintf(buffer, "Press UP to restart this homebrew.");
	drawString(buffer, 1, 211, 255, 255, 255, screenBottom, GFX_BOTTOM);

	sprintf(buffer, "Press DOWN to go back to 3DS main menu.");
	drawString(buffer, 1, 221, 255, 255, 255, screenBottom, GFX_BOTTOM);
}
void renderconversion()
{
	printconversion();
	screenrender();
	printconversion();
	screenrender();
}