#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "src/parser.h"
#include "src/scanner.h"
#include "src/solvetree.h"
#include <string>

std::optional<std::int32_t> compute(std::string &expression) {
  Scanner tl(expression);
  std::vector<Token> token_list = tl.scan_tokens();

  StreamingTokenParser stp = StreamingTokenParser(token_list);
  SolverTree st = stp.generate_tree();

  std::optional<std::int32_t> res = st.solve();
  return res;
}

TEST_CASE("parse and solve") {
  std::string exp("30+9*3+27/(3-2)*3^2");
  CHECK(compute(exp) == std::optional(300));
}

TEST_CASE("integral arithmetic on NumericNode") {
  CHECK((NumericNode(3) + NumericNode(5)) == NumericNode(8));
  CHECK((NumericNode(3) - NumericNode(5)) == NumericNode(-2));
  CHECK((NumericNode(3) * NumericNode(5)) == NumericNode(15));
  CHECK((NumericNode(30) / NumericNode(5)) == NumericNode(6));
}

TEST_CASE("decimal arithmetic on NumericNode") {
  SUBCASE("Add") {
    CHECK(NumericNode(3, 1) + NumericNode(5, 1) == NumericNode(8, 1));
    CHECK(NumericNode(3, 1) + NumericNode(5, 2) == NumericNode(35, 2));
  }
  SUBCASE("Subtract") {
    CHECK(NumericNode(3, 1) - NumericNode(5, 1) == NumericNode(-2, 1));
    CHECK(NumericNode(3, 1) - NumericNode(5, 2) == NumericNode(25, 2));
  }
  SUBCASE("Multiply") {
    CHECK(NumericNode(3, 1) * NumericNode(5, 1) == NumericNode(15, 2));
    CHECK(NumericNode(3, 1) * NumericNode(5, 2) == NumericNode(15, 3));
  }
  // CHECK((NumericNode(3) - NumericNode(5)) == NumericNode(-2));
  // CHECK((NumericNode(3) * NumericNode(5)) == NumericNode(15));
  // CHECK((NumericNode(30) / NumericNode(5)) == NumericNode(6));
}
