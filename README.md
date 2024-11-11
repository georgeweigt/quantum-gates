The following quantum gates are implemented in quantum-gates.c
```
pauli_x
pauli_y
pauli_z
hadamard
cnot		Controlled not and Toffoli
phase		S and T gates
cphase		Controlled phase
swap
cswap		Controlled swap
ft		Fourier transform
ift		Inverse fourier transform
```

To build and run a demo
```
gcc two-bit-demo.c
./a.out
```

The result is a list of eigenstate probabilities.

```
Ground state
00 1.000000 ****************************************************************************************************
10 0.000000 
01 0.000000 
11 0.000000 
Invert bit 0
00 0.000000 
10 1.000000 ****************************************************************************************************
01 0.000000 
11 0.000000 
Invert bit 1
00 0.000000 
10 0.000000 
01 0.000000 
11 1.000000 ****************************************************************************************************
```
#

[Quantum logic gate - Wikipedia](https://en.wikipedia.org/wiki/Quantum_logic_gate)
