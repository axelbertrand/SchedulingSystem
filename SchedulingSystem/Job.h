#ifndef JOB_H_INCLUDED
#define JOB_H_INCLUDED

#include "Task.h"
#include <vector>

class Job
{
    public :
        Job();

    private :
        static unsigned int totalJobNumber;

        unsigned int mJobNo;
        std::vector<Task> mTasks;
};

#endif // JOB_H_INCLUDED
