#include "AMDSettings.hpp"
#include "Utils.hpp"

#define PATH_CARD          "/sys/class/drm/card"
#define PREPATH_INFO       "/sys/kernel/debug/dri/"
#define PATH_INFO          "/radeon_pm_info"
#define PATH_INFO_TEMP     "/device/hwmon/hwmon1/temp1_input"
#define PATH_POWER_METHOD  "/device/power_method"
#define PATH_POWER_PROFILE "/device/power_profile"

void AMDSettings::init(){

    Driver driver;
    int i=0;

    //for each cards
    while(Utils::dirExist(PATH_CARD+Utils::toString(i))){
        //radeon_pm_info existed if is a radeon gpu
        driver.sIdDevice=Utils::toString(i);
        std::string driverInfo=PREPATH_INFO+driver.sIdDevice+PATH_INFO;

        if(Utils::fileReadable(driverInfo)){//ati driver?

            driver.idDevice=i;
            driver.sIdDevice=Utils::toString(i);
            driver.temperatur=PATH_CARD+driver.sIdDevice+PATH_INFO_TEMP;
            driver.info=driverInfo;
            driver.powerMethod=PATH_CARD+driver.sIdDevice+PATH_POWER_METHOD;
            driver.powerProfile=PATH_CARD+driver.sIdDevice+PATH_POWER_PROFILE;

            drivers.push_back(driver);
        }
        ++i;
    }

}
//set methods for all gpu
void AMDSettings::setMethod(AMDSettings::Method mtd){
    //setting string
    std::string setting=methodToString(mtd);
    //write methods on al devices
    for(auto& driver:drivers)
        Utils::stringToExistFile(driver.powerMethod,setting);
}
AMDSettings::Method AMDSettings::getMethod(){

    //string file
    std::string sfile;
    //get max value
    int tmpmethod=-1;
    int outmethod=-1;
    bool diff=false;
    //get max setting
    for(auto& driver:drivers){
        if(Utils::fileToString(driver.powerMethod,sfile)){
             outmethod=stringToMethod(sfile);
             diff=tmpmethod!=-1 && tmpmethod!=outmethod;
             tmpmethod=outmethod;
        }
    }

    //set equals setting for all gpu cards
    if(diff){
        setMethod(PROFILE);
        return PROFILE;
    }
    //return equals settings
    return    (AMDSettings::Method)outmethod;
}
//set/get power status
void   AMDSettings::setPower(AMDSettings::Power pwr){
    //setting string
    std::string setting=powerToString(pwr);
    //write status on al devices
    for(auto& driver:drivers)
        Utils::stringToExistFile(driver.powerProfile,setting);
    //
}
AMDSettings::Power  AMDSettings::getPower(){
    //string file
    std::string sfile;
    //get max value
    int powermax=-1;
    int powertmp=-1;
    int countDif=0;

    //get max setting
    for(auto& driver:drivers){
        if(Utils::fileToString(driver.powerProfile,sfile)){
            powertmp=stringToPower(sfile);
            //get "maximun setting"
            if(powertmp!=powermax){
                powermax=powermax>powertmp?powertmp:powermax;
                ++countDif;
            }
        }
    }

    //set equals setting for all gpu cards
    if(countDif>1)
        setPower((AMDSettings::Power)powermax);

    return (AMDSettings::Power)powermax;
}
//other info
long  AMDSettings::getTemperatur() const{

    std::string sTemp;
    int i=0;
    long tmpTemp=0;
    long maxTemp=0;

    for(auto& driver:drivers){
        if(Utils::fileToString(driver.temperatur,sTemp)){
            tmpTemp=Utils::stringToLong(sTemp);
            maxTemp=maxTemp>tmpTemp?maxTemp:tmpTemp;
        }
    }

    return maxTemp;
}

//query for cards
AMDSettings::Method AMDSettings::getMethod(const Driver& driver) const{
  std::string sfile;
  if(Utils::fileToString(driver.powerMethod,sfile)) return stringToMethod(sfile);
  return PROFILE;
}
AMDSettings::Power AMDSettings::getPower(const Driver& driver) const{
  std::string sfile;
  if(Utils::fileToString(driver.powerProfile,sfile)) return stringToPower(sfile);
  return DEFAULT;
}
long AMDSettings::getTemperatur(const Driver& driver) const{
  std::string sTemp;
  if(Utils::fileToString(driver.temperatur,sTemp))
    return Utils::stringToLong(sTemp);
  return 0;
}
void  AMDSettings::getInfo(std::string& info) const{
    std::string sfile;
    for(const auto& driver:drivers){
        info+="----- Card"+driver.sIdDevice+" -----\n";
        info+="Power method: " +methodToString(getMethod(driver))+"\n";
        info+="Power profile: "+powerToString(getPower(driver))+"\n";
        info+="Temperatur: "   +Utils::toString((int)((double)getTemperatur(driver)/1000.0))+" °C\n";
        Utils::fileToString(driver.info,sfile);
        info+= sfile;
    }
}

//get drivers
const AMDSettings::Driver& AMDSettings::getDriver(int i) const {
    return drivers[i];
}
int AMDSettings::getDriversSize() const {
    return drivers.size();
}

//easy cast
std::string AMDSettings::powerToString(AMDSettings::Power pow) const{
    switch(pow){
        default:
        case DEFAULT: return "default";
        case HIGH: return "high";
        case MID: return "mid";
        case LOW: return "low";
        case AUTO: return "auto";
    }
}
AMDSettings::Power AMDSettings::stringToPower(const std::string& pow) const{
    if(pow=="high"||pow=="high\n") return HIGH;
    if(pow=="mid"||pow=="mid\n") return MID;
    if(pow=="low"||pow=="low\n") return LOW;
    if(pow=="auto"||pow=="auto\n") return AUTO;
    return DEFAULT;
}

std::string AMDSettings::methodToString(AMDSettings::Method mtd) const{
    switch(mtd){
        default:
        case PROFILE: return "profile";
        case DYNPM: return "dynpm";
    }
}
AMDSettings::Method AMDSettings::stringToMethod(const std::string& mtd) const{
    if(mtd=="dynpm"||mtd=="dynpm\n") return DYNPM;
    return PROFILE;
}
