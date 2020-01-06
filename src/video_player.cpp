#include "video_player.h"
#include "main_window.h"
#include "video_widget/output_video_surface.h"

#include <QCameraInfo>

VideoPlayer::VideoPlayer()
    : m_mediaPlayer(new QMediaPlayer(this)),
      m_gaussianBlur(new GaussianBlur(this)),
      m_outputSurface(new OutputVideoSurface(this)) {}

void VideoPlayer::playVideoFile(const QString &filepath) {
  auto sender = dynamic_cast<MainWindow *>(QObject::sender());
  auto widget = dynamic_cast<QLabel *>(sender->getVideoWidget());

  stopVideoFile();
  stopWebCamera();

  m_mediaPlayer->setVideoOutput(m_gaussianBlur);
  m_gaussianBlur->setVideoSurface(m_outputSurface);
  m_outputSurface->setOutputLabel(widget);

  m_mediaPlayer->setMedia(QUrl::fromLocalFile(filepath));
  m_mediaPlayer->play();
}

void VideoPlayer::playWebCamera() {
  auto sender = dynamic_cast<MainWindow *>(QObject::sender());
  auto widget = dynamic_cast<QLabel *>(sender->getVideoWidget());

  stopVideoFile();
  stopWebCamera();

  auto camera = getCamera();

  m_camera->setViewfinder(m_gaussianBlur);
  m_gaussianBlur->setVideoSurface(m_outputSurface);
  m_outputSurface->setOutputLabel(widget);

  m_camera->setCaptureMode(QCamera::CaptureVideo);
  m_camera->start();
}

QCamera *VideoPlayer::getCamera() {
  if (!m_camera) {
    auto cameraInfo = QCameraInfo::defaultCamera();
    m_camera = new QCamera(cameraInfo, this);
  }

  return m_camera;
}

void VideoPlayer::stopVideoFile() {
  m_mediaPlayer->stop();
}

void VideoPlayer::stopWebCamera() {
  if (!m_camera) return;

  m_camera->stop();
}

