#include "Job.h"

unsigned int Job::totalJobNumber = 0;

Job::Job() :
mJobNo(++totalJobNumber),
mTasks()
{
}
