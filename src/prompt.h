#pragma once
#include "amdcalc/amdcalc.hpp"
#include "cpp-terminal/key.hpp"
#include <string>
#include <string_view>

enum class input_response {
  quit,
  confirm,
};

class cli_prompt {
protected:
  std::uint16_t insertion_col = 0;
  bool quit = false;
  bool confirm = false;
  std::string **input;
  input_response ip;
  std::string_view prompt_text = ": ";
  void handle_key_event(Term::Key key);

public:
  cli_prompt(std::string **input) : input(input) {}
  cli_prompt(std::string **input, std::string_view prompt_text)
      : input(input), prompt_text(prompt_text) {}
  static input_response easy_input(std::string *buffer) {
    cli_prompt prompt(&buffer);
    return prompt.get_input();
  }
  static input_response easy_input(std::string *buffer,
                                   const char *prompt_text) {
    cli_prompt prompt(&buffer, prompt_text);
    return prompt.get_input();
  }
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
  expression_prompt(std::string_view prompt, History &history)
      : cli_prompt(&curr, prompt), history(history), curr(history.top()) {};

  bool handle_additional_keys(Term::Key key) override;
};
