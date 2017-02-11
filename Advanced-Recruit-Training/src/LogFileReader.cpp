#include <QDebug>
#include <QFile>
#include <QString>
#include <QTextStream>

#include "LogFileReader.h"

namespace
{
    const QString STRING_TIME_FORMAT= "hh:mm:ss.zzz";
    const QString BATDATA_DELIMITER= ", ";
    const int COLUMNS = 3;
    const int Data_Count = 0;
}

LogFileReader::LogFileReader()  //Constructor
{
}

LogFileReader::~LogFileReader() //Destructor
{
}

bool LogFileReader::readAll(const QString& fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) //if the file doesn't open, return false and exit out of function LogFileReader
    {
        qDebug() << "Unable to open file" << fileName;  //prints at the top, the text and the directory
        return false;
    }

    QTextStream input(&file);   //text from file is put into QTextStream Object called input
    while(!input.atEnd())   //loop through while the input hasn't ended
    {
        QString line = input.readLine();    //read's a line of BatData
        BatteryData batteryData;
        if (!parseLine(line, batteryData))  //if there was no problem parsing, move to else
        {
            qDebug() << "Error while parsing" << line;
             return false;
        }
        else
        {
            // This is how to send out a signal in QT using the emit keyword.
            // This line notifies the classes listening to this signal
            // that battery data has been received.
            emit batteryDataReceived(batteryData);  //this sends a signal that tells a class/function that
        }                                           //data has been received and what the data is (batteryData)
    }

    return true;
}

/* File input is a csv file in the format of hh:mm:ss:zzz, voltage, current
 * Need to implement error checking for the correct number of values and
 * that the conversion from string to double is sucessful.*/

bool LogFileReader::parseLine(const QString& line, BatteryData& batteryData) const
{
    QStringList sections = line.split(BATDATA_DELIMITER);   //for each line of the output, seperate values

    if (sections.length() < 3)  {
        qDebug() << "sections length is less than 3";
        return false;
    }

    QString timeString = sections.at(0);    //tells u all the entries of the first column are considered time strings
    batteryData.time = QTime::fromString(timeString, STRING_TIME_FORMAT);   //check if there is an invalid character

    bool double_conversion = true;

    batteryData.voltage = sections.at(1).toDouble(&double_conversion);    //.at function just takes an array from the string

    batteryData.current = sections.at(2).toDouble(&double_conversion);

    if (double_conversion == false) {
        qDebug() << "double_conversion unsuccessful";
        return false;
}
    //Check format of timeString
    if (timeString[2] != ':')    {
        qDebug() << "Error, timeString has incorrect format" << timeString;
        return false;
    }   else if (timeString[5] != ':')  {
        qDebug() << "Error, timeString has incorrect format" << timeString;
        return false;
    }   else if (timeString[8] != '.')  {
        qDebug() << "Error, timeString has incorrect format" << timeString;
        return false;
    }
    //check that timeString has all correct values
     for (int i = 0; i < timeString.length(); i++) {
        if (timeString[i].isLetter())   {
        qDebug() << "Error, timeString has incorrect character" << timeString;
        return false;
        }
     }
     //check that timeString has correct number of values
     if (timeString.length() < 12)  {
         qDebug() << "Error, timeString has incorrect format" << timeString;
         return false;
     }

    return true;
}
