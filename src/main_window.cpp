//
// Created by radko on 27.12.2019.
//

#include "main_window.hpp"

#include <QLayout>
#include <QMenuBar>
#include <QVideoWidget>
#include <QCameraInfo>
#include <QtWidgets/QFileDialog>
#include <QStandardPaths>

MainWindow::MainWindow()
    : mediaPlayer(new QMediaPlayer(this)), videoWidget(new QVideoWidget()) {

  this->addMenu();

  videoWidget->setMinimumSize(300, 300);

  // MainWindow takes ownership over QVideoWidget
  setCentralWidget(videoWidget);
  mediaPlayer->setVideoOutput(videoWidget);
}

void MainWindow::addMenu() {
  auto menu = this->menuBar();

  auto menuFile = new QMenu("&File", menu);
  auto loadFileAction = menuFile->addAction("&Load file...");
  auto cameraAction = menuFile->addAction("&Camera");

  connect(loadFileAction, &QAction::triggered, [this]() {
    QString videoFilePath = QFileDialog::getOpenFileName(this,
                                                         tr("Select video"),
                                                         QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                         tr("Video files (*.mp4 *.avi *.mkv);;All files (*)"));
    if (videoFilePath.isNull()) return;

    this->playVideo(videoFilePath);
  });

  auto menuFilter = new QMenu("&Filter", menu);
  menuFilter->addAction("&No filter");
  menuFilter->addAction("&Gaussian blur");
  menuFilter->addAction("&Sobel operator");

  menu->addMenu(menuFile);
  menu->addMenu(menuFilter);

  // MainWindow takes ownership over the QMenuBar
  this->setMenuBar(menu);
}

void MainWindow::playVideo(QString &filepath) {
  mediaPlayer->setMedia(QUrl::fromLocalFile(filepath));
  mediaPlayer->play();
}

QCamera *MainWindow::findCamera() {
  const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
  if (cameras.empty()) return nullptr;

  return new QCamera(cameras.at(0), this);
}

