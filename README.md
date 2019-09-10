# SAT-Solver-DPLL

[![Build Status](https://travis-ci.com/sukrutrao/SAT-Solver-DPLL.svg?branch=master)](https://travis-ci.com/sukrutrao/SAT-Solver-DPLL)

A SAT Solver based on the [Davis-Putnam-Logemann-Loveland (DPLL) algorithm](https://en.wikipedia.org/wiki/DPLL_algorithm).

## Installation

### Prerequisites

* g++ (or any other C++ compiler) with C++11 support
* GNU make

### Building the project

* Clone the repository
```
$ git clone https://github.com/sukrutrao/SAT-Solver-DPLL.git
$ cd SAT-Solver-DPLL
```

* Compile the program
```
$ make
```

If you use a different compiler, please edit the Makefile accordingly.

## Running the solver

The solver accepts input from standard input (STDIN) and sends output to the standard output (STDOUT).

### Input format
The input is a SAT formula is DIMACS format. A detailed description can be found [here](http://www.satcompetition.org/2009/format-benchmarks2009.html).

### Output format
* If the formula is satisfiable, the output consists of two lines. The first line of the output is a single word, `SAT`. The second line is any satisfying assignment. It consists of space separated boolean variables in ascending order, where the variables have a negative sign if assigned false and no negative sign if assigned true. The last variable is followed by a space and then a `0`.
* If the formula is unsatisfiable, the output consists of a single word, `UNSAT`.

### Running the solver
If the input is in a file input.cnf, use
```
$ ./solver < input.cnf
```

### Example
Let the input be
```
c 3 variables, 6 clauses
p cnf 3 6
1 2 0
1 -2 0
3 2 0
-3 1 0
1 2 3 0
-1 -2 0
```

The output is
```
SAT
1 -2 3 0
```

Here, the formuls is satisfiable. Variables `1` and `3` are assigned true, and variable `2` is assigned false. This is one possible satisfying assignment.

## License
This project is licensed under the [MIT License](LICENSE).

## Author
[Sukrut Rao](https://sukrutrao.github.io)

For any issues, queries, or suggestions, please [open an issue](https://github.com/sukrutrao/SAT-Solver-DPLL/issues/new).

---

This project was created as a part of the course CS6403: Constraint Solving at IIT Hyderabad.