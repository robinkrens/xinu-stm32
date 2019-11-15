/**
 * File              : tftinit.c
 * Author            : Robin Krens <robin@robinkrens.nl>
 * Date              : 15.11.2019
 * Last Modified Date: 16.11.2019
 * Last Modified By  : Robin Krens <robin@robinkrens.nl>
 */
#include <xinu.h>
#define CLKSPEED_MHZ 8

struct tftcblk tftcontrol;

/* Block in us*/
void _block(uint32 us) {
 
         uint32 count = (us/3) * CLKSPEED_MHZ; // x cycles 
         while (count > 0) {
		 count--;
	 }
	 //asm volatile("b1: subs %0, %1, #1" "\n\t"
         //        "bne b1" : "=r" (count) : "r" (count));
 
}
 
int32	tftinit (struct	dentry *devptr)
{

	kprintf("INIT TFT!\n");

	struct dentry * gpiobptr = (struct dentry *) &devtab[GPIO1]; // B
	struct dentry * gpiocptr = (struct dentry *) &devtab[GPIO2]; // C
	struct dentry * spiptr = (struct dentry *) &devtab[SPI2];

	/* Dirty quick GPIO config */
	struct gpio_csreg * reg = gpiobptr->dvcsr;
	reg->crh = 0xA4AA4444;
	reg = gpiocptr->dvcsr;
	reg->crl = 0x42444444;
	
	/* Initialize control block */
	tftcontrol.tftnum = 0;
	tftcontrol.gpioptr =  gpiocptr;
	tftcontrol.spiptr = spiptr;

	/* Init sequence
	 * Sequence that cannot be interrupted 
	 * or otherwise init fails */
	tftcommand(TFT_SWRESET, 0);
	_block(120000);
	tftcommand(0x11, 0);
	_block(120000);

	/* Frame rate control */
	tftcommand(TFT_FRMCTR1, 3, 0x01, 0x2C, 0x2D);
	tftcommand(TFT_FRMCTR2, 3, 0x01, 0x2C, 0x2D);
	tftcommand(TFT_FRMCTR3, 6, 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D);	

	/* Power control */
	tftcommand(TFT_PWCTR1, 3, 0xA2, 0x02, 0x84);
	tftcommand(TFT_PWCTR2, 1, 0xC5);
	tftcommand(TFT_PWCTR3, 2, 0x0A, 0x00);
	tftcommand(TFT_PWCTR4, 2, 0x8A, 0x2A);
	tftcommand(TFT_PWCTR5, 2, 0x8A, 0xEE);
	tftcommand(TFT_VMCTR1, 1, 0x0E);

	tftcommand(TFT_INVOFF, 0);
	tftcommand(TFT_COLMOD, 1, 0x05); // 0x05
	tftcommand(TFT_MADCTL, 1, 0xC0); // TODO: check

	tftcommand(TFT_CASET, 4, 0x00, 0x00, 0x00, 0x7F);
	tftcommand(TFT_RASET, 4, 0x00, 0x00, 0x00, 0x9F);

	/* Gamma Settings */
	tftcommand(TFT_GMCTRP1, 16, 0x02, 0x1C, 0x07, 0x12,
			0x37, 0x32, 0x29, 0x2D,
			0x29, 0x25, 0x2B, 0x39,
			0x00, 0x01, 0x03, 0x10);
	tftcommand(TFT_GMCTRN1, 16, 0x03, 0x1D, 0x07, 0x06,
			0x2E, 0x2C, 0x29, 0x2D,
			0x2E, 0x2E, 0x37, 0x3F,
			0x00, 0x00, 0x02, 0x10);

	/* Before turning on the display, fill the display
	 * so no random display data is shown */
	//tft_fill(0,0,SCRWIDTH-1,SCRHEIGHT-1,0x0000);

	/* Turn on */
	tftcommand(TFT_NORON, 0);
	_block(10000);
	tftcommand(TFT_DISPON, 0);
	_block(100000);

	return OK;
}



