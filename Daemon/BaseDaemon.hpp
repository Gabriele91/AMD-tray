#ifndef BASEDAEMON_HPP
#define BASEDAEMON_HPP

class Daemon;
class ApplicationDaemon;

//interface
class Daemon{

    public:

    virtual bool onStart()=0;
    virtual bool onRun()=0;
    virtual bool onClose()=0;
};

//application class (no multi theads application)
class ApplicationDaemon{

    ApplicationDaemon(){};
    static Daemon *daemon;
    static int pid,session;
    static void exit(int);

    public:

        static int exec(Daemon*);
        //getters
        static int getPid();
        static int getSession();

};

#endif
