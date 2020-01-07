#include "frame_emitter.h"

FrameEmitter::FrameEmitter(QObject *parent)
  : QAbstractVideoSurface(parent) {}

bool FrameEmitter::present(const QVideoFrame &frame) {
  emit signalNextFrame(frame);
  return true;
}

QList<QVideoFrame::PixelFormat> FrameEmitter::supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const {
  return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB32;
}
