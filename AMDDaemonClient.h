#ifndef AMDDAEMONCLIENT_H
#define AMDDAEMONCLIENT_H

#include <QString>
#include <netinet/in.h>

class AMDDaemonClient
{

    int soketidServer;
    struct sockaddr_in echoserver;
    QString readString();
    int readInt();
    void sendInt(int i);

public:

    enum Method{
        PROFILE=0,
        DYNPM=1
    };

    enum Power{
        DEFAULT=1,
        MID=2,
        LOW=3,
        HIGH=4,
        AUTO=5
    };

    enum commands{
        //profile event
        H_POWER=1,
        M_POWER=2,
        L_POWER=3,
        A_POWER=4,
        D_POWER=5,
        //change in profile or dynpm
        P_METHOD=6,
        D_METHOD=7,
        //query
        GET_INFO=8,
        GET_POWER=9,
        GET_METHOD=10,
        //exit
        EXIT_CLIENT=20,
        EXIT_SERVICE=30
    };

    AMDDaemonClient();
    bool openConnection();
    void closeConnection();
    QString getInfo();
    Power getPower();
    Method getMethod();

    void setPowerHigh();
    void setPowerMid();
    void setPowerLow();
    void setPowerAuto();
    void setPowerDefault();

    void setMethodProfile();
    void setMethodDynpm();


};

#endif // AMDDAEMONCLIENT_H
