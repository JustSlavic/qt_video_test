//
// Created by radko on 27.12.2019.
//

#include "main_window.hpp"

#include <QLayout>
#include <QMenuBar>
#include <QVideoWidget>


MainWindow::MainWindow() {
    this->addMenu();
    this->addVideoWidget();
}

void MainWindow::addMenu() {
    auto menu = this->menuBar();

    auto menuFile = new QMenu("&File", menu);
    menuFile->addAction("&Load file...");

    menu->addMenu(menuFile);

    // MainWindow takes ownership over the QMenuBar
    this->setMenuBar(menu);
}

void MainWindow::addVideoWidget() {
    auto videoWidget = new QVideoWidget();

    // layout takes ownership over the QVideoWidget
    this->setCentralWidget(videoWidget);
}
