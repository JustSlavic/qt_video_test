//
// Created by radko on 27.12.2019.
//

#pragma once

#include <QtWidgets/QMainWindow>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QCamera>

class MainWindow : public QMainWindow {
 Q_OBJECT
 public:
  MainWindow();

 public slots:
  QCamera *findCamera();
 private:

  void playVideo(QString &filepath);
  void addMenu();

  QMediaPlayer *mediaPlayer;
  QVideoWidget *videoWidget;
};
