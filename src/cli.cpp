#include "amdcalc/amdcalc.h"
#include "cpp-terminal/exception.hpp"
#include "cpp-terminal/tty.hpp"
#include "muParser.h"
#include "prompt.h"
#include <cpp-terminal/event.hpp>
#include <cpp-terminal/terminal.hpp>
#include <iomanip>
#include <iostream>
#include <string>

void expression_solver() {
  History history;
  ExpressionSolver solver;
  bool quit = false;

  while (!quit) {
    expression_prompt prompt(solver.get_total_exp_count() + 1, history);
    input_response resp = prompt.get_input();
    if (resp.type == input_response_t::quit) {
      quit = true;
      break;
    }

    std::string *expression = resp.input.value();
    solver.set_expression(*expression);
    try {
      solver.eval();
    } catch (mu::Parser::exception_type &e) {
      std::cout << "Error: " << e.GetMsg() << std::endl;
      continue;
    } catch (SpecialVariableAssignment &e) {
      std::cout << "Error: " << e.what() << std::endl;
      continue;
    }
    for (double it : solver) {
      std::cout << it << std::endl;
    }
    history.confirm_current_expression();
    history.reset();
  }
}

void equation_solver() {
  std::string buffer;
  input_response resp =
      cli_prompt::easy_input(&buffer, "Enter number of variables: ");
  if (resp == input_response::quit) {
    return;
  }
  std::uint8_t n_var = std::stoi(buffer);

  EquationSolver eqsolver(n_var);
}

int main(int argc, char *argv[]) {
  std::cout << std::setprecision(7);
  try {
    Term::terminal.setOptions(Term::Option::NoClearScreen,
                              Term::Option::NoSignalKeys, Term::Option::Cursor,
                              Term::Option::Raw);
    if (!Term::is_stdin_a_tty()) {
      throw Term::Exception(
          "The terminal is not attached to a TTY and therefore "
          "can't catch user input. Exiting...");
    }
  } catch (Term::Exception &e) {
    std::cout << e.what() << std::endl;
    return -1;
  }

  equation_solver();
  // expression_solver();

  return 0;
}
