/*

Q0 ----H----.---------.----M
            |         |
Q1 ----H----|----.----.----M
            |    |    |
Q2 ---------X----X----|----M    sum (xor) of Q0 and Q1
                      |
Q3 -------------------X----M    carry out bit

*/

#define NUMQBITS 4

#include "quantum-gates.h"

#define Q0 0
#define Q1 1
#define Q2 2
#define Q3 3

int
main()
{
	init();
	hadamard(Q0);
	hadamard(Q1);
	cxgate(Q2, 1 << Q0);
	cxgate(Q2, 1 << Q1);
	cxgate(Q3, 1 << Q1 | 1 << Q0);
	measure(NUMQBITS);
}
