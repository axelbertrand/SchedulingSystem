#ifndef TASKGENERATOR_H_INCLUDED
#define TASKGENERATOR_H_INCLUDED

#include "Job.h"
#include <vector>

class JobGenerator
{
    public :
        JobGenerator();

        std::vector<Job> generate() const;

    private :
        static const unsigned int MAX_TASKS_NUMBER;
};

#endif // TASKGENERATOR_H_INCLUDED
