#ifndef VIDEO_STREAM_SRC_VIDEO_WIDGET_SOBEL_OPERATOR_H_
#define VIDEO_STREAM_SRC_VIDEO_WIDGET_SOBEL_OPERATOR_H_

#include <QImage>
#include <QVideoFrame>
#include <QVector>

class SobelOperator : public QObject {
 Q_OBJECT
 public:
  explicit SobelOperator(QObject *parent = nullptr);

  QVideoFrame getLastFrame() const;

 signals:
  void signalNextFrameReady();
  void signalPassImage(QImage);

 public slots:
  bool receiveNextFrame();
  bool receiveImage(QImage);
  bool toggle();

 private:
  QVideoFrame m_lastSavedFrame;

  void applyOperator(const uchar *oldBytes, uchar *newBytes, int height, int width);

  const QVector<QVector<int>> m_kernelX;
  const QVector<QVector<int>> m_kernelY;

  bool active{false};
};

#endif //VIDEO_STREAM_SRC_VIDEO_WIDGET_SOBEL_OPERATOR_H_
