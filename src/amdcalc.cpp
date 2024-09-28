#include "src/parser.h"
#include "src/scanner.h"
#include "src/solvetree.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  std::string expression("30+9*3+27/3");
  Scanner tl(expression);
  std::vector<Token> token_list = tl.scan_tokens();

  StreamingTokenParser stp = StreamingTokenParser(token_list);
  SolverTree st = stp.generate_tree();

  std::optional<std::int32_t> res = st.solve();

  std::cout << res.value() << std::endl;

  return 0;
}
