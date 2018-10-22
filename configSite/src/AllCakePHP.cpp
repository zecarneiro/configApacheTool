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

/**
 * Create new project
 */
void AllCakePHP::newProjectCakePHP(string path, string nameProject) {
    string fullPath = "\"" + path + "/" + nameProject + "\"";
    string command = this->newProjectCmd + " " + fullPath;

    // Execute
    this->classAllCommands.executeCommands(command);

    // Set Group apache
    this->classAllApache.setGroupApache(fullPath);

    // Set Permission
    this->classAllCommands.setPermission(fullPath);
}

/**
 * Config existing project
 */
void AllCakePHP::configProjectCakePHP(string path, string nameProject) {
    string fullPath = "\"" + path + "/" + nameProject + "\"";
    string command = this->configProjectCmd + " " + path;

    // Execute
    this->classAllCommands.executeCommands(command);

    // Set Permission
    this->classAllCommands.setPermission(fullPath);
}

/**
 * Update existng project
 */
void AllCakePHP::updateProjectCakePHP(string path, string nameProject) {
    string fullPath = "\"" + path + "/" + nameProject + "\"";
    string command = this->updateProjectCmd + " " + path;

    // Execute
    this->classAllCommands.executeCommands(command);

    // Set Permission
    this->classAllCommands.setPermission(fullPath);
}

void AllCakePHP::printActiveProjectCakePHP() {
    string query = "select ConfigSite.name_project, ConfigSite.port, Directory.directory, Server.name_server "
		"from ConfigSite "
		"inner join Directory on ConfigSite.directory_id = Directory.id "
        "inner join Server on ConfigSite.server_id = Server.id;";
	this->resultDbConfig = this->classAllDataBase.execQueryReturnData(query.c_str());

	for(vector< vector<string> >::iterator row = this->resultDbConfig.begin(); row != this->resultDbConfig.end(); ++row) {
        int i = 1;
        for(vector<string>::iterator col = row->begin(); col != row->end(); ++col) {
            switch (i) {
                case 1:
                    cout << "Name" + this->classAllCommands.intToString(i) + ": ";
                    break;
                case 2:
                    cout << "Port" + this->classAllCommands.intToString(i) + ": ";
                    break;
                case 3:
                    cout << "Directory" + this->classAllCommands.intToString(i) + ": ";
                    break;
                case 4:
                    cout << "Server" + this->classAllCommands.intToString(i) + ": ";
                    break;
            }
            cout << *col << endl;
            ++i;
        }
        cout << endl;
    }
}