// Copyright 2023 School21 @gruntmet Snezhana Valeeva
#ifndef CPP3_SMARTCALC_V2_SRC_MODEL_BADEXPRESSION_H_
#define CPP3_SMARTCALC_V2_SRC_MODEL_BADEXPRESSION_H_

#include <stdexcept>
#include <string>

namespace s21 {
class BadExpression final : public std::runtime_error {
 public:
  BadExpression(const std::string& what) : std::runtime_error(what) {}
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_SRC_MODEL_BADEXPRESSION_H_
