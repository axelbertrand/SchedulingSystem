#ifndef TASK_H_INCLUDED
#define TASK_H_INCLUDED

#include <vector>
#include <memory>
#include "Server.h"

class Task
{
    public :
        Task(unsigned int size, Server::Type serverType);

        void setDependentTask(std::shared_ptr<Task> task);
		bool hasParent() const;
		Task* getParent() const;
		Task* getLastParent() const;
		unsigned int getTaskNo() const;
		
		friend std::ostream& operator<<(std::ostream& stream, const Task& task);

    private :
        static unsigned int totalTasksNumber;

        unsigned int mTaskNo;
        unsigned int mSize;
        Server::Type mServerType;
        std::vector<Task> mDependencies;
		std::shared_ptr<Task> mParent;
};

#endif // TASK_H_INCLUDED
