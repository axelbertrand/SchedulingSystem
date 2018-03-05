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
        static const unsigned int MAX_SERVERS_NUMBER;

        unsigned int mServersNumber;
        unsigned int mCpusNumber;
        unsigned int mGpusNumber;
        unsigned int mIosNumber;
};

#endif // SERVERGENERATOR_H_INCLUDED
