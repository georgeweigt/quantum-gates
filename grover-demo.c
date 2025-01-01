/* arxiv.org/abs/1703.10535

Q0 ----H----.---------H----X----.----X----H----M
            |                   |
Q1 ----H----|----.----H----X----.----X----H----M
            |    |              |
Q2 ----H----Z----Z----H----X----Z----X----H----M

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
	hadamard(Q1);
	hadamard(Q2);

	// oracle

	czgate(Q2, 1 << Q0);
	czgate(Q2, 1 << Q1);

	// amplification (diffuser)

	hadamard(Q0);
	hadamard(Q1);
	hadamard(Q2);

	xgate(Q0);
	xgate(Q1);
	xgate(Q2);

	czgate(Q2, 1 << Q1 | 1 << Q0);

	xgate(Q0);
	xgate(Q1);
	xgate(Q2);

	hadamard(Q0);
	hadamard(Q1);
	hadamard(Q2);

	measure(NUMQBITS);
}
