// Copyright 2023 School21 @gruntmet Snezhana Valeeva
/**
 * @file translator.h
 * @brief Header file for the ShuntingYardTranslator class, which performs
 * translation of mathematical expressions.
 */
#ifndef CPP3_SMARTCALC_V2_SRC_MODEL_TRANSLATOR_H_
#define CPP3_SMARTCALC_V2_SRC_MODEL_TRANSLATOR_H_

#include <list>
#include <stack>
#include <string>

#include "tokenizer.h"

namespace s21 {
/**
 * @class ShuntingYardTranslator
 * @brief This class is responsible for translating tokens obtained with the
 * Tokenizer into Reverse Polish Notation (RPN).
 */
class ShuntingYardTranslator final {
 public:
  using TokenType = Tokenizer::TokenType;

  /**
   * @brief Translates a list of tokens into Reverse Polish Notation (RPN)
   * format
   * @param tokens A list of tokens to translate.
   * @return A translated list of tokens in RPN format.
   */
  std::list<std::string> Translate(const std::list<std::string>& tokens);

 private:
  /**
   * @brief Pushes the top operator from the operator stack to the output list.
   * @param dest The list of tokens to modify.
   */
  void PushToOut(std::list<std::string>& dest) {
    dest.push_back(std::string(operator_stack_.top()));
    operator_stack_.pop();
  }

  /**
   * @brief Checks if the priority of the input token is less than or equal to
   * the priority of the top token in the stack.
   * @param token The input token.
   * @return true if the priority of the input token is less than or equal to
   * the priority of the top token in the stack.
   */

  bool PriorityLess(const std::string& token) {
    return !operator_stack_.empty() &&
           Tokenizer::GetPriority(token.at(0)) <=
               Tokenizer::GetPriority(operator_stack_.top().at(0)) &&
           Tokenizer::isLeftWise(token.at(0));
  }

  /**
   * @brief Checks if the top element in the operator stack is not an open
   * bracket.
   * @return true if the top element is not an open bracket.
   */
  bool NotOpenBracket() {
    return !operator_stack_.empty() &&
           Tokenizer::GetTokenType(operator_stack_.top().at(0)) !=
               TokenType::kOpenBracket;
  }

  /**
   * @brief Checks if there is a function on top of the operator stack.
   * @return true if there is a function on top of the operator stack.
   */
  bool FucntionOnTop() {
    return !operator_stack_.empty() &&
           Tokenizer::GetTokenType(operator_stack_.top().at(0)) ==
               TokenType::kFunction;
  }

  std::stack<std::string> operator_stack_ =
      std::stack<std::string>(); /**< Stack for holding operators and
                                    functions*/
  TokenType current_token_ =
      TokenType(); /**< The current TokenType being processed*/
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_SRC_MODEL_TRANSLATOR_H_
