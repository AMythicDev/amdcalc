#include "src/solvetree.h"
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
  }
}
