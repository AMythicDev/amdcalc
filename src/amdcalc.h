#pragma once
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <muParser.h>
#include <optional>

#define MAX_VARIABLE_COUNT 2048
#define MAX_EVALUATION_ARRAY_SIZE 256

class VariableManager {
  double variable_storage[MAX_VARIABLE_COUNT];
  uint8_t var_count = 0;

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
    return EvaluationIterator(&eval_arr[eval_count]);
  }
  uint16_t get_total_exp_count() { return total_expr_count; }
};

struct History {
  std::string list[256];
  std::uint8_t index = 0;
  std::uint8_t entries = 0;

  std::optional<std::string *> previous() {
    if (index == 0) {
      return std::nullopt;
    }
    return &list[--index];
  }
  std::optional<std::string *> next() {
    if (index >= entries) {
      return std::nullopt;
    }
    return &list[++index];
  }
  std::string *top() { return &list[entries]; }
  void confirm_current_expression() {
    entries++;
    index++;
  }
  void reset() { index = entries; }
};
