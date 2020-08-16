#ifndef AFVIDEOPLAYER_H
#define AFVIDEOPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QPushButton>
#include <QSlider>
#include <QTextEdit>
#include <QComboBox>
#include <QGraphicsView>
#include <QGraphicsVideoItem>
#include "VideoFrameProcessor.h"

class afVideoPlayer : public QWidget
{
Q_OBJECT
public:
    afVideoPlayer(QWidget* parent = nullptr);
    
    bool selectFile(); // open dialog
    void loadFile();
    void switchPlayState();
public slots:
    void onNextFrame();
    void onSelectFile();
    void onButtonClicked();
    void onSliderValueChanged();
    void onVolumeChanged(int value);
    void onZoom();
    void onCharSetChange(const QString& text);

    void onMediaStateChanged(QMediaPlayer::State state);
    void onMediaPositionChanged(qint64 position);
    void onMediaDurationChanged(qint64 duration);
    void onError(QMediaPlayer::Error error);
    void onNewFrame(QImage* frame);
public:
    VideoFrameProcessor* video_processor_view = nullptr;
    QMediaPlayer* media_player = nullptr;
    QPushButton* play_btn = nullptr;
    QPushButton* open_file = nullptr;
    QComboBox* charset_select = nullptr;
    QSlider* slider = nullptr;
    QSlider* volume = nullptr;
    QSlider* zoom_slider = nullptr;
    QTextEdit* ascii_result = nullptr;
    
    QUrl video_path;
    QString current_charset;
};

#endif // AFVIDEOPLAYER_H
