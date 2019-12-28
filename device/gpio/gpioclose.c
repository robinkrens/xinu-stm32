/**
 * File              : gpioclose.c
 * Author            : Robin Krens <robin@robinkrens.nl>
 * Date              : 16.11.2019
 * Last Modified Date: 16.11.2019
 * Last Modified By  : Robin Krens <robin@robinkrens.nl>
 */

#include <xinu.h>

devcall	gpioclose(struct dentry	*devptr)
{
	struct clock_csreg * clockptr;
	clockptr = (struct clock_csreg *)CLOCK_BASE;
	
	/* Disable clock on GPIO (~ close device) */
	switch (devptr->dvminor) {
		case 0:
			clockptr->apb2enr &= ~(1 << IOPAEN);
			break;
		case 1:
			clockptr->apb2enr &= ~(1 << IOPBEN);
			break;
		case 2:
			clockptr->apb2enr &= ~(1 << IOPCEN);
			break;
		case 3:
			clockptr->apb2enr &= ~(1 << IOPDEN);
			break;
		default:
			kprintf("Failed to close GPIO device");
	}

	return OK;
}
