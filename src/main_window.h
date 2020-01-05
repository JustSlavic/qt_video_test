#ifndef VIDEO_STREAM_SRC_MAIN_WINDOW_H_
#define VIDEO_STREAM_SRC_MAIN_WINDOW_H_

#include <QtWidgets/QMainWindow>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QCamera>

class MainWindow : public QMainWindow {
 Q_OBJECT
 public:
  MainWindow();

  QWidget* getVideoWidget() const;

 signals:
  void signalPlayVideoFile(const QString &filepath);
  void signalPlayWebCamera();

 private:
  void addMenu();

  QWidget *videoWidget;
};

#endif //VIDEO_STREAM_SRC_MAIN_WINDOW_H_
