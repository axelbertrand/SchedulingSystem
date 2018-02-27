#include "Server.h"

Server::Server(int operationNumber, char prefix, Type serverType) :
mOperationNumber(operationNumber),
mPrefix(prefix),
mServerType(serverType)
{
}

int Server::getOperationNumber() const
{
    return mOperationNumber;
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
    stream << server.mOperationNumber << server.mPrefix;
    return stream;
}
