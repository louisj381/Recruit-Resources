#include <QTextStream>
#include "BatteryData.h"
#include "BatteryStateDisplayService.h"
#include "I_BatteryDataSource.h"
#include "I_BatteryStateOfChargeService.h"
#include <QString>

namespace
{
    const QString TIME_FORMAT = "hh:mm:ss.zzz";
}

BatteryStateDisplayService::BatteryStateDisplayService(
    const I_BatteryDataSource& batteryDataSource,
    I_BatteryStateOfChargeService& batteryStateOfChargeService)
: batteryStateOfChargeService_(batteryStateOfChargeService)
{
    connect(&batteryDataSource, SIGNAL(batteryDataReceived(const BatteryData&)),
        this, SLOT(handleBatteryDataReceived(const BatteryData&)));
}

BatteryStateDisplayService::~BatteryStateDisplayService()
{
}

void BatteryStateDisplayService::handleBatteryDataReceived(const BatteryData& batteryData)
{
    batteryStateOfChargeService_.addData(batteryData);

    QString TIME_PARAMETER;

    if (batteryStateOfChargeService_.isCharging())
    {
        TIME_PARAMETER = " Time remaining until Battery is Charged: ";
    }
    else
    {
        TIME_PARAMETER = " Time remaining until Battery is Depleted: ";
    }

    QTextStream(stdout) << "Voltage: " << batteryData.voltage
        << " Current: " << batteryData.current
        << " Total Ah used: " << batteryStateOfChargeService_.totalAmpHoursUsed()
        << TIME_PARAMETER << batteryStateOfChargeService_.timeWhenChargedOrDepleted().toString(TIME_FORMAT) << endl;

}
