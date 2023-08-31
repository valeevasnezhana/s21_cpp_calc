#include "tokenizer.h"

#include "badexpression.h"

namespace s21 {
std::list<std::string> Tokenizer::Tokenize(const std::string_view& expression) {
  if (expression.begin() == expression.end())
    throw BadExpression("Expression is empty");
  std::list<std::string> tokens;
  pos_ = expression.begin();
  end_ = expression.end();
  prev_token_ = TokenType::kUndefined;
  current_token_ = TokenType::kUndefined;
  fixed_ = false;
  for (; !brackets_.empty(); brackets_.pop()) {
  }
  for (; pos_ != end_ && *pos_ == ' '; ++pos_, fixed_ = true) {
  }
  do {
    push_ = State::kPush;
    current_token_ = GetTokenType(*pos_);
    Fix(tokens);
  } while (PushToken(tokens) && ValidState());
  ThrowErrors(tokens);
  for (; !brackets_.empty(); brackets_.pop(), fixed_ = true)
    tokens.push_back(")");
  return tokens;
}

void Tokenizer::Fix(std::list<std::string>& dest) {
  if (BracketSkipped()) {
    dest.push_back("(");
    brackets_.push(-')');
    fixed_ = true;
  } else if (BracketFinished()) {
    for (; !brackets_.empty() && brackets_.top() < 0;
         brackets_.pop(), fixed_ = true)
      dest.push_back(")");
  }

  if (current_token_ == TokenType::kOperator) {
    CollapseOperator(dest);
  } else if (current_token_ == TokenType::kOpenBracket) {
    brackets_.push(')');
  } else if (current_token_ == TokenType::kCloseBracket) {
    CloseBracket();
  }

  if (MultiplySkipped()) {
    fixed_ = true;
    dest.push_back("*");
  } else if (BracketsBroken()) {
    push_ = State::kMismatch;
  }
}

void Tokenizer::CloseBracket() noexcept {
  if (brackets_.empty()) {
    fixed_ = true;
    push_ = State::kDiscard;
  } else {
    brackets_.pop();
  }
}

void Tokenizer::CollapseOperator(std::list<std::string>& dest) {
  push_ = State::kDiscard;
  if (OpBinary(pos_)) {
    dest.push_back(std::string(1, OpBinary(pos_)));
  } else if (OpUnary(pos_)) {
    dest.push_back(std::string(1, OpUnary(pos_)));
  } else {
    push_ = State::kMismatch;
  }
  prev_token_ = current_token_;
}

bool Tokenizer::PushToken(std::list<std::string>& dest) {
  position start = pos_;
  AdvancePosition();
  if (start == pos_) push_ = State::kFunctionErr;
  if (push_ == State::kPush) {
    dest.push_back(std::string(start, pos_));
    prev_token_ = current_token_;
  }
  for (; pos_ != end_ && *pos_ == ' '; ++pos_) {
  }
  return pos_ != end_;
}

char Tokenizer::OpUnary(const position& op) const noexcept {
  if (*op == '+' || *op == '#')
    return '#';
  else if (*op == '-' || *op == '~')
    return '~';
  return '\0';
}

char Tokenizer::OpBinary(const position& op) const noexcept {
  if (!IsNumeric(prev_token_) && prev_token_ != TokenType::kCloseBracket)
    return '\0';
  if (*op == '+' || *op == '#')
    return '+';
  else if (*op == '-' || *op == '~')
    return '-';
  return *op;
}

constexpr bool Tokenizer::ValidState() const noexcept {
  return push_ == State::kPush || push_ == State::kDiscard;
}

constexpr bool Tokenizer::MultiplySkipped() const noexcept {
  return (IsNumeric(prev_token_) || prev_token_ == TokenType::kCloseBracket) &&
         (IsNumeric(current_token_) ||
          current_token_ == TokenType::kOpenBracket ||
          current_token_ == TokenType::kFunction);
}

constexpr bool Tokenizer::FunctionEmpty() const noexcept {
  return prev_token_ == TokenType::kFunction && !IsNumeric(current_token_);
}

constexpr bool Tokenizer::BracketSkipped() const noexcept {
  return prev_token_ == TokenType::kFunction &&
         current_token_ != TokenType::kOpenBracket;
}

constexpr bool Tokenizer::BracketFinished() const noexcept {
  return !IsNumeric(current_token_) && prev_token_ != TokenType::kFunction &&
         prev_token_ != TokenType::kOperator;
}

constexpr bool Tokenizer::BracketsBroken() const noexcept {
  return (prev_token_ == TokenType::kOperator ||
          prev_token_ == TokenType::kFunction ||
          prev_token_ == TokenType::kOpenBracket) &&
         current_token_ == TokenType::kCloseBracket;
}

constexpr bool Tokenizer::OneSymboled() const noexcept {
  return current_token_ == TokenType::kOperator ||
         current_token_ == TokenType::kArg ||
         current_token_ == TokenType::kOpenBracket ||
         current_token_ == TokenType::kCloseBracket;
}

constexpr bool Tokenizer::ExprFinished(TokenType last_token) const noexcept {
  return last_token == TokenType::kCloseBracket || IsNumeric(last_token);
}

void Tokenizer::AdvancePosition() noexcept {
  if (OneSymboled()) {
    ++pos_;
  } else if (current_token_ == TokenType::kFunction) {
    std::size_t rem = std::distance(pos_, end_);
    auto func = kFunctions.begin();
    for (;
         func != kFunctions.end() &&
         func->compare(std::string_view(&pos_[0], std::min(func->size(), rem)));
         ++func) {
    }
    pos_ += std::min(func->size(), rem) * (func != kFunctions.end());
  } else {
    for (; pos_ != end_ && (GetTokenType(*pos_) == TokenType::kDigit); ++pos_) {
    };
    if (pos_ != end_ && *pos_ == 'e') {
      ++pos_;
      if (pos_ != end_ && (*pos_ == '+' || *pos_ == '-')) ++pos_;
      for (; pos_ != end_ && (GetTokenType(*pos_) == TokenType::kDigit);
           ++pos_) {
      };
    }
  }
}

void Tokenizer::ThrowErrors(const std::list<std::string>& tokens) const {
  if (push_ == State::kFunctionErr)
    throw BadExpression("Expression has unknown function");
  else if (push_ == State::kMismatch)
    throw BadExpression("Expression has mismatched token");
  else if (tokens.empty() ||
           !ExprFinished(
               GetTokenType(*std::string_view(tokens.back().c_str()).begin())))
    throw BadExpression("Expression is not finished");
}
}  // namespace s21
