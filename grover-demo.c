// https://arxiv.org/abs/1703.10535

#define NUMQBITS 3

#include "quantum-gates.c"

#define BIT0 0
#define BIT1 1
#define BIT2 2

int
main()
{
	psi[0] = 1.0; // ground state

	// init

	hadamard(BIT0);
	hadamard(BIT1);
	hadamard(BIT2);

	// oracle

	czgate(BIT2, 1 << BIT0);
	czgate(BIT2, 1 << BIT1);

	// amplification (diffuser)

	hadamard(BIT0);
	hadamard(BIT1);
	hadamard(BIT2);

	xgate(BIT0);
	xgate(BIT1);
	xgate(BIT2);

	czgate(BIT2, 1 << BIT1 | 1 << BIT0);

	xgate(BIT0);
	xgate(BIT1);
	xgate(BIT2);

	hadamard(BIT0);
	hadamard(BIT1);
	hadamard(BIT2);

	measure(3); // measure 3 qubits
}
