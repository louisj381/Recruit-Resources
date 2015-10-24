#pragma once

#include "I_BatteryStateOfChargeService.h"

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
    // Convention is to have an _ after private members
    const double initialStateOfChargePercent_;
    int count;
    double objectCurrent;
    double inCurrent=0;
    double changeTime;
    double AmpHours;
    bool firstRun=true;
   // double avgCurrent=0;

    QTime intialTime;
       QTime currentTime;
    // Recommend adding necessary variables here
};
