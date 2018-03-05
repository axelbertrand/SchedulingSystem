#ifndef JOB_H_INCLUDED
#define JOB_H_INCLUDED

#include "Task.h"
#include <vector>

class Job
{
    public :
        Job();
		
		void addTask(Task& Task, double dependencyTestRatio);
		std::vector<Task*> getTasks() const;
		
		friend std::ostream& operator<<(std::ostream& stream, const Job& job);

    private :
        static unsigned int totalJobsNumber;

        unsigned int mJobNo;
        std::vector<Task*> mInitialTasks;
};

#endif // JOB_H_INCLUDED
