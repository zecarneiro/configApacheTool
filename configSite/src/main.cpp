/**
 * Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
 */
#include "../lib/includes.h"
#include "../lib/ConfigSiteTool.h"

/**
 * Create config directory if not exist
 */
void createConfigFolder() {
	string command = "mkdir -p " + CONFIGFOLDER;
	system(command.c_str());
}

/**
 * Main
 */
int main(int argc, char *argv[]){
	string argvCpp[argc];

	// Copy all argv
	for (int i = 0; i < argc; ++i) {
		argvCpp[i] = argv[i];
	}

	// Create config directory if not exist
	createConfigFolder();

	// Execute based on input
	ConfigSiteTool *configSite;
	configSite = new ConfigSiteTool();
	configSite->readInputAndSelectCLI(argc, argvCpp);

	// Delete
	delete configSite;

  	// exit
  	return 0;
}