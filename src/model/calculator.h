#ifndef SRC_MODEL_CALCULATOR_H_
#define SRC_MODEL_CALCULATOR_H_
#include <cmath>
#include <functional>
#include <list>
#include <map>
#include <stack>
#include <stdexcept>
#include <utility>
#include <vector>

#include "tokenizer.h"

namespace s21 {
class RPNCalculator final {
 public:
  RPNCalculator()
      : kUnaryFunctions(
            {std::make_pair("#", [](double x) { return x; }),
             std::make_pair("~", [](double x) { return -x; }),
             std::make_pair("ln", [](double x) { return std::log(x); }),
             std::make_pair("tg", [](double x) { return std::tan(x); }),
             std::make_pair("sin", [](double x) { return std::sin(x); }),
             std::make_pair("cos", [](double x) { return std::cos(x); }),
             std::make_pair("tan", [](double x) { return std::tan(x); }),
             std::make_pair("ctg", [](double x) { return 1 / std::tan(x); }),
             std::make_pair("cot", [](double x) { return 1 / std::tan(x); }),
             std::make_pair("exp", [](double x) { return std::exp(x); }),
             std::make_pair("log", [](double x) { return std::log10(x); }),
             std::make_pair("atg", [](double x) { return std::atan(x); }),
             std::make_pair("asin", [](double x) { return std::asin(x); }),
             std::make_pair("acos", [](double x) { return std::acos(x); }),
             std::make_pair("atan", [](double x) { return std::atan(x); }),
             std::make_pair(
                 "acot", [](double x) { return std::acos(0) - std::atan(x); }),
             std::make_pair(
                 "actg", [](double x) { return std::acos(0) - std::atan(x); }),
             std::make_pair("sqrt", [](double x) { return std::sqrt(x); })}),
        kBinaryFunctions(
            {std::make_pair("^", [](double lhs,
                                    double rhs) { return std::pow(lhs, rhs); }),
             std::make_pair(
                 "%",
                 [](double lhs, double rhs) { return std::fmod(lhs, rhs); }),
             std::make_pair("+",
                            [](double lhs, double rhs) { return lhs + rhs; }),
             std::make_pair("-",
                            [](double lhs, double rhs) { return lhs - rhs; }),
             std::make_pair("*",
                            [](double lhs, double rhs) { return lhs * rhs; }),
             std::make_pair(
                 "/", [](double lhs, double rhs) { return lhs / rhs; })}){};

  typedef Tokenizer::TokenType TokenType;

  double Calculate(const std::list<std::string>& expr, double x = 0);

  std::vector<double> GenerateSet(double l, double r, int fin) const;

 private:
  std::map<std::string_view, std::function<double(double)>> kUnaryFunctions;
  std::map<std::string_view, std::function<double(double, double)>>
      kBinaryFunctions;

  void Operate(const std::string_view& operation);

  double CalcStackPop() {
    double val = calc_stack_.top();
    calc_stack_.pop();
    return val;
  }

  double ToDouble(std::string src, double x) const;

  TokenType current_token_;
  std::stack<double> calc_stack_;
};
}  // namespace s21

#endif  // SRC_MODEL_CALCULATOR_H_
