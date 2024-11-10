// https://arxiv.org/abs/2408.11606

#define LENGTH 8192 // 13 qubits (2^13 = 8192)
#include "quantum-gates.c"

// qubit names

#define X0 0
#define X1 1
#define X2 2

#define Y0 3
#define Y1 4
#define Y2 5

#define A0 6
#define A1 7

#define S0 8
#define S1 9
#define S2 10
#define S3 11

#define Q12 12

#define QUERY_MASK (1 << S0 | 1 << S1 | 1 << S2 | 1 << S3)

#define DIFFUSER_MASK (1 << X0 | 1 << X1 | 1 << X2 | 1 << Y0 | 1 << Y1 | 1 << Y2)

void Query(void);
void Diffuser(void);
void QuantumAdder(void);
void InverseQuantumAdder(void);

int
main(int argc, char *argv[])
{
	int i;

	psi[0] = 1.0; // ground state |0000000000000>

	hadamard(X0);
	hadamard(X1);
	hadamard(X2);

	hadamard(Y0);
	hadamard(Y1);
	hadamard(Y2);

	pauli_x(Q12);
	hadamard(Q12);

	for (i = 0; i < 2; i++) {
		QuantumAdder();
		Query();
		InverseQuantumAdder();
		Diffuser();
	}

	measure(6); // measure 6 qubits
}

void
Query(void)
{
	pauli_x(S0);
	pauli_x(S2);
	cnot(Q12, QUERY_MASK);
	pauli_x(S0);
	pauli_x(S2);
}

void
Diffuser(void)
{
	hadamard(X0);
	hadamard(X1);
	hadamard(X2);

	hadamard(Y0);
	hadamard(Y1);
	hadamard(Y2);

	pauli_x(X0);
	pauli_x(X1);
	pauli_x(X2);

	pauli_x(Y0);
	pauli_x(Y1);
	pauli_x(Y2);

	cnot(Q12, DIFFUSER_MASK);

	pauli_x(X0);
	pauli_x(X1);
	pauli_x(X2);

	pauli_x(Y0);
	pauli_x(Y1);
	pauli_x(Y2);

	hadamard(X0);
	hadamard(X1);
	hadamard(X2);

	hadamard(Y0);
	hadamard(Y1);
	hadamard(Y2);
}

void
QuantumAdder(void)
{
	cnot(S3, 1 << X2);
	cnot(S3, 1 << Y2);
	cnot(A0, 1 << X2 | 1 << Y2);

	cnot(S2, 1 << X1);
	cnot(S2, 1 << Y1);
	cnot(A1, 1 << X1 | 1 << Y1);
	cnot(S2, 1 << A0);
	cnot(A1, 1 << X1 | 1 << A0);
	cnot(A1, 1 << Y1 | 1 << A0);

	cnot(S1, 1 << X0);
	cnot(S1, 1 << Y0);
	cnot(S0, 1 << X0 | 1 << Y0);
	cnot(S1, 1 << A1);
	cnot(S0, 1 << X0 | 1 << A1);
	cnot(S0, 1 << Y0 | 1 << A1);
}

void
InverseQuantumAdder(void)
{
	cnot(S0, 1 << Y0 | 1 << A1);
	cnot(S0, 1 << X0 | 1 << A1);
	cnot(S1, 1 << A1);
	cnot(S0, 1 << X0 | 1 << Y0);
	cnot(S1, 1 << Y0);
	cnot(S1, 1 << X0);

	cnot(A1, 1 << Y1 | 1 << A0);
	cnot(A1, 1 << X1 | 1 << A0);
	cnot(S2, 1 << A0);
	cnot(A1, 1 << X1 | 1 << Y1);
	cnot(S2, 1 << Y1);
	cnot(S2, 1 << X1);

	cnot(A0, 1 << X2 | 1 << Y2);
	cnot(S3, 1 << Y2);
	cnot(S3, 1 << X2);
}
