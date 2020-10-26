#include <stdio.h>
#include "darts.h"

unsigned long long int throw_darts(int darts_per_drone){/*{{{*/
	unsigned long long int hits = 0;
	for(int i=0; i< darts_per_drone; i++){
		hits+=throw_one_dart();
	}
	return hits;
}/*}}}*/

int throw_one_dart(){/*{{{*/
	int random_int;
	float x;
	float y;
	float r_squared;

	int ret;

	random_int = rand();
	x = ((( (float)random_int/RAND_MAX) *2) -1);

	random_int = rand();
	y = ((( (float)random_int/RAND_MAX) *2) -1);

	r_squared = x*x + y*y;
	
	ret = (r_squared <= 1.0);
	return ret;
}/*}}}*/
