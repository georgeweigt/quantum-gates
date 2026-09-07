#include <complex.h>

// n is number of qbits

extern void init(int n);

// n is the qubit number from 0 to NUMQBITS - 1

extern void xgate(int n);

// n is the qubit number from 0 to NUMQBITS - 1

extern void ygate(int n);

// n is the qubit number from 0 to NUMQBITS - 1

extern void zgate(int n);

// controlled x (multiple control bits can be set in cbitmask)

extern void cxgate(int n, int cbitmask);

// controlled y	(multiple control bits can be set in cbitmask)

extern void cygate(int n, int cbitmask);

// controlled z (multiple control bits can be set in cbitmask)

extern void czgate(int n, int cbitmask);

extern void hadamard(int n);

// use z = I for an S gate
// use z = cexp(I * M_PI / 4) for a T gate

extern void phase(double complex z, int n);

// controlled phase (multiple control bits can be set in cbitmask)

extern void cphase(double complex z, int n, int cbitmask);

// swap |00> = |00>
// swap |01> = |10>
// swap |10> = |01>
// swap |11> = |11>

extern void swap(int m, int n);

// controlled swap (multiple control bits can be set in cbitmask)

extern void cswap(int m, int n, int cbitmask);

// fourier transform of qubits 0 to n - 1

extern void ft(int n);

// inverse fourier transform of qubits 0 to n - 1

extern void ift(int n);

// measure qubits 0 to m - 1

extern void measure(int m);

// inspect state vector

extern void peek(void);
