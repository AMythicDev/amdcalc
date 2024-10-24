#include "src/solvetree.h"
#include <cmath>
#include <decimal>

std::optional<Value> SolverTree::solve() {
  std::optional<Value> result;
  if (root != NULL) {
    result = root->eval();
  }
  return result;
}

Value OperationNode::eval() {
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
    return Value(pow(lhs->eval().toDouble(), rhs->eval().toDouble()));
  }
}
