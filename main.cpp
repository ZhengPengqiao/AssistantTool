#include "mainwindow.h"
#include <QApplication>
#include <QLatin1String>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QFile qssFile(":/assert/qt.qss");
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen())
    {
        QString qss = QLatin1String(qssFile.readAll());
        qApp->setStyleSheet(qss);
        qssFile.close();
    }

    MainWindow w;
    w.show();

    return a.exec();
}
