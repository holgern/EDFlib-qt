#include <QCoreApplication>
#include <QString>
#include <QTextStream>

#include "EDFFile.h"

int main(int argc, char *argv[])
{
  int channel,
      n;

  double *buf;


  if(argc!=3)
  {
    printf("\nusage: test_edflib <file> <signal nr>\n\n");
    return(1);
  }

  channel = atoi(argv[2]);
  if(channel<1)
  {
    printf("\nsignalnumber must be > 0\n\n");
    return(1);
  }
  qDebug()<<"File name"<<QString(argv[1])<<endl;

  EDFFile* file = new EDFFile();
  if (!file->open(QString(argv[1]),EDFFile::readOnlyReadAllAnnotations)) {
      qDebug()<<file->lastError()<<endl;
      return 1;
  }


  if(channel>(file->getSignalCount()))
  {
    qDebug("\nerror: file has %i signals and you selected signal %i\n\n", file->getSignalCount(), channel);
    file->close();
    return(1);
  }

  channel--;

//  printf("\nlibrary version: %i.%02i\n", edflib_version() / 100, edflib_version() % 100);

  qDebug("\ngeneral header:\n\n");

  qDebug("filetype: %i\n", file->header()->getFiletype());
  qDebug("edfsignals: %i\n", file->getSignalCount());
#ifdef WIN32
//  printf("file duration: %I64d seconds\n", hdr.file_duration / EDFLIB_TIME_DIMENSION);
#else
//  printf("file duration: %lli seconds\n", hdr.file_duration / EDFLIB_TIME_DIMENSION);
  qDebug("file duration: %f seconds\n", file->getDataRecordDuration());
#endif
  qDebug("startdate: %i-%i-%i\n", file->header()->getStartDate().getDay(), file->header()->getStartDate().getMonth(), file->header()->getStartDate().getYear());
  qDebug("starttime: %i:%02i:%02i\n", file->header()->getStartTime().getHour(),file->header()->getStartTime().getMinute(), file->header()->getStartTime().getSecond());
  qDebug("patient: %s\n", file->header()->getPatient().getName().toLatin1().data());
  qDebug("recording: %s\n", file->header()->getMeasurementSetup().getRecording().toLatin1().data());
  qDebug("patientcode: %s\n", file->header()->getPatient().getCode().toLatin1().data());
  //qDebug("gender: %s\n", hdr.gender);
  qDebug("birthdate: %s\n", file->header()->getPatient().getBirthdate().toString().toLatin1().data());
  qDebug("patient_name: %s\n", file->header()->getPatient().getName().toLatin1().data());
  qDebug("patient_additional: %s\n", file->header()->getPatient().getAdditional().toLatin1().data());
  qDebug("admincode: %s\n", file->header()->getMeasurementSetup().getAdminCode().toLatin1().data());
  qDebug("technician: %s\n", file->header()->getMeasurementSetup().getTechnician().toLatin1().data());
  qDebug("equipment: %s\n", file->header()->getMeasurementSetup().getEquipment().toLatin1().data());
  qDebug("recording_additional: %s\n", file->header()->getMeasurementSetup().getRecordingAdditional().toLatin1().data());
//  qDebug("datarecord duration: %f seconds\n", ((double)hdr.datarecord_duration) / EDFLIB_TIME_DIMENSION);
//#ifdef WIN32
//  qDebug("number of datarecords in the file: %I64d\n", hdr.datarecords_in_file);
//  qDebug("number of annotations in the file: %I64d\n", hdr.annotations_in_file);
//#else
//  qDebug("number of datarecords in the file: %lli\n", hdr.datarecords_in_file);
//  qDebug("number of annotations in the file: %lli\n", hdr.annotations_in_file);
//#endif

  qDebug("\nsignal parameters:\n\n");

  qDebug("label: %s\n", file->signalHeader(channel)->getLabel().toLatin1().data());
//#ifdef WIN32
//  printf("samples in file: %I64d\n", hdr.signalparam[channel].smp_in_file);
//#else
//  printf("samples in file: %lli\n", hdr.signalparam[channel].smp_in_file);
//#endif
  qDebug("samples in datarecord: %i\n", file->signalHeader(channel)->getSignalSampleCount());
  qDebug("physical maximum: %f\n", file->signalHeader(channel)->getPhysicalMax());
  qDebug("physical minimum: %f\n", file->signalHeader(channel)->getPhysicalMin());
  qDebug("digital maximum: %i\n", file->signalHeader(channel)->getDigitalMax());
  qDebug("digital minimum: %i\n", file->signalHeader(channel)->getDigitalMin());
  qDebug("physical dimension: %s\n", file->signalHeader(channel)->getPhysicalDimension().toLatin1().data());
  qDebug("prefilter: %s\n", file->signalHeader(channel)->getPrefilter().toLatin1().data());
  qDebug("transducer: %s\n", file->signalHeader(channel)->getTransducer().toLatin1().data());
 // qDebug("samplefrequency: %f\n", ((double)hdr.signalparam[channel].smp_in_datarecord / (double)hdr.datarecord_duration) * EDFLIB_TIME_DIMENSION);

//  struct edf_annotation_struct annot;

//  printf("\n");

//  for(i=0; i<hdr.annotations_in_file; i++)
//  {
//    if(edf_get_annotation(hdl, i, &annot))
//    {
//      printf("\nerror: edf_get_annotations()\n");
//      edfclose_file(hdl);
//      return(1);
//    }
//    else
//    {
//#ifdef WIN32
//      printf("annotation: onset is %I64d    duration is %s    description is %s\n",
//            annot.onset / EDFLIB_TIME_DIMENSION,
//            annot.duration,
//            annot.annotation);
//#else
//      printf("annotation: onset is %lli    duration is %s    description is %s\n",
//            annot.onset / EDFLIB_TIME_DIMENSION,
//            annot.duration,
//            annot.annotation);
//#endif
//    }
//  }

  n = 200; /* read n samples from the file */

  buf = (double *)malloc(n);

  if(buf==NULL)
  {
    printf("\nmalloc error\n");
    file->close();
    return(1);
  }

  int x=0; /* start reading x seconds from start of file */

//  edfseek(hdl, channel, (long long)((((double)x) / ((double)hdr.file_duration / (double)EDFLIB_TIME_DIMENSION)) * ((double)hdr.signalparam[channel].smp_in_file)), EDFSEEK_SET);

//  n = edfread_physical_samples(hdl, channel, n, buf);
    //file->readPhysicalData(channel,n);


//  if(n==(-1))
//  {
//    printf("\nerror: edf_read_physical_samples()\n");
//    edfclose_file(hdl);
//    free(buf);
//    return(1);
//  }

  printf("\nread %i samples, started at %i seconds from start of file:\n\n", n, x);

  for(int i=0; i<n; i++)
  {
      file->readPhysicalData(channel,1);
    printf("%.3f  ", file->signalData(channel)->at(file->signalData(channel)->size()-1));
  }

  printf("\n\n");

//  edfclose_file(hdl);

//  free(buf);
  file->close();
  delete file;
  free(buf);
  return(0);
}





























