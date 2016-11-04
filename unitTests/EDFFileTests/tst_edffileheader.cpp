
#include <QString>
#include "BoostTest.h"

#include <QObject>
#include "EDFFileHeader.h"

BOOST_AUTO_TEST_SUITE( EDFFileHeaderTest )

BOOST_AUTO_TEST_CASE(testHeaderCreation)
{
    EDFFileHeader *edfheader = new EDFFileHeader(EDFFileHeader::BDFPlus,EDFFileHeader::CONTINUOUS);
    BOOST_REQUIRE_EQUAL(edfheader->getFiletype(),EDFFileHeader::BDFPlus);
    BOOST_REQUIRE_EQUAL(edfheader->getContinuity(),EDFFileHeader::CONTINUOUS);
    delete edfheader;

}

BOOST_AUTO_TEST_CASE(testStringParsing)
//void EDFFileHeaderTest::testStringParsing()
{
    EDFFileHeader *edfheader = new EDFFileHeader(EDFFileHeader::BDFPlus,EDFFileHeader::CONTINUOUS);
    edfheader->setPatient(EDFPatient("id","code","name","add",EDFPatient::Female,EDFDate(2,3,2004)));
    edfheader->setMeasurementSetup(EDFMeasurementSetup(edfheader->getStartDate(),"admincode","technician","equipment","recordingAdditional","recording"));
    QString headerstr = edfheader->getHeaderString(2,10000000LL);
    BOOST_REQUIRE(headerstr.size()==256);
    delete edfheader;
    //qDebug() << header;

    edfheader = new EDFFileHeader();
    BOOST_REQUIRE(edfheader->parseHeader(headerstr));
    BOOST_REQUIRE(edfheader->getPatient().getCode()==QString("code"));
    BOOST_REQUIRE(edfheader->getPatient().getName()==QString("name"));
    BOOST_REQUIRE(edfheader->getPatient().getAdditional()==QString("add"));
    BOOST_REQUIRE_EQUAL(edfheader->getPatient().getGender(),EDFPatient::Female);
    BOOST_REQUIRE_EQUAL(edfheader->getPatient().getBirthdate(),EDFDate(2,3,2004));

    BOOST_REQUIRE_EQUAL(edfheader->getContinuity(),EDFFileHeader::CONTINUOUS);
    BOOST_REQUIRE_EQUAL(edfheader->getFiletype(),EDFFileHeader::BDFPlus);

    //BOOST_REQUIRE_EQUAL(edfheader->getMeasurementSetup().getDate(),EDFDate(2,3,2004));
    BOOST_REQUIRE(edfheader->getMeasurementSetup().getAdminCode()==QString("admincode"));
    BOOST_REQUIRE(edfheader->getMeasurementSetup().getTechnician()==QString("technician"));
    BOOST_REQUIRE(edfheader->getMeasurementSetup().getEquipment()==QString("equipment"));
    BOOST_REQUIRE(edfheader->getMeasurementSetup().getRecordingAdditional()==QString("recordingAdditional"));


    delete edfheader;

}


BOOST_AUTO_TEST_SUITE_END()
