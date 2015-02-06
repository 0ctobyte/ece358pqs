#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "rv.h"

uint64_t rv_seed() {
	uint64_t seed = time(0);
	srand(seed);
	return seed;
}

double rv_uniform() {
	return (double)rand()/((uint64_t)RAND_MAX+1);
}

double rv_exp(double lambda) {
	return -(log(1-rv_uniform())/lambda);
}


