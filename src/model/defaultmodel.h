// Copyright 2023 School21 @gruntmet Snezhana Valeeva
#ifndef CPP3_SMARTCALC_V2_SRC_MODEL_DEFAULT_MODEL_H_
#define CPP3_SMARTCALC_V2_SRC_MODEL_DEFAULT_MODEL_H_

#include <list>
#include <string>
#include <utility>
#include <vector>

#include "calculator.h"
#include "model_interface.h"
#include "tokenizer.h"
#include "translator.h"

/*!

\file defaultmodel.h
\namespace s21
*/
namespace s21 {
/*!

\class DefaultModel
\brief Default implementation of the ICalculationModel interface.
\details Inherits from ICalculationModel and provides a default calculation
model.
*/
class DefaultModel final : public ICalculationModel {
 public:
  using BaseModel = ICalculationModel;
  using set_type = typename ICalculationModel::set_type;

  /*!

\var DefaultModel::kExprMaxSize
\brief Maximum size constant for the underlying expression.
*/
  static constexpr std::size_t kExprMaxSize = 255;

  /*!

\var DefaultModel::kRangeFinesse
\brief Number of points generated for the plot's range.
*/
  static constexpr std::size_t kRangeFinesse = 20000;

  /*!

\fn double DefaultModel::Calculate
\brief Overrides the base class Calculate function.
\param x Optional parameter representing the input value for the calculation
(default is 0).
\return The result of the calculation.
*/

  double Calculate(double x = 0) override {
    return calc_.Calculate(reverse_polish_notation_, x);
  };

  /*!

\fn DefaultModel::set_type DefaultModel::Plot
\brief Overrides the base class Plot function for generating a set of plot
points. \details Uses the range [x_left, x_right] and [y_min, y_max] to generate
the set of points. \param x_left Left boundary of the X range. \param x_right
Right boundary of the X range. \param y_min Lower boundary of the Y range.
\param y_max Upper boundary of the Y range.
\return A set of points, represented as a set_type object.
*/

  set_type Plot(double x_left, double x_right, double y_min,
                double y_max) override {
    std::vector<double> x_set =
        calc_.GenerateSet(x_left, x_right, kRangeFinesse);
    std::vector<double> y_set;
    y_set.reserve(kRangeFinesse);
    for (double x : x_set) {
      double tmp_y = calc_.Calculate(reverse_polish_notation_, x);
      if (tmp_y > y_max || tmp_y < y_min) {
        y_set.push_back(NAN);
      } else {
        y_set.push_back(tmp_y);
      }
    }

    return std::make_pair(x_set, y_set);
  }

  /*!

\fn void DefaultModel::setExpression
\brief Overrides the setExpression function from the base class.
\details Updates the expression being used by the DefaultModel.
\param expression The expression to be set.
\exception BadExpression If the provided expression is empty or too long.
*/

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

  /*!

\fn bool DefaultModel::ExressionChanged
\brief Overrides the ExressionChanged function from the base class.
\details Checks if the input_expression_ has changed since the last calculation.
\return True if the input_expression_ has changed, false otherwise.
*/

  bool ExressionChanged() noexcept override {
    return tokenizer_.ExpressionChanged();
  }

 private:
  /*!

\private
\var DefaultModel::input_expression_
\brief A string representing the input expression for the DefaultModel.
*/
  std::string input_expression_;
  /*!

\private
\var DefaultModel::reverse_polish_notation_
\brief A list of strings representing the reverse Polish notation of the input
expression.
*/
  std::list<std::string> reverse_polish_notation_;

  /*!

\private
\var DefaultModel::tokenizer_
\brief Tokenizer object responsible for splitting the input expression into
smaller tokens.
*/
  Tokenizer tokenizer_;
  /*!

\private
\var DefaultModel::to_polish_
\brief ShuntingYardTranslator object responsible for translating the tokenized
expression into the reverse Polish notation.
*/
  ShuntingYardTranslator to_polish_;
  /*!

\private
\var DefaultModel::calc_
\brief RPNCalculator object responsible for performing calculations using the
reverse Polish notation.
*/
  RPNCalculator calc_;
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_SRC_MODEL_DEFAULT_MODEL_H_
