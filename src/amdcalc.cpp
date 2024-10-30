#include <iostream>
#include <muParser.h>

class VariableManager {
  double variable_storage[100];
  uint8_t var_count = 0;

public:
  double *add_variable(const char *var_name) {
    if (var_count > 99)
      throw mu::ParserError("Variable buffer overflow.");
    variable_storage[var_count] = 0;
    var_count++;
    return &variable_storage[var_count];
  }

  void reset() { var_count = 0; }

  static double *add_variable_to_instance(const char *var_name, void *vm_ptr) {
    VariableManager *vm = (VariableManager *)vm_ptr;
    return vm->add_variable(var_name);
  }
};

int main(int argc, char *argv[]) {
  char expression[1024];
  mu::Parser parser;
  VariableManager vm;
  parser.SetVarFactory(&VariableManager::add_variable_to_instance, &vm);
  while (true) {
    std::cout << "Calc: ";
    std::cin.getline(expression, 1024);

    parser.SetExpr(expression);
    double res;
    try {
      res = parser.Eval();
    } catch (mu::Parser::exception_type &e) {
      std::cout << '\n' << e.GetMsg() << std::endl;
      return -1;
    }
    std::cout << res << std::endl;
    vm.reset();
  }

  return 0;
}
