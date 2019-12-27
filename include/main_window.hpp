//
// Created by radko on 27.12.2019.
//

#pragma once

#include <QtWidgets/QMainWindow>


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();

private:
    void addMenu();
    void addVideoWidget();
};
