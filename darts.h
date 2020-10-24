#ifndef DARTS_H
#define DARTS_H

#include <stdlib.h>

unsigned long long int throw_darts(int darts_per_drone, unsigned long long int* shm_p);
int throw_one_dart();

#endif
