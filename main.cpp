#include <iostream>

#include <QApplication>
#include <QDir>
#include <QThread>

#include "src/main_window.h"
#include "src/video_player.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  auto currentPath = QCoreApplication::applicationDirPath();
  QDir::setCurrent(currentPath);

//  auto videoPlayerThread = new QThread(&app);
  auto videoPlayer = new VideoPlayer();

//  videoPlayer->moveToThread(videoPlayerThread);
//  videoPlayerThread->start();

  auto mainWindow = new MainWindow();

  QObject::connect(mainWindow,
                   &MainWindow::signalPlayVideoFile,
                   videoPlayer,
                   &VideoPlayer::playVideoFile,
                   Qt::QueuedConnection);
  QObject::connect(mainWindow,
                   &MainWindow::signalPlayWebCamera,
                   videoPlayer,
                   &VideoPlayer::playWebCamera,
                   Qt::QueuedConnection);

  QObject::connect(mainWindow,
                   &MainWindow::signalToggleGaussianFilter,
                   videoPlayer,
                   &VideoPlayer::signalToggleGaussianFilter,
                   Qt::QueuedConnection);
  QObject::connect(mainWindow,
                   &MainWindow::signalToggleSobelFilter,
                   videoPlayer,
                   &VideoPlayer::signalToggleSobelFilter,
                   Qt::QueuedConnection);

  QObject::connect(videoPlayer,
                   &VideoPlayer::signalOutputImage,
                   mainWindow,
                   &MainWindow::drawImageOnWidget,
                   Qt::QueuedConnection);

  mainWindow->show();

  return QApplication::exec();
}
