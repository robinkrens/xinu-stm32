/**
 * File              : main.c
 * Author            : Robin Krens <robin@robinkrens.nl>
 * Date              : 18.11.2019
 * Last Modified Date: 18.11.2019
 * Last Modified By  : Robin Krens <robin@robinkrens.nl>
 */
/*  main.c  - main */
// This is user land

#include <xinu.h>
#include "shprototypes.h"

process	main(void)
{
	/* Run the Xinu shell */
	recvclr();
	resume(create(shell, 4096, 50, "shell", 1, 0));
	//resume(create(xsh_move, 4096, 50, "pong", 1, 0));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		//kprintf("Entering sleep");
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
    
}
