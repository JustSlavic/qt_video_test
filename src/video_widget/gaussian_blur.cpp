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
    : QAbstractVideoSurface(parent), matrix(B_SIZE, QVector<double>(B_SIZE, 0)) {

  for (int i = 0; i < B_SIZE; ++i) {
    for (int j = 0; j < B_SIZE; ++j) {
      matrix[i][j] = gaussian(M, SIGMA, static_cast<double>(i), static_cast<double>(j));
    }
  }
}

bool GaussianBlur::present(const QVideoFrame &frame) {
  QElapsedTimer timer;
  timer.start();

  QVideoFrame forMapping(frame);

  if (!forMapping.map(QAbstractVideoBuffer::ReadOnly)) {
    setError(ResourceError);
    qDebug() << "Cannot map forMapping frame";
    return false;
  }

  QVideoFrame newFrame(
      forMapping.mappedBytes(),
      forMapping.size(),
      forMapping.bytesPerLine(),
      forMapping.pixelFormat());

  if (!newFrame.map(QAbstractPlanarVideoBuffer::WriteOnly)) {
    setError(ResourceError);
    qDebug() << "Cannot map newly created frame";
    return false;
  }

  uchar *oldBytes = forMapping.bits();
  uchar *newBytes = newFrame.bits();

//  std::memcpy(newBytes, oldBytes, forMapping.mappedBytes());

  int bytesOverall = forMapping.mappedBytes();
  int bytesPerLine = forMapping.bytesPerLine();
  int height = forMapping.height();
  int width = forMapping.width();

  for (int i = 1; i < forMapping.height() - 1; ++i) {
    for (int j = 1; j < forMapping.width() - 1; ++j) {
      double averageA = 0;
      double averageR = 0;
      double averageG = 0;
      double averageB = 0;

      for (int s = 0; s < B_SIZE; ++s) {
        for (int t = 0; t < B_SIZE; ++t) {
          averageA += matrix[s][t] * *(oldBytes + (i + s - 1)*bytesPerLine + (j + t - 1)*4 + A_SHIFT);
          averageR += matrix[s][t] * *(oldBytes + (i + s - 1)*bytesPerLine + (j + t - 1)*4 + R_SHIFT);
          averageG += matrix[s][t] * *(oldBytes + (i + s - 1)*bytesPerLine + (j + t - 1)*4 + G_SHIFT);
          averageB += matrix[s][t] * *(oldBytes + (i + s - 1)*bytesPerLine + (j + t - 1)*4 + B_SHIFT);

//          averageA = *(oldBytes + i*bytesPerLine + j*4 + A_SHIFT);
//          averageR = *(oldBytes + i*bytesPerLine + j*4 + R_SHIFT);
//          averageG = *(oldBytes + i*bytesPerLine + j*4 + G_SHIFT);
//          averageB = *(oldBytes + i*bytesPerLine + j*4 + B_SHIFT);
        }
      }

      *(newBytes + i*bytesPerLine + j*4 + A_SHIFT) = static_cast<uchar>(averageA);

//      qDebug() << "old: ("
//        << (i*bytesPerLine + j*4 + R_SHIFT) << ", "
//        << (i*bytesPerLine + j*4 + G_SHIFT) << ", "
//        << (i*bytesPerLine + j*4 + B_SHIFT) << ") "
//        << " new: ("
//        << static_cast<uchar>(averageR) << ", "
//        << static_cast<uchar>(averageG) << ", "
//        << static_cast<uchar>(averageB) << ") ";

      *(newBytes + i*bytesPerLine + j*4 + R_SHIFT) = static_cast<uchar>(averageR);
      *(newBytes + i*bytesPerLine + j*4 + G_SHIFT) = static_cast<uchar>(averageG);
      *(newBytes + i*bytesPerLine + j*4 + B_SHIFT) = static_cast<uchar>(averageB);
    }
  }

//  QThread::msleep(500);

  forMapping.unmap();
  newFrame.unmap();

  qDebug() << "The gaussian blur took" << timer.elapsed() << " milliseconds";

  if (m_surface) m_surface->present(newFrame);

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

double GaussianBlur::gaussian(double m, double sigma, double x, double y) {
  double exponential = -((x - m) * (x - m) + (y - m) * (y - m)) / (2 * sigma * sigma);
  double normalize_coefficient = 2 * M_PI * sigma * sigma;

  return std::exp(exponential) / normalize_coefficient;
}
