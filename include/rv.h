#ifndef __RV_H__
#define __RV_H__

#include <stdint.h>

uint64_t rv_seed(); 
double rv_uniform();
double rv_exp(double lambda);

#endif // __RV_H__
