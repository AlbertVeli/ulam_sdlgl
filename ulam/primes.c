/* (k) ALL RIGHTS REVERSED - Reprint what you like
 *
 * Little routine to generate primes.
 *
 *
 * Prickle-Prickle, the 73rd day of Confusion in the YOLD 3179
 *
 * Albert Veli
 */

#include <stdio.h>
#include <stdlib.h>

#include "primes.h"

static uint32_t curr_prime_ix = 3;
uint8_t *primes = NULL;


/* Simple prime number "sieve" */
static int is_prime(uint32_t num)
{
   uint32_t limit;
   uint32_t i;
   uint8_t *p;

   /* Quickly sort out even numbers */
   if ((num & 1) == 0) {
      return 0;
   }

   /* Check if divisible by primes we have so far.
    *
    * If num is composite, one of the composites muste be less than
    * square root of num.  Only need to loop up to square root.
    *
    * For now, just loop to half (which is always bigger than square
    * root of num) so we don't have to do the expensive square root
    * calculation. This could be optimized so square root is
    * calculated if num is bigger than some limit.
    */
   limit = (num >> 1) + 1;
   for (i = 3, p = &primes[3]; i < limit; i += 2, p += 2) {
      if ((*p) && ((num % i) == 0)) {
         /* Not a prime */
         return 0;
      }
   }

   /* This must be a prime */
   return 1;
}


void cleanup_primes(void)
{
if (primes) {
  free(primes);
  primes = NULL;
 }
}


/* Set each primes[i] to:
 * 1 if i is prime
 * 0 if i is composite
 */
void init_primes(void)
{
   uint8_t *p;

   if (primes) {
      cleanup_primes();
   }
   primes = malloc(NUM_PRIMES);
   if (!primes) {
      fprintf(stderr, "Could not init primes\n");
      exit(EXIT_FAILURE);
   }

   /* Start at 3 */
   primes[0] = 0; /* Invalid, just set to 0 */
   primes[1] = 1;
   primes[2] = 1;

   curr_prime_ix = 3;
   p = &primes[curr_prime_ix];

   /* Fill in prime/composite array */
   for (; curr_prime_ix < NUM_PRIMES; curr_prime_ix++) {
      if (is_prime(curr_prime_ix)) {
         *p = 1;
      } else {
         *p = 0;
      }
      p++;
   }
}


/* Count number of composites (recursively).
 * Slower than just checking if prime or not.
 */
static int get_num_composites(uint32_t num)
{
   uint32_t limit;
   uint32_t i;
   uint8_t *p;

   /* Stop recursion early */
   if (num < 4) {
      return 0;
   }

   /* Check if even */
   if ((num & 1) == 0) {
      return get_num_composites(num >> 1) + 1;
   }

   /* Check if divisible by primes we have so far. */
   limit = (num >> 1) + 1;

   for (i = 3, p = &primes[3]; i < limit; i += 2, p += 2) {
      if ((*p == 0) && ((num % i) == 0)) {
         /* Not a prime */
         return get_num_composites(num / i) + 1;
      }
   }

   /* Is prime */
   return 0;
}


/* Set each primes[i] to:
 * 0 if i is prime
 * 1-255 number of composites. Clamp to 255.
 * Note. This is slower than init_primes() above.
 */
void init_prime_composites(void)
{
   uint8_t *p;
   uint32_t composites;

   if (primes) {
      cleanup_primes();
   }
   primes = malloc(NUM_PRIMES);
   if (!primes) {
      fprintf(stderr, "Could not init primes\n");
      exit(EXIT_FAILURE);
   }

   /* Start at 3 */
   primes[0] = 0; /* Invalid, just set to 0 */
   primes[1] = 0;
   primes[2] = 0;

   curr_prime_ix = 3;
   p = &primes[curr_prime_ix];

   /* Fill in prime/composite array */
   for (; curr_prime_ix < NUM_PRIMES; curr_prime_ix++) {
      composites = get_num_composites(curr_prime_ix);
      if (composites > 255) {
         composites = 255;
      }
      *p++ = composites;
   }
}


/**
 * Local Variables:
 * mode: c
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */
