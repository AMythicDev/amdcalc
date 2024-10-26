#include "src/parser.h"
#include "src/scanner.h"
#include "src/solvetree.h"
#include <memory>
#include <stack>
#include <vector>

std::unique_ptr<EvaluateNode> *
node_to_attach(std::unique_ptr<EvaluateNode> *node, OperationType op) {
  if ((*node)->is_numeric_node()) {
    return node_to_attach((*node)->parent_node, op);
  }

  if ((*node)->is_placeholder())
    return NULL;

  if ((*node)->is_operation_node() && (*node)->parent_node == NULL) {
    OperationNode *node_to_check = dynamic_cast<OperationNode *>(node->get());

    if (static_cast<std::uint8_t>(op) <
        static_cast<std::uint8_t>(node_to_check->op)) {
      return NULL;
    } else {
      return node;
    }
  }

  OperationNode *node_to_check = dynamic_cast<OperationNode *>(node->get());

  if (static_cast<std::uint8_t>(op) <
      static_cast<std::uint8_t>(node_to_check->op)) {
    return node_to_attach((*node)->parent_node, op);
  }

  return node;
}

void attach_to_tree(std::unique_ptr<EvaluateNode> node,
                    std::unique_ptr<EvaluateNode> **curr_node,
                    std::unique_ptr<EvaluateNode> *root, OperationType op) {
  std::unique_ptr<EvaluateNode> *attach_node = node_to_attach(*curr_node, op);

  if (attach_node == NULL) {
    (node)->parent_node = (*root)->parent_node;
    (node)->lhs = std::move(*root);
    (node)->lhs->parent_node = &node;
    *root = std::move(node);
    *curr_node = root;
    return;
  }

  (node)->parent_node = attach_node;
  (node)->lhs = std::move((*attach_node)->rhs);
  (*attach_node)->rhs = std::move(node);
  (*attach_node)->rhs->lhs->parent_node = &(*attach_node)->rhs;
  *curr_node = &(*attach_node)->rhs;
}

SolverTree StreamingTokenParser::generate_tree() {
  std::vector<Token>::const_iterator iter = stream.begin();

  SolverTree st;

  std::unique_ptr<EvaluateNode> *curr_node = &st.root;

  std::stack<std::unique_ptr<EvaluateNode> *> root_stack;
  root_stack.push(&st.root);

  for (; iter != stream.end(); iter++) {
    switch (iter->type) {
    case TokenType::Numeric: {
      std::unique_ptr<EvaluateNode> node =
          std::make_unique<NumericNode>(iter->value.value());
      if ((*curr_node)->is_placeholder()) {
        node->parent_node = curr_node;
        (*curr_node)->lhs = std::move(node);
        curr_node = &(*curr_node)->lhs;
      } else {
        node->parent_node = curr_node;
        (*curr_node)->rhs = std::move(node);
        curr_node = &(*curr_node)->rhs;
      }
      break;
    }
    case TokenType::Add: {
      std::unique_ptr<EvaluateNode> node =
          std::make_unique<OperationNode>(OperationType::Add);
      attach_to_tree(std::move(node), &curr_node, root_stack.top(),
                     OperationType::Add);
      if ((*curr_node)->lhs == NULL) {
        (*curr_node)->lhs = std::make_unique<NumericNode>(Value(0.0));
      }
      break;
    }
    case TokenType::Subtract: {
      std::unique_ptr<EvaluateNode> node =
          std::make_unique<OperationNode>(OperationType::Sub);
      attach_to_tree(std::move(node), &curr_node, root_stack.top(),
                     OperationType::Sub);
      if ((*curr_node)->lhs == NULL) {
        (*curr_node)->lhs = std::make_unique<NumericNode>(Value(0.0));
      }
      break;
    }
    case TokenType::Multiply: {
      std::unique_ptr<EvaluateNode> node =
          std::make_unique<OperationNode>(OperationType::Mul);
      attach_to_tree(std::move(node), &curr_node, root_stack.top(),
                     OperationType::Mul);
      break;
    }
    case TokenType::Divide: {
      std::unique_ptr<EvaluateNode> node =
          std::make_unique<OperationNode>(OperationType::Div);
      attach_to_tree(std::move(node), &curr_node, root_stack.top(),
                     OperationType::Div);
      break;
    }
    case TokenType::Exponent: {
      std::unique_ptr<EvaluateNode> node =
          std::make_unique<OperationNode>(OperationType::Exp);
      attach_to_tree(std::move(node), &curr_node, root_stack.top(),
                     OperationType::Exp);
      break;
    }
    case TokenType::ParenOpen: {
      if ((*curr_node)->is_numeric_node()) {
        curr_node = (*curr_node)->parent_node;
      }

      std::unique_ptr<EvaluateNode> node = std::make_unique<PlaceholderNode>();
      node->parent_node = curr_node;
      if ((*curr_node)->is_placeholder()) {
        (*curr_node)->lhs = std::move(node);
        curr_node = &(*curr_node)->lhs;
      } else {
        (*curr_node)->rhs = std::move(node);
        curr_node = &(*curr_node)->rhs;
      }
      root_stack.push(curr_node);
      break;
    }
    case TokenType::ParenClose: {
      if (root_stack.size() > 1) {
        curr_node = (*root_stack.top())->parent_node;
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
