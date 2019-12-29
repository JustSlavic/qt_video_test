#include "video_player.h"
#include "main_window.h"

#include <QCameraInfo>

VideoPlayer::VideoPlayer()
    : m_mediaPlayer(new QMediaPlayer(this)),
      m_gaussianBlur(new GaussianBlur(this)) {}

void VideoPlayer::playVideoFile(const QString &filepath) {
  auto sender = dynamic_cast<MainWindow *>(QObject::sender());
  auto widget = sender->getVideoWidget();

  stopVideoFile();
  stopWebCamera();

  m_mediaPlayer->setVideoOutput(widget);
  m_mediaPlayer->setMedia(QUrl::fromLocalFile(filepath));
  m_mediaPlayer->play();
}

void VideoPlayer::playWebCamera() {
  auto sender = dynamic_cast<MainWindow *>(QObject::sender());
  auto widget = sender->getVideoWidget();

  stopVideoFile();
  stopWebCamera();

  auto camera = getCamera();

  m_camera->setViewfinder(widget);
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

