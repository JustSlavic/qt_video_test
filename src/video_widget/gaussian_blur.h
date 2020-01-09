#ifndef VIDEO_STREAM_SRC_GAUSSIAN_BLUR_H_
#define VIDEO_STREAM_SRC_GAUSSIAN_BLUR_H_

#define _USE_MATH_DEFINES
#include <cmath>
#include <QVideoSurfaceFormat>

class GaussianBlur : public QObject {
 Q_OBJECT
 public:
  explicit GaussianBlur(double sigma, QObject *parent = nullptr);

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

  const double SIGMA;
  const int KERNEL_SIZE;
  const int M;
  QVector<double> kernel;

  bool active{false};
};

#endif //VIDEO_STREAM_SRC_GAUSSIAN_BLUR_H_
