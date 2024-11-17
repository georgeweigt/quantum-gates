The following quantum gates are implemented in quantum-gates.h
```
xgate
ygate
zgate
cxgate		Controlled X and Toffoli
cygate		Controlled Y
czgate		Controlled Z
hadamard
phase		S, T, and general phase gate
cphase		Controlled phase
swap		Swap qubits
cswap		Controlled swap
ft		Fourier transform
ift		Inverse fourier transform
measure		Transition to eigenstate
```
![simple-circuit](https://github.com/user-attachments/assets/5ba71f35-ba8d-4fb9-9a56-d5a9d0937bed)

To build and run a demo
```
gcc grover-demo.c
./a.out
```
#

[Quantum logic gate - Wikipedia](https://en.wikipedia.org/wiki/Quantum_logic_gate)

[Qubit - Wikipedia](https://en.wikipedia.org/wiki/Qubit)
