#ifndef __RANDVAR_H__
#define __RANDVAR_H__

#include <stdint.h>

uint32_t randvar_seed(); 
float randvar_uniform();
float randvar_exp(float lambda);

#endif // __RANDVAR_H__
