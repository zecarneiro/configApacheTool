/* Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
*/
#include "../lib/AllCakePHP.h"

/**
 * Constructor
 */
AllCakePHP::AllCakePHP() {
    this->newProjectCmd = "composer create-project --prefer-dist cakephp/app";
    this->updateProjectCmd = "composer update -d";
    this->configProjectCmd = "composer install -d";

    this->nameOnDBCakePHP = "CakePHP";

    // Insert Default value on data base
    this->insertCakePHPInfoDB();
}

/**
 * Destructor
 */
AllCakePHP::~AllCakePHP() {}

/**
 * Insert default value on Data Base
 */
void AllCakePHP::insertCakePHPInfoDB() {
    string query = "INSERT INTO Framework(name) "
        "SELECT '" + this->nameOnDBCakePHP + "' "
        "WHERE NOT EXISTS(SELECT 1 FROM Framework WHERE name = '" + this->nameOnDBCakePHP + "');";
    
    // Save on Data Base
    this->classAllDataBase.execQuery(query.c_str());
}

/**
 * Set Permission on Logs and Tmp directory
 */
void AllCakePHP::setPermissionCakePHP(string path, string nameProject) {
    string fullPathLogs = "\"" + path + "/" + nameProject + "/logs" + "\"";
    string fullPathTmp = "\"" + path + "/" + nameProject + "/tmp" + "\"";
    string logsCmd = "sudo mkdir -p " + fullPathLogs;
    string tmpCmd = "sudo mkdir -p " + fullPathTmp;

    // Print Info
    cout << "\nSet Permission on Logs and Tmp directory...\n";

    // Create logs em tmp dir if not exist
    this->classAllOperationGlobal.executeCommands(logsCmd);
    this->classAllOperationGlobal.executeCommands(tmpCmd);

    // Set Permission
    this->classAllOperationGlobal.setPermission(fullPathLogs, "777");
    this->classAllOperationGlobal.setPermission(fullPathTmp, "777");
}

/**
 * Create new project
 */
void AllCakePHP::newProjectCakePHP(string path, string nameProject) {
    string fullPath = "\"" + path + "/" + nameProject + "\"";
    string command = this->newProjectCmd + " " + fullPath;

    // Print Info
    cout << "\nCreate project on CakePHP with name: " << nameProject << endl;

    // Execute
    this->classAllOperationGlobal.executeCommands(command);

    // Set Permission
    this->setPermissionCakePHP(path, nameProject);
}

/**
 * Config existing project
 */
void AllCakePHP::configProjectCakePHP(string path, string nameProject) {
    string fullPath = "\"" + path + "/" + nameProject + "\"";
    string command = this->configProjectCmd + " " + path;

    // Print Info
    cout << "\nConfig project on CakePHP with name: " << nameProject << endl;

    // Execute
    this->classAllOperationGlobal.executeCommands(command);

    // Set Permission
    this->setPermissionCakePHP(path, nameProject);
}

/**
 * Update existng project
 */
void AllCakePHP::updateProjectCakePHP(string path, string nameProject) {
    string fullPath = "\"" + path + "/" + nameProject + "\"";
    string command = this->updateProjectCmd + " " + fullPath;

    // Print Info
    cout << "\nUpdate project on CakePHP with name: " << nameProject << endl;

    // Execute
    this->classAllOperationGlobal.executeCommands(command);

    // Set Permission
    this->setPermissionCakePHP(path, nameProject);
}

/**
 * Print All Project
 */
void AllCakePHP::printActiveProjectCakePHP() {
    string query = "SELECT ConfigSite.name, ConfigSite.port, ConfigSite.directory, Server.name "
		"FROM ConfigSite "
        "INNER JOIN Server on ConfigSite.server_id = Server.id "
        "INNER JOIN Framework on ConfigSite.framework_id = Framework.id "
        "WHERE Framework.name = '" + this->nameOnDBCakePHP + "';";

    // Execute Query
	this->resultDbCakePHP = this->classAllDataBase.execQueryReturnData(query.c_str());

    cout << endl;

    if (this->resultDbCakePHP.empty()) {
        cout << "Empty List..." << endl;
    } else {
        int index = 1;
        cout << "\nList of project on CakePHP" << endl << endl;
        for(vector< vector<string> >::iterator row = this->resultDbCakePHP.begin(); row != this->resultDbCakePHP.end(); ++row) {
            int colIndex = 0;
            for(vector<string>::iterator col = row->begin(); col != row->end(); ++col) {
                switch (colIndex) {
                    case 0:
                        cout << "Name" + this->classAllOperationGlobal.intToString(index) + ": ";
                        break;
                    case 1:
                        cout << "Port" + this->classAllOperationGlobal.intToString(index) + ": ";
                        break;
                    case 2:
                        cout << "Directory" + this->classAllOperationGlobal.intToString(index) + ": ";
                        break;
                    case 3:
                        cout << "Server" + this->classAllOperationGlobal.intToString(index) + ": ";
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

/**
 * Set operation
 */
void AllCakePHP::setOperationCakePHP(string operation, string pathProject, string nameProject) {
    if (operation.compare("new") == 0) {
        this->newProjectCakePHP(pathProject, nameProject);
    } else if (operation.compare("config") == 0) {
        this->configProjectCakePHP(pathProject, nameProject);
    } else if (operation.compare("update") == 0) {
        this->updateProjectCakePHP(pathProject, nameProject);
    } else {
        this->printActiveProjectCakePHP();
    }
}