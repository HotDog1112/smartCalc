# smartCalc
Written in C language; it can build graphs, calculate complex functions, and calculate credit and deposits.

## For start:
- compiling the library and .o files with the `make s21_calc_polish.a`
- test code with the `make test`
- create html page with test results and coverage with `make gcov_report` *(need gcov)
- to install app, write `make install`, before changing installed project's path, make sure that you uninstalled it - files contain cash and default paths

## About:
- Arithmetic operations: +, -, /, *, %, unary +/-, (), ^;
- Implemented functions: sin, cos, tan, asin, acos, atan, ln, log, sqrt.
