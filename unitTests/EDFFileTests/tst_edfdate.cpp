#include "EDFDate.h"
// Qt includes
#include <QObject>
#include <QString>
#include "BoostTest.h"

BOOST_TEST_DONT_PRINT_LOG_VALUE(EDFDate)


BOOST_AUTO_TEST_SUITE( EDFDateTest )

BOOST_AUTO_TEST_CASE(testDateCreation)
//void EDFDateTest::testDateCreation()
{
    EDFDate *edfdate = new EDFDate(2,3,1990);
    BOOST_REQUIRE_EQUAL(edfdate->getDay(),2);

    BOOST_REQUIRE_EQUAL(edfdate->getMonth(),3);
    BOOST_REQUIRE_EQUAL(edfdate->getYear(),1990);
    delete edfdate;

    edfdate = new EDFDate(40,20,-1990);
    BOOST_REQUIRE_EQUAL(edfdate->getDay(),1);
    BOOST_REQUIRE_EQUAL(edfdate->getMonth(),1);
    BOOST_REQUIRE_EQUAL(edfdate->getYear(),1);
    delete edfdate;

    QDate today = QDate(2014,3,2);

    edfdate = new EDFDate(today);
    BOOST_REQUIRE_EQUAL(edfdate->getDay(),today.day());
    BOOST_REQUIRE_EQUAL(edfdate->getMonth(),today.month());
    BOOST_REQUIRE_EQUAL(edfdate->getYear(),today.year());
    delete edfdate;

    edfdate = new EDFDate(EDFDate(today));
    BOOST_REQUIRE_EQUAL(edfdate->getDay(),today.day());
    BOOST_REQUIRE_EQUAL(edfdate->getMonth(),today.month());
    BOOST_REQUIRE_EQUAL(edfdate->getYear(),today.year());
    delete edfdate;

    EDFDate *edfdate2 = new EDFDate(today);
    edfdate = new EDFDate(*edfdate2);
    BOOST_REQUIRE_EQUAL(edfdate->getDay(),today.day());
    BOOST_REQUIRE_EQUAL(edfdate->getMonth(),today.month());
    BOOST_REQUIRE_EQUAL(edfdate->getYear(),today.year());
    delete edfdate;
}
BOOST_AUTO_TEST_CASE(testSetGetMethods)
//void EDFDateTest::testSetGetMethods()
{
    EDFDate *edfdate = new EDFDate(2,3,1990);
    edfdate->setDay(15);
    BOOST_REQUIRE_EQUAL(edfdate->getDay(),15);

    edfdate->setMonth(5);
    BOOST_REQUIRE_EQUAL(edfdate->getMonth(),5);
    edfdate->setYear(2011);
    BOOST_REQUIRE_EQUAL(edfdate->getYear(),2011);
    QDate date;
    date.setDate(2011,5,15);
    BOOST_REQUIRE_EQUAL(edfdate->toQDate(),date);

    BOOST_REQUIRE(edfdate->toString()==QString("15-MAY-2011"));
    BOOST_REQUIRE_EQUAL(*edfdate,EDFDate(15,5,2011));
    //Invalid qdate
    BOOST_REQUIRE(!edfdate->fromQDate(QDate().fromString("10.1.2001","dd.mm.yy")));

    BOOST_REQUIRE_EQUAL(*edfdate,EDFDate(15,5,2011));
    delete edfdate;
}
BOOST_AUTO_TEST_CASE(testParsing)
//void EDFDateTest::testParsing()
{
    EDFDate *edfdate = new EDFDate();
    BOOST_REQUIRE(edfdate->parseHeaderString("04-MAR-2014"));
    QDate date;
    date.setDate(2014,3,4);
    BOOST_REQUIRE_EQUAL(edfdate->toQDate(),date);

    BOOST_REQUIRE(!edfdate->parseHeaderString("04.03.2014"));
    BOOST_REQUIRE(!edfdate->parseHeaderString("2014.03.04"));
    BOOST_REQUIRE(!edfdate->parseHeaderString("2014-03-04"));
    BOOST_REQUIRE(!edfdate->parseHeaderString("2014-MAR-04"));
    delete edfdate;
}

BOOST_AUTO_TEST_CASE(dateToString)
{
    EDFDate *edfdate = new EDFDate(2,3,1990);
    QString strDate = edfdate->toString();
    BOOST_REQUIRE(strDate==QString("02-MAR-1990"));

    delete edfdate;
}

BOOST_AUTO_TEST_SUITE_END()

//QTEST_MAIN(EDFDateTest)
//#include "tst_edfdate.moc"
