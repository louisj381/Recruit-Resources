#include "BatteryStateOfChargeService.h"

namespace
{
    const double BATTERY_AMP_HOUR_CAPACITY = 123.0;

}

BatteryStateOfChargeService::BatteryStateOfChargeService(double initialStateOfChargePercent)
    : initialStateOfChargePercent_(initialStateOfChargePercent)
{
}


BatteryStateOfChargeService::~BatteryStateOfChargeService()
{
}

double BatteryStateOfChargeService::totalAmpHoursUsed() const
{
    //total used will be a dynamic initial plus either a positive or negative value, positive if depleting or negative is charging
    return initialAhUsed_ + chargeAh_;
}

bool BatteryStateOfChargeService::isCharging() const
{
    if (averageCurrent_ < 0)
       return true;
        return false;
}

QTime BatteryStateOfChargeService::timeWhenChargedOrDepleted() const
{
    QTime timeWhenChargedOrDepleted(0, 0, 0, 0);
    int ms_remaining;
      if (isCharging() == true)  {
        ms_remaining = totalAmpHoursUsed()/averageCurrent_ * 3.6e10;
    }   else    {
        ms_remaining = (BATTERY_AMP_HOUR_CAPACITY - totalAmpHoursUsed())/averageCurrent_ * 3.6e10;
    }

    timeWhenChargedOrDepleted.addMSecs(ms_remaining);
    return timeWhenChargedOrDepleted;

}

void BatteryStateOfChargeService::addData(const BatteryData& batteryData)
{

    QTime milliseconds;
    double Mseconds = milliseconds.msec();

    current = batteryData.current;

    initialAhUsed_ = (100 - initialStateOfChargePercent_) / 100 * BATTERY_AMP_HOUR_CAPACITY;

    totalCurrent_ = batteryData.current + totalCurrent_;

    averageCurrent_ = totalCurrent_/counter;

    counter++;

    chargeAh_ =  averageCurrent_*Mseconds;


    Q_UNUSED(batteryData);
    // This is where you can update your variables
}
