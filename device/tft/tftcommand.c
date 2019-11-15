/**
 * File              : tftcommand.c
 * Author            : Robin Krens <robin@robinkrens.nl>
 * Date              : 15.11.2019
 * Last Modified Date: 16.11.2019
 * Last Modified By  : Robin Krens <robin@robinkrens.nl>
 */
#include <xinu.h>

extern struct tftcblk tftcontrol;

//static int txbuf_empty (struct spi_csreg * reg) {
//	
//	int cnt = 0;
//        //while(!rchkbit(SPI2_SR, 1)) {
//	while (!((reg->sr >> SPI_TXE) & 0x1)) {
//                cnt++;
//                if (cnt > TFT_TIMEOUT) {
//                        kprintf("Error: transmit timeout!\n");
//                        return 0;
//                }
//        }
//        return 1;
//
//}

int tftcommand(uint8 cmd, int argsc, ...) {

	va_list ap;
	struct tftcblk *tftptr =  &tftcontrol;
	struct dentry * gptr = tftptr->gpioptr;
	struct dentry * sptr = tftptr->spiptr;

	// command
	gptr->dvwrite(gptr, 0x0, PIN6); // line low
	if (sptr->dvputc(sptr, cmd) == SYSERR)
		return -1;

	// parameter or data
	if (argsc > 0) {
		va_start(ap, argsc);
		gptr->dvwrite(gptr, (void *) 0x1, PIN6); // line high
		for (int i = 0; i < argsc; i++) {
			uint8 p = (uint8) va_arg(ap, unsigned int);
			if (sptr->dvputc(sptr, p) == SYSERR)
				return -1;
			//if (!txbuf_empty((struct spi_csreg *) sptr->dvcsr))
		}
		va_end(ap);
	}
	return 0;
}

