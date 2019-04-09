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
	this->serverSelected = 0;
	this->frameworkSelected = 0;
	this->portSelected = 0;

	this->nameServerSelected = "";
	this->nameFrameworkSelected = "";

	this->onlyInsertedOnDataBase = false;
	this->checkName = true;
	
	this->nomeProjecto = "";
	this->oldPorto = "";
	this->porto = "";
	this->pathWWW = "";
}

/**
 * Destructor
 */
ConfigSiteTool::~ConfigSiteTool() {}

/*********************************************************************************************
 * DATA BASE METHODS
 ********************************************************************************************/
/**
 * Save info on Data Base
 */
void ConfigSiteTool::saveDeleteInfo(bool isDelete) {
	bool isNameEmpty = this->nomeProjecto.length() == 0;
	bool isPortEmpty = this->porto.length() == 0;
	bool isPathEmpty = this->pathWWW.length() == 0;
	bool setChange = false;

	string query;
	if (isDelete) {
		if (isNameEmpty) cout << "\nName of project can be empty...\n";
		else setChange = true;

		query = 
			"DELETE FROM ConfigSite "
			"WHERE name = '" + this->nomeProjecto + "' "
			"AND server_id = " + this->intToString(this->serverSelected) + ";";
	} else {
		if (!isNameEmpty && !isPortEmpty && !isPathEmpty) setChange = true;
		else cout << "\nName/Port/Path of project can be empty...\n";

		query = "INSERT INTO ConfigSite(name,port,directory,framework_id,server_id) "
        	"SELECT "
			"'" + this->nomeProjecto + "',"
			"'" + this->porto + "',"
			"'" + this->pathWWW + "',"
			"'" + this->intToString(this->frameworkSelected) + "',"
			"'" + this->intToString(this->serverSelected) + "' "
			"WHERE "
			"NOT EXISTS("
			"SELECT 1 FROM ConfigSite "
			"INNER JOIN Server ON ConfigSite.server_id = Server.id "
			"WHERE ConfigSite.name = '" + this->nomeProjecto + "' COLLATE NOCASE "
			"AND Server.name = '" + this->nameServerSelected + "');";
	}

	// Execute Query
	if (setChange) this->execQuery(query.c_str());
}

/**
 * Get info of project on DB
 */
bool ConfigSiteTool::getInfoProjectDB() {
	string query = "SELECT ConfigSite.port, ConfigSite.directory "
		"FROM ConfigSite "
		"INNER JOIN Server ON ConfigSite.server_id = Server.id "
		"INNER JOIN Framework ON ConfigSite.framework_id = Framework.id "
		"WHERE ConfigSite.name = '" + this->nomeProjecto + "' "
		"AND ConfigSite.server_id = " + this->intToString(this->serverSelected) + " "
		"AND ConfigSite.framework_id = " + this->intToString(this->frameworkSelected) + ";";
	
	// Execute Query
	this->resultDbConfig = this->execQueryReturnData(query.c_str());

	if (this->resultDbConfig.empty()) {
		cout << "Not exist " + this->nomeProjecto + " on " + this->nameServerSelected + " on data base" << endl;
		return false;
	} else {
		this->oldPorto = this->resultDbConfig[0][0];
		this->pathWWW = this->resultDbConfig[0][1];
		return true;
	}
}

/**
 * Set Operation on Data Base by user option selected
 */
void ConfigSiteTool::setOperationDB(int opcao) {
	string query;
	switch(opcao) {
		case 3:
			query = "UPDATE ConfigSite "
			"SET port = '" + this->porto + "' "
			"WHERE name = '" + this->nomeProjecto + "';";
			this->execQuery(query.c_str());
			break;
		case 4:
			this->saveDeleteInfo(true);
			break;
		default:
			if (opcao != 8 && opcao != 9 && opcao != 5) this->saveDeleteInfo(false);
	}
}

/**
 * Print All Project
 */
void ConfigSiteTool::printActiveProject() {
    string query = 
		"SELECT ConfigSite.name, ConfigSite.port, ConfigSite.directory, Server.name, Framework.name "
		"FROM ConfigSite "
        "JOIN Server on ConfigSite.server_id = Server.id "
        "JOIN Framework on ConfigSite.framework_id = Framework.id;";

    // Execute Query
	this->resultDbConfig = this->execQueryReturnData(query.c_str());

    cout << endl;

    if (this->resultDbConfig.empty()) {
        cout << "Empty List..." << endl;
    } else {
        cout << "\nList of project" << endl << endl;
        for(vector< vector<string> >::iterator row = this->resultDbConfig.begin(); row != this->resultDbConfig.end(); ++row) {
            int index = 1, colIndex = 0;
            for(vector<string>::iterator col = row->begin(); col != row->end(); ++col) {
                switch (colIndex) {
                    case 0:
                        cout << "Name" + this->intToString(index) + ": ";
                        break;
                    case 1:
                        cout << "Port" + this->intToString(index) + ": ";
                        break;
                    case 2:
                        cout << "Directory" + this->intToString(index) + ": ";
                        break;
                    case 3:
                        cout << "Server" + this->intToString(index) + ": ";
                        break;
					case 4:
                        cout << "Framework" + this->intToString(index) + ": ";
                        break;
                }
                cout << *col << endl;
                ++colIndex;
            }
            ++index;
            cout << endl;
        }
    }
}

/*********************************************************************************************
 * CONFIG SERVER AND FRAMEWORK
 ********************************************************************************************/
/**
 * Configura, activa e reinicia as novas configurações no apache
 */
void ConfigSiteTool::configServer(string operation) {
	switch (this->serverSelected) {
		case 1: // Apache
			this->configApache(operation, this->nomeProjecto, this->oldPorto);
			break;
		case 2: // NGinx
			this->configNginx(operation, this->nomeProjecto, this->oldPorto);
			break;
	}
}

/**
 * Config Framework
 */
void ConfigSiteTool::configFramework(string operation) {
	switch (this->frameworkSelected) {
		case 1: // CakePHP
			this->setOperationCakePHP(operation, this->pathWWW, this->nomeProjecto);
			break;
	}
}

/**
 * Check port by Framework
 */
bool ConfigSiteTool::checkPortByServer(string port) {
	bool isUsed;
	switch (this->serverSelected) {
		case 1: // Apache
			isUsed = this->checkPortsUsedApache(port, true);
			break;
		case 2: // NGinx
			isUsed = this->checkPortsUsedNginx(port, true);
			break;
	}
	return isUsed;
}

/*********************************************************************************************
 * PRINT MENU AND SELECT SERVER, FRAMEWORK AND OTHER OPERATION
 ********************************************************************************************/
/**
 * Select Server
 */
int ConfigSiteTool::selectServer() {
	int option;

	// Get list of server
	string query = "SELECT id, name FROM Server;";
    this->resultDbConfig = this->execQueryReturnData(query.c_str());

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
	cout << "Insert an option: ";

	do{
		option = this->getOlyInteger("Insert only integers: ");
		
		// Apresenta uma mensagem se o utilizador não introduzir uma opção correcta
		if(option < 1 || option > 3){
			cout << "\nInsert valid option only!!!"<<endl;
			cout << "Insert an option: ";
		}
	}while (option < 1 || option > 3);

	// Exit
	if (option == 3) option = _EXITCODE;
	else {
		cout << endl;
		this->serverSelected = option;

		// Get name Server
		query = "SELECT name FROM Server WHERE id = '" + this->intToString(option) + "';";
		this->resultDbConfig = this->execQueryReturnData(query.c_str());
		this->nameServerSelected = this->resultDbConfig[0][0];
	}

	// Return user option inserted
	return option;
}

/**
 * Select Framework
 */
int ConfigSiteTool::selectFramework() {
	int option;

	// Get list of server
	string query = "SELECT id, name FROM Framework;";
    this->resultDbConfig = this->execQueryReturnData(query.c_str());

	if (!this->resultDbConfig.empty()) {
		// Print Menu
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

		do{
			option = this->getOlyInteger("Insert only integers: ");		
			
			// Apresenta uma mensagem se o utilizador não introduzir uma opção correcta
			if(option < 1 || option > 3){
				cout << "\nInsert valid option only!!!"<< endl;
				cout << "Insert an option: ";
			}
		}while (option < 1 || option > 3);

		if (option == 3) option = _EXITCODE; // EXIT
		else if (option == 2) option = _RETURNCODE; // RETURN
		else {
			cout << endl;
			this->frameworkSelected = option;

			// Get name Framework
			query = "SELECT name FROM Framework WHERE id = '" + this->intToString(option) + "';";
			this->resultDbConfig = this->execQueryReturnData(query.c_str());
			this->nameFrameworkSelected = this->resultDbConfig[0][0];
		}
	} else {
		option = _RETURNCODE;
	}

	// Return user option inserted
	return option;
}

/**
 * Select Operation
 */
int ConfigSiteTool::selectOperation() {
	int option;

	// Print Server and Framework Selected
	cout << "Server Selected: " << this->nameServerSelected << endl;
	cout << "Framweork Selected: " << this->nameFrameworkSelected << endl;

	// Print Menu
	cout << "\n Options 1, 2, 3 and 6 also activate project on apache !!!\n\n";
	cout << "1 - Create new project on " << this->nameFrameworkSelected  << "/" << this->nameServerSelected << endl;
	cout << "2 - Config existing project(Run composer install) on " << this->nameFrameworkSelected  << "/" << this->nameServerSelected << endl;
	cout << "3 - Change port for an project(For activated project) on " << this->nameFrameworkSelected  << "/" << this->nameServerSelected << endl;
	cout << "4 - Disable an existing project(Not delete project folder) on " << this->nameFrameworkSelected  << "/" << this->nameServerSelected << endl;
	cout << "5 - Update existing project(Only Run Composer update) on " << this->nameFrameworkSelected  << "/" << this->nameServerSelected << endl;
	cout << "6 - Activate project already configured(On Server) for " << this->nameFrameworkSelected  << "/" << this->nameServerSelected << endl;
	cout << "7 - Insert already activate project(On Data Base) for " << this->nameFrameworkSelected  << "/" << this->nameServerSelected << endl;
	cout << "8 - Show all active project on " << this->nameFrameworkSelected << endl;
	cout << "9 - Show all active project"  << endl;
	cout << "10 - Back HOME" << endl;
	cout << "11 - Only Run Compose Install" << endl;
	cout << "12 - Exit" << endl;
	cout << "Insert an option: ";

	do{
		option = this->getOlyInteger("Insert only integers: ");

		// Invalid Inserted
		if(option < 1 || option > 12){
			cout << "\nInsert valid option only!!!"<<endl;
			cout << "Insert an option: ";
		}
	}while (option < 1 || option > 12);

	if (option == 12) option = _EXITCODE; // EXIT
	else if (option == 10) option = _RETURNCODE; // RETURN
	else if (option == 7) this->onlyInsertedOnDataBase = true;
	else if (option == 3 || option == 4 || option == 5) this->checkName = false;

	cout << endl;

	// Return user option selected
	return option;
}

/*********************************************************************************************
 * GET PROJECT INFO
 ********************************************************************************************/
/**
 * Get Name for Project
 */
bool ConfigSiteTool::getNameProject() {
	// clear cin
	this->clearCin();

	this->nomeProjecto = "";
	cout << "Insert project name(PRESS ENTER TO CANCEL): ";
	getline(cin, this->nomeProjecto);

	if (this->nomeProjecto.length() != 0 && this->checkName) {
		string query = 
			"SELECT COUNT(*) FROM ConfigSite "
			"INNER JOIN Server ON ConfigSite.server_id = Server.id "
			"WHERE ConfigSite.name = '" + this->nomeProjecto + "' COLLATE NOCASE "
			"AND Server.name = '" + this->nameServerSelected + "';";

		// Execute Query
		this->resultDbConfig = this->execQueryReturnData(query.c_str());

		while(this->stringToInt(this->resultDbConfig[0][0]) > 0) {
			cout << "Already Exist " + this->nomeProjecto + " using " + this->nameServerSelected << endl;

			this->clearCin();
			this->nomeProjecto = "";
			cout << "Insert project name(PRESS ENTER TO CANCEL): ";
			getline(cin, this->nomeProjecto);

			if (this->nomeProjecto.length() == 0) break;

			// Execute Query
			query = 
				"SELECT COUNT(*) FROM ConfigSite "
				"INNER JOIN Server ON ConfigSite.server_id = Server.id "
				"WHERE ConfigSite.name = '" + this->nomeProjecto + "' COLLATE NOCASE "
				"AND Server.name = '" + this->nameServerSelected + "';";
			this->resultDbConfig = this->execQueryReturnData(query.c_str());
		}
	}
	if (!this->checkName) this->checkName = true;
	return (this->nomeProjecto.length() == 0) ? true : false;
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
 * Get Port for Project
 */
bool ConfigSiteTool::getPortProject(bool isChange) {
	int porto;
	string printMessagePorto, errorPortoMessage, msgCancel;

	porto = -1;
	errorPortoMessage = "\nInsert valid port: ";
	msgCancel = "PRESS -1 TO CANCEL";
	printMessagePorto = (!isChange) ?
		"Insert port(" + msgCancel + "): " :
		"Insert new port(" + msgCancel + "): ";

	cout << printMessagePorto;
	porto = this->getOlyInteger(errorPortoMessage);

	if (porto != -1) {
		// Pede o porto enquanto o portio indicado já esta a ser usado
		this->porto = this->intToString(porto);
		bool portIsUsed;
		while(1) {
			portIsUsed = this->checkPortByServer(this->porto) && !this->onlyInsertedOnDataBase;
			if (!portIsUsed && porto > 0) break;
			
			// Print Messages
			if (porto <= 0) {
				cout << "Port is only positive." << endl;
			} else if (portIsUsed) {
				cout << "Port inserted is used by other project." << endl;
			}

			cout << printMessagePorto;
			porto = this->getOlyInteger(errorPortoMessage);
			this->porto = this->intToString(porto);

			if (porto == -1) {
				break;
			}
		}
	}

	return (porto == -1) ? true : false;
}

/**
 * Execute Option selected
 */
bool ConfigSiteTool::executeOptionSelected(int option) {
	bool isCancel = true;
	if (option == 1 || option == 2 || option == 6 || option == 7) {
		isCancel = this->getNameProject();
		if (!isCancel) {
			isCancel = this->getPortProject(false);
			if (!isCancel) {
				this->getPathProject();
			}
		}
	} else if (option == 3) {
		isCancel = this->getNameProject();
		if (!isCancel) {
			isCancel = this->getPortProject(true);
		}
	} else if (option == 4 || option == 5) {
		isCancel = this->getNameProject();
	} else if (option == 8 || option == 9) {
		isCancel = false;
	} else if (option == 11) {
		isCancel = this->getNameProject();
		if (!isCancel) {
			this->getPathProject();
		}
	}
    return isCancel;
}

/*********************************************************************************************
 * OTHER
 ********************************************************************************************/
/**
 * Execute all necessary command. Call apache command here
 */
void ConfigSiteTool::executeAllNecessaryCommand(int option) {
	switch (option) {
		case 1: // Create new Project
			this->setOperationDB(option);
			this->configFramework("new");
			this->configServer("enable");
			break;
		case 2: // Config existing Project
			this->setOperationDB(option);
			this->configFramework("config");
			this->configServer("enable");
			break;
		case 3: // Change port for an project
			if (this->getInfoProjectDB()) {
				this->setOperationDB(option);
				this->configServer("changePort");
			}
			break;
		case 4: // Disable an existing project
			if (this->getInfoProjectDB()) {
				this->configServer("disable");
				this->setOperationDB(option);
			}
			break;
		case 5: // Update existing project
			if (this->getInfoProjectDB()) {
				this->configFramework("update");
			}
			break;
		case 6: // Activate project already configured
			this->setOperationDB(option);
			this->configServer("enable");
			break;
		case 7: // Insert already activate project(On Data Base)
			this->setOperationDB(option);
			this->onlyInsertedOnDataBase = false;
			break;
		case 8: // Show all active project by framework and server
			this->executeCommands("clear && clear");
			this->configFramework("");
			break;
		case 9: // Show all active project
			this->executeCommands("clear && clear");
			this->printActiveProject();
			break;
		case 11: // Only Run Compose Install
			this->configFramework("config");
			break;
	}
}

/**
 * First Execution
 */
void ConfigSiteTool::initExecution() {
	// Definições das vairiáveis
	bool firstRun=true;
	int  option;

	while(1) {
		if (firstRun) {
			// Select Server
			option = this->selectServer();
			if (option == _EXITCODE) break;

			// Select Framework
			option = this->selectFramework();
			if (option == _EXITCODE) break;
			else if (option == _RETURNCODE) {
				firstRun = true;
				this->executeCommands("clear && clear");
				continue;
			}
			this->executeCommands("clear && clear");
		} else {
			cout << "\n\n#### OPERATION FINISHED ####\n\n";
		}

		// Select Operation
		option = this->selectOperation();
		if (option == _EXITCODE) break;
		else if (option == _RETURNCODE) {
			firstRun = true;
			this->executeCommands("clear && clear");
			continue;
		} else {
			// Execute option selected
			if (!this->executeOptionSelected(option)) {
				this->executeAllNecessaryCommand(option);
			}

			// Set is not first run
			firstRun = false;
		}
	}
}

/*********************************************************************************************
 * CLI
 ********************************************************************************************/

/**
 * HELP
 */
void ConfigSiteTool::helpCLI(string executable) {
	cout << "#### HELP ####" << endl;
	cout << executable << " (-e|-iP) ARGS\n" << endl;
	cout << "\t -e: Execute the app\n";
	cout << "\t -iP: Insert existing and active project\n";
	cout << "\t\t ARGS: \"Name\" Port \"Path\"\n";
}

/**
 * Insert Project
 */
void ConfigSiteTool::insertProjectCLI(int argc, string *argv) {
	this->nomeProjecto = argv[2];
	this->porto = this->stringToInt(argv[3]);

	if (argc < 5) {
		this->pathWWW = PATHWWW;
	} else {
		this->pathWWW = (argv[4].length() == 0) ? PATHWWW : argv[4];
	}

	// Select Server and Framework
	int option;
	while(1) {
		// Select Server
		option = this->selectServer();
		if (option == _EXITCODE) break;

		// Select Framework
		option = this->selectFramework();
		if (option == _EXITCODE) break;
		else if (option == _RETURNCODE) {
			this->executeCommands("clear");
			continue;
		} else {
			break;
		}
	}

	// Save New Info
	this->saveDeleteInfo(false);

	cout << "Operation Finished\n";
}

/**
 * Execute based on input
 */
void ConfigSiteTool::readInputAndSelectCLI(int argc, string *argv) {
	string firstArg[] = {"-e", "-iP", "-h"};

	if (argc > 1) {
		if (firstArg[0].compare(argv[1]) == 0) {
			this->initExecution();
		} else if (firstArg[1].compare(argv[1]) == 0) {
			if (argc >= 4) {
				this->insertProjectCLI(argc, argv);
			} else {
				cout << "\nMinimum Argument acepted: 2\n\n";
			}
		} else if (firstArg[2].compare(argv[1]) == 0) {
			this->helpCLI(argv[0]);
		} else {
			cout << "\nInvalid Argument\n\n";
		}
	} else {
		cout << "\nInvalid Argument\n\n";
	}
}