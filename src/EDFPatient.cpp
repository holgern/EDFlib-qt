

#include "EDFPatient.h"

EDFPatient::EDFPatient() : birthdate(EDFDate())
{
    id = "";
    code = "";
    name = "";
    additional = "";
    gender = Unknown;
}

EDFPatient::EDFPatient(QString newId, QString newCode, QString newName, QString newAdditional,
                       EDFPatient::Gender newGender, EDFDate newBirthdate)
{
    id = newId;
    code = convertString(newCode);
    name = convertString(newName);
    additional = convertString(newAdditional);
    gender = newGender;
    birthdate = newBirthdate;
}

// EDFPatient::EDFPatient(const EDFPatient& orig) {
//    id = orig.id;
//    code = orig.code;
//    name = orig.name;
//    additional = orig.additional;
//    gender = orig.gender;
//    birthdate = orig.birthdate;
//}

// EDFPatient::~EDFPatient() {
//}

// EDFPatient& EDFPatient::operator=(const EDFPatient& rhs) {
//    if (this != &rhs)
//    {
//        id = rhs.id;
//        code = rhs.code;
//        name = rhs.name;
//        additional = rhs.additional;
//        gender = rhs.gender;
//        birthdate = rhs.birthdate;
//    }

//    return *this;
//}

std::ostream& operator<<(std::ostream& s, EDFPatient p)
{
    s << "ID           | " << p.id.toStdString() << std::endl;
    s << "Code         | " << p.code.toStdString() << std::endl;
    s << "Name         | " << p.name.toStdString() << std::endl;
    s << "Gender       | ";
    if (p.gender == EDFPatient::Female)
        s << "Female" << std::endl;
    else if (p.gender == EDFPatient::Male)
        s << "Male" << std::endl;
    else
        s << "X" << std::endl;
    s << "Birthdate    | " << p.birthdate.toString().toStdString() << std::endl;
    s << "Additional   | " << p.additional.toStdString();
    return s;
}

QString EDFPatient::getId() const { return id; }
QString EDFPatient::getCode() const { return code; }
QString EDFPatient::getName() const { return name; }
QString EDFPatient::getAdditional() const { return additional; }
EDFPatient::Gender EDFPatient::getGender() const { return gender; }
EDFDate EDFPatient::getBirthdate() const { return birthdate; }

void EDFPatient::setId(const QString& newId) { id = newId; }

void EDFPatient::setCode(const QString& newCode) { code = convertString(newCode); }

void EDFPatient::setName(const QString& newName) { name = convertString(newName); }

void EDFPatient::setAdditional(const QString& newAdditional) { additional = convertString(newAdditional); }

void EDFPatient::setGender(const EDFPatient::Gender& newGender) { gender = newGender; }

void EDFPatient::setBirthdate(const EDFDate& newBirthdate) { birthdate = newBirthdate; }

QString EDFPatient::convertString(const QString& str)
{
    QString out = str.trimmed();
    // out.remove(QRegExp("[^a-zA-Z\\d\\s]"));
    out = QString(out.toLatin1().data());
    out.replace(QString(" "), QString("_"));
    return out;
}

QString EDFPatient::toHeaderInfoString() const
{
    QString str;

    if (code.isEmpty())
    {
        str += "X ";
    }
    else
    {
        str += convertString(code) + " ";
    }
    if (gender == Male)
    {
        str += "M ";
    }
    else if (gender == Female)
    {
        str += "F ";
    }
    else
    {
        str += "X ";
    }
    str += birthdate.toString().toUpper() + QString(" ");

    if (name.isEmpty())
    {
        str += "X ";
    }
    else
    {
        str += convertString(name) + " ";
    }

    if (additional.isEmpty())
    {
        str += "X";
    }
    else
    {
        str += convertString(additional);
    }

    return str.leftJustified(80, ' ', true);
}

bool EDFPatient::parseHeaderString(const QString& edf_hdr, bool plusFormat)
{
    if (plusFormat)
    {
        QStringList str_list = edf_hdr.split(" ", QString::SkipEmptyParts);
        if (str_list.size() < 4)
            return false;
        if (!birthdate.parseHeaderString(str_list.at(2)))
            return false;
        if (str_list.at(0) != "X")
            setCode(str_list.at(0));
        if (str_list.at(1) != "X")
        {
            if (str_list.at(1) == "M")
                setGender(Male);
            else
                setGender(Female);
        }
        if (str_list.at(3) != "X")
            setName(str_list.at(3));
        if (str_list.size()>4 && str_list.at(4) != "X")
            setAdditional(str_list.at(4));
    }
    else
    {
        if (edf_hdr.size() > 0 && edf_hdr != "X")
            setAdditional(edf_hdr);
    }
    return true;
}
