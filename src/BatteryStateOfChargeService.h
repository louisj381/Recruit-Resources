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

    double AmpHoursUsed_;
    double inCurrent_;
    double newCurrent_=0;
    double sumCurrent_=0;
    bool firstRun_=true;
    QTime initialTime_;
    QTime currentTime_;
    // Recommend adding necessary variables here
};
