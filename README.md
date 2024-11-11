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

#

[Quantum logic gate - Wikipedia](https://en.wikipedia.org/wiki/Quantum_logic_gate)
