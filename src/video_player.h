#ifndef VIDEO_STREAM_SRC_VIDEO_PLAYER_H_
#define VIDEO_STREAM_SRC_VIDEO_PLAYER_H_

#include <QMediaPlayer>
#include <QCamera>

class VideoPlayer : public QObject {
  Q_OBJECT
 public:
  VideoPlayer();

 public slots:
  void playVideoFile(const QString &filepath);
  void playWebCamera();

 private:
  QCamera *getCamera();
  void stopVideoFile();
  void stopWebCamera();

 private:
  QMediaPlayer *m_mediaPlayer;
  QCamera *m_camera;
};

#endif //VIDEO_STREAM_SRC_VIDEO_PLAYER_H_
