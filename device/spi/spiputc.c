/**
 * File              : spiputc.c
 * Author            : Robin Krens <robin@robinkrens.nl>
 * Date              : 16.11.2019
 * Last Modified Date: 16.11.2019
 * Last Modified By  : Robin Krens <robin@robinkrens.nl>
 */
#include <xinu.h>

int32	spiputc (struct dentry *devptr, uint8 c)
{
	struct spi_csreg *spiptr;	/* SPI control and status regs	*/
	int cnt = 0;

	/* Configure SPI peripheral */
	if(devptr->dvminor == 0) {
		spiptr = (struct spi_csreg *)SPI2_BASE;
	}
	else {
		kprintf("SPI device not recognized\n");
		return SYSERR;
	}

	/* Check if buffer is empty -- busy wait TODO: interrupts */
	//while (spiptr->sr & ~(1 << SPI_TXE));
        //while(!rchkbit(SPI2_SR, 1)) {
	while (!((spiptr->sr >> SPI_TXE) & 0x1)) {
                cnt++;
                if (cnt > TFT_TIMEOUT) {
                        kprintf("Error: transmit timeout!\n");
                        return SYSERR;
                }
	}
	/* Write data register */
	spiptr->dr = c;

	return OK;
}
