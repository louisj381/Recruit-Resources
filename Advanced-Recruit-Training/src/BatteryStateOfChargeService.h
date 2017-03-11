#pragma once

#include "I_BatteryStateOfChargeService.h"
#include <QString>

class BatteryStateOfChargeService : public I_BatteryStateOfChargeService
{
public:
    explicit BatteryStateOfChargeService(double initialStateOfChargePercent);
    virtual ~BatteryStateOfChargeService();


    double totalAmpHoursUsed() const;


    bool isCharging() const;


    QTime timeWhenChargedOrDepleted() const;


    void addData(const BatteryData& batteryData);

private:
    const double initialStateOfChargePercent_;
    double totalAmpHoursUsed_;
    double averageCurrent_; //
    double totalCurrent_;
    int counter_;
    double hours_remaining_;
    double current_;
    int initialAhUsed_;
    QTime initialTime_;
    QTime previousMilliseconds_;
};
