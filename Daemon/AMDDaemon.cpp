#include "AMDDaemon.hpp"
#define SERVICE_PORT 5540

void AMDDaemon::onHighPower(){
    settings.setPower(AMDSettings::HIGH);
}
void AMDDaemon::onMidPower(){
    settings.setPower(AMDSettings::MID);
}
void AMDDaemon::onLowPower(){
    settings.setPower(AMDSettings::LOW);
}
void AMDDaemon::onAuto(){
    settings.setPower(AMDSettings::AUTO);
}
void AMDDaemon::onDefault(){
    settings.setPower(AMDSettings::DEFAULT);
}

void AMDDaemon::onProfile(){
    settings.setMethod(AMDSettings::PROFILE);
}
void AMDDaemon::onDynpm(){
    settings.setMethod(AMDSettings::DYNPM);
}


void AMDDaemon::onInfo(){
    std::string info;
    settings.getInfo(info);
    sendString(info);
}
void AMDDaemon::onGetPower(){
    sendInt(settings.getPower());
}
void AMDDaemon::onGetMethod(){
    sendInt(settings.getMethod());
}

void AMDDaemon::sendString(const std::string& msg){

    int length=(int)msg.size();
    sendInt(length); //send size
    send(soketidClient, msg.c_str(), length,0);  //send string

}
void AMDDaemon::sendInt(int msg){
    send(soketidClient, &msg, sizeof(int),0);
}

bool  AMDDaemon::onStart(){
    //init settings
    if(settings.getDriversSize()<1)
        settings.init();
    //init values:
    soketidServer=-1;
    soketidClient=-1;
    //socket tcp
    soketidServer=socket(AF_INET,SOCK_STREAM,0);
    if(soketidServer<0)
        return false;

    //bind server
    memset(&echoserver,0,sizeof(sockaddr_in));
    echoserver.sin_family=AF_INET ; //internet ip
    echoserver.sin_addr.s_addr=htonl(INADDR_ANY);   //any anddres
    echoserver.sin_port=htons(SERVICE_PORT);       //port
    if(bind(soketidServer,(struct sockaddr*)&echoserver,sizeof(sockaddr_in))<0)
        return false;

    //wait client
    if(listen(soketidServer,1)<0)//not supported multiple client
        return false;

    //get client
    soketidClient=accept( soketidServer, (struct sockaddr*)NULL, NULL );
    //socklen_t lengstructclient=0;
    //soketidClient=accept(soketidServer,
    //                     (struct sockaddr*)&echoclient,
    //                     &lengstructclient);

    if(soketidClient<0)
        return false;

    return true;
}

bool  AMDDaemon::onRun(){
    int message=0;

    if(recv(soketidClient,&message,sizeof(int),0)<0)
        message=EXIT_CLIENT;

    switch(message){
     case H_POWER: onHighPower(); break;
     case M_POWER: onMidPower();  break;
     case L_POWER: onLowPower();  break;
     case A_POWER: onAuto();  break;
     case D_POWER: onDefault();  break;

     case P_METHOD: onProfile();  break;
     case D_METHOD: onDynpm();  break;

     case GET_INFO: onInfo();  break;
     case GET_POWER: onGetPower();  break;
     case GET_METHOD: onGetMethod();  break;

     case EXIT_CLIENT:
            //restart
            onClose();
            onStart();
     break;
     case EXIT_SERVICE:
            //close this daemon
            return false;
     break;
     default:break;
    }
    return true;
}

bool  AMDDaemon::onClose(){
    if(soketidServer>=0)
        close(soketidServer);
    if(soketidClient>=0)
        close(soketidClient);
    return true;
}
