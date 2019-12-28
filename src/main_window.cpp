//
// Created by radko on 27.12.2019.
//

#include "main_window.hpp"

#include <QLayout>
#include <QMenuBar>
#include <QVideoWidget>
#include <QCameraInfo>


MainWindow::MainWindow()
    : mediaPlayer(new QMediaPlayer(this))
    , videoWidget(new QVideoWidget()) {

    this->addMenu();

    videoWidget->setMinimumSize(300, 300);

    // MainWindow takes ownership over QVideoWidget
    setCentralWidget(videoWidget);
    mediaPlayer->setVideoOutput(videoWidget);
}

void MainWindow::addMenu() {
    auto menu = this->menuBar();

    auto menuFile = new QMenu("&File", menu);
    menuFile->addAction("&Load file...");

    menu->addMenu(menuFile);

    // MainWindow takes ownership over the QMenuBar
    this->setMenuBar(menu);
}

void MainWindow::playVideo() {
    mediaPlayer->setMedia(QUrl::fromLocalFile("/home/radko/Storage/Anime/me!me!me!_daoko_feat_teddyloid.mp4"));
    mediaPlayer->play();

    qDebug() << videoWidget->isVisible();
}

QCamera* MainWindow::findCamera() {
    const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    if (cameras.empty()) return nullptr;

    return new QCamera(cameras.at(0));
}

