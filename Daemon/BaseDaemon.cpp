#include "BaseDaemon.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>

int ApplicationDaemon::pid=NULL;
int ApplicationDaemon::session=NULL;
Daemon* ApplicationDaemon::daemon=NULL;

int ApplicationDaemon::getPid(){
    return ApplicationDaemon::pid;
}
int ApplicationDaemon::getSession(){
    return ApplicationDaemon::session;
}

void ApplicationDaemon::exit(int){
    if(ApplicationDaemon::daemon)
        exit(ApplicationDaemon::daemon->onClose());
}

int ApplicationDaemon::exec(Daemon *argDaemon){

        //save current thread
        daemon=argDaemon;

        // Fork off the parent process
        pid = fork();
        //error
        if (pid < 0)
                return (EXIT_FAILURE);
        //isn't child
        if (pid > 0)
            return EXIT_SUCCESS;


        // Change the file mode mask
        umask(0);

        /* Create a new SID for the child process */
        session = setsid();
        if (session < 0)
            return EXIT_FAILURE;



        // Change the current working directory (root dir)
        if ((chdir("/")) < 0)
                return EXIT_FAILURE;

        // Close out the standard file descriptors
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        //exits signals
        signal(SIGINT,ApplicationDaemon::exit);
        signal(SIGQUIT,ApplicationDaemon::exit);
        //error exit
        bool status=true;
        //init daemon
        status=daemon->onStart();
        //loop
        while (status&&(status=daemon->onRun()));
        //exit
        status=daemon->onClose();
        return status;

}
