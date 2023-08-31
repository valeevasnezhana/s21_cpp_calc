#include "calculator.h"

#include <clocale>
#include <string>
#include <string_view>

#include "badexpression.h"

namespace s21 {
double RPNCalculator::Calculate(const std::list<std::string>& expr, double x) {
  std::setlocale(LC_NUMERIC, "C");
  for (auto token : expr) {
    current_token_ = Tokenizer::GetTokenType(token.at(0));
    if (Tokenizer::IsNumeric(current_token_)) {
      calc_stack_.push(ToDouble(token, x));
    } else if (Tokenizer::IsOperation(current_token_)) {
      Operate(token.c_str());
    }
  }
  double result = CalcStackPop();
  return result;
}

void RPNCalculator::Operate(const std::string_view& operation) {
  double r = CalcStackPop();
  if (kUnaryFunctions.find(operation) != kUnaryFunctions.end())
    return calc_stack_.push(kUnaryFunctions[operation](r));
  double l = CalcStackPop();
  if (kBinaryFunctions.find(operation) != kBinaryFunctions.end())
    return calc_stack_.push(kBinaryFunctions[operation](l, r));
}

double RPNCalculator::ToDouble(std::string src, double x) const {
  if (src.at(0) == 'x') return x;
  std::size_t idx;
  double dbl = NAN;
  try {
    dbl = std::stod(src, &idx);
    if (idx != src.size()) throw std::logic_error("");
  } catch (std::logic_error& e) {
    throw BadExpression("Invalid number in expression");
  }
  return dbl;
}

std::vector<double> RPNCalculator::GenerateSet(double l, double r,
                                               int fin) const {
  if (l > r) throw BadExpression("Invalid set borders");
  std::vector<double> points;
  points.reserve(fin);
  double delta = (r - l) / (fin - 1);
  for (int i = 0; i < fin; i++) {
    points.push_back(l);
    l += delta;
  }
  return points;
}
}  // namespace s21
