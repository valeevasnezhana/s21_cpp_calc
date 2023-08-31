#ifndef SRC_MODEL_TOKENIZER_H_
#define SRC_MODEL_TOKENIZER_H_

#include <array>
#include <list>
#include <stack>
#include <string>

namespace s21 {
class Tokenizer final {
 public:
  enum class TokenType {
    kDigit,
    kArg,
    kOperator,
    kFunction,
    kOpenBracket,
    kCloseBracket,
    kUndefined
  };

  static constexpr TokenType GetTokenType(char symbol) noexcept {
    if (('0' <= symbol && symbol <= '9') || symbol == '.')
      return TokenType::kDigit;
    else if (kOperators.find(symbol) + 1)
      return TokenType::kOperator;
    else if (symbol == '(')
      return TokenType::kOpenBracket;
    else if (symbol == ')')
      return TokenType::kCloseBracket;
    else if (symbol == 'x' || symbol == 'X' || symbol == 'y' || symbol == 'Y')
      return TokenType::kArg;
    else if (symbol == ' ' || !symbol)
      return TokenType::kUndefined;
    return TokenType::kFunction;
  }

  static constexpr int GetPriority(char op) noexcept {
    if (op == '+' || op == '-')
      return 0;
    else if (op == '*' || op == '/' || op == '%')
      return 1;
    else if (op == '~' || op == '#')
      return 2;
    else if (op == '^')
      return 3;
    return -1;
  }

  static constexpr bool isLeftWise(char op) noexcept {
    return op != '^' && op != '~' && op != '#';
  }

  static constexpr bool IsNumeric(TokenType token) noexcept {
    return token == TokenType::kDigit || token == TokenType::kArg;
  }

  static constexpr bool IsOperation(TokenType token) noexcept {
    return token == TokenType::kFunction || token == TokenType::kOperator;
  }

  std::list<std::string> Tokenize(const std::string_view& expression);

  bool ExpressionChanged() noexcept { return fixed_; }

 private:
  typedef std::string_view::const_iterator position;

  static constexpr std::string_view kOperators = "+-/*^%#~";
  static constexpr std::array<std::string_view, 16> kFunctions = {
      "ln",  "tg",  "sin",  "cos",  "tan",  "ctg",  "cot",  "log",
      "exp", "atg", "asin", "acos", "atan", "acot", "actg", "sqrt"};

  enum class State { kPush, kDiscard, kFunctionErr, kMismatch };

  void Fix(std::list<std::string>& dest);

  void CloseBracket() noexcept;
  void CollapseOperator(std::list<std::string>& dest);
  bool PushToken(std::list<std::string>& dest);

  void AdvancePosition() noexcept;
  void ThrowErrors(const std::list<std::string>& tokens) const;

  char OpUnary(const position& op) const noexcept;
  char OpBinary(const position& op) const noexcept;

  constexpr bool ValidState() const noexcept;
  constexpr bool MultiplySkipped() const noexcept;
  constexpr bool FunctionEmpty() const noexcept;
  constexpr bool BracketSkipped() const noexcept;
  constexpr bool BracketFinished() const noexcept;
  constexpr bool BracketsBroken() const noexcept;
  constexpr bool OneSymboled() const noexcept;
  constexpr bool ExprFinished(TokenType last_token) const noexcept;

  TokenType prev_token_;
  TokenType current_token_;
  State push_;
  std::stack<char> brackets_;
  position pos_;
  position end_;
  bool fixed_;
};
}  // namespace s21

#endif  // SRC_MODEL_TOKENIZER_H_
