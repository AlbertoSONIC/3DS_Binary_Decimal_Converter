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
char HEX[6];
int bit_count = 0;
int hex_count = -1;
int mode;

//Variable reset
void variableReset()
{
	int i = 0;
	DECIMAL = 0;
	BINARY = 0;
	for (i = 0; i < 5; i++)
	{
		HEX[i] = '\0'; 
	}
	bit_count = 0;
}

void app()
{
	//UP TO RESET
	if (input & KEY_UP) variableReset();
	
	//CHECK PRESSED NUMBERS
	//Get input
	//DECIMAL INPUT
	if ((posX >= 10 && posX <= 51) && (posY >= 9 && posY <= 48))PRESSED_NUMBER = 1;
	else if ((posX >= 55 && posX <= 95) && (posY >= 9 && posY <= 48))PRESSED_NUMBER = 2;
	else if ((posX >= 100 && posX <= 140) && (posY >= 9 && posY <= 48))PRESSED_NUMBER = 3;
	else if ((posX >= 10 && posX <= 51) && (posY >= 53 && posY <= 95))PRESSED_NUMBER = 4;
	else if ((posX >= 55 && posX <= 95) && (posY >= 53 && posY <= 95))PRESSED_NUMBER = 5;
	else if ((posX >= 100 && posX <= 140) && (posY >= 53 && posY <= 95))PRESSED_NUMBER = 6;
	else if ((posX >= 10 && posX <= 51) && (posY >= 100 && posY <= 140))PRESSED_NUMBER = 7;
	else if ((posX >= 55 && posX <= 95) && (posY >= 100 && posY <= 140))PRESSED_NUMBER = 8;
	else if ((posX >= 100 && posX <= 140) && (posY >= 100 && posY <= 140))PRESSED_NUMBER = 9;
	else if ((posX >= 10 && posX <= 77) && (posY >= 163 && posY <= 193))PRESSED_NUMBER = 0;
	else if ((posX >= 82 && posX <= 140) && (posY >= 163 && posY <= 193)) PRESSED_NUMBER = -1; //Decimal "Cancel" button//BINARY KEYBOARD
	else if ((posX >= 124 && posX <= 184) && (posY >= 202 && posY <= 236))PRESSED_NUMBER = 20; //Binary "0"
	else if ((posX >= 59 && posX <= 118) && (posY >= 202 && posY <= 236))PRESSED_NUMBER = 21; //Binary "1"
	else if ((posX >= 189 && posX <= 250) && (posY >= 202 && posY <= 236)) PRESSED_NUMBER = -2; //Binary "Cancel" button//HEX KEYBOARD
	else if ((posX >= 179 && posX <= 209) && (posY >= 9 && posY <= 48))PRESSED_NUMBER = 31; //hex 1
	else if ((posX >= 213 && posX <= 244) && (posY >= 9 && posY <= 48))PRESSED_NUMBER = 32; //hex 2
	else if ((posX >= 247 && posX <= 279) && (posY >= 9 && posY <= 48))PRESSED_NUMBER = 33; //hex 3
	else if ((posX >= 179 && posX <= 209) && (posY >= 54 && posY <= 96))PRESSED_NUMBER = 34; //hex 4
	else if ((posX >= 213 && posX <= 244) && (posY >= 54 && posY <= 96))PRESSED_NUMBER = 35; //hex 5
	else if ((posX >= 247 && posX <= 279) && (posY >= 54 && posY <= 96))PRESSED_NUMBER = 36; //hex 6
	else if ((posX >= 179 && posX <= 209) && (posY >= 108 && posY <= 140))PRESSED_NUMBER = 37; //hex 7
	else if ((posX >= 213 && posX <= 244) && (posY >= 108 && posY <= 140))PRESSED_NUMBER = 38; //hex 8
	else if ((posX >= 247 && posX <= 279) && (posY >= 108 && posY <= 140))PRESSED_NUMBER = 39; //hex 9
	else if ((posX >= 179 && posX <= 226) && (posY >= 146 && posY <= 176))PRESSED_NUMBER = 30; //hex 0
	else if ((posX >= 231 && posX <= 279) && (posY >= 146 && posY <= 176))PRESSED_NUMBER = -3; //hex "Cancel"
	else if ((posX >= 283 && posX <= 313) && (posY >= 9 && posY <= 31))PRESSED_NUMBER = 41; //hex A
	else if ((posX >= 283 && posX <= 313) && (posY >= 36 && posY <= 59))PRESSED_NUMBER = 42; //hex B
	else if ((posX >= 283 && posX <= 313) && (posY >= 64 && posY <= 89))PRESSED_NUMBER = 43; //hex C
	else if ((posX >= 283 && posX <= 313) && (posY >= 94 && posY <= 118))PRESSED_NUMBER = 44; //hex D
	else if ((posX >= 283 && posX <= 313) && (posY >= 122 && posY <= 148))PRESSED_NUMBER = 45; //hex E
	else if ((posX >= 283 && posX <= 313) && (posY >= 153 && posY <= 176))PRESSED_NUMBER = 46; //hex F
	else PRESSED_NUMBER = 10;

	//Process input and conversion
	if (OLD_PRESSED_NUMBER == 10 && PRESSED_NUMBER != 10)
	{
		if (PRESSED_NUMBER == 20 || PRESSED_NUMBER == 21 || PRESSED_NUMBER == -2)
		{
			//BINARY--->DECIMAL & HEX:
			if (PRESSED_NUMBER != -2) { if (bit_count < 20){ BINARY *= 10; BINARY += (PRESSED_NUMBER - 20); bit_count++; } }
			else { BINARY /= 10; bit_count--; }
			DECIMAL = bin2Dec(BINARY);
			dec2Hex(DECIMAL, HEX);
		}
		else if (posX < 160)
		{
			//DECIMAL--->BINARY & HEX:
			if (PRESSED_NUMBER != -1) { if (DECIMAL*10+PRESSED_NUMBER<=maxDec){ DECIMAL *= 10; DECIMAL += PRESSED_NUMBER; } }
			else DECIMAL /= 10;
			BINARY = dec2Bin(DECIMAL);
			dec2Hex(DECIMAL, HEX);
		}
		else
		{
			//HEX--->DECIMAL&BIN  
			if (PRESSED_NUMBER == -3)
			{
				if (hex_count != -1)
				{
					HEX[hex_count] = '\0';
					hex_count--;
				}				
			}
			else if (hex_count!=4)
			{				
				hex_count++;				
				if (PRESSED_NUMBER > 40 && PRESSED_NUMBER < 48)
				{
					if (PRESSED_NUMBER == 41)HEX[hex_count] = 'A';
					if (PRESSED_NUMBER == 42)HEX[hex_count] = 'B';
					if (PRESSED_NUMBER == 43)HEX[hex_count] = 'C';
					if (PRESSED_NUMBER == 44)HEX[hex_count] = 'D';
					if (PRESSED_NUMBER == 45)HEX[hex_count] = 'E';
					if (PRESSED_NUMBER == 46)HEX[hex_count] = 'F';
				}
				else
				{
					if(!(PRESSED_NUMBER==30 && hex_count==0)) HEX[hex_count] = PRESSED_NUMBER - 30 + 48;
					else hex_count--;
				}
				
			}
			if (hex2Dec(HEX) > maxDec)
			{
				HEX[hex_count] = '\0';
				hex_count--;
			}
			DECIMAL = hex2Dec(HEX);
			BINARY = dec2Bin(DECIMAL);
		}
	}
	OLD_PRESSED_NUMBER = PRESSED_NUMBER;
}

void printGUI()
{
	//Prints the gui
	guiTop(DECIMAL, BINARY, HEX);
	guiBottom(); 
}

// ------------------------------ CONVERSION FUNCTIONS -------------------------------------
// Everything below is completely written by AlbertoSONIC. Fell free to suggest improvements!
long long unsigned dec2Bin(long long unsigned DECIMAL)
{
	int size = 0;
	int i = 0;
	long long unsigned n2 = DECIMAL;
	long long unsigned n = n2;
	bit_count = 0;
	long long unsigned BINARY = 0;
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
	return BINARY;
}

void dec2Hex(long long unsigned DECIMAL, char HEX[])
{
	int i = 0;
	long long unsigned n = DECIMAL;
	int vet[5];
	hex_count = -1;
	for (i = 0; i <5; i++) HEX[i] = '\0';
	i = 0;
	while (n != 0)
	{
		hex_count++;
		int k = n % 16;
		vet[i] = k;
		n /= 16;
		i++;
	}
	int j = 0;
	for (i = hex_count; i >= 0; i--)
	{
		if (vet[j] == 10)HEX[i] = 65;
		else if (vet[j] == 11)HEX[i] = 66;
		else if (vet[j] == 12)HEX[i] = 67;
		else if (vet[j] == 13)HEX[i] = 68;
		else if (vet[j] == 14)HEX[i] = 69;
		else if (vet[j] == 15)HEX[i] = 70;
		else HEX[i] = vet[j] + 48;
		j++;
	}
}

long long unsigned bin2Dec(long long unsigned BINARY)
{
	long long unsigned int r = 0;
	long long unsigned int e = 0;
	long long unsigned n = BINARY;
	long long unsigned DECIMAL = 0;
	while (n != 0)
	{
		r = n % 10;
		n /= 10;
		DECIMAL += r*pow(2, e);
		e++;
	}
	return DECIMAL;
}


long long unsigned hex2Dec(char HEX[])
{
	int r = 0;
	int e = 0;
	long long unsigned DECIMAL = 0;
	int i = 0;
	for (i = hex_count; i >= 0; i--)
	{
		if (HEX[i] == 65)r = 10;
		else if (HEX[i] == 66)r = 11;
		else if (HEX[i] == 67)r = 12;
		else if (HEX[i] == 68)r = 13;
		else if (HEX[i] == 69)r = 14;
		else if (HEX[i] == 70)r = 15;
		else r = HEX[i] - 48;
		DECIMAL += r*pow(16, e);
		e++;
	}
	return DECIMAL;
}


