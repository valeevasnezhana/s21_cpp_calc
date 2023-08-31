#ifndef SRC_MODEL_DEFAULT_MODEL_H_
#define SRC_MODEL_DEFAULT_MODEL_H_

#include "calculator.h"
#include "icalculationmodel.h"
#include "tokenizer.h"
#include "translator.h"

namespace s21 {
class DefaultModel final : public ICalculationModel {
 public:
  typedef ICalculationModel BaseModel;
  typedef typename ICalculationModel::set_type set_type;

  static constexpr std::size_t kExprMaxSize = 255;
  static constexpr std::size_t kRangeFinesse = 20000;

  double Calculate(double x = 0) override {
    return calc_.Calculate(reverse_polish_notation_, x);
  };

  set_type Plot(double x_left, double x_right, double y_min,
                double y_max) override {
    std::vector<double> x_set =
        calc_.GenerateSet(x_left, x_right, kRangeFinesse);
    std::vector<double> y_set;
    y_set.reserve(kRangeFinesse);
    for (double x : x_set) {
      double tmp_y = calc_.Calculate(reverse_polish_notation_, x);
      if (tmp_y > y_max or tmp_y < y_min) {
        y_set.push_back(NAN);
      } else {
        y_set.push_back(tmp_y);
      }
    }

    return std::make_pair(x_set, y_set);
  }

  void setExpression(const std::string_view& expression) override {
    if (!expression.size()) throw BadExpression("Empty expression");
    if (expression.size() > kExprMaxSize)
      throw BadExpression("Expression is too long");
    if (expression.compare(input_expression_.c_str())) {
      reverse_polish_notation_ =
          to_polish_.Translate(tokenizer_.Tokenize(expression));
      input_expression_ = std::string(expression.begin(), expression.end());
    }
  }

  bool ExressionChanged() noexcept override {
    return tokenizer_.ExpressionChanged();
  }

 private:
  std::string input_expression_;
  std::list<std::string> reverse_polish_notation_;
  Tokenizer tokenizer_;
  ShuntingYardTranslator to_polish_;
  RPNCalculator calc_;
};
}  // namespace s21

#endif  // SRC_MODEL_DEFAULT_MODEL_H_
