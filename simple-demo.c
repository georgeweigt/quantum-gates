/*

Q0 ----H----.---------M
            |
Q1 ---------X----.----M
                 |
Q2 --------------X----M

*/

#define NUMQBITS 3

#include "quantum-gates.h"

#define Q0 0
#define Q1 1
#define Q2 2

int
main()
{
	init();
	hadamard(Q0);
	cxgate(Q1, 1 << Q0);
	cxgate(Q2, 1 << Q1);
	measure(NUMQBITS);
}
