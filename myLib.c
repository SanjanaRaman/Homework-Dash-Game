#include <stdio.h>
#include "myLib.h"
#include "sanj.h"
#include "startscreen.h"
#include "bill.h"
#include "endscreen.h"
#include "pass_screen.h"


u16* videoBuffer = (u16*)0x6000000;

void waitForVblank()
{
    while(SCANLINECOUNTER > 160);
    while(SCANLINECOUNTER < 160);
}

void drawBlackRectangle(int row, int col, int width, int height) {
	unsigned short black = BLACK; 
	for (int r = 0; r < height; r++) {
		DMA[3].src = &black;
		DMA[3].dst = &videoBuffer[(row+r) * 240 + col];
		DMA[3].cnt = width | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | DMA_ON;
	}
}

void setPixel(int row, int col, unsigned short color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}



int rekt(int pointrow, int pointcol, int rectrow, int rectcol, int width, int height) {
	// Logic for how a homework is hit by the laser
	return pointrow > rectrow && pointcol > rectcol && pointcol < rectcol + width && pointrow < rectrow + height;
}

void drawBullet(int row, int col, int width, int height) { //Draws red bullets
	unsigned short red = RED; 
	for (int r = 0; r < height; r++) {
		DMA[3].src = &red;
		DMA[3].dst = &videoBuffer[(row+r) * 240 + col];
		DMA[3].cnt = width | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | DMA_ON;
	}
}

void drawImage(int row, int col, int width, int height, const u16* image) {
    for (int r = 0; r < height; r++) {
	DMA[3].src = &image[width*r];
	DMA[3].dst = &videoBuffer[(row+r) * 240 + col];
	DMA[3].cnt = width | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
    }
}


void drawChar(int row, int col, char ch, unsigned short color) {
	for(int r = 0; r< 8; r++) {
		for(int c=0; c< 6; c++) {
			if(fontdata_6x8[OFFSET(r, c, 6) + ch*48] == 1) {
				setPixel(r+row, c+col, color);
			}
		}

	}
}

void drawString(int row, int col, char str[], unsigned short color) {
	while(*str) {
		drawChar(row, col, *str++, color);
		col += 6;
	}
}

void reset() { //Resets all variables for new game
	prevloc = 0;
	bulletmoving = 0;
	bulletx = 0;
	bullety = 150 - SANJ_HEIGHT;
	initialhwcol = 0;
	droppedhomeworks = 0;
	passedhomeworks = 0;
	lives = 5;
	gameisplaying = 0;

}

void drawGame() { //Draws game with scores and Bill
	drawBlackRectangle(0, 0, STARTSCREEN_WIDTH, STARTSCREEN_HEIGHT);
	drawString(5, 25, "Passed:", RED);
	drawString(5, 150, "Lives:", RED);
    	drawImage(0, (240 - BILL_WIDTH)/2, BILL_WIDTH, BILL_HEIGHT, bill);
	gameisplaying = 1;
}

void drawEndScreen() { //Draws screen when game is lost
	drawImage(0, 0, ENDSCREEN_WIDTH, ENDSCREEN_HEIGHT, endscreen);
	drawString(102, 140, "You only passed", RED);
	char buffer[41];
	sprintf(buffer, "%d homeworks", passedhomeworks);
	drawString(122, 150, buffer, RED);
}

void scores() { //Sets passed homeworks and lives scores
	char buffer[41];
	drawBlackRectangle(5, 70, 20, 20);
	sprintf(buffer, "%d", passedhomeworks);
	drawString(5, 70, buffer, RED);
	drawBlackRectangle(5, 190, 20, 20);
	sprintf(buffer, "%d", lives);
	drawString(5, 190, buffer, RED);
}

void drawWinScreen() { //Draws screen when game is won
	drawImage(0, 0, PASS_SCREEN_WIDTH, PASS_SCREEN_HEIGHT, pass_screen);
	drawString(102, 140, "Congrats you", RED);
	drawString(122, 150, "passed", RED);
}
