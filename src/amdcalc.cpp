#include "amdcalc.h"
#include "muParserDef.h"
#include <cstdint>
#include <cstring>

double *VariableManager::add_variable(const char *var_name) {
  if (var_count > 99)
    throw mu::ParserError("Variable buffer overflow.");
  variable_storage[var_count] = 0;
  double *addr = &variable_storage[var_count];
  var_count++;
  return addr;
}

void VariableManager::reset() {
  memset(variable_storage, 0, MAX_VARIABLE_COUNT);
  var_count = 0;
}

double *VariableManager::add_variable_to_instance(const char *var_name,
                                                  void *vm_ptr) {
  VariableManager *vm = (VariableManager *)vm_ptr;
  return vm->add_variable(var_name);
}

ExpressionSolver::ExpressionSolver() {
  parser.SetVarFactory(&VariableManager::add_variable_to_instance, &vm);
}

void ExpressionSolver::set_expression(const std::string &exp) {
  parser.SetExpr(exp);
}

void ExpressionSolver::eval() {
  mu::value_type *v = parser.Eval((int &)eval_count);

  for (uint16_t i = 0; i < eval_count; i++) {
    eval_arr[i] = v[i];
  }
}
