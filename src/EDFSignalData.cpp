#include "EDFSignalData.h"

// EDFSignalData::EDFSignalData(double newFrequency, double max, double min) {
//    this->max = max;
//    this->min = min;
//    frequency = newFrequency;
//}

// EDFSignalData::EDFSignalData(const EDFSignalData& orig) {
//    max = orig.max;
//    min = orig.min;
//    frequency = orig.frequency;
//    data = orig.data;
//}

// EDFSignalData::~EDFSignalData() {}

// EDFSignalData& EDFSignalData::operator=(const EDFSignalData& rhs) {
//    max = rhs.max;
//    min = rhs.min;
//    frequency = rhs.frequency;
//    data = rhs.data;

//    return *this;
//}

// void EDFSignalData::addElement(int16_t val) {
//    data.push_back(val);
//}

// void EDFSignalData::addElements(vector<int16_t> vals) {
//    for (vector<int16_t>::iterator it = vals.begin(); it != vals.end(); it++)
//        data.push_back(*it);
//}

// void EDFSignalData::addElements(int16_t* vals, int length) {
//    for (int i = 0; i < length; i++)
//        data.push_back(vals[i]);
//}

// double EDFSignalData::getLength() { return data.size(); }

// double EDFSignalData::getFrequency() { return frequency; }

// vector<int16_t> EDFSignalData::getData() { return data; }

// double EDFSignalData::getMax() { return max; }

// double EDFSignalData::getMin() { return min; }

EDFSignalData::EDFSignalData()
{
    max = 1;
    min = -1;
    frequency = 0;
}
// EDFSignalData::EDFSignalData(const EDFSignalData& orig) {
//    max = orig.max;
//    min = orig.min;
//    frequency = orig.frequency;
//    data = orig.data;
//}

EDFSignalData::~EDFSignalData() {}

void EDFSignalData::addPhysicalData(double in) { data.append(in); }
void EDFSignalData::addPhysicalData(double* buf, int l)
{
    for (int i = 0; i < l; i++)
        data.append(buf[i]);
}
