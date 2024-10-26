#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "src/parser.h"
#include "src/scanner.h"
#include "src/solvetree.h"
#include <doctest.h>

Value solve_expression(std::string exp) {
  Scanner tl(exp);
  std::vector<Token> token_list = tl.scan_tokens();

  StreamingTokenParser stp = StreamingTokenParser(token_list);
  SolverTree st = stp.generate_tree();

  std::optional<Value> res = st.solve();

  return res.value();
}

TEST_CASE("Single operation integral arithmatic") {
  CHECK(solve_expression("10+2") == Value(12));
  CHECK(solve_expression("10-2") == Value(8));
  CHECK(solve_expression("10*2") == Value(20));
  CHECK(solve_expression("10/2") == Value(5));
  CHECK(solve_expression("10^2") == Value(100));
}

TEST_CASE("Single operation floating point arithmatic") {
  CHECK(solve_expression("10+0.2").compare(Value(10.2)));
  CHECK(solve_expression("10-0.2").compare(Value(9.8)));
  CHECK(solve_expression("10*0.2").compare(Value(2)));
  CHECK(solve_expression("10/0.2").compare(Value(50)));
  CHECK(solve_expression("100^0.5").compare(Value(10)));
}

TEST_CASE("Complex arithmatic") {
  CHECK(solve_expression("5^2*4").compare(Value(100)));
  CHECK(solve_expression("5^2*4*1/2+8").compare(Value(58)));
  CHECK(solve_expression("5^2*4*1/(2+8)").compare(Value(100)));
  // CHECK(solve_expression("5^2*4*1/(2+8)*(-1)").compare(Value(-10)));
}
