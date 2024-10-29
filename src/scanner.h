#pragma once
#include "solvetree.h"
#include <optional>
#include <vector>

enum class TokenType {
  Numeric,
  Add = '+',
  Subtract = '-',
  Multiply = '*',
  Divide = '/',
  Exponent = '^',
  ParenOpen = '(',
  ParenClose = ')',
};

struct Token {
  TokenType type;
  std::optional<Value> value;
};

std::ostream &operator<<(std::ostream &os, Token const &t);

class Scanner {
  const std::string_view source;

  Token parseNumber(std::string_view::const_iterator &iter);

public:
  Scanner(const std::string source) : source(source) {}
  std::vector<Token> scan_tokens();
};
