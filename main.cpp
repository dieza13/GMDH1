#include <QtGui/QApplication>
#include "mainwindow.h"
#include "QTextCodec"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(codec);
    MainWindow w;
    w.show();
    
    return a.exec();
}
