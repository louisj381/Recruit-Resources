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
    int msRemaining;
    if (isCharging())
    {
      msRemaining = totalAmpHoursUsed() / averageCurrent_ * HOURS_TO_MS_CONVERSION;
    }
    else
    {
      msRemaining = (BATTERY_AMP_HOUR_CAPACITY - totalAmpHoursUsed()) / averageCurrent_ * HOURS_TO_MS_CONVERSION;
    }

    return QTime(0, 0, 0, 0).addMSecs(msRemaining);

}
void BatteryStateOfChargeService::addData(const BatteryData& batteryData)
{
    if (previousTime_.isNull())
    {
        previousTime_ = batteryData.time;
    }
    QTime presentTime = batteryData.time;
    int deltaMseconds = abs(presentTime.msecsTo(previousTime_));
    double deltaHours = (double)deltaMseconds * MS_TO_HOURS_CONVERSION;
    previousTime_ = batteryData.time;

    if (previousCurrent_ == NULL)
    {
        previousCurrent_ = batteryData.current;
    }
    current_ = batteryData.current;
    averageCurrent_ = (batteryData.current + previousCurrent_) / 2;
    totalAmpHoursUsed_ += averageCurrent_ * deltaHours;
    previousCurrent_ = batteryData.current;
}
