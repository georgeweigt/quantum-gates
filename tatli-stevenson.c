// https://arxiv.org/abs/2408.11606

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <complex.h>

void pauli_x(int);
void pauli_y(int);
void pauli_z(int);
void cnot(int, uint32_t);
void hadamard(int);
void reduce(int);

void Query(void);
void Diffuser(void);
void QuantumAdder(void);
void InverseQuantumAdder(void);

#define LENGTH 8192 // 13 qubits

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

double complex psi[LENGTH]; // state vector
double p[LENGTH]; // prob.

int
main(int argc, char *argv[])
{
	int i, j, n;

	psi[0] = 1.0;

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

	reduce(64); // reduce to 64 eigenstates

	// bar chart

	printf("state  prob.\n");
	for (i = 0; i < 64; i++) {
		n = round(250.0 * p[i]);
		for (j = 0; j < 6; j++)
			if (i & 1 << j)
				printf("1");
			else
				printf("0");
		printf(" %f ", p[i]);
		for (j = 0; j < n; j++)
			printf("*");
		printf("\n");
	}
}

void
pauli_x(int n)
{
	double complex z;
	uint32_t i, bitmask = 1 << n;
	for (i = 0; i < LENGTH; i++)
		if (i & bitmask) {
			z = psi[i ^ bitmask];
			psi[i ^ bitmask] = psi[i];
			psi[i] = z;
		}
}

void
pauli_y(int n)
{
	double complex z0, z1;
	uint32_t i, bitmask = 1 << n;
	for (i = 0; i < LENGTH; i++)
		if (i & bitmask) {
			z0 = psi[i ^ bitmask];
			z1 = psi[i];
			psi[i ^ bitmask] = -I * z1;
			psi[i] = I * z0;
		}
}

void
pauli_z(int n)
{
	uint32_t i, bitmask = 1 << n;
	for (i = 0; i < LENGTH; i++)
		if (i & bitmask)
			psi[i] = -psi[i];
}

void
cnot(int n, uint32_t cbitmask)
{
	double complex z;
	uint32_t i, bitmask = 1 << n;
	for (i = 0; i < LENGTH; i++)
		if ((i & cbitmask) == cbitmask && (i & bitmask)) {
			z = psi[i ^ bitmask];
			psi[i ^ bitmask] = psi[i];
			psi[i] = z;
		}
}

void
hadamard(int n)
{
	double complex z0, z1;
	uint32_t i, bitmask = 1 << n;
	for (i = 0; i < LENGTH; i++)
		if (i & bitmask) {
			z0 = psi[i ^ bitmask];
			z1 = psi[i];
			psi[i ^ bitmask] = (z0 + z1)  / M_SQRT2;
			psi[i] = (z0 - z1) / M_SQRT2;
		}
}

void
reduce(int n)
{
	int i, j, m = LENGTH / n;
	for (i = 0; i < LENGTH; i++)
		p[i] = psi[i] * conj(psi[i]);
	for (i = 0; i < n; i++)
		for (j = 1; j < m; j++)
			p[i] += p[i + n * j];
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
