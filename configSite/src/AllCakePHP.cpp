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
}

/**
 * Destructor
 */
AllCakePHP::~AllCakePHP() {}

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