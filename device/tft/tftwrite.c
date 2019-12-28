/**
 * File              : tftwrite.c
 * Author            : Robin Krens <robin@robinkrens.nl>
 * Date              : 16.11.2019
 * Last Modified Date: 16.11.2019
 * Last Modified By  : Robin Krens <robin@robinkrens.nl>
 */

#include <xinu.h>

extern struct tftcblk tftcontrol;

int tftwrite(struct dentry * dvptr, void * drawstruct, uint32 count) {

	struct tftshape * dptr = (struct tftshape *) drawstruct;

	int shape = dptr->shape;
	uint8 beginx = dptr->beginx;
	uint8 beginy = dptr->beginy;
	uint8 endx = dptr->endx;
	uint8 endy = dptr->endy;
	uint16 color = dptr->color;

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
