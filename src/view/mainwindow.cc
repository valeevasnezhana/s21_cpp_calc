// Copyright 2023 School21 @gruntmet Snezhana Valeeva
#include "mainwindow.h"

#include <QDoubleValidator>
#include <QFile>
#include <QFontDatabase>
#include <QTextStream>
#include <regex>
#include <string>

#include "./ui_mainwindow.h"

const std::set<QString> MainWindow::banned_buttons = {QString("button_ac"),
                                                      QString("button_del")};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  SetRestrictions();
  LoadStyle();
  ConnectEvents();
}

std::string MainWindow::GetExpr() {
  return ReplaceCrutch(ui->edit_input->text().toStdString());
}

void MainWindow::SetRestrictions() {
  QLocale lo(QLocale::C);
  lo.setNumberOptions(QLocale::RejectGroupSeparator);
  auto val = new QDoubleValidator();
  val->setLocale(lo);
  ui->input_x->setValidator(val);
  ui->input_xl->setValidator(val);
  ui->input_xr->setValidator(val);
  ui->input_yl->setValidator(val);
  ui->input_yr->setValidator(val);
}

void MainWindow::LoadStyle() {
  QFontDatabase::addApplicationFont(":/fonts/Tahoma.ttf");
  QFont font("Tahoma", 14);

  QFile style_file(":/styles/style.ss");
  style_file.open(QIODevice::ReadOnly);

  QTextStream style_stream(&style_file);
  setStyleSheet(style_stream.readAll());

  ui->plot->setBackground(QColor(0x1f, 0x1f, 0x1f, 0xff));

  ui->plot->xAxis->setBasePen(QColor(0xff, 0xff, 0xff, 0xff));
  ui->plot->xAxis->setTickLabelColor(QColor(0xff, 0xff, 0xff, 0xff));
  ui->plot->xAxis->setLabelColor(QColor(0xff, 0xff, 0xff, 0xff));
  ui->plot->xAxis->setTickLabelFont(font);
  ui->plot->xAxis->setLabelFont(font);

  ui->plot->yAxis->setBasePen(QColor(0xff, 0xff, 0xff, 0xff));
  ui->plot->yAxis->setTickLabelColor(QColor(0xff, 0xff, 0xff, 0xff));
  ui->plot->yAxis->setLabelColor(QColor(0xff, 0xff, 0xff, 0xff));
  ui->plot->yAxis->setTickLabelFont(font);
  ui->plot->yAxis->setLabelFont(font);

  ui->plot->addGraph();
  ui->plot->graph(0)->setPen(QPen(QColor(0xff, 0xaa, 0x44, 0xff), 1.2));

  ui->plot->xAxis->setLabel("x");
  ui->plot->yAxis->setLabel("y");
  ui->plot->xAxis->setRange(ui->input_xl->text().toDouble(),
                            ui->input_xr->text().toDouble());
  ui->plot->yAxis->setRange(ui->input_yl->text().toDouble(),
                            ui->input_yr->text().toDouble());
}

void MainWindow::ConnectEvents() {
  QPushButton* target_button;
  for (auto button : ui->Buttons->children()) {
    if (banned_buttons.find(button->objectName()) == banned_buttons.end()) {
      target_button = dynamic_cast<QPushButton*>(button);
      if (target_button)
        connect(target_button, &QPushButton::clicked, this,
                &MainWindow::InputButtonPressed);
    }
  }
  for (auto func : ui->Functions->children()) {
    target_button = dynamic_cast<QPushButton*>(func);
    if (target_button)
      connect(target_button, &QPushButton::clicked, this,
              &MainWindow::InputButtonPressed);
  }
  for (auto br : ui->Brackets->children()) {
    target_button = dynamic_cast<QPushButton*>(br);
    if (target_button)
      connect(target_button, &QPushButton::clicked, this,
              &MainWindow::InputButtonPressed);
  }
  connect(ui->button_ac, &QPushButton::clicked, this, &MainWindow::ClearAll);
  connect(ui->button_del, &QPushButton::clicked, this, &MainWindow::DelSymbol);
  connect(ui->button_eq_2, &QPushButton::clicked, this, &MainWindow::Eval);
  connect(ui->button_plot, &QPushButton::clicked, this, &MainWindow::Plot);

  connect(ui->edit_input, &QLineEdit::editingFinished, this, &MainWindow::Eval);
}

void MainWindow::InputButtonPressed() {
  QPushButton* sender_button = static_cast<QPushButton*>(sender());
  ui->edit_input->insert(sender_button->text());
}

void MainWindow::DelSymbol() { ui->edit_input->backspace(); }

void MainWindow::ClearAll() {
  ui->edit_input->clear();
  ui->input_x->setText("0");
  ui->input_xl->setText("-10");
  ui->input_xr->setText("10");
  ui->input_yl->setText("-10");
  ui->input_yr->setText("10");

  ui->label_msg->clear();
  ui->label_output->setText("0");
  ui->plot->graph(0)->data()->clear();
  ui->plot->replot();
}

void MainWindow::SendError(const std::string& msg) {
  ui->label_msg->setText(QString::fromStdString(msg));
}

void MainWindow::Eval() {
  ui->label_msg->clear();
  bool succ;
  double arg = ui->input_x->text().toDouble(&succ);
  if (succ)
    ui->label_output->setText(QString::number(on_eval_(arg)));
  else
    SendError("Error: Invalid x argument");
}

void MainWindow::Plot() {
  ui->label_msg->clear();
  bool succ, accumulated = true;
  double xlb = ui->input_xl->text().toDouble(&succ);
  accumulated &= succ;
  double xrb = ui->input_xr->text().toDouble(&succ);
  accumulated &= succ;
  double ylb = ui->input_yl->text().toDouble(&succ);
  accumulated &= succ;
  double yrb = ui->input_yr->text().toDouble(&succ);
  if (yrb < ylb) {
    SendError("Error: Invalid set boundaries");
  } else if (accumulated) {
    auto set = on_plot_(xlb, xrb, 2 * ylb - yrb, 2 * yrb - ylb);
    QVector<double> x_set(set.first.begin(), set.first.end());
    QVector<double> y_set(set.second.begin(), set.second.end());
    ui->plot->graph(0)->setData(x_set, y_set);

    ui->plot->xAxis->setRange(xlb, xrb);
    ui->plot->yAxis->setRange(ylb, yrb);
    ui->plot->replot();
  } else {
    SendError("Error: Invalid x argument");
  }
}

void MainWindow::SubscribeExprEval(const ExprEvalDelegate& delegate) {
  on_eval_ = delegate;
}

void MainWindow::SubscribePlotEval(const PlotEvalDelegate& delegate) {
  on_plot_ = delegate;
}

std::string MainWindow::ReplaceCrutch(const std::string& text) const {
  return std::regex_replace(text, std::regex("mod"), "%");
}

MainWindow::~MainWindow() { delete ui; }
