#ifndef SYSTEMSETTINGSMODEL_H
#define SYSTEMSETTINGSMODEL_H

#include "Model.h"

class SystemSettingsModel: public Model
{
public:
    /**
     * Constructor
     */
    SystemSettingsModel() = default;

    // Prevent implicit copy constructor & assignment operator
    SystemSettingsModel(const SystemSettingsModel&) = delete;
    SystemSettingsModel& operator=(const SystemSettingsModel&) = delete;

    std::string getWifiAPSsid() const;
    void setWifiAPSsid(std::string wifiStationSsid);
    std::string getWifiAPPassword() const;
    void setWifiAPPassword(std::string wifiStationPassword);

private:
    /**
     * Name of the WiFi network when in AP mode
     */
    std::string wifiAPSsid = "PianoLeds";

    /**
     * Password of the WiFi network when in AP mode
     */
    std::string wifiAPPassword = "LedsFlashSomeNotes";
};


#endif //SYSTEMSETTINGSMODEL_H
