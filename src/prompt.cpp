#include "prompt.h"
#include "amdcalc/amdcalc.hpp"
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

void cli_prompt::handle_key_event(Term::Key key) {
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
    confirm = true;
    break;
  }
  case Term::Key::Backspace: {
    if (insertion_col == 0) {
      break;
    }
    (*input)->erase(--insertion_col, 1);
    std::cout << Term::cursor_left(1) << Term::clear_eol()
              << std::string_view((*input)->c_str()).substr(insertion_col);
    std::uint16_t to_move =
        saturating_sub((std::uint16_t)(*input)->length(), insertion_col);
    if (to_move != 0)
      std::cout << Term::cursor_left(to_move);
    std::cout.flush();
    break;
  }
  case Term::Key::Del: {
    if (insertion_col == (*input)->length()) {
      break;
    }
    (*input)->erase(insertion_col, 1);
    std::cout << Term::clear_eol()
              << std::string_view((*input)->c_str()).substr(insertion_col);
    std::uint16_t to_move =
        saturating_sub((std::uint16_t)(*input)->length(), insertion_col);
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
    if (insertion_col == (*input)->length()) {
      break;
    }
    insertion_col++;
    std::cout << Term::cursor_right(1);
    std::cout.flush();
    break;
  }
  default: {
    if (handle_additional_keys(key)) {
      break;
    }
    if (key.isprint()) {
      (*input)->insert(insertion_col++, 1, key.name().c_str()[0]);
      std::cout << Term::clear_eol() << (*input)->substr(insertion_col - 1);
      std::cout.flush();
    }
    break;
  }
  }
}

bool expression_prompt::handle_additional_keys(Term::Key key) {
  switch (key) {
  case Term::Key::ArrowUp: {
    std::optional<std::string *> prev = history.previous();
    if (prev.has_value()) {
      std::uint16_t to_move = (*input)->length();
      if (to_move != 0)
        std::cout << Term::cursor_left(to_move) << Term::clear_eol();
      *input = prev.value();
      std::cout << **input;
      insertion_col = (*input)->length();
      std::cout.flush();
    }
    return true;
  }
  case Term::Key::ArrowDown: {
    std::optional<std::string *> next = history.next();
    if (next.has_value()) {
      std::uint16_t to_move = (*input)->length();
      if (to_move != 0) {
        std::cout << Term::cursor_left(to_move) << Term::clear_eol();
      }
      *input = next.value();
      std::cout << **input;
      insertion_col = (*input)->length();
      std::cout.flush();
    }
    break;
    return true;
  }
  }
  return false;
}

input_response cli_prompt::get_input() {
  (*input)->clear();
  std::cout << prompt_text;
  std::cout.flush();

  while (!quit && !confirm) {
    Term::Event event = Term::read_event();
    if (event.type() == Term::Event::Type::Key) {
      Term::Key key(event);
      handle_key_event(key);
    }
    if (confirm) {
      std::cout << std::endl;
      return input_response::confirm;
    }
    if (quit) {
      std::cout << '\r' << Term::clear_eol() << "Bye" << std::endl;
      return input_response::quit;
    }
  }
  std::cout.flush();
  return input_response::quit;
}
