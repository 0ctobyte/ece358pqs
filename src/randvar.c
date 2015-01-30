#include <stdlib.h>
#include <math.h>
#include <time.h>

uint32_t randvar_seed() {
	uint32_t seed = time(0);
	srand(seed);
	return seed;
}

float randvar_uniform() {
	return (float)rand()/((uint32_t)RAND_MAX+1);
}

float randvar_exp(float lambda) {
	return -(log(1-randvar_uniform())/lambda);
}


