// https://github.com/Qiskit/textbook/blob/main/notebooks/ch-algorithms/simon.ipynb

#define NUMQBITS 6

#include "quantum-gates.c"

#define Q0 0
#define Q1 1
#define Q2 2
#define Q3 3
#define Q4 4
#define Q5 5

int
main()
{
	psi[0] = 1.0; // ground state

	hadamard(Q0);
	hadamard(Q1);
	hadamard(Q2);

	cxgate(Q3, 1 << Q0);
	cxgate(Q4, 1 << Q1);
	cxgate(Q5, 1 << Q2);
	cxgate(Q4, 1 << Q1);
	cxgate(Q5, 1 << Q1);

	hadamard(Q0);
	hadamard(Q1);
	hadamard(Q2);

	measure(3); // measure 3 qubits
}
