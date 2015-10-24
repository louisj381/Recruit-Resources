//Find AmpHours (Delta Current * Delta Time) (Absoulte current)

#include "BatteryStateOfChargeService.h"
#include "BatteryData.h"
#include "iostream"
#include <QTextStream>
#include <QDebug>

namespace
{
    const double BATTERY_AMP_HOUR_CAPACITY = 123.0;
}

BatteryStateOfChargeService::BatteryStateOfChargeService(double initialStateOfChargePercent)
: initialStateOfChargePercent_(initialStateOfChargePercent)
{
 AmpHours=BATTERY_AMP_HOUR_CAPACITY*(initialStateOfChargePercent_/100);
}
BatteryStateOfChargeService::~BatteryStateOfChargeService()
{
}

double BatteryStateOfChargeService::totalAmpHoursUsed() const
{
    return (AmpHours);
}

bool BatteryStateOfChargeService::isCharging() const
{
    return false;
}

QTime BatteryStateOfChargeService::timeWhenChargedOrDepleted() const
{
    return QTime::currentTime();
}

void BatteryStateOfChargeService::addData(const BatteryData& batteryData)
{

    //Current Calculation
    //Present value of current
    objectCurrent=batteryData.current;
    inCurrent=objectCurrent;

    //Time Stuff
    currentTime=batteryData.time;
    if(firstRun!=true){
    changeTime=abs(intialTime.msecsTo(currentTime));
    changeTime=changeTime*2.77778e-7;

    //Average
    double avgCurrent=(objectCurrent+inCurrent)/2;
    //Amp hours Stuff
   double ptpAh= avgCurrent*changeTime;
AmpHours=AmpHours-ptpAh;
    }
    firstRun=false;

//Update the initial current for next run;
inCurrent=objectCurrent;
//Update the initial time for next run
intialTime=currentTime;

}
