#include "program.h"
#include "draw.h"

//For ctrulib
#include <3ds\types.h>
#include <3ds\services\hid.h>
#include <3ds\gfx.h>

//For sprintF
#include <stdio.h>


//STATE: 0=start, 2=end, 1=on going, 25=init
int converted = 0;
int max = 8;
int bit = 0;
int state = 10;
int refreshrequired = 0;
int valtoprint;
int val[100];
int position = 1;
char* A = "1";
char* B = "0";
u32 input;
u8* screenBottom = 0;
u8* screenTopLeft = 0;
u8* screenTopRight = 0;

void variablereset()
{
	position = 1;
	bit = 0;
		converted = 0;
		int ii;
		for (ii = 0; ii<max; ii++)
		{
			val[ii] = 0;
		}
		input = 0;
}


void program()
{
	hidScanInput();
	input = hidKeysDown();

	//Top screen print
	if (state == 10)
	{
		screenTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		screenTopRight = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		clearScreen(screenTopLeft, GFX_LEFT);
		clearScreen(screenTopRight, GFX_LEFT);

		printtop();
		gfxFlushBuffers();
		gfxSwapBuffers();
		screenTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		screenTopRight = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		printtop();
		gfxFlushBuffers();
		gfxSwapBuffers();
		screenTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		screenTopRight = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);

		state = 25;


	}

	//Init
	if (state == 25 || (state==2 && input & KEY_SELECT))
	{
		state = 25;
		screenBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);
		
		printbottominit();
		screenrender();


		if (input&KEY_R)
		{
			max = max + 1;
		}

		if (input & KEY_L)
		{
			max = max - 1;
		}

		if (input & KEY_START)
		{
			state = 0;
			bit = max;
			
		}
	}
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
	if (bit < max || state==1)
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
	if (bit == max && state==1)
	{
        conversion();
		state = 2;
	}

	//Press DOWN to go home!!!!!!
	if (state==2 && input & KEY_DOWN)
	{
		aptReturnToMenu();
		refreshrequired = 1;
	}
	//refresh required checks if you went back to home and then back to homebrew. If so, it needs to refresh graphics.
	if (refreshrequired == 1)
	{
		gfxFlushBuffers();
		gfxSwapBuffers();
		refreshrequired = 0;
	}	
}


void printbottominit()
{
	clearScreen(screenBottom, GFX_BOTTOM);
	char buffer[100];
	sprintf(buffer, "Choose binary number lenght.");
	drawString(buffer, 1, 1, 255, 255, 255, screenBottom, GFX_BOTTOM);

	sprintf(buffer, "The actual one is %d", max);
	drawString(buffer, 1, 11, 255, 255, 255, screenBottom, GFX_BOTTOM);

	sprintf(buffer, "Use L and R buttons to change it.");
	drawString(buffer, 1, 31, 255, 255, 255, screenBottom, GFX_BOTTOM);

	sprintf(buffer, "Press START to confirm.");
	drawString(buffer, 1, 41, 255, 255, 255, screenBottom, GFX_BOTTOM);
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

	sprintf(buffer, "Insert a binary number that you want to");
	drawString(buffer, 1, 1, 255, 255, 255, screenBottom, GFX_BOTTOM);

	sprintf(buffer, "convert (A=1, B=0, max %d)", max);
	drawString(buffer, 1, 11, 255, 255, 255, screenBottom, GFX_BOTTOM);

	sprintf(buffer, "INSERTED:");
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
	drawString(buffer, 1, 141, 255, 255, 255, screenBottom, GFX_BOTTOM);

	sprintf(buffer, "Press UP to insert a new binary num.");
	drawString(buffer, 1, 211, 255, 255, 255, screenBottom, GFX_BOTTOM);

	sprintf(buffer, "Press SELECT to change bin. num. lenght.");
	drawString(buffer, 1, 221, 255, 255, 255, screenBottom, GFX_BOTTOM);

	sprintf(buffer, "Press DOWN to go back to 3DS main menu.");
	drawString(buffer, 1, 231, 255, 255, 255, screenBottom, GFX_BOTTOM);
}
void renderconversion()
{
	printconversion();
	screenrender();
	printconversion();
	screenrender();
}


void printtop()
{
	char buffer[100];
	sprintf(buffer, "*************************************************");
	drawString(buffer, 1, 1, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 1, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "*************************************************");
	drawString(buffer, 1, 11, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 11, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "**                                             **");
	drawString(buffer, 1, 21, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 21, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "**                                             **");
	drawString(buffer, 1, 31, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 31, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "**                                             **");
	drawString(buffer, 1, 41, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 41, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "**                                             **");
	drawString(buffer, 1, 51, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 51, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "**          3DS Binary ---> Decimal            **");
	drawString(buffer, 1, 61, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 61, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "**              -  CONVERTER  -                **");
	drawString(buffer, 1, 71, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 71, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "**                                             **");
	drawString(buffer, 1, 81, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 81, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "**                                             **");
	drawString(buffer, 1, 91, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 91, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "**                                             **");
	drawString(buffer, 1, 101, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 101, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "**                                             **");
	drawString(buffer, 1, 111, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 111, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "*************************************************");
	drawString(buffer, 1, 121, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 121, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "*************************************************");
	drawString(buffer, 1, 131, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 131, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "**                                             **");
	drawString(buffer, 1, 141, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 141, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "**                                             **");
	drawString(buffer, 1, 151, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 151, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "**                                             **");
	drawString(buffer, 1, 161, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 161, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "**               Developed by                  **");
	drawString(buffer, 1, 171, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 171, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "**                                             **");
	drawString(buffer, 1, 181, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 181, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "**           AlbertoSONIC and Relys            **");
	drawString(buffer, 1, 191, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 191, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "**                                             **");
	drawString(buffer, 1, 201, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 201, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "**                                             **");
	drawString(buffer, 1, 211, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 211, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "*************************************************");
	drawString(buffer, 1, 221, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 221, 255, 255, 255, screenTopRight, GFX_LEFT);

	sprintf(buffer, "*************************************************");
	drawString(buffer, 1, 231, 255, 255, 255, screenTopLeft, GFX_LEFT);
	drawString(buffer, 1, 231, 255, 255, 255, screenTopRight, GFX_LEFT);
}