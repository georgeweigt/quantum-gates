// https://arxiv.org/abs/1703.10535

#define NUMQBITS 3

#include "quantum-gates.c"

int
main()
{
	psi[0] = 1.0; // ground state

	// init

	hadamard(0);
	hadamard(1);
	hadamard(2);

	// oracle

	cz(2, 1 << 0); // 2nd arg is bitmask
	cz(2, 1 << 1); // 2nd arg is bitmask

	// amplification (diffuser)

	hadamard(0);
	hadamard(1);
	hadamard(2);

	pauli_x(0);
	pauli_x(1);
	pauli_x(2);

	cz(2, 1 << 1 | 1 << 0); // 2nd arg is bitmask

	pauli_x(0);
	pauli_x(1);
	pauli_x(2);

	hadamard(0);
	hadamard(1);
	hadamard(2);

	measure(3);
}
