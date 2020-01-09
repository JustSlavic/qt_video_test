#include "gaussian_blur.h"
#include "frame_emitter.h"

#include <iostream>
#include <cmath>
#include <QGenericMatrix>
#include <QThread>
#include <cstring>
#include <QtCore/QElapsedTimer>

enum ColorShift {
  B_SHIFT = 0,
  G_SHIFT = 1,
  R_SHIFT = 2,
  A_SHIFT = 3
};

GaussianBlur::GaussianBlur(double standardDeviation, QObject *parent)
    : QObject(parent),
      m_standardDeviation(standardDeviation),
      m_kernelSize(6*static_cast<int>(std::floor(standardDeviation)) + 1),
      m_expectedValue(m_kernelSize / 2),
      m_kernel(m_kernelSize, 0) {

  for (int i = 0; i < m_kernelSize; ++i) {
    m_kernel[i] = gaussian(m_expectedValue, m_standardDeviation, static_cast<double>(i));
  }

  std::cerr << "kernel: [";
  for (int i = 0; i < m_kernelSize; ++i) {
    std::cerr << m_kernel[i] << " ";
  }
  std::cerr << "]" << std::endl;
}

bool GaussianBlur::receiveNextFrame() {
  auto sender = dynamic_cast<FrameEmitter *>(QObject::sender());
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

  blur(oldBytes, newBytes, height, width);

  frame.unmap();
  newFrame.unmap();

  m_lastSavedFrame = newFrame;
  emit signalNextFrameReady();
  return true;
}

void GaussianBlur::blur(const uchar *oldBytes, uchar *newBytes, int height, int width) {
  int bytesPerLine = width * 4;

  int boundary = (m_kernelSize - 1) / 2;
  for (int i = 0; i < height; ++i) {
    for (int j = boundary; j < width - boundary; ++j) {
      double averageR = 0;
      double averageG = 0;
      double averageB = 0;

      for (int s = 0; s < m_kernelSize; ++s) {
        averageR += m_kernel[s] * *(oldBytes + i * bytesPerLine + (j + s - boundary) * 4 + R_SHIFT);
        averageG += m_kernel[s] * *(oldBytes + i * bytesPerLine + (j + s - boundary) * 4 + G_SHIFT);
        averageB += m_kernel[s] * *(oldBytes + i * bytesPerLine + (j + s - boundary) * 4 + B_SHIFT);
      }

      *(newBytes + i * bytesPerLine + j * 4 + R_SHIFT) = static_cast<uchar>(averageR);
      *(newBytes + i * bytesPerLine + j * 4 + G_SHIFT) = static_cast<uchar>(averageG);
      *(newBytes + i * bytesPerLine + j * 4 + B_SHIFT) = static_cast<uchar>(averageB);
    }
  }

  for (int j = 0; j < width; ++j) {
    for (int i = boundary; i < height - boundary; ++i) {
      double averageR = 0;
      double averageG = 0;
      double averageB = 0;

      for (int s = 0; s < m_kernelSize; ++s) {
        averageR += m_kernel[s] * *(newBytes + (i + s - boundary) * bytesPerLine + j * 4 + R_SHIFT);
        averageG += m_kernel[s] * *(newBytes + (i + s - boundary) * bytesPerLine + j * 4 + G_SHIFT);
        averageB += m_kernel[s] * *(newBytes + (i + s - boundary) * bytesPerLine + j * 4 + B_SHIFT);
      }

      *(newBytes + i * bytesPerLine + j * 4 + R_SHIFT) = static_cast<uchar>(averageR);
      *(newBytes + i * bytesPerLine + j * 4 + G_SHIFT) = static_cast<uchar>(averageG);
      *(newBytes + i * bytesPerLine + j * 4 + B_SHIFT) = static_cast<uchar>(averageB);
    }
  }
}

double GaussianBlur::gaussian(double m, double sigma, double x) {
  double exponential = -((x - m) * (x - m) / (2 * sigma * sigma));
  double normalize_coefficient = std::sqrt(2 * M_PI) * sigma;
  return std::exp(exponential) / normalize_coefficient;
}

double GaussianBlur::gaussian(double m, double sigma, double x, double y) {
  double exponential = -((x - m) * (x - m) + (y - m) * (y - m)) / (2 * sigma * sigma);
  double normalize_coefficient = 2 * M_PI * sigma * sigma;

  return std::exp(exponential) / normalize_coefficient;
}

bool GaussianBlur::toggle() {
  active = !active;
  qDebug() << "Gaussian blur turned " << (active ? "on" : "off");
  return active;
}

bool GaussianBlur::receiveImage(QImage oldImage) {
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

  blur(oldBytes, newBytes, height, width);

  emit signalPassImage(newImage);
  return true;
}

QVideoFrame GaussianBlur::getLastFrame() const {
  return m_lastSavedFrame;
}
