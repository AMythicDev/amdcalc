#include "src/parser.h"
#include "src/scanner.h"
#include "src/solvetree.h"
#include <vector>

EvaluateNode **node_to_attach(EvaluateNode **node, EvaluateNode **root,
                              OperationType op) {
  if (*node == NULL)
    return NULL;

  if ((*node)->is_numeric_node()) {
    return node_to_attach(&(*node)->parent_node, root, op);
  }

  OperationNode *node_to_check = dynamic_cast<OperationNode *>(*node);

  if (static_cast<std::uint8_t>(op) <
      static_cast<std::uint8_t>(node_to_check->op)) {
    return node_to_attach(&(*node)->parent_node, root, op);
  }

  return node;
}

void attach_to_tree(EvaluateNode *node, EvaluateNode **curr_node,
                    EvaluateNode **root, OperationType op) {
  EvaluateNode **attach_node = node_to_attach(curr_node, root, op);

  if (attach_node == NULL) {
    node->lhs = *root;
    *root = node;
    return;
  }

  EvaluateNode *prev_rhs = (*attach_node)->rhs;

  (*attach_node)->rhs = node;
  node->lhs = prev_rhs;
  node->parent_node = *attach_node;
}

SolverTree StreamingTokenParser::generate_tree() {
  std::vector<Token>::const_iterator iter = stream.begin();

  SolverTree st;

  EvaluateNode **curr_node = NULL;

  for (; iter != stream.end(); iter++) {
    switch (iter->type) {
    case TokenType::Numeric: {
      EvaluateNode *node = new NumericNode(iter->value.value());
      if (st.root == NULL) {
        st.root = node;
      } else {
        (*curr_node)->rhs = node;
        node->parent_node = *curr_node;
      }
      curr_node = &node;
      break;
    }
    case TokenType::Add: {
      EvaluateNode *node = new OperationNode(OperationType::Add);
      node->lhs = *curr_node;
      if (node->lhs == NULL) {
        node->lhs = new NumericNode(0);
      }
      attach_to_tree(node, curr_node, &st.root, OperationType::Sub);
      curr_node = &node;
      break;
    }
    case TokenType::Subtract: {
      EvaluateNode *node = new OperationNode(OperationType::Sub);
      if (node->lhs == NULL) {
        node->lhs = new NumericNode(0);
      }
      attach_to_tree(node, curr_node, &st.root, OperationType::Sub);
      curr_node = &node;
      break;
    }
    case TokenType::Multiply: {
      EvaluateNode *node = new OperationNode(OperationType::Mul);
      attach_to_tree(node, curr_node, &st.root, OperationType::Mul);
      curr_node = &node;
      break;
    }
    case TokenType::Divide: {
      EvaluateNode *node = new OperationNode(OperationType::Div);
      attach_to_tree(node, curr_node, &st.root, OperationType::Div);
      curr_node = &node;
      break;
    }
    case TokenType::Exponent: {
      EvaluateNode *node = new OperationNode(OperationType::Exp);
      attach_to_tree(node, curr_node, &st.root, OperationType::Exp);
      curr_node = &node;
      break;
    }
    default:
      break;
    }
  }

  return st;
};
