#ifndef VIDEO_STREAM_SRC_VIDEO_WIDGET_SOBEL_OPERATOR_H_
#define VIDEO_STREAM_SRC_VIDEO_WIDGET_SOBEL_OPERATOR_H_

#include <QImage>
#include <QVideoFrame>
#include <QVector>

class SobelOperator : public QObject {
 Q_OBJECT
 public:
  explicit SobelOperator(QObject *parent = nullptr);

 signals:
  void signalNextFrame(const QVideoFrame &);
  void signalPassImage(QImage);

 public slots:
  bool receiveNextFrame(const QVideoFrame &);
  bool receiveImage(QImage);
  bool toggle();

 private:
  QVector<QVector<int>> m_kernelX;
  QVector<QVector<int>> m_kernelY;
  QVector<QVector<double>> G;

  bool active{false};
};

#endif //VIDEO_STREAM_SRC_VIDEO_WIDGET_SOBEL_OPERATOR_H_
