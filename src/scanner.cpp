#include "src/scanner.h"
#include <iostream>
#include <optional>
#include <string>
#include <vector>

std::ostream &operator<<(std::ostream &os, Token const &t) {
  std::string op, val;
  switch (t.type) {
  case TokenType::Add:
    op = "TokenType::Add";
    break;
  case TokenType::Subtract:
    op = "TokenType::Subtract";
    break;
  case TokenType::Numeric:
    op = "TokenType::Numeric";
    break;
  case TokenType::ParenOpen:
    op = "TokenType::ParenOpen";
    break;
  case TokenType::ParenClose:
    op = "TokenType::ParenClose";
    break;
  case TokenType::Multiply:
    op = "TokenType::Multiply";
    break;
  case TokenType::Divide:
    op = "TokenType::Divide";
    break;
  }

  if (t.value.has_value()) {
    val = std::to_string(t.value.value());
  } else {
    val = "null";
  }

  return os << "Token(type: " << op << ", value: " << val << ")";
}

Token Scanner::parseNumber(std::string::iterator &iter) {
  std::int32_t num = 0;
  int cur_digit = *iter - '0';

  if (cur_digit > 0 && cur_digit < 10) {
    num += cur_digit;
    iter++;
    cur_digit = *iter - '0';
  }

  while (cur_digit > 0 && cur_digit < 10) {
    num = num * 10 + cur_digit;
    iter++;
    cur_digit = *iter - '0';
  }

  return Token{.type = TokenType::Numeric, .value = std::optional(num)};
}

std::vector<Token> Scanner::scan_tokens() {
  std::vector<Token> token_list;
  std::string::iterator iter = source.begin();

  while (iter != source.end()) {
    if (isdigit(*iter)) {
      token_list.push_back(parseNumber(iter));
      continue;
    }
    switch (*iter) {
    case static_cast<int>(TokenType::Add):
      token_list.push_back(Token{.type = TokenType::Add});
      break;
    case static_cast<int>(TokenType::Subtract):
      token_list.push_back(Token{.type = TokenType::Subtract});
      break;
    case static_cast<int>(TokenType::Multiply):
      token_list.push_back(Token{.type = TokenType::Multiply});
      break;
    case static_cast<int>(TokenType::Divide):
      token_list.push_back(Token{.type = TokenType::Divide});
      break;
    case static_cast<int>(TokenType::ParenOpen):
      token_list.push_back(Token{.type = TokenType::ParenOpen});
      break;
    case static_cast<int>(TokenType::ParenClose):
      token_list.push_back(Token{.type = TokenType::ParenClose});
      break;
    case static_cast<int>(' '):
      break;
    }
    iter++;
  }

  return token_list;
}
