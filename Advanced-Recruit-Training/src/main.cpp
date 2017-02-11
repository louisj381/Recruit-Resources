#include <QCommandLineParser>
#include <QCoreApplication>
#include <QTimer>

#include "BatteryStateDisplayService.h"
#include "BatteryStateOfChargeService.h"
#include "LogFileReader.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);   //shell for main function
    QCoreApplication::setApplicationName("Battery Life Predictor");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption filenameOption(QStringList() << "f" << "filename",
            QCoreApplication::translate("main", "Name of csv log file of battery data"),    //returns the translation text for sourcetext, by querying the installed translation files
            QCoreApplication::translate("main", "filename"));
    parser.addOption(filenameOption);   //actually updates the file

    QCommandLineOption initialStateOfChargeOption(QStringList() << "i" << "initStateOfCharge",
            QCoreApplication::translate("main", "Initial State Of Charge of Battery as percent from 0-100"),
            QCoreApplication::translate("main", "stateOfCharge"));
    parser.addOption(initialStateOfChargeOption);

    parser.process(app);    //Process the actual command line arguments given by the user

    QString filename = parser.value(filenameOption);    //parse filename to filenameOption
    QString initialStateOfChargeString = parser.value(initialStateOfChargeOption);
    if (filename.isNull() || initialStateOfChargeString.isNull())   //check if the filename OR initial state is null
    {
        parser.showHelp();  //likely based off parser.addHelpOption()
        return -1;  //why do you return -1?
    }
    double initialStateOfCharge = initialStateOfChargeString.toDouble();    //Convert the word to a number

    LogFileReader logFileReader;
    BatteryStateOfChargeService batteryStateOfChargeService(initialStateOfCharge);  //must adjust this class to complete battery predictions
    BatteryStateDisplayService batteryStateOfChargeDisplayService(logFileReader, batteryStateOfChargeService);  //batteryState..DisplayService is not used yet, find out where it goes

    logFileReader.readAll(filename);

    // Quit the program when it is done.
    QTimer::singleShot(0, &app, SLOT(quit()));
    return app.exec();
};
