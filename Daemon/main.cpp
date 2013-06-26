#include "AMDDaemon.hpp"

int main() {
    AMDDaemon daemon;
    return ApplicationDaemon::exec((Daemon*)&daemon);
}


