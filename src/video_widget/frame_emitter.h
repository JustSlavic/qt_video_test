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

  QVideoFrame getLastFrame() const;

 signals:
  void signalPassImage(QImage);
  void signalNextFrameReady();

 private:
  QVideoFrame m_lastSavedFrame;
};

#endif //VIDEO_STREAM_SRC_VIDEO_WIDGET_FRAME_EMITTER_H_
