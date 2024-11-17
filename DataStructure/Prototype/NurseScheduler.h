#ifndef NURSESCHEDULER_H
#define NURSESCHEDULER_H

#include "Schedule.h"
#include <string>

class NurseScheduler {
public:
    Schedule schedule;
    void loadFromCSV(const std::string& filePath);
};

#endif // NURSESCHEDULER_H
