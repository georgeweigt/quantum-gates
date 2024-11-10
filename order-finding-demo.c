// Find r such that 7^r mod 15 = 1
// See qiskit.org/textbook/ch-algorithms/shor.html

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
void reduce(int);
void ft(int);
void ift(int);

void U(int);

#define LENGTH 4096 // 12 qubits

double complex psi[LENGTH]; // state vector
double p[LENGTH]; // prob.

int
main(int argc, char *argv[])
{
	int i, j, n;

	psi[0] = 1.0;

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

	ift(8); // inverse fourier transform of qubits 0..7

	reduce(256); // reduce to 256 eigenstates

	// bar chart

	for (i = 0; i < 256; i++) {
		n = round(100.0 * p[i]);
		for (j = 0; j < 8; j++)
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

// controlled phase

void
cphase(double complex z, int n, uint32_t cbitmask)
{
	uint32_t i, bitmask = 1 << n;
	for (i = 0; i < LENGTH; i++)
		if ((i & cbitmask) == cbitmask && (i & bitmask))
			psi[i] *= z;
}

void
swap(int n, int m)
{
	double complex z;
	uint32_t i, bitmask = 1 << n | 1 << m;
	for (i = 0; i < LENGTH; i++)
		if ((i & bitmask) == bitmask) {
			z = psi[i ^ bitmask];
			psi[i ^ bitmask] = psi[i];
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
