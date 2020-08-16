#include "afvideoplayer.h"
#include <QPushButton>
#include <QAbstractButton>
#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>
#include <QStyle>
#include <QLabel>
#include <map>
#include <QGraphicsScene>

#include "ASCIIVideoConverter.h"
static std::map<QString, QString> charset_map {
    {"STANDARD", ASCIIVideoConverter::CHARS::STANDARD},
    {"GRAY", ASCIIVideoConverter::CHARS::GRAY},
    {"GRAY MIN", ASCIIVideoConverter::CHARS::STD_MIN},
    {"GRAY LV 0", ASCIIVideoConverter::CHARS::GRAY_0},
    {"GRAY LV 1", ASCIIVideoConverter::CHARS::GRAY_1},
    {"GRAY LV 2", ASCIIVideoConverter::CHARS::GRAY_2}
};

afVideoPlayer::afVideoPlayer(QWidget* parent) : QWidget(parent)
{
    // player setup
    // QVideoWidget* video_widget = new QVideoWidget(this);
    video_processor_view = new VideoFrameProcessor(0.25);
            
    media_player = new QMediaPlayer;
    media_player->setVideoOutput(video_processor_view->output());
    
    ascii_result = new QTextEdit;
    QFont font;
    ascii_result->setLineWrapMode(QTextEdit::NoWrap);
    ascii_result->resize(100, 200);  
    
    // controls
    play_btn = new QPushButton("PLAY");
    open_file = new QPushButton("Open");
    charset_select = new QComboBox;
    for (auto v : charset_map ) {
        charset_select->addItem(v.first);
    }
    current_charset = charset_map[ charset_select->itemText(0) ];
    
    slider = new QSlider();
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0, 200);
    
    volume = new QSlider();
    volume->setOrientation(Qt::Horizontal);
    volume->setRange(0, 100);
    volume->setValue(10);
    volume->setToolTip( QString("Volume %1").arg(volume->value()) );
    
    zoom_slider = new QSlider();
    zoom_slider->setOrientation(Qt::Horizontal);
    zoom_slider->setRange(0, 150);
    zoom_slider->setValue(70);
    zoom_slider->setToolTip( QString("Zoom %1").arg(zoom_slider->value()));
    
    // sizes
    // video_widget->setMinimumSize(400, 360);
    video_processor_view->setMinimumSize(50, 50);
    
    // ascii_result->setMinimumSize(500, 360);
    
    // main layout
    QGridLayout* control_layout = new QGridLayout;
    control_layout->setColumnStretch(8, 1);
    QLabel* volume_label = new QLabel("Volume");
    QLabel* text_size_label = new QLabel("Text size");    
    QLabel* charset_label = new QLabel("ASCII Charset");        
    control_layout->addWidget(volume_label);
    control_layout->addWidget(volume);
    control_layout->addWidget(open_file);
    control_layout->addWidget(play_btn);
    control_layout->addWidget(charset_label);
    control_layout->addWidget(charset_select);
    control_layout->addWidget(text_size_label);
    control_layout->addWidget(zoom_slider);
    
    
    
    QGridLayout* output_layout = new QGridLayout;
    output_layout->setColumnStretch(2, 0);
    // output_layout->addWidget(video_widget);
    output_layout->addWidget(video_processor_view);
    output_layout->addWidget(ascii_result);
    
    QVBoxLayout* global_layout = new QVBoxLayout;       
    global_layout->addLayout(output_layout);
    global_layout->addWidget(slider);
    global_layout->addLayout(control_layout);
    
    setLayout(global_layout);
    
    // events
    
    // controls
    connect(open_file, SIGNAL(clicked()), SLOT(onSelectFile()));
    connect(play_btn, SIGNAL(clicked()), SLOT(onButtonClicked()));
    connect(slider, SIGNAL(sliderReleased()), SLOT(onSliderValueChanged()));
    connect(volume, SIGNAL(valueChanged(int)), SLOT(onVolumeChanged(int)));
    connect(zoom_slider, SIGNAL(sliderReleased()), SLOT(onZoom()));
    connect(charset_select, SIGNAL(currentIndexChanged(const QString&)), SLOT(onCharSetChange(const QString&)));
    // media signals
    connect(media_player, SIGNAL(stateChanged(QMediaPlayer::State)), SLOT(onMediaStateChanged(QMediaPlayer::State)));
    connect(media_player, SIGNAL(positionChanged(qint64)), SLOT(onMediaPositionChanged(qint64)));
    connect(media_player, SIGNAL(durationChanged(qint64)), SLOT(onMediaDurationChanged(qint64)));
    connect(media_player, SIGNAL(error(QMediaPlayer::Error)), SLOT(onError(QMediaPlayer::Error)));
    connect(video_processor_view, SIGNAL(newFrameSignal(QImage*)), SLOT(onNewFrame(QImage*)));

    // main window
    resize(1000, 500);    
    setWindowTitle(tr("afASCII Video Player"));
}

bool afVideoPlayer::selectFile()
{   
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open a video"));
    
    QStringList supportedMimeTypes = media_player->supportedMimeTypes();
    if (!supportedMimeTypes.isEmpty()) {
        fileDialog.setMimeTypeFilters(supportedMimeTypes);    
    }
    
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    
    if (fileDialog.exec() == QDialog::Accepted) {
        video_path = fileDialog.selectedUrls().constFirst();
        return true;
    }
    return false;
}

void afVideoPlayer::switchPlayState()
{
    QMediaPlayer::State state = media_player->state();
    if ( state == QMediaPlayer::PausedState || state == QMediaPlayer::StoppedState ) {
        qDebug() << "playing...";
        media_player->play();
    } else {
        qDebug() << "paused...";        
        media_player->pause();
    }
}


void afVideoPlayer::onNextFrame()
{
    
}

void afVideoPlayer::onSelectFile()
{
    if (selectFile()) {
        play_btn->setEnabled(true);        
        qDebug() << video_path;
        media_player->setMedia(video_path);
        media_player->setVolume( volume->value() );
        switchPlayState(); // play
    } else {
        qDebug() << "No File selected";
    }
}

void afVideoPlayer::onButtonClicked()
{
    if (!video_path.isEmpty()) {
       switchPlayState();
    }    
}

void afVideoPlayer::onSliderValueChanged()
{
    media_player->setPosition(slider->value());
}

void afVideoPlayer::onVolumeChanged(int value)
{
    media_player->setVolume( value );
    volume->setToolTip( QString("Volume %1").arg(value) );
}

void afVideoPlayer::onZoom()
{
    int value = zoom_slider->value();
    zoom_slider->setToolTip(QString("Zoom %1").arg(value));
    int px = value / 10;
    QFont font;
    font.setFamily("consolas");
    font.setPixelSize(px);
    ascii_result->setFont(font);
}

void afVideoPlayer::onCharSetChange(const QString& text)
{
    current_charset = charset_map[ text ];
}

void afVideoPlayer::onMediaStateChanged(QMediaPlayer::State state)
{
    if ( state == QMediaPlayer::PausedState || state == QMediaPlayer::StoppedState ) {
        play_btn->setText("PLAY");
    } else {
        play_btn->setText("PAUSE");
        video_processor_view->screenshot();
    }    
}

void afVideoPlayer::onMediaPositionChanged(qint64 position)
{
    video_processor_view->screenshot();    
    slider->setValue(static_cast<int>(position));
}

void afVideoPlayer::onMediaDurationChanged(qint64 duration)
{
    slider->setRange(0, static_cast<int>(duration));
}

void afVideoPlayer::onError(QMediaPlayer::Error error)
{
    play_btn->setEnabled(false);
    QString message = media_player->errorString();
    if (message.isEmpty()) {
        message = "Error " + QString::number(int(media_player->error()));    
    }
    qDebug() << message << " # " << error;
}

void afVideoPlayer::onNewFrame(QImage *frame)
{
    // qDebug() << *frame;
    QString result = ASCIIVideoConverter::convertFrame(frame, current_charset);
    ascii_result->setPlainText(result);    
    // qDebug() << "New frame";
}
