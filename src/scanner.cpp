#include "src/scanner.h"
#include "solvetree.h"
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
  case TokenType::Exponent:
    op = "TokenType::Exponent";
    break;
  }

  if (t.value.has_value()) {
    val = t.value.value().toString();
  } else {
    val = "null";
  }

  return os << "Token(type: " << op << ", value: " << val << ")";
}

Token Scanner::parseNumber(std::string::iterator &iter) {
  char buff[16];

  bool got_decimal_point = false;
  std::uint8_t index = 0;
  do {
    if (*iter == '.') {
      if (!got_decimal_point) {
        got_decimal_point = true;
      } else {
        // TODO throw exception
      }
    }
    buff[index] = *iter;
    iter++;
    index++;
  } while (*iter == '.' || isdigit(*iter));

  buff[index] = '\0';

  Value num = Value(buff);

  return Token{.type = TokenType::Numeric, .value = std::optional(num)};
}

std::vector<Token> Scanner::scan_tokens() {
  std::vector<Token> token_list;
  std::string::iterator iter = source.begin();

  while (iter != source.end()) {
    if (isdigit(*iter) || *iter == '.') {
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
    case static_cast<int>(TokenType::Exponent):
      token_list.push_back(Token{.type = TokenType::Exponent});
      break;
    case static_cast<int>(' '):
      break;
    }
    iter++;
  }

  return token_list;
}
