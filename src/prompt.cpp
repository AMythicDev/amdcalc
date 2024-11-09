#include "prompt.h"
#include "amdcalc/amdcalc.h"
#include "cpp-terminal/cursor.hpp"
#include "cpp-terminal/input.hpp"
#include "cpp-terminal/key.hpp"
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

template <typename T> T saturating_sub(T n1, T n2) {
  if (n1 < n2) {
    return 0;
  }
  return n1 - n2;
}

void cli_prompt::handle_key_event(Term::Key key, std::string **expression,
                                  History &history,
                                  std::uint16_t &insertion_col, bool &corfirm,
                                  bool &quit) {
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
    (*expression)->erase(--insertion_col, 1);
    std::cout << Term::cursor_left(1) << Term::clear_eol()
              << std::string_view((*expression)->c_str()).substr(insertion_col);
    std::uint16_t to_move =
        saturating_sub((std::uint16_t)(*expression)->length(), insertion_col);
    if (to_move != 0)
      std::cout << Term::cursor_left(to_move);
    std::cout.flush();
    break;
  }
  case Term::Key::Del: {
    if (insertion_col == (*expression)->length()) {
      break;
    }
    (*expression)->erase(insertion_col, 1);
    std::cout << Term::clear_eol()
              << std::string_view((*expression)->c_str()).substr(insertion_col);
    std::uint16_t to_move =
        saturating_sub((std::uint16_t)(*expression)->length(), insertion_col);
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
    if (insertion_col == (*expression)->length()) {
      break;
    }
    insertion_col++;
    std::cout << Term::cursor_right(1);
    std::cout.flush();
    break;
  }
  case Term::Key::ArrowUp: {
    std::optional<std::string *> prev = history.previous();
    if (prev.has_value()) {
      std::uint16_t to_move = (*expression)->length();
      if (to_move != 0)
        std::cout << Term::cursor_left(to_move) << Term::clear_eol();
      *expression = prev.value();
      std::cout << **expression;
      insertion_col = (*expression)->length();
      std::cout.flush();
    }
    break;
  }
  case Term::Key::ArrowDown: {
    std::optional<std::string *> next = history.next();
    if (next.has_value()) {
      std::uint16_t to_move = (*expression)->length();
      if (to_move != 0) {
        std::cout << Term::cursor_left(to_move) << Term::clear_eol();
      }
      *expression = next.value();
      std::cout << **expression;
      insertion_col = (*expression)->length();
      std::cout.flush();
    }
    break;
  }
  default: {
    if (key.isprint()) {
      (*expression)->insert(insertion_col++, 1, key.name().c_str()[0]);
      std::cout << Term::clear_eol()
                << (*expression)->substr(insertion_col - 1);
      std::cout.flush();
    }
    break;
  }
  }
}

input_response cli_prompt::get_input() {
  std::uint16_t insertion_col = 0;
  bool quit = false;
  bool confirm = false;
  input_response ip;

  std::string *curr = history.top();
  curr->clear();
  std::string **expression = &curr;

  std::cout << '$' << eval_num << ": ";
  std::cout.flush();

  while (!quit && !confirm) {
    Term::Event event = Term::read_event();
    if (event.type() == Term::Event::Type::Key) {
      Term::Key key(event);
      handle_key_event(key, expression, history, insertion_col, confirm, quit);
    }
    if (confirm) {
      std::cout << std::endl;
      ip.type = input_response_t::confirm;
      ip.input = *expression;
      break;
    }
    if (quit) {
      std::cout << '\r' << Term::clear_eol() << "Bye" << std::endl;
      ip.type = input_response_t::quit;
      break;
    }
  }
  std::cout.flush();
  return ip;
}
