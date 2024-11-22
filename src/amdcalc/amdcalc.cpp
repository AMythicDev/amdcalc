#include "amdcalc.hpp"
#include "muParserDef.h"
#include <Eigen/Dense>
#include <cstdint>
#include <cstring>
#include <optional>
#include <string>

SpecialVariableAssignment::SpecialVariableAssignment(const char *vname) {
  strcpy(var_name, vname);
  strcpy(message, "Assignment to special variable '");
  strcat(message, var_name);
  strcat(message, "'");
}
const char *SpecialVariableAssignment::what() const noexcept { return message; }

double *VariableManager::add_variable(const char *var_name) {
  if (var_count > 99)
    throw mu::ParserError("Variable buffer overflow.");
  variable_storage[var_count] = 0;
  double *addr = &variable_storage[var_count];
  var_count++;
  return addr;
}

void VariableManager::reset() {
  memset(variable_storage, 0, MAX_VARIABLE_COUNT * sizeof(double));
  var_count = 0;
}

double *VariableManager::add_variable_to_instance(const char *var_name,
                                                  void *vm_ptr) {
  if (var_name[0] == '$') {
    throw SpecialVariableAssignment(var_name);
  }
  VariableManager *vm = (VariableManager *)vm_ptr;
  return vm->add_variable(var_name);
}

ExpressionSolver::ExpressionSolver() {
  parser.DefineNameChars(
      "0123456789_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ$");
  parser.SetVarFactory(&VariableManager::add_variable_to_instance, &vm);
}

void ExpressionSolver::set_expression(const std::string &exp) {
  parser.SetExpr(exp);
}

void ExpressionSolver::eval() {
  // HACK: Due to some reason parser.Eval resets total_expr_count to 0. Until
  // this is fixed we have a workaround to copy total_expr_count and reset back
  // to it after parser.Eval is done.
  std::uint16_t total_expr_count_copy = total_expr_count;
  mu::value_type *v = parser.Eval((int &)eval_count);
  total_expr_count = total_expr_count_copy;
  for (uint16_t i = 0; i < eval_count; i++) {
    eval_arr[i] = v[i];
  }

  total_expr_count++;
  std::string var_name;
  var_name.append("$");
  var_name.append(std::to_string(total_expr_count));
  double *final_exp_var = vm.add_variable(var_name.c_str());
  *final_exp_var = eval_arr[eval_count - 1];
  parser.DefineVar(var_name, final_exp_var);
}

template <typename T> T History::wrapping_add(T num) {
  if (num == HISTORY_CAPACITY - 1)
    return 0;
  else
    return num + 1;
}

template <typename T> T History::wrapping_sub(T num) {
  if (num == 0)
    return HISTORY_CAPACITY - 1;
  else
    return num - 1;
}

std::optional<std::string *> History::next() {
  if (index == head && !covering_before_end) {
    return std::nullopt;
  }
  index = History::wrapping_add(index);
  if (index == 0) {
    covering_before_end = false;
  }
  return &list[index];
}

std::optional<std::string *> History::previous() {
  if (index == tail + 1 && covering_before_end) {
    return std::nullopt;
  }
  if (index == 0 && !is_filled) {
    return std::nullopt;
  }
  index = History::wrapping_sub(index);
  if (index == HISTORY_CAPACITY - 1) {
    covering_before_end = true;
  }
  return &list[index];
}

void History::confirm_current_expression() {
  head = History::wrapping_add(head);
  if (is_filled) {
    tail = History::wrapping_add(tail);
  }
  if (head == 0) {
    is_filled = true;
  }

  index = head;
}

EquationSolver::EquationSolver(std::uint8_t vars) {
  var_count = vars;
  switch (vars) {
  case 2:
    coffs = Eigen::Matrix2d(vars, vars);
    break;
  case 3:
    coffs = Eigen::Matrix3d(vars, vars);
    break;
  case 4:
    coffs = Eigen::Matrix4d(vars, vars);
    break;
  }
  consts = Eigen::VectorXd(vars);
}

void EquationSolver::set_coff_at_index(std::uint8_t eqn_num,
                                       std::uint8_t var_num, double value) {
  switch (var_count) {
  case 2: {
    Eigen::Matrix2d matrix = std::get<Eigen::Matrix2d>(coffs);
    matrix(eqn_num, var_num) = value;
    break;
  }
  case 3: {
    Eigen::Matrix3d matrix = std::get<Eigen::Matrix3d>(coffs);
    matrix(eqn_num, var_num) = value;
    break;
  }
  case 4: {
    Eigen::Matrix4d matrix = std::get<Eigen::Matrix4d>(coffs);
    matrix(eqn_num, var_num) = value;
    break;
  }
  default: {
    Eigen::MatrixXd cons = std::get<Eigen::MatrixXd>(coffs);
    cons(eqn_num) = value;
    break;
  }
  }
}

void EquationSolver::set_const_for_eqn(std::uint8_t eqn_num, double value) {
  switch (var_count) {
  case 2: {
    Eigen::Vector2d cons = std::get<Eigen::Vector2d>(consts);
    cons(eqn_num) = value;
    break;
  }
  case 3: {
    Eigen::Vector3d cons = std::get<Eigen::Vector3d>(consts);
    cons(eqn_num) = value;
    break;
  }
  case 4: {
    Eigen::Vector4d cons = std::get<Eigen::Vector4d>(consts);
    cons(eqn_num) = value;
    break;
  }
  default: {
    Eigen::VectorXd cons = std::get<Eigen::VectorXd>(consts);
    cons(eqn_num) = value;
    break;
  }
  }
}

Eigen::VectorXd EquationSolver::eval() {
  switch (var_count) {
  case 2: {
    Eigen::Matrix2d matrix = std::get<Eigen::Matrix2d>(coffs);
    Eigen::Vector2d cons = std::get<Eigen::Vector2d>(consts);
    return matrix.colPivHouseholderQr().solve(cons);
  }
  case 3: {
    Eigen::Matrix3d matrix = std::get<Eigen::Matrix3d>(coffs);
    Eigen::Vector3d cons = std::get<Eigen::Vector3d>(consts);
    return matrix.colPivHouseholderQr().solve(cons);
  }
  case 4: {
    Eigen::Matrix4d matrix = std::get<Eigen::Matrix4d>(coffs);
    Eigen::Vector4d cons = std::get<Eigen::Vector4d>(consts);
    return matrix.colPivHouseholderQr().solve(cons);
  }
  default: {
    Eigen::MatrixXd matrix = std::get<Eigen::MatrixXd>(coffs);
    Eigen::VectorXd cons = std::get<Eigen::VectorXd>(consts);
    return matrix.colPivHouseholderQr().solve(cons);
  }
  }
}
