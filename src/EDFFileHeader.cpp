#include "EDFFileHeader.h"

EDFFileHeader::EDFFileHeader()
{
    setStartDateTimeToCurrent();
    filetype = BDFPlus;
    dataRecordSize = 0;
    continuity = CONTINUOUS;
    dataRecordCount = 0;
    annotationIndex = 0;
}

EDFFileHeader::EDFFileHeader(FileType newFileType, CONTINUITY newCont)
{
    filetype = newFileType;
    continuity = newCont;
    setStartDateTimeToCurrent();
    dataRecordCount = 0;
    annotationIndex = 0;
    dataRecordSize = 0;
}

// EDFFileHeader::EDFFileHeader(const EDFFileHeader& orig){
//    filetype = orig.filetype;
//    continuity = orig.continuity;
//    startDate = orig.startDate;
//    startTime = orig.startTime;
//    patient = orig.patient;
//    setup = orig.setup;
//    dataRecordCount = orig.dataRecordCount;
//    dataRecordSize = orig.dataRecordSize;
//    annotationIndex = orig.annotationIndex;
//}

EDFFileHeader::~EDFFileHeader() {}

void EDFFileHeader::setFiletype(EDFFileHeader::FileType filetype) { this->filetype = filetype; }

void EDFFileHeader::setContinuity(CONTINUITY continuity) { this->continuity = continuity; }

void EDFFileHeader::setStartDateTimeToCurrent()
{
    startDate = EDFDate(QDate::currentDate());
    startTime = EDFTime(QTime::currentTime());
}

void EDFFileHeader::setPatient(EDFPatient patient) { this->patient = patient; }

QString EDFFileHeader::getHeaderString(int signalCount, long long longDataRecordDuration)
{
    QString hdr_str;

    // 8 ascii : version of this data format (0)
    if (getFiletype() == EDFFileHeader::EDF || getFiletype() == EDFFileHeader::EDFPlus)
    {
        hdr_str = QString("0       ");

        // fprintf(file, "0       ");
    }
    else
    {
        hdr_str.append(QChar(255));
        // fputc(255, file);
        // fprintf(file, "BIOSEMI");
        hdr_str.append("BIOSEMI");
    }
    // 80 ascii : local patient identification
    hdr_str.append(getPatient().toHeaderInfoString().toLatin1());
    // 80 ascii : local recording identification.
    EDFMeasurementSetup setup = getMeasurementSetup();

    setup.setDate(getStartDate());
    setMeasurementSetup(setup);
    hdr_str.append(getMeasurementSetup().toHeaderInfoString().toLatin1());

    // 8 ascii : startdate of recording (dd.mm.yy)
    hdr_str.append(getStartDate().toQDate().toString("dd.MM.yy").toLatin1());
    // 8 ascii : starttime of recording (hh.mm.ss).
    hdr_str.append(getStartTime().toQTime().toString("hh.mm.ss").toLatin1());

    // 8 ascii : number of bytes in header record
    hdr_str.append(QString::number((signalCount * 256) + 256).leftJustified(8, ' ', true).toLatin1());

    // 44 ascii : reserved
    if (getFiletype() == EDFFileHeader::EDF || getFiletype() == EDFFileHeader::EDFPlus)
    {
        hdr_str.append(QString("EDF+C").leftJustified(44, ' ', true).toLatin1());
    }
    else
    {
        hdr_str.append(QString("BDF+C").leftJustified(44, ' ', true).toLatin1());
    }

    // 8 ascii : number of data records (-1 if unknown)
    hdr_str.append(QString("-1      ").leftJustified(8, ' ', true).toLatin1());
    // 8 ascii : duration of a data record, in seconds
    if (longDataRecordDuration == 10000000LL)
    {
        hdr_str.append(QString("1       ").leftJustified(8, ' ', true).toLatin1());
    }
    else
    {
        hdr_str.append(QString::number(longDataRecordDuration, 'f', 3).leftJustified(8, ' ', true).toLatin1());
    }

    // 4 ascii : number of signals (ns) in data record
    hdr_str.append(QString::number(signalCount).leftJustified(4, ' ', true).toLatin1());

    return hdr_str;
}

bool EDFFileHeader::parseHeader(QString edf_hdr)
{
    int pos = 0;
    if (edf_hdr.size() != 256)
    {
        // error = EDFError(EDFError::FileReadError,"Could not read File-Header!");
        return false;
    }
    // 8 ascii : version of this data format (0)
    if (edf_hdr.mid(pos, 1).at(0).toLatin1() == -1)
    {
        setFiletype(EDFFileHeader::BDF);
    }
    else
    {
        setFiletype(EDFFileHeader::EDF);
    }
    pos += 8;
    // 80 ascii : local patient identification
    // is parsed when the filetype is known
    QString patientInfo = edf_hdr.mid(pos, 80);
    pos += 80;
    // 80 ascii : local recording identification.
    // is parsed when the filetype is known
    QString recordingInfo = edf_hdr.mid(pos, 80);
    pos += 80;
    // 8 ascii : startdate of recording (dd.mm.yy)
    EDFDate date;
    if (date.parseHeaderString(edf_hdr.mid(pos, 8)))
        setStartDate(date);
    pos += 8;
    // 8 ascii : starttime of recording (hh.mm.ss).
    EDFTime time;
    if (time.parseHeaderString(edf_hdr.mid(pos, 8)))
        setStartTime(time);
    pos += 8;
    // 8 ascii : number of bytes in header record
    pos += 8;
    // 44 ascii : reserved
    if (getFiletype() == EDFFileHeader::EDF)
    {
        if (edf_hdr.mid(pos, 44).contains("EDF+C"))
        {
            setContinuity(EDFFileHeader::CONTINUOUS);
            setFiletype(EDFFileHeader::EDFPlus);
        }
        else if (edf_hdr.mid(pos, 44).contains("EDF+D") == 0)
        {
            setContinuity(EDFFileHeader::DISCONTINUOUS);
            setFiletype(EDFFileHeader::EDFPlus);
        }
        else
        {
            setContinuity(EDFFileHeader::CONTINUOUS);
            setFiletype(EDFFileHeader::EDF);
        }
    }
    else
    {  // BDF
        if (edf_hdr.mid(pos, 44).contains("BDF+C"))
        {
            setContinuity(EDFFileHeader::CONTINUOUS);
            setFiletype(EDFFileHeader::BDFPlus);
        }
        else if (edf_hdr.mid(pos, 44).contains("BDF+D"))
        {
            setContinuity(EDFFileHeader::DISCONTINUOUS);
            setFiletype(EDFFileHeader::BDFPlus);
        }
        else
        {
            setContinuity(EDFFileHeader::CONTINUOUS);
            setFiletype(EDFFileHeader::BDF);
        }
    }
    EDFPatient pat;
    if (pat.parseHeaderString(patientInfo,
                              (getFiletype() == EDFFileHeader::BDFPlus || getFiletype() == EDFFileHeader::EDFPlus)))
        setPatient(pat);
    EDFMeasurementSetup setup;
    if (setup.parseHeaderString(QString(recordingInfo),
                                (getFiletype() == EDFFileHeader::BDFPlus || getFiletype() == EDFFileHeader::EDFPlus)))
        setMeasurementSetup(setup);
    pos += 44;

    // 8 ascii : number of data records (-1 if unknown)
    pos += 8;
    // 8 ascii : duration of a data record, in seconds
    pos += 8;
    // 4 ascii : number of signals (ns) in data record
    // setSignalCount(QString(edf_hdr.mid(pos,4).data()).toInt());
    //pos += 4;
    return true;
}
