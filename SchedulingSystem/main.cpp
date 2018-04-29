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
vector<vector<Server>> readServers(std::ifstream& stream);
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
			vector<vector<Server>> serverLists = readServers(file);
			vector<Job> jobList = readJobs(file);

			for (vector<Server> serverList : serverLists)
			{
				printServerList(serverList, cout);
			}
			printJobList(jobList, cout);
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

vector<vector<Server>> readServers(std::ifstream& stream)
{
	vector<Server> cpuList;
	vector<Server> gpuList;
	vector<Server> ioList;

	std::string serverString;
	std::string line;

	for (int i = 0; i < 4; ++i)
	{
		getline(stream, line);
		if (i != 0)
		{
			// Type de serveur
			std::string serverTypeString = line.substr(1, 3);

			// Capacités des serveurs
			std::string serverListString = line.substr(8);
			serverListString.erase(serverListString.end() - 1);
			std::string delimiter = ", ";

			size_t pos = 0;
			std::string token;
			do
			{
				pos = serverListString.find(delimiter);
				token = serverListString.substr(0, pos);
				serverListString.erase(0, pos + delimiter.length());

				// Capacité du serveur
				stringstream ss(token.substr(0, token.size() - 1));
				int operationsNumber;
				ss >> operationsNumber;

				// Préfixe
				char prefix = token[token.size() - 1];

				// Ajout du serveur à la liste
				if (serverTypeString == "CPU")
				{
					Server server(operationsNumber, prefix, Server::Type::CPU);
					cpuList.push_back(server);
				}
				else if (serverTypeString == "GPU")
				{
					Server server(operationsNumber, prefix, Server::Type::GPU);
					gpuList.push_back(server);
				}
				else if (serverTypeString == "I/O")
				{
					Server server(operationsNumber, prefix, Server::Type::IO);
					ioList.push_back(server);
				}
				
			} while (pos != std::string::npos);
		}
	}

	vector<vector<Server>> serverLists = { cpuList, gpuList, ioList };
	return serverLists;
}

vector<Job> readJobs(std::ifstream& stream)
{
	vector<Job> jobList;

	std::string line;
	while (getline(stream, line))
	{
		if (line.empty())
		{
			continue;
		}
		else if (line[0] != '\t') // C'est un job
		{
			jobList.push_back(Job());
		}
		else // C'est une tâche
		{
			int i = 2;
			while (line[i] != ' ')
				++i;

			line.erase(0, i + 3);

			// Type de tâche, taille de la tâche, dépendences de la tâche
			Server::Type serverType;
			int operationsNumber;
			std::vector<unsigned int> dependentTaskNumbers;

			i = 0;
			std::string delimiter = ", ";
			size_t pos = 0;
			std::string token;

			std::string serverTypeString;
			stringstream ss;
			do
			{
				pos = line.find(delimiter);
				token = line.substr(0, pos);
				switch(i)
				{
				case 0 : // On récupère le type de tâche
					if (token == "CPU")
					{
						serverType = Server::Type::CPU;
					}
					else if (token == "GPU")
					{
						serverType = Server::Type::GPU;
					}
					else if (token == "I/O")
					{
						serverType = Server::Type::IO;
					}
					break;
				case 1 : // On récupère la taille de la tâche
					ss.str(token.substr(0, token.size() - 1));
					ss >> operationsNumber;
					break;
				case 2 : // On récupère les dépendences de la tâche
					if (token.size() != 2)
					{
						std::string input = token.substr(1, token.size() - 2);
						size_t p = 0;
						do
						{
							p = input.find(delimiter);
							stringstream ss(input.substr(1));
							unsigned int taskNumber;
							ss >> taskNumber;
							dependentTaskNumbers.push_back(taskNumber);

							token.erase(0, p + delimiter.length());
						} while (p != std::string::npos);
					}
					break;
				}
				line.erase(0, pos + delimiter.length());

				++i;
			} while (pos != std::string::npos);

			std::shared_ptr<Task> task = std::make_shared<Task>(operationsNumber, serverType);
			jobList[jobList.size() - 1].addTask(task, dependentTaskNumbers);
		}
	}

	return jobList;
}