#pragma once
#include <Eigen/Dense>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <muParser.h>
#include <optional>

#define MAX_VARIABLE_COUNT 2048
#define MAX_EVALUATION_ARRAY_SIZE 256
#define HISTORY_CAPACITY 256

class VariableManager {
  double variable_storage[MAX_VARIABLE_COUNT];
  uint16_t var_count = 0;

public:
  double *add_variable(const char *var_name);
  void reset();
  static double *add_variable_to_instance(const char *var_name, void *vm_ptr);
};

class ExpressionSolver {
  mu::Parser parser;
  VariableManager vm;
  double eval_arr[MAX_EVALUATION_ARRAY_SIZE];
  uint16_t eval_count = 0;
  uint16_t total_expr_count = 0;

public:
  struct EvaluationIterator {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = double;
    using pointer = const double *;
    using reference = const double &;

    explicit EvaluationIterator(pointer ptr) : m_ptr(ptr), base(ptr) {}

    reference operator*() const { return *m_ptr; }
    pointer operator->() const { return m_ptr; }
    reference operator[](uint16_t idx) { return base[idx]; }
    EvaluationIterator &operator++() {
      m_ptr++;
      return *this;
    }
    EvaluationIterator operator++(int) {
      EvaluationIterator tmp = *this;
      ++(*this);
      return tmp;
    }
    friend bool operator==(const EvaluationIterator &a,
                           const EvaluationIterator &b) {
      return a.m_ptr == b.m_ptr;
    };
    friend bool operator!=(const EvaluationIterator &a,
                           const EvaluationIterator &b) {
      return a.m_ptr != b.m_ptr;
    };

  private:
    pointer m_ptr;
    pointer base;
  };

  ExpressionSolver();
  void set_expression(const std::string &exp);
  void eval();
  EvaluationIterator begin() const { return EvaluationIterator(&eval_arr[0]); }
  EvaluationIterator end() const {
    return EvaluationIterator(&eval_arr[eval_count - 1]);
  }
  uint16_t get_total_exp_count() { return total_expr_count; }
};

class EquationSolver {
  Eigen::MatrixXd coffs;
  Eigen::VectorXd consts;
  std::uint8_t var_count;

public:
  EquationSolver(std::uint8_t vars) {
    var_count = vars;
    coffs = Eigen::MatrixXd(vars, vars);
    consts = Eigen::VectorXd(vars);
  }

  void set_coff_at_index(std::uint8_t eqn_num, std::uint8_t var_num,
                         double value) {
    coffs(eqn_num, var_num) = value;
  }
  void set_const_for_eqn(std::uint8_t eqn_num, double value) {
    consts(eqn_num) = value;
  }

  Eigen::VectorXd eval() { return coffs.colPivHouseholderQr().solve(consts); }
};

class History {
private:
  std::string list[HISTORY_CAPACITY];
  std::uint8_t head = 0;
  std::uint8_t tail = 0;
  std::uint8_t index = 0;
  bool is_filled = false;
  bool covering_before_end = false;

  template <typename T> static T wrapping_add(T num);
  template <typename T> static T wrapping_sub(T num);

public:
  std::optional<std::string *> previous();
  std::optional<std::string *> next();
  std::string *top() { return &list[head]; }
  void confirm_current_expression();
  void reset() { index = head; }
};

class SpecialVariableAssignment : public std::exception {
private:
  char var_name[5];
  char message[256];

public:
  SpecialVariableAssignment(const char *vname);
  const char *what() const noexcept;
};
