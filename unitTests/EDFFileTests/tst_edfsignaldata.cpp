#include <QString>
#include "BoostTest.h"

#include <QObject>
#include "EDFSignalData.h"
 BOOST_AUTO_TEST_SUITE( EDFSignalDataTest )
  BOOST_AUTO_TEST_CASE(testSetGetMethods)
//void EDFSignalDataTest::testDataCreation()
 {
    EDFSignalData * edfsignaldata = new EDFSignalData();
    BOOST_REQUIRE(edfsignaldata->size()==0);
    edfsignaldata->addPhysicalData(1.);
    edfsignaldata->addPhysicalData(2.);
    BOOST_REQUIRE(edfsignaldata->size()==2);
    BOOST_REQUIRE_EQUAL(edfsignaldata->at(0),1.0);

    BOOST_REQUIRE_EQUAL(edfsignaldata->at(1),2.0);
    edfsignaldata->clear();
    BOOST_REQUIRE(edfsignaldata->size()==0);

    delete edfsignaldata;
}
//void EDFSignalDataTest::testSetGetMethods(){

//}
BOOST_AUTO_TEST_SUITE_END()
