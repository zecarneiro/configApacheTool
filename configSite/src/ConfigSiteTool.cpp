/**
 * Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
 */
#include "../lib/ConfigSiteTool.h"

/**
 * Constructor
 */
ConfigSiteTool::ConfigSiteTool() {
	this->nomeProjecto = "novoProjectoCake";
	this->porto = 9000;
	this->oldPorto = -1;
	this->pathWWW  = PATHWWW;
}

/**
 * Destructor
 */
ConfigSiteTool::~ConfigSiteTool() {}

/**
 * Clear cin
 */
void ConfigSiteTool::clearCin(){
	cin.clear();
    cin.ignore(256,'\n');
}

/**
 * Print Menu
 */
void ConfigSiteTool::printMenu() {
	cout << "\n Options 1, 2, 3 and 6 also activate project on apache !!!\n\n";
	cout << "1 - Create new project" << endl;
	cout << "2 - Config existing project(Run composer install)" << endl;
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
int ConfigSiteTool::getOlyInteger(string errorMessage) {
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
string ConfigSiteTool::intToString(int integerToString) {
	// get Int to convert to string
	stringstream ssInt;
	ssInt << integerToString;
	return ssInt.str();
}

/**
 * Configura, activa e reinicia as novas configurações no apache
 */
void ConfigSiteTool::configApache(bool isChangePort) {
	string porto = intToString(this->porto);

	// Set Port
	this->setPort(porto, this->nomeProjecto);

	// Set Path
	this->setPath(this->pathWWW, this->nomeProjecto);

	// Desactiva o site
	if (isChangePort){
		this->disableSite(this->nomeProjecto);
	}

	// Activa o site sudo a2ensite nome_projecto.conf
	this->enableSite(this->nomeProjecto);
}

/**
 * Disable Project
 */
void ConfigSiteTool::disableProject(){
	string porto;

	// Remove Port
	porto = this->intToString(this->porto);
	this->unsetPort(porto);

	// Remove Virtual Conf File
	this->deleteVirtualConfFile(this->nomeProjecto);

	// Disable Site
	this->disableSite(this->nomeProjecto);
}

/**
 * Save info on file
 */
void ConfigSiteTool::saveInfo(string name, int porto, bool isDelete) {
	string operation = (isDelete) ? " delete " : " save ";
	string sPorto = intToString(porto);
	string cmdToExec = "bash " + APPFOLDER + "infoListProject.sh" + operation + name + " " + sPorto;
	system(cmdToExec.c_str());
}

/**
 * Get all necessary info for the project
 */
void ConfigSiteTool::getInfoProject(int typeInfo, bool isNewProject) {
	string errorPortoMessage = "\nInsert valid port: ";
	string printMessagePorto;
	string confirm, path;

	// clear cin
	this->clearCin();

	// Pede o path do projecto
	if ( (typeInfo == 0) | (typeInfo == -1) ) {
		cout << "\n\n### DEFAULT PATH: " + PATHWWW << endl << endl;
		cout << "Insert project path (PRESS ENTER FOR DEFAULT PATH): ";
		getline(cin, path);

		if(path.length() > 0 && path != "\n") {
			this->pathWWW = path;
		}
	}

	// Pede o nome do projecto
	if ( (typeInfo == 1) | (typeInfo == -1) ) {
		cout << "Insert project name: ";
		getline(cin, this->nomeProjecto);
	}

	// Pede o número do porto
	if ( (typeInfo == 2) | (typeInfo == -1) ) {
		printMessagePorto = (isNewProject) ? "Insert port: " : "Insert new port: ";
		cout << printMessagePorto;
		this->porto = this->getOlyInteger(errorPortoMessage);
		this->clearCin();

		// Pede o porto enquanto o portio indicado já esta a ser usado
		while(this->verifyPort(this->porto)) {
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
}

/**
 * Execute all necessary command. Call apache command here
 */
void ConfigSiteTool::executeAllNecessaryCommand(int option) {
	string cmdToExec;
	string oldPorto;

	// Copy tamplate
	this->copyVirtualConf(this->nomeProjecto);

	// Config Apache
	if (option == 3){
		this->configApache(true);
	} else {
		this->configApache(false);
	}

	switch (option) {
		case 1:
			// Create new Project
			this->newProject(this->pathWWW, this->nomeProjecto);
			break;
		case 2:
			// Config existing Project
			this->configProject(this->pathWWW, this->nomeProjecto);
			break;
		case 3:
			oldPorto = intToString(this->oldPorto);
			this->unsetPort(oldPorto);
			break;			
	}
}

/**
 * Execute Option selected
 */
void ConfigSiteTool::executeOptionSelected(int opcao) {
    // Consoante a opção do utilizador, a acção é realizada
	if ( (opcao == 1) | (opcao == 2) | (opcao == 3) ) {
		if (opcao == 3) {
			this->getInfoProject(-1, false);
		} else {
			this->getInfoProject(-1, true);
		}
	} else {
		switch (opcao) {
			case 4:
				this->getInfoProject(-1, true);
				this->disableProject();
				this->saveInfo(this->nomeProjecto, this->porto, true);
				break;
			case 5:
				// clear cin
				this->clearCin();

				this->getInfoProject(0, false);
				this->getInfoProject(1, false);

				// Update existing Project
				this->updateProject(this->pathWWW, this->nomeProjecto);
				break;
			case 6:
				this->getInfoProject(-1, true);
				break;
			case 7:
				string cmdToExec;
				cmdToExec = "./" + APPFOLDER + "infoListProject.sh" + " show " + INFOPROJECTAPACHE;
				AllCommands::executeCommands(cmdToExec);
				break;
		}
	}

    // Executa se a opcao não fôr sair
    if (opcao != 4 && opcao != 5 && opcao != 7 && opcao != 8) {
        this->executeAllNecessaryCommand(opcao);
       	this->saveInfo(this->nomeProjecto, this->porto, false);
    }
}