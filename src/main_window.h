#ifndef VIDEO_STREAM_SRC_MAIN_WINDOW_H_
#define VIDEO_STREAM_SRC_MAIN_WINDOW_H_

#include <QtWidgets/QMainWindow>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QCamera>
#include <QtWidgets/QLabel>

class MainWindow : public QMainWindow {
 Q_OBJECT
 public:
  MainWindow();

 signals:
  void signalPlayVideoFile(const QString &filepath);
  void signalPlayWebCamera();

 public slots:
  void drawImageOnWidget(QImage);

 private:
  void addMenu();

  QLabel *m_videoWidget;
};

#endif //VIDEO_STREAM_SRC_MAIN_WINDOW_H_
