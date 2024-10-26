#ifndef MERSENNE_TWISTER_H
#define MERSENNE_TWISTER_H

#include <stdint.h>

void mt_srand(uint32_t s);
uint32_t mt_read(unsigned int i);

#endif
