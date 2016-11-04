#include "EDFError.h"
EDFError::EDFError()
{
    code = EDFError::NoError;
    msg = "";
}
EDFError::EDFError(ErrorType error, const QString& mess)
    : code(error)

{
    msg = QString(mess);
}

EDFError::EDFError(const EDFError& other)
    : code(other.code)
    , msg(other.msg)
{
}

EDFError& EDFError::operator=(const EDFError& other)
{
    if (this->msg == other.msg && this->code == other.code)
        return *this;

    code = other.code;
    msg = other.msg;

    return *this;
}

EDFError::ErrorType EDFError::type() const { return code; }

QString EDFError::message() const { return msg; }

bool EDFError::isValid() const { return (code != NoError); }

QDebug operator<<(QDebug dbg, const EDFError& msg)
{
    dbg.nospace() << "SerialDeviceError(" << msg.message() << ")";
    return dbg.space();
}
