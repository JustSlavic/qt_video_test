#ifndef VIDEO_STREAM_SRC_MAIN_WINDOW_H_
#define VIDEO_STREAM_SRC_MAIN_WINDOW_H_

#include <QtWidgets/QMainWindow>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QCamera>
#include <QtWidgets/QLabel>

class MainWindow : public QMainWindow {
 Q_OBJECT
 public:
  enum class State {
    Idle,
    ImageFile,
    VideoFile,
    Camera,
  };

  MainWindow();

 signals:
  void signalLoadImageFile(const QString &filepath);
  void signalUpdateLastImage();
  void signalPlayVideoFile(const QString &filepath);
  void signalPlayWebCamera();

  void signalToggleGaussianFilter();
  void signalToggleSobelFilter();

 public slots:
  void drawImageOnWidget(QImage);

 private:
  void addMenu();

  QLabel *m_videoWidget;
  State m_state{State::Idle};
};

#endif //VIDEO_STREAM_SRC_MAIN_WINDOW_H_
