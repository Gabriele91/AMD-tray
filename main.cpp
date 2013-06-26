#include <QApplication>
#include <QWidget>
#include "MesaSystemTrayIcon.h"

int main(int argc,char **args){

    QApplication app(argc,args);
    //application
    QWidget wid;
    MESASystemTrayIcon msti(&wid,QIcon("none.svg"));
    msti.show();

    return app.exec();
}
