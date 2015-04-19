#include <3ds.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "main.h"
#include "app.h"
#include "draw.h"
#include "input.h"
#include "rendering.h"

//FPS Counter
bool GW_MODE;

int main()
{
	// Initialize services
	srvInit();
	aptInit();
	hidInit(NULL);
	gfxInitDefault();

	// Main loop
	while (aptMainLoop())
	{
		//As nop90 suggested
		getFB();

		//Gets input (keys and touch)
		getInput();

		//Prints the GUI
		printGUI();  

		//Do stuff
		app();

		//Exit code
		if (input & KEY_START) break;
	
		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();

	}

	// Exit services
	gfxExit();
	hidExit();
	aptExit();
	srvExit();
	return 0;
}