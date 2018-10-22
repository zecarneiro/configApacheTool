/**
 * Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
 */
#include "../lib/includes.h"
#include "../lib/ConfigSiteTool.h"

void help(char* executable) {
	cout << "#### HELP ####" << endl;
	cout << executable << " (-e|-iP) ARGS\n" << endl;
	cout << "\t -e: Execute the app\n";
	cout << "\t -iP: Insert existing and active project\n";
	cout << "\t\t ARGS: \"Name\" Port \"Path\"\n";
}
	
void readInputAndSelect(int argc, char* argv[]) {
	
	ConfigSiteTool *configSite;
	configSite = new ConfigSiteTool();
	string firstArg[] = {"-e", "-iP", "-h"};

	if (argc > 1) {
		if (firstArg[0].compare(argv[1]) == 0) {
			configSite->initExecution();
		} else if (firstArg[1].compare(argv[1]) == 0) {
			if (argc >= 4) {
				configSite->insertProjectCLI(argc, argv);
			} else {
				cout << "\nMinimum Argument acepted: 2\n\n";
			}
		} else if (firstArg[2].compare(argv[1]) == 0) {
			help(argv[0]);
		} else {
			cout << "\nInvalid Argument\n\n";
		}
	} else {
		cout << "\nInvalid Argument\n\n";
	}	

	// Delete
	delete configSite;
}

/**
 * Main
 */
int main(int argc, char *argv[]){
	readInputAndSelect(argc, argv);	
  	// exit
  	return 0;
}