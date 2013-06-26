#ifndef AMDDAEMON_HPP
#define AMDDAEMON_HPP

#include "BaseDaemon.hpp"
#include "AMDSettings.hpp"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


class AMDDaemon : public Daemon{

    int soketidServer;
    int soketidClient;
    struct sockaddr_in echoserver,echoclient;
    AMDSettings settings;

    void sendString(const std::string& msg);
    void sendInt(int msg);

    public:

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

    void onHighPower();
    void onMidPower();
    void onLowPower();
    void onAuto();
    void onDefault();

    void onProfile();
    void onDynpm();

    void onInfo();
    void onGetPower();
    void onGetMethod();

    virtual bool onStart();
    virtual bool onRun();
    virtual bool onClose();
};


#endif
