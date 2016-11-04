#ifndef EDFFILEMANAGER_H
#define EDFFILEMANAGER_H
/*
 * Copyright (c) Holger Nahrstaedt
 */

#include <QString>
#include <QThread>
#include <QtCore>
#include "EDFFile.h"
/*!
 * \brief The EDFFileManager class
 */
class EDFFileManager : public QObject
{
    Q_OBJECT

public:
    EDFFileManager(){};
    virtual ~EDFFileManager();

    // EDFFile* openNewFileWriteOnly(QString name, EDFFile::FileType type, int channelNr);
    // EDFFile* openNewFileReadOnly();
private:
    // QString fileName;
    QList<EDFFile*> edfFiles;
};

#endif  // EDFFILEMANAGER_H
