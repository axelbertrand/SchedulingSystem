#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <vector>
#include <string>
#include <sstream>
#include "Server.h"
#include "ServerGenerator.h"
#include "JobGenerator.h"

using namespace std;

void printServerList(const vector<Server>& serverList, std::ostream& stream);
void printJobList(const vector<Job>& jobList, std::ostream& stream);
vector<Server> readServers(std::ifstream& stream);
vector<Job> readJobs(std::ifstream& stream);

/**
 * Point d'entrée du programme
 * 
 * @param mode string Mode d'utilisation du programme. Deux valeurs possibles : read, generate
 * @param filename string Nom du fichier à utiliser pour lire/générer un plan de scheduling
 * @param params int... Paramètres de génération du plan de scheduling (utilisables uniquement en mode generate
 */
int main(int argc, char **argv)
{
	std::string mode;

	if (argc > 1)
		mode = argv[1];


	if (mode == "read")
	{
		std::cout << "read" << std::endl;
		// Lire le fichier passé dans argv[2]
		std::string fileName = argv[2];

		ifstream file(fileName, std::ios::in);
		if (file)
		{
			vector<Server> serverList = readServers(file);
			vector<Job> jobList = readJobs(file);
		}
		else
		{
			cerr << "Erreur d'ouverture du fichier " << fileName << endl;
		}

	}
	else if (mode == "generate")
	{
		// Générer le fichier avec le nom dans argv[2] en utilisant les paramètres dans argv[3], argv[4]...
		std::cout << "generate" << std::endl;
		std::string fileName = argv[2];

		ServerGenerator serverGenerator;
		JobGenerator jobGenerator;

		vector<Server> cpuList(serverGenerator.generate(Server::Type::CPU));
		vector<Server> gpuList(serverGenerator.generate(Server::Type::GPU));
		vector<Server> ioList(serverGenerator.generate(Server::Type::IO));
		
		vector<Job> jobList(jobGenerator.generate());

		ofstream file(fileName, std::ios::out | std::ios::trunc);
		if (file)
		{
			file << "Servers" << endl;
			printServerList(cpuList, file);
			printServerList(gpuList, file);
			printServerList(ioList, file);

			printJobList(jobList, file);
		}
		else
		{
			cerr << "Erreur d'ouverture du fichier " << fileName << endl;
		}
	}	
	else if (mode == "")
	{
		std::cout << "Aucun mode specifie" << std::endl;
	}
	else
	{
		std::cout << "Mode non reconnu" << std::endl;
	}

	std::cin.clear();
	std::cout << std::endl << "Press any key to continue...";
	std::cin.ignore();

	return 0;
}

void printServerList(const vector<Server>& serverList, std::ostream& stream)
{
	string serverTypeString;
	switch (serverList[0].getServerType())
	{
		case Server::Type::CPU :
			serverTypeString = "CPU";
			break;
		case Server::Type::GPU :
			serverTypeString = "GPU";
			break;
		case Server::Type::IO :
			serverTypeString = "I/O";
			break;
	}

	stream << '\t';
	stream << serverTypeString.c_str() << " = [";
	for (unsigned int i = 0; i < serverList.size(); ++i)
	{
		stream << serverList[i];
		if (i == serverList.size() - 1)
			stream << "]";
		else
			stream << ", ";
	}
	stream << endl;
}

void printJobList(const vector<Job>& jobList, std::ostream& stream)
{
	for(Job job : jobList)
	{
		stream << job << endl;
	}
}

vector<Server> readServers(std::ifstream& stream)
{
	vector<Server> serverList;

	std::string serverString;
	std::string line;

	for (int i = 0; i < 4; ++i)
	{
		getline(stream, line);
		if (i != 0)
		{
			std::string serverTypeString = line.substr(1, 3);
			Server::Type serverType;
			
			if (serverTypeString == "CPU")
			{
				serverType = Server::Type::CPU;
			}
			else if (serverTypeString == "GPU")
			{
				serverType = Server::Type::GPU;
			}
			else if (serverTypeString == "I/O")
			{
				serverType = Server::Type::IO;
			}

			std::string serverListString = line.substr(8, line.size() - 2);
			std::string delimiter = ", ";

			size_t pos = 0;
			std::string token;
			while ((pos = serverListString.find(delimiter)) != std::string::npos) {
				token = serverListString.substr(0, pos);
				serverListString.erase(0, pos + delimiter.length());

				stringstream ss(token.substr(0, token.size() - 2));
				int operationsNumber;
				ss >> operationsNumber;

				char prefix = token[token.size() - 1];

				Server server(operationsNumber, prefix, serverType);
				serverList.push_back(server);
			}
		}
	}

	return serverList;
}

vector<Job> readJobs(std::ifstream& stream)
{
	vector<Job> jobList;

	std::string line;
	while (getline(stream, line))
	{
		if (line[0] != '\t')
		{
			jobList.push_back(Job());
		}
		else
		{
			int i = 2;
			while (line[i] != ' ')
				++i;

			std::string taskNumberString = line.substr(2, i - 2);
			cout << taskNumberString << endl;
			line.erase(0, i + 2);

			Server::Type serverType;
			int operationsNumber;

			i = 0;
			std::string delimiter = ", ";
			size_t pos = 0;
			std::string token;
			/*while ((pos = line.find(delimiter)) != std::string::npos) {
				token = line.substr(0, pos);
				switch(i)
				{
				case 0 :
					std::string serverTypeString = token.substr(0, 3);
					if (serverTypeString == "CPU")
					{
						serverType = Server::Type::CPU;
					}
					else if (serverTypeString == "GPU")
					{
						serverType = Server::Type::GPU;
					}
					else if (serverTypeString == "I/O")
					{
						serverType = Server::Type::IO;
					}
					break;
				case 1 :
					stringstream ss(token.substr(0, token.size() - 2));
					int operationsNumber;
					ss >> operationsNumber;
					break;
				case 3 :
					if (token.size() != 2)
					{
						std::string input = token.substr(1, token.size() - 2);
						std::string d = ", ";
						size_t p = 0;
						std::string t;
						while ((p = input.find(d)) != std::string::npos) {

							token.erase(0, p + d.length());
						}
					}
					break;
				}
				line.erase(0, pos + delimiter.length());

				stringstream ss(token.substr(1));
				int taskNumber;
				ss >> taskNumber;


			}*/
		}
	}

	return jobList;
}