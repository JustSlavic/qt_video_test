#ifndef VIDEO_STREAM_SRC_GAUSSIAN_BLUR_H_
#define VIDEO_STREAM_SRC_GAUSSIAN_BLUR_H_

#define _USE_MATH_DEFINES
#include <cmath>
#include <QVideoSurfaceFormat>

class GaussianBlur : public QObject {
 Q_OBJECT
 public:
  explicit GaussianBlur(QObject *parent = nullptr);

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
