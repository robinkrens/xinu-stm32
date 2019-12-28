/**
 * File              : gpiowrite.c
 * Author            : Robin Krens <robin@robinkrens.nl>
 * Date              : 16.11.2019
 * Last Modified Date: 16.11.2019
 * Last Modified By  : Robin Krens <robin@robinkrens.nl>
 */
/* gpiowrite.c - gpiowrite */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  gpiowrite  -  Write value of gpio pins
 *------------------------------------------------------------------------
 */
devcall	gpiowrite(
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  char *value,			/* Pointer to 32-bit buffer	*/
	  int32 pin			/* Pins to be written written	*/
	)
{
	struct gpio_csreg * csrptr;
	uint8 val;
	
	csrptr = (struct gpio_csreg *)(devptr->dvcsr);
	val = (uint8) value;

	if (val == 0) {
		csrptr->odr &= ~(1 << pin);
	}
	else  if (val == 1) {
		csrptr->odr |= (1 << pin);
	}
	
	return OK;
}
