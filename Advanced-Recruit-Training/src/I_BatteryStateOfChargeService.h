#pragma once

#include <QTime>
#include "BatteryData.h"    //added BatteryData.h so I can use BatteryData.current in BatteryStateOfChargeService

struct BatteryData;

class I_BatteryStateOfChargeService
{
public:
    virtual ~I_BatteryStateOfChargeService() {}

    virtual double totalAmpHoursUsed() const = 0;
    virtual bool isCharging() const = 0;
    virtual QTime timeWhenChargedOrDepleted() const = 0;

    virtual void addData(const BatteryData& batteryData) = 0;

};
