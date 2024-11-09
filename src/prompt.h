#pragma once
#include "amdcalc/amdcalc.h"
#include "cpp-terminal/key.hpp"
#include <optional>
#include <string>

enum class input_response_t {
  quit,
  confirm,
};

struct input_response {
  input_response_t type;
  std::optional<std::string *> input = std::nullopt;
};

class cli_prompt {
protected:
  std::uint16_t insertion_col = 0;
  bool quit = false;
  bool confirm = false;
  std::string **input;
  input_response ip;
  const char *prompt_text = ": ";
  void handle_key_event(Term::Key key);

public:
  cli_prompt(std::string **input) : input(input) {}
  cli_prompt(std::string **input, const char *prompt_text)
      : input(input), prompt_text(prompt_text) {}
  virtual bool handle_additional_keys(Term::Key key) { return false; }
  input_response get_input();
};

class expression_prompt : public cli_prompt {
  History &history;
  std::string *curr;

public:
  expression_prompt(std::uint16_t eval_num, History &history)
      : cli_prompt(
            &curr,
            (std::string("$") + std::to_string(eval_num) + ": ").c_str()),
        history(history), curr(history.top()) {};
  bool handle_additional_keys(Term::Key key) override;
};
