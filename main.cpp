#include "mainwindow.h"
#include "appconfig.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppConfig::instance().initDir();
    MainWindow w;
    w.show();
    return a.exec();
}