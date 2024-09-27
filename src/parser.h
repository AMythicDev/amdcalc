#pragma once
#include "src/scanner.h"
#include "src/solvetree.h"
#include <vector>

class StreamingTokenParser {
  const std::vector<Token> &stream;

public:
  StreamingTokenParser(const std::vector<Token> &stream) : stream(stream){};
  SolverTree generate_tree();
};
