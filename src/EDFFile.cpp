
#include "EDFFile.h"
#include <QDebug>

EDFFile::EDFFile()
    : edfHeader(nullptr),
      file(nullptr)
{

    init();
}

EDFFile::~EDFFile()
{
    clear();
}

bool EDFFile::open(QString name, FileStatus st)
{
    init();
    if (file == nullptr)
        file = new QFile(name, this);
    else
    {
        if (file->isOpen())
            file->close();
        file->setFileName(name);
    }

    status = st;
    if (st == EDFFile::writeOnly)
    {
        return openWriteOnly();
    }
    else if (st == readOnlyReadAllAnnotations || st == readOnlyReadAnnotations || st == readOnlyWithOutAnnotations)
    {
        return openReadOnly();
    }
    else
        return false;
}

bool EDFFile::openWriteOnly()
{
    if (file == nullptr)
        return false;
    if (!file->open(QIODevice::WriteOnly))
    {
        delete file;
        file = nullptr;
        error = EDFError(EDFError::FileWriteError, "File could not be written");
        return false;
    }
    longDataRecordDuration = EDFLIB_TIME_DIMENSION;
    annotationsOffset = 0;
    annotationsWrittenToFile = 0;
    annotationChannelCount = 0;
    annotationsDatrecsize = 114;
    dataRecordDuration = 1.0;
    signalWriteSequencePos = 0;
    annotationsDatarecords = 0LL;
    annotations.clear();

    return true;
}

bool EDFFile::openReadOnly()
{
    if (file == nullptr)
        return false;
    if (!file->open(QIODevice::ReadOnly))
    {
        delete file;
        file = nullptr;
        error = EDFError(EDFError::FileReadOnly, "File could not be opened");
        return false;
    }

    file->seek(0);
    if (!parseHeader(file->read(256)))
    {
        error = EDFError(EDFError::FileReadOnly, "Header could not be parsed!");
        return false;
    }
    if (!parseSignalHeader(file->read((getSignalCount()) * 256)))
    {
        error = EDFError(EDFError::FileReadOnly, "SignalHeader could not be parsed!");
        return false;
    }
    if (dataRecords < 1)
    {
        if (!repairDataRecords())
        {
            error = EDFError(EDFError::FileReadOnly, QString("Error, number of datarecords is %1, expected >0. Could not be repaired").arg(dataRecords));
            return false;
        }
    }

    if (status == readOnlyReadAllAnnotations || status == readOnlyReadAnnotations)
        edflib_get_annotations();
    return true;
}

bool EDFFile::repairDataRecords()
{
    if (!file->isOpen())
    {
        error = EDFError(EDFError::FileReadOnly, "File could not be opened");
        return false;
    }
    file->seek(0);
    qint64 filesize = file->size();
    qint64 l_tmp = 0;
    if (filesize != (((long long)recordSize * (long long)dataRecords)) + ((getSignalCount() * 256) + 256))
    {
        l_tmp = filesize - ((getSignalCount() * 256) + 256);
        dataRecords = l_tmp / (long long)recordSize;

        if (dataRecords > 0)
        {
            if (dataRecords > 99999999)
            {
                dataRecords = 99999999;
            }
            file->seek(236LL);

            l_tmp = ((getSignalCount() * 256) + 256);
            l_tmp += (long long)dataRecords * (long long)recordSize;

            if (l_tmp != filesize)
            {
                file->close();

                if (QFile::resize(file->fileName(), l_tmp) == false)
                {
                    error = EDFError(EDFError::FileReadOnly, "Can not resize file.");
                    return false;
                }

                if (!file->open(QIODevice::ReadOnly))
                {
                    error = EDFError(EDFError::FileReadOnly, "File could not be opened");
                    return false;
                }
            }
        }
        file->flush();
    }
    return true;

}
bool EDFFile::isOpen()
{
    if(!file)
    {
        return false;
    }
    return file->isOpen();
}
bool EDFFile::close()
{
    bool result = true;

    if (file != nullptr)
    {
        if (status != NotDefined && file->isOpen())
        {
            if (status == writeOnly)
            {
                // qDebug("dataRecords %d",dataRecords);
                if (dataRecords < 100000000LL)
                {
                    file->seek(236LL);
                    file->write(QString::number(static_cast<int>(dataRecords)).leftJustified(8, ' ', true).toLatin1());
                }
                if (header()->getFiletype() == EDFFileHeader::EDFPlus || header()->getFiletype() == EDFFileHeader::BDFPlus)
                {
                    writeAllAnnotations();
                }
            }
            file->close();
            status = NotDefined;
        }
        else
        {
            result = false;
        }
    }
    else
    {
        result = false;
    }

    if (!result)
    {
        error = EDFError(EDFError::CouldNotBeClosed, "File could not be closed!");        
    }

    return result;
}

void EDFFile::clear()
{
    //if (status != NotDefined && file && file->isOpen())
    //    file->close();
    if (file != nullptr)
    {
        delete file;
        file = nullptr;
    }
    if (edfHeader != nullptr)
    {
        delete edfHeader;
        edfHeader = nullptr;
    }

    for (int i = 0; i < signalsHeader.size(); i++)
    {
        delete signalsHeader[i];
    }
    signalsHeader.clear();

    for (int i = 0; i < signalsData.size(); i++)
    {
        delete signalsData[i];
    }
    signalsData.clear();
}

bool EDFFile::validSignal(int i) const
{
    if (i < signalsHeader.size())
        return true;
    else
        return false;
}

void EDFFile::setSignalCount(int signalCount, bool createAnnChannel)
{
    for (int i = 0; i < signalsHeader.size(); i++)
    {
        delete signalsHeader[i];
    }
    for (int i = 0; i < signalsData.size(); i++)
    {
        delete signalsData[i];
    }
    signalsHeader.clear();
    signalsData.clear();


    this->signalCount = signalCount;

    for (int i = 0; i < signalCount; i++)
    {
        signalsHeader.append(new EDFSignalHeader(false, (header()->getFiletype() == EDFFileHeader::EDF ||
                                                         header()->getFiletype() == EDFFileHeader::EDFPlus)));
        signalsData.append(new EDFSignalData());
    }
    if (createAnnChannel &&
        (header()->getFiletype() == EDFFileHeader::EDFPlus || header()->getFiletype() == EDFFileHeader::BDFPlus))
    {
        signalsHeader.append(new EDFSignalHeader(true, (header()->getFiletype() == EDFFileHeader::EDF ||
                                                        header()->getFiletype() == EDFFileHeader::EDFPlus)));
        signalsData.append(new EDFSignalData());
        annotationChannelCount = 1;
        this->signalCount++;
    }
}

QList<EDFSignalHeader*> EDFFile::signalHeader() const { return signalsHeader; }

QList<EDFSignalData*> EDFFile::signalData() const { return signalsData; }
double EDFFile::signalData(int channel, int pos)
{
    if (signalsData.size() > channel && signalsData.at(channel)->size() > pos)

        return signalsData.at(channel)->at(pos);
    else
    {
        error = EDFError(EDFError::MallocError, "Could not access Data!");
        //qDebug() << error.message() << endl;
        return 0;
    }
}
EDFSignalData* EDFFile::signalData(int channel)
{
    if (signalsData.size() > channel)
        return signalsData.at(channel);
    else
    {
        error = EDFError(EDFError::MallocError, "Could not access Data!");
        return nullptr;
    }
}

void EDFFile::clearSignalData(int channel)
{
    if (signalsData.size() > channel)
        signalsData[channel]->clear();
    else
    {
        error = EDFError(EDFError::MallocError, "Could not clear Data!");
        return;
    }
}

int EDFFile::getHeaderSize() const { return (signalCount * 256) + 256; }

void EDFFile::init()
{
    clear();
    error = EDFError(EDFError::NoError, "");
    status = NotDefined;
    signalCount = 0;
    annotationChannelCount = 0;
    signalWriteSequencePos = 0;
    edfHeader = new EDFFileHeader(EDFFileHeader::BDFPlus, EDFFileHeader::CONTINUOUS);
    dataRecords = 0;

    annotationChannel.clear();
    dataRecordDuration = 0;
    annotationsOffset = 0;
    annotationsWrittenToFile = 0;
    annotationsDatrecsize = 114;
    annotationsDatarecords = 0LL;

    recordSize = 0;
    longDataRecordDuration = 0;
}

bool EDFFile::writeHeader()
{
    if (file == nullptr)
    {
        error = EDFError(EDFError::MallocError, "Malloc error!");
        return false;
    }
    if (!file->isOpen())
    {
        error = EDFError(EDFError::FileWriteError, "File is not opened!");
        return false;
    }
    if (status != EDFFile::writeOnly)
    {
            error = EDFError(EDFError::FileReadOnly, "File is opened ReadOnly!");
            return false;
    }

    if (!file->seek(0))
    {
        error = EDFError(EDFError::FileWriteError, "Could not seek!");
        return false;
    }
    if (file->write(createHeader()) != 256)
    {
        error = EDFError(EDFError::FileWriteError, "Could not write 256 Bytes for Header info");
        return false;
    }
    if (file->write(createSignalHeader()) != (256 * getSignalCount()))
    {
        error = EDFError(EDFError::FileWriteError, "Could not write Signal Header info!");
        return false;
    }
    //        //8 ascii : version of this data format (0)
    //        file->write(header()->getHeaderString(getSignalCount(),longDataRecordDuration).toLatin1());

    //        //ns * 16 ascii : ns * label
    //        for (int i=0;i<getSignalCount();i++){
    //            file->write(signalHeader().at(i)->getLabel().leftJustified(16, ' ', true).toLatin1());
    //        }
    //        //ns * 80 ascii : ns * transducer type (e.g. AgAgCl electrode)
    //        for (int i=0;i<getSignalCount();i++){
    //            file->write(signalHeader().at(i)->getTransducer().leftJustified(80, ' ', true).toLatin1());
    //        }

    //        //ns * 8 ascii : ns * physical dimension (e.g. uV)
    //        for (int i=0;i<getSignalCount();i++){
    //            file->write(signalHeader().at(i)->getPhysicalDimension().leftJustified(8, ' ', true).toLatin1());
    //        }

    //        //ns * 8 ascii : ns * physical minimum (e.g. -500 or 34)
    //        for (int i=0;i<getSignalCount();i++){
    //            file->write(QString::number(signalHeader().at(i)->getPhysicalMin(),'f',7).leftJustified(8, ' ',
    //            true).toLatin1());
    //        }

    //        //ns * 8 ascii : ns * physical maximum (e.g. 500 or 40)
    //        for (int i=0;i<getSignalCount();i++){
    //            file->write(QString::number(signalHeader().at(i)->getPhysicalMax(),'f',7).leftJustified(8, ' ',
    //            true).toLatin1());
    //        }

    //        //ns * 8 ascii : ns * digital minimum (e.g. -2048)
    //        for (int i=0;i<getSignalCount();i++){
    //            file->write(QString::number(signalHeader().at(i)->getDigitalMin()).leftJustified(8, ' ',
    //            true).toLatin1());
    //        }

    //        //ns * 8 ascii : ns * digital maximum (e.g. 2047)
    //        for (int i=0;i<getSignalCount();i++){
    //            file->write(QString::number(signalHeader().at(i)->getDigitalMax()).leftJustified(8, ' ',
    //            true).toLatin1());
    //        }

    //        //ns * 80 ascii : ns * prefiltering (e.g. HP:0.1Hz LP:75Hz)
    //        for (int i=0;i<getSignalCount();i++){
    //            file->write(signalHeader().at(i)->getPrefilter().leftJustified(80, ' ', true).toLatin1());
    //        }

    //        //ns * 8 ascii : ns * nr of samples in each data record
    //        for (int i=0;i<getSignalCount();i++){
    //            file->write(QString::number(signalHeader().at(i)->getSignalSampleCount()).leftJustified(8, ' ',
    //            true).toLatin1());
    //        }

    //        //ns * 32 ascii : ns * reserved
    //        for (int i=0;i<getSignalCount();i++){
    //            file->write(signalHeader().at(i)->getReserved().leftJustified(32, ' ', true).toLatin1());
    //        }

    return true;
}
QByteArray EDFFile::createHeader()
{
    return header()->getHeaderString(getSignalCount(), longDataRecordDuration).toLatin1();
}
QByteArray EDFFile::createSignalHeader()
{
    QByteArray signalHeaderByteArray;
    // signalHeaderByteArray.resize(256*getSignalCount());
    // ns * 16 ascii : ns * label
    for (int i = 0; i < getSignalCount(); i++)
    {
        signalHeaderByteArray.append(signalHeader().at(i)->getLabel().leftJustified(16, ' ', true).toLatin1());
    }
    // ns * 80 ascii : ns * transducer type (e.g. AgAgCl electrode)
    for (int i = 0; i < getSignalCount(); i++)
    {
        signalHeaderByteArray.append(signalHeader().at(i)->getTransducer().leftJustified(80, ' ', true).toLatin1());
    }

    // ns * 8 ascii : ns * physical dimension (e.g. uV)
    for (int i = 0; i < getSignalCount(); i++)
    {
        signalHeaderByteArray.append(
            signalHeader().at(i)->getPhysicalDimension().leftJustified(8, ' ', true).toLatin1());
    }

    // ns * 8 ascii : ns * physical minimum (e.g. -500 or 34)
    for (int i = 0; i < getSignalCount(); i++)
    {
        signalHeaderByteArray.append(
            QString::number(signalHeader().at(i)->getPhysicalMin(), 'f', 7).leftJustified(8, ' ', true).toLatin1());
    }

    // ns * 8 ascii : ns * physical maximum (e.g. 500 or 40)
    for (int i = 0; i < getSignalCount(); i++)
    {
        signalHeaderByteArray.append(
            QString::number(signalHeader().at(i)->getPhysicalMax(), 'f', 7).leftJustified(8, ' ', true).toLatin1());
    }

    // ns * 8 ascii : ns * digital minimum (e.g. -2048)
    for (int i = 0; i < getSignalCount(); i++)
    {
        signalHeaderByteArray.append(
            QString::number(signalHeader().at(i)->getDigitalMin()).leftJustified(8, ' ', true).toLatin1());
    }

    // ns * 8 ascii : ns * digital maximum (e.g. 2047)
    for (int i = 0; i < getSignalCount(); i++)
    {
        signalHeaderByteArray.append(
            QString::number(signalHeader().at(i)->getDigitalMax()).leftJustified(8, ' ', true).toLatin1());
    }

    // ns * 80 ascii : ns * prefiltering (e.g. HP:0.1Hz LP:75Hz)
    for (int i = 0; i < getSignalCount(); i++)
    {
        signalHeaderByteArray.append(signalHeader().at(i)->getPrefilter().leftJustified(80, ' ', true).toLatin1());
    }

    // ns * 8 ascii : ns * nr of samples in each data record
    for (int i = 0; i < getSignalCount(); i++)
    {
        signalHeaderByteArray.append(
            QString::number(signalHeader().at(i)->getSignalSampleCount()).leftJustified(8, ' ', true).toLatin1());
    }

    // ns * 32 ascii : ns * reserved
    for (int i = 0; i < getSignalCount(); i++)
    {
        signalHeaderByteArray.append(signalHeader().at(i)->getReserved().leftJustified(32, ' ', true).toLatin1());
    }
    return signalHeaderByteArray;
}

bool EDFFile::parseHeader(const QByteArray& edf_hdr)
{
    int pos = 0;
    if (edf_hdr.size() != 256)
    {
        error = EDFError(EDFError::FileReadError, "Could not read File-Header!");
        return false;
    }
    // 8 ascii : version of this data format (0)
    if (edf_hdr.mid(pos, 1).at(0) == -1)
    {
        header()->setFiletype(EDFFileHeader::BDF);
    }
    else
    {
        header()->setFiletype(EDFFileHeader::EDF);
    }
    pos += 8;
    // 80 ascii : local patient identification
    // is parsed when the filetype is known
    QString patientInfo = QString(edf_hdr.mid(pos, 80).data());
    pos += 80;
    // 80 ascii : local recording identification.
    // is parsed when the filetype is known
    QString recordingInfo = QString(edf_hdr.mid(pos, 80).data());
    pos += 80;
    // 8 ascii : startdate of recording (dd.mm.yy)
    EDFDate date;
    if (date.parseHeaderString(QString(edf_hdr.mid(pos, 8).data())))
        header()->setStartDate(date);
    pos += 8;
    // 8 ascii : starttime of recording (hh.mm.ss).
    EDFTime time;
    if (time.parseHeaderString(QString(edf_hdr.mid(pos, 8).data())))
        header()->setStartTime(time);
    pos += 8;
    // 8 ascii : number of bytes in header record
    pos += 8;
    // 44 ascii : reserved
    if (header()->getFiletype() == EDFFileHeader::EDF)
    {
        if (QString(edf_hdr.mid(pos, 44).data()).contains("EDF+C"))
        {
            header()->setContinuity(EDFFileHeader::CONTINUOUS);
            header()->setFiletype(EDFFileHeader::EDFPlus);
        }
        else if (QString(edf_hdr.mid(pos, 44).data()).contains("EDF+D") == 0)
        {
            header()->setContinuity(EDFFileHeader::DISCONTINUOUS);
            header()->setFiletype(EDFFileHeader::EDFPlus);
        }
        else
        {
            header()->setContinuity(EDFFileHeader::CONTINUOUS);
            header()->setFiletype(EDFFileHeader::EDF);
        }
    }
    else
    {  // BDF
        if (QString(edf_hdr.mid(pos, 44).data()).contains("BDF+C"))
        {
            header()->setContinuity(EDFFileHeader::CONTINUOUS);
            header()->setFiletype(EDFFileHeader::BDFPlus);
        }
        else if (QString(edf_hdr.mid(pos, 44).data()).contains("BDF+D"))
        {
            header()->setContinuity(EDFFileHeader::DISCONTINUOUS);
            header()->setFiletype(EDFFileHeader::BDFPlus);
        }
        else
        {
            header()->setContinuity(EDFFileHeader::CONTINUOUS);
            header()->setFiletype(EDFFileHeader::BDF);
        }
    }
    EDFPatient pat;
    if (pat.parseHeaderString(patientInfo, (header()->getFiletype() == EDFFileHeader::BDFPlus ||
                                            header()->getFiletype() == EDFFileHeader::EDFPlus)))
    {
        header()->setPatient(pat);

        // qDebug()<<"pat code"<<pat.getCode()<<endl;
    }

    EDFMeasurementSetup setup;
    if (setup.parseHeaderString(recordingInfo, (header()->getFiletype() == EDFFileHeader::BDFPlus ||
                                                header()->getFiletype() == EDFFileHeader::EDFPlus)))
    {
        header()->setMeasurementSetup(setup);
        // qDebug()<<recordingInfo<<endl;
        // qDebug()<<"admin code"<<setup.getAdminCode()<<endl;
    }
    pos += 44;

    // 8 ascii : number of data records (-1 if unknown)
    dataRecords = QString(edf_hdr.mid(pos, 8).data()).toInt();
    // qDebug("dataRecords %d",dataRecords);
    pos += 8;
    // 8 ascii : duration of a data record, in seconds
    dataRecordDuration = QString(edf_hdr.mid(pos, 8).data()).toDouble();
    // longDataRecordDuration=QString(edf_hdr.mid(pos,8).data()).toLongLong();
    // dataRecordDuration = longDataRecordDuration * dataRecords;
    longDataRecordDuration = dataRecordDuration * EDFLIB_TIME_DIMENSION;
    pos += 8;
    // 4 ascii : number of signals (ns) in data record
    setSignalCount(QString(edf_hdr.mid(pos, 4).data()).toInt(), false);
    //pos += 4;

    return true;
}
bool EDFFile::parseSignalHeader(const QByteArray& edf_hdr)
{
    if (edf_hdr.size() != (getSignalCount() * 256))
    {
        error = EDFError(EDFError::FileReadError, "Could not read File-Header!");
        return false;
    }
    // edf_hdr = file->read((getSignalCount()) * 256);
    // ns * 16 ascii : ns * label
    for (int i = 0; i < getSignalCount(); i++)
    {
        signalHeader(i)->setLabel(QString(edf_hdr.mid((i * 16), 16).data()));
        if (signalHeader(i)->getLabel().contains("BDF Annotation"))
        {
            annotationChannel.push_back(i);
            annotationChannelCount++;
            signalHeader(i)->setAnnotationChannel(true);
        }
        else if (signalHeader(i)->getLabel().contains("EDF Annotation"))
        {
            annotationChannel.push_back(i);
            annotationChannelCount++;
            signalHeader(i)->setAnnotationChannel(true);
        }
    }
    // ns * 80 ascii : ns * transducer type (e.g. AgAgCl electrode)
    for (int i = 0; i < getSignalCount(); i++)
    {
        signalHeader(i)->setTransducer(QString(edf_hdr.mid((getSignalCount() * 16) + (i * 80), 80).data()));
    }
    // ns * 8 ascii : ns * physical dimension (e.g. uV)
    for (int i = 0; i < getSignalCount(); i++)
    {
        signalHeader(i)->setPhysicalDimension(QString(edf_hdr.mid((getSignalCount() * 96) + (i * 8), 8).data()));
    }
    // ns * 8 ascii : ns * physical minimum (e.g. -500 or 34)
    for (int i = 0; i < getSignalCount(); i++)
    {
        signalHeader(i)->setPhysicalMin(QString(edf_hdr.mid((getSignalCount() * 104) + (i * 8), 8).data()).toDouble());
    }
    // ns * 8 ascii : ns * physical maximum (e.g. 500 or 40)
    for (int i = 0; i < getSignalCount(); i++)
    {
        signalHeader(i)->setPhysicalMax(QString(edf_hdr.mid((getSignalCount() * 112) + (i * 8), 8).data()).toDouble());
    }
    // ns * 8 ascii : ns * digital minimum (e.g. -2048)
    for (int i = 0; i < getSignalCount(); i++)
    {
        signalHeader(i)->setDigitalMin(QString(edf_hdr.mid((getSignalCount() * 120) + (i * 8), 8).data()).toInt());
    }
    // ns * 8 ascii : ns * digital maximum (e.g. 2047)
    for (int i = 0; i < getSignalCount(); i++)
    {
        signalHeader(i)->setDigitalMax(QString(edf_hdr.mid((getSignalCount() * 128) + (i * 8), 8).data()).toInt());
    }
    // ns * 80 ascii : ns * prefiltering (e.g. HP:0.1Hz LP:75Hz)
    for (int i = 0; i < getSignalCount(); i++)
    {
        signalHeader(i)->setPrefilter(QString(edf_hdr.mid((getSignalCount() * 136) + (i * 80), 80).data()));
    }
    // ns * 8 ascii : ns * nr of samples in each data record
    recordSize = 0;
    for (int i = 0; i < getSignalCount(); i++)
    {
        signalHeader(i)
            ->setSignalSampleFrequency(QString(edf_hdr.mid((getSignalCount() * 216) + (i * 8), 8).data()).toInt());
        recordSize += signalHeader(i)->getSignalSampleCount();
    }
    if (header()->getFiletype() == EDFFileHeader::BDF || header()->getFiletype() == EDFFileHeader::BDFPlus)
    {
        recordSize *= 3;
    }
    else
    {
        recordSize *= 2;
    }
    // ns * 32 ascii : ns * reserved
    for (int i = 0; i < getSignalCount(); i++)
    {
        signalHeader(i)->setReserved(QString(edf_hdr.mid((getSignalCount() * 224) + (i * 32), 32).data()));
    }

    int n = 0;

    for (int i = 0; i < getSignalCount(); i++)
    {
        //          edfhdr->edfparam[i].buf_offset = n;
        signalHeader(i)->setBufferOffset(n);
        //          if(edfhdr->bdf)  n += edfhdr->edfparam[i].smp_per_record * 3;
        //          else  n += edfhdr->edfparam[i].smp_per_record * 2;
        if (header()->getFiletype() == EDFFileHeader::BDF || header()->getFiletype() == EDFFileHeader::BDFPlus)
            n += signalHeader(i)->getSignalSampleCount() * 3;
        else
            n += signalHeader(i)->getSignalSampleCount() * 2;

        //          edfhdr->edfparam[i].bitvalue = (edfhdr->edfparam[i].phys_max - edfhdr->edfparam[i].phys_min) /
        //          (edfhdr->edfparam[i].dig_max - edfhdr->edfparam[i].dig_min);
        //          edfhdr->edfparam[i].offset = edfhdr->edfparam[i].phys_max / edfhdr->edfparam[i].bitvalue -
        //          edfhdr->edfparam[i].dig_max;
    }
    return true;
}

QVector<double> EDFFile::getInterval(int edfsignal, uint startTime, uint endTime)
{
    int numberOfSignals = getSignalCount();
    if((header()->getFiletype() == EDFFileHeader::EDFPlus) ||
     (header()->getFiletype() == EDFFileHeader::BDFPlus))
        numberOfSignals--;

    if (edfsignal >= numberOfSignals)
    {
        qWarning() << "EDFFile::getInterval(): edfsignal " << edfsignal
                   << " cannot be there. The signal count is: " << numberOfSignals;
        return QVector<double>();
    }

    int samplingFrequency = signalHeader(edfsignal)->getSignalSampleCount();

    if (startTime >= endTime || endTime >= getDataRecords())
    {
        qWarning() << "EDFFile::getInterval(): starTime >= entTime: "
                   << startTime << " >= " << endTime
                   << " or endTime <= " << getDataRecords();
        return QVector<double>();
    }

    long long offset = samplingFrequency * startTime;
    long long res = 0;
    if (offset != (res = seek(edfsignal, offset, SeekSet)))
    {
        qWarning() << "EDFFile::getInterval(): seek failed with return: " << res
                   << " and offset " << offset;
    }
    int nSamplesToRead = (endTime - startTime) * samplingFrequency;
    if (nSamplesToRead != (res = readPhysicalData(edfsignal, nSamplesToRead)))
    {
        qWarning() << "EDFFile::getInterval(): readPhysicalData() failed with "
                   << "return value " << res << " and nSamplesToRead: "
                   << nSamplesToRead;
    }
    return signalData(edfsignal)->getData();
}

long long EDFFile::seek(int channel, long long offset, Whence whence)
{
    long long smp_in_file;

    // TODO: conversion edfchannel -> channel (handle annotation channel)

    // smp_in_file = edfheaderlist[handle]->edfparam[channel].smp_per_record * edfheaderlist[handle]->datarecords;
    smp_in_file = signalHeader(channel)->getSignalSampleCount() * getDataRecords();

    if (whence == SeekSet)
    {
        signalHeader(channel)->setSamplePointer(offset);
        // edfheaderlist[handle]->edfparam[channel].sample_pntr = offset;
    }

    if (whence == SeekCur)
    {
        // edfheaderlist[handle]->edfparam[channel].sample_pntr += offset;
        signalHeader(channel)->setSamplePointer(signalHeader(channel)->getSamplePointer() + offset);
    }

    if (whence == SeekEnd)
    {
        // edfheaderlist[handle]->edfparam[channel].sample_pntr =
        //  (edfheaderlist[handle]->edfparam[channel].smp_per_record * edfheaderlist[handle]->datarecords) + offset;
        signalHeader(channel)->setSamplePointer(smp_in_file + offset);
    }

    if (signalHeader(channel)->getSamplePointer() > smp_in_file)
    {
        signalHeader(channel)->setSamplePointer(smp_in_file);
    }

    if (signalHeader(channel)->getSamplePointer() < 0LL)
    {
        signalHeader(channel)->setSamplePointer(0LL);
    }

    return (signalHeader(channel)->getSamplePointer());
}

long long EDFFile::tell(int channel)
{
    // TODO: conversion edfchannel -> channel (handle annotation channel)
    return (signalHeader(channel)->getSamplePointer());
}

void EDFFile::rewind(int channel)
{
    // TODO: conversion edfchannel -> channel (handle annotation channel)
    signalHeader(channel)->setSamplePointer(0LL);
}

int EDFFile::readPhysicalData(int channel, int n)
{
    int bytes_per_smpl = 2, i;

    char tmp[1];
    double phys_bitvalue, phys_offset;

    long long smp_in_file, offset, sample_pntr, smp_per_record, jump;

    union
    {
        signed int one_signed;
        signed short two_signed[2];
        unsigned char four[4];
    } var;

    if (signalHeader().size() < channel)
    {
        error = EDFError(EDFError::MallocError, "Could not Access channel");
        //qDebug() << error.message() << endl;
        return -1;
    }

    if (header()->getFiletype() == EDFFileHeader::EDF || header()->getFiletype() == EDFFileHeader::EDFPlus)
    {
        bytes_per_smpl = 2;
    }

    if (header()->getFiletype() == EDFFileHeader::BDF || header()->getFiletype() == EDFFileHeader::BDFPlus)
    {
        bytes_per_smpl = 3;
    }

    // smp_in_file = hdr->edfparam[channel].smp_per_record * hdr->datarecords;
    smp_in_file = signalHeader(channel)->getSignalSampleCount() * getDataRecords();

    // if((hdr->edfparam[channel].sample_pntr + n) > smp_in_file)
    if ((signalHeader(channel)->getSamplePointer() + n) > smp_in_file)
    {
        n = smp_in_file - signalHeader(channel)->getSamplePointer();

        if (n == 0)
        {
            return (0LL);
        }

        if (n < 0)
        {
            error = EDFError(EDFError::MallocError, "Could not Access Data");
            //qDebug() << error.message() << endl;
            return (-1);
        }
    }

    //    file = hdr->file_hdl;

    offset = getHeaderSize();
    //    offset += (hdr->edfparam[channel].sample_pntr / hdr->edfparam[channel].smp_per_record) * hdr->recordsize;
    offset += (signalHeader(channel)->getSamplePointer() / signalHeader(channel)->getSignalSampleCount()) * recordSize;
    //    offset += hdr->edfparam[channel].buf_offset;
    offset += signalHeader(channel)->getBufferOffset();
    //    offset += ((hdr->edfparam[channel].sample_pntr % hdr->edfparam[channel].smp_per_record) * bytes_per_smpl);
    offset +=
        (signalHeader(channel)->getSamplePointer() % signalHeader(channel)->getSignalSampleCount()) * bytes_per_smpl;

    if (file == nullptr)
    {
        error = EDFError(EDFError::FileReadError, "Could not Access Data");
        //qDebug() << error.message() << endl;
        return (-1);
    }
    if (!file->seek(offset))
    {
        error = EDFError(EDFError::FileReadError, "Could not Access Data");
        //qDebug() << error.message() << endl;
        return (-1);
    }

    //    sample_pntr = hdr->edfparam[channel].sample_pntr;
    sample_pntr = signalHeader(channel)->getSamplePointer();
    //    smp_per_record = hdr->edfparam[channel].smp_per_record;
    smp_per_record = signalHeader(channel)->getSignalSampleCount();
    //    jump = hdr->recordsize - (smp_per_record * bytes_per_smpl);
    jump = recordSize - (smp_per_record * bytes_per_smpl);

    //    phys_bitvalue = hdr->edfparam[channel].bitvalue;
    phys_bitvalue = signalHeader(channel)->getBitValue();

    //    phys_offset = hdr->edfparam[channel].offset;
    phys_offset = signalHeader(channel)->getOffset();
    // qDebug("channel %d sample_pntr %d offset %d",channel,sample_pntr,offset);

    if (header()->getFiletype() == EDFFileHeader::EDF || header()->getFiletype() == EDFFileHeader::EDFPlus)
    {
        i = 0;
        //for (i = 0; i < n; i++)
        while (i<n)
        {
            if (!(sample_pntr % smp_per_record))
            {
                if (i)
                {
                    // fseeko(file, jump, SEEK_CUR);
                    file->seek(file->pos() + jump);
                }
            }
            unsigned int samplesToRead = (smp_per_record - (sample_pntr % smp_per_record));
            if (samplesToRead > static_cast<unsigned int> (n))
                samplesToRead = n;
            if (samplesToRead * 2 > file->bytesAvailable())
                samplesToRead = file->bytesAvailable() / 2;
            QByteArray readBytes = file->read(samplesToRead * 2);
            i += samplesToRead;
            for (unsigned int j = 0; j < samplesToRead; j++)
            {
                // file->readData(tmp,1);
                // tmp[0] = file->read(1).at(0);
                //var.four[0] = file->read(1).at(0);
                var.four[0] = readBytes[j * 2];
                // var.four[0] = fgetc(file);
                // tmp = fgetc(file);
                // file->readData(tmp,1);
                //tmp[0] = file->read(1).at(0);
                tmp[0] = readBytes[j * 2 + 1];
                if (tmp[0] == EOF)
                {
                    error = EDFError(EDFError::FileReadError, "EOF");
                    //qDebug() << error.message() << endl;
                    return (-1);
                }
                var.four[1] = tmp[0];

                var.two_signed[0] += phys_offset;

                // buf[i] = phys_bitvalue * (double)var.two_signed[0];
                signalData(channel)->addPhysicalData(phys_bitvalue * static_cast<double>(var.two_signed[0]));
                sample_pntr++;
            }
        }
    }

    if (header()->getFiletype() == EDFFileHeader::BDF || header()->getFiletype() == EDFFileHeader::BDFPlus)
    {
        i = 0;
        //for (i = 0; i < n; i++)
        while (i<n)
        {
            if (!(sample_pntr % smp_per_record))
            {
                if (i)
                {
                    // fseeko(file, jump, SEEK_CUR);
                    file->seek(file->pos() + jump);
                }
            }
            unsigned int samplesToRead = (smp_per_record - (sample_pntr % smp_per_record));
            if (samplesToRead > static_cast<unsigned int> (n))
                samplesToRead = n;
            if (samplesToRead * 3 > file->bytesAvailable())
                samplesToRead = file->bytesAvailable() / 3;
            QByteArray readBytes = file->read(samplesToRead * 3);
            i += samplesToRead;
            if (file->atEnd() && i < n)
            {
                error = EDFError(EDFError::FileReadError, "EOF");
                //qDebug("n %d", n);
                //qDebug() << error.message() << endl;
                return (-1);
            }
            // var.four[0] = fgetc(file);
            // var.four[1] = fgetc(file);
            // tmp = fgetc(file);
            // file->readData(tmp,1);
            // tmp = file->read(1).data();
            for (unsigned int j = 0; j < samplesToRead; j++)
            {
                
                //var.four[0] = file->read(1).at(0);
                var.four[0] = readBytes[j*3];
                // file->readData(tmp,1);
                // tmp = file->read(1).data();
                //var.four[1] = file->read(1).at(0);
                var.four[1] = readBytes[j * 3+1];
                // file->readData(tmp,1);
                //tmp[0] = file->read(1).at(0);
                tmp[0] = readBytes[j * 3+2];

                // if(tmp[0]==EOF)

                var.four[2] = tmp[0];

                if (var.four[2] & 0x80)
                {
                    var.four[3] = 0xff;
                }
                else
                {
                    var.four[3] = 0x00;
                }

                // var.one_signed += phys_offset;
                signalData(channel)->addPhysicalData(phys_bitvalue * (phys_offset + static_cast<double>(var.one_signed)));
                // buf[i] = phys_bitvalue *(phys_offset + (double)var.one_signed);

                sample_pntr++;
            }
            
        }
    }

    //    hdr->edfparam[channel].sample_pntr = sample_pntr;
    signalHeader(channel)->setSamplePointer(sample_pntr);

    return (n);
}

bool EDFFile::writePhysicalData(double* buf, int size)
{
    if (size != signalHeader().at(signalWriteSequencePos)->getSignalSampleCount())
    {
        qDebug() << "Error: EDFFile::writePhysicalData size != signalHeader().at(signalWriteSequencePos)->getSignalSampleCount() == "
                 << signalHeader().at(signalWriteSequencePos)->getSignalSampleCount();
        return false;
    }

    if (file == nullptr)
    {
        qDebug() << "Error: EDFFile::writePhysicalData file == nullptr";
        return false;
    }

    for (int i = 0; i < signalHeader().at(signalWriteSequencePos)->getSignalSampleCount(); i++)
    {
        int value = buf[i] / signalHeader().at(signalWriteSequencePos)->getBitValue();

        value -= signalHeader().at(signalWriteSequencePos)->getOffset();

        if (value > signalHeader().at(signalWriteSequencePos)->getDigitalMax())
        {
            value = signalHeader().at(signalWriteSequencePos)->getDigitalMax();
        }

        if (value < signalHeader().at(signalWriteSequencePos)->getDigitalMin())
        {
            value = signalHeader().at(signalWriteSequencePos)->getDigitalMin();
        }

        file->putChar(value & 0xff);

        if (!file->putChar((value >> 8) & 0xff))
        {
            qDebug() << "Error: EDFFile::writePhysicalData: "
                        "!file->putChar((value >> 8) & 0xff)";
            return false;
        }

        if (header()->getFiletype() == EDFFileHeader::BDF || header()->getFiletype() == EDFFileHeader::BDFPlus)
        {
            file->putChar((value >> 16) & 0xff);
        }
    }
    signalWriteSequencePos++;
    if (signalWriteSequencePos == getSignalCount() - 1 &&
        (header()->getFiletype() == EDFFileHeader::EDFPlus || header()->getFiletype() == EDFFileHeader::BDFPlus))
    {
        signalWriteSequencePos = 0;

        int p = edflib_fprint_ll_number_nonlocalized((dataRecords * longDataRecordDuration) / EDFLIB_TIME_DIMENSION, 0, 1);
        if (longDataRecordDuration % EDFLIB_TIME_DIMENSION)
        {
            file->putChar('.');
            p++;
            p += edflib_fprint_ll_number_nonlocalized((dataRecords * longDataRecordDuration) % EDFLIB_TIME_DIMENSION, 7, 0);
        }
        file->putChar(20);
        file->putChar(20);
        p += 2;
        for (; p < 114; p++)
        {
            file->putChar(0);
        }

        dataRecords++;
        // qDebug("dataRecords %d",dataRecords);
        // if (dataRecords>1)
        writeAllAnnotations();
        file->flush();
    }
    else if (signalWriteSequencePos == getSignalCount() &&
             (header()->getFiletype() == EDFFileHeader::EDF || header()->getFiletype() == EDFFileHeader::BDF))
    {
        signalWriteSequencePos = 0;
        dataRecords++;
        file->flush();
    }

    return true;
}

bool EDFFile::writeAllAnnotations()
{
    char str[114 * 2];

    if (file == nullptr)
        return false;

    if (annotations.size() == 0)
        return true;
    if (annotations.size() == annotationsWrittenToFile)
        return true;
    if (annotationsOffset == 0)
    {
        annotationsDatrecsize = 114;
        annotationsOffset = static_cast<long long>((getSignalCount()) * 256 + 256);

        for (int i = 0; i < getSignalCount(); i++)
        {
            if (!signalHeader().at(i)->isAnnotationChannel())
            {
                if (header()->getFiletype() == EDFFileHeader::EDF || header()->getFiletype() == EDFFileHeader::EDFPlus)
                {
                    annotationsOffset += static_cast<long long>(signalHeader().at(i)->getSignalSampleCount() * 2);

                    annotationsDatrecsize += (signalHeader().at(i)->getSignalSampleCount() * 2);
                }
                else
                {
                    annotationsOffset += static_cast<long long>(signalHeader().at(i)->getSignalSampleCount() * 3);

                    annotationsDatrecsize += (signalHeader().at(i)->getSignalSampleCount() * 3);
                }
            }
        }
    }
    if (annotationsDatrecsize == 0)
        return true;

    long long oldOffset = file->pos();

    //    if (oldOffset<(annotationsOffset+annotationsDatrecsize))
    //        qDebug("write annotation offset %d datasize %d pos %d",annotationsOffset,annotationsDatrecsize,oldOffset);

    //while (annotations.size() > 0)

    for (int k = annotationsWrittenToFile; k < annotations.size(); k++)
    {
        if (!file->seek(annotationsOffset))
        {
            break;
        }
        if (oldOffset < (annotationsOffset + annotationsDatrecsize))
        {
            //qDebug("write annotation offset %d datasize %d pos %d", annotationsOffset, annotationsDatrecsize,
            //       oldOffset);
            //break;
        }

        //EDFAnnotation annot = annotations.first();
        EDFAnnotation annot = annotations.at(k);

        int p = edflib_sprint_ll_number_nonlocalized(
            str, (annotationsDatarecords * longDataRecordDuration) / EDFLIB_TIME_DIMENSION, 0, 1);

        if (longDataRecordDuration % EDFLIB_TIME_DIMENSION)
        {
            str[p++] = '.';
            p += edflib_sprint_ll_number_nonlocalized(
                str + p, (annotationsDatarecords * longDataRecordDuration) % EDFLIB_TIME_DIMENSION, 7, 0);
        }
        str[p++] = 20;
        str[p++] = 20;
        str[p++] = 0;

        p += edflib_sprint_ll_number_nonlocalized(str + p, annot.getOnsetIn100USec() / 10000LL, 0, 1);
        if (annot.getOnsetIn100USec() % 10000LL)
        {
            str[p++] = '.';
            p += edflib_sprint_ll_number_nonlocalized(str + p, annot.getOnsetIn100USec() % 10000LL, 4, 0);
        }
        if (annot.getDurationIn100USec() >= 0LL)
        {
            str[p++] = 21;
            p += edflib_sprint_ll_number_nonlocalized(str + p, annot.getDurationIn100USec() / 10000LL, 0, 0);
            if (annot.getDurationIn100USec() % 10000LL)
            {
                str[p++] = '.';
                p += edflib_sprint_ll_number_nonlocalized(str + p, annot.getDurationIn100USec() % 10000LL, 4, 0);
            }
        }
        str[p++] = 20;
        for (int i = 0; i < 40; i++)
        {
            if (annot.getText().size() > i)
                str[p++] = annot.getText().at(i).toLatin1();  // annot2->annotation[i];
        }
        str[p++] = 20;

        for (; p < 114; p++)
        {
            str[p] = 0;
        }

        file->write(str, 114);

        annotationsOffset += annotationsDatrecsize;

        annotationsDatarecords++;

        annotationsWrittenToFile++;

        if (annotationsDatarecords >= dataRecords)
        {
            break;
        }

        //annotations.pop_front();
    }
    file->seek(oldOffset);
    return true;
}

int EDFFile::edflib_fprint_ll_number_nonlocalized(long long q, int minimum, int sign) const
{
    int flag = 0, i, j = 0;

    long long base = 1000000000000000000LL;

    if (minimum < 0)
    {
        minimum = 0;
    }

    if (minimum > 18)
    {
        flag = 1;
    }

    if (q < 0LL)
    {
        file->putChar('-');

        j++;

        q = -q;
    }
    else
    {
        if (sign)
        {
            file->putChar('+');

            j++;
        }
    }

    for (i = 19; i; i--)
    {
        if (minimum == i)
        {
            flag = 1;
        }

        int z = q / base;

        q %= base;

        if (z || flag)
        {
            file->putChar('0' + z);

            j++;

            flag = 1;
        }

        base /= 10LL;
    }

    if (!flag)
    {
        file->putChar('0');

        j++;
    }

    return (j);
}

int EDFFile::edflib_sprint_ll_number_nonlocalized(char* str, long long q, int minimum, int sign)
{
    int flag = 0, i, j = 0;

    long long base = 1000000000000000000LL;

    if (minimum < 0)
    {
        minimum = 0;
    }

    if (minimum > 18)
    {
        flag = 1;
    }

    if (q < 0LL)
    {
        str[j++] = '-';

        q = -q;
    }
    else
    {
        if (sign)
        {
            str[j++] = '+';
        }
    }

    for (i = 19; i; i--)
    {
        if (minimum == i)
        {
            flag = 1;
        }

        int z = q / base;

        q %= base;

        if (z || flag)
        {
            str[j++] = '0' + z;

            flag = 1;
        }

        base /= 10LL;
    }

    if (!flag)
    {
        str[j++] = '0';
    }

    str[j] = 0;

    return (j);
}
EDFSignalHeader* EDFFile::annotationHeader(int annotchannel)
{
    if (annotationChannel.size() > annotchannel)
    {
        return signalsHeader[annotationChannel[annotchannel]];
    }
    else
        return nullptr;
}

int EDFFile::edflib_get_annotations()
{
    int i, j, k, p, r = 0, n,
        edfsignals,
        datarecords,
        max,
        onset,
        duration,
        duration_start,
        zero,
        max_tal_ln,
        error,
        annots_in_record,
        annots_in_tal,
        samplesize = 2;

    QByteArray cnv_buf;
    QByteArray scratchpad;
    QByteArray duration_in_txt;
    QByteArray time_in_txt;
     

    long long  elapsedtime,
        time_tmp = 0;



    EDFAnnotation new_annotation;
    edfsignals = getSignalCount();
    //edfparam = edfhdr->edfparam;
    datarecords = getDataRecords();

    if (header()->getFiletype() == EDFFileHeader::EDFPlus)
    {
        samplesize = 2;
    }
    if (header()->getFiletype() == EDFFileHeader::BDFPlus)
    {
        samplesize = 3;
    }

    max_tal_ln = 0;

    for (i = 0; i<annotationChannel.size(); i++)
    {
        if (max_tal_ln < annotationHeader(i)->getSignalSampleCount()* samplesize)
            max_tal_ln = annotationHeader(i)->getSignalSampleCount() * samplesize;
        //if (max_tal_ln<edfparam[annot_ch[i]].smp_per_record * samplesize)  max_tal_ln = edfparam[annot_ch[i]].smp_per_record * samplesize;
    }

    if (max_tal_ln<128)  max_tal_ln = 128;

    scratchpad.resize(max_tal_ln + 3);
    duration_in_txt.resize(max_tal_ln + 3);
    time_in_txt.resize(max_tal_ln + 3);


    //if (fseeko(inputfile, (long long)((edfsignals + 1) * 256), SEEK_SET))
    if (!file->seek((qint64)((edfsignals + 1) * 256)))
    {

        return(2);
    }

    elapsedtime = 0;

    for (i = 0; i<datarecords; i++)
    {
        //if (fread(cnv_buf, recordSize, 1, inputfile) != 1)
        cnv_buf = file->read(recordSize);
        if (cnv_buf.size() <= 0)
        {
            return(2);
        }


        /************** process annotationsignals (if any) **************/

        error = 0;

        for (r = 0; r<annotationChannel.size(); r++)
        {
            n = 0;
            zero = 0;
            onset = 0;
            duration = 0;
            duration_start = 0;

            annots_in_tal = 0;
            annots_in_record = 0;

            p = annotationHeader(r)->getBufferOffset();
            max = annotationHeader(r)->getSignalSampleCount() * samplesize;

            /************** process one annotation signal ****************/

            if (cnv_buf.at(p + max - 1) != 0)
            {
                error = 5;
                return(9);
            }

            if (!r)  /* if it's the first annotation signal, then check */
            {       /* the timekeeping annotation */
                error = 1;

                for (k = 0; k<(max - 2); k++)
                {
                    scratchpad[k] = cnv_buf[p + k];

                    if (scratchpad.at(k) == 20)
                    {
                        if (cnv_buf.at(p + k + 1) != 20)
                        {
                            error = 6;
                            return(9);
                        }
                        scratchpad[k] = 0;
                        if (edflib_is_onset_number(scratchpad.data()))
                        {
                            error = 36;
                            return(9);
                        }
                        else
                        {
                            time_tmp = edflib_get_long_time(scratchpad.data());
                            if (i)
                            {
                                if (header()->getContinuity() == EDFFileHeader::DISCONTINUOUS)
                                {
                                    if ((time_tmp - elapsedtime)<longDataRecordDuration)
                                    {
                                        error = 4;
                                        return(9);
                                    }
                                }
                                else
                                {
                                    if ((time_tmp - elapsedtime) != longDataRecordDuration)
                                    {
                                        error = 3;
                                        return(9);
                                    }
                                }
                            }
                            else
                            {
                                if (time_tmp >= EDFLIB_TIME_DIMENSION)
                                {
                                    error = 2;
                                    return(9);
                                }
                                else
                                {
                                    starttime_offset = time_tmp;
                                }
                            }
                            elapsedtime = time_tmp;
                            error = 0;
                            break;
                        }
                    }
                }
            }

            for (k = 0; k<max; k++)
            {
                scratchpad[n] = cnv_buf[p + k];

                if (scratchpad.at(n)==0)
                {
                    if (!zero)
                    {
                        if (k)
                        {
                            if (cnv_buf.at(p + k - 1) != 20)
                            {
                                error = 33;
                                return(9);
                            }
                        }
                        n = 0;
                        onset = 0;
                        duration = 0;
                        duration_start = 0;
                        scratchpad[0] = 0;
                        annots_in_tal = 0;
                    }
                    zero++;
                    continue;
                }
                if (zero>1)
                {
                    error = 34;
                    return(9);
                }
                zero = 0;

                if ((scratchpad.at(n) == 20) || (scratchpad.at(n) == 21))
                {
                    if (scratchpad.at(n) == 21)
                    {
                        if (duration || duration_start || onset || annots_in_tal)
                        {               /* it's not allowed to have multiple duration fields */
                            error = 35;   /* in one TAL or to have a duration field which is   */
                            return(9);    /* not immediately behind the onsetfield             */
                        }
                        duration_start = 1;
                    }

                    if ((scratchpad.at(n) == 20) && onset && (!duration_start))
                    {
                        if (r || annots_in_record)
                        {
                            if (n >= 0)
                            {
                                

                                if (duration)
                                    new_annotation.setDurationInEDFTime(edflib_get_long_time(duration_in_txt.data()));
                                else  
                                    new_annotation.setDurationInEDFTime(0);
                                QString ann;
                                for (j = 0; j<n; j++)
                                {
                                    if (j == EDFLIB_MAX_ANNOTATION_LEN)  break;
                                    //new_annotation->annotation[j] = scratchpad[j];
                                    ann.append(scratchpad.at(j));
                                }
                                //new_annotation->annotation[j] = 0;
                                new_annotation.setText(ann);

                                new_annotation.setOnsetInEDFTime(edflib_get_long_time(time_in_txt.data()));
                                annotations.push_back(new_annotation);

                                if (status == readOnlyReadAnnotations)
                                {
                                    if (new_annotation.getText().contains("Recording ends"))
                                    {
                                        if (annotationChannel.size() == 1)
                                        {
                                            return(9);
                                        }
                                    }
                                }
                            }
                        }

                        annots_in_tal++;
                        annots_in_record++;
                        n = 0;
                        continue;
                    }

                    if (!onset)
                    {
                        scratchpad[n] = 0;
                        if (edflib_is_onset_number(scratchpad.data()))
                        {
                            error = 36;
                            return(9);
                        }
                        onset = 1;
                        n = 0;
                        time_in_txt = scratchpad;
                        //strncpy(time_in_txt, scratchpad, max_tal_ln + 3);
                        continue;
                    }

                    if (duration_start)
                    {
                        scratchpad[n] = 0;
                        if (edflib_is_duration_number(scratchpad.data()))
                        {
                            error = 37;
                            return(9);
                        }

                        for (j = 0; j<n; j++)
                        {
                            if (j == 15)  break;
                            duration_in_txt[j] = scratchpad[j];
                            if ((duration_in_txt.at(j)<32) || (duration_in_txt.at(j)>126))
                            {
                                duration_in_txt[j] = '.';
                            }
                        }
                        duration_in_txt[j] = 0;

                        duration = 1;
                        duration_start = 0;
                        n = 0;
                        continue;
                    }
                }

                n++;
            }

            /****************** end ************************/

            if (error)
            {

                return(9);
            }
        }
    }

    return(0);
}
int EDFFile::edflib_is_duration_number(char *str)
{
    int i, l, hasdot = 0;

    l = strlen(str);

    if (!l)  return(1);

    if ((str[0] == '.') || (str[l - 1] == '.'))  return(1);

    for (i = 0; i<l; i++)
    {
        if (str[i] == '.')
        {
            if (hasdot)  return(1);
            hasdot++;
        }
        else
        {
            if ((str[i]<48) || (str[i]>57))  return(1);
        }
    }

    return(0);
}


int EDFFile::edflib_is_onset_number(char *str)
{
    int i, l, hasdot = 0;

    l = strlen(str);

    if (l<2)  return(1);

    if ((str[0] != '+') && (str[0] != '-'))  return(1);

    if ((str[1] == '.') || (str[l - 1] == '.'))  return(1);

    for (i = 1; i<l; i++)
    {
        if (str[i] == '.')
        {
            if (hasdot)  return(1);
            hasdot++;
        }
        else
        {
            if ((str[i]<48) || (str[i]>57))  return(1);
        }
    }

    return(0);
}


long long EDFFile::edflib_get_long_time(char *str) const
{
    int i, len, hasdot = 0, dotposition = 0;

    long long value = 0, radix;

    str = str + 1;

    len = strlen(str);

    for (i = 0; i<len; i++)
    {
        if (str[i] == '.')
        {
            hasdot = 1;
            dotposition = i;
            break;
        }
    }

    if (hasdot)
    {
        radix = EDFLIB_TIME_DIMENSION;

        for (i = dotposition - 1; i >= 0; i--)
        {
            value += ((long long)(str[i] - 48)) * radix;
            radix *= 10;
        }

        radix = EDFLIB_TIME_DIMENSION / 10;

        for (i = dotposition + 1; i<len; i++)
        {
            value += ((long long)(str[i] - 48)) * radix;
            radix /= 10;
        }
    }
    else
    {
        radix = EDFLIB_TIME_DIMENSION;

        for (i = len - 1; i >= 0; i--)
        {
            value += ((long long)(str[i] - 48)) * radix;
            radix *= 10;
        }
    }

    if (str[-1] == '-')  value = -value;

    return(value);
}

