#define NUMQBITS 2

#include "quantum-gates.c"

int
main()
{
	psi[0] = 1.0; // ground state

	printf("Ground state\n");
	peek();

	printf("Invert bit 0\n");
	pauli_x(0);
	peek();

	printf("Invert bit 1\n");
	pauli_x(1);
	peek();

	printf("Fourier transform\n");
	ft(2);
	peek();

	printf("Measure\n");
	measure(2);
}
