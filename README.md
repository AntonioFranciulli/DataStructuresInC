# DataStructuresInC
Different Data Structures implemented in C with tests.

## Introduction and Objectives
These Data Stuctures were assignments for an [Algorithms and Programming](https://algoritmos-rw.github.io/algoritmos/) course that I attended to during college. Each data structure has its own set of tests that we had to implement in order to verify they worked correctly.

## Commands and Usage
In order to test each data structure, you will need to compile the source code ([GCC](https://gcc.gnu.org/) was the compiler that I personally used). If you are using GCC, compile the tests with:
```
gcc -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror -o pruebas *.c
```
In order to find out if there were any memory leaks, [Valgrind](https://valgrind.org/) was used:
```
valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas
```

Finally, to run the tests simply execute the compiled test on your command line:
```
./pruebas
```
