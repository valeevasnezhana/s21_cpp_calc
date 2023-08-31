#ifndef SRC_MODEL_I_CALCULATION_MODEL_H_
#define SRC_MODEL_I_CALCULATION_MODEL_H_
#include <string_view>
#include <vector>

#include "badexpression.h"

namespace s21 {
class ICalculationModel {
 public:
  typedef std::pair<std::vector<double>, std::vector<double>> set_type;

  virtual ~ICalculationModel() = default;

  virtual double Calculate(double x = 0) = 0;

  virtual set_type Plot(double x_left, double x_right, double y_min,
                        double y_max) = 0;

  virtual void setExpression(const std::string_view& expr) = 0;

  virtual bool ExressionChanged() noexcept = 0;
};
}  // namespace s21

#endif  // SRC_MODEL_I_CALCULATION_MODEL_H_
