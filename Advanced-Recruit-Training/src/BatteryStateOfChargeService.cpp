#include "BatteryStateOfChargeService.h"
#include <QDebug>

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
    return initialAhUsed_ + chargeAh_;
}

bool BatteryStateOfChargeService::isCharging() const
{
    if (current_ < 0)
       return true;
        return false;
}

QTime BatteryStateOfChargeService::timeWhenChargedOrDepleted() const
{
    QTime time(0,0,0,0);
    int ms_remaining;
      if (isCharging() == true)  {
        ms_remaining = totalAmpHoursUsed()/averageCurrent_;
    }   else    {
        ms_remaining = (BATTERY_AMP_HOUR_CAPACITY - totalAmpHoursUsed())/averageCurrent_;
        ms_remaining *= -1;
    }

    QTime timeWhenChargedOrDepleted = time.addMSecs(ms_remaining);

    return timeWhenChargedOrDepleted;

}

void BatteryStateOfChargeService::addData(const BatteryData& batteryData)
{

    double DeltaMseconds = batteryData.time.msec() - Previous_milliseconds_.msec();

    Previous_milliseconds_.addMSecs(batteryData.time.msec());

    current_ = batteryData.current;

    initialAhUsed_ = (100 - initialStateOfChargePercent_) / 100 * BATTERY_AMP_HOUR_CAPACITY;

    totalCurrent_ = batteryData.current + totalCurrent_;

    averageCurrent_ = totalCurrent_/counter_;

    counter_++;

    chargeAh_ =  averageCurrent_*DeltaMseconds;
}
