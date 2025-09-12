#ifndef SYSTEMSETTINGSMODEL_H
#define SYSTEMSETTINGSMODEL_H

#include <string>

#include "Model.hpp"

class SystemSettingsModel : public Model
{
  public:
    SystemSettingsModel() = default;

    SystemSettingsModel(const SystemSettingsModel &) = delete;
    SystemSettingsModel &operator=(const SystemSettingsModel &) = delete;

    std::string getWifiAPSsid() const;
    void setWifiAPSsid(std::string wifiStationSsid);
    std::string getWifiAPPassword() const;
    void setWifiAPPassword(std::string wifiStationPassword);

  private:
    std::string wifiAPSsid = "PianoLeds";
    std::string wifiAPPassword = "LedsFlashSomeNotes";
};

#endif  // SYSTEMSETTINGSMODEL_H
