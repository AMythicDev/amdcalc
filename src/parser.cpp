#include "src/parser.h"
#include "src/scanner.h"
#include "src/solvetree.h"
#include <stack>
#include <vector>

EvaluateNode **node_to_attach(EvaluateNode **node, OperationType op) {
  if ((*node)->is_numeric_node()) {
    return node_to_attach(&(*node)->parent_node, op);
  }

  if ((*node)->is_placeholder())
    return NULL;

  if ((*node)->is_operation_node() && (*node)->parent_node == NULL) {
    OperationNode *node_to_check = dynamic_cast<OperationNode *>(*node);

    if (static_cast<std::uint8_t>(op) <
        static_cast<std::uint8_t>(node_to_check->op)) {
      return NULL;
    } else {
      return node;
    }
  }

  OperationNode *node_to_check = dynamic_cast<OperationNode *>(*node);

  if (static_cast<std::uint8_t>(op) <
      static_cast<std::uint8_t>(node_to_check->op)) {
    return node_to_attach(&(*node)->parent_node, op);
  }

  return node;
}

void attach_to_tree(EvaluateNode *node, EvaluateNode **curr_node,
                    EvaluateNode **root, OperationType op) {
  EvaluateNode **attach_node = node_to_attach(curr_node, op);

  if (attach_node == NULL) {
    node->lhs = *root;
    node->parent_node = (*root)->parent_node;
    node->lhs->parent_node = node;
    *root = node;
    return;
  }

  EvaluateNode *prev_rhs = (*attach_node)->rhs;

  (*attach_node)->rhs = node;
  node->lhs = prev_rhs;
  node->parent_node = *attach_node;
  prev_rhs->parent_node = node;
}

SolverTree StreamingTokenParser::generate_tree() {
  std::vector<Token>::const_iterator iter = stream.begin();

  SolverTree st;

  EvaluateNode **curr_node = &st.root;

  std::stack<EvaluateNode **> root_stack;
  root_stack.push(&st.root);

  for (; iter != stream.end(); iter++) {
    switch (iter->type) {
    case TokenType::Numeric: {
      EvaluateNode *node = new NumericNode(iter->value.value());
      if ((*curr_node)->is_placeholder()) {
        node->parent_node = (*curr_node);
        (*curr_node)->lhs = node;
      } else {
        (*curr_node)->rhs = node;
        node->parent_node = *curr_node;
      }
      curr_node = &node;
      break;
    }
    case TokenType::Add: {
      EvaluateNode *node = new OperationNode(OperationType::Add);
      attach_to_tree(node, curr_node, root_stack.top(), OperationType::Add);
      if (node->lhs == NULL) {
        node->lhs = new NumericNode(Value(0.0));
      }
      curr_node = &node;
      break;
    }
    case TokenType::Subtract: {
      EvaluateNode *node = new OperationNode(OperationType::Sub);
      attach_to_tree(node, curr_node, root_stack.top(), OperationType::Sub);
      if (node->lhs == NULL) {
        node->lhs = new NumericNode(0);
      }
      curr_node = &node;
      break;
    }
    case TokenType::Multiply: {
      EvaluateNode *node = new OperationNode(OperationType::Mul);
      attach_to_tree(node, curr_node, root_stack.top(), OperationType::Mul);
      curr_node = &node;
      break;
    }
    case TokenType::Divide: {
      EvaluateNode *node = new OperationNode(OperationType::Div);
      attach_to_tree(node, curr_node, root_stack.top(), OperationType::Div);
      curr_node = &node;
      break;
    }
    case TokenType::Exponent: {
      EvaluateNode *node = new OperationNode(OperationType::Exp);
      attach_to_tree(node, curr_node, root_stack.top(), OperationType::Exp);
      curr_node = &node;
      break;
    }
    case TokenType::ParenOpen: {
      if ((*curr_node)->is_numeric_node()) {
        curr_node = &(*curr_node)->parent_node;
      }

      EvaluateNode *node = new PlaceholderNode();
      node->parent_node = *curr_node;
      if ((*curr_node)->is_placeholder()) {
        (*curr_node)->lhs = node;
        curr_node = &(*curr_node)->lhs;
      } else {
        (*curr_node)->rhs = node;
        curr_node = &(*curr_node)->rhs;
      }
      root_stack.push(curr_node);
      break;
    }
    case TokenType::ParenClose: {
      if (root_stack.size() > 1) {
        curr_node = &(*root_stack.top())->parent_node;
        root_stack.pop();
        break;
      }
    }
    default:
      break;
    }
  }

  return st;
};
