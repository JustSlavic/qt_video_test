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

  m_videoWidget->setMinimumSize(300, 300);
  m_videoWidget->setMaximumSize(1600, 900);

  // MainWindow takes ownership over QVideoWidget
  setCentralWidget(m_videoWidget);
}

void MainWindow::addMenu() {
  auto menu = this->menuBar();

  auto menuFile = new QMenu("&File", menu);
  auto loadFileAction = menuFile->addAction("&Load file...");
  auto cameraAction = menuFile->addAction("&Camera");
  auto exitAction = menuFile->addAction("&Exit");

  connect(loadFileAction, &QAction::triggered, [this] {
    QString filepath = QFileDialog::getOpenFileName(this,
                                                    tr("Select video"),
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                    tr("Video files (*.mp4 *.avi *.mkv);;All files (*)"));
    if (filepath.isNull()) return;
    emit signalPlayVideoFile(filepath);
  });

  connect(cameraAction, &QAction::triggered, this, &MainWindow::signalPlayWebCamera);
  connect(exitAction, &QAction::triggered, []{ QApplication::quit(); });

  auto menuFilter = new QMenu("&Filter", menu);
  menuFilter->addAction("&No filter");
  menuFilter->addAction("&Gaussian blur");
  menuFilter->addAction("&Sobel operator");

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

