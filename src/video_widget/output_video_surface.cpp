#include "output_video_surface.h"

#include <QDebug>

OutputVideoSurface::OutputVideoSurface(QObject *parent) : QAbstractVideoSurface(parent) {}

QList<QVideoFrame::PixelFormat> OutputVideoSurface::supportedPixelFormats(
    QAbstractVideoBuffer::HandleType handleType) const {

  if (handleType == QAbstractVideoBuffer::NoHandle) {
    return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB32;
  } else {
    return QList<QVideoFrame::PixelFormat>();
  }
}

bool OutputVideoSurface::present(const QVideoFrame &frame) {
  if (!m_label) {
    qDebug() << "The label is not set!";
    return false;
  }

  QVideoFrame toDraw(frame);

  if (!toDraw.map(QAbstractVideoBuffer::ReadOnly)) {
    qDebug() << "Mapping toDraw frame to readonly goes wrong";
    return false;
  }

  QImage image(
      toDraw.bits(),
      toDraw.width(),
      toDraw.height(),
      toDraw.bytesPerLine(),
      QImage::Format_RGB32
  );

  m_label->resize(image.size());
  m_label->setPixmap(QPixmap::fromImage(image));
  m_label->update();

  toDraw.unmap();

  return true;
}

bool OutputVideoSurface::setOutputLabel(QLabel *label) {
  if (!label) {
    qDebug() << "Try to set nullptr instead of label";
    return false;
  }

  m_label = label;
  return true;
}
