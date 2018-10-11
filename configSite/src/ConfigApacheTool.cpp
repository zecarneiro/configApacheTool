/**
 * Create By José M. C. Noronha
 * Lisbon 10 May 2018
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
 */
#include "../lib/ConfigApacheTool.h"

/**
 * Constructor
 */
ConfigApacheTool::ConfigApacheTool() {
	this->copyTemplate  = this->commands.sudo + " " + this->commands.copy + " " + this->APACHEVIRTUALCONF;
	this->nomeProjecto = "novoProjectoCake";
	this->porto = 9000;
	this->oldPorto = -1;
	this->pathWWW  = "PATH_WWW";
}

/**
 * Destructor
 */
ConfigApacheTool::~ConfigApacheTool() {}

/**
 * Clear cin
 */
void ConfigApacheTool::clearCin(){
	cin.clear();
    cin.ignore(256,'\n');
}

/**
 * Print Menu
 */
void ConfigApacheTool::printMenu() {
	cout << "\n Options 1, 2, 3 and 6 also activate project on apache !!!\n\n";
	cout << "1 - Create new project CakePHP" << endl;
	cout << "2 - Config existing CakePHP project(Run composer install)" << endl;
 	cout << "3 - Change port for an project(For activated project)" << endl;
 	cout << "4 - Disable an existing project(Not delete project folder)" << endl;
 	cout << "5 - Update existing project" << endl;
 	cout << "6 - Only activate project(On apache)" << endl;
	cout << "7 - Show all active project" << endl;
	cout << "8 - Exit" << endl;
	cout << "Introduz a opcao: ";
}

/**
 * Get only number from cin
 */
int ConfigApacheTool::getOlyInteger(string errorMessage) {
	int integer;
	cin >> integer;

	// ask when user not introduce integer
    while(cin.fail()) {
        cout << errorMessage;
        clearCin();
        cin >> integer;
    }
    return integer;
}

/**
 * Convert intinger to string
 */
string ConfigApacheTool::intToString(int integerToString) {
	// get Int to convert to string
	stringstream ssInt;
	ssInt << integerToString;
	return ssInt.str();
}

/**
 * Verify if port introduced by user is defined o port.conf
 */
bool ConfigApacheTool::verifyPort(int port) {
	// Define Variable
	string portConverted = intToString(port);
	bool portIsUsed = false;
	string outputToCompared = "Listen " + portConverted;
	string output;

	// Construct command
	string command = apacheConfig.verifyPort + " -w \"" + portConverted + "\"";

	// Exec and get output command
	FILE *fp;
	char outputCommand[1000];

	// Executa o comando e guarda em um ficheiro
	fp = popen(command.c_str(), "r");

	// Faz a leitura do output do comando
	while (fgets(outputCommand, sizeof(outputCommand), fp) != NULL) 
	{
		output = outputCommand;

		// Comparo o resultado do output com a string esperada.
		if (output.compare(outputToCompared) || output.compare(outputToCompared + "\n")) {
			portIsUsed = true;
			break;
		}
	}
	// Fecha o ficheiro
	pclose(fp);

	// Retorna o resultado
	return portIsUsed;
}

/**
 * Get all necessary info for the project
 */
void ConfigApacheTool::getInfoProject(bool isNewProject) {
	string errorPortoMessage = "\nInsert valid port: ";
	string printMessagePorto;
	string confirm;

	// clear cin
	this->clearCin();

    /**
     * Get info
     */
    // Pede o nome do projecto
	cout << "Insert project name( Or full path for the project ): ";
	getline(cin, this->nomeProjecto);

	// Pede o número do porto
	printMessagePorto = (isNewProject) ? "Insert port: " : "Insert new port: ";
	cout << printMessagePorto;
	this->porto = this->getOlyInteger(errorPortoMessage);
	this->clearCin();

	// Pede o porto enquanto o portio indicado já esta a ser usado
	while(verifyPort(this->porto)) {
		cout << "Port inserted is used by other project." << endl;
		cout << "Continue? (S/N): ";
		getline(cin, confirm);

		if (confirm == "S" || confirm == "s"){
			break;
		} else if (confirm == "N" || confirm == "n") {
			cout << printMessagePorto;
			this->porto = this->getOlyInteger(errorPortoMessage);
		}
		this->clearCin();
	}
}

/**
 * Configura, activa e reinicia as novas configurações no apache
 */
void ConfigApacheTool::configApache(bool isChangePort) {
	string cmdToExec;
	string porto = intToString(this->porto);
	string pathProjecto = this->pathWWW + "/" + this->nomeProjecto;
	string webrootFolder = "webroot/";

	// Virtual Conf Porto(Comando a executar) : sudo sed -i 's/PORTO/porto/' /etc/apache2/sites-available/nomeProjecto.conf
	cmdToExec = this->commands.sudo + " " + this->commands.sed + " -i " + "'s/PORTO/" + porto + "/' ";
	cmdToExec += apacheConfig.sitesEnable + this->nomeProjecto + ".conf";
	system(cmdToExec.c_str());

	// Virtual Conf Pasta do Projecto(Comando a executar) : sudo sed -i 's/FULL_PATH_PROJECT/nomeProjecto/' /etc/apache2/sites-available/nomeProjecto.conf
	cmdToExec = this->commands.sudo + " " + this->commands.sed + " -i " + "\"s#FULL_PATH_PROJECT#" + pathProjecto + "/" + webrootFolder + "#\" ";
	cmdToExec += apacheConfig.sitesEnable + this->nomeProjecto + ".conf";
	system(cmdToExec.c_str());

	// Se o porto ainda não não foi definido no ficheiro ports.conf então é inserido este porto
	if (!verifyPort(this->porto)) {
		// Ports conf : sudo sed -i "/Listen 80/a\\Listen PORTOPROJECTO" ports.conf
		cmdToExec = this->commands.sudo + " " + this->commands.sed + " -i " + "\"/Listen 80/a\\\\Listen " + porto + "\" ";
		cmdToExec += apacheConfig.apachePath + apacheConfig.portsConf;
		system(cmdToExec.c_str());
	}

	// Desactiva o site sudo a2dissite nome_projecto.conf
	if (isChangePort){
		cmdToExec = this->commands.sudo + " " + this->apacheConfig.disableSite + " " + this->nomeProjecto + ".conf";
		system(cmdToExec.c_str());

		// Reload and Restart Apache
		this->apacheConfig.reloadRestart(-1);
	}

	// Activa o site sudo a2ensite nome_projecto.conf
	cmdToExec = this->commands.sudo + " " + this->apacheConfig.enableSite + " " + this->nomeProjecto + ".conf";
	system(cmdToExec.c_str());

	// Reload and Restart Apache
	this->apacheConfig.reloadRestart(-1);
}

/**
 * Execute all necessary command. Call apache command here
 */
void ConfigApacheTool::executeAllNecessaryCommand(int option) {
	string cmdToExec;
	string oldPorto;
	string groupApache = "www-data";

	// Copy tamplate
	this->copyTemplate += " " + apacheConfig.sitesEnable + this->nomeProjecto + ".conf";
	system(this->copyTemplate.c_str());

	// Config Apache
	if (option == 3){
		this->configApache(true);
	} else {
		this->configApache(false);
	}

	switch (option) {
		case 1:
			// Construct command
			cmdToExec = "composer create-project --prefer-dist cakephp/app";
			cmdToExec += " " + this->pathWWW + "/" + this->nomeProjecto;
			system(cmdToExec.c_str());
			break;
		case 2:
			// Contruct command
			cmdToExec = "composer install -d " + this->pathWWW + "/" + this->nomeProjecto;
			system(cmdToExec.c_str());
			break;
		case 3:
			oldPorto = intToString(this->oldPorto);
			// Ports conf : sed -i '/Listen OLD_PORTO/d' ports.conf
			cmdToExec = this->commands.sudo + " " + this->commands.sed + " -i " + "\"/Listen " + oldPorto + "/d\" ";
			cmdToExec += apacheConfig.apachePath + apacheConfig.portsConf;
			system(cmdToExec.c_str());
			break;			
	}

	if(option != 3) {
		// Set group to new project folder
		cmdToExec = this->commands.sudo + " " + this->commands.chown + " -R :" + groupApache + " " + this->pathWWW + "/" + this->nomeProjecto;
		system(cmdToExec.c_str());

		// Set permission on logs folder
		cmdToExec = this->commands.sudo + " " + this->commands.chmod + " -R 777 " + this->pathWWW + "/" + this->nomeProjecto;
		system(cmdToExec.c_str());
	}
}

/**
 * Disable Project
 */
void ConfigApacheTool::disableProject(){
	string cmdToExec, porto;

	// Remove Port
	porto = this->intToString(this->porto);
	cmdToExec = this->commands.sudo + " " + this->commands.sed;
	cmdToExec += " -i /Listen " + porto + "/d" + this->apacheConfig.apachePath + "/" + this->apacheConfig.portsConf;
	system(cmdToExec.c_str());

	// Remove VirtualHost
	cmdToExec = this->commands.sudo + " " + this->commands.del + " ";
	cmdToExec += this->apacheConfig.apachePath + "/sites-available/" + this->nomeProjecto;
	system(cmdToExec.c_str());

	// Disable Site
	cmdToExec = this->commands.sudo + " " + this->apacheConfig.disableSite + " " + this->nomeProjecto;
	system(cmdToExec.c_str());

	// Restart and Reload Apache
	this->apacheConfig.reloadRestart(-1);
}

/**
 * Save info on file
 */
void ConfigApacheTool::saveInfo(string name, int porto, bool isDelete) {
	string operation = (isDelete) ? " delete " : " save ";
	string sPorto = intToString(porto);
	string cmdToExec = "bash " + APPFOLDER + "infoListProject.sh" + operation + name + " " + sPorto;
	system(cmdToExec.c_str());
}