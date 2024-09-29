#include "src/parser.h"
#include "src/scanner.h"
#include "src/solvetree.h"
#include <stack>
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

    node->parent_node = (*root)->parent_node;
    (*root)->parent_node = node;

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

  EvaluateNode **curr_node = NULL;

  std::stack<EvaluateNode **> root_stack;

  root_stack.push(&st.root);

  EvaluateNode *subtree_ptr = NULL;

  for (; iter != stream.end(); iter++) {
    switch (iter->type) {
    case TokenType::Numeric: {
      EvaluateNode *node = new NumericNode(iter->value.value());
      if (*root_stack.top() == NULL) {
        *root_stack.top() = node;
        node->parent_node = subtree_ptr;
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
      attach_to_tree(node, curr_node, root_stack.top(), OperationType::Sub);
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
      subtree_ptr = *curr_node;
      root_stack.push(&(*curr_node)->rhs);
      curr_node = &(*curr_node)->rhs;
      continue;
    }
    case TokenType::ParenClose: {
      if (root_stack.size() > 1) {
        *curr_node = (*root_stack.top())->parent_node;
        root_stack.pop();
        break;
      }
    }
    default:
      break;
    }
    subtree_ptr = NULL;
  }

  return st;
};
