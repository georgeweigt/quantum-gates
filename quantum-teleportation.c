#define NUMQBITS 3

#include "quantum-gates.h"

#define Q0 0
#define Q1 1
#define Q2 2

int
main()
{
	init();

	// xgate(Q0);

	hadamard(Q1);
	cxgate(Q2, 1 << Q1);

	cxgate(Q1, 1 << Q0);
	hadamard(Q0);

	measure(3);

	if (psi[2] == 1.0 || psi[6] == 1.0) {
		printf("X\n");
		xgate(Q2);
	} else if (psi[1] == 1.0 || psi[5] == 1.0) {
		printf("Z\n");
		zgate(Q2);
	} else if (psi[3] == 1.0 || psi[7] == 1.0) {
		printf("XZ\n");
		xgate(Q2);
		zgate(Q2);
	} else
		printf("\n");

	peek();
	printf("\n");

	swap(Q0, Q2); // swap to measure
	measure(1);
}
