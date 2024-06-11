[![tests](https://github.com/DainfromLiria/spreadsheet/actions/workflows/main.yml/badge.svg)](https://github.com/DainfromLiria/spreadsheet/actions/workflows/main.yml)
# Spreadsheet

### About
This is my final homework for the subject Programming and Algorithmics 2 in the academic year 2023/2024. This project is a C++ implementation of the backend of a spreadsheet application.

The application implements basic features of a spreadsheet:
* Set value in a cell, including basic formulas such as addition, subtraction, multiplication, division, negation, exponentiation, and all comparison operators.
* Get value from a user-defined cell.
* Read data from an input stream into a table.
* Save data from the table into an output stream.
* Copy a rectangular section of cells from one part of the table to another. Relative references will change.

Additionally, the program can detect cyclic dependencies and will return false if a user tries to set a value that references a cell which in turn references the setting cell.

To parse input formulas, I used a library provided with the homework task by the teacher.

### How to Run

To assemble this program, I use a Makefile. The Makefile has the following commands:
* `make` - assemble the project and create the output file (by default, the main file is the file with tests).
* `make run` - run the application (by default, it runs tests from the file `unit_tests.cpp`).
* `make clean` - remove all created `.o` files, the final compiled program file (by default `unit_tests`), and the dependencies file `Makefile.d`.

