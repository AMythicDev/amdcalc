#include "src/parser.h"
#include "src/scanner.h"
#include "src/solvetree.h"
#include <stack>
#include <vector>

SolverTree StreamingTokenParser::generate_tree() {
  std::vector<Token>::const_iterator iter = stream.begin();

  SolverTree st;

  for (; iter != stream.end(); iter++) {
    switch (iter->type) {
    case TokenType::Numeric: {
      NumericNode *node = new NumericNode(iter->value.value());
      if (st.root == NULL) {
        st.root = node;
      } else {
        st.root->rhs = node;
      }
      break;
    }
    case TokenType::Add: {
      OperationNode *node = new OperationNode(OperationType::Add);
      node->lhs = st.root;
      st.root = node;
      break;
    }
    case TokenType::Subtract: {
      OperationNode *node = new OperationNode(OperationType::Sub);
      node->lhs = st.root;
      st.root = node;
      break;
    }
    default:
      break;
    }
  }

  return st;
};
