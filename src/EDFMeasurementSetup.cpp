#include "EDFMeasurementSetup.h"

EDFMeasurementSetup::EDFMeasurementSetup()
    :date(EDFDate(1, 1, 1900))
{
    recording = "";
    recordingAdditional = "";
    adminCode = "";
    technician = "";
    equipment = "";
}
EDFMeasurementSetup::~EDFMeasurementSetup() {}

EDFMeasurementSetup::EDFMeasurementSetup(EDFDate newDate, QString newAdminCode, QString newTechnician,
                                         QString newEquipment, QString newRecordingAdd, QString newRecording)
    : date(newDate)
    , adminCode(newAdminCode)
    , technician(newTechnician)
    , equipment(newEquipment)
    , recordingAdditional(newRecordingAdd)
    , recording(newRecording)
{
}

QString EDFMeasurementSetup::convertString(QString& str)
{
    QString out = str.trimmed();
    // out.remove(QRegExp("[^a-zA-Z\\d\\s]"));
    out = QString(out.toLatin1().data());
    out.replace(QString(" "), QString("_"));
    return out;
}

QString EDFMeasurementSetup::toHeaderInfoString() const
{
    QString str;

    str += QString("Startdate ") + date.toString().toUpper() + QString(" ");

    if (adminCode.isEmpty())
    {
        str += QString("X ");
    }
    else
    {
        str += adminCode + QString(" ");
    }
    if (technician.isEmpty())
    {
        str += QString("X ");
    }
    else
    {
        str += technician + QString(" ");
    }
    if (equipment.isEmpty())
    {
        str += QString("X ");
    }
    else
    {
        str += equipment + QString(" ");
    }
    if (recordingAdditional.isEmpty())
    {
        str += QString("X");
    }
    else
    {
        str += recordingAdditional;
    }

    return str.leftJustified(80, ' ', true);
}
bool EDFMeasurementSetup::parseHeaderString(QString edf_hdr, bool plusFormat)
{
    if (plusFormat)
    {
        QStringList str_list = edf_hdr.split(" ", QString::SkipEmptyParts);
        if (str_list.size() < 6)
            return false;
        if (!date.parseHeaderString(str_list.at(1)))
            return false;
        if (str_list.at(2) != "X")
            setAdminCode(str_list.at(2));
        if (str_list.at(3) != "X")
            setTechnician(str_list.at(3));
        if (str_list.at(4) != "X")
            setEquipment(str_list.at(4));
        if (str_list.at(5) != "X")
            setRecordingAdditional(str_list.at(5));
    }
    else
    {
        if (edf_hdr.size() > 0 && edf_hdr != "X")
            setRecording(edf_hdr);
    }
    return true;
}
