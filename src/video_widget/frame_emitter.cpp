#include "frame_emitter.h"

FrameEmitter::FrameEmitter(QObject *parent)
  : QAbstractVideoSurface(parent) {}

bool FrameEmitter::present(const QVideoFrame &frame) {
  m_lastSavedFrame = frame;
  emit signalNextFrameReady();
  return true;
}

QList<QVideoFrame::PixelFormat> FrameEmitter::supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const {
  return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB32;
}

QVideoFrame FrameEmitter::getLastFrame() const {
  return m_lastSavedFrame;
}
