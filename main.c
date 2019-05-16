#include <stdlib.h>
#include "myLib.h"
#include "bill.h"
#include "sanj.h"
#include "hwpic.h"
#include "endscreen.h"
#include "startscreen.h"


int x = (240 - SANJ_WIDTH)/2;

int main() {
	reset();


    	REG_DISPCNT = MODE_3 | BG2_ENABLE;
	HOMEWORKS hw;
	

    	while (1) {
		if(KEY_DOWN_NOW(BUTTON_SELECT)) { //Resets (start screen) game when select button is pressed
			reset(); //Resets the score and other variables when game begins
			x = (240 - SANJ_WIDTH)/2;
			hw.isonscreen = 0;
			continue;
		}
		if (!gameisplaying) { //draws start screen if game is not playing
			drawImage(0, 0, STARTSCREEN_WIDTH, STARTSCREEN_HEIGHT, startscreen);		
			if(KEY_DOWN_NOW(BUTTON_START)) { //starts game when start button is pressed
				drawGame();
			}
		} else {
			if (droppedhomeworks >= 5) { //Game ends if user drops 5 homeworks
				drawEndScreen();
				while(1) {
					if (KEY_DOWN_NOW(BUTTON_SELECT)) { //Revert to start screen any time select is pressed
						reset();
						x = (240 - SANJ_WIDTH)/2;
						hw.isonscreen = 0;
						break;
					}
				}
			}
			else if (passedhomeworks >= 2110) { //User wins if they shoot down 2110 homeworks
				drawWinScreen();
				while(1) {
					if (KEY_DOWN_NOW(BUTTON_SELECT)) { //Revert to start screen any time select is pressed
						reset();
						x = (240 - SANJ_WIDTH)/2;
						hw.isonscreen = 0;
						break;
					}
				}
			}
			else {
				waitForVblank();
				scores(); //Sets up passed and lives scoring on the top of the game
				if (hw.isonscreen == 0) { //Randomly sends down one homework at a time in a zig zag pattern
					hw.row = BILL_HEIGHT;;
					hw.zigzagwidth = rand()%40 + 10;
					hw.col = rand()%(240-hw.zigzagwidth-HWPIC_WIDTH) + hw.zigzagwidth;
					hw.isonscreen = 1;
					initialhwcol = hw.col;
				}	
				prevloc = x;
				if(KEY_DOWN_NOW(BUTTON_RIGHT)) { //Right button shifts player to the right
					x = x + 2;
					if(x>(239-SANJ_WIDTH)) { //Ensures player stays within the screen
						x = 239-SANJ_WIDTH;
					}
				}
				if(KEY_DOWN_NOW(BUTTON_LEFT)) { //Left button shifts player to the left
					x = x - 2;
					if(x<0) {
						x=0;
					}
				}
				if(KEY_DOWN_NOW(BUTTON_UP)) { //Up button shoots lasers
					if (bulletmoving != 1) { //resets bullet for next shot
						bulletx = x + (SANJ_WIDTH/2);			
						bulletmoving = 1;	
					}		

				}
				drawBlackRectangle(159 - SANJ_HEIGHT, prevloc, SANJ_WIDTH, SANJ_HEIGHT);
	    			drawImage(159 - SANJ_HEIGHT, x, SANJ_WIDTH, SANJ_HEIGHT, sanj);
				if (bulletmoving != 0) {
					drawBlackRectangle(bullety + 4, bulletx, 2, 5);
					drawBullet(bullety, bulletx, 2, 5);
					bullety = bullety - 4;
				}
				if (bullety <= BILL_HEIGHT) { //stops bullet before it reaches header (scores and Bill)
					drawBlackRectangle(bullety + 4, bulletx, 2, 5);
					bulletmoving = 0;
					bullety = 150 - SANJ_HEIGHT;
				}
				if (hw.isonscreen != 0) { //zig zag motion for homework
					hw.row = hw.row + 1;
					if (hw.col == initialhwcol || hw.col <= 0) {
						hw.iszigging = 1;
					}
					if (hw.col == (initialhwcol + hw.zigzagwidth) || hw.col >= (239 - HWPIC_WIDTH)) {
						hw.iszigging = 0;
					}
					if (hw.iszigging == 1) {
						hw.col = hw.col + 1;
						if (hw.col >= (239 - HWPIC_WIDTH)) {
							hw.col = 239 - HWPIC_WIDTH;
						}
						if (hw.col < 0) {
							hw.col = 0;
						}
					} else {
						hw.col = hw.col - 1;
						if (hw.col < 0) {
							hw.col = 0;
						}
						if (hw.col > (239 - HWPIC_WIDTH)) {
							hw.col = 239 - HWPIC_WIDTH;	
						}
					}
					drawBlackRectangle(hw.row - 1, hw.col + 1, HWPIC_WIDTH, HWPIC_HEIGHT);
					drawImage(hw.row, hw.col, HWPIC_WIDTH, HWPIC_HEIGHT, hwpic);
				}
				if (hw.row >= 160) { //User loses life if homework falls
					hw.isonscreen = 0;
					droppedhomeworks = droppedhomeworks + 1;
					lives = lives - 1;
				}
				if (rekt(bullety, bulletx, hw.row, hw.col, HWPIC_WIDTH, HWPIC_HEIGHT)) { 
					//Score increases if homework is shot down
					hw.isonscreen = 0;
					drawBlackRectangle(hw.row, hw.col, HWPIC_WIDTH, HWPIC_HEIGHT);
					drawBlackRectangle(bullety + 4, bulletx, 2, 5);			
					bulletmoving = 0;
					bullety = 150 - SANJ_HEIGHT;
					passedhomeworks = passedhomeworks + 1;
				}
			}
		}
	} 
}
