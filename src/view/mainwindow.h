// Copyright 2023 School21 @gruntmet Snezhana Valeeva
/**
 * @file mainwindow.h
 * @brief Header file for the MainWindow class, which serves as the main
 * calculator window.
 */
#ifndef CPP3_SMARTCALC_V2_SRC_VIEW_MAINWINDOW_H_
#define CPP3_SMARTCALC_V2_SRC_VIEW_MAINWINDOW_H_

#include <QLineEdit>
#include <QMainWindow>
#include <set>
#include <string>

#include "view_interface.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public s21::ICalculatorView {
  Q_OBJECT

 public:
  typedef s21::ICalculatorView BaseView;
  typedef BaseView::ExprChangedDelegate ExprChangedDelegate;
  typedef BaseView::ExprEvalDelegate ExprEvalDelegate;
  typedef BaseView::PlotEvalDelegate PlotEvalDelegate;

  /**
   * @brief Construct a MainWindow object.
   * @param parent The parent QWidget object for the MainWindow.
   */
  explicit MainWindow(QWidget* parent = nullptr);
  /**
   * @brief Destructor for the MainWindow object.
   */
  ~MainWindow() override;
  /**
   * @brief Subscribes a callback to handle the expression evaluation event.
   * @param delegate The callback to be invoked for expression evaluation.
   */
  void SubscribeExprEval(const ExprEvalDelegate& delegate) override;
  /**
   * @brief Subscribes a callback to handle the plot evaluation event.
   * @param delegate The callback to be invoked for plotting evaluation.
   */
  void SubscribePlotEval(const PlotEvalDelegate& delegate) override;
  /**
   * @brief Sends an error message to be displayed.
   * @param msg The error message to be displayed.
   */
  void SendError(const std::string& msg) override;
  /**
   * @brief Returns the current expression entered in the MainWindow.
   * @return The entered expression as a string.
   */
  std::string GetExpr() override;

 private:
  /**
   * @brief Set restrictions on input fields and other UI elements.
   */
  void SetRestrictions();
  /**
   * @brief Load and apply styles for UI elements.
   */
  void LoadStyle();
  /**
   * @brief Connect the events for UI elements to corresponding event handlers.
   */
  void ConnectEvents();
  /**
   * @brief Event handler for updating the expression.
   * @param text The updated expression.
   */
  void OnExprChanged(const QString& text);
  /**
   * @brief Handles the event when an input button is pressed.
   */
  void InputButtonPressed();
  /**
   * @brief Deletes a symbol from the input line edit when called.
   */
  void DelSymbol();
  /**
   * @brief Clears all input fields, labels, and the graph.
   */
  void ClearAll();
  /**
   * @brief Evaluates the input expression and displays the result.
   */
  void Eval();
  /**
   * @brief Plots the function based on user input.
   */
  void Plot();
  /**
   * @brief Replaces the string "mod" with the "%" sign in the given text.
   * @param text The string containing the text to replace "mod" with "%".
   * @return Modified text with "mod" replaced by the "%" sign.
   */
  std::string ReplaceCrutch(const std::string& text) const;
  /**
   * @brief Set of banned button names - buttons which don't print their inner
   * text.
   */
  static const std::set<QString> banned_buttons;

  Ui::MainWindow* ui;

  ExprEvalDelegate on_eval_;
  PlotEvalDelegate on_plot_;
};
#endif  // CPP3_SMARTCALC_V2_SRC_VIEW_MAINWINDOW_H_
