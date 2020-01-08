#include "sobel_operator.h"

#include <QDebug>

SobelOperator::SobelOperator(QObject *parent)
    : QObject(parent),
      m_kernelX{{-1, 0, 1},
                {-2, 0, 2},
                {-1, 0, 1}},
      m_kernelY{{-1, -2, -1},
                {0, 0, 0},
                {1, 2, 1}} {}

bool SobelOperator::receiveNextFrame(const QVideoFrame &frame) {
  emit signalNextFrame(frame);
  return false;
}

bool SobelOperator::receiveImage(QImage image) {
  emit signalPassImage(image);
  return false;
}

bool SobelOperator::toggle() {
  active = !active;
  qDebug() << "Sobel operator turned " << (active ? "on" : "off");
  return active;
}
