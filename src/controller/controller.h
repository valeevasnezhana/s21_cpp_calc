// Copyright 2023 School21 @gruntmet Snezhana Valeeva
#ifndef CPP3_SMARTCALC_V2_SRC_CONTROLLER_CONTROLLER_H_
#define CPP3_SMARTCALC_V2_SRC_CONTROLLER_CONTROLLER_H_

#include <cmath>
#include <string>

#include "../model/model_interface.h"
#include "../view/view_interface.h"

/*!

\brief Links CalculationModel with Calculator View.
\file controller.h
*/

namespace s21 {

/*!

\class CalcModelController
\brief Controller class for connecting model and view.
*/
class CalcModelController {
 public:
  static_assert(std::is_same<ICalculationModel::set_type,
                             ICalculatorView::set_type>::value);

  using set_type = ICalculationModel::set_type;

  /*!

\fn CalcModelController::CalcModelController
\brief Constructs CalcModelController and subscribes to events.
\param model Model pointer.
\param view View pointer.
*/
  CalcModelController(ICalculationModel* model, ICalculatorView* view)
      : model_(model), view_(view) {
    view_->SubscribeExprEval(std::bind(&CalcModelController::EvaluationEvent,
                                       this, std::placeholders::_1));
    view_->SubscribePlotEval(std::bind(
        &CalcModelController::PlotEvent, this, std::placeholders::_1,
        std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
  }

 private:
  /*!

\fn EvaluationEvent
\brief Handles expression evaluation events for current x-value.
\param x argument value.
*/

  double EvaluationEvent(double x) {
    double eval_result = NAN;
    try {
      model_->setExpression(view_->GetExpr());
      eval_result = model_->Calculate(x);
      if (model_->ExressionChanged())
        view_->SendError("Note: An attempt was made to fix expression");
    } catch (BadExpression& err) {
      view_->SendError("Error: " + std::string(err.what()));
    }
    return eval_result;
  }

  /*!

\fn PlotEvent
\brief Handles plot evaluation events.
\param left Lower x-axis bound.
\param right Upper x-axis bound.
\param y_min Lower y-axis bound.
\param y_max Upper y-axis bound.

*/

  set_type PlotEvent(double left, double right, double y_min, double y_max) {
    set_type eval_result;
    try {
      model_->setExpression(view_->GetExpr());
      eval_result = model_->Plot(left, right, y_min, y_max);
      if (model_->ExressionChanged())
        view_->SendError("Note: An attempt was made to fix expression");
    } catch (BadExpression& err) {
      view_->SendError("Error: " + std::string(err.what()));
    }
    return eval_result;
  }

  ICalculationModel* model_;
  ICalculatorView* view_;
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_SRC_CONTROLLER_CONTROLLER_H_
