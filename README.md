The following quantum gates are implemented in quantum-gates.h

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
measure		Print a histogram of the current state and go to a basis state
```

#

Consider the following circuit.

![simple-circuit](https://github.com/user-attachments/assets/5ba71f35-ba8d-4fb9-9a56-d5a9d0937bed)

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
gcc simple-demo.c
./a.out
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
