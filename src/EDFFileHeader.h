#ifndef EDFFILEHEADER_H
#define EDFFILEHEADER_H
/*
 * Copyright (c) Holger Nahrstaedt
 */

#include <QString>
#include <QList>
#include <QtCore>
#include "EDFPatient.h"
#include "EDFTime.h"
#include "EDFDate.h"

#include "EDFMeasurementSetup.h"

/*!
 * \brief The EDFFileHeader class
 *
 */
class EDFFileHeader
{
public:
    enum FileType
    {
        EDF = 0,
        EDFPlus,
        BDF,
        BDFPlus
    };
    enum CONTINUITY
    {
        CONTINUOUS,
        DISCONTINUOUS
    };

    EDFFileHeader();
    EDFFileHeader(FileType, CONTINUITY);
    EDFFileHeader(const EDFFileHeader&) = default;
    virtual ~EDFFileHeader();
    //    EDFFileHeader& operator=(const EDFFileHeader&);

    void setFiletype(FileType);
    void setContinuity(CONTINUITY);
    //    void setFileDuration(int);
    void setStartDate(EDFDate date) { startDate = date; }
    void setStartTime(EDFTime time) { startTime = time; }
    void setStartDateTimeToCurrent();
    void setPatient(EDFPatient);
    void setMeasurementSetup(EDFMeasurementSetup s) { setup = s; }
    QString getHeaderString(int signalCount, long long longDataRecordDuration);
    bool parseHeader(QString edf_hdr);
    //    void setDataRecordDuration(double);
    //    void setDataRecordCount(int);
    //    void setDataRecordSize(int);
    //    void setAnnotationIndex(int);

    FileType getFiletype() const { return filetype; }
    CONTINUITY getContinuity() const { return continuity; }

    EDFDate getStartDate() const { return startDate; }
    EDFTime getStartTime() const { return startTime; }
    EDFPatient getPatient() const { return patient; }
    EDFMeasurementSetup getMeasurementSetup() const { return setup; }

    //    int        getDataRecordCount() {return dataRecordCount;}
    //    int        getDataRecordSize() {return dataRecordSize;}
    //    int        getAnnotationIndex(){return annotationIndex;}

    //    bool   hasAnnotations();
    //    double getTotalRecordingTime();

private:
    FileType filetype;
    CONTINUITY continuity;

    EDFDate startDate;
    EDFTime startTime;
    EDFPatient patient;
    EDFMeasurementSetup setup;

    int dataRecordCount;
    int dataRecordSize;
    int annotationIndex;
};

#endif  // EDFFILEHEADER_H
