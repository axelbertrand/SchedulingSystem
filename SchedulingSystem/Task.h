#ifndef TASK_H_INCLUDED
#define TASK_H_INCLUDED

#include <vector>
#include "Server.h"

class Task
{
    public :
        Task(unsigned int size, Server::Type serverType);

        void addDependency(const Task& task);

    private :
        static unsigned int totalTaskNumber;

        unsigned int mTaskNo;
        unsigned int mSize;
        Server::Type mServerType;
        std::vector<Task> mDependencies;
};

#endif // TASK_H_INCLUDED
