#pragma once
#include <cinttypes>
#include <optional>
#include <string>
#include <vector>

enum class TokenType {
  Numeric,
  Add = '+',
  Subtract = '-',
  Multiply = '*',
  Divide = '/',
  ParenOpen = '(',
  ParenClose = ')',
};

struct Token {
  TokenType type;
  std::optional<std::int32_t> value;
};

std::ostream &operator<<(std::ostream &os, Token const &t);

class Scanner {
  std::string source;

  Token parseNumber(std::string::iterator &iter);

public:
  Scanner(std::string source) : source(source) {}
  std::vector<Token> scan_tokens();
};
