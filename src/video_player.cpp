#include "video_player.h"
#include "main_window.h"
#include "video_widget/output_video_surface.h"

#include <QCameraInfo>
#include <QThread>

VideoPlayer::VideoPlayer()
    : m_mediaPlayer(new QMediaPlayer(this)),
      m_frameEmitter(new FrameEmitter(this)),
      m_gaussianBlur(new GaussianBlur()),
      m_gaussianBlurThread(new QThread(this)),
      m_outputSurface(new OutputVideoSurface()),
      m_outputSurfaceThread(new QThread(this)) {

  m_mediaPlayer->setVideoOutput(m_frameEmitter);

  getCamera();
  if (m_camera) {
    m_camera->setViewfinder(m_frameEmitter);
    m_camera->setCaptureMode(QCamera::CaptureVideo);
  }

  connect(m_frameEmitter,
          &FrameEmitter::signalNextFrame,
          m_gaussianBlur,
          &GaussianBlur::receiveNextFrame,
          Qt::QueuedConnection);
  connect(m_gaussianBlur,
          &GaussianBlur::signalNextFrame,
          m_outputSurface,
          &OutputVideoSurface::receiveNextFrame,
          Qt::QueuedConnection);
  connect(m_outputSurface,
          &OutputVideoSurface::signalOutputImage,
          this,
          &VideoPlayer::signalOutputImage,
          Qt::QueuedConnection);

  connect(this,
          &VideoPlayer::signalToggleGaussianFilter,
          m_gaussianBlur,
          &GaussianBlur::toggle,
          Qt::QueuedConnection);

  m_gaussianBlur->moveToThread(m_gaussianBlurThread);
  m_gaussianBlurThread->start();

  m_outputSurface->moveToThread(m_outputSurfaceThread);
  m_outputSurfaceThread->start();
}

void VideoPlayer::playVideoFile(const QString &filepath) {
  if (m_camera) m_camera->stop();
  m_mediaPlayer->stop();
  m_mediaPlayer->setMedia(QUrl::fromLocalFile(filepath));
  m_mediaPlayer->play();
}

void VideoPlayer::playWebCamera() {
  m_mediaPlayer->stop();
  if (m_camera) m_camera->start();
}

QCamera *VideoPlayer::getCamera() {
  if (!m_camera) {
    auto cameraInfo = QCameraInfo::defaultCamera();
    m_camera = new QCamera(cameraInfo, this);
  }

  return m_camera;
}
