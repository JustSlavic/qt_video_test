#include "video_player.h"
#include "main_window.h"
#include "video_widget/output_video_surface.h"

#include <QCameraInfo>
#include <QThread>

VideoPlayer::VideoPlayer()
    : m_mediaPlayer(new QMediaPlayer(this)),
      m_frameEmitter(new FrameEmitter(this)),
      m_gaussianBlur(new GaussianBlur(3)),
      m_gaussianBlurThread(new QThread(this)),
      m_sobelOperator(new SobelOperator()),
      m_sobelOperatorThread(new QThread(this)),
      m_outputSurface(new OutputVideoSurface()),
      m_outputSurfaceThread(new QThread(this)) {

  m_mediaPlayer->setVideoOutput(m_frameEmitter);

  auto cameraInfo = QCameraInfo::defaultCamera();
  if (!cameraInfo.isNull()) {
    m_camera = new QCamera(cameraInfo, this);
    m_camera->setViewfinder(m_frameEmitter);
    m_camera->setCaptureMode(QCamera::CaptureVideo);
  }

  connect(this,
          &VideoPlayer::signalPassImage,
          m_frameEmitter,
          &FrameEmitter::signalPassImage,
          Qt::QueuedConnection);
  connect(m_frameEmitter,
          &FrameEmitter::signalPassImage,
          m_gaussianBlur,
          &GaussianBlur::receiveImage,
          Qt::QueuedConnection);
  connect(m_gaussianBlur,
          &GaussianBlur::signalPassImage,
          m_sobelOperator,
          &SobelOperator::receiveImage,
          Qt::QueuedConnection);
  connect(m_sobelOperator,
          &SobelOperator::signalPassImage,
          m_outputSurface,
          &OutputVideoSurface::signalOutputImage);

  connect(m_frameEmitter,
          &FrameEmitter::signalNextFrameReady,
          m_gaussianBlur,
          &GaussianBlur::receiveNextFrame,
          Qt::QueuedConnection);
  connect(m_gaussianBlur,
          &GaussianBlur::signalNextFrameReady,
          m_sobelOperator,
          &SobelOperator::receiveNextFrame,
          Qt::QueuedConnection);
  connect(m_sobelOperator,
          &SobelOperator::signalNextFrameReady,
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
  connect(this,
          &VideoPlayer::signalToggleSobelFilter,
          m_sobelOperator,
          &SobelOperator::toggle,
          Qt::QueuedConnection);

  m_gaussianBlur->moveToThread(m_gaussianBlurThread);
  m_gaussianBlurThread->start();

  m_sobelOperator->moveToThread(m_sobelOperatorThread);
  m_sobelOperatorThread->start();

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

void VideoPlayer::loadImageFile(const QString &filepath) {
  m_lastImage.load(filepath);
  if (m_lastImage.isNull()) return;

  m_mediaPlayer->stop();
  if (m_camera) m_camera->stop();

  emit signalPassImage(m_lastImage);
}

void VideoPlayer::updateLastImage() {
  if (!m_lastImage.isNull()) {
    emit signalPassImage(m_lastImage);
  }
}
