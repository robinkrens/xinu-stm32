/**
 * File              : tftcommand.c
 * Author            : Robin Krens <robin@robinkrens.nl>
 * Date              : 15.11.2019
 * Last Modified Date: 16.11.2019
 * Last Modified By  : Robin Krens <robin@robinkrens.nl>
 */
#include <xinu.h>

extern struct tftcblk tftcontrol;

int tftcommand(uint8 cmd, int argsc, ...) {

	va_list ap;
	struct tftcblk *tftptr =  &tftcontrol;
	struct dentry * gptr = tftptr->gpioptr;
	struct dentry * sptr = tftptr->spiptr;

	// command
	gptr->dvwrite(gptr, 0x0, PIN6); // line low
	if (sptr->dvputc(sptr, cmd) == SYSERR)
		return SYSERR;

	// parameter or data
	if (argsc > 0) {
		va_start(ap, argsc);
		gptr->dvwrite(gptr, (void *) 0x1, PIN6); // line high
		for (int i = 0; i < argsc; i++) {
			uint8 p = (uint8) va_arg(ap, unsigned int);
			if (sptr->dvputc(sptr, p) == SYSERR)
				return SYSERR;
		}
		va_end(ap);
	}
	return OK;
}

