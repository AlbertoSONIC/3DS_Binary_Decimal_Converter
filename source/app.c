#include <3ds.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "app.h"
#include "draw.h"
#include "input.h"
#include "rendering.h"
#include "gui.h"

//Vars
int maxDec = 1048575;
int PRESSED_NUMBER= 10; //10 means no number pressed
int OLD_PRESSED_NUMBER= 10;
unsigned long long DECIMAL = 0;
unsigned long long BINARY = 0;
int bit_count = 0;
int mode;

//Variable reset
void variableReset()
{
	DECIMAL = 0;
	BINARY = 0;
	bit_count = 0;
}

void app()
{
	//UP TO RESET
	if (input & KEY_UP) variableReset();
	//CHECK PRESSED NUMBERS
	//Get input
	if ((posX >= 13 && posX <= 57) && (posY >= 26 && posY <= 66))PRESSED_NUMBER = 1;
	else if ((posX >= 61 && posX <= 100) && (posY >= 26 && posY <= 66))PRESSED_NUMBER = 2;
	else if ((posX >= 106 && posX <= 145) && (posY >= 26 && posY <= 66))PRESSED_NUMBER = 3;
	else if ((posX >= 13 && posX <= 57) && (posY >= 71 && posY <= 113))PRESSED_NUMBER = 4;
	else if ((posX >= 61 && posX <= 100) && (posY >= 71 && posY <= 113))PRESSED_NUMBER = 5;
	else if ((posX >= 106 && posX <= 145) && (posY >= 71 && posY <= 113))PRESSED_NUMBER = 6;
	else if ((posX >= 13 && posX <= 57) && (posY >= 118 && posY <= 158))PRESSED_NUMBER = 7;
	else if ((posX >= 61 && posX <= 100) && (posY >= 118 && posY <= 158))PRESSED_NUMBER = 8;
	else if ((posX >= 106 && posX <= 145) && (posY >= 118 && posY <= 158))PRESSED_NUMBER = 9;
	else if ((posX >= 13 && posX <= 77) && (posY >= 163 && posY <= 193))PRESSED_NUMBER = 0;
	else if ((posX >= 82 && posX <= 145) && (posY >= 163 && posY <= 193)) PRESSED_NUMBER = -1; //Decimal "Cancel" button
	else if ((posX >= 243 && posX <= 303) && (posY >= 67 && posY <= 101))PRESSED_NUMBER = 20; //Binary "0"
	else if ((posX >= 178 && posX <= 237) && (posY >= 67 && posY <= 101))PRESSED_NUMBER = 21; //Binary "1"
	else if ((posX >= 243 && posX <= 303) && (posY >= 106 && posY <= 142)) PRESSED_NUMBER = -2; //Binary "Cancel" button
	else PRESSED_NUMBER = 10;

	//Apply input
	if (OLD_PRESSED_NUMBER == 10 && PRESSED_NUMBER != 10)
	{
		if (PRESSED_NUMBER != 20 && PRESSED_NUMBER != 21 && PRESSED_NUMBER != -2) //If it's decimal input...
		{
			if (PRESSED_NUMBER != -1) { if (DECIMAL*10+PRESSED_NUMBER<=maxDec){ DECIMAL *= 10; DECIMAL += PRESSED_NUMBER; } }
			else DECIMAL /= 10;
		}
		else //else it's binary input
		{
			if (PRESSED_NUMBER != -2) { if (bit_count < 20){ BINARY *= 10; BINARY += (PRESSED_NUMBER - 20); bit_count++; } }
			else { BINARY /= 10; bit_count--; }
		}
	}
	OLD_PRESSED_NUMBER = PRESSED_NUMBER;

	//CONVERSION CODE
	if (PRESSED_NUMBER != 10)
	{
		if (PRESSED_NUMBER != 20 && PRESSED_NUMBER != 21 && PRESSED_NUMBER != -2) mode = 1; //if last input was related to the decimal part, then it does decimal-->binary conversion
		else mode = 2; //else it's binary--->decimal

		if (mode == 1)
		{  //DECIMAL--->BINARY
			int size = 0;
			int i = 0;
			int n2 = DECIMAL;
			int n = n2;
			bit_count = 0;
			BINARY = 0;
			while (n2 != 0)
			{
				n2 /= 2;
				size++;
			}
			int vet[size];
			while (n != 0)
			{
				vet[i] = n % 2;
				n /= 2;
				i++;
			}
			for (i = size - 1; i >= 0; i--)
			{
				BINARY *= 10; BINARY += vet[i]; bit_count++;
			}
		}
		else
		{   //BINARY--->DECIMAL:
			long long unsigned int r = 0;
			long long unsigned int e = 0;
			long long unsigned n = BINARY;
			DECIMAL = 0;
			while (n != 0)
			{
				r = n % 10;
				n /= 10;
				DECIMAL += r*pow(2, e);
				e++;
			}	
		}
	}
}

void printGUI()
{
	//Prints the GUI depending on the active mode
	guiTop(DECIMAL, BINARY);
	guiBottom(); 
}

