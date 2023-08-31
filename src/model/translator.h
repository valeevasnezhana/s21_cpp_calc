#ifndef SRC_MODEL_TRANSLATOR_H_
#define SRC_MODEL_TRANSLATOR_H_

#include <list>

#include "tokenizer.h"

namespace s21 {
class ShuntingYardTranslator final {
 public:
  typedef Tokenizer::TokenType TokenType;

  std::list<std::string> Translate(const std::list<std::string>& tokens);

 private:
  void PushToOut(std::list<std::string>& dest) {
    dest.push_back(std::string(operator_stack_.top()));
    operator_stack_.pop();
  }

  bool PriorityLess(const std::string& token) {
    return !operator_stack_.empty() &&
           Tokenizer::GetPriority(token.at(0)) <=
               Tokenizer::GetPriority(operator_stack_.top().at(0)) &&
           Tokenizer::isLeftWise(token.at(0));
  };

  bool NotOpenBracket() {
    return !operator_stack_.empty() &&
           Tokenizer::GetTokenType(operator_stack_.top().at(0)) !=
               TokenType::kOpenBracket;
  };

  bool FucntionOnTop() {
    return !operator_stack_.empty() &&
           Tokenizer::GetTokenType(operator_stack_.top().at(0)) ==
               TokenType::kFunction;
  };

  std::stack<std::string> operator_stack_;
  TokenType current_token_;
};
}  // namespace s21

#endif  // SRC_MODEL_TRANSLATOR_H_
