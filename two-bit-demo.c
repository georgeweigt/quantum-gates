// https://arxiv.org/abs/2408.11606

#define LENGTH 4 // 2 qubits (2^2 = 4)

#include "quantum-gates.c"

int
main(int argc, char *argv[])
{
	psi[0] = 1.0; // ground state |00>

	printf("Ground state\n");
	measure(2);

	printf("Invert bit 0\n");
	pauli_x(0);
	measure(2);

	printf("Invert bit 1\n");
	pauli_x(1);
	measure(2);
}
