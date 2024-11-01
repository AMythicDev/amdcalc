#pragma once
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <muParser.h>

class VariableManager {
  const static uint16_t MAX_VARIABLE_COUNT = 256;
  double variable_storage[MAX_VARIABLE_COUNT];
  uint8_t var_count = 0;

public:
  double *add_variable(const char *var_name);
  void reset();
  static double *add_variable_to_instance(const char *var_name, void *vm_ptr);
};

class ExpressionSolver {
  const static uint16_t MAX_EVALUATION_ARRAY_SIZE = 256;
  mu::Parser parser;
  VariableManager vm;
  double eval_arr[MAX_EVALUATION_ARRAY_SIZE];
  uint16_t eval_count = 0;

public:
  struct EvaluationIterator {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = double;
    using pointer = double *;
    using reference = double &;

    EvaluationIterator(pointer ptr) : m_ptr(ptr) {}

    reference operator*() const { return *m_ptr; }
    pointer operator->() { return m_ptr; }
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
  };

  ExpressionSolver();
  void set_expression(const std::string &exp);
  void eval();
  EvaluationIterator begin() { return EvaluationIterator(&eval_arr[0]); }
  EvaluationIterator end() { return EvaluationIterator(&eval_arr[eval_count]); }
};
