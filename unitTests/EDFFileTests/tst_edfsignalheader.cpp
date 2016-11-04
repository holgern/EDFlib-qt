#include "BoostTest.h"

#include <QString>
#include <QObject>
#include "EDFSignalHeader.h"

 BOOST_AUTO_TEST_SUITE( EDFSignalHeaderTest )

//void EDFSignalHeaderTest::testHeaderCreation()
//{
//    edfsignalheader = new EDFSignalHeader(true,false);


//    delete edfsignalheader;

//}
 BOOST_AUTO_TEST_CASE(testSetGetMethods)
//void EDFSignalHeaderTest::testSetGetMethods()
 {

    EDFSignalHeader *edfsignalheader = new EDFSignalHeader(false,false);
    edfsignalheader->setSignalSampleFrequency(1000);
    BOOST_REQUIRE_EQUAL(edfsignalheader->getSignalSampleCount(),1000);

    edfsignalheader->setLabel("label");
    BOOST_REQUIRE(edfsignalheader->getLabel()==QString("label"));
    edfsignalheader->setPhysicalDimension("dim");
    BOOST_REQUIRE(edfsignalheader->getPhysicalDimension()==QString("dim"));
    edfsignalheader->setPrefilter("filter");
    BOOST_REQUIRE(edfsignalheader->getPrefilter()==QString("filter"));
    edfsignalheader->setTransducer("electrode");
    BOOST_REQUIRE(edfsignalheader->getTransducer()==QString("electrode"));
    edfsignalheader->setPhysicalMax(10);
    BOOST_REQUIRE_EQUAL(edfsignalheader->getPhysicalMax(),10.0);
    edfsignalheader->setPhysicalMin(-10);
    BOOST_REQUIRE_EQUAL(edfsignalheader->getPhysicalMin(),-10.0);
    edfsignalheader->setDigitalMax(8388607);
    BOOST_REQUIRE_EQUAL(edfsignalheader->getDigitalMax(),8388607);
    edfsignalheader->setDigitalMin(-8388608);
    BOOST_REQUIRE_EQUAL(edfsignalheader->getDigitalMin(),-8388608);
    delete edfsignalheader;
    //AnnotationChannel
    edfsignalheader = new EDFSignalHeader(true,false);
    BOOST_REQUIRE_EQUAL(edfsignalheader->getSignalSampleCount(),38);
    BOOST_REQUIRE(edfsignalheader->getLabel()==QString("BDF Annotations"));
    BOOST_REQUIRE(edfsignalheader->getPhysicalDimension()==QString(""));
    BOOST_REQUIRE(edfsignalheader->getPrefilter()==QString(""));
    BOOST_REQUIRE(edfsignalheader->getTransducer()==QString(""));
    BOOST_REQUIRE_EQUAL(edfsignalheader->getPhysicalMax(),1.0);
    BOOST_REQUIRE_EQUAL(edfsignalheader->getPhysicalMin(),-1.0);
    BOOST_REQUIRE_EQUAL(edfsignalheader->getDigitalMax(),8388607);
    BOOST_REQUIRE_EQUAL(edfsignalheader->getDigitalMin(),-8388608);
    delete edfsignalheader;
    edfsignalheader = new EDFSignalHeader(true,true);
    BOOST_REQUIRE_EQUAL(edfsignalheader->getSignalSampleCount(),57);
    BOOST_REQUIRE(edfsignalheader->getLabel()==QString("EDF Annotations"));
    BOOST_REQUIRE(edfsignalheader->getPhysicalDimension()==QString(""));
    BOOST_REQUIRE(edfsignalheader->getPrefilter()==QString(""));
    BOOST_REQUIRE(edfsignalheader->getTransducer()==QString(""));
    BOOST_REQUIRE_EQUAL(edfsignalheader->getPhysicalMax(),1.0);
    BOOST_REQUIRE_EQUAL(edfsignalheader->getPhysicalMin(),-1.0);
    BOOST_REQUIRE_EQUAL(edfsignalheader->getDigitalMax(),32767);
    BOOST_REQUIRE_EQUAL(edfsignalheader->getDigitalMin(),-32768);
    delete edfsignalheader;
}

//void EDFSignalHeaderTest::testStringParsing(){

//    edfsignalheader = new EDFSignalHeader();
//    edfsignalheader->setSignalSampleFrequency(1000);
//    edfsignalheader->setLabel("label");
//    edfsignalheader->setPhysicalDimension("dim");
//    edfsignalheader->setPrefilter("filter");
//    edfsignalheader->setTransducer("electrode");
//    edfsignalheader->setPhysicalMax(10);
//    edfsignalheader->setPhysicalMin(-10);
//    edfsignalheader->setDigitalMax(8388607);
//    edfsignalheader->setDigitalMin(-8388608);
//    QString header = edfsignalheader->getHeaderString();
//    QVERIFY(header.size()==256);
//    delete edfsignalheader;
//    qDebug() << header;

//    edfsignalheader = new EDFSignalHeader();
//    QVERIFY(edfsignalheader->parseSignalHeader(header));
//    BOOST_REQUIRE_EQUAL(edfsignalheader->getSignalSampleCount(),1000);
//    BOOST_REQUIRE_EQUAL(edfsignalheader->getLabel(),QString("label"));
//    BOOST_REQUIRE_EQUAL(edfsignalheader->getPhysicalDimension(),QString("dim"));
//    BOOST_REQUIRE_EQUAL(edfsignalheader->getPrefilter(),QString("filter"));
//    BOOST_REQUIRE_EQUAL(edfsignalheader->getTransducer(),QString("electrode"));
//    BOOST_REQUIRE_EQUAL(edfsignalheader->getPhysicalMax(),10.0);
//    BOOST_REQUIRE_EQUAL(edfsignalheader->getPhysicalMin(),-10.0);
//    BOOST_REQUIRE_EQUAL(edfsignalheader->getDigitalMax(),8388607);
//    BOOST_REQUIRE_EQUAL(edfsignalheader->getDigitalMin(),-8388608);
//    delete edfsignalheader;
//}



BOOST_AUTO_TEST_SUITE_END()
