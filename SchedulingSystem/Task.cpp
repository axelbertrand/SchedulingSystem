#include "Task.h"
#include <string>

unsigned int Task::totalTasksNumber = 0;

Task::Task(unsigned int size, Server::Type serverType) :
mTaskNo(++totalTasksNumber),
mSize(size),
mServerType(serverType),
mDependencies(),
mParent(nullptr)
{
}

void Task::setDependentTask(std::shared_ptr<Task> task)
{
	
	Task* lastParent = getLastParent();
	Task* thisTask(lastParent != nullptr ? lastParent : this);
	thisTask->mParent = task;
	
	task->mDependencies.push_back(*thisTask);
}

bool Task::hasParent() const
{
	return mParent != nullptr;
}

Task* Task::getParent() const
{
	return mParent.get();
}

Task* Task::getLastParent() const
{
	Task* lastParent = getParent();
	while (lastParent != nullptr)
	{
		if (lastParent->mParent == nullptr)
			return lastParent;

		lastParent = lastParent->getParent();
	}	

	return nullptr;
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
		stream << "T" << task.mDependencies[i].mTaskNo;
		if(i < task.mDependencies.size() - 1)
			stream << ",";
	}
	stream << "]";
	
	return stream;
}