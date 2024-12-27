/* Q0 and Q1 are local, Q2 is remote after entanglement

Q0 --------------.----H----M
                 |         |
Q1 ----H----.----X----M    |
            |         |    |
Q2 ---------X---------X----Z----

            ^
            Q1 and Q2 maximally entangled (bell state)
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

	// Q0 is the qubit to be teleported (default is Q0 = 0)

	xgate(Q0); // make Q0 = 1

	hadamard(Q1);
	cxgate(Q2, 1 << Q1);

	cxgate(Q1, 1 << Q0);
	hadamard(Q0);

	measure(3);

	switch (newstate & (1 << Q1 | 1 << Q0)) {

	case 1 << Q0:
		printf("Z\n");
		zgate(Q2);
		break;

	case 1 << Q1:
		printf("X\n");
		xgate(Q2);
		break;

	case 1 << Q1 | 1 << Q0:
		printf("XZ\n");
		xgate(Q2);
		zgate(Q2);
		break;

	default:
		printf("\n");
		break;
	}

	measure(3);

	if (newstate & (1 << Q2))
		printf("Q2: 1\n");
	else
		printf("Q2: 0\n");
}
