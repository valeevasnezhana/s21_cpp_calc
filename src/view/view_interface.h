// Copyright 2023 School21 @gruntmet Snezhana Valeeva
/**
 * @file view_interface.h
 * @brief Header file for the ICalculatorView interface for views of the
 * calculator application.
 */
#ifndef CPP3_SMARTCALC_V2_SRC_VIEW_VIEW_INTERFACE_H_
#define CPP3_SMARTCALC_V2_SRC_VIEW_VIEW_INTERFACE_H_

#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace s21 {
/**
 * @class ICalculatorView
 * @brief Interface for providing a generic view for various types of calculator
 * applications.
 */
class ICalculatorView {
 public:
  typedef std::pair<std::vector<double>, std::vector<double>> set_type;
  typedef std::function<void(const std::string&)> ExprChangedDelegate;
  typedef std::function<double(double)> ExprEvalDelegate;
  typedef std::function<set_type(double, double, double, double)>
      PlotEvalDelegate;

  virtual ~ICalculatorView() = default;

  /**
   * @brief Subscribes a callback to handle the expression evaluation event.
   * @param delegate The callback to be invoked for expression evaluation.
   */
  virtual void SubscribeExprEval(const ExprEvalDelegate& delegate) = 0;

  /**
   * @brief Subscribes a callback to handle the plot evaluation event.
   * @param delegate The callback to be invoked for plotting evaluation.
   */

  virtual void SubscribePlotEval(const PlotEvalDelegate& delegate) = 0;

  /**
   * @brief Sends an error message to the view to be displayed.
   * @param msg The error message to be displayed.
   */

  virtual void SendError(const std::string& msg) = 0;

  /**
   * @brief Returns the current expression entered in the view.
   * @return The entered expression as a string.
   */
  virtual std::string GetExpr() = 0;
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_SRC_VIEW_VIEW_INTERFACE_H_
