#include "mainwindow.h"
#include <QApplication>

#include "databasesqlite.h"
#include "uidata.h"
#include <QDebug>
#include <QSplashScreen>
#include <windows.h>
#include "g_def.h"

//DataBaseSqlite *db=new DataBaseSqlite();
int main(int argc, char *argv[])
{
#if 0
   c_def.g_cnt=2;
   qDebug()<<c_def.g_cnt<<endl;
   c_def.g_cnt=3;
   qDebug()<<c_def.g_cnt<<endl;
    return 0;
#endif
    QApplication a(argc, argv);
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap("://start"));
    splash->show();
#if 1
       DataBaseSqlite *db=new DataBaseSqlite();
       UiData uidata;
#endif
#if 0
    db datebase;
#endif
#if 1
    //Sleep(5000);
    qDebug()<<"Main: init begin";
    MainWindow w(db,&uidata);
    w.show();
    splash->finish(&w);
    delete splash;
#endif
    return a.exec();
}
