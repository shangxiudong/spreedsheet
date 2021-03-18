#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen> //启动界面

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/image/house.png"));
    splash->show();
    splash->showMessage(QObject::tr("Setting up the main window..."),
                        Qt::AlignRight | Qt::AlignTop, Qt::white);
    MainWindow *w = new MainWindow;
    splash->showMessage(QObject::tr("loading modules..."),
                        Qt::AlignRight | Qt::AlignTop, Qt::white);

    w->show();
    splash->finish(w);
    delete splash;

    return a.exec();
}
