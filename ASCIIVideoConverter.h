#ifndef ASCIIVIDEOCONVERTER_H
#define ASCIIVIDEOCONVERTER_H

#include <QString>
#include <QColor>
#include <QFile>
#include <QImage>
#include <QDebug>
#include <QProgressBar>

namespace ASCIIVideoConverter {
    namespace CHARS {
        static QString GRAY_0 = "$$PPXX00wwooIIccvv::++oooooooooooooooooooooo";
        static QString GRAY_1 = "XX00wwooIIccvv::++!!~~::::::::::::::::::::::";
        static QString GRAY_2 = "wooIIccvv::++!!~~""..,......................";
        static QString GRAY = "@@@@@@@@@@@@%%%%%%%%#########********+++++++++====";
        static QString STANDARD = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^'. ";
        static QString STD_MIN = "@%#*+=-:. "; 
    };
    
    char convert(const QColor& pixel, const QString mode = CHARS::STD_MIN ) {
        int nbcolor = (pixel.red() + pixel.green() + pixel.blue()) / 3;
        qreal size = mode.size();        
        qreal idx = (size - 1) * (qreal(nbcolor) / qreal(255));
        return mode.toStdString()[static_cast<unsigned int>(idx)];
    }
    
    QString convertFrame(const QImage* frame, const QString mode = CHARS::STD_MIN ) {
        std::string result = "";
        for (int y = 0; y < frame->height(); y++) {
            std::string temp;
            for (int x = 0; x < frame->width(); x++) {
                const char res = convert(frame->pixelColor(x, y), mode);   
                temp += res;
            }
            result.append(temp + "\n");
        }
        return QString::fromStdString(result);
    }
};

#endif // ASCIIVIDEOCONVERTER_H
