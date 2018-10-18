/**
 * Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
 */
#include "../lib/includes.h"
#include "../lib/ConfigSiteTool.h"

void help(char* executable) {
	cout << "#### HELP ####" << endl;
	cout << executable << " (-e|-iP|-iS|-iF|-iD) ARGS\n" << endl;
	cout << "\t -e: Execute the app\n";
	cout << "\t -iP: Insert existing and active project\n";
	cout << "\t\t ARGS: \"Name\" Port\n";
	cout << "\t -iS: Insert Server\n";
	cout << "\t\t ARGS: \"Name\" \"Name1\" ...\n";
	cout << "\t -iF: Insert Framework\n";
	cout << "\t\t ARGS: \"Name\" \"Name1\" ...\n";
	cout << "\t -iD: Insert Directory\n";
	cout << "\t\t ARGS: \"Name\" \"Name1\" ...\n";
}

void readInputAndSelect(int argc, char* argv[]) {
	ConfigSiteTool *configSite;
	configSite = new ConfigSiteTool();
	string firstArg[] = {"-e", "-iP", "-iS", "-iF", "-iD", "-h"};

	if (firstArg[0].compare(argv[1]) == 0) {
		configSite->initExecution();
	} else if (firstArg[1].compare(argv[1]) == 0) {

	} else if (firstArg[2].compare(argv[1]) == 0) {
		
	} else if (firstArg[3].compare(argv[1]) == 0) {
		
	} else if (firstArg[4].compare(argv[1]) == 0) {

	} else if (firstArg[5].compare(argv[1]) == 0) {
		help(argv[0]);
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