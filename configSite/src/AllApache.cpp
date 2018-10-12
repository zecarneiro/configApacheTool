/* Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
*/
#include "../lib/AllApache.h"

AllApache::AllApache() {
    this->enableSiteCmd = "a2ensite";
    this->disableSiteCmd = "a2dissite";
    this->restartApacheCmd = "service apache2 restart";
    this->reloadApacheCmd = "service apache2 reload";
    this->apachePath = "/etc/apache2/";
    this->apachePort = this->apachePath + "ports.conf";
    this->apacheSitesAvailable = this->apachePath + "sites-available/";
    this->extensionVirtualConf = ".conf";
    this->groupApache = "www-data";
}

AllApache::~AllApache() {}

/**
 * Set Permission
 */
void AllApache::setGroupApache(string fullPath) {
    // Set group to new project folder
    string command = " -R :" + this->groupApache + " " + fullPath;
    command = this->classAllCommands.getCommandChown(command);
    command = this->classAllCommands.getCommandSudo(command);

    // Execute command
    this->classAllCommands.executeCommands(command);
}

/**
 * Check if Port is defined on port.conf
 */
bool AllApache::checkPortsUsed(string port) {
	bool portIsUsed = false;
	string outputToCompared = "Listen " + port;
	string output, command;

	// Construct command
	command = " -w \"" + port + "\"";
    command = this->apachePort + " | " + this->classAllCommands.getCommandGrep(command);
    command = this->classAllCommands.getCommandCat(command);

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
 * Reload/Restart Apache
 */
void AllApache::reloadRestart(int option) {
    string command;
    switch (option) {
        case 0:
            command = this->classAllCommands.getCommandSudo(this->reloadApacheCmd);
            break;
        case 1:
            command = this->classAllCommands.getCommandSudo(this->restartApacheCmd);
            break;
        default:
            command = this->classAllCommands.getCommandSudo(this->reloadApacheCmd);
            command += " && " + this->classAllCommands.getCommandSudo(this->restartApacheCmd);
    }

    // Execute command
    this->classAllCommands.executeCommands(command);
}

/**
 * Set Port for apache
 */
void AllApache::setPort(string port, string nameProject) {
    string command = " -i 's/" + PORTOIDENTIFY + "/" + port + "/' ";

    // Get sed command
    command = this->classAllCommands.getCommandSed(command);

    // Get sudo command
    command = this->classAllCommands.getCommandSudo(command);

    /* Create command to insert port virtual conf file
     * sudo sed -i 's/PORTO/porto/' /etc/apache2/sites-available/nomeProjecto.conf
     */
    command += this->apacheSitesAvailable + nameProject + this->extensionVirtualConf;

    // if port is not defined, set the port
	if (!this->checkPortsUsed(port)) {
        // Construct command
        // Ports conf : sudo sed -i "/Listen 80/a\\Listen PORTOPROJECTO" ports.conf
        string commandPortConf = " -i \"/Listen 80/a\\\\Listen " + port + "\" ";
        commandPortConf = this->classAllCommands.getCommandSed(commandPortConf);
        commandPortConf = this->classAllCommands.getCommandSudo(commandPortConf);
        commandPortConf += this->apachePort;
        command += " && " + commandPortConf;
	}

    // Execute command to insert port on virtual conf file
    this->classAllCommands.executeCommands(command);
}

/**
 * Unset Port, Remove port from port.conf
 */
void AllApache::unsetPort(string port) {
    string command = "-i /Listen " + port + "/d" + " " + this->apachePort;
    command = this->classAllCommands.getCommandSed(command);
    command = this->classAllCommands.getCommandSudo(command);

    // Execute command
    this->classAllCommands.executeCommands(command);
}

/**
 * Set Path for apache
 */
void AllApache::setPath(string path, string nameProject) {
    string webrootFolder = "webroot/";
    string command = " -i \"s#" +  PATHIDENTIFY + "#" + path + "/" + nameProject;
    command += "/" + webrootFolder + "#\" ";
    
    // Virtual Conf Pasta do Projecto(Comando a executar)
    // sudo sed -i 's/FULL_PATH_PROJECT/nomeProjecto/' /etc/apache2/sites-available/nomeProjecto.conf
	command = this->classAllCommands.getCommandSed(command);
    command = this->classAllCommands.getCommandSudo(command);
    command += this->apacheSitesAvailable + nameProject + this->extensionVirtualConf;

    // Execute command to insert port on virtual conf file
    this->classAllCommands.executeCommands(command);
}

/**
 * Delete Virtual conf file
 */
void AllApache::deleteVirtualConfFile(string nameProject) {
    string command = this->apacheSitesAvailable + nameProject + this->extensionVirtualConf;
    command = this->classAllCommands.getCommandDel(command);
    command = this->classAllCommands.getCommandSudo(command);

    // Execute command
    this->classAllCommands.executeCommands(command);
}

/**
 * Enable Site
 */
void AllApache::enableSite(string nameProject) {
    string command = this->enableSiteCmd + " " + nameProject + this->extensionVirtualConf;
    command = this->classAllCommands.getCommandSudo(command);
    
    // Execute command
    this->classAllCommands.executeCommands(command);

    // Reload and Restart Apache
    this->reloadRestart(-1);
}

/**
 * Disable Site
 */
void AllApache::disableSite(string nameProject) {
    string command = this->disableSiteCmd + " " + nameProject + this->extensionVirtualConf;
    command = this->classAllCommands.getCommandSudo(command);
    
    // Execute command : sudo a2dissite nome_projecto.conf
    this->classAllCommands.executeCommands(command);

    // Reload and Restart Apache
    this->reloadRestart(-1);
}

void AllApache::copyVirtualConf(string nameProject) {
    string command = this->classAllCommands.getCommandCopy(APACHEVIRTUALCONF);
    command = this->classAllCommands.getCommandSudo(command);
    command += this->apacheSitesAvailable + nameProject + this->extensionVirtualConf;

    // Execute Command
    this->classAllCommands.executeCommands(command);
}

bool AllApache::checkVirtualConfExist(string nameProject) {
    string command, output;

    command = "ls " + this->apacheSitesAvailable;
    command += " | grep \"" + nameProject + this->extensionVirtualConf + "\"";

    // Execute command
    output = this->classAllCommands.executeCommandsWithOutput(command.c_str());

    return (output.length() > 0 && output != "\n") ? true : false; 
}