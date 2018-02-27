#include "Task.h"

unsigned int Task::totalTaskNumber = 0;

Task::Task(unsigned int size, Server::Type serverType) :
mTaskNo(++totalTaskNumber),
mSize(size),
mServerType(serverType),
mDependencies()
{
}

void Task::addDependency(const Task& task)
{
    mDependencies.push_back(task);
}
