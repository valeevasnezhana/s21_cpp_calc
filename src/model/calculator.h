// Copyright 2023 School21 @gruntmet Snezhana Valeeva
#ifndef CPP3_SMARTCALC_V2_SRC_MODEL_CALCULATOR_H_
#define CPP3_SMARTCALC_V2_SRC_MODEL_CALCULATOR_H_
#include <cmath>
#include <functional>
#include <list>
#include <map>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "tokenizer.h"

/*!
\file calculator.h
 \brief Implements a Reverse Polish Notation (RPN) calculator.
\namespace s21
*/

namespace s21 {
class RPNCalculator final {
 public:
  /*!

\class RPNCalculator
\brief RPN calculator handling expressions with unary and binary functions.
*/

  RPNCalculator()
      /*!

    \fn RPNCalculator::RPNCalculator
    \brief Constructs an RPNCalculator with predefined unary functions.
    Initializes the RPNCalculator instance with a set of predefined unary
    functions, including identity, negation, natural and common logarithm, and
    trigonometry functions, binary operations including power, mul, div, sum,
    sub and mod.
    */
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
                 "/", [](double lhs, double rhs) { return lhs / rhs; })}) {}

  using TokenType = Tokenizer::TokenType;

  /*!

\fn double RPNCalculator::Calculate
\brief Evaluates the given RPN expression.
\param expr Expression list in Reverse Polish Notation.
\param x Optional variable value (default is 0).
\return The result of the expression evaluation.
*/

  double Calculate(const std::list<std::string>& expr, double x = 0);

  /*!

\fn std::vector<double> RPNCalculator::GenerateSet
\brief Generates a set of equally spaced points between l and r.
\param l Lower bound of the range.
\param r Upper bound of the range.
\param fin Number of points in the range.
\return A vector of equally spaced points in the range.
*/

  std::vector<double> GenerateSet(double l, double r, int fin) const;

 private:
  /*!

\var RPNCalculator::kUnaryFunctions
\brief Maps supported unary functions to their corresponding operations.
*/
  std::map<std::string_view, std::function<double(double)>> kUnaryFunctions;
  /*!

\var RPNCalculator::kBinaryFunctions
\brief Maps supported binary functions to their corresponding operations.
*/
  std::map<std::string_view, std::function<double(double, double)>>
      kBinaryFunctions;
  /*!

  \fn void RPNCalculator::Operate
  \brief Applies the given operation to the top values in the stack.
  \param operation The operation to apply as a std::string_view.
  */
  void Operate(const std::string_view& operation);
  /*!

  \fn double RPNCalculator::CalcStackPop
  \brief Pops the top value from the stack and returns it.
  \return The value popped from the stack, as a double.
  */
  double CalcStackPop() {
    double val = calc_stack_.top();
    calc_stack_.pop();
    return val;
  }
  /*!

  \fn double RPNCalculator::ToDouble
  \brief Converts a string to a double value, replacing variable x if required.
  \param src The source string to convert.
  \param x The variable x value to substitute (if necessary).
  \return A double value representation of the src string.
  */
  double ToDouble(std::string src, double x) const;

  /*!

\enum RPNCalculator::TokenType
\brief Enumerates different types of tokens encountered in processing RPN
expressions.
*/
  TokenType current_token_;
  /*!

\var RPNCalculator::calc_stack_
\brief Stack to hold intermediate values during RPN calculations.
*/
  std::stack<double> calc_stack_;
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_SRC_MODEL_CALCULATOR_H_
