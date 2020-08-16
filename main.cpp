#include <QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QLabel>
#include "afvideoplayer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    afVideoPlayer player;
    player.show();
    return app.exec();
}
