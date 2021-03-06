#ifndef VIDEO_STREAM_SRC_VIDEO_PLAYER_H_
#define VIDEO_STREAM_SRC_VIDEO_PLAYER_H_

#include "video_widget/frame_emitter.h"
#include "video_widget/gaussian_blur.h"
#include "video_widget/sobel_operator.h"
#include "video_widget/output_video_surface.h"

#include <QMediaPlayer>
#include <QCamera>

class VideoPlayer : public QObject {
 Q_OBJECT
 public:
  VideoPlayer();

 signals:
  void signalPassImage(QImage);
  void signalOutputImage(QImage);

  void signalToggleGaussianFilter();
  void signalToggleSobelFilter();

 public slots:
  void loadImageFile(const QString &filepath);
  void updateLastImage();
  void playVideoFile(const QString &filepath);
  void playWebCamera();

 private:
  QMediaPlayer *m_mediaPlayer;
  QCamera *m_camera{nullptr};

  FrameEmitter *m_frameEmitter;
  GaussianBlur *m_gaussianBlur;
  QThread *m_gaussianBlurThread;
  SobelOperator *m_sobelOperator;
  QThread *m_sobelOperatorThread;
  OutputVideoSurface *m_outputSurface;
  QThread *m_outputSurfaceThread;
  QImage m_lastImage;
};

#endif //VIDEO_STREAM_SRC_VIDEO_PLAYER_H_
