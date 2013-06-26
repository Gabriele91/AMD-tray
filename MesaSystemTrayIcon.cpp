#include "MesaSystemTrayIcon.h"
#include <QtCore>
#include <QApplication>
#include <QMenu>
#include <QAction>

MESASystemTrayIcon::MESASystemTrayIcon(QWidget *ptr,const QIcon& icon):QSystemTrayIcon(icon,ptr){
    //connect to daemon
    daemon.openConnection();
    //create a menu
    QMenu *menu=new QMenu();
    QAction *acHighPower=new QAction(QIcon("high.svg"),"High Power",this);
    QAction *acMidPower=new QAction(QIcon("mid.svg"),"Mid Power",this);
    QAction *acLowPower=new QAction( QIcon("low.svg"),"Low Power",this);
    QAction *acAuto=new QAction(QIcon("auto.svg"),"Auto",this);
    QAction *acDefault=new QAction(QIcon("default.svg"),"Default",this);
    QAction *acProfile=new QAction(QIcon("profile.svg"),"Profile",this);
    QAction *acDynpm=new QAction(QIcon("dynpm.svg"),"Dynpm",this);
    QAction *acExit=new QAction("exit",this);
    //enable icons
    acHighPower->setIconVisibleInMenu(true);
    acMidPower->setIconVisibleInMenu(true);
    acLowPower->setIconVisibleInMenu(true);
    acAuto->setIconVisibleInMenu(true);
    acDefault->setIconVisibleInMenu(true);
    acProfile->setIconVisibleInMenu(true);
    acDynpm->setIconVisibleInMenu(true);
    //add objects
    menu->addAction(acHighPower);
    menu->addAction(acMidPower);
    menu->addAction(acLowPower);
    menu->addAction(acAuto);
    menu->addAction(acDefault);
    menu->addSeparator();
    menu->addAction(acProfile);
    menu->addAction(acDynpm);
    menu->addSeparator();
    menu->addAction(acExit);
    //setup events
    QObject::connect(acHighPower,SIGNAL(triggered(bool)), this, SLOT(onHighPower(bool)));
    QObject::connect(acMidPower,SIGNAL(triggered(bool)), this, SLOT(onMidPower(bool)));
    QObject::connect(acLowPower,SIGNAL(triggered(bool)), this, SLOT(onLowPower(bool)));
    QObject::connect(acAuto,SIGNAL(triggered(bool)), this, SLOT(onAuto(bool)));
    QObject::connect(acDefault,SIGNAL(triggered(bool)), this, SLOT(onDefault(bool)));
    QObject::connect(acProfile,SIGNAL(triggered(bool)), this, SLOT(onProfile(bool)));
    QObject::connect(acDynpm,SIGNAL(triggered(bool)), this, SLOT(onDynpm(bool)));
    QObject::connect(acExit,SIGNAL(triggered(bool)), this, SLOT(onExit(bool)));
    //show event
    QObject::connect(this,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                     this,SLOT(onActivated(QSystemTrayIcon::ActivationReason)));
    this->setContextMenu(menu);

    //super ico
    if(AMDDaemonClient::PROFILE==daemon.getMethod())
        setIcon(QIcon("profile.svg"));
    else
        setIcon(QIcon("dynpm.svg"));


}

MESASystemTrayIcon::~MESASystemTrayIcon(){
    //close
    daemon.closeConnection();
}

//Events
void MESASystemTrayIcon::onHighPower(bool){
    daemon.setPowerHigh();
}
void MESASystemTrayIcon::onMidPower(bool){
    daemon.setPowerMid();

}
void MESASystemTrayIcon::onLowPower(bool){
    daemon.setPowerLow();


}
void MESASystemTrayIcon::onAuto(bool){
    daemon.setPowerAuto();
}
void MESASystemTrayIcon::onDefault(bool){
    daemon.setPowerDefault();
}
void MESASystemTrayIcon::onProfile(bool){
    daemon.setMethodProfile();
    setIcon(QIcon("profile.svg"));
}
void MESASystemTrayIcon::onDynpm(bool){
    daemon.setMethodDynpm();
    setIcon(QIcon("dynpm.svg"));
}
void MESASystemTrayIcon::onExit(bool){
    //close
    daemon.closeConnection();
    QApplication::exit(0);
}

void MESASystemTrayIcon::onActivated(QSystemTrayIcon::ActivationReason actv){
    if(actv==3){
        showMessage(QString("AMD-INFO"),daemon.getInfo(),QSystemTrayIcon::Information,10000);
    }
}
//
