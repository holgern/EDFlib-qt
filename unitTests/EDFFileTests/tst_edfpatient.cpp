#include <QString>
#include "BoostTest.h"

#include <QObject>
#include "EDFPatient.h"

BOOST_AUTO_TEST_SUITE( EDFPatientTest )

BOOST_AUTO_TEST_CASE(testPatientCreation)
//void EDFPatientTest::testPatientCreation()
{
    EDFPatient *edfpatient = new EDFPatient("id","code","name","add",EDFPatient::Female,EDFDate(2,3,2004));
    BOOST_REQUIRE(edfpatient->getId()==QString("id"));

    BOOST_REQUIRE(edfpatient->getCode()==QString("code"));
    BOOST_REQUIRE(edfpatient->getName()==QString("name"));
    BOOST_REQUIRE(edfpatient->getAdditional()==QString("add"));
    BOOST_REQUIRE_EQUAL(edfpatient->getGender(),EDFPatient::Female);

    BOOST_REQUIRE_EQUAL(edfpatient->getBirthdate(),EDFDate(2,3,2004));


    delete edfpatient;
}
BOOST_AUTO_TEST_CASE(testSetGetMethods)
//void EDFPatientTest::testSetGetMethods()
{

    EDFPatient *edfpatient = new EDFPatient();
    edfpatient->setCode(QString(" wq%1"));
    BOOST_REQUIRE(edfpatient->getCode()==QString("wq%1"));

    edfpatient->setName(QString(" wq%1"));
    BOOST_REQUIRE(edfpatient->getName()==QString("wq%1"));

    edfpatient->setAdditional(QString(" wq%1"));
    BOOST_REQUIRE(edfpatient->getAdditional()==QString("wq%1"));

    edfpatient->setGender(EDFPatient::Male);
    BOOST_REQUIRE_EQUAL(edfpatient->getGender(),EDFPatient::Male);

    edfpatient->setBirthdate(EDFDate(12,2,1985));
    BOOST_REQUIRE_EQUAL(edfpatient->getBirthdate(),EDFDate(12,2,1985));

    edfpatient->setName(QString(" w q_1"));
    BOOST_REQUIRE(edfpatient->getName()==QString("w_q_1"));

    //invalid qdate
    edfpatient->setBirthdate(EDFDate(12,2,1985));
    edfpatient->setBirthdate(EDFDate(QDate().fromString("01.05.2001","dd.mm.yy")));
    BOOST_REQUIRE_EQUAL(edfpatient->getBirthdate(),EDFDate(1,1,1900));
    delete edfpatient;

}
BOOST_AUTO_TEST_CASE(testStringParsing)
//void EDFPatientTest::testStringParsing()
{
    EDFPatient *edfpatient = new EDFPatient("id","code","name","add",EDFPatient::Female,EDFDate(2,3,2004));
    QString header = edfpatient->toHeaderInfoString();
    BOOST_REQUIRE(header.size()==80);
    //qDebug() << header;
    delete edfpatient;

    edfpatient = new EDFPatient();
    BOOST_REQUIRE(edfpatient->parseHeaderString(header,true));
    BOOST_REQUIRE(edfpatient->getId()==QString(""));
    BOOST_REQUIRE(edfpatient->getCode()==QString("code"));
    BOOST_REQUIRE(edfpatient->getName()==QString("name"));
    BOOST_REQUIRE(edfpatient->getAdditional()==QString("add"));
    BOOST_REQUIRE_EQUAL(edfpatient->getGender(),EDFPatient::Female);
    BOOST_REQUIRE_EQUAL(edfpatient->getBirthdate(),EDFDate(2,3,2004));
    BOOST_REQUIRE(!edfpatient->parseHeaderString(header.mid(7,-1),true));
    //qDebug() << header.mid(7,-1);
    //qDebug() << header.mid(0,18);
    BOOST_REQUIRE(!edfpatient->parseHeaderString(header.mid(0,18),true));
    delete edfpatient;

    edfpatient = new EDFPatient();
    BOOST_REQUIRE(edfpatient->parseHeaderString(header.mid(0,6),false));
    BOOST_REQUIRE(edfpatient->getAdditional()==QString("code_F"));
    delete edfpatient;

    edfpatient = new EDFPatient();
    edfpatient->setCode(QString(" "));
    edfpatient->setName(QString(" "));
    edfpatient->setAdditional(QString(" "));
    edfpatient->setGender(EDFPatient::Male);
    edfpatient->setName(QString(" "));
    edfpatient->setBirthdate(EDFDate(QDate().fromString("01.05.2001","dd.mm.yy")));
    BOOST_REQUIRE(edfpatient->toHeaderInfoString().trimmed()==QString("X M 01-JAN-1900 X X"));

    delete edfpatient;
}
BOOST_AUTO_TEST_SUITE_END()
