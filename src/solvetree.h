#pragma once
#include <cstddef>
#include <decimal>
#include <memory>
#include <optional>

enum class OperationType {
  Sub = 0,
  Add = 1,
  Mul = 2,
  Div = 3,
  Exp = 4,
};

typedef dec::DecDouble Value;

class EvaluateNode {
public:
  std::unique_ptr<EvaluateNode> lhs = NULL;
  std::unique_ptr<EvaluateNode> rhs = NULL;
  std::unique_ptr<EvaluateNode> *parent_node = NULL;

  virtual Value eval() = 0;
  virtual bool is_numeric_node() { return false; };
  virtual bool is_placeholder() { return false; };
  virtual bool is_operation_node() { return false; };
  virtual ~EvaluateNode() = default;
};

class NumericNode : public EvaluateNode {
  Value value;

public:
  NumericNode(Value value) : value(value) {};
  Value eval() { return value; }
  bool is_numeric_node() { return true; }
};

class PlaceholderNode : public EvaluateNode {
public:
  bool is_placeholder() { return true; }

  PlaceholderNode() {};
  Value eval() { return lhs->eval(); }
};

class OperationNode : public EvaluateNode {
public:
  OperationType op;
  OperationNode(OperationType op) : op(op) {};

  Value eval();

  bool is_operation_node() { return true; };
};

class SolverTree {
public:
  std::unique_ptr<EvaluateNode> root = std::make_unique<PlaceholderNode>();

  ~SolverTree() = default;

  std::optional<Value> solve();
};
