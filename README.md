# Program Specification
This program solves the following problem:

## Input
- Chip area width, chip area height
- Number n of rectangles
- n times: Rectangle width, rectangle height

The input file contains the numbers as 32-bit integers separated by whitespaces

## Task
Place the rectangles disjointly onto the chip area or assert that no such placement exists

## Output
- n times: lower left x coordinate, lower left y coordinate of the position of the rectangle

or

- Writes "no feasible placement exists"

The output is written to standard output. If a feasible placement exists, the output is of the form `<lower left x coordinate> <lower left y coordinate>\n` for each rectangle

# How to compile?
- run `make` in the project directory 

If your compiler does not support the C++20 standard, change `c++20` to `c++17` in the `makefile`

# How to execute?
- run `bin/main <input-file>` in the project directory
