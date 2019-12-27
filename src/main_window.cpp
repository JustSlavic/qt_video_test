//
// Created by radko on 27.12.2019.
//

#include "main_window.hpp"

#include <QMenuBar>


MainWindow::MainWindow() {
    this->createMenu();
}


void MainWindow::createMenu() {
    auto menu = new QMenuBar();

    auto menuFile = new QMenu("File", menu);
    menuFile->addAction("Load file...");

    menu->addMenu(menuFile);

    this->setMenuBar(menu);
}
