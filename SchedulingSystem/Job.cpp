#include "Job.h"

#include <random>
#include <chrono>
#include <algorithm>

unsigned int Job::totalJobsNumber = 0;

Job::Job() :
mJobNo(++totalJobsNumber),
mInitialTasks()
{
}

void Job::addTask(Task& task, double dependencyTestRatio)
{
	std::default_random_engine randomEngine(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<double> distribDependencyTest(0, 1);
	
	std::vector<Task*> visitedParents;
	
	for(Task* initialTask : mInitialTasks)
	{
		if(initialTask->hasParent())
		{
			Task* lastParent = initialTask->getLastParent();
			if(std::search_n(visitedParents.begin(), visitedParents.end(), 1, lastParent) == visitedParents.end())
			{
				visitedParents.push_back(lastParent);
				if(distribDependencyTest(randomEngine) < dependencyTestRatio)
				{
					lastParent->setDependentTask(std::move(task));
					return;
				}
			}
		}
		else
		{
			if(distribDependencyTest(randomEngine) < dependencyTestRatio)
			{
				initialTask->setDependentTask(std::move(task));
				return;
			}
		}
	}

	mInitialTasks.push_back(&task);
}

std::vector<Task*> Job::getTasks() const
{
	std::vector<Task*> visitedTasks;
	for(Task* initialTask : mInitialTasks)
	{
		Task* currentTask = initialTask;
		do
		{
			if(std::search_n(visitedTasks.begin(), visitedTasks.end(), 1, currentTask) == visitedTasks.end())
			{
				visitedTasks.push_back(currentTask);
			}
			currentTask = currentTask->getParent();
		}
		while(currentTask->hasParent());
	}

	return visitedTasks;
}

std::ostream& operator<<(std::ostream& stream, const Job& job)
{
	stream << "Job " << job.mJobNo << " = [";
	std::vector<Task*> allTasks(job.getTasks());
	for(int i = 0; i < allTasks.size(); ++i)
	{
		stream << "T" << allTasks[i]->getTaskNo();
		if(i < allTasks.size() - 1)
			stream << ", ";
	}
	stream << "]" << std::endl;
	
	for(int i = 0; i < allTasks.size(); ++i)
		stream << allTasks[i] << std::endl;
	
	return stream;
}