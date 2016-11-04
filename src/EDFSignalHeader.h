#ifndef EDFSIGNALHEADER_H
#define EDFSIGNALHEADER_H
/*
 * Copyright (c) Holger Nahrstaedt
 */

#include <QString>
/*!
 * \brief The EDFSignalHeader class
 */
class EDFSignalHeader
{
public:
    EDFSignalHeader(bool isAnnotationChannel, bool isEDF);
    //    EDFSignalHeader(const EDFSignalHeader&);
    virtual ~EDFSignalHeader();

    bool setLabel(const QString&);
    bool setPhysicalMax(double);
    bool setPhysicalMin(double);
    bool setDigitalMax(int);
    bool setDigitalMin(int);
    bool setSignalSampleFrequency(int);
    bool setPhysicalDimension(const QString&);
    bool setPrefilter(const QString&);
    bool setTransducer(const QString&);
    void setReserved(const QString& str) { reserved = str; }
    void setBufferOffset(int bo) { bufferOffset = bo; }
    void setAnnotationChannel(bool b) { annotationChannel = b; }

    QString getLabel() const { return label; }
    double getPhysicalMax() const { return physicalMax; }
    double getPhysicalMin() const { return physicalMin; }
    int getDigitalMax() const { return digitalMax; }
    int getDigitalMin() const { return digitalMin; }
    int getSignalSampleCount() const { return smpPerRecord; }
    QString getPhysicalDimension() const { return physicalDimension; }
    QString getPrefilter() const { return prefilter; }
    QString getTransducer() const { return transducer; }
    QString getReserved() const { return reserved; }
    double getBitValue() const { return bitvalue; }
    int getOffset() const { return offset; }
    bool isAnnotationChannel() const { return annotationChannel; }
    int getBufferOffset() const { return bufferOffset; }
    long long getSamplePointer() const { return samplePointer; }
    void setSamplePointer(long long s) { samplePointer = s; }
    double getResolution() const { return (physicalMax - physicalMin) / (digitalMax - digitalMin); }

private:
    void updateBufferOffset();
    QString label;
    double physicalMax;
    double physicalMin;
    int digitalMax;
    int digitalMin;
    int smpPerRecord;
    QString physicalDimension;
    QString prefilter;
    QString transducer;
    QString reserved;
    int bufferOffset;
    double bitvalue;
    int offset;
    bool annotationChannel;
    bool edfFormat;
    long long samplePointer;
};

#endif  // EDFSIGNALHEADER_H
