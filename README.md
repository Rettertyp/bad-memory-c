# Bad Memory Algorithm Implementation

This repository contains an implementation of the "Bad Memory Algorithm", an algorithm by Tim Posielek. It is designed to solve the Group Assignment on Intervals problem.

It is part of the bachelor thesis "Implementing and Evaluating the Bad Memory Group Assignment Algorithm" by Marius Kaufmann at RWTH Aachen University i1, supervised by PD Dr. Walter Unger and Prof. Dr. Peter Rossmanith.

The results of the algorithm that were presented and discussed in the thesis can be found in the `results/` directory.

## Prerequisites

Before you begin, ensure you have met the following requirements:

- You have a C compiler installed. The code was tested using the MinGW-W64 version of `gcc`.
- You have installed `make`.
- You have installed `cmake`.
- You have installed the C library `json-c`.

## Running the Algorithm

To compile and run the program, clone the repository and run the following commands:

```sh
cmake
cd build/
cmake --build .
./badMemAlg <N_INTERVALS>
```

Replace `<N_INTERVALS>` with the number of intervals you want the instances that the algorithm is run on to have. The algorithm and its variants will then be run on 20 simple yes- and no-instances, on max witness instances, on max group witness instances, and on an all full instance. The results will both be printed to the console and saved to a JSON file in the `build/results/` directory.
