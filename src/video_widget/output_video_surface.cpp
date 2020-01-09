#include "output_video_surface.h"
#include "sobel_operator.h"

#include <QDebug>

OutputVideoSurface::OutputVideoSurface(QObject *parent) : QObject(parent) {}

bool OutputVideoSurface::receiveNextFrame() {
  auto sender = dynamic_cast<SobelOperator *>(QObject::sender());
  QVideoFrame frame = sender->getLastFrame();

  if (!frame.map(QAbstractVideoBuffer::ReadOnly)) {
    qDebug() << "Mapping toDraw frame to readonly goes wrong";
    return false;
  }

  QImage image(
      frame.bits(),
      frame.width(),
      frame.height(),
      frame.bytesPerLine(),
      QImage::Format_RGB32
  );

  frame.unmap();

  emit signalOutputImage(image);
  return true;
}

bool OutputVideoSurface::receiveImage(QImage) {
  return false;
}
