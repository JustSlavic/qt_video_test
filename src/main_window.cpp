//
// Created by radko on 27.12.2019.
//

#include "main_window.h"

#include <QApplication>
#include <QLayout>
#include <QMenuBar>
#include <QtWidgets/QFileDialog>
#include <QStandardPaths>
#include <QtWidgets/QLabel>

MainWindow::MainWindow()
    : m_videoWidget(new QLabel(this)) {

  addMenu();

  m_videoWidget->setMinimumSize(1280, 720);
  m_videoWidget->setMaximumSize(1600, 900);

  // MainWindow takes ownership over QVideoWidget
  setCentralWidget(m_videoWidget);
}

void MainWindow::addMenu() {
  auto menu = this->menuBar();

  auto menuFile = new QMenu("&File", menu);
  auto loadImageAction = menuFile->addAction("Load &Image...");
  auto loadVideoAction = menuFile->addAction("Load &Video...");
  auto cameraAction = menuFile->addAction("&Camera");
  auto exitAction = menuFile->addAction("&Exit");

  connect(loadImageAction, &QAction::triggered, [this] {
    QString filepath = QFileDialog::getOpenFileName(this,
                                                    tr("Select image"),
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                    tr("Image files (*.jpg *.png);;All files (*)"));

    if (filepath.isNull()) return;
    m_state = State::ImageFile;
    qDebug() << "State::ImageFile";
    emit signalLoadImageFile(filepath);
  });

  connect(loadVideoAction, &QAction::triggered, [this] {
    QString filepath = QFileDialog::getOpenFileName(this,
                                                    tr("Select video"),
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                    tr("Video files (*.mp4 *.avi *.mkv);;All files (*)"));
    if (filepath.isNull()) return;
    m_state = State::VideoFile;
    qDebug() << "State::VideoFile";
    emit signalPlayVideoFile(filepath);
  });

  connect(cameraAction, &QAction::triggered, [this] {
    m_state = State::Camera;
    qDebug() << "State::Camera";
    emit signalPlayWebCamera();
  });

  connect(exitAction, &QAction::triggered, [] { QApplication::quit(); });

  auto menuFilter = new QMenu("&Filter", menu);
  auto gaussianAction = menuFilter->addAction("&Gaussian blur");
  gaussianAction->setCheckable(true);
  gaussianAction->setChecked(false);

  auto sobelAction = menuFilter->addAction("&Sobel operator");
  sobelAction->setCheckable(true);
  sobelAction->setChecked(false);

  connect(gaussianAction, &QAction::triggered, [this] {
    emit signalToggleGaussianFilter();

    if (m_state == State::ImageFile) emit signalUpdateLastImage();
  });
  connect(sobelAction, &QAction::triggered, [this] {
    emit signalToggleSobelFilter();

    if (m_state == State::ImageFile) emit signalUpdateLastImage();
  });

  menu->addMenu(menuFile);
  menu->addMenu(menuFilter);

  // MainWindow takes ownership over the QMenuBar
  this->setMenuBar(menu);
}

void MainWindow::drawImageOnWidget(QImage image) {
  m_videoWidget->resize(image.size());
  m_videoWidget->setPixmap(QPixmap::fromImage(image));
  m_videoWidget->update();
}

