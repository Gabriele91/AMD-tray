#include "AMDDaemonClient.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <arpa/inet.h>
#include <QApplication>
#include <QErrorMessage>
#define SERVICE_PORT 5540
#define SERVICE_ADDR "127.0.0.1"

#define exitShowError(m)\
{\
QErrorMessage errorMessage;\
errorMessage.showMessage(m);\
errorMessage.exec();\
QApplication::exit(0);\
}


AMDDaemonClient::AMDDaemonClient()
{
}
QString AMDDaemonClient::readString(){

    int leng=readInt();
    char* buffer=(char*)alloca(leng+1);
    if(recv(soketidServer,buffer,leng,0)<0)
        exitShowError("Error connection: fails on read string")
    buffer[leng]='\0';
    return QString(buffer);

}
int AMDDaemonClient::readInt(){
    int msg;
    if(recv(soketidServer,&msg,sizeof(int),0)<0)
        exitShowError("Error connection: fails on read int")
    return msg;
}
void AMDDaemonClient::sendInt(int i){
    if(send(soketidServer, &i, sizeof(int),0)<0)
        exitShowError("Error connection: fails on send int")
}

bool AMDDaemonClient::openConnection(){

    //create coket
    if((soketidServer = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        exitShowError("Error open connection: fails on creating tcp socket")

    memset(&echoserver, '0', sizeof(sockaddr_in));
    echoserver.sin_family = AF_INET;
    echoserver.sin_port = htons(SERVICE_PORT);
    echoserver.sin_addr.s_addr = inet_addr(SERVICE_ADDR);

    if( connect(soketidServer, (struct sockaddr *)&echoserver, sizeof(sockaddr_in)) < 0)
        exitShowError("Error open connection: connect failed")


}
void AMDDaemonClient::closeConnection(){
    if(soketidServer>=0){
        sendInt(EXIT_CLIENT);
        close(soketidServer);
        soketidServer=-1;
    }
}
QString AMDDaemonClient::getInfo(){
    sendInt(GET_INFO);
    return readString();
}
AMDDaemonClient::Power AMDDaemonClient::getPower(){
    sendInt(GET_POWER);
    return (AMDDaemonClient::Power)readInt();
}
AMDDaemonClient::Method AMDDaemonClient::getMethod(){
    sendInt(GET_METHOD);
    return (AMDDaemonClient::Method)readInt();
}

void AMDDaemonClient::setPowerHigh(){
    sendInt(H_POWER);
}
void AMDDaemonClient::setPowerMid(){
    sendInt(M_POWER);
}
void AMDDaemonClient::setPowerLow(){
    sendInt(L_POWER);
}
void AMDDaemonClient::setPowerAuto(){
    sendInt(A_POWER);
}
void AMDDaemonClient::setPowerDefault(){
    sendInt(D_POWER);
}

void AMDDaemonClient::setMethodProfile(){
    sendInt(P_METHOD);
}
void AMDDaemonClient::setMethodDynpm(){
    sendInt(D_METHOD);
}

