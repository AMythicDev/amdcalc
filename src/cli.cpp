#include "amdcalc.h"
#include <iomanip>
#include <iostream>

int main(int argc, char *argv[]) {
  char expression[1024];
  std::cout << std::setprecision(7);
  ExpressionSolver solver;
  while (true) {
    std::cout << "Calc: ";
    std::cin.getline(expression, 1024);
    solver.set_expression(expression);
    try {
      solver.eval();
    } catch (mu::Parser::exception_type &e) {
      std::cout << '\n' << e.GetMsg() << std::endl;
      return -1;
    }
    for (double it : solver) {
      std::cout << it << std::endl;
    }
  }
}
