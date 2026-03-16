# Numerical Analysis Methods Calculator in C

This project is a comprehensive Numerical Analysis tool written in C. It allows users to perform various root-finding methods, matrix operations, numerical derivative/integral calculations, and interpolation operations via a command-line interface. 

## 🚀 Features

The application supports the following numerical methods and features:

### 🧮 Advanced Mathematical Expression Parser
* **Dynamic Input:** Users can input mathematical functions directly from the console as text (strings) instead of hardcoding them (e.g., `sin(x)+x^2-log(10,x)+e^x`).
* **Use of Shunting Yard and Postfix:** The entered mathematical equations are parsed at runtime, converted from Infix format to Postfix format, and all calculations are performed on this Postfix notation using a custom stack data structure.
* **Supported Operators:** `+`, `-`, `*`, `/`, `^` (exponentiation).
* **Supported Functions:** Trigonometric (`sin`, `cos`, `tan`, `cot`), Inverse Trigonometric (`arcsin`, `arccos`, `arctan`, `arccot`), and Logarithmic (`log`).
* **Constants and Variables:** The constants `pi`, `e`, and the variable `x` are supported.

### 🎯 Root Finding Methods
* **Bisection Method:** Searches for a root based on sign changes by continuously halving a specified interval. It presents the error rate in a tabular format for each iteration.
* **Regula-Falsi Method:** Aims to find roots successively using linear interpolation.
* **Newton-Raphson Method:** Finds roots much faster with a linear approach by taking the numerical derivative (with central differences) of the function.
  
### 📐 Linear Algebra and Matrix Operations
* **NxN Matrix Inversion:** Calculates the inverse of square matrices using the Gauss-Jordan elimination method. It checks whether the matrix is singular (determinant = 0).
* **Cholesky Decomposition:** Solves linear equation systems (`[A][X] = [C]`) by performing lower and upper (L and U) triangular matrix decomposition.
* **Gauss-Seidel Method:** An iterative approach to solving linear equation systems. The algorithm finds the largest diagonal elements (pivoting) and performs row swapping to guarantee convergence.

### 📈 Numerical Integral, Derivative, and Interpolation
* **Numerical Derivative:** Calculates the derivative for a given `x` value and step size `h` using Forward, Backward, and Central finite differences methods.
* **Simpson's Rule (Integral):** Automatically applies Simpson's 1/3 or Simpson's 3/8 rules depending on whether the entered number of segments (n) is even or odd.
* **Trapezoidal Rule (Integral):** Approximates the definite integral by calculating the area under the curve as the sum of the areas of trapezoids.
* **Gregory-Newton Interpolation:** Performs interpolation without variable transformation by creating a forward finite difference table (displayed as output) over equally spaced data points.

## 🛠️ Technology Stack

* **Language:** C
* **Standard Libraries:** `<stdio.h>`, `<stdlib.h>`, `<math.h>`, `<string.h>`, `<ctype.h>`
* **Custom Parser:** Custom stack and Postfix data structures created from scratch using advanced string processing capabilities and dynamic memory allocation (`malloc`, `realloc`, `calloc`).
