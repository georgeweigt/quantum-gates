// https://arxiv.org/abs/2408.11606

// Find X and Y such that X + Y = 5

#define NUMQBITS 13

#include "quantum-gates.h"

// qubit names (reverse bit order for X and Y)

#define X2 0
#define X1 1
#define X0 2

#define Y2 3
#define Y1 4
#define Y0 5

#define A0 6
#define A1 7

#define S0 8
#define S1 9
#define S2 10
#define S3 11

#define Q12 12

void Query(void);
void Diffuser(void);
void QuantumAdder(void);
void InverseQuantumAdder(void);

int
main()
{
	int i;

	psi[0] = 1.0; // ground state

	hadamard(X0);
	hadamard(X1);
	hadamard(X2);

	hadamard(Y0);
	hadamard(Y1);
	hadamard(Y2);

	xgate(Q12);
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
	xgate(S0);
	xgate(S2);
	cxgate(Q12, 1<<S0 | 1<<S1 | 1<<S2 | 1<<S3);
	xgate(S0);
	xgate(S2);
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

	xgate(X0);
	xgate(X1);
	xgate(X2);

	xgate(Y0);
	xgate(Y1);
	xgate(Y2);

	cxgate(Q12, 1<<X0 | 1<<X1 | 1<<X2 | 1<<Y0 | 1<<Y1 | 1<<Y2);

	xgate(X0);
	xgate(X1);
	xgate(X2);

	xgate(Y0);
	xgate(Y1);
	xgate(Y2);

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
	cxgate(S3, 1 << X2);
	cxgate(S3, 1 << Y2);
	cxgate(A0, 1 << X2 | 1 << Y2);

	cxgate(S2, 1 << X1);
	cxgate(S2, 1 << Y1);
	cxgate(A1, 1 << X1 | 1 << Y1);

	cxgate(S2, 1 << A0);
	cxgate(A1, 1 << X1 | 1 << A0);
	cxgate(A1, 1 << Y1 | 1 << A0);

	cxgate(S1, 1 << X0);
	cxgate(S1, 1 << Y0);
	cxgate(S0, 1 << X0 | 1 << Y0);

	cxgate(S1, 1 << A1);
	cxgate(S0, 1 << X0 | 1 << A1);
	cxgate(S0, 1 << Y0 | 1 << A1);
}

void
InverseQuantumAdder(void)
{
	cxgate(S0, 1 << Y0 | 1 << A1);
	cxgate(S0, 1 << X0 | 1 << A1);
	cxgate(S1, 1 << A1);

	cxgate(S0, 1 << X0 | 1 << Y0);
	cxgate(S1, 1 << Y0);
	cxgate(S1, 1 << X0);

	cxgate(A1, 1 << Y1 | 1 << A0);
	cxgate(A1, 1 << X1 | 1 << A0);
	cxgate(S2, 1 << A0);

	cxgate(A1, 1 << X1 | 1 << Y1);
	cxgate(S2, 1 << Y1);
	cxgate(S2, 1 << X1);

	cxgate(A0, 1 << X2 | 1 << Y2);
	cxgate(S3, 1 << Y2);
	cxgate(S3, 1 << X2);
}
