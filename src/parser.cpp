#include "src/parser.h"
#include "src/scanner.h"
#include "src/solvetree.h"
#include <vector>

SolverTree StreamingTokenParser::generate_tree() {
  std::vector<Token>::iterator iter = stream.begin();

  SolverTree st;

  for (; iter == token_list.end(); iter++) {
    switch (iter->type) {
    case TokenType::Numeric:
      if (root == NULL) {
        break;
      }
    case TokenType::Add:
      if (root == NULL) {
        root = new ExpNode(*iter);
        break;
      }
      EvaluateNode *node = new ExpNode(*iter);
      node->leftChild = root;
    default:
      break;
    }
  }
};
