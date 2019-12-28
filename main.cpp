#include <iostream>

#include <QApplication>
#include <QDir>

#include "main_window.hpp"

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  auto currentPath = QCoreApplication::applicationDirPath();
  QDir::setCurrent(currentPath);

  auto mainWindow = new MainWindow();

  mainWindow->show();

  return QApplication::exec();
}
