#include "gaussian_blur.h"

GaussianBlur::GaussianBlur(QObject *parent) : QAbstractVideoSurface(parent) {};

bool GaussianBlur::present(const QVideoFrame &frame) {
  // todo gaussian blur
  if (m_surface) m_surface->present(frame);

  return true;
}

QList<QVideoFrame::PixelFormat> GaussianBlur::supportedPixelFormats(
    QAbstractVideoBuffer::HandleType type) const {

  return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB32;
}

void GaussianBlur::setVideoSurface(QAbstractVideoSurface *surface) {
  if (m_surface && m_surface != surface && m_surface->isActive()) {
    m_surface->stop();
  }

  m_surface = surface;
  if (m_surface) {
    m_surface->start(m_format);
  }
}
