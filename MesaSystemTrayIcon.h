#ifndef MESASYSTEMTRAYICON_H
#define MESASYSTEMTRAYICON_H

#include <QSystemTrayIcon>
#include "AMDDaemonClient.h"

class MESASystemTrayIcon : public QSystemTrayIcon
{

    Q_OBJECT
    AMDDaemonClient daemon;

public:
    MESASystemTrayIcon(QWidget *parent,const QIcon& icon);
    ~MESASystemTrayIcon();

public slots:

    void onHighPower(bool);
    void onMidPower(bool);
    void onLowPower(bool);
    void onAuto(bool);
    void onDefault(bool);
    void onProfile(bool);
    void onDynpm(bool);
    void onExit(bool);
    void onActivated(QSystemTrayIcon::ActivationReason);

};

#endif // MESASYSTEMTRAYICON_H
