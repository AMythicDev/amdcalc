#include "src/parser.h"
#include "src/scanner.h"
#include "src/solvetree.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  char expression[1024];
  while (true) {
    std::cout << "Calc: ";
    std::cin.getline(expression, 1023);
    Scanner tl(expression);
    std::vector<Token> token_list = tl.scan_tokens();

    StreamingTokenParser stp = StreamingTokenParser(token_list);
    SolverTree st = stp.generate_tree();

    std::optional<Value> res = st.solve();

    std::cout << res.value() << std::endl;
  }

  return 0;
}
