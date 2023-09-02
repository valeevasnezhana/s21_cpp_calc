// Copyright 2023 School21 @gruntmet Snezhana Valeeva
#include "translator.h"

#include <string>

namespace s21 {
std::list<std::string> ShuntingYardTranslator::Translate(
    const std::list<std::string>& tokens) {
  std::list<std::string> reverse_polish_notation;
  for (auto token : tokens) {
    current_token_ = Tokenizer::GetTokenType(token.at(0));
    if (Tokenizer::IsNumeric(current_token_)) {
      reverse_polish_notation.push_back(token);
    } else if (current_token_ == TokenType::kFunction ||
               current_token_ == TokenType::kOpenBracket) {
      operator_stack_.push(token);
    } else if (current_token_ == TokenType::kOperator) {
      while (PriorityLess(token)) PushToOut(reverse_polish_notation);
      operator_stack_.push(token);
    } else if (current_token_ == TokenType::kCloseBracket) {
      while (NotOpenBracket()) PushToOut(reverse_polish_notation);
      operator_stack_.pop();
      if (FucntionOnTop()) PushToOut(reverse_polish_notation);
    }
  }
  while (!operator_stack_.empty()) {
    PushToOut(reverse_polish_notation);
  }
  return reverse_polish_notation;
}
}  // namespace s21
