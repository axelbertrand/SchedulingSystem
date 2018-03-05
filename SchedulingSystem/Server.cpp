#include "Server.h"

Server::Server(int operationsNumber, char prefix, Type serverType) :
mOperationsNumber(operationsNumber),
mPrefix(prefix),
mServerType(serverType)
{
}

int Server::getOperationsNumber() const
{
    return mOperationsNumber;
}

char Server::getPrefix() const
{
    return mPrefix;
}

Server::Type Server::getServerType() const
{
    return mServerType;
}

std::ostream& operator<<(std::ostream& stream, const Server& server)
{
    stream << server.mOperationsNumber << server.mPrefix;
    return stream;
}
