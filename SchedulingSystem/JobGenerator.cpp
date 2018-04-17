#include "JobGenerator.h"

#include <random>
#include <chrono>
#include <memory>

const unsigned int JobGenerator::MAX_TASKS_NUMBER = 10000;

JobGenerator::JobGenerator()
{
}

std::vector<Job> JobGenerator::generate() const
{
    std::vector<Job> jobList;
	
	std::default_random_engine randomEngine(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<unsigned int> distribTasksSize(1, 84600);
	std::uniform_int_distribution<unsigned int> distribJobsNumber(1, MAX_TASKS_NUMBER);
	
	unsigned int remainingTasks = distribJobsNumber(randomEngine);
	
	while(remainingTasks > 0)
	{
		Job job;
		std::uniform_int_distribution<unsigned int> distribTasksNumber(1, remainingTasks);
		unsigned int tasksNumber = distribTasksNumber(randomEngine);
		
		std::uniform_int_distribution<unsigned int> distribDependencyTest(0, tasksNumber);

		for(unsigned int i = 1; i <= tasksNumber; ++i)
		{
			Server::Type serverType;
			
			std::uniform_int_distribution<unsigned int> distribServerType(1, 3);
			
			switch(distribServerType(randomEngine))
			{
				case 1 :
					serverType = Server::Type::CPU;
					break;
				case 2 :
					serverType = Server::Type::GPU;
					break;
				case 3 :
					serverType = Server::Type::IO;
					break;
			}
			
			std::shared_ptr<Task> task = std::make_shared<Task>(distribTasksSize(randomEngine), serverType);
			job.addTask(task, static_cast<double>(i) / tasksNumber);
		}
		
		jobList.push_back(job);
		
		remainingTasks -= tasksNumber;
	}

    return jobList;
}
