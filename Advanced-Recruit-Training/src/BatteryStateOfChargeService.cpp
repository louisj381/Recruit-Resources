#include "BatteryStateOfChargeService.h"
#include "BatteryData.h"
#include <QDebug>

namespace
{
    const double BATTERY_AMP_HOUR_CAPACITY = 123.0;
    const double HOURS_TO_MS_CONVERSION = 3.6E6;
    const double MS_TO_HOURS_CONVERSION = 2.7778E-7;
}

BatteryStateOfChargeService::BatteryStateOfChargeService(double initialStateOfChargePercent)
    : initialStateOfChargePercent_(initialStateOfChargePercent)
{
    counter_ = 0;
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
      if (isCharging())
      {
        ms_remaining = totalAmpHoursUsed() / averageCurrent_ * HOURS_TO_MS_CONVERSION;
      }

      else
      {
        ms_remaining = (BATTERY_AMP_HOUR_CAPACITY - totalAmpHoursUsed()) / averageCurrent_ * HOURS_TO_MS_CONVERSION;
      }

    QTime timeWhenChargedOrDepleted = time.addMSecs(ms_remaining);

    return timeWhenChargedOrDepleted;

}

void BatteryStateOfChargeService::addData(const BatteryData& batteryData)
{
    counter_++;
    int deltaMseconds;
    double chargeAh;

    if (previousMilliseconds_.isNull())
    {
        previousMilliseconds_ = batteryData.time;
    }

    QTime presentTime = batteryData.time;

    deltaMseconds = abs(presentTime.msecsTo(previousMilliseconds_));

    double deltaHours = deltaMseconds * MS_TO_HOURS_CONVERSION;

    previousMilliseconds_ = batteryData.time;

    current_ = batteryData.current;

    totalCurrent_ = batteryData.current + totalCurrent_;

    averageCurrent_ = totalCurrent_ / counter_;

    chargeAh =  averageCurrent_ * deltaHours;

    totalAmpHoursUsed_ = initialAhUsed_ + chargeAh;
}
