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
    string name = "CakePHP";
    string query = "INSERT INTO Framework(name_framework) "
        "SELECT '" + name + "' "
        "WHERE NOT EXISTS(SELECT 1 FROM Framework WHERE name_framework = '" + name + "');";
    
    // Save on Data Base
    this->classAllDataBase.execQuery(query.c_str());
}

void AllCakePHP::setPermissionCakePHP(string path, string nameProject) {
    string fullPathLogs = "\"" + path + "/" + nameProject + "/logs" + "\"";
    string fullPathTmp = "\"" + path + "/" + nameProject + "/tmp" + "\"";
    string logsCmd = "mkdir -p " + fullPathLogs;
    string tmpCmd = "mkdir -p " + fullPathTmp;

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

    // Execute
    this->classAllOperationGlobal.executeCommands(command);

    // Set Permission
    this->setPermissionCakePHP(path, nameProject);
}

void AllCakePHP::printActiveProjectCakePHP() {
    string query = "select ConfigSite.name_project, ConfigSite.port, ConfigSite.directory, Server.name_server "
		"from ConfigSite "
        "inner join Server on ConfigSite.server_id = Server.id "
        "inner join Framework on ConfigSite.framework_id = Framework.id "
        "where Framework.name_framework = 'CakePHP';";
	this->resultDbConfig = this->classAllDataBase.execQueryReturnData(query.c_str());

    cout << endl;

    if (this->resultDbConfig.empty()) {
        cout << "Empty List..." << endl;
    } else {
        cout << "\nList of project on CakePHP" << endl << endl;
        for(vector< vector<string> >::iterator row = this->resultDbConfig.begin(); row != this->resultDbConfig.end(); ++row) {
            int index = 1, colIndex = 0;
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