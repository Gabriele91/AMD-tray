#ifndef AMDSETTINGS_HPP
#define AMDSETTINGS_HPP

#include <string>
#include <vector>

class AMDSettings{

    struct Driver{
        //id gpu
        int  idDevice;
        std::string   sIdDevice;
        //paths
        std::string temperatur;
        std::string info;
        std::string powerMethod;
        std::string powerProfile;
    };

    std::vector<Driver> drivers;

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

    //init
    void init();

    //set/get methods for all gpu
    void setMethod(Method mtd);
    Method getMethod();

    //set/get power status for all gpu
    void setPower(Power setting);
    Power getPower();

    //other info
    long getTemperatur() const;
    void getInfo(std::string& info) const;
    //query card
    const Driver& getDriver(int i) const;
    int getDriversSize() const;

    //query per card
    Method getMethod(const Driver& driver) const;
    Power getPower(const Driver& driver) const;
    long getTemperatur(const Driver& driver) const;

    //easy cast
    std::string powerToString(Power pow) const;
    Power stringToPower(const std::string& pow) const;

    std::string methodToString(Method mtd) const;
    Method stringToMethod(const std::string& mtd) const;



};

#endif
