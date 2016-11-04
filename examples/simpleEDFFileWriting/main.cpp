
/*
  this program generates an EDFplus or BDFplus testfile with the following signals:

 signal label/waveform  amplitude    f       sf
 ---------------------------------------------------
    1    squarewave        100 uV    0.1Hz   200 Hz
    2    ramp              100 uV    1 Hz    200 Hz
    3    pulse 1           100 uV    1 Hz    200 Hz
    4    pulse 2           100 uV    1 Hz    256 Hz
    5    pulse 3           100 uV    1 Hz    217 Hz
    6    noise             100 uV    - Hz    200 Hz
    7    sine 1 Hz         100 uV    1 Hz    200 Hz
    8    sine 8 Hz         100 uV    8 Hz    200 Hz
    9    sine 8.1777 Hz    100 uV    8.25 Hz 200 Hz
   10    sine 8.5 Hz       100 uV    8.5Hz   200 Hz
   11    sine 15 Hz        100 uV   15 Hz    200 Hz
   12    sine 17 Hz        100 uV   17 Hz    200 Hz
   13    sine 50 Hz        100 uV   50 Hz    200 Hz
   14    DC event 8-bits code 1 V   100 mS/bit  200 Hz

*/



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <QCoreApplication>
#include <QString>
#include <QTextStream>

#include "EDFFile.h"



#define SMP_FREQ 200

#define SMP_FREQ_2 256

#define SMP_FREQ_3 217

#define FILE_DURATION 600


/* if you want to create an EDFplus file instead of BDFplus, outcomment the next line: */


#define BDF_FORMAT

int main(int argc, char *argv[])
{
    int i, j,
        chns;

    double buf[1000],
           q,
           sine_1,
           sine_8,
           sine_81777,
           sine_85,
           sine_15,
           sine_17,
           sine_50;

    struct{
            long long samples;
            long long triggers[512];
            int index;
            int code;
            int bitposition;
            int smp_in_bit;
          } dc_event_stat;

    QCoreApplication app(argc, argv);




  memset(&dc_event_stat, 0, sizeof(dc_event_stat));

  dc_event_stat.code = 0;

  dc_event_stat.triggers[0] = 1951;

  for(i=1; i<512; i++)
  {
    dc_event_stat.triggers[i] = (i * 1667) + 1951;
  }

  chns = 14;

  EDFFile* file = new EDFFile();


//#ifdef BDF_FORMAT
//  hdl = edfopen_file_writeonly("test_generator.bdf", EDFLIB_FILETYPE_BDFPLUS, chns);
//#else
//  hdl = edfopen_file_writeonly("test_generator.edf", EDFLIB_FILETYPE_EDFPLUS, chns);
//#endif

//  if(hdl<0)
//  {
//    printf("error: edfopen_file_writeonly()\n");

//    return(1);
//  }
  if (!file->open("test_generator.bdf",EDFFile::writeOnly))
  {
      qDebug()<<file->lastError()<<endl;
      return -1;
  }

  EDFFileHeader* header = file->header();
#ifdef BDF_FORMAT
  header->setFiletype(EDFFileHeader::BDFPlus);
#else
  header->setFiletype(EDFFileHeader::EDFPlus);
#endif

  file->setSignalCount(chns);

  for (i = 0; i<chns; i++)
  {
	  if (!(file->validSignal(i) && file->signalHeader(i)->setSignalSampleFrequency(SMP_FREQ)))
	  {
		  qDebug("error: set Samplefrequency");
		  delete file;
		  return -1;

	  }
  }


  if (!(file->validSignal(3) && file->signalHeader(3)->setSignalSampleFrequency(SMP_FREQ_2)))
  {
      qDebug("error: set Samplefrequency");
      delete file;
      return -1;

  }

  if (!(file->validSignal(4) && file->signalHeader(4)->setSignalSampleFrequency(SMP_FREQ_3)))
  {
      qDebug("error: set Samplefrequency");
      delete file;
      return -1;

  }

   if (!(file->validSignal(13) && file->signalHeader(13)->setSignalSampleFrequency(1000)))
   {
       qDebug("error: set Samplefrequency");
       delete file;
       return -1;

   }


#ifdef BDF_FORMAT
  for(i=0; i<chns; i++)
  {
    if (!(file->validSignal(i) && file->signalHeader(i)->setDigitalMax(8388607)))
    //if(edf_set_digital_maximum(hdl, i, 8388607))
    {
      qDebug("error: edf_set_digital_maximum()\n");
      delete file;
      return(1);
    }
  }

  for(i=0; i<chns; i++)
  {
    if(!(file->validSignal(i) && file->signalHeader(i)->setDigitalMin(-8388608)))
    {
      qDebug("error: edf_set_digital_minimum()\n");
      return(1);
    }
  }

  if(!(file->validSignal(13) && file->signalHeader(13)->setDigitalMin(8300000)))
  {
    qDebug("error: edf_set_digital_minimum()\n");
    return(1);
  }

#else
  for(i=0; i<chns; i++)
  {
    if(!(file->validSignal(i) && file->getSignals().at(i)->setDigitalMax(32767)))
    {
      qDebug("error: edf_set_digital_maximum()\n");

      return(1);
    }
  }

  for(i=0; i<chns; i++)
  {
    if(!(file->validSignal(i) && file->getSignals().at(i)->setDigitalMin(-32768)))
    {
      qDebug("error: edf_set_digital_minimum()\n");

      return(1);
    }
  }
#endif

  for(i=0; i<chns; i++)
  {
    if(!(file->validSignal(i) && file->signalHeader(i)->setPhysicalMax(1000.0)))
    {
      qDebug("error: edf_set_physical_maximum()\n");

      return(1);
    }
  }

  if(!(file->validSignal(8) && file->signalHeader(8)->setPhysicalMax(262143.0)))
  {
    qDebug("error: edf_set_physical_maximum()\n");

    return(1);
  }

  if(!(file->validSignal(13) && file->signalHeader(13)->setPhysicalMax(10.0)))
  {
    qDebug("error: edf_set_physical_maximum()\n");

    return(1);
  }

  for(i=0; i<chns; i++)
  {
    if(!(file->validSignal(i) && file->signalHeader(i)->setPhysicalMin(-1000.0)))
    {
      qDebug("error: edf_set_physical_minimum()\n");

      return(1);
    }
  }

  if(!(file->validSignal(8) && file->signalHeader(8)->setPhysicalMin( -262144.0)))
  {
    qDebug("error: edf_set_physical_minimum()\n");

    return(1);
  }

  if(!(file->validSignal(13) && file->signalHeader(13)->setPhysicalMin(-10.0)))
  {
    qDebug("error: edf_set_physical_minimum()\n");

    return(1);
  }

  for(i=0; i<chns; i++)
  {
    if(!(file->validSignal(i) && file->signalHeader(i)->setPhysicalDimension("uV")))
    {
      qDebug("error: edf_set_physical_dimension()\n");

      return(1);
    }
  }

  if(!(file->validSignal(13) && file->signalHeader(13)->setPhysicalDimension("V")))
  {
    qDebug("error: edf_set_physical_dimension()\n");

    return(1);
  }

  i = 0;

  if(!(file->validSignal(i) && file->signalHeader(i)->setLabel("squarewave")))
  {
    qDebug("error: edf_set_label()\n");

    return(1);
  }
  i++;

  if(!(file->validSignal(i) && file->signalHeader(i)->setLabel("ramp")))
  {
    qDebug("error: edf_set_label()\n");

    return(1);
  }
  i++;

  if(!(file->validSignal(i) && file->signalHeader(i)->setLabel( "pulse 1")))
  {
    qDebug("error: edf_set_label()\n");

    return(1);
  }
  i++;

  if(!(file->validSignal(i) && file->signalHeader(i)->setLabel("pulse 2")))
  {
    qDebug("error: edf_set_label()\n");

    return(1);
  }
  i++;

  if(!(file->validSignal(i) && file->signalHeader(i)->setLabel("pulse 3")))
  {
    qDebug("error: edf_set_label()\n");

    return(1);
  }
  i++;

  if(!(file->validSignal(i) && file->signalHeader(i)->setLabel( "noise")))
  {
    qDebug("error: edf_set_label()\n");

    return(1);
  }
  i++;

  if(!(file->validSignal(i) && file->signalHeader(i)->setLabel("sine 1 Hz")))
  {
    qDebug("error: edf_set_label()\n");

    return(1);
  }
  i++;

  if(!(file->validSignal(i) && file->signalHeader(i)->setLabel("sine 8 Hz")))
  {
    qDebug("error: edf_set_label()\n");

    return(1);
  }
  i++;

  if(!(file->validSignal(i) && file->signalHeader(i)->setLabel("sine 8.1777 Hz")))
  {
    qDebug("error: edf_set_label()\n");

    return(1);
  }
  i++;

  if(!(file->validSignal(i) && file->signalHeader(i)->setLabel("sine 8.5 Hz")))
  {
    qDebug("error: edf_set_label()\n");

    return(1);
  }
  i++;

  if(!(file->validSignal(i) && file->signalHeader(i)->setLabel("sine 15 Hz")))
  {
    qDebug("error: edf_set_label()\n");

    return(1);
  }
  i++;

  if(!(file->validSignal(i) && file->signalHeader(i)->setLabel( "sine 17 Hz")))
  {
    qDebug("error: edf_set_label()\n");

    return(1);
  }
  i++;

  if(!(file->validSignal(i) && file->signalHeader(i)->setLabel( "sine 50 Hz")))
  {
    qDebug("error: edf_set_label()\n");

    return(1);
  }
  i++;

  if(!(file->validSignal(i) && file->signalHeader(i)->setLabel("DC 01")))
  {
    qDebug("error: edf_set_label()\n");

    return(1);
  }
  i++;
  header->getMeasurementSetup().setEquipment("test generator");

//  if(edf_set_equipment(hdl, "test generator"))
//  {
//    printf("edf_set_equipment()\n");

//    return(1);
//  }

//  edf_set_birthdate(hdl, 1969, 6, 30);
  EDFPatient pat;
  pat.setBirthdate(EDFDate(30,6,1969));
  header->setPatient(pat);

  if(!file->writeHeader()){
      qDebug("error: could not write header\n");

      return(1);
  }

  qDebug("start writing data");

  sine_1 = 0.0;
  sine_8 = 0.0;
  sine_81777 = 0.0;
  sine_85 = 0.0;
  sine_15 = 0.0;
  sine_17 = 0.0;
  sine_50 = 0.0;

  for(j=0; j<FILE_DURATION; j++)
  {
      qDebug("j: %d",j);
    if((j%10)<5)                    /* square */
    {
      for(i=0; i<SMP_FREQ; i++)
      {
        buf[i] = 100.0;
      }
    }
    else
    {
      for(i=0; i<SMP_FREQ; i++)
      {
        buf[i] = -100.0;
      }
    }


    if(!file->writePhysicalData(buf,SMP_FREQ))
    {
      qDebug("error: edfwrite_physical_samples()\n");

      return(1);
    }

    for(i=0; i<SMP_FREQ; i++)                       /* ramp */
    {
      buf[i] = -100.0 + (i * (200.0 / SMP_FREQ));
    }

    if(!file->writePhysicalData(buf,SMP_FREQ))
    {
      qDebug("error: edfwrite_physical_samples()\n");

      return(1);
    }

    for(i=0; i<SMP_FREQ; i++)                 /* pulse 1 */
    {
      buf[i] = 0.0;
    }

    buf[0] = 100.0;

    buf[SMP_FREQ - 2] = 100.0;

    if(!file->writePhysicalData(buf,SMP_FREQ))
    {
      qDebug("error: edfwrite_physical_samples()\n");

      return(1);
    }

    for(i=0; i<SMP_FREQ_2; i++)               /* pulse 2 */
    {
      buf[i] = 0.0;
    }

    buf[0] = 100.0;

    buf[SMP_FREQ_2 - 2] = 100.0;

    if(!file->writePhysicalData(buf,SMP_FREQ_2))
    {
      qDebug("error: edfwrite_physical_samples()\n");

      return(1);
    }

    for(i=0; i<SMP_FREQ_3; i++)              /* pulse 3 */
    {
      buf[i] = 0.0;
    }

    buf[0] = 100.0;

    buf[SMP_FREQ_3 - 2] = 100.0;

    if(!file->writePhysicalData(buf,SMP_FREQ_3))
    {
      qDebug("error: edfwrite_physical_samples()\n");

      return(1);
    }

    for(i=0; i<SMP_FREQ; i++)                /* noise */
    {
      buf[i] = (int)(100.0 * (rand() / (RAND_MAX + 1.0)));
    }

    if(!file->writePhysicalData(buf,SMP_FREQ))
    {
      qDebug("error: edfwrite_physical_samples()\n");

      return(1);
    }

    for(i=0; i<SMP_FREQ; i++)                /* sine 1 Hz */
    {
      q = M_PI * 2.0;
      q /= SMP_FREQ;
      sine_1 += q;
      q = sin(sine_1);
      q *= 100.0;
      buf[i] = q;
    }

    if(!file->writePhysicalData(buf,SMP_FREQ))
    {
      qDebug("error: edfwrite_physical_samples()\n");

      return(1);
    }

    for(i=0; i<SMP_FREQ; i++)                /* sine 8 Hz */
    {
      q = M_PI * 2.0;
      q /= (SMP_FREQ / 8.0);
      sine_8 += q;
      q = sin(sine_8);
      q *= 100.0;
      buf[i] = q + 800.0;         /* add dc-offset */
    }

    if(!file->writePhysicalData(buf,SMP_FREQ))
    {
      qDebug("error: edfwrite_physical_samples()\n");

      return(1);
    }

    for(i=0; i<SMP_FREQ; i++)                /* sine 8.1777 Hz */
    {
      q = M_PI * 2.0;
      q /= (SMP_FREQ / 8.1777);
      sine_81777 += q;
      q = sin(sine_81777);
      q *= 100.0;
      buf[i] = q + 6000.0;       /* add dc-offset */
    }

    if(!file->writePhysicalData(buf,SMP_FREQ))
    {
      qDebug("error: edfwrite_physical_samples()\n");

      return(1);
    }

    for(i=0; i<SMP_FREQ; i++)                /* sine 8.5 Hz */
    {
      q = M_PI * 2.0;
      q /= (SMP_FREQ / 8.5);
      sine_85 += q;
      q = sin(sine_85);
      q *= 100.0;
      buf[i] = q;
    }

    if(!file->writePhysicalData(buf,SMP_FREQ))
    {
      qDebug("error: edfwrite_physical_samples()\n");

      return(1);
    }

    for(i=0; i<SMP_FREQ; i++)                /* sine 15 Hz */
    {
      q = M_PI * 2.0;
      q /= (SMP_FREQ / 15.0);
      sine_15 += q;
      q = sin(sine_15);
      q *= 100.0;
      buf[i] = q;
    }

    if(!file->writePhysicalData(buf,SMP_FREQ))
    {
      qDebug("error: edfwrite_physical_samples()\n");

      return(1);
    }

    for(i=0; i<SMP_FREQ; i++)                /* sine 17 Hz */
    {
      q = M_PI * 2.0;
      q /= (SMP_FREQ / 17.0);
      sine_17 += q;
      q = sin(sine_17);
      q *= 100.0;
      buf[i] = q;
    }

    if(!file->writePhysicalData(buf,SMP_FREQ))
    {
      qDebug("error: edfwrite_physical_samples()\n");

      return(1);
    }

    for(i=0; i<SMP_FREQ; i++)                /* sine 50 Hz */
    {
      q = M_PI * 2.0;
      q /= (SMP_FREQ / 50.0);
      sine_50 += q;
      q = sin(sine_50);
      q *= 100.0;
      buf[i] = q;
    }

    if(!file->writePhysicalData(buf,SMP_FREQ))
    {
      qDebug("error: edfwrite_physical_samples()\n");

      return(1);
    }

    for(i=0; i<1000; i++)                /* DC 01 */
    {
      if(dc_event_stat.bitposition)
      {
        if(dc_event_stat.bitposition == 1)
        {
          buf[i] = 1.0;
        }
        else
        {
          if(dc_event_stat.code & (1 << (dc_event_stat.bitposition - 2)))
          {
            buf[i] = 1.0;
          }
          else
          {
            buf[i] = 0.0;
          }
        }

        if(++dc_event_stat.smp_in_bit >= 10)
        {
          dc_event_stat.smp_in_bit = 0;

          dc_event_stat.bitposition++;
        }

        if(dc_event_stat.bitposition > 10)
        {
          dc_event_stat.bitposition = 0;

          dc_event_stat.smp_in_bit = 0;

          dc_event_stat.code++;

          dc_event_stat.code &= 255;

          if(++dc_event_stat.index >= 512)
          {
            dc_event_stat.index = 0;

            dc_event_stat.code = 0;
          }
        }
      }
      else
      {
        if(dc_event_stat.samples == dc_event_stat.triggers[dc_event_stat.index])
        {
/*          edfwrite_annotation_latin1(hdl, dc_event_stat.samples * 10LL, -1LL, "Trigger");  */

          dc_event_stat.bitposition = 1;
          dc_event_stat.smp_in_bit = 1;
          buf[i] = 1.0;
        }
        else
        {
          buf[i] = 0.0;
        }
      }

      dc_event_stat.samples++;
    }

    if(!file->writePhysicalData(buf,1000))
    {
      qDebug("error: edfwrite_physical_samples()\n");

      return(1);
    }
  }

  file->addAnnotation(EDFAnnotation(0LL,-1LL,"Recording starts"));
//  edfwrite_annotation_latin1(hdl, 0LL, -1LL, "Recording starts");

  file->addAnnotation(EDFAnnotation(2980000LL,-1LL,"Test 1"));
//  edfwrite_annotation_latin1(hdl, 2980000LL, -1LL, "Test 1");

file->addAnnotation(EDFAnnotation(2940000LL + (long long)((10000.0 / SMP_FREQ) * (SMP_FREQ - 2)), -1LL, "pulse 1"));

file->addAnnotation(EDFAnnotation(2950000LL + (long long)((10000.0 / SMP_FREQ_2) * (SMP_FREQ_2 - 2)), -1LL, "pulse 2"));

file->addAnnotation(EDFAnnotation(2960000LL + (long long)((10000.0 / SMP_FREQ_3) * (SMP_FREQ_3 - 2)), -1LL, "pulse 3"));

file->addAnnotation(EDFAnnotation(FILE_DURATION * 10000LL, -1LL, "Recording ends"));

//  edfclose_file(hdl);


  if(!file->close())
  {
    qDebug("error: close()\n");

    return(1);
  }
  delete file;
  qDebug("Ok!");
  return app.exec();
}





















