#include <QString>
#include "BoostTest.h"

#include <QObject>
#include "EDFMeasurementSetup.h"

BOOST_AUTO_TEST_SUITE( EDFMeasurementSetupTest )

BOOST_AUTO_TEST_CASE(testSetupCreation)
//void EDFMeasurementSetupTest::testSetupCreation()
{
    EDFMeasurementSetup *edfsetup = new EDFMeasurementSetup(EDFDate(2,3,2004),"admincode","technician","equipment","recordingAdditional","recording");
    BOOST_REQUIRE_EQUAL(edfsetup->getDate(),EDFDate(2,3,2004));

    BOOST_REQUIRE(edfsetup->getAdminCode()==QString("admincode"));

    BOOST_REQUIRE(edfsetup->getTechnician()==QString("technician"));
    BOOST_REQUIRE(edfsetup->getEquipment()==QString("equipment"));
    BOOST_REQUIRE(edfsetup->getRecordingAdditional()==QString("recordingAdditional"));
    BOOST_REQUIRE(edfsetup->getRecording()==QString("recording"));


    delete edfsetup;
}
BOOST_AUTO_TEST_CASE(testSetGetMethods)
//void EDFMeasurementSetupTest::testSetGetMethods()
{
    EDFMeasurementSetup *edfsetup = new EDFMeasurementSetup();
    edfsetup->setDate(EDFDate(2,4,2020));
    BOOST_REQUIRE_EQUAL(edfsetup->getDate(),EDFDate(2,4,2020));
    edfsetup->setAdminCode(" wq% 1");
    BOOST_REQUIRE(edfsetup->getAdminCode()==QString("wq%_1"));
    edfsetup->setTechnician(" wq%1");
    BOOST_REQUIRE(edfsetup->getTechnician()==QString("wq%1"));
    edfsetup->setEquipment(" wq%1");
    BOOST_REQUIRE(edfsetup->getEquipment()==QString("wq%1"));
    edfsetup->setRecordingAdditional(" wq%1");
    BOOST_REQUIRE(edfsetup->getRecordingAdditional()==QString("wq%1"));
    edfsetup->setRecording(" wq%1");
    BOOST_REQUIRE(edfsetup->getRecording()==QString("wq%1"));

    delete edfsetup;
}
BOOST_AUTO_TEST_CASE(testStringParsing)
//void EDFMeasurementSetupTest::testStringParsing()
{
    EDFMeasurementSetup *edfsetup = new EDFMeasurementSetup(EDFDate(2,3,2004),"admincode","technician","equipment","recordingAdditional","recording");
    QString header = edfsetup->toHeaderInfoString();
    BOOST_REQUIRE(header.size()==80);
    //qDebug() << header;
    delete edfsetup;

    edfsetup = new EDFMeasurementSetup();
    BOOST_REQUIRE(edfsetup->parseHeaderString(header,true));
    BOOST_REQUIRE_EQUAL(edfsetup->getDate(),EDFDate(2,3,2004));
    BOOST_REQUIRE(edfsetup->getAdminCode()==QString("admincode"));
    BOOST_REQUIRE(edfsetup->getTechnician()==QString("technician"));
    BOOST_REQUIRE(edfsetup->getEquipment()==QString("equipment"));
    BOOST_REQUIRE(edfsetup->getRecordingAdditional()==QString("recordingAdditional"));
    delete edfsetup;
}


BOOST_AUTO_TEST_SUITE_END()
