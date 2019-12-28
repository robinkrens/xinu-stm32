/**
 * File              : xsh_move.c
 * Author            : Robin Krens <robin@robinkrens.nl>
 * Date              : 16.11.2019
 * Last Modified Date: 18.11.2019
 * Last Modified By  : Robin Krens <robin@robinkrens.nl>
 */

#include <xinu.h>
#include <stdio.h>
#include <string.h>

/*------------------------------------------------------------------------
 * xsh_sleep  -  Shell command to delay for a specified number of seconds
 *------------------------------------------------------------------------
 */

struct ball {
	int x;
	int y;
	uint8 dir;
};

/* Ball direction enum */
enum bd {
        UR = 0,
        UL,
        DR,
        DL
};

void bc(struct ball *);
void bc2(struct ball *);
void tu(struct ball *);

int init_playfield(struct dentry *);
int pad_update(struct dentry *, struct tftshape *, int *, int *);
int ball_draw(struct dentry *, struct tftshape *, struct ball *);

shellcmd xsh_move(int nargs, char *args[])
{
	//int32	delay;			/* Delay in seconds		*/
	//char	*chptr;			/* Walks through argument	*/
	//char	ch;			/* Next character of argument	*/

	/* For argument '--help', emit help about the 'move' command	*/

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Use: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tMove in tft screen\n");
		printf("Options:\n");
		printf("\t--help\t display this help and exit\n");
		return 0;
	}

	/* Check for valid number of arguments */

	if (nargs > 1) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
				args[0]);
		return 1;
	}
	
	
	//int tmpbeginy = 0;
	//int tmpendy = 15;

	struct ball b;
	b.x = 0;
	b.y = 50;
	b.dir = UR;

	struct tftshape test;
	test.shape = 1;
	test.beginx = 0;
	test.beginy = 50;
	test.endx = 1;
	test.endy = 51;
	test.color = 0xFFFF;


	struct dentry * tftptr = &devtab[TFT0];
	init_playfield(tftptr);
	tftptr->dvwrite(tftptr, (void *) &test, sizeof(test));
	
	
	bool8 exit = FALSE;
	struct dentry * dev = &devtab[CONSOLE];
	dev->dvcntl(dev, TC_MODER, 0, 0);

	while(1) {
		tu(&b);
		ball_draw(tftptr, &test, &b);
		sleepms(20);
	}

	//while (!exit) {

	//	char c = dev->dvgetc(dev);
	//	switch(c) {
	//		case 'x':
	//			printf("Exit");
	//			exit = TRUE;
	//			break;
	//		case 'q':
	//			printf("UP PLAYER 1\n");
	//			break;
	//		case 'a':
	//			pad_update(tftptr, &test, &tmpbeginy, &tmpendy);
	//			//printf("DOWN PLAYER 1\n");
	//			break;
	//		case 'l':
	//			printf("DOWN PLAYER 2\n");
	//			break;
	//		case 'p':
	//			printf("UP PLAYER 2\n");
	//			break;
	//		default:
	//			break;
	//	}
	//}

	dev->dvcntl(dev, TC_MODEC, 0, 0);
	return 0;
}

int init_playfield(struct dentry * tftptr) {

	struct tftshape field;
	field.beginx = 0;
	field.endx = 132;
	field.beginy = 20;
	field.endy = 120;
	field.color = 0x0000;

	struct tftshape topline;
	topline.beginx = 0;
	topline.endx = 132;
	topline.beginy = 119;
	topline.endy = 120;
	topline.color = 0xFFFF;
	
	struct tftshape bottomline;
	bottomline.beginx = 0;
	bottomline.endx = 132;
	bottomline.beginy = 20;
	bottomline.endy = 21;
	bottomline.color = 0xFFFF;
	
	struct tftshape midsection;
	midsection.beginx = 66;
	midsection.endx = 67;
	midsection.beginy = 20;
	midsection.endy = 120;
	midsection.color = 0xFFFF;
	
	tftptr->dvwrite(tftptr, &field, sizeof(field));
	tftptr->dvwrite(tftptr, &topline, sizeof(field));
	tftptr->dvwrite(tftptr, &bottomline, sizeof(field));
	tftptr->dvwrite(tftptr, &midsection, sizeof(field));

	return OK;

}
void bc(struct ball * bp) {
// TODO: reset ball?
        switch(bp->dir) {
                case(UR):
                        bp->dir = UL;
                        break;
                case(UL):
                        bp->dir = UR;
                        break;
                case(DL):
                        bp->dir = DR;
                        break;
                case(DR):
                        bp->dir = DL;
                        break;
        }

}
/* Ball change direction */
void bc2(struct ball * bp) {

        switch(bp->dir) {
                case(UR):
                        bp->dir = DR;
                        break;
                case(UL):
                        bp->dir = DL;
                        break;
                case(DL):
                        bp->dir = UL;
                        break;
                case(DR):
                        bp->dir = UR;
                        break;
        }

}

void tu(struct ball * bp) {
    
    
        //bu(bp);
	
	switch(bp->dir) {
                case(UR):
                        bp->x++;
                        bp->y++;
                        break;
                case(UL):
                        bp->x--;
                        bp->y++;
                        break;
                case(DR):
                        bp->x++;
                        bp->y--;
                        break;
                case(DL):
                        bp->x--;
                        bp->y--;

        }

        if (bp->y == 120)
                bc2(bp);
        else if (bp->y == 20)
                bc2(bp);
        else if (bp->x == 0) /* && pr(p1p, bp->y)) */
                bc(bp);
        else if (bp->x == 132) /* && pr(p2p, bp->y)) */
                bc(bp); // ball change direction
        //else if (bp->x > w.x)
        //        printf("should not be here");
        //else if (bp->x < w.a)
   	//	printf("should not be here"); 
}

int ball_draw(struct dentry * tftptr, struct tftshape * test, struct ball * b) {

	//	if (b->dir == 0) {
	//		if (b->x < 132)
	//			b->x++;
	//		else {
	//			b->dir = 1;
	//			b->x--;
	//		}
	//	}

	//	else if (b->dir == 1) {
	//		if (b->x > 0)
	//			b->x--;
	//		else {
	//			b->dir = 0;
	//			b->x++;
	//		}
	//	}
	
			



		//test->endx = test->beginx
		//test->color = 0x0000;
		//tftptr->dvwrite(tftptr, (void *) test, sizeof(test));
		
		test->beginx = b->x;
		test->endx = b->x + 1;
		test->beginy = b->y;
		test->endy = b->y + 1;
		test->color = 0xFFFF;
		tftptr->dvwrite(tftptr, (void *) test, sizeof(test));

		//printf("Ball: %d, %d, dir: %d", b->x, b->y, b->dir);

		return OK;
}

int pad_update(struct dentry * tftptr, struct tftshape * test, int * tmpbeginy, int * tmpendy) {

		(*tmpbeginy)++;
		(*tmpendy)++;

		// overdraw old
		test->endy = *tmpbeginy;
		test->color = 0x0000;
		tftptr->dvwrite(tftptr, (void *) test, sizeof(test));
		
		test->beginy = *tmpbeginy;
		test->endy = *tmpendy;
		test->color = 0xFFFF;
		tftptr->dvwrite(tftptr, (void *) test, sizeof(test));

		return OK;
}
