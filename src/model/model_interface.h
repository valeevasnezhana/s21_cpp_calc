// Copyright 2023 School21 @gruntmet Snezhana Valeeva
#ifndef CPP3_SMARTCALC_V2_SRC_MODEL_I_CALCULATION_MODEL_H_
#define CPP3_SMARTCALC_V2_SRC_MODEL_I_CALCULATION_MODEL_H_
#include <string_view>
#include <utility>
#include <vector>

#include "badexpression.h"

/*!

\file model_interface.h
 \brief Calculation model interface for various calculation models.
\namespace s21
*/
namespace s21 {
/*!

\class ICalculationModel
\brief Interface definition for a generic calculation model.
*/
class ICalculationModel {
 public:
  /*!

\typedef ICalculationModel::set_type
\brief Pair of std::vector objects representing the X and Y coordinates of a set
of points.
*/
  using set_type = std::pair<std::vector<double>, std::vector<double>>;

  /*!

\fn ICalculationModel::~ICalculationModel
\brief Virtual destructor for the ICalculationModel interface.
*/

  virtual ~ICalculationModel() = default;

  /*!

\fn double ICalculationModel::Calculate
\brief Performs a calculation based on the implemented model.
\param x Optional input value for the calculation (default is 0).
\return The result of the calculation.
*/

  virtual double Calculate(double x = 0) = 0;

  /*!

\fn ICalculationModel::set_type ICalculationModel::Plot
\brief Generates a set of points for plotting in the specified range.
\param x_left Left boundary of the X range.
\param x_right Right boundary of the X range.
\param y_min Lower boundary of the Y range.
\param y_max Upper boundary of the Y range.
\return A set of points, represented as a set_type object.
*/

  virtual set_type Plot(double x_left, double x_right, double y_min,
                        double y_max) = 0;

  /*!

\fn void ICalculationModel::setExpression
\brief Sets the expression for the calculation model.
\param expr The expression to be used in the calculation model.
*/

  virtual void setExpression(const std::string_view& expr) = 0;

  /*!

\fn bool ICalculationModel::ExressionChanged
\brief Checks if the expression has changed since the last calculation.
\return True if the expression has changed, false otherwise.
*/

  virtual bool ExressionChanged() noexcept = 0;
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_SRC_MODEL_I_CALCULATION_MODEL_H_
