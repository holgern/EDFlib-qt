#ifndef EDFERROR_H
#define EDFERROR_H
/*
 * Copyright (c) Holger Nahrstaedt 
 */

#include <QString>
#include <QDebug>

/*!
 * \brief The EDFError class
 *
 * isValid() is true if an error occured
 */
class EDFError
{
public:
    enum ErrorType
    {
        NoError = 0,
        MallocError = 1,
        NoSuchFileOrDirectory,
        FileContainsFormatErrors,
        MaxFilesReached,
        AlreadyOpened,
        FileTypeError,
        FileWriteError,
        FileReadError,
        FileReadOnly,
        FileWriteOnly,
        CouldNotBeClosed,
        NumberOfSingalsInvalid,
        FileIsDiscontinuous,
        InvalidReadAnnotsValue,
        NoSignals,
        TooManySignals,
        NoSamplesInRecord,
        DigMinIsDigMax,
        DigMaxLowerThanDigMin,
        PhysMinIsPhysMax

    };
    EDFError();
    EDFError(ErrorType error, const QString& message);
    EDFError(const EDFError& other);
    EDFError& operator=(const EDFError& other);
    ErrorType type() const;
    QString message() const;
    bool isValid() const;

private:
    ErrorType code;
    QString msg;
};
QDebug operator<<(QDebug, const EDFError&);
#endif  // EDFERROR_H
