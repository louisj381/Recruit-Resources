#include "BatteryStateOfChargeService.h"
#include "BatteryData.h"
#include <QDebug>

namespace
{
    const double BATTERY_AMP_HOUR_CAPACITY = 123.0;
    const double ms_conversion = 3.6E6;
    const double hour_conversion = 3.6E-6;
    int counter = 1;
    QTime placeholder(0,0,0,0);
    int initialAhUsed;
}

BatteryStateOfChargeService::BatteryStateOfChargeService(double initialStateOfChargePercent)
    : initialStateOfChargePercent_(initialStateOfChargePercent)
{
    initialAhUsed = (100 - initialStateOfChargePercent_) / 100 * BATTERY_AMP_HOUR_CAPACITY;
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
      if (isCharging() == true)  {
        ms_remaining = totalAmpHoursUsed() / averageCurrent_ * ms_conversion;
    }   else    {
        ms_remaining = (BATTERY_AMP_HOUR_CAPACITY - totalAmpHoursUsed()) / averageCurrent_ * ms_conversion;
    }

    QTime timeWhenChargedOrDepleted = time.addMSecs(ms_remaining);

    return timeWhenChargedOrDepleted;

}

void BatteryStateOfChargeService::addData(const BatteryData& batteryData)
{

    int DeltaMseconds;
    double chargeAh;
    QTime msTime;
    QTime msTime1(0,0,0,0);

    if (PreviousMilliseconds_.isNull())    {
        qDebug() << "this is the first run";
        PreviousMilliseconds_ = initialTime_.addMSecs(batteryData.time.msec());
    }

    msTime = msTime1.addMSecs(batteryData.time.msec());

    DeltaMseconds = abs(msTime.msecsTo(PreviousMilliseconds_));

    qDebug() << DeltaMseconds;

    double DeltaHours = DeltaMseconds * hour_conversion;

    PreviousMilliseconds_ = initialTime_.addMSecs(batteryData.time.msec());

    current_ = batteryData.current;

    totalCurrent_ = batteryData.current + totalCurrent_;

    averageCurrent_ = totalCurrent_ / counter;

    counter++;

    chargeAh =  averageCurrent_ * DeltaHours;

    totalAmpHoursUsed_ = initialAhUsed + chargeAh;
}
