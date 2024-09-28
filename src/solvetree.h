#pragma once
#include <cstddef>
#include <cstdint>
#include <optional>

enum class OperationType {
  Sub = 0,
  Add = 1,
  Mul = 2,
  Div = 3,
  Exp = 4,
};

class EvaluateNode {
public:
  EvaluateNode *lhs = NULL;
  EvaluateNode *rhs = NULL;
  EvaluateNode *parent_node = NULL;

  virtual std::int32_t eval() = 0;
  virtual bool is_numeric_node() = 0;
};

class NumericNode : public EvaluateNode {
  std::int32_t value;

public:
  NumericNode(std::int32_t value) : value(value){};
  std::int32_t eval() { return value; }
  bool is_numeric_node() { return true; }
};

class OperationNode : public EvaluateNode {
public:
  OperationType op;
  OperationNode(OperationType op) : op(op){};

  std::int32_t eval();
  bool is_numeric_node() { return false; }
};

class SolverTree {
public:
  EvaluateNode *root = NULL;

  std::optional<std::int32_t> solve();
};
