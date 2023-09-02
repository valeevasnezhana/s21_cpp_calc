// Copyright 2023 School21 @gruntmet Snezhana Valeeva
#ifndef CPP3_SMARTCALC_V2_SRC_MODEL_TOKENIZER_H_
#define CPP3_SMARTCALC_V2_SRC_MODEL_TOKENIZER_H_

#include <array>
#include <list>
#include <stack>
#include <string>

/*!

\file tokenizer.h
\brief Header file for the Tokenizer
\namespace s21
*/

namespace s21 {
/*!

\class Tokenizer
\brief A class for tokenizing a mathematical expression.
\details Tokenizes input expressions and categorizes them into various token
types.
*/
class Tokenizer final {
 public:
  /*!

\enum Tokenizer::TokenType
\brief Enumeration for different token types.
*/
  enum class TokenType {
    kDigit,
    kArg,
    kOperator,
    kFunction,
    kOpenBracket,
    kCloseBracket,
    kUndefined
  };

  /*!

\fn Tokenizer::GetTokenType
\brief Classify the token according to the input symbol.
\param symbol A single character, representing the input symbol.
\return TokenType classification of the input symbol.
*/
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

  /*!

\fn int Tokenizer::GetPriority
\brief Retrieves the priority of the input operation.
\param op A single character, representing the operation.
\return The priority level of the operation (0-3), with higher numbers being
higher priority.
*/
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

  /*!

\fn bool Tokenizer::isLeftWise
\brief Determines if the operator is left-associative.
\param op A single character, representing the operation.
\return True if the operation is left-associative, false otherwise.
*/
  static constexpr bool isLeftWise(char op) noexcept {
    return op != '^' && op != '~' && op != '#';
  }

  /*!

\fn bool Tokenizer::IsNumeric
\brief Determines if the TokenType is of numeric type.
\param token A TokenType to be checked.
\return True if the TokenType is of numeric type, false otherwise.
*/
  static constexpr bool IsNumeric(TokenType token) noexcept {
    return token == TokenType::kDigit || token == TokenType::kArg;
  }

  /*!

\fn bool Tokenizer::IsOperation
\brief Determines if the TokenType is of operation type.
\param token A TokenType to be checked.
\return True if the TokenType is of operation type, false otherwise.
*/
  static constexpr bool IsOperation(TokenType token) noexcept {
    return token == TokenType::kFunction || token == TokenType::kOperator;
  }

  /*!

\fn std::liststd::string Tokenizer::Tokenize
\brief Tokenizes the input expression.
\param expression A string_view of the input expression.
\return A list of strings containing the tokens from the input expression.
*/

  std::list<std::string> Tokenize(const std::string_view& expression);

  /*!

\fn bool Tokenizer::ExpressionChanged
\brief Check if the input expression has changed since last tokenizing
operation. \return True if the expression has changed, false otherwise.
*/

  bool ExpressionChanged() noexcept { return fixed_; }

 private:
  typedef std::string_view::const_iterator position;
  /*!

  \var Tokenizer::kOperators
  \brief String view that stores all the available operator symbols.
  */
  static constexpr std::string_view kOperators = "+-/*^%#~";

  /*!

\var Tokenizer::kFunctions
\brief An array of string views storing all supported function names.
*/
  static constexpr std::array<std::string_view, 16> kFunctions = {
      "ln",  "tg",  "sin",  "cos",  "tan",  "ctg",  "cot",  "log",
      "exp", "atg", "asin", "acos", "atan", "acot", "actg", "sqrt"};

  /*!

\enum Tokenizer::State
\brief Enumeration for various states of the token parsing process.
*/

  enum class State { kPush, kDiscard, kFunctionErr, kMismatch };
  /**
   * @brief Fixes the current token in the tokens list.
   * @param dest The list of tokens to modify.
   */
  void Fix(std::list<std::string>& dest);

  /**
   * @brief Handles closing brackets in the tokenization process.
   */
  void CloseBracket() noexcept;
  /**
   * @brief Collapses operator tokens and appends them to the list of tokens.
   * @param dest The list of tokens.
   */
  void CollapseOperator(std::list<std::string>& dest);

  /**
   * @brief Pushes tokens into the result list if they are part of a valid
   * expression.
   * @param dest The list of tokens.
   * @return true if the tokenizer position is not at the end of the expression.
   */
  bool PushToken(std::list<std::string>& dest);

  /**
   * @brief Advances the position in the given input expression and handles
   * special conditions.
   */
  void AdvancePosition() noexcept;

  /**
   * @brief Throws errors based on invalid states in the tokenization process.
   * @param tokens The list of tokens.
   */
  void ThrowErrors(const std::list<std::string>& tokens) const;

  /**
   * @brief Returns the unary operation symbol based on the given operator
   * character.
   * @param op The position of the operator character.
   * @return The unary operation symbol as a char.
   */
  char OpUnary(const position& op) const noexcept;

  /**
   * @brief Returns the binary operation symbol based on the given operator
   * character.
   * @param op The position of the operator character.
   * @return The binary operation symbol as a char.
   */
  char OpBinary(const position& op) const noexcept;

  /**
   * @brief Checks if the tokenizer is in a valid state.
   * @return true if the tokenizer state is valid.
   */
  constexpr bool ValidState() const noexcept;

  /**
   * @brief Checks if a multiplication should be inserted between the previous
   * and current token.
   * @return true if a multiplication is needed.
   */
  constexpr bool MultiplySkipped() const noexcept;

  /**
   * @brief Checks if a function is followed by an invalid token.
   * @return true if a function is followed by an invalid token.
   */
  constexpr bool FunctionEmpty() const noexcept;
  /**
   * @brief Checks if a function is not followed by an open bracket.
   * @return true if a function is not followed by an open bracket.
   */
  constexpr bool BracketSkipped() const noexcept;
  /**
   * @brief Checks if the current placing of brackets is legitimate or not.
   * @return true if the bracket placement in the expression is valid.
   */
  constexpr bool BracketFinished() const noexcept;
  /**
   * @brief Checks if closing a bracket without an associated open bracket is
   * occurring.
   * @return true if an unmatched closing bracket was found.
   */
  constexpr bool BracketsBroken() const noexcept;

  /**
   * @brief Determines if the token type only requires one symbol.
   * @return true if the token type has just one symbol.
   */
  constexpr bool OneSymboled() const noexcept;

  /**
   * @brief Checks if the last token in the tokens list indicates that the
   * expression is complete.
   * @param last_token The last token in the list.
   * @return true if the expression is finished based on the last token.
   */
  constexpr bool ExprFinished(TokenType last_token) const noexcept;

  /*!

\private
\var Tokenizer::prev_token_
\brief The previous TokenType during the tokenizing process.
*/
  TokenType prev_token_;

  /*!

\private
\var Tokenizer::current_token_
\brief The current TokenType during the tokenizing process.
*/
  TokenType current_token_;
  /*!

\private
\var Tokenizer::push_
\brief The current parsing state of the tokenizing process.
*/
  State push_;
  /*!

\private
\var Tokenizer::brackets_
\brief A stack for tracking open brackets.
*/
  std::stack<char> brackets_;
  /*!

\private
\var Tokenizer::pos_
\brief A position iterator pointing to the current symbol in the input
expression.
*/
  position pos_;
  /*!

\private
\var Tokenizer::end_
\brief A position iterator pointing to the end of the input expression.
*/
  position end_;

  /*!

\private
\var Tokenizer::fixed_
\brief A boolean flag indicating if the expression has changed since the last
tokenizing operation.
*/
  bool fixed_;
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_SRC_MODEL_TOKENIZER_H_
