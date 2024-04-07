#include "videoplay.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    videoPlay w;
    w.show();
    return a.exec();
}
