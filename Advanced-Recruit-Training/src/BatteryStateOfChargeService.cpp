#include "BatteryStateOfChargeService.h"
#include "BatteryData.h"

namespace
{
    const double BATTERY_AMP_HOUR_CAPACITY = 123.0;
    const double HOURS_TO_MS_CONVERSION = 3.6E6;
    const double MS_TO_HOURS_CONVERSION = 2.7778E-7;
}

BatteryStateOfChargeService::BatteryStateOfChargeService(double initialStateOfChargePercent)
    : initialStateOfChargePercent_(initialStateOfChargePercent)
, totalAmpHoursUsed_((100 - initialStateOfChargePercent_) / 100 * BATTERY_AMP_HOUR_CAPACITY)
, totalCurrent_(0)
{
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
    {
       return true;
    }
    else
    {
        return false;
    }
}

QTime BatteryStateOfChargeService::timeWhenChargedOrDepleted() const
{
    int ms_remaining;
    if (isCharging())
    {
      ms_remaining = totalAmpHoursUsed() / averageCurrent_ * HOURS_TO_MS_CONVERSION;
    }
    else
    {
      ms_remaining = (BATTERY_AMP_HOUR_CAPACITY - totalAmpHoursUsed()) / averageCurrent_ * HOURS_TO_MS_CONVERSION;
    }

    return QTime(0, 0, 0, 0).addMSecs(ms_remaining);

}
/*running average needs to be just the one after it, so I need to write the average as the current + past current divided by 2*/
void BatteryStateOfChargeService::addData(const BatteryData& batteryData)
{
    if (previousTime_.isNull())
    {
        previousTime_ = batteryData.time;
    }
    QTime presentTime = batteryData.time;
    int deltaMseconds;
    deltaMseconds = abs(presentTime.msecsTo(previousTime_));
    double deltaHours = deltaMseconds * MS_TO_HOURS_CONVERSION;
    previousTime_ = batteryData.time;

    if (previousCurrent_ == NULL)
    {
        previousCurrent_ = batteryData.current;
    }
    current_ = batteryData.current;
    totalCurrent_ = batteryData.current + previousCurrent_;
    averageCurrent_ = totalCurrent_ / 2;
    double deltaAh;
    deltaAh =  averageCurrent_ * deltaHours;
    totalAmpHoursUsed_ += deltaAh;
    previousCurrent_ = batteryData.current;
}
