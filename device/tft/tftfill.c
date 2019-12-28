/**
 * File              : tftfill.c
 * Author            : Robin Krens <robin@robinkrens.nl>
 * Date              : 16.11.2019
 * Last Modified Date: 16.11.2019
 * Last Modified By  : Robin Krens <robin@robinkrens.nl>
 */
/* Function to fill an area starting at (beginx, beginy)
 * and end ending at (endx, endy */

#include <xinu.h>

extern struct tftcblk tftcontrol;

int tft_fill(uint8 beginx, uint8 beginy, uint8 endx,
	       	uint8 endy, uint16 color) {

	struct tftcblk *tftptr =  &tftcontrol;
	struct dentry * gptr = tftptr->gpioptr;
	struct dentry * sptr = tftptr->spiptr;

	uint8 colorh = (uint8) (color >> 8);
	uint8 colorl = (uint8) (color & 0xFF);
	/* The CASET and RASET commands set the begin X/Y
	 * and end X/Y of the fill area. Both are split over 
	 * two paramters/bytes. Most screen are small, so you will 
	 * only need to consider the first 8 MSBs. Parameter 1 and 
	 * 3 are always 0. 
	 *
	 * In theory, TFT screens of 65,535 by 65,535 could be
	 * supported */
	tftcommand(TFT_CASET, 4, 0x00, beginx, 0x00, endx);
	tftcommand(TFT_RASET, 4, 0x00, beginy, 0x00, endy);

	/* After setting the fill area, we can send the color
	 * data. The chip autoincrements the address so we can
	 * keep writing continiously for the fill area. Note
	 * that each pixel requires two bytes to be send (16 
	 * bit mode: ...)   */
	uint32 totalwrites = (endx - beginx) * (endy - beginy) * 2;
	//printf("tw: %d, x: %d, y: %d\n", totalwrites, beginx, beginy);
	tftcommand(TFT_RAMWR, 0);
	gptr->dvwrite(gptr, (void *) 0x1, PIN6); // line high
	for (int i = 0; i < totalwrites; i++) {
			if (sptr->dvputc(sptr, colorh) == SYSERR)
				return SYSERR;
			if (sptr->dvputc(sptr, colorl) == SYSERR)
				return SYSERR;
	}
	return OK;
}
