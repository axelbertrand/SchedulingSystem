#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <iostream>

class Server
{
    public :
        enum class Type
        {
            CPU,
            GPU,
            IO,
        };

        Server(int operationNumber, char prefix, Type serverType);

        int getOperationNumber() const;
        char getPrefix() const;
        Type getServerType() const;

        friend std::ostream& operator<<(std::ostream& stream, const Server& server);

    private :
        int mOperationNumber;
        char mPrefix;
        Type mServerType;
};

#endif // SERVER_H_INCLUDED
