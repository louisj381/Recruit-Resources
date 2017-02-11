#pragma once

#include "I_BatteryStateOfChargeService.h"

class BatteryStateOfChargeService : public I_BatteryStateOfChargeService
{
public:
    explicit BatteryStateOfChargeService(double initialStateOfChargePercent);
    virtual ~BatteryStateOfChargeService();

    /*Returns the total amp hours used*/
    double totalAmpHoursUsed() const;

    /*Returns true if the battery is being charged, false if not.*/
    bool isCharging() const;

    /*Return the time remaining to when the battery is charged or depleted*/
    QTime timeWhenChargedOrDepleted() const;

    /*Update Variables*/
    void addData(const BatteryData& batteryData);

private:
    // Convention is to have an _ after private members
    const double initialStateOfChargePercent_;
    double initialAhUsed_;
    double chargeAh_;
    double averageCurrent_;
    double totalCurrent_ = 0;
    int counter_ = 1;
    double hours_remaining_;
    double current_;
    //QTime timeWhenChargedOrDepleted_;
    QTime Previous_milliseconds_;

    // Add your necessary variables here
};
