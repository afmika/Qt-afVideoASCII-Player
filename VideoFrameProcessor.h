#ifndef VIDEOFRAMEPROCESSOR_H
#define VIDEOFRAMEPROCESSOR_H
#include <QGraphicsVideoItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QThread>

class VideoFrameProcessor : public QGraphicsView
{
    Q_OBJECT
public:
    VideoFrameProcessor(qreal image_factor = 0.25) {
        this->image_factor = image_factor;
        video_output = new QGraphicsVideoItem;
        QGraphicsScene *scene = new QGraphicsScene;
        scene->addItem(video_output);    
        setScene(scene);
    }
    
    QGraphicsVideoItem* output() {
        return video_output;
    }
    
    void screenshot () {
        if ( current_frame != nullptr ) {
            delete  current_frame;
        }
        QPixmap pixmap = grab();
        QImage temp = pixmap.toImage();
        current_frame = new QImage(temp.scaled(temp.size() * image_factor));
        emit newFrameSignal(current_frame);
    }
    
private:
    QImage* current_frame = nullptr;
    qreal image_factor = 0.25;
    QGraphicsVideoItem* video_output = nullptr;

signals:
    void newFrameSignal(QImage* frame);
};

#endif // VIDEOFRAMEPROCESSOR_H
