#include "BoostTest.h"


#include <QString>
#include <QObject>
#include "EDFTime.h"


 BOOST_AUTO_TEST_SUITE( EDFTimeTest )

//void EDFTimeTest::testDateCreation()
 BOOST_AUTO_TEST_CASE(DateCreation)
{
    EDFTime *edftime = new EDFTime(10,30,40,100);

    BOOST_REQUIRE_EQUAL(edftime->getHour(),10);

    BOOST_REQUIRE_EQUAL(edftime->getMinute(),30);
    BOOST_REQUIRE_EQUAL(edftime->getSecond(),40);
    BOOST_REQUIRE_EQUAL(edftime->getSubSecond(),100);
    delete edftime;

//    edftime = new EDFTime(30,200,-10,100);
//    QCOMPARE(edftime->getHour(),0);
//    QCOMPARE(edftime->getMinute(),0);
//    QCOMPARE(edftime->getSecond(),0);
//    QCOMPARE(edftime->getSubSecond(),0);
//    delete edftime;

    QTime time = QTime(12,20,30,100);

    edftime = new EDFTime(time);
    BOOST_REQUIRE_EQUAL(edftime->getHour(),12);
    BOOST_REQUIRE_EQUAL(edftime->getMinute(),20);
    BOOST_REQUIRE_EQUAL(edftime->getSecond(),30);
    BOOST_REQUIRE_EQUAL(edftime->getSubSecond(),100*10000);
    delete edftime;

    edftime = new EDFTime(EDFTime(time));
    BOOST_REQUIRE_EQUAL(edftime->getHour(),time.hour());
    BOOST_REQUIRE_EQUAL(edftime->getMinute(),time.minute());
    BOOST_REQUIRE_EQUAL(edftime->getSecond(),time.second());
    BOOST_REQUIRE_EQUAL(edftime->getSubSecond(),time.msec()*10000);
    delete edftime;

    EDFTime *edftime2 = new EDFTime(time);
    edftime = new EDFTime(*edftime2);
    BOOST_REQUIRE_EQUAL(edftime->getHour(),time.hour());
    BOOST_REQUIRE_EQUAL(edftime->getMinute(),time.minute());
    BOOST_REQUIRE_EQUAL(edftime->getSecond(),time.second());
    BOOST_REQUIRE_EQUAL(edftime->getSubSecond(),time.msec()*10000);
    delete edftime;
}

//void EDFTimeTest::testSetGetMethods()
 BOOST_AUTO_TEST_CASE(SetGetMethods)
{
    EDFTime *edftime = new EDFTime(2,3,40,50);
    edftime->setHour(15);
    BOOST_REQUIRE_EQUAL(edftime->getHour(),15);

    edftime->setMinute(5);
    BOOST_REQUIRE_EQUAL(edftime->getMinute(),5);
    edftime->setSecond(20);
    BOOST_REQUIRE_EQUAL(edftime->getSecond(),20);
    edftime->setSubSecond(400000);
    BOOST_REQUIRE_EQUAL(edftime->getSubSecond(),400000);
    QTime time;
    time.setHMS(15,5,20,40);
    BOOST_REQUIRE_EQUAL(edftime->toQTime(),time);

    //QCOMPARE(edftime->toString(),date.toString("dd-MMM-yyyy"));

    delete edftime;
}
//void EDFTimeTest::testParsing()
 BOOST_AUTO_TEST_CASE(parsing)
{

    EDFTime *edftime = new EDFTime();
    BOOST_REQUIRE(edftime->parseHeaderString("14.20.40"));
    QTime time;
    time.setHMS(14,20,40);
    BOOST_REQUIRE_EQUAL(edftime->toQTime(),time);

    BOOST_REQUIRE(!edftime->parseHeaderString("14:20:40"));
    BOOST_REQUIRE(!edftime->parseHeaderString("14:20:40.100"));
    BOOST_REQUIRE(!edftime->parseHeaderString("14.20.40.100"));
    BOOST_REQUIRE(!edftime->parseHeaderString("44.20.40"));
    delete edftime;
}


BOOST_AUTO_TEST_SUITE_END()


//QTEST_MAIN(EDFTimeTest)
//#include "tst_edftime.moc"
