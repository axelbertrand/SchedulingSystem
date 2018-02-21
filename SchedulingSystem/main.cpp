#include <iostream>

/**
 * Point d'entr�e du programme
 * 
 * @param mode string Mode d'utilisation du programme. Deux valeurs possibles : read, generate
 * @param filename string Nom du fichier � utiliser pour lire/g�n�rer un plan de scheduling
 * @param params int... Param�tres de g�n�ration du plan de scheduling (utilisables uniquement en mode generate
 */
int main(int argc, char **argv)
{
	std::string mode;

	if (argc > 1)
		mode = argv[1];


	if (mode == "read")
	{
		std::cout << "read" << std::endl;
		// Lire le fichier pass� dans argv[2]
	}
	else if (mode == "generate")
	{
		std::cout << "generate" << std::endl;
		// G�n�rer le fichier avec le nom dans argv[2] en utilisant les param�tres dans argv[3], argv[4]...
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