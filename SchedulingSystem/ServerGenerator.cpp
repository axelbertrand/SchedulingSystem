#include "ServerGenerator.h"
#include <random>
#include <chrono>

const unsigned int ServerGenerator::MAX_SERVER_NUMBER = 100;

ServerGenerator::ServerGenerator()
{
    std::default_random_engine randomEngine(std::chrono::system_clock::now().time_since_epoch().count());

    std::uniform_int_distribution<unsigned int> distribServerNumber(3, MAX_SERVER_NUMBER);
    mServerNumber = distribServerNumber(randomEngine);

    std::uniform_int_distribution<unsigned int> distribCpuNumber(static_cast<int>(0.25 * mServerNumber), static_cast<int>(0.5 * mServerNumber));
    mCpuNumber = distribCpuNumber(randomEngine);

    std::uniform_int_distribution<unsigned int> distribGpuNumber(static_cast<int>(0.25 * mServerNumber), static_cast<int>(0.75 * mServerNumber - mCpuNumber));
    mGpuNumber = distribGpuNumber(randomEngine);

    mIoNumber = mServerNumber - mCpuNumber - mGpuNumber;
}


std::vector<Server> ServerGenerator::generate(Server::Type serverType) const
{
    std::default_random_engine randomEngine(std::chrono::system_clock::now().time_since_epoch().count());

    unsigned int maxLoad;
    char prefix;
    unsigned int serverTypeNumber;

    switch(serverType)
    {
        case Server::Type::CPU :
            maxLoad = 1000;
            prefix = 'G';
            serverTypeNumber = mCpuNumber;
            break;
        case Server::Type::GPU :
            maxLoad = 1000;
            prefix = 'T';
            serverTypeNumber = mGpuNumber;
            break;
        case Server::Type::IO :
            maxLoad = 10;
            prefix = 'G';
            serverTypeNumber = mIoNumber;
            break;
    }

    std::vector<Server> serverList;

    std::uniform_int_distribution<unsigned int> distribLoad(1, maxLoad);
    for(unsigned int i = 0; i < serverTypeNumber; ++i)
    {
        serverList.push_back(Server(distribLoad(randomEngine), prefix, serverType));
    }

    return serverList;
}
