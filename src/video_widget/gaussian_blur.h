#ifndef VIDEO_STREAM_SRC_GAUSSIAN_BLUR_H_
#define VIDEO_STREAM_SRC_GAUSSIAN_BLUR_H_

#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>

class GaussianBlur : public QAbstractVideoSurface {
 Q_OBJECT
 public:
  explicit GaussianBlur(QObject *parent);

  bool present(const QVideoFrame &frame) override;
  void setVideoSurface(QAbstractVideoSurface *surface);

  QList<QVideoFrame::PixelFormat> supportedPixelFormats(
      QAbstractVideoBuffer::HandleType type) const override;
 private:

  QAbstractVideoSurface *m_surface{nullptr};
  QVideoSurfaceFormat m_format;

  static double gaussian(double m, double sigma, double x, double y);

  const double M = 1;
  const double SIGMA = 1;
  const int B_SIZE = 3;
  QVector<QVector<double>> matrix;
};

#endif //VIDEO_STREAM_SRC_GAUSSIAN_BLUR_H_
