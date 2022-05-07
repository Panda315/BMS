#include "loginwindow.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LogInWindow w;
    a.QApplication::setQuitOnLastWindowClosed(false);
    w.show();
    return a.exec();
}
