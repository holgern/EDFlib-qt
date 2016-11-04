#include "EDFSignalHeader.h"

EDFSignalHeader::EDFSignalHeader(bool isAnnotationChannel, bool isEDF)
{
    annotationChannel = isAnnotationChannel;
    edfFormat = isEDF;
    if (annotationChannel)
    {
        if (edfFormat)
            setLabel("EDF Annotations");
        else
            setLabel("BDF Annotations");
        setTransducer(" ");
        setPhysicalDimension(" ");
        setPhysicalMin(-1.0);
        setPhysicalMax(1.0);
        if (edfFormat)
            setDigitalMax(32767);
        else
            setDigitalMax(8388607);
        if (edfFormat)
            setDigitalMin(-32768);
        else
            setDigitalMin(-8388608);
        setPrefilter(" ");
        if (edfFormat)
            setSignalSampleFrequency(57);
        else
            setSignalSampleFrequency(38);
        setReserved(" ");
    }
    else
    {
        setLabel("");
        setTransducer("");
        setPhysicalDimension("");
        setPhysicalMin(-1.0);
        setPhysicalMax(1.0);
        if (edfFormat)
            setDigitalMax(32767);
        else
            setDigitalMax(8388607);
        if (edfFormat)
            setDigitalMin(-32768);
        else
            setDigitalMin(-8388608);
        setPrefilter("");
        setSignalSampleFrequency(1);
        setReserved("");
    }
    samplePointer = 0LL;
    bufferOffset = 0;
}

EDFSignalHeader::~EDFSignalHeader() {}

bool EDFSignalHeader::setSignalSampleFrequency(int sampleFreq)
{
    if (sampleFreq < 1)
        return false;
    else
    {
        this->smpPerRecord = sampleFreq;
        return true;
    }
}

void EDFSignalHeader::updateBufferOffset()
{
    bitvalue = (physicalMax - physicalMin) / (digitalMax - digitalMin);
    offset = static_cast<int>(physicalMax / bitvalue - digitalMax);
}

bool EDFSignalHeader::setDigitalMin(int dig_min)
{
    if (dig_min < (-8388608))
        return false;
    else
    {
        this->digitalMin = dig_min;
        updateBufferOffset();
        return true;
    }
}

bool EDFSignalHeader::setDigitalMax(int dig_max)
{
    if (dig_max > 8388607)
        return false;
    else
    {
        this->digitalMax = dig_max;
        updateBufferOffset();
        return true;
    }
}

bool EDFSignalHeader::setPhysicalMin(double phys_min)
{
    this->physicalMin = phys_min;
    updateBufferOffset();
    return true;
}

bool EDFSignalHeader::setPhysicalMax(double phys_max)
{
    this->physicalMax = phys_max;
    updateBufferOffset();
    return true;
}

bool EDFSignalHeader::setPhysicalDimension(const QString& s)
{
    if (s.isEmpty())
        physicalDimension = s;
    else
        physicalDimension = s.trimmed().left(8);
    if (s.trimmed().size() > 8 || s.isEmpty())
        return false;
    else
        return true;
}

bool EDFSignalHeader::setLabel(const QString& s)
{
    if (s.isEmpty())
        label = s;
    else
        label = s.trimmed().left(16);
    if (s.trimmed().size() > 16 || s.isEmpty())
        return false;
    else
        return true;
}
bool EDFSignalHeader::setPrefilter(const QString& s)
{
    prefilter = s.trimmed().left(80);
    if (s.trimmed().size() > 80 || s.isEmpty())
        return false;
    else
        return true;
}
bool EDFSignalHeader::setTransducer(const QString& s)
{
    if (s.isEmpty())
        transducer = s;
    else
        transducer = s.trimmed().left(80);
    if (s.trimmed().size() > 80 || s.isEmpty())
        return false;
    else
        return true;
}
