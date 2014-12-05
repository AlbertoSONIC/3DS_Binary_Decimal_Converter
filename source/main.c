#include "main.h"
#include "program.h"

/* New ctrulib */
#include <3ds.h>
int main()
{
	//Initialize 3ds services
	srvInit();
	fsInit();
	aptInit();
	gfxInit();
	hidInit(NULL);

	// Main loop
	while (aptMainLoop())
	{
		gspWaitForVBlank();
		program();
	}

	//Close all opened services.
	hidExit();
	gfxExit();
	aptExit();
	srvExit();
	//Kill the process.
	svcExitProcess();
	return 0;
}