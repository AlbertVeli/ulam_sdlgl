#ifndef __PRIMES_H
#define __PRIMES_H

/* (k) ALL RIGHTS REVERSED - Reprint what you like */

#include <stdint.h>

#define NUM_PRIMES (1<<14)

extern uint8_t *primes;
extern void init_primes(void);
extern void cleanup_primes(void);

#endif /* __PRIMES_H */
