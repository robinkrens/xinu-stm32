/**
 * File              : device.h
 * Author            : Robin Krens <robin@robinkrens.nl>
 * Date              : 15.11.2019
 * Last Modified Date: 15.11.2019
 * Last Modified By  : Robin Krens <robin@robinkrens.nl>
 */
/* device.h - isbaddev */

#define	DEVNAMLEN	16	/* maximum size of a device name	*/

/* Macro used to verify device ID is valid  */

#define isbaddev(f)  ( ((f) < 0) | ((f) >= NDEVS) )
