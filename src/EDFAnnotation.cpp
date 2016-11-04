#include "EDFAnnotation.h"

#include <iomanip>

EDFAnnotation::EDFAnnotation()
{
    onset = 0;
    duration = -1;
}

EDFAnnotation::EDFAnnotation(long long onset, long long duration, QString ann)
{
    this->onset = (onset >= 0) ? onset : 0;
    this->duration = (duration >= 0) ? duration : 0;
    this->annotations.append(ann);
}

EDFAnnotation::EDFAnnotation(double onset, double duration, QString ann)
{
    this->onset = (onset >= 0) ? onset * EDFLIB_TIME_DIMENSION : 0;
    this->duration = (duration >= 0) ? duration * EDFLIB_TIME_DIMENSION : 0;
    this->annotations.append(ann);
}

EDFAnnotation::EDFAnnotation(long long onset, long long duration, QVector<QString> ann)
{
    this->onset = (onset >= 0) ? onset : 0;
    this->duration = (duration >= 0) ? duration : 0;
    this->annotations.append(ann);
}

EDFAnnotation::EDFAnnotation(double onset, double duration, QVector<QString> ann)
{
    this->onset = (onset >= 0) ? onset * EDFLIB_TIME_DIMENSION : 0;
    this->duration = (duration >= 0) ? duration * EDFLIB_TIME_DIMENSION : 0;
    this->annotations.append(ann);
}

// EDFAnnotation::EDFAnnotation(const EDFAnnotation& orig) {
//    onset = orig.onset;
//    duration = orig.duration;
//    strings = orig.strings;
//}

EDFAnnotation::~EDFAnnotation() {}

// EDFAnnotation& EDFAnnotation::operator=(const EDFAnnotation& rhs) {
//    if (this != &rhs) {
//        onset = rhs.onset;
//        duration = rhs.duration;
//        strings = rhs.strings;
//    }

//    return *this;
//}

std::ostream& operator<<(std::ostream& s, EDFAnnotation& ann)
{
    QVector<QString>::iterator it;
    s << std::fixed << std::setprecision(1);
    for (it = ann.annotations.begin(); it != ann.annotations.end(); ++it)
        s << ann.onset << " " << ann.duration << " " << (*it).toStdString() << std::endl;
    return s;
}

long long EDFAnnotation::getOnsetInEDFTime() const
{ return onset; }

long long EDFAnnotation::getOnsetIn100USec() const
{
    return onset / 1000LL;
}

double EDFAnnotation::getOnsetInSec() const
{
    return static_cast<double>(onset) / EDFLIB_TIME_DIMENSION;
}

long long EDFAnnotation::getDurationInEDFTime() const
{ return duration; }

long long EDFAnnotation::getDurationIn100USec() const
{
    return duration  / 1000LL;
}

double EDFAnnotation::getDurationInSec() const
{
    return static_cast<double>(duration) / EDFLIB_TIME_DIMENSION;
}

QString EDFAnnotation::getText(int index) const
{
    if (annotations.size() > index && index >= 0)
        return annotations.at(index);
    else
        return QString("");
}


QVector<QString> EDFAnnotation::getList() const
{
   return annotations;
}


void EDFAnnotation::setOnsetInEDFTime(long long onset)
{
    if (onset >= 0)
        this->onset = onset;
}

void EDFAnnotation::setOnsetInSec(double onset)
{
    if (onset >= 0)
        this->onset = static_cast<long long>(onset * EDFLIB_TIME_DIMENSION);
}

void EDFAnnotation::setDurationInEDFTime(long long duration)
{
    if (duration >= 0)
        this->duration = duration;
}


void EDFAnnotation::setDurationInSec(double duration)
{
    if (duration >= 0)
        this->duration = static_cast<long long>(duration * EDFLIB_TIME_DIMENSION);
}

void EDFAnnotation::setText(const QString& ann)
{
    annotations.clear();
    this->annotations.append(ann);
}

void EDFAnnotation::setList(const QVector<QString>& ann)
{
    annotations = ann;
}

