Quantum gates are rotation matrices that preserve the unit magnitude of a state
vector.
The file `quantum-gates.h` implements rotation matrices using component
arithmetic.
See the demo programs `*-demo.c` for examples of how to include
`quantum-gates.h` and implement quantum algorithms in C.

The following quantum gates are implemented in `quantum-gates.h`

```
init		Ground state
xgate		Pauli X
ygate		Pauli Y
zgate		Pauli Z
cxgate		Controlled X and Toffoli
cygate		Controlled Y
czgate		Controlled Z
hadamard	Hadamard gate
phase		S, T, and general phase gate
cphase		Controlled phase
swap		Swap qubits
cswap		Controlled swap
ft		Fourier transform
ift		Inverse fourier transform
measure		Print a histogram of the current state and rotate to a basis state
```

Controlled gates can have any number of control bits.
Control bits are selected by setting bits in an `int`
as shown in the following example.

```
#define Q0 0
#define Q1 1
#define Q2 2

// Toffoli gate

// Q2 is target, Q1 and Q0 are control

cxgate(Q2, 1 << Q1 | 1 << Q0);
```

#

Consider the following quantum circuit.

```
Q0 ----H----.---------M
            |
Q1 ---------X----.----M
                 |
Q2 --------------X----M
```

This is the circuit in C.

```
#define NUMQBITS 3

#include "quantum-gates.h"

#define Q0 0
#define Q1 1
#define Q2 2

int
main()
{
	init();
	hadamard(Q0);
	cxgate(Q1, 1 << Q0); // Q1 is target, Q0 is control
	cxgate(Q2, 1 << Q1); // Q2 is target, Q1 is control
	measure(NUMQBITS);
}
```

To build and run

```
make
./simple-demo
```

The result is a histogram of basis state probabilities.

```
000 0.500000 **************************************************
001 0.000000 
010 0.000000 
011 0.000000 
100 0.000000 
101 0.000000 
110 0.000000 
111 0.500000 **************************************************
```

The result shows a 50% chance of getting 000 from the circuit and a 50% chance getting 111.
