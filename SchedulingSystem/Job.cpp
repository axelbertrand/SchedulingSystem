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

void Job::addTask(std::shared_ptr<Task> task, double dependencyTestRatio)
{
	std::default_random_engine randomEngine(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<double> distribDependencyTest(0, 1);
	
	std::vector<Task*> visitedParents;
	bool isDependent = false;
	
	for(auto initialTask : mInitialTasks)
	{
		std::weak_ptr<Task> lastParent = initialTask->getLastParent();
		std::shared_ptr<Task> currentTask = lastParent.lock();
		if (!currentTask)
			currentTask = initialTask;
		
		if(std::search_n(visitedParents.begin(), visitedParents.end(), 1, currentTask.get()) == visitedParents.end())
		{
			visitedParents.push_back(currentTask.get());
			if(distribDependencyTest(randomEngine) < dependencyTestRatio)
			{
				currentTask->setDependentTask(task);
				visitedParents.push_back(task.get());
				isDependent = true;
			}
		}
	}

	if(!isDependent)
		mInitialTasks.push_back(task);
}

void Job::addTask(std::shared_ptr<Task> task, std::vector<unsigned int> dependentTaskNumbers)
{
	if (dependentTaskNumbers.empty())
	{
		mInitialTasks.push_back(task);
		return;
	}

	std::vector<std::shared_ptr<Task>> visitedTasks;
	for (auto initialTask : mInitialTasks)
	{
		std::shared_ptr<Task> currentTask = initialTask;
		do
		{
			if (std::search_n(visitedTasks.begin(), visitedTasks.end(), 1, currentTask) == visitedTasks.end())
			{
				visitedTasks.push_back(currentTask);
				for (unsigned int dependentTaskNumber : dependentTaskNumbers)
				{
					if (currentTask->getTaskNo() == dependentTaskNumber)
					{
						currentTask->setDependentTask(task);
						break;
					}
				}
			}

			if (!currentTask->hasParent())
				break;

			currentTask = currentTask->getParent();
		} while (currentTask != nullptr);
	}
}

std::vector<std::shared_ptr<Task>> Job::getTasks() const
{
	std::vector<std::shared_ptr<Task>> visitedTasks;
	for (auto initialTask : mInitialTasks)
	{
		std::shared_ptr<Task> currentTask = initialTask;
		do
		{
			if(std::search_n(visitedTasks.begin(), visitedTasks.end(), 1, currentTask) == visitedTasks.end())
			{
				visitedTasks.push_back(currentTask);
			}

			if (!currentTask->hasParent())
				break;

			currentTask = currentTask->getParent();
		}
		while(currentTask != nullptr);
	}

	sort(visitedTasks.begin(), visitedTasks.end(), [](std::shared_ptr<Task> t1, std::shared_ptr<Task> t2) {
		return t1->getTaskNo() < t2->getTaskNo();
	});

	return visitedTasks;
}

std::ostream& operator<<(std::ostream& stream, const Job& job)
{
	stream << "Job " << job.mJobNo << " = [";
	std::vector<std::shared_ptr<Task>> allTasks(job.getTasks());
	for(int i = 0; i < allTasks.size(); ++i)
	{
		stream << "T" << allTasks[i]->getTaskNo();
		if(i < allTasks.size() - 1)
			stream << ", ";
	}
	stream << "]" << std::endl;
	
	for (int i = 0; i < allTasks.size(); ++i)
		stream << '\t' << *allTasks[i] << std::endl;
	
	return stream;
}