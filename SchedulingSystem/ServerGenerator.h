#ifndef SERVERGENERATOR_H_INCLUDED
#define SERVERGENERATOR_H_INCLUDED

#include <vector>
#include "Server.h"

class ServerGenerator
{
    public :
        ServerGenerator();

        std::vector<Server> generate(Server::Type serverType) const;

    private :
        static const unsigned int MAX_SERVER_NUMBER;

        unsigned int mServerNumber;
        unsigned int mCpuNumber;
        unsigned int mGpuNumber;
        unsigned int mIoNumber;
};

#endif // SERVERGENERATOR_H_INCLUDED
