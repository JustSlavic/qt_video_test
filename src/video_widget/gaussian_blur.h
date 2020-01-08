#ifndef VIDEO_STREAM_SRC_GAUSSIAN_BLUR_H_
#define VIDEO_STREAM_SRC_GAUSSIAN_BLUR_H_

#include <QVideoSurfaceFormat>

class GaussianBlur : public QObject {
 Q_OBJECT
 public:
  explicit GaussianBlur(QObject *parent = nullptr);

 signals:
  void signalNextFrame(const QVideoFrame &);
  void signalPassImage(QImage);

 public slots:
  bool receiveNextFrame(const QVideoFrame &);
  bool receiveImage(QImage);
  bool toggle();

 private:
  QVideoSurfaceFormat m_format;

  static double gaussian(double m, double sigma, double x);
  static double gaussian(double m, double sigma, double x, double y);

  void blur(const uchar *oldBytes, uchar *newBytes, int height, int width);

  const double M = 4;
  const double SIGMA = 2;
  const int KERNEL_SIZE = 9;
  QVector<double> kernel;

  bool active{false};
};

#endif //VIDEO_STREAM_SRC_GAUSSIAN_BLUR_H_
