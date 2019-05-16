typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef char s8;
typedef short s16;
typedef int s32;

typedef struct
{
	int row;
	int col;
	int zigzagwidth;
	short iszigging; 
	short isonscreen;
} HOMEWORKS;


int prevloc;
short bulletmoving;
int bulletx;
int bullety;
int initialhwcol;
int droppedhomeworks;
int passedhomeworks;
int lives;
int gameisplaying;


#define TRUE 1
#define FALSE 0
extern const unsigned char fontdata_6x8[12288];
extern u16* videoBuffer;
#define REG_DISPCNT  *(u16*) 0x4000000
#define SCANLINECOUNTER *(unsigned short *)0x4000006

#define COLOR(r,g,b) ((r) + ((g) << 5) + ((b) << 10))

//Background Enables
#define BG0_ENABLE  (1<<8)
#define BG1_ENABLE  (1<<9)
#define BG2_ENABLE  (1<<10)
#define BG3_ENABLE  (1<<11)

#define BLACK   COLOR(0,0,0)
#define RED     COLOR(31,0,0)

#define MODE_0 0
#define MODE_1 1
#define MODE_2 2
#define MODE_3 3
#define MODE_4 4
#define MODE_5 5
#define MODE_6 6

/* DMA */

typedef struct
{
	const volatile void *src;
	const volatile void *dst;
	u32                  cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
// DMA channels
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

// DMA copies 16 bits at a time normally (1 pixel), 
// but can be set to copy 32 bits at a time to be 
// even faster for mutliples of 32 bits.
#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)
#define OFFSET(r, c, rowlen) ((r)*(rowlen)+(c))


// *** Input =========================================================

// Buttons

#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

#define BUTTONS (*(unsigned int *)0x4000130)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

// Prototypes
void drawChar(int row, int col, char ch, unsigned short color);
void drawString(int row, int col, char str[], unsigned short color);
void waitForVblank();
int rekt(int pointrow, int pointcol, int rectrow, int rectcol, int width, int height);
void drawBlackRectangle(int row, int col, int width, int height);
void drawBullet(int row, int col, int width, int height);
void drawImage(int row, int col, int width, int height, const u16* image);
void setPixel(int row, int col, unsigned short color);
void reset();
void drawGame();
void drawEndScreen();
void scores();
void drawWinScreen();
