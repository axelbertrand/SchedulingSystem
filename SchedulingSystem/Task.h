#ifndef TASK_H_INCLUDED
#define TASK_H_INCLUDED

#include <vector>
#include <memory>
#include "Server.h"

class Task : public std::enable_shared_from_this<Task>
{
    public :
        Task(unsigned int size, Server::Type serverType);
		std::shared_ptr<Task> getSharedThis();

        void setDependentTask(std::shared_ptr<Task> task);
		bool hasParent() const;
		std::shared_ptr<Task> getParent() const;
		std::shared_ptr<Task> getLastParent() const;
		unsigned int getTaskNo() const;
		
		friend std::ostream& operator<<(std::ostream& stream, const Task& task);

    private :
        static unsigned int totalTasksNumber;

        unsigned int mTaskNo;
        unsigned int mSize;
        Server::Type mServerType;
        std::vector<std::weak_ptr<Task>> mDependencies;
		std::shared_ptr<Task> mParent;
};

#endif // TASK_H_INCLUDED
