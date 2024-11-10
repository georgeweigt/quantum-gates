// Find r such that 7^r mod 15 = 1
// See qiskit.org/textbook/ch-algorithms/shor.html

#define LENGTH 4096 // 12 qubits
#include "quantum-gates.c"

void U(int);

int
main(int argc, char *argv[])
{
	int i;

	psi[0] = 1.0; // ground state |000000000000>

	hadamard(0);
	hadamard(1);
	hadamard(2);
	hadamard(3);
	hadamard(4);
	hadamard(5);
	hadamard(6);
	hadamard(7);

	pauli_x(8);

	U(0);

	for (i = 0; i < 2; i++)
		U(1);

	for (i = 0; i < 4; i++)
		U(2);

	for (i = 0; i < 8; i++)
		U(3);

	for (i = 0; i < 16; i++)
		U(4);

	for (i = 0; i < 32; i++)
		U(5);

	for (i = 0; i < 64; i++)
		U(6);

	for (i = 0; i < 128; i++)
		U(7);

	ift(8); // inverse fourier transform of qubits 0 to 7

	measure(8); // measure 8 qubits
}

void
U(int i)
{
	uint32_t ctrl = 1 << i;
	cswap(10, 11, ctrl);
	cswap(9, 10, ctrl);
	cswap(8, 9, ctrl);
	cnot(8, ctrl);
	cnot(9, ctrl);
	cnot(10, ctrl);
	cnot(11, ctrl);
}
