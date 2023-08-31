#include <QApplication>
#include <clocale>
#include <iostream>

#include "controller/controller.h"
#include "model/defaultmodel.h"
#include "view/mainwindow.h"

int main(int argc, char *argv[]) {
  std::setlocale(LC_NUMERIC, "C");
  QApplication application(argc, argv);
  MainWindow window;
  s21::DefaultModel model;
  s21::CalcModelController controller(&model, &window);
  window.show();
  return application.exec();
}
