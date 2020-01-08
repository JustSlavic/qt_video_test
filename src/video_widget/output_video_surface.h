#ifndef VIDEO_STREAM_SRC_VIDEO_WIDGET_OUTPUT_VIDEO_SURFACE_H_
#define VIDEO_STREAM_SRC_VIDEO_WIDGET_OUTPUT_VIDEO_SURFACE_H_

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtMultimedia/QVideoFrame>

class OutputVideoSurface : public QObject {
 Q_OBJECT
 public:
  explicit OutputVideoSurface(QObject *parent = nullptr);

 signals:
  void signalOutputImage(QImage);

 public slots:
  bool receiveNextFrame(const QVideoFrame &frame);
};

#endif //VIDEO_STREAM_SRC_VIDEO_WIDGET_OUTPUT_VIDEO_SURFACE_H_
