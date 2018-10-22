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
	// Create Default config folder
	this->createConfigFolderCmd = "mkdir -p " + CONFIGFOLDER;
	this->executeCommands(this->createConfigFolderCmd);
}

/**
 * Destructor
 */
ConfigSiteTool::~ConfigSiteTool() {}

void ConfigSiteTool::insertProjectCLI(int argc, char* argv[]) {
	bool isBack;
	this->nomeProjecto = argv[2];
	this->porto = this->stringToInt(argv[3]);

	if (argc < 5) {
		this->pathWWW = PATHWWW;
	} else {
		this->pathWWW = (strlen(argv[4]) == 0) ? PATHWWW : argv[4];
	}

	this->selectServer();
	cout << endl;
	this->selectFramework(isBack);

	// Save New Info
	this->saveDeleteInfo(false);

	cout << "Operation Finished\n";
}

/**
 * Save info on file
 */
void ConfigSiteTool::saveDeleteInfo(bool isDelete) {
	string query;
	if (isDelete) {
		query = "delete from ConfigSite where name_project = '" + this->nomeProjecto + "';";
	} else {
		query = "INSERT INTO ConfigSite(name_project,port,directory,framework_id,server_id) "
        	"VALUES ("
			"'" + this->nomeProjecto + "',"
			"'" + this->intToString(this->porto) + "',"
			"'" + this->pathWWW + "',"
			"'" + this->intToString(this->frameworkSelected) + "',"
			"'" + this->intToString(this->serverSelected) + "'"
			");"; 
	}
	this->execQuery(query.c_str());
}

/**
 * Configura, activa e reinicia as novas configurações no apache
 */
void ConfigSiteTool::configServer(bool isDisable, bool isChangePort) {
	switch (this->serverSelected) {
		case 1: // Apache
			if (isDisable) {
				this->disableSiteApache(this->nomeProjecto, this->oldPorto);
			} else {
				this->configApache(this->nomeProjecto, this->pathWWW, this->porto, this->oldPorto, isChangePort);
			}
			break;
	}
	
}

/**
 * Get info of project on DB
 */
bool ConfigSiteTool::getInfoProjectDB() {
	string query = "select port, directory "
		"from ConfigSite "
		"where ConfigSite.name_project = '" + this->nomeProjecto + "';";
	this->resultDbConfig = this->execQueryReturnData(query.c_str());

	if (this->resultDbConfig.empty()) {
		cout << "Not exist " + this->nomeProjecto + " on data base" << endl;
		return false;
	} else {
		this->oldPorto = this->stringToInt(this->resultDbConfig[0][0]);
		this->pathWWW = this->resultDbConfig[0][1];
		return true;
	}
}

/**
 * Execute all necessary command. Call apache command here
 */
void ConfigSiteTool::executeAllNecessaryCommand(int option) {
	switch (option) {
		case 1:
			// Create new Project
			this->newProjectCakePHP(this->pathWWW, this->nomeProjecto);
			this->configServer(false, false);
			break;
		case 2:
			// Config existing Project
			this->configProjectCakePHP(this->pathWWW, this->nomeProjecto);
			this->configServer(false, false);
			break;
		case 3:
			if (this->getInfoProjectDB()) {
				this->configServer(false, true);
			}
			break;
		case 4:
			if (this->getInfoProjectDB()) {
				this->configServer(true, false);
			}
			break;
		case 5:
			if (this->getInfoProjectDB()) {
				this->updateProjectCakePHP(this->pathWWW, this->nomeProjecto);
			}
			break;
		case 6:
			this->configServer(false, false);
			break;
		case 7:
			this->executeCommands("clear");
			this->printActiveProjectCakePHP();
			break;
	}
}

void ConfigSiteTool::setOperationDB(int opcao) {
	string query;
	switch(opcao) {
		case 3:
			query = "update ConfigSite "
			"set port = '" + this->intToString(this->porto) + "' "
			"where name_project = '" + this->nomeProjecto + "';";
			this->execQuery(query.c_str());
			break;
		case 4:
			this->saveDeleteInfo(true);
			break;
		default:
			if (opcao != 7 && opcao != 5) this->saveDeleteInfo(false);
	}
}

/**
 * Get Name for Project
 */
bool ConfigSiteTool::getNameProject() {
	// clear cin
	this->clearCin();

	this->nomeProjecto = "";
	cout << "Insert project name(PRESS ENTER TO CANCEL): ";
	getline(cin, this->nomeProjecto);
	return (this->nomeProjecto.length() == 0) ? true : false;
}

/**
 * Get Port for Project
 */
bool ConfigSiteTool::getPortProject(bool isChange) {
	string porto, printMessagePorto, errorPortoMessage, msgCancel;

	this->porto = -1;
	errorPortoMessage = "\nInsert valid port: ";
	msgCancel = "PRESS -1 TO CANCEL";
	printMessagePorto = (!isChange) ?
		"Insert port(" + msgCancel + "): " :
		"Insert new port(" + msgCancel + "): ";

	cout << printMessagePorto;
	this->porto = this->getOlyInteger(errorPortoMessage);
	this->clearCin();

	if (this->porto != -1) {
		// Pede o porto enquanto o portio indicado já esta a ser usado
		porto = this->intToString(this->porto);
		while(this->checkPortsUsedApache(porto) || this->porto <= 0) {
			if (this->porto <= 0) {
				cout << "Port is only positive." << endl;
			} else {
				cout << "Port inserted is used by other project." << endl;
			}

			cout << printMessagePorto;
			this->porto = this->getOlyInteger(errorPortoMessage);
			porto = this->intToString(this->porto);

			if (this->porto == -1) {
				break;
			}
		}
	}

	return (this->porto == -1) ? true : false;
}

/**
 * Get path for project
 */
void ConfigSiteTool::getPathProject() {
	// clear cin
	this->clearCin();
	
	this->pathWWW = "";
	cout << "\n\n### DEFAULT PATH: " + PATHWWW << endl << endl;
	cout << "Insert project path (PRESS ENTER FOR DEFAULT PATH): ";
	getline(cin, this->pathWWW);

	if (this->pathWWW.length() == 0) this->pathWWW = PATHWWW;
}

/**
 * Execute Option selected
 */
void ConfigSiteTool::executeOptionSelected(int opcao) {
	bool cancel = true;
	if (opcao == 1 || opcao == 2 || opcao == 6) {
		cancel = this->getNameProject();
		if (!cancel) {
			cancel = this->getPortProject(false);
			if (!cancel) {
				this->getPathProject();
			}
		}
	} else if (opcao == 3) {
		cancel = this->getNameProject();
		if (!cancel) {
			cancel = this->getPortProject(true);
		}
	} else if (opcao == 4 || opcao == 5) {
		cancel = this->getNameProject();
	} else if (opcao == 7) {
		cancel = false;
	}

    // Executa se a opcao não fôr sair
    if (!cancel) {
        this->executeAllNecessaryCommand(opcao);
		this->setOperationDB(opcao);
    }
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
	cout << "5 - Update existing project(Only Run Composer update)" << endl;
	cout << "6 - Activate project already configured(On Server)" << endl;
	cout << "7 - Show all active project" << endl;
	cout << "8 - Back HOME" << endl;
	cout << "9 - Exit" << endl;
	cout << "Insert an option: ";
}

/**
 * Select Framework
 */
void ConfigSiteTool::selectFramework(bool &isBack) {
	// Get list of server
	string query = "select id, name_framework from Framework;";
    this->resultDbConfig = this->execQueryReturnData(query.c_str());

	bool firstShow = true;
	int option;

	if (!this->resultDbConfig.empty()) {
		do{
			if (firstShow) {
				for(vector< vector<string> >::iterator row = this->resultDbConfig.begin(); row != this->resultDbConfig.end(); ++row) {
					int i = 0;
					for(vector<string>::iterator col = row->begin(); col != row->end(); ++col) {
						cout << *col;
						if ( i == 0) {
							cout << " - ";
							++i;
						}
					}
					cout << endl;
				}
				cout << "2 - Back HOME" << endl;
				cout << "3 - Exit" << endl;
				cout << "Insert an option: ";

				firstShow = false;
			}

			// Espera pela opção escolhida pelo utilizador
			option = this->getOlyInteger("\nInsert valid option: ");
			
			// Apresenta uma mensagem se o utilizador não introduzir uma opção correcta
			if(option < 1 || option > 3){
				cout << "\nInsert valid option only!!!"<< endl;
				cout<<"Insert an option: ";
			}
		}while (option < 1 || option > 3);
	} else {
		option = 2;
	}

	if (option == 2) isBack = true;
	else if (option == 3) exit(0);
	this->frameworkSelected = option;

	// Get name Framework
	query = "select name_framework from Framework where id = '" + this->intToString(option) + "';";
	this->resultDbConfig = this->execQueryReturnData(query.c_str());
	this->nameFrameworkSelected = this->resultDbConfig[0][0];
}

/**
 * Select Server
 */
void ConfigSiteTool::selectServer() {
	// Get list of server
	string query = "select id, name_server from Server;";
    this->resultDbConfig = this->execQueryReturnData(query.c_str());

	bool firstShow = true;
	int option;
	do{
		if (firstShow) {
			for(vector< vector<string> >::iterator row = this->resultDbConfig.begin(); row != this->resultDbConfig.end(); ++row) {
				int i = 0;
				for(vector<string>::iterator col = row->begin(); col != row->end(); ++col) {
					cout << *col;
					if ( i == 0) {
						cout << " - ";
						++i;
					}
				}
				cout << endl;
			}
			cout << "3 - Exit" << endl;
			cout << "Introduz a opcao: ";

			firstShow = false;
		}

		// Espera pela opção escolhida pelo utilizador
		option = this->getOlyInteger("\nInsert valid option: ");
		
		// Apresenta uma mensagem se o utilizador não introduzir uma opção correcta
		if(option < 1 || option > 3){
			cout << "\nInsert valid option only!!!"<<endl;
			cout<<"Insert an option: ";
		}
	}while (option < 1 || option > 3);

	if (option == 3) exit(0);
	this->serverSelected = option;

	// Get name Server
	query = "select name_server from Server where id = '" + this->intToString(option) + "';";
	this->resultDbConfig = this->execQueryReturnData(query.c_str());
	this->nameServerSelected = this->resultDbConfig[0][0];
}

/**
 * First Execution
 */
void ConfigSiteTool::initExecution() {
	// Definições das vairiáveis
	bool showMenu = true, firstRun=true, isBack = false;
	int  opcao = -1;

	while(1) {
		do{
			// Se o menu não foi imprimido, então faz o print do mesmo
	    	if (showMenu) {
				if (opcao == -1) {
					this->selectServer();
					cout << endl;
					this->selectFramework(isBack);
					this->executeCommands("clear");
				}
				if (!isBack) {
					if (!firstRun){
						cout << "\n\n\n### Operation finished! ###\n" << endl;
					}

					cout << "Server Selected: " << this->nameServerSelected << endl;
					cout << "Framweork Selected: " << this->nameFrameworkSelected << endl;
					this->printMenu();
				}			
	    		showMenu = false;
	    	}

			if (!isBack) {
				// Espera pela opção escolhida pelo utilizador
	      		opcao = this->getOlyInteger("\nInsert valid option: ");
			} else {
				opcao = 8;
				isBack = false;
			}
	      	
	      	// Apresenta uma mensagem se o utilizador não introduzir uma opção correcta
	      	if(opcao < 1 || opcao > 9){
				cout << "\nInsert valid option only!!!"<<endl;
	  			cout<<"Insert an option: ";
	      	}
		}while (opcao < 1 || opcao > 9);

		// Exit
		if (opcao == 9) {
			break;
		}

		if (opcao != 8) {
			// Execute option selected
        	this->executeOptionSelected(opcao);
			firstRun = false;
			opcao = 0;
		} else {
			this->executeCommands("clear");
			firstRun = true;
			opcao = -1;
		}

	  	// Change config for variable
	  	showMenu = true;
	}
}