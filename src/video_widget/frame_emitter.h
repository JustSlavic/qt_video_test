#ifndef VIDEO_STREAM_SRC_VIDEO_WIDGET_FRAME_EMITTER_H_
#define VIDEO_STREAM_SRC_VIDEO_WIDGET_FRAME_EMITTER_H_

#include <QAbstractVideoSurface>

class FrameEmitter : public QAbstractVideoSurface {
 Q_OBJECT
 public:
  explicit FrameEmitter(QObject *parent);

  QList<QVideoFrame::PixelFormat> supportedPixelFormats(
      QAbstractVideoBuffer::HandleType type) const override;

  bool present(const QVideoFrame &frame) override;

 signals:
  void signalPassImage(QImage);
  void signalNextFrame(const QVideoFrame &);
};

#endif //VIDEO_STREAM_SRC_VIDEO_WIDGET_FRAME_EMITTER_H_
