#include "SystemSettingsModel.h"

std::string SystemSettingsModel::getWifiAPSsid() const
{
    return get(wifiAPSsid);
}

void SystemSettingsModel::setWifiAPSsid(std::string wifiStationSsid)
{
    set(wifiAPSsid, wifiStationSsid);
}

std::string SystemSettingsModel::getWifiAPPassword() const
{
    return get(wifiAPPassword);
}

void SystemSettingsModel::setWifiAPPassword(std::string wifiStationPassword)
{
    set(wifiAPPassword, wifiStationPassword);
}
