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
void cphase(double complex, int, uint32_t);
void swap(int, int);
void cswap(int, int, uint32_t);
void ft(int);
void ift(int);
void measure(int);

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
	int i;

	psi[0] = 1.0; // ground state |0...0>

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

// multiple bits can be set in cbitmask to implement a toffoli gate

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

// controlled phase

void
cphase(double complex z, int n, uint32_t cbitmask)
{
	uint32_t i, bitmask = 1 << n;
	for (i = 0; i < LENGTH; i++)
		if ((i & cbitmask) == cbitmask && (i & bitmask))
			psi[i] *= z;
}

// swap |00> = |00>
// swap |01> = |10>
// swap |10> = |01>
// swap |11> = |11>

void
swap(int m, int n)
{
	double complex z;
	uint32_t i, mask1 = 1 << m, mask2 = 1 << n;
	for (i = 0; i < LENGTH; i++)
		if ((i & mask1) && !(i & mask2)) {
			z = psi[i ^ mask1 ^ mask2];
			psi[i ^ mask1 ^ mask2] = psi[i];
			psi[i] = z;
		}
}

// controlled swap

void
cswap(int m, int n, uint32_t cbitmask)
{
	double complex z;
	uint32_t i, mask1 = 1 << m, mask2 = 1 << n;
	for (i = 0; i < LENGTH; i++)
		if ((i & cbitmask) == cbitmask && (i & mask1) && !(i & mask2)) {
			z = psi[i ^ mask1 ^ mask2];
			psi[i ^ mask1 ^ mask2] = psi[i];
			psi[i] = z;
		}
}

// fourier transform of qubits 0 to n - 1 where n is a power of 2

void
ft(int n)
{
	int i, j;
	double complex z;
	for (i = n - 1; i >= 0; i--) {
		hadamard(i);
		for (j = 0; j < i; j++) {
			z = pow(0.5, i - j) * I * M_PI;
			z  = cexp(z);
			cphase(z, i, 1 << j);
		}
	}
	for (i = 0; i < n / 2; i++)
		swap(i, n - i - 1);
}

// inverse fourier transform of qubits 0 to n - 1 where n is a power of 2

void
ift(int n)
{
	int i, j;
	double complex z;
	for (i = 0; i < n / 2; i++)
		swap(i, n - i - 1);
	for (i = 0; i < n; i++) {
		for (j = i - 1; j >= 0; j--) {
			z = -pow(0.5, i - j) * I * M_PI;
			z = cexp(z);
			cphase(z, i, 1 << j);
		}
		hadamard(i);
	}
}

// measure m qubits

void
measure(int m)
{
	int i, j, k, n;

	n = 1 << m; // n = 2^m

	// probabilities

	for (i = 0; i < LENGTH; i++)
		p[i] = psi[i] * conj(psi[i]);

	// sum over don't care bits

	k = LENGTH / n;

	for (i = 0; i < n; i++)
		for (j = 1; j < k; j++)
			p[i] += p[i + n * j];

	// histogram

	for (i = 0; i < n; i++) {

		// print eigenstate

		for (j = 0; j < m; j++)
			if (i & 1 << j)
				printf("1");
			else
				printf("0");

		// print probability

		printf(" %f ", p[i]);

		k = round(100.0 * p[i]);

		for (j = 0; j < k; j++)
			printf("*");

		printf("\n");
	}
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
