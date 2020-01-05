#ifndef VIDEO_STREAM_SRC_VIDEO_WIDGET_OUTPUT_VIDEO_SURFACE_H_
#define VIDEO_STREAM_SRC_VIDEO_WIDGET_OUTPUT_VIDEO_SURFACE_H_

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtMultimedia/QAbstractVideoSurface>

class OutputVideoSurface : public QAbstractVideoSurface {
 Q_OBJECT
 public:
  explicit OutputVideoSurface(QObject *parent);

  QList<QVideoFrame::PixelFormat> supportedPixelFormats(
      QAbstractVideoBuffer::HandleType handleType) const override;

  bool present(const QVideoFrame &frame) override;

  bool setOutputLabel(QLabel *label);

 private:
  QLabel *m_label{nullptr};
};

#endif //VIDEO_STREAM_SRC_VIDEO_WIDGET_OUTPUT_VIDEO_SURFACE_H_
