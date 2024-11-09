#pragma once
#include "amdcalc/amdcalc.h"
#include "cpp-terminal/key.hpp"
#include <optional>

enum class input_response_t {
  quit,
  confirm,
};

struct input_response {
  input_response_t type;
  std::optional<std::string *> input = std::nullopt;
};

class cli_prompt {
  std::uint16_t eval_num;
  History &history;
  void handle_key_event(Term::Key key, std::string **expression,
                        History &history, std::uint16_t &insertion_col,
                        bool &corfirm, bool &quit);

public:
  cli_prompt(std::uint16_t eval_num, History &history)
      : eval_num(eval_num), history(history) {};
  cli_prompt(History &history) : eval_num(1), history(history) {};
  input_response get_input();
};
