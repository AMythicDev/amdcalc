#pragma once
#include "src/scanner.h"
#include <cstddef>
#include <iostream>
#include <vector>

enum class OperationType {
  Add,
  Sub,
  Mul,
  Div,
};

class EvaluateNode {
public:
  virtual std::int32_t eval() = 0;
  EvaluateNode *lhs = NULL;
  EvaluateNode *rhs = NULL;
};

class NumericNode : public EvaluateNode {
  std::int32_t value;

public:
  NumericNode(std::int32_t value) : value(value){};
  std::int32_t eval() { return value; }
};

class OperationNode : public EvaluateNode {
  OperationType op;

public:
  OperationNode(OperationType op) : op(op){};
  std::int32_t eval();
};

class SolverTree {
public:
  OperationNode *root = NULL;

  std::optional<std::int32_t> solve();
};
