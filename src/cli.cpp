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
    cli_prompt prompt(solver.get_total_exp_count() + 1, history);
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

  expression_solver();

  // std::cout << "Enter number of variables: ";
  return 0;
}
