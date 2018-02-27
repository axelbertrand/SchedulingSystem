#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include "Server.h"
#include "ServerGenerator.h"
#include "JobGenerator.h"

using namespace std;

void printServerList(const vector<Server>& serverList);

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
	}
	else if (mode == "generate")
	{
		std::cout << "generate" << std::endl;
		// Générer le fichier avec le nom dans argv[2] en utilisant les paramètres dans argv[3], argv[4]...

		ServerGenerator serverGenerator;

		vector<Server> cpuList(serverGenerator.generate(Server::Type::CPU));
		vector<Server> gpuList(serverGenerator.generate(Server::Type::GPU));
		vector<Server> ioList(serverGenerator.generate(Server::Type::IO));

		printServerList(cpuList);
		printServerList(gpuList);
		printServerList(ioList);
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

void printServerList(const vector<Server>& serverList)
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

	cout << serverTypeString.c_str() << " = [";
	for (unsigned int i = 0; i < serverList.size(); ++i)
	{
		cout << serverList[i];
		if (i == serverList.size() - 1)
			cout << "]";
		else
			cout << ", ";
	}
	cout << endl;
}