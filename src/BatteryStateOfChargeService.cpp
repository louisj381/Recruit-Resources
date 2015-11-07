
#include "BatteryStateOfChargeService.h"
#include "BatteryData.h"
#include <QTextStream>
#include <QDebug>
#include <QFile>

namespace
{
    const double BATTERY_AMP_HOUR_CAPACITY = 123.0;
}

BatteryStateOfChargeService::BatteryStateOfChargeService(double initialStateOfChargePercent)
: initialStateOfChargePercent_(initialStateOfChargePercent)
{
 AmpHoursUsed_=BATTERY_AMP_HOUR_CAPACITY*(initialStateOfChargePercent_/100);
}
BatteryStateOfChargeService::~BatteryStateOfChargeService()
{
}

double BatteryStateOfChargeService::totalAmpHoursUsed() const
{
    return (AmpHoursUsed_);
}

bool BatteryStateOfChargeService::isCharging() const
{
    if(newCurrent_>=0)
    return false;
    else
    return true;
}

QTime BatteryStateOfChargeService::timeWhenChargedOrDepleted() const
{
  double timeLeft;
  timeLeft=(AmpHoursUsed_/newCurrent_);
  timeLeft=qAbs(timeLeft);
  timeLeft=timeLeft/2.77778e-7;

  QTime base(0,0,0);
  QTime timeOutput= base.addMSecs(timeLeft);
  return timeOutput;
}

void BatteryStateOfChargeService::addData(const BatteryData& batteryData)
{
 //Setting Values

    inCurrent_=newCurrent_;
    initialTime_=currentTime_;

    newCurrent_=batteryData.current;
    currentTime_=batteryData.time;

    if(firstRun_!=true){

    int mschangeTime=abs(initialTime_.msecsTo(currentTime_));
    double changeTime=mschangeTime*2.77778e-7;

    double avgCurrent=(newCurrent_+inCurrent_)/2;
    double pointAverageAH= avgCurrent*changeTime;

    AmpHoursUsed_=AmpHoursUsed_-pointAverageAH;
    }
    firstRun_=false;

    sumCurrent_+=inCurrent_;
}
