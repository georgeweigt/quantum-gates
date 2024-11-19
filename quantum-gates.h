// define NUMQBITS before including this file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#define LENGTH (1 << NUMQBITS)

double complex psi[LENGTH]; // state vector
double p[LENGTH]; // probability vector

void
init(void)
{
	psi[0] = 1.0;
}

void
xgate(int n)
{
	int i, bitmask = 1 << n;
	double complex z;
	for (i = 0; i < LENGTH; i++)
		if (i & bitmask) {
			z = psi[i ^ bitmask];
			psi[i ^ bitmask] = psi[i];
			psi[i] = z;
		}
}

void
ygate(int n)
{
	int i, bitmask = 1 << n;
	double complex z0, z1;
	for (i = 0; i < LENGTH; i++)
		if (i & bitmask) {
			z0 = psi[i ^ bitmask];
			z1 = psi[i];
			psi[i ^ bitmask] = -I * z1;
			psi[i] = I * z0;
		}
}

void
zgate(int n)
{
	int i, bitmask = 1 << n;
	for (i = 0; i < LENGTH; i++)
		if (i & bitmask)
			psi[i] *= -1.0;
}

// controlled x (multiple bits can be set in cbitmask)

void
cxgate(int n, int cbitmask)
{
	int i, bitmask = 1 << n;
	double complex z;
	for (i = 0; i < LENGTH; i++)
		if ((i & cbitmask) == cbitmask && (i & bitmask)) {
			z = psi[i ^ bitmask];
			psi[i ^ bitmask] = psi[i];
			psi[i] = z;
		}
}

// controlled y	(multiple bits can be set in cbitmask)

void
cygate(int n, int cbitmask)
{
	int i, bitmask = 1 << n;
	double complex z0, z1;
	for (i = 0; i < LENGTH; i++)
		if ((i & cbitmask) == cbitmask && (i & bitmask)) {
			z0 = psi[i ^ bitmask];
			z1 = psi[i];
			psi[i ^ bitmask] = -I * z1;
			psi[i] = I * z0;
		}
}

// controlled z (multiple bits can be set in cbitmask)

void
czgate(int n, int cbitmask)
{
	int i, bitmask = 1 << n;
	for (i = 0; i < LENGTH; i++)
		if ((i & cbitmask) == cbitmask && (i & bitmask))
			psi[i] *= -1.0;
}

void
hadamard(int n)
{
	int i, bitmask = 1 << n;
	double complex z0, z1;
	for (i = 0; i < LENGTH; i++)
		if (i & bitmask) {
			z0 = psi[i ^ bitmask];
			z1 = psi[i];
			psi[i ^ bitmask] = (z0 + z1)  / M_SQRT2;
			psi[i] = (z0 - z1) / M_SQRT2;
		}
}

// use z = I for an S gate
// use z = cexp(I * M_PI / 4) for a T gate

void
phase(double complex z, int n)
{
	int i, bitmask = 1 << n;
	for (i = 0; i < LENGTH; i++)
		if (i & bitmask)
			psi[i] *= z;
}

// controlled phase (multiple bits can be set in cbitmask)

void
cphase(double complex z, int n, int cbitmask)
{
	int i, bitmask = 1 << n;
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
	int i, mbit = 1 << m, nbit = 1 << n;
	double complex z;
	for (i = 0; i < LENGTH; i++)
		if ((i & mbit) && !(i & nbit)) {
			z = psi[i ^ mbit ^ nbit];
			psi[i ^ mbit ^ nbit] = psi[i];
			psi[i] = z;
		}
}

// controlled swap (multiple bits can be set in cbitmask)

void
cswap(int m, int n, int cbitmask)
{
	int i, mbit = 1 << m, nbit = 1 << n;
	double complex z;
	for (i = 0; i < LENGTH; i++)
		if ((i & cbitmask) == cbitmask && (i & mbit) && !(i & nbit)) {
			z = psi[i ^ mbit ^ nbit];
			psi[i ^ mbit ^ nbit] = psi[i];
			psi[i] = z;
		}
}

// fourier transform of qubits 0 to n - 1

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

// inverse fourier transform of qubits 0 to n - 1

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

// measure qubits 0 to m - 1

void
measure(int m)
{
	int b, i, j, n;
	double r, s;

	n = 1 << m; // n = 2^m

	// probabilities

	for (i = 0; i < LENGTH; i++)
		p[i] = psi[i] * conj(psi[i]);

	// sum over don't care bits

	for (i = 0; i < n; i++)
		for (j = i + n; j < LENGTH; j += n)
			p[i] += p[j];

	// histogram

	for (i = 0; i < n; i++) {
		b = 1 << m;
		while (b >>= 1)
			putchar((i & b) ? '1' : '0');
		printf(" %f ", p[i]);
		for (j = round(100.0 * p[i]); j > 0; j--)
			putchar('*');
		putchar('\n');
	}

	// transition to eigenstate

	for (i = 0; i < LENGTH; i++)
		psi[i] = 0.0;
	r = (double) rand() / RAND_MAX;
	s = 0.0;
	for (i = 0; i < n - 1; i++) {
		s += p[i];
		if (r < s)
			break;
	}
	psi[i] = 1.0;
}

// inspect state vector

void
peek(void)
{
	int b, i;
	for (i = 0; i < LENGTH; i++) {
		b = 1 << NUMQBITS;
		while (b >>= 1)
			putchar((i & b) ? '1' : '0');
		printf(" %+f%+fI\n", creal(psi[i]), cimag(psi[i]));
	}
}
