#ifndef VIDEO_STREAM_SRC_VIDEO_PLAYER_H_
#define VIDEO_STREAM_SRC_VIDEO_PLAYER_H_

#include "video_widget/frame_emitter.h"
#include "video_widget/gaussian_blur.h"
#include "video_widget/output_video_surface.h"

#include <QMediaPlayer>
#include <QCamera>

class VideoPlayer : public QObject {
 Q_OBJECT
 public:
  VideoPlayer();

 signals:
  void signalOutputImage(QImage);

 public slots:
  void playVideoFile(const QString &filepath);
  void playWebCamera();

 private:
  QCamera *getCamera();
  void stopWebCamera();

 private:
  QMediaPlayer *m_mediaPlayer;
  QCamera *m_camera{nullptr};

  FrameEmitter *m_frameEmitter;
  GaussianBlur *m_gaussianBlur;
  QThread *m_gaussianBlurThread;
  OutputVideoSurface *m_outputSurface;
  QThread *m_outputSurfaceThread;
};

#endif //VIDEO_STREAM_SRC_VIDEO_PLAYER_H_
