/**
 * File              : tft.h
 * Author            : Robin Krens <robin@robinkrens.nl>
 * Date              : 15.11.2019
 * Last Modified Date: 16.11.2019
 * Last Modified By  : Robin Krens <robin@robinkrens.nl>
 */
#define TFT_TIMEOUT 500
#define SCRWIDTH 132
#define SCRHEIGHT 132
#define STARTX 5
#define STARTY 5
#define XPOS(x) (x * 6)
#define YPOS(y) (y * 8)
#define BUFFER 352 /* Max number of characters on screen */

struct tftcblk {
	int32 tftnum;
	struct dentry * spiptr;
	struct dentry * gpioptr;
};

struct tftshape {
	uint8 shape;
	uint8 beginx;
	uint8 beginy;
	uint8 endy;
	uint8 endx;
	uint16 color;
};

struct tftscreen {
	 uint16 cpos;
	 uint8 * textmemptr;
       	 uint8 buf[BUFFER];
         uint8 x;
       	 uint8 y;
}; 

enum CMDLIST {
	TFT_NOP = 0x00,
	TFT_SWRESET = 0x01,
	/* READ COMMANDS: returns one or multiple bytes,
	 * use these for debugging to check if certain
	 * registers are set correctly */
	TFT_RDDID = 0x04,
	TFT_RDDST = 0x09,
        TFT_RDDPM = 0x0A,
	TFT_RDDMADCTL = 0x0B,
	TFT_RDDCOLMOD = 0x0C,
	TFT_RDDIM = 0x0D,
	TFT_RDDSM = 0x0E,
	TFT_RDDSDR = 0x0F,
	TFT_RDID1 = 0xDA,
	TFT_RDID2 = 0xDB,
	TFT_RDID3 = 0xDC,
	TFT_RAMRD = 0x2E,
	/* DISPLAY MODE COMMANDS */
	TFT_SLPIN = 0x10,
       	TFT_SLPOUT = 0x11,
	TFT_PTLON = 0x12,
	TFT_NORON = 0x13,
	TFT_INVOFF = 0x20,
	TFT_INVON = 0x21,
	TFT_GAMSET = 0x26,
	TFT_DISPOFF = 0x28,
	TFT_DISPON = 0x29,
	TFT_IDMOFF = 0x38,
	TFT_IDMON = 0x39,
	TFT_TEOFF = 0x34,
	TFT_TEON = 0x35,
	/* BASIC SETTINGS COMMANDS */
	TFT_CASET = 0x2A,
	TFT_RASET = 0x2B,
	TFT_RAMWR = 0x2C,
	TFT_RGBSET = 0x2D,
	TFT_PTLAR = 0x30,
	TFT_SCRLAR = 0x33,
	TFT_MADCTL = 0x36,
	TFT_VSCSAD = 0x37,
	TFT_COLMOD = 0x3A,
	/* PANEL FUNCTIONS COMMANDS */
	TFT_FRMCTR1 = 0xB1,
	TFT_FRMCTR2 = 0xB2,
	TFT_FRMCTR3 = 0xB3,
	TFT_INVCTR = 0xB4,
	TFT_PWCTR1 = 0xC0,
	TFT_PWCTR2 = 0xC1,
	TFT_PWCTR3 = 0xC2,
	TFT_PWCTR4 = 0xC3,
	TFT_PWCTR5 = 0xC4,
	TFT_VMCTR1 = 0xC5,
	TFT_VMOFCTR = 0xC7,
        TFT_WRID2 = 0xD1,
	TFT_WRID3 = 0xD2,
	TFT_NVCTR = 0xD9,
	TFT_NVCTR2 = 0xDE,
	TFT_NVCTR3 = 0xDF,
	/* GAMMA SETTINGS */
	TFT_GMCTRP1 = 0xE0,
	TFT_GMCTRN1 = 0xE1

};
