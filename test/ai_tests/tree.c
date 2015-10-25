/********************************************************************
 *                              tree.c                              *
 *          various tests for the AI move tree functions            *
 *                                                                  *
 *                 (C)2015 <cyberchiller@gmail.com>                 *
 *                                                                  *
 ********************************************************************/

#include <chesslib.h>
#include <chlib-cli.h> 

#define TEST_DEPTH 10

int main(void)
{
	clock_t start, stop;
	double total_time_elapsed;
	int round = WHITE;

	ch_template chb[8][8];
	initChessboard(chb);

	//playMoves(chb, &round, 9, "e2e4", "e7e5", "g1f3", "b8c6", "f1c4", "c6d4", "f3e5", "d8g5", "e5f7");
	start = clock();
	getAImove(chb, round, TEST_DEPTH);
	stop = clock();
	total_time_elapsed = ((double)(stop-start))/CLOCKS_PER_SEC;
	printf("For depth %d ply, time elapsed is %lf seconds.\n", TEST_DEPTH, total_time_elapsed);
	return 0;
}
