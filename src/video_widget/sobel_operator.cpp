#include "sobel_operator.h"
#include "gaussian_blur.h"

#include <cstring>
#include <QDebug>
#include <cmath>

enum ColorShift {
  B_SHIFT = 0,
  G_SHIFT = 1,
  R_SHIFT = 2,
  A_SHIFT = 3
};

SobelOperator::SobelOperator(QObject *parent)
    : QObject(parent),
      m_kernelX{{-1, 0, 1},
                {-2, 0, 2},
                {-1, 0, 1}},
      m_kernelY{{-1, -2, -1},
                {0, 0, 0},
                {1, 2, 1}} {}

bool SobelOperator::receiveNextFrame() {
  auto sender = dynamic_cast<GaussianBlur *>(QObject::sender());
  QVideoFrame frame = sender->getLastFrame();

  if (!active) {
    m_lastSavedFrame = frame;
    emit signalNextFrameReady();
    return true;
  }

  if (!frame.map(QAbstractVideoBuffer::ReadOnly)) {
    qDebug() << "Cannot map forMapping frame";
    return false;
  }

  QVideoFrame newFrame(
      frame.mappedBytes(),
      frame.size(),
      frame.bytesPerLine(),
      frame.pixelFormat());

  if (!newFrame.map(QAbstractPlanarVideoBuffer::WriteOnly)) {
    qDebug() << "Cannot map newly created frame";
    return false;
  }

  uchar *oldBytes = frame.bits();
  uchar *newBytes = newFrame.bits();

  std::memcpy(newBytes, oldBytes, frame.mappedBytes());

  int height = frame.height();
  int width = frame.width();

  applyOperator(oldBytes, newBytes, height, width);

  frame.unmap();
  newFrame.unmap();

  m_lastSavedFrame = newFrame;
  emit signalNextFrameReady();
  return true;
}

bool SobelOperator::receiveImage(QImage oldImage) {
  if (!active) {
    emit signalPassImage(oldImage);
    return true;
  }

  QImage newImage(
      oldImage.width(),
      oldImage.height(),
      oldImage.format());

  uchar *oldBytes = oldImage.bits();
  uchar *newBytes = newImage.bits();

  int byteCount = oldImage.byteCount();
  int width = oldImage.width();
  int height = oldImage.height();

  std::memcpy(newBytes, oldBytes, byteCount);

  applyOperator(oldBytes, newBytes, height, width);

  emit signalPassImage(newImage);
  return true;
}

void SobelOperator::applyOperator(const uchar *oldBytes, uchar *newBytes, int height, int width) {
  int bytesPerLine = width * 4;

  int boundary = 1;
  for (int i = boundary; i < height - boundary; ++i) {
    for (int j = boundary; j < width - boundary; ++j) {
      int tmpR_Gx = 0, tmpR_Gy = 0;
      int tmpG_Gx = 0, tmpG_Gy = 0;
      int tmpB_Gx = 0, tmpB_Gy = 0;

      for (int s = 0; s < 3; ++s) {
        for (int t = 0; t < 3; ++t) {
          tmpR_Gx +=
              m_kernelX[s][t] * *(oldBytes + (i + s - boundary) * bytesPerLine + (j + t - boundary) * 4 + R_SHIFT);
          tmpG_Gx +=
              m_kernelX[s][t] * *(oldBytes + (i + s - boundary) * bytesPerLine + (j + t - boundary) * 4 + G_SHIFT);
          tmpB_Gx +=
              m_kernelX[s][t] * *(oldBytes + (i + s - boundary) * bytesPerLine + (j + t - boundary) * 4 + B_SHIFT);

          tmpR_Gy +=
              m_kernelY[s][t] * *(oldBytes + (i + s - boundary) * bytesPerLine + (j + t - boundary) * 4 + R_SHIFT);
          tmpG_Gy +=
              m_kernelY[s][t] * *(oldBytes + (i + s - boundary) * bytesPerLine + (j + t - boundary) * 4 + G_SHIFT);
          tmpB_Gy +=
              m_kernelY[s][t] * *(oldBytes + (i + s - boundary) * bytesPerLine + (j + t - boundary) * 4 + B_SHIFT);
        }
      }

      *(newBytes + i * bytesPerLine + j * 4 + R_SHIFT) = std::sqrt(tmpR_Gx * tmpR_Gx + tmpR_Gy * tmpR_Gy);
      *(newBytes + i * bytesPerLine + j * 4 + G_SHIFT) = std::sqrt(tmpG_Gx * tmpG_Gx + tmpG_Gy * tmpG_Gy);
      *(newBytes + i * bytesPerLine + j * 4 + B_SHIFT) = std::sqrt(tmpB_Gx * tmpB_Gx + tmpB_Gy * tmpB_Gy);
    }
  }
}

bool SobelOperator::toggle() {
  active = !active;
  qDebug() << "Sobel operator turned " << (active ? "on" : "off");
  return active;
}

QVideoFrame SobelOperator::getLastFrame() const {
  return m_lastSavedFrame;
}
