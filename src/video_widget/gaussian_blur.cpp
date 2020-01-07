#include "gaussian_blur.h"

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

GaussianBlur::GaussianBlur(QObject *parent)
    : QObject(parent), kernel(KERNEL_SIZE, 0) {

  for (int i = 0; i < KERNEL_SIZE; ++i) {
    kernel[i] = gaussian(M, SIGMA, static_cast<double>(i));
  }

  std::cerr << "kernel: [";
  for (int i = 0; i < KERNEL_SIZE; ++i) {
    std::cerr << kernel[i] << " ";
  }
  std::cerr << "]" << std::endl;
}

bool GaussianBlur::receiveNextFrame(const QVideoFrame &frame) {
  QVideoFrame forMapping(frame);

  QVideoFrame newFrame(
      forMapping.mappedBytes(),
      forMapping.size(),
      forMapping.bytesPerLine(),
      forMapping.pixelFormat());

  if (!newFrame.map(QAbstractPlanarVideoBuffer::WriteOnly)) {
    qDebug() << "Cannot map newly created frame";
    return false;
  }

  uchar *oldBytes = forMapping.bits();
  uchar *newBytes = newFrame.bits();

  std::memcpy(newBytes, oldBytes, forMapping.mappedBytes());

  int bytesOverall = forMapping.mappedBytes();
  int bytesPerLine = forMapping.bytesPerLine();
  int height = forMapping.height();
  int width = forMapping.width();

  int boundary = (KERNEL_SIZE - 1)/2;
  for (int i = 0; i < forMapping.height(); ++i) {
    for (int j = boundary; j < forMapping.width() - boundary; ++j) {
      double averageA = 0;
      double averageR = 0;
      double averageG = 0;
      double averageB = 0;

      for (int s = 0; s < KERNEL_SIZE; ++s) {
        averageA += kernel[s] * *(oldBytes + i*bytesPerLine + (j + s - 1)*4 + A_SHIFT);
        averageR += kernel[s] * *(oldBytes + i*bytesPerLine + (j + s - 1)*4 + R_SHIFT);
        averageG += kernel[s] * *(oldBytes + i*bytesPerLine + (j + s - 1)*4 + G_SHIFT);
        averageB += kernel[s] * *(oldBytes + i*bytesPerLine + (j + s - 1)*4 + B_SHIFT);
      }

      *(newBytes + i*bytesPerLine + j*4 + A_SHIFT) = static_cast<uchar>(averageA);
      *(newBytes + i*bytesPerLine + j*4 + R_SHIFT) = static_cast<uchar>(averageR);
      *(newBytes + i*bytesPerLine + j*4 + G_SHIFT) = static_cast<uchar>(averageG);
      *(newBytes + i*bytesPerLine + j*4 + B_SHIFT) = static_cast<uchar>(averageB);
    }
  }

  forMapping.unmap();
  newFrame.unmap();

  emit signalNextFrame(newFrame);
  return true;
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
