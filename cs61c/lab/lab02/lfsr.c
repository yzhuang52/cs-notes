#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lfsr.h"

// int next_head(uint16_t *reg) {
// 	int x = 0;
// 	x ^= ((*reg)&(1<<0));
// 	x ^= (((*reg))&(1<<2));
// 	x ^= (((*reg))&(1<<3));
// 	x ^= (((*reg))&(1<<5));
// 	return x;
// }
// void lfsr_calculate(uint16_t *reg) {
//     /* YOUR CODE HERE */
// 	int head = next_head(reg);
// 	(*reg) >>= 1;
// 	(*reg) |= (head << 15);
// }

void lfsr_calculate(uint16_t *reg) {
    /* YOUR CODE HERE */
    unsigned bit = ((*reg >> 0) ^ (*reg >> 2) ^ (*reg >> 3) ^ (*reg >> 5)) & 1u;
    *reg = (*reg >> 1) | (bit << 15);
}

