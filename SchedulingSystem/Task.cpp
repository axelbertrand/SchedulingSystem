#include "Task.h"
#include <string>

unsigned int Task::totalTasksNumber = 0;

Task::Task(unsigned int size, Server::Type serverType) :
mTaskNo(++totalTasksNumber),
mSize(size),
mServerType(serverType),
mDependencies(),
mParent()
{
}

std::shared_ptr<Task> Task::getSharedThis()
{
	return shared_from_this();
}

void Task::setDependentTask(std::shared_ptr<Task> task)
{
	std::weak_ptr<Task> lastParent = getLastParent();

	std::shared_ptr<Task> thisTask = lastParent.lock();
	if (!thisTask)
		thisTask = getSharedThis();

	thisTask->mParent = task;
	
	task->mDependencies.push_back(thisTask);
}

bool Task::hasParent() const
{
	return mParent != nullptr;
}

std::shared_ptr<Task> Task::getParent() const
{
	return mParent;
}

std::shared_ptr<Task> Task::getLastParent() const
{
	std::shared_ptr<Task> currentTask = getParent();
	while (currentTask != nullptr)
	{
		if (!currentTask->hasParent())
			break;

		currentTask = currentTask->getParent();
	}	

	return currentTask;
}

unsigned int Task::getTaskNo() const
{
	return mTaskNo;
}

std::ostream& operator<<(std::ostream& stream, const Task& task)
{
	stream << "T" << task.mTaskNo << " = ";
	
	std::string serverTypeString;
	char prefix;
	switch (task.mServerType)
	{
		case Server::Type::CPU :
			serverTypeString = "CPU";
			prefix = 'G';
			break;
		case Server::Type::GPU :
			serverTypeString = "GPU";
			prefix = 'T';
			break;
		case Server::Type::IO :
			serverTypeString = "I/O";
			prefix = 'G';
			break;
	}
	
	stream << serverTypeString << ", " << task.mSize << prefix << ", ";
	
	stream << "[";
	for(int i = 0; i < task.mDependencies.size(); ++i)
	{
		std::shared_ptr<Task> currentTask = task.mDependencies[i].lock();
		if (currentTask)
		{
			stream << "T" << currentTask->mTaskNo;
			if(i < task.mDependencies.size() - 1)
				stream << ",";
		}
	}
	stream << "]";
	
	return stream;
}