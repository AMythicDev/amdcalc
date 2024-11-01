#include "amdcalc.h"
#include "cpp-terminal/cursor.hpp"
#include "cpp-terminal/exception.hpp"
#include "cpp-terminal/input.hpp"
#include "cpp-terminal/key.hpp"
#include "cpp-terminal/tty.hpp"
#include <cpp-terminal/event.hpp>
#include <cpp-terminal/terminal.hpp>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>

template <typename T> T saturating_sub(T n1, T n2) {
  if (n1 < n2) {
    return 0;
  }
  return n1 - n2;
}

struct History {
  std::string list[256];
  std::uint8_t index = 0;
  std::uint8_t entries = 0;

  void append(std::string &expression) {
    list[entries] = expression;
    index++;
    entries++;
  }
  std::optional<std::string> previous() {
    if (index == 0) {
      return std::nullopt;
    }
    return list[--index];
  }
  std::optional<std::string> next() {
    if (index == entries - 1) {
      return std::nullopt;
    }
    return list[++index];
  }
  void reset() { index = entries; }
};

void handle_key_event(Term::Key key, std::string &expression, History &history,
                      std::uint16_t &insertion_col, bool &corfirm, bool &quit) {
  switch (key) {
  case Term::Key::Ctrl_C: {
    quit = true;
    break;
  }
  case Term::Key::Ctrl_D: {
    quit = true;
    break;
  }
  case Term::Key::Enter: {
    corfirm = true;
    break;
  }
  case Term::Key::Backspace: {
    if (insertion_col == 0) {
      break;
    }
    expression.erase(--insertion_col, 1);
    std::cout << Term::cursor_left(1) << Term::clear_eol()
              << expression.substr(insertion_col);
    std::uint16_t to_move =
        saturating_sub((std::uint16_t)expression.length(), insertion_col);
    if (to_move != 0)
      std::cout << Term::cursor_left(to_move);
    std::cout.flush();
    break;
  }
  case Term::Key::Del: {
    if (insertion_col == expression.length()) {
      break;
    }
    expression.erase(insertion_col, 1);
    std::cout << Term::clear_eol() << expression.substr(insertion_col);
    std::uint16_t to_move =
        saturating_sub((std::uint16_t)expression.length(), insertion_col);
    if (to_move != 0)
      std::cout << Term::cursor_left(to_move);
    std::cout.flush();
    break;
  }
  case Term::Key::ArrowLeft: {
    if (insertion_col == 0) {
      break;
    }
    insertion_col--;
    std::cout << Term::cursor_left(1);
    std::cout.flush();
    break;
  }
  case Term::Key::ArrowRight: {
    if (insertion_col == expression.length()) {
      break;
    }
    insertion_col++;
    std::cout << Term::cursor_right(1);
    std::cout.flush();
    break;
  }
  case Term::Key::ArrowUp: {
    std::uint16_t to_move = expression.length();
    if (to_move != 0)
      std::cout << Term::cursor_left(to_move) << Term::clear_eol();
    std::optional<std::string> prev = history.previous();
    if (prev.has_value()) {
      expression = prev.value();
    }
    std::cout << expression;
    std::cout.flush();
    break;
  }
  case Term::Key::ArrowDown: {
    std::uint16_t to_move = expression.length();
    if (to_move != 0)
      std::cout << Term::cursor_left(to_move) << Term::clear_eol();
    std::optional<std::string> next = history.next();
    if (next.has_value()) {
      expression = next.value();
    }
    std::cout << expression;
    std::cout.flush();
    break;
  }
  default: {
    if (key.isprint()) {
      expression.insert(insertion_col++, 1, key.name().c_str()[0]);
      std::cout << Term::clear_eol() << expression.substr(insertion_col - 1);
      std::cout.flush();
    }
    break;
  }
  }
}

int main(int argc, char *argv[]) {
  Term::terminal.setOptions(Term::Option::NoClearScreen,
                            Term::Option::NoSignalKeys, Term::Option::Cursor,
                            Term::Option::Raw);
  if (!Term::is_stdin_a_tty()) {
    throw Term::Exception("The terminal is not attached to a TTY and therefore "
                          "can't catch user input. Exiting...");
  }

  std::string expression;
  std::uint16_t insertion_col = 0;
  std::cout << std::setprecision(7);
  ExpressionSolver solver;

  bool quit = false;
  bool confirm = false;
  History history;

  while (!quit) {
    insertion_col = 0;
    confirm = false;
    expression.clear();

    std::cout << "Calc: ";
    std::cout.flush();

    while (!quit && !confirm) {
      Term::Event event = Term::read_event();
      if (event.type() == Term::Event::Type::Key) {
        Term::Key key(event);
        handle_key_event(key, expression, history, insertion_col, confirm,
                         quit);
      }
      if (confirm) {
        history.append(expression);
        std::cout << std::endl;
      }
      if (quit) {
        std::cout << '\r' << Term::clear_eol() << "Bye" << std::endl;
        return 0;
      }
    }
    std::cout.flush();

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

  return 0;
}
