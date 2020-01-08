#include "output_video_surface.h"

#include <QDebug>

OutputVideoSurface::OutputVideoSurface(QObject *parent) : QObject(parent) {}

bool OutputVideoSurface::receiveNextFrame(const QVideoFrame &frame) {
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

  toDraw.unmap();

  emit signalOutputImage(image);
  return true;
}
bool OutputVideoSurface::receiveImage(QImage) {
  return false;
}
