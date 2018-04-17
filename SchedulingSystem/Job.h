#ifndef JOB_H_INCLUDED
#define JOB_H_INCLUDED

#include "Task.h"
#include <vector>

class Job
{
    public :
        Job();
		
		void addTask(std::shared_ptr<Task> task, double dependencyTestRatio);
		std::vector<std::shared_ptr<Task>> getTasks() const;
		
		friend std::ostream& operator<<(std::ostream& stream, const Job& job);

    private :
        static unsigned int totalJobsNumber;

        unsigned int mJobNo;
        std::vector<std::shared_ptr<Task>> mInitialTasks;
};

#endif // JOB_H_INCLUDED
