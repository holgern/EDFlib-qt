#include "EDFFile.h"
// Qt includes
#include <QString>
#include <QObject>
#include "qmath.h"
#include "BoostTest.h"

BOOST_AUTO_TEST_SUITE( EDFFileTestSuite )

BOOST_AUTO_TEST_CASE(initTestSignalCreation)
//void EDFFileTest::initTestSignalCreation()
{
    EDFFile *edffile;
    double TA,TA2;
    QVector<double> testSignal;
    QVector<double> testSignal2;
    TA=1./1000.;
    testSignal.resize(10000);
    for (int i=0;i<testSignal.size();i++){
        testSignal[i]= sin(2.* M_PI * 10. * (double)i*TA);
    }

    TA2=1./500.;
    testSignal2.resize(5000);
    for (int i=0;i<testSignal2.size();i++){
        testSignal2[i]= cos(2.* M_PI * 10. * (double)i*TA2);
    }

    edffile = new EDFFile();
    edffile->header()->setFiletype(EDFFileHeader::BDFPlus);
    BOOST_REQUIRE(edffile->getSignalCount()==0);
    //BOOST_REQUIRE(edffile->getHeaderSize()==256);
    BOOST_REQUIRE(edffile->getDataRecords()==0LL);
    edffile->setSignalCount(2,true);
    BOOST_REQUIRE(edffile->getSignalCount()==3);

    BOOST_REQUIRE_EQUAL(edffile->signalHeader(0)->getSignalSampleCount(),1);
    BOOST_REQUIRE_EQUAL(edffile->signalHeader(1)->getSignalSampleCount(),1);
    BOOST_REQUIRE_EQUAL(edffile->signalHeader(2)->getSignalSampleCount(),38);
    edffile->signalHeader(0)->setSignalSampleFrequency(1000);
    edffile->signalHeader(1)->setSignalSampleFrequency(500);
    for (int i=0;i<testSignal.size();i++)
        edffile->signalData(0)->addPhysicalData(testSignal.at(i));
    for (int i=0;i<testSignal2.size();i++)
        edffile->signalData(1)->addPhysicalData(testSignal2.at(i));

    delete edffile;

}
BOOST_AUTO_TEST_CASE(testFileCreation)
//void EDFFileTest::testFileCreation()
{
    EDFFile *edffile;
    double TA,TA2;
    QVector<double> testSignal;
    QVector<double> testSignal2;
    TA=1./1000.;
    testSignal.resize(10000);
    for (int i=0;i<testSignal.size();i++){
        testSignal[i]= sin(2.* M_PI * 10. * (double)i*TA);
    }

    TA2=1./500.;
    testSignal2.resize(5000);
    for (int i=0;i<testSignal2.size();i++){
        testSignal2[i]= cos(2.* M_PI * 10. * (double)i*TA2);
    }


   edffile = new EDFFile();
   edffile->header()->setFiletype(EDFFileHeader::BDFPlus);

   BOOST_REQUIRE(edffile->open("test.bdf",EDFFile::writeOnly));
   EDFPatient patient;
   patient.setCode("1");
   patient.setAdditional("add");
   patient.setBirthdate(EDFDate(10,2,1990));
   patient.setGender(EDFPatient::Female);
   patient.setName("name");
   EDFMeasurementSetup setup;
   setup.setAdminCode("admincode");
   setup.setEquipment("equipment");
   setup.setRecordingAdditional("recording");
   setup.setTechnician("technician");

   edffile->header()->setPatient(patient);
   edffile->header()->setMeasurementSetup(setup);
   edffile->setSignalCount(1);
   edffile->signalHeader(0)->setSignalSampleFrequency(1000);
   edffile->signalHeader(0)->setPhysicalMin(-1.);
   edffile->signalHeader(0)->setPhysicalMax(1.);
   edffile->signalHeader(0)->setDigitalMin(-8388608);
   edffile->signalHeader(0)->setDigitalMax(8388607);
   BOOST_REQUIRE(edffile->writeHeader());
   setup = edffile->header()->getMeasurementSetup();
   for (int i=0;i<10;i++)
        edffile->writePhysicalData(testSignal.mid(i*1000,1000).data(),1000);
   BOOST_REQUIRE(edffile->close());
   BOOST_REQUIRE_EQUAL(edffile->getDataRecordDuration(),1.);
   BOOST_REQUIRE_EQUAL(edffile->getDataRecords(),10LL);
   BOOST_REQUIRE_EQUAL(edffile->signalHeader(0)->getSignalSampleCount(),1000);

   delete edffile;

   EDFPatient patient2;
   EDFMeasurementSetup setup2;
   edffile = new EDFFile();
   BOOST_REQUIRE(edffile->open("test.bdf",EDFFile::readOnlyWithOutAnnotations));
   patient2 = edffile->header()->getPatient();
   setup2 = edffile->header()->getMeasurementSetup();
   BOOST_REQUIRE(patient2.getCode()==patient.getCode());
   BOOST_REQUIRE(patient2.getName()==patient.getName());
   BOOST_REQUIRE_EQUAL(patient2.getBirthdate(),patient.getBirthdate());
   BOOST_REQUIRE_EQUAL(patient2.getGender(),patient.getGender());
   BOOST_REQUIRE(setup2.getAdminCode()==setup.getAdminCode());
   BOOST_REQUIRE(setup2.getDate().toString()==setup.getDate().toString());
   BOOST_REQUIRE(setup2.getRecordingAdditional()==setup.getRecordingAdditional());
   BOOST_REQUIRE(setup2.getTechnician()==setup.getTechnician());
   BOOST_REQUIRE_EQUAL(edffile->getSignalCount(),2);
   BOOST_REQUIRE_EQUAL(edffile->getDataRecordDuration(),1.);
   BOOST_REQUIRE_EQUAL(edffile->getDataRecords(),10LL);
   BOOST_REQUIRE_EQUAL(edffile->signalHeader(0)->getSignalSampleCount(),1000);
   edffile->readPhysicalData(0,10000);
   qDebug("max error %g",edffile->signalHeader(0)->getResolution());
   for (int i=0;i<10000;i++){
       BOOST_REQUIRE((edffile->signalData(0,i)-testSignal.at(i))<edffile->signalHeader(0)->getResolution());
//       if ((buf[i]-testSignal.at(i))>1e-7){
//           qDebug("i %d, ist-soll %g",i,buf[i]-testSignal.at(i));
//       }
   }

   edffile->close();
   delete edffile;

}

BOOST_AUTO_TEST_CASE(testFileCreation2)
//void EDFFileTest::testFileCreation2()
{
    EDFFile *edffile;
    double TA,TA2;
    QVector<double> testSignal;
    QVector<double> testSignal2;
    TA=1./1000.;
    testSignal.resize(10000);
    for (int i=0;i<testSignal.size();i++){
        testSignal[i]= sin(2.* M_PI * 10. * (double)i*TA);
    }

    TA2=1./500.;
    testSignal2.resize(5000);
    for (int i=0;i<testSignal2.size();i++){
        testSignal2[i]= cos(2.* M_PI * 10. * (double)i*TA2);
    }

   edffile = new EDFFile();
   edffile->header()->setFiletype(EDFFileHeader::BDFPlus);

   BOOST_REQUIRE(edffile->open("test2.bdf",EDFFile::writeOnly));
   EDFPatient patient;
   patient.setCode("1");
   patient.setAdditional("add");
   patient.setBirthdate(EDFDate(10,2,1990));
   patient.setGender(EDFPatient::Female);
   patient.setName("name");
   EDFMeasurementSetup setup;
   setup.setAdminCode("admincode");
   setup.setEquipment("equipment");
   setup.setRecordingAdditional("recording");
   setup.setTechnician("technician");

   edffile->header()->setPatient(patient);
   edffile->header()->setMeasurementSetup(setup);
   edffile->setSignalCount(2);
   edffile->signalHeader(0)->setSignalSampleFrequency(1000);
   edffile->signalHeader(0)->setPhysicalMin(-1.);
   edffile->signalHeader(0)->setPhysicalMax(1.);
   edffile->signalHeader(0)->setDigitalMin(-8388608);
   edffile->signalHeader(0)->setDigitalMax(8388607);

   edffile->signalHeader(1)->setSignalSampleFrequency(500);
   edffile->signalHeader(1)->setPhysicalMin(-1.);
   edffile->signalHeader(1)->setPhysicalMax(1.);
   edffile->signalHeader(1)->setDigitalMin(-8388608);
   edffile->signalHeader(1)->setDigitalMax(8388607);
   BOOST_REQUIRE(edffile->writeHeader());
   setup = edffile->header()->getMeasurementSetup();
   for (int i=0;i<10;i++){
        edffile->writePhysicalData(testSignal.mid(i*1000,1000).data(),1000);
        edffile->writePhysicalData(testSignal2.mid(i*500,500).data(),500);
   }

   BOOST_REQUIRE(edffile->close());
   BOOST_REQUIRE_EQUAL(edffile->getDataRecordDuration(),1.);
   BOOST_REQUIRE_EQUAL(edffile->getDataRecords(),10LL);
   BOOST_REQUIRE_EQUAL(edffile->signalHeader(0)->getSignalSampleCount(),1000);
   BOOST_REQUIRE_EQUAL(edffile->signalHeader(1)->getSignalSampleCount(),500);

   delete edffile;

   EDFPatient patient2;
   EDFMeasurementSetup setup2;
   edffile = new EDFFile();
   BOOST_REQUIRE(edffile->open("test2.bdf",EDFFile::readOnlyReadAllAnnotations));
   patient2 = edffile->header()->getPatient();
   setup2 = edffile->header()->getMeasurementSetup();
   BOOST_REQUIRE(patient2.getCode()==patient.getCode());
   BOOST_REQUIRE(patient2.getName()==patient.getName());
   BOOST_REQUIRE_EQUAL(patient2.getBirthdate(),patient.getBirthdate());
   BOOST_REQUIRE_EQUAL(patient2.getGender(),patient.getGender());
   BOOST_REQUIRE(setup2.getAdminCode()==setup.getAdminCode());
   BOOST_REQUIRE(setup2.getDate().toString()==setup.getDate().toString());
   BOOST_REQUIRE(setup2.getRecordingAdditional()==setup.getRecordingAdditional());
   BOOST_REQUIRE(setup2.getTechnician()==setup.getTechnician());
   BOOST_REQUIRE_EQUAL(edffile->getSignalCount(),3);
   BOOST_REQUIRE_EQUAL(edffile->getDataRecordDuration(),1.);
   BOOST_REQUIRE_EQUAL(edffile->getDataRecords(),10LL);
   BOOST_REQUIRE_EQUAL(edffile->signalHeader(0)->getSignalSampleCount(),1000);
   BOOST_REQUIRE_EQUAL(edffile->signalHeader(1)->getSignalSampleCount(),500);
   BOOST_REQUIRE(edffile->signalHeader(0)->getSamplePointer()==0LL);
   BOOST_REQUIRE(edffile->signalHeader(1)->getSamplePointer()==0LL);

   //BOOST_REQUIRE_EQUAL(edffile->getHeaderSize(),256+(256*3));
   BOOST_REQUIRE(edffile->signalHeader(0)->getSamplePointer()==0LL);
   BOOST_REQUIRE(edffile->signalHeader(1)->getSamplePointer()==0LL);
   //BOOST_REQUIRE_EQUAL(edffile->recordSize,(1000+500+38)*3);
   BOOST_REQUIRE_EQUAL(edffile->signalHeader(0)->getBufferOffset(),0);
   BOOST_REQUIRE_EQUAL(edffile->signalHeader(1)->getBufferOffset(),3000);


   BOOST_REQUIRE(edffile->readPhysicalData(0,10000)>0);
   qDebug("max error %g",edffile->signalHeader(0)->getResolution());
   for (int i=0;i<testSignal.size();i++){
       if (i>=9999) qDebug("i %d",i);
       BOOST_REQUIRE((edffile->signalData(0)->at(i)-testSignal.at(i))<edffile->signalHeader(0)->getResolution());
//       if ((buf[i]-testSignal.at(i))>1e-7){
//           qDebug("i %d, ist-soll %g",i,buf[i]-testSignal.at(i));
//       }
   }
   edffile->rewind(1);
   BOOST_REQUIRE(edffile->readPhysicalData(1,5000)==5000);
   qDebug("max error %g",edffile->signalHeader(1)->getResolution());
   for (int i=0;i<testSignal2.size();i++){
       BOOST_REQUIRE((edffile->signalData(1)->at(i)-testSignal2.at(i))<edffile->signalHeader(1)->getResolution());
       // BOOST_REQUIRE((edffile->signalData(1,i)-testSignal2.at(i))<1e-4);
//       if ((edffile->signalData(1,i)-testSignal2.at(i))>1e-7){
//           qDebug("i %d, ist %f soll2 %f soll1 %f",i,edffile->signalData(1,i),testSignal2.at(i),testSignal.at(i));
//       }
   }


   edffile->close();
   delete edffile;

   edffile = new EDFFile();
   BOOST_REQUIRE(edffile->open("test2.bdf",EDFFile::readOnlyReadAllAnnotations));


   for (int i=0;i<2500;i++){
       BOOST_REQUIRE(edffile->readPhysicalData(0,4)==4);
       BOOST_REQUIRE(edffile->readPhysicalData(1,2)==2);
   }
   for (int i=0;i<testSignal.size();i++){
       BOOST_REQUIRE((edffile->signalData(0)->at(i)-testSignal.at(i))<edffile->signalHeader(0)->getResolution());
//       if ((buf[i]-testSignal.at(i))>1e-7){
//           qDebug("i %d, ist-soll %g",i,buf[i]-testSignal.at(i));
//       }
   }
   for (int i=0;i<testSignal2.size();i++){
       BOOST_REQUIRE((edffile->signalData(1)->at(i)-testSignal2.at(i))<edffile->signalHeader(1)->getResolution());
       // BOOST_REQUIRE((edffile->signalData(1,i)-testSignal2.at(i))<1e-4);
//       if ((edffile->signalData(1,i)-testSignal2.at(i))>1e-6){
//           qDebug("i %d, ist-soll %g",i,edffile->signalData(1,i)-testSignal2.at(i));
//       }
   }

   qDebug() << "values from interval [2s,4s] of signal 0: " << edffile->getInterval(0,2,4).size()
            << "samples in interval";
   qDebug() << "values from interval [0s,2s] of signal 1: " << edffile->getInterval(1,0,2).size()
            << "samples in interval";
   edffile->close();
   delete edffile;
}


BOOST_AUTO_TEST_CASE(testHeaderParsing)
//void EDFFileTest::testHeaderParsing()
{
    EDFFile *edffile;
    double TA,TA2;
    QVector<double> testSignal;
    QVector<double> testSignal2;
    TA=1./1000.;
    testSignal.resize(10000);
    for (int i=0;i<testSignal.size();i++){
        testSignal[i]= sin(2.* M_PI * 10. * (double)i*TA);
    }

    TA2=1./500.;
    testSignal2.resize(5000);
    for (int i=0;i<testSignal2.size();i++){
        testSignal2[i]= cos(2.* M_PI * 10. * (double)i*TA2);
    }
    edffile = new EDFFile();
    edffile->header()->setFiletype(EDFFileHeader::BDFPlus);
    edffile->setSignalCount(1,true);
    EDFPatient patient;
    patient.setCode("1");
    patient.setAdditional("add");
    patient.setBirthdate(EDFDate(10,2,1990));
    patient.setGender(EDFPatient::Female);
    patient.setName("name");
    EDFMeasurementSetup setup;
    setup.setAdminCode("admincode");
    setup.setEquipment("equipment");
    setup.setRecordingAdditional("recording");
    setup.setTechnician("technician");

    edffile->header()->setPatient(patient);
    edffile->setSignalCount(1);
    edffile->signalHeader(0)->setSignalSampleFrequency(1000);
    edffile->signalHeader(0)->setPhysicalMin(-1.);
    edffile->signalHeader(0)->setPhysicalMax(1.);
    edffile->signalHeader(0)->setDigitalMin(-8388608);
    edffile->signalHeader(0)->setDigitalMax(8388607);

    //QByteArray edfhdr = edffile->createHeader();
    //BOOST_REQUIRE(edfhdr.size()==256);
    //QByteArray edfsighdr = edffile->createSignalHeader();
    //BOOST_REQUIRE(edfsighdr.size()==256*2);

    delete edffile;

    edffile = new EDFFile();
    edffile->setSignalCount(1,true);
    BOOST_REQUIRE_EQUAL(edffile->header()->getFiletype(),EDFFileHeader::BDFPlus);
    //BOOST_REQUIRE(edffile->parseHeader(edfhdr));
    BOOST_REQUIRE_EQUAL(edffile->header()->getFiletype(),EDFFileHeader::BDFPlus);
   // BOOST_REQUIRE(edffile->parseSignalHeader(edfsighdr));
    BOOST_REQUIRE_EQUAL(edffile->header()->getFiletype(),EDFFileHeader::BDFPlus);
    EDFPatient patient2 = edffile->header()->getPatient();
    //BOOST_REQUIRE(patient2.getCode()==patient.getCode());
}

BOOST_AUTO_TEST_CASE(testFileRepair)
//void EDFFileTest::testFileCreation2()
{
    EDFFile *edffile;
    double TA, TA2;
    QVector<double> testSignal;
    QVector<double> testSignal2;
    TA = 1. / 1000.;
    testSignal.resize(10000);
    for (int i = 0; i<testSignal.size(); i++){
        testSignal[i] = sin(2.* M_PI * 10. * (double)i*TA);
    }

    TA2 = 1. / 500.;
    testSignal2.resize(5000);
    for (int i = 0; i<testSignal2.size(); i++){
        testSignal2[i] = cos(2.* M_PI * 10. * (double)i*TA2);
    }

    edffile = new EDFFile();
    edffile->header()->setFiletype(EDFFileHeader::BDFPlus);

    BOOST_REQUIRE(edffile->open("test3.bdf", EDFFile::writeOnly));
    EDFPatient patient;
    patient.setCode("1");
    patient.setAdditional("add");
    patient.setBirthdate(EDFDate(10, 2, 1990));
    patient.setGender(EDFPatient::Female);
    patient.setName("name");
    EDFMeasurementSetup setup;
    setup.setAdminCode("admincode");
    setup.setEquipment("equipment");
    setup.setRecordingAdditional("recording");
    setup.setTechnician("technician");

    edffile->header()->setPatient(patient);
    edffile->header()->setMeasurementSetup(setup);
    edffile->setSignalCount(2);
    edffile->signalHeader(0)->setSignalSampleFrequency(1000);
    edffile->signalHeader(0)->setPhysicalMin(-1.);
    edffile->signalHeader(0)->setPhysicalMax(1.);
    edffile->signalHeader(0)->setDigitalMin(-8388608);
    edffile->signalHeader(0)->setDigitalMax(8388607);

    edffile->signalHeader(1)->setSignalSampleFrequency(500);
    edffile->signalHeader(1)->setPhysicalMin(-1.);
    edffile->signalHeader(1)->setPhysicalMax(1.);
    edffile->signalHeader(1)->setDigitalMin(-8388608);
    edffile->signalHeader(1)->setDigitalMax(8388607);
    BOOST_REQUIRE(edffile->writeHeader());
    setup = edffile->header()->getMeasurementSetup();
    for (int i = 0; i<10; i++){
        edffile->writePhysicalData(testSignal.mid(i * 1000, 1000).data(), 1000);
        edffile->writePhysicalData(testSignal2.mid(i * 500, 500).data(), 500);
    }

    delete edffile;
    edffile = new EDFFile();
    BOOST_REQUIRE(edffile->open("test3.bdf", EDFFile::readOnlyReadAllAnnotations));



    for (int i = 0; i<2500; i++){
        BOOST_REQUIRE(edffile->readPhysicalData(0, 4) == 4);
        BOOST_REQUIRE(edffile->readPhysicalData(1, 2) == 2);
    }
    for (int i = 0; i<testSignal.size(); i++){
        BOOST_REQUIRE((edffile->signalData(0)->at(i) - testSignal.at(i))<edffile->signalHeader(0)->getResolution());
        //       if ((buf[i]-testSignal.at(i))>1e-7){
        //           qDebug("i %d, ist-soll %g",i,buf[i]-testSignal.at(i));
        //       }
    }
    for (int i = 0; i<testSignal2.size(); i++){
        BOOST_REQUIRE((edffile->signalData(1)->at(i) - testSignal2.at(i))<edffile->signalHeader(1)->getResolution());
        // BOOST_REQUIRE((edffile->signalData(1,i)-testSignal2.at(i))<1e-4);
        //       if ((edffile->signalData(1,i)-testSignal2.at(i))>1e-6){
        //           qDebug("i %d, ist-soll %g",i,edffile->signalData(1,i)-testSignal2.at(i));
        //       }
    }
    edffile->close();
    delete edffile;

}


BOOST_AUTO_TEST_SUITE_END()
