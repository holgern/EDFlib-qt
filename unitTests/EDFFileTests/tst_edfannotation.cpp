
#include <boost/test/unit_test.hpp>
#include <QVector>
#include "EDFFile.h"

//void EDFAnnotationTest::initTestCase()
//{
//    TA=1./1000.;
//    testSignal.resize(10000);
//    for (int i=0;i<testSignal.size();i++){
//        testSignal[i]= sin(2.* M_PI * 10. * (double)i*TA);
//    }

//}

struct EDFAnnotationFixure
{
    EDFAnnotationFixure()
    {
        TA=1./1000.;
        testSignal.resize(10000);
        for (int i=0;i<testSignal.size();i++){
            testSignal[i]= sin(2.* M_PI * 10. * (double)i*TA);
        }
    }
    QVector<double> testSignal;
    double TA;
};

BOOST_FIXTURE_TEST_SUITE(EDFAnnotationtest, EDFAnnotationFixure)

BOOST_AUTO_TEST_CASE(testAnnotationCreationAtEnd)
{



   EDFFile* edffile = new EDFFile();
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
   BOOST_CHECK(edffile->writeHeader());
   setup = edffile->header()->getMeasurementSetup();
   for (int i=0;i<10;i++)
        edffile->writePhysicalData(testSignal.mid(i*1000,1000).data(),1000);

   edffile->addAnnotation(EDFAnnotation(0.0,-1,"test1"));
   edffile->addAnnotation(EDFAnnotation(0.6, -1, "test2"));
  
   BOOST_CHECK(edffile->close());

   delete edffile;

   edffile = new EDFFile();
   BOOST_REQUIRE(edffile->open("test.bdf",EDFFile::readOnlyReadAllAnnotations));
   
   BOOST_REQUIRE(edffile->getAnnotations().size() == 2);
   BOOST_CHECK_CLOSE(edffile->getAnnotations().at(0).getOnsetInSec(), 0, 0.1);
   BOOST_CHECK(edffile->getAnnotations().at(0).getText() == "test1");
   BOOST_CHECK_CLOSE(edffile->getAnnotations().at(1).getOnsetInSec(), 0.6, 0.1);
   BOOST_CHECK_CLOSE(edffile->getAnnotations().at(1).getDurationInSec(), 0, 0.1);
   BOOST_CHECK(edffile->getAnnotations().at(1).getText() == "test2");
   edffile->close();
   delete edffile;

}

BOOST_AUTO_TEST_CASE(testAnnotationCreationAtStart)
{



    EDFFile* edffile = new EDFFile();
    edffile->header()->setFiletype(EDFFileHeader::BDFPlus);

    BOOST_REQUIRE(edffile->open("test.bdf", EDFFile::writeOnly));
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
    edffile->setSignalCount(1);
    edffile->signalHeader(0)->setSignalSampleFrequency(1000);
    edffile->signalHeader(0)->setPhysicalMin(-1.);
    edffile->signalHeader(0)->setPhysicalMax(1.);
    edffile->signalHeader(0)->setDigitalMin(-8388608);
    edffile->signalHeader(0)->setDigitalMax(8388607);
    BOOST_CHECK(edffile->writeHeader());
    setup = edffile->header()->getMeasurementSetup();
    edffile->addAnnotation(EDFAnnotation(0.0, -1, "test1"));
    edffile->addAnnotation(EDFAnnotation(0.6, -1, "test2"));
    for (int i = 0; i<10; i++)
        edffile->writePhysicalData(testSignal.mid(i * 1000, 1000).data(), 1000);



    BOOST_CHECK(edffile->close());

    delete edffile;

    edffile = new EDFFile();
    BOOST_REQUIRE(edffile->open("test.bdf", EDFFile::readOnlyReadAllAnnotations));

    BOOST_REQUIRE(edffile->getAnnotations().size() == 2);
    BOOST_CHECK_CLOSE(edffile->getAnnotations().at(0).getOnsetInSec(), 0, 0.1);
    BOOST_CHECK(edffile->getAnnotations().at(0).getText() == "test1");
    BOOST_CHECK_CLOSE(edffile->getAnnotations().at(1).getOnsetInSec(), 0.6, 0.1);
    BOOST_CHECK_CLOSE(edffile->getAnnotations().at(1).getDurationInSec(), 0, 0.1);
    BOOST_CHECK(edffile->getAnnotations().at(1).getText() == "test2");
    edffile->close();
    delete edffile;

}

BOOST_AUTO_TEST_CASE(testReadAnnotations)
{
    EDFFile* edffile = new EDFFile();
    edffile->header()->setFiletype(EDFFileHeader::BDFPlus);

    //BOOST_REQUIRE(edffile->open("4-546-Zunge_edited_reduced.bdf",EDFFile::readOnlyReadAllAnnotations));
    //BOOST_CHECK(edffile->getAnnotations().at(0).getOnsetInSec() == 0);
    //BOOST_CHECK(edffile->getAnnotations().at(0).getText() == "start(11:38:03.742)");
    //BOOST_CHECK_CLOSE(edffile->getAnnotations().at(1).getOnsetInSec(), 5.5150000, 0.1);
    //BOOST_CHECK(edffile->getAnnotations().at(1).getText() == "Schlucken normal(Speichel)");
    //edffile->close();
    delete edffile;
}

BOOST_AUTO_TEST_SUITE_END()
