#ifndef VIDEO_STREAM_SRC_MAIN_WINDOW_H_
#define VIDEO_STREAM_SRC_MAIN_WINDOW_H_

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
  void addMenu();

  QMediaPlayer *mediaPlayer;
  QVideoWidget *videoWidget;
  QCamera *camera;
};

#endif //VIDEO_STREAM_SRC_MAIN_WINDOW_H_
