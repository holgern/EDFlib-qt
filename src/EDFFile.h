#ifndef EDFFILE_H
#define EDFFILE_H
/*
 * Copyright (c) Holger Nahrstaedt
 */

#include <QString>
#include <QtCore>
#include "EDFFileHeader.h"
#include "EDFAnnotation.h"
#include "EDFError.h"
#include "EDFSignalHeader.h"
#include "EDFSignalData.h"
#include <memory>

/*!
 * \brief The EDFFile class
 * Contains a edfHeader, QList<EDFSignalHeader*> and QList<EDFSignalData*>
 * They can be  either extracted from a file or written to a file
 */
class EDFFile : public QObject
{
    Q_OBJECT
public:
    EDFFile();
    virtual ~EDFFile();
    enum FileStatus
    {
        NotDefined = 0,
        readOnlyWithOutAnnotations,
        readOnlyReadAnnotations,
        readOnlyReadAllAnnotations,
        writeOnly
    };
    enum Whence
    {
        SeekSet = 0,
        SeekCur,
        SeekEnd
    };
    EDFFileHeader* header() const { return edfHeader; }
    bool writeHeader();
    /*!
     * \brief open file either for reading or writing
     * \param QString name - filename
     * \param FileStatus st - can be readOnlyWithOutAnnotations,readOnlyReadAnnotations,readOnlyReadAllAnnotations or
     * writeOnly
     * \return
     * true if the file could be successfully opened
     */
    bool open(QString name, FileStatus st);



    bool isOpen();
    /*!
     * \brief close file
     * \return
     */
    bool close();
    /*!
     * \brief lastError
     * \return EDFError
     *
     */
    EDFError lastError() const { return error; }

    void clear();

    /*!
     * \brief getSignalCount
     * \return Number of Channels available in the EDF/BDF file or Number of Channels which are written into an EDF/BDF file
     */
    int getSignalCount() const { return signalCount; }
    /*!
     * \brief getDataRecordDuration
     * \return number of data records (-1 if unknown)
     *  Sets the datarecord duration. The default value is 1 second.
     * This function is optional, normally you don't need to change the default value.
     * The datarecord duration must be in the range 0.025 to 20.0 seconds.
     *
     * The getDataRecordDuration is the package length.
     */
    double getDataRecordDuration() const { return dataRecordDuration; }
    /*!
     * \brief getDataRecords
     * \return duration of a data record, in seconds
     */
    long long getDataRecords() const { return dataRecords; }
    /*!
     * \brief setSignalCount
     * \return Number of Channels available in the EDF/BDF file or Number of Channels which are written into an EDF/BDF file
     */
    void setSignalCount(int, bool = true);
    /*!
     * \brief validSignal
     * \param channel nr
     * \return Checks weather a channel nr is lower than  SignalCount
     */
    bool validSignal(int) const;

    /*!
     * \brief signalHeader
     * \param channel
     * \return
     */
    EDFSignalHeader* signalHeader(int channel) { return signalsHeader[channel]; }
    /*!
    * \brief signalHeader
    * \param channel
    * \return
    */
    EDFSignalHeader* annotationHeader(int annotchannel);
    /*!
     * \brief signalData
     * \param channel
     * \param pos
     * \return
     */
    double signalData(int channel, int pos);
    /*!
     * \brief signalData
     * \param channel
     * \return
     */
    EDFSignalData* signalData(int channel);

    void clearSignalData(int channel);

    /*!
     * \brief writePhysicalData
     * \param buf
     * \param size
     *
  Writes "size" physical samples (uV, mA, Ohm) from *buf belonging to one signal
  where "size" is the samplefrequency of the signal.
  The physical samples will be converted to digital samples using the
  values of physical maximum, physical minimum, digital maximum and digital minimum.
  The number of samples written is equal to the samplefrequency of the signal.
  Size of buf should be equal to or bigger than sizeof(double[samplefrequency]).
  Call this function for every signal in the file. The order is important!
  When there are 4 signals in the file, the order of calling this function
  must be: signal 0, signal 1, signal 2, signal 3, signal 0, signal 1, signal 2, etc.
     */
    bool writePhysicalData(double* buf, int size);

    QList<EDFAnnotation> getAnnotations(){ return annotations; }

    void addAnnotation(EDFAnnotation a) { annotations.append(a); }
    /*!
     * \brief readPhysicalData
     * \param channel
     * \param n
     * \return
     *   Reads n samples from edfsignal, starting from the current sample position indicator, into EDFSignalData.
  The values are converted to their physical values e.g. microVolts, beats per minute, etc.
  Returns the amount of samples read (this can be less than n or zero) or -1 in case of an error
  The sample position indicator will be increased with the amount of samples read.


     */
    int readPhysicalData(int channel, int n);
    /*!
     * \brief seek
     * \param edfsignal
     * \param offset
     * \param whence
     * \return
  The seek() function sets the sample position indicator for the edfsignal pointed to by edfsignal.
  The new position, measured in samples, is obtained by adding offset samples to the position specified by whence.
  If whence is set to SeekSet, SeekCur, or SeekEnd, the offset is relative to the start of the file,
  the current position indicator, or end-of-file, respectively.
  Returns the current offset. Otherwise, -1 is returned.
  Note that every signal has it's own independent sample position indicator and seek() affects only one of them.
     */
    long long seek(int edfsignal, long long offset, Whence whence);
    /*!
     * \brief tell
     * \param edfsignal
     * \return
  The tell(int edfsignal) function obtains the current value of the sample position indicator for the edfsignal pointed
  to by edfsignal.
  Returns the current offset. Otherwise, -1 is returned
  Note that every signal has it's own independent sample position indicator and tell() affects only one of them.
     */


    /**
     * @brief getInterval returns a vector of measurement values from the
     * interval [startTime, endTime] with startTime and endTime in seconds and
     * 0 <= startTime < endTime <= signalHeader(edfsignal)->getSignalSampleCount() * getDataRecords
     * @return
     */
    QVector<double> getInterval(int edfsignal, uint startTime, uint endTime);

    long long tell(int edfsignal);
    /*!
     * \brief rewind
     * \param edfsignal
     *
  The rewind() function sets the sample position indicator for the edfsignal pointed to by edfsignal to the beginning of
  the file.
  It is equivalent to:  seek(int edfsignal, 0LL, SeekSet).
  Note that every signal has it's own independent sample position indicator and edfrewind() affects only one of them.
     */
    void rewind(int edfsignal);



    QString name() const
    {
        if( !file )
        {
            qCritical() << "EDFFile::fileName(): file not yet initialized";
            return QString();
        }
        return file->fileName();
    }
protected:
    /*!
    * \brief repair file either for reading
    * \param QString name - filename
    * \return
    * true if the file could be successfully repaired
    */
    bool repairDataRecords();
    QList<EDFSignalHeader*> signalHeader() const;
    QList<EDFSignalData*> signalData() const;

    bool openWriteOnly();
    bool openReadOnly();

    bool parseHeader(const QByteArray& edf_hdr);
    bool parseSignalHeader(const QByteArray& edf_hdr);
    QByteArray createHeader();
    QByteArray createSignalHeader();
    int getHeaderSize() const;
    /*!
     * \brief signalHeader
     * \return
     * Allows access to the signalHeader
     * The return variable is a QList of EDFSignalHeader
     */

private:
    void init();
    int edflib_fprint_ll_number_nonlocalized(long long q, int minimum, int sign) const;
    static int edflib_sprint_ll_number_nonlocalized(char* str, long long q, int minimum, int sign);
    static int edflib_is_duration_number(char *str);
    static int edflib_is_onset_number(char *str);
    long long edflib_get_long_time(char *str) const;
    bool writeAllAnnotations();
    int edflib_get_annotations();
    EDFFileHeader* edfHeader;
    FileStatus status;
    EDFError error;
    QFile* file;
    //QFile* file;
    QList<EDFSignalHeader*> signalsHeader;
    QList<EDFSignalData*> signalsData;
    QList<EDFAnnotation> annotations;
    int signalCount;
    int annotationChannelCount;
    double dataRecordDuration;
    long long longDataRecordDuration;
    int recordSize;
    int signalWriteSequencePos;
    long long dataRecords;
    QList<int> annotationChannel;
    long long annotationsOffset;
    long long annotationsDatarecords;
    long long starttime_offset;
    int annotationsDatrecsize;
    int annotationsWrittenToFile;
    const long long EDFLIB_TIME_DIMENSION = 10000000LL;
    const int EDFLIB_MAX_ANNOTATION_LEN = 512;
#ifdef UNIT_TEST
    friend class EDFFileTest;
#endif
};

#endif  // EDFFILE_H
