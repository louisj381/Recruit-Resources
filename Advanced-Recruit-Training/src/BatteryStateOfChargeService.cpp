#include "BatteryStateOfChargeService.h"
#include "BatteryData.h"
#include <QDebug>

namespace
{
    const double BATTERY_AMP_HOUR_CAPACITY = 123.0;
    const double MS_CONVERSION = 3.6E6;
    const double HOUR_CONVERSION = 3.6E-6;
}

BatteryStateOfChargeService::BatteryStateOfChargeService(double initialStateOfChargePercent)
    : initialStateOfChargePercent_(initialStateOfChargePercent)
{
    counter_ = 1;
    QTime placeholder(0,0,0,0);
    initialAhUsed_ = (100 - initialStateOfChargePercent_) / 100 * BATTERY_AMP_HOUR_CAPACITY;
    totalCurrent_ = 0;
    initialTime_ = placeholder;

}

BatteryStateOfChargeService::~BatteryStateOfChargeService()
{
}

double BatteryStateOfChargeService::totalAmpHoursUsed() const
{
    return totalAmpHoursUsed_;
}

bool BatteryStateOfChargeService::isCharging() const
{
    if (current_ < 0)
       return true;
    else
        return false;
}

QTime BatteryStateOfChargeService::timeWhenChargedOrDepleted() const
{
    QTime time(0,0,0,0);
    int ms_remaining;
      if (isCharging() == true)
      {
        ms_remaining = totalAmpHoursUsed() / averageCurrent_ * MS_CONVERSION;
      }   else
      {
        ms_remaining = (BATTERY_AMP_HOUR_CAPACITY - totalAmpHoursUsed()) / averageCurrent_ * MS_CONVERSION;
      }

    QTime timeWhenChargedOrDepleted = time.addMSecs(ms_remaining);

    return timeWhenChargedOrDepleted;

}

void BatteryStateOfChargeService::addData(const BatteryData& batteryData)
{
    int deltaMseconds;
    double chargeAh;
    QTime msTime(0,0,0,0);

    if (previousMilliseconds_.isNull())
    {
        previousMilliseconds_ = initialTime_.addMSecs(batteryData.time.msec());
    }

    msTime = msTime.addMSecs(batteryData.time.msec());

    deltaMseconds = abs(msTime.msecsTo(previousMilliseconds_));

    double deltaHours = deltaMseconds * HOUR_CONVERSION;

    previousMilliseconds_ = initialTime_.addMSecs(batteryData.time.msec());

    current_ = batteryData.current;

    totalCurrent_ = batteryData.current + totalCurrent_;

    averageCurrent_ = totalCurrent_ / counter_;

    counter_++;

    chargeAh =  averageCurrent_ * deltaHours;

    totalAmpHoursUsed_ = initialAhUsed_ + chargeAh;
}
