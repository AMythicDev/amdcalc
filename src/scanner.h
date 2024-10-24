#pragma once
#include "solvetree.h"
#include <cstdint>
#include <optional>
#include <string>
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
  std::string source;

  Token parseNumber(std::string::iterator &iter);

public:
  Scanner(std::string source) : source(source) {}
  std::vector<Token> scan_tokens();
};
