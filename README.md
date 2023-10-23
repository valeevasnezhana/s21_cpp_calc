# s21_cpp_calc
<h2>SmartCalc v2.0</h2>

Quick run: in directory src "make run"

- To install the calculator, type `make install` in the terminal.
- To uninstall the calculator, type `make uninstall`.
- To open the calculator, type `make open` in the terminal after the installation.
- To archive the program, type `make dist`.


<h3>Interface:</h3>

- To calculate an expression, enter the expression in the top line and press `=`,
- you can also enter the value `x` and evaluate the expression containing "x" with value substitution

- The calculator can perform basic arithmetic operations (addition, subtraction, multiplication, division), trigonometric operations (sine, cosine, tangent, arcsine, arccosine, arctangent), as well as exponentiation, division fraction, square root and calculate natural and decimal logarithms.
- You can enter up to 255 characters on the top line.
- At the input, you can submit both the target programs of the number, and the identified numbers, metabolic levels through the dot.
- It is possible to display the graph of the function. To plot a function, the display domains and ranges of values must be included. The domain of definition and the range of values of functions by default are limited to the values of numbers from -10 to 10.



<h3>Supported Functions and Operators:</h3>



| Operator name   | Infix Form |
|-----------------|------------|
| Brackets        | (a + b)    |
| Addition        | a + b      |
| Subtraction     | a - b      |
| Multiplication* | a * b      |
| Division        | a / b      |
| Power           | a ^ b      |
| Modulus         | a mod b    |
| Unary plus      | +a         |
| Unary minus     | -a         |



| Function description       | Function |
|----------------------------|----------|
| Computes cosine            | cos(x)   |
| Computes sine              | sin(x)   |
| Computes tangent           | tan(x)   |
| Computes arc cosine        | acos(x)  |
| Computes arc sine          | asin(x)  |
| Computes arc tangent       | atan(x)  |
| Computes square root       | sqrt(x)  |
| Computes natural logarithm | ln(x)    |
| Computes common logarithm  | log(x)   |
