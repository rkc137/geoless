#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QFile style_file(":/res/PicPax.qss");
//    style_file.open(QFile::ReadOnly);
//    a.setStyleSheet(QLatin1String(style_file.readAll()));

    MainWindow w;
    w.show();
    return a.exec();
}
