#include "src/solvetree.h"
#include <cmath>
#include <optional>

std::optional<std::int32_t> SolverTree::solve() {
  std::optional<std::int32_t> result;
  if (root != NULL) {
    result = root->eval();
  }
  return result;
}

std::int32_t OperationNode::eval() {
  lhs = new NumericNode(lhs->eval());
  rhs = new NumericNode(rhs->eval());

  switch (op) {
  case OperationType::Add:
    return lhs->eval() + rhs->eval();
  case OperationType::Sub:
    return lhs->eval() - rhs->eval();
  case OperationType::Mul:
    return lhs->eval() * rhs->eval();
  case OperationType::Div:
    return lhs->eval() / rhs->eval();
  case OperationType::Exp:
    return pow(lhs->eval(), rhs->eval());
  }
}

bool operator==(NumericNode const &n1, NumericNode const &n2) {
  return n1.value == n2.value && n1.decimal == n2.decimal;
}

NumericNode operator+(NumericNode const &n1, NumericNode const &n2) {
  uint8_t max_decimal = std::max(n1.decimal, n2.decimal);
  uint32_t x = n1.value * std::pow(10, max_decimal - n1.decimal);
  uint32_t y = n2.value * std::pow(10, max_decimal - n2.decimal);
  return NumericNode(x + y, max_decimal);
}
NumericNode operator-(NumericNode const &n1, NumericNode const &n2) {
  uint8_t max_decimal = std::max(n1.decimal, n2.decimal);
  uint32_t x = n1.value * std::pow(10, max_decimal - n1.decimal);
  uint32_t y = n2.value * std::pow(10, max_decimal - n2.decimal);
  return NumericNode(x - y, max_decimal);
}
NumericNode operator*(NumericNode const &n1, NumericNode const &n2) {
  return NumericNode(n1.value * n2.value, n1.decimal + n2.decimal);
}
