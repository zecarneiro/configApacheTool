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
    this->apacheVirtualConfTemplate = APPFOLDER + "virtualConfApacheTemplate.conf";
    this->extensionVirtualConf = ".conf";
    this->groupApache = this->getGroupApache();

    // Insert Default value on data base
    this->insertApacheInfoDB();

    // ps aux | egrep '([n|N][g|G]inx|[h|H]ttpd)' | awk '{ print $1}' | uniq | tail -1
}

AllApache::~AllApache() {}

/**
 * Insert default value on Data Base
 */
void AllApache::insertApacheInfoDB() {
    string name = "Apache2";
    string query = "INSERT INTO Server(name_server,virtual_conf) "
        "SELECT '" + name + "','" + this->apacheVirtualConfTemplate + "' "
        "WHERE NOT EXISTS(SELECT 1 FROM Server WHERE name_server = '" + name + "');";
    
    // Save on Data Base
    this->classAllDataBase.execQuery(query.c_str());
}

/**
 * Return Group of Apache2
 */
string AllApache::getGroupApache() {
    string command = "ps aux | egrep '([a|A]pache|[h|H]ttpd)' | awk '{ print $1}' | uniq | tail -1";
    string group = this->classAllOperationGlobal.executeCommandsWithOutput(command.c_str());
    group.erase(remove(group.begin(), group.end(), '\n'), group.end());
    return group;
}

/**
 * Set Permission
 */
void AllApache::setGroupApache(string fullPath) {
    // Set group to new project folder
    string command = "-R :" + this->groupApache + " " + fullPath;
    command = this->classAllOperationGlobal.getCommandChown(command);
    command = this->classAllOperationGlobal.getCommandSudo(command);

    // Execute command
    this->classAllOperationGlobal.executeCommands(command);

    // Set All Permission
    command = "chmod -R 775 " + fullPath;
    this->classAllOperationGlobal.executeCommands(command);
}

/**
 * Check if Port is defined on port.conf
 */
bool AllApache::checkPortsUsedApache(string port) {
	bool portIsUsed = false;
	string outputToCompared = "Listen " + port;
	string output, command;

	// Construct command
	command = " -w \"" + port + "\"";
    command = this->apachePort + " | " + this->classAllOperationGlobal.getCommandGrep(command);
    command = this->classAllOperationGlobal.getCommandCat(command);

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

bool AllApache::checkVirtualConfExistApache(string nameProject) {
    string command, output;

    command = "ls " + this->apacheSitesAvailable;
    command += " | grep \"" + nameProject + this->extensionVirtualConf + "\"";

    // Execute command
    output = this->classAllOperationGlobal.executeCommandsWithOutput(command.c_str());

    return (output.length() > 0 && output != "\n") ? true : false; 
}

/**
 * Reload/Restart Apache
 */
void AllApache::reloadRestartApache(int option) {
    string command;
    switch (option) {
        case 0:
            command = this->classAllOperationGlobal.getCommandSudo(this->reloadApacheCmd);
            break;
        case 1:
            command = this->classAllOperationGlobal.getCommandSudo(this->restartApacheCmd);
            break;
        default:
            command = this->classAllOperationGlobal.getCommandSudo(this->reloadApacheCmd);
            command += " && " + this->classAllOperationGlobal.getCommandSudo(this->restartApacheCmd);
    }

    // Execute command
    this->classAllOperationGlobal.executeCommands(command);
}

/**
 * Delete Virtual conf file
 */
void AllApache::deleteVirtualConfFileApache(string nameProject) {
    string command = this->apacheSitesAvailable + nameProject + this->extensionVirtualConf;
    command = this->classAllOperationGlobal.getCommandDel(command);
    command = this->classAllOperationGlobal.getCommandSudo(command);

    // Execute command
    this->classAllOperationGlobal.executeCommands(command);
}

/**
 * Unset Port, Remove port from port.conf
 */
void AllApache::unsetPortApache(string port) {
    string command = "-i '/Listen " + port + "/d'" + " " + this->apachePort;
    command = this->classAllOperationGlobal.getCommandSed(command);
    command = this->classAllOperationGlobal.getCommandSudo(command);

    // Execute command
    this->classAllOperationGlobal.executeCommands(command);
}

/**
 * Enable Site
 */
void AllApache::enableSiteApache(string nameProject) {
    string command = this->enableSiteCmd + " " + nameProject + this->extensionVirtualConf;
    command = this->classAllOperationGlobal.getCommandSudo(command);
    
    // Execute command
    this->classAllOperationGlobal.executeCommands(command);

    // Reload and Restart Apache
    this->reloadRestartApache(-1);
}

/**
 * Set Path for apache
 */
void AllApache::setPathApache(string path, string nameProject) {
    string webrootFolder = "webroot/";
    string command = "-i \"s#" +  PATHIDENTIFY + "#" + path + "/" + nameProject;
    command += "/" + webrootFolder + "#\" ";
    
    // Virtual Conf Pasta do Projecto(Comando a executar)
    // sudo sed -i 's/FULL_PATH_PROJECT/nomeProjecto/' /etc/apache2/sites-available/nomeProjecto.conf
	command = this->classAllOperationGlobal.getCommandSed(command);
    command = this->classAllOperationGlobal.getCommandSudo(command);
    command += this->apacheSitesAvailable + nameProject + this->extensionVirtualConf;

    // Execute command to insert port on virtual conf file
    this->classAllOperationGlobal.executeCommands(command);
}

/**
 * Set Port for apache
 */
void AllApache::setPortApache(string port, string nameProject) {
    string command = "-i 's/" + PORTOIDENTIFY + "/" + port + "/' ";

    // Get sed command
    command = this->classAllOperationGlobal.getCommandSed(command);

    // Get sudo command
    command = this->classAllOperationGlobal.getCommandSudo(command);

    /* Create command to insert port virtual conf file
     * sudo sed -i 's/PORTO/porto/' /etc/apache2/sites-available/nomeProjecto.conf
     */
    command += this->apacheSitesAvailable + nameProject + this->extensionVirtualConf;

    // if port is not defined, set the port
	if (!this->checkPortsUsedApache(port)) {
        // Construct command
        // Ports conf : sudo sed -i "/Listen 80/a\\Listen PORTOPROJECTO" ports.conf
        string commandPortConf = "-i \"/Listen 80/a\\\\Listen " + port + "\" ";
        commandPortConf = this->classAllOperationGlobal.getCommandSed(commandPortConf);
        commandPortConf = this->classAllOperationGlobal.getCommandSudo(commandPortConf);
        commandPortConf += this->apachePort;
        command += " && " + commandPortConf;
	}

    // Execute command to insert port on virtual conf file
    this->classAllOperationGlobal.executeCommands(command);
}

/**
 * Copy virtual conf for apache
 */
void AllApache::copyVirtualConfApache(string nameProject) {
    string command = this->classAllOperationGlobal.getCommandCopy(this->apacheVirtualConfTemplate);
    command = this->classAllOperationGlobal.getCommandSudo(command);
    command += " " + this->apacheSitesAvailable + nameProject + this->extensionVirtualConf;
    cout << command << endl;

    // Execute Command
    this->classAllOperationGlobal.executeCommands(command);
}

/**
 * Disable Site
 */
void AllApache::disableSiteApache(string nameProject, int port) {
    string portString;

	// Remove Port
	portString = this->classAllOperationGlobal.intToString(port);
	this->unsetPortApache(portString);

	// Remove Virtual Conf File
	this->deleteVirtualConfFileApache(nameProject);

    string command = this->disableSiteCmd + " " + nameProject + this->extensionVirtualConf;
    command = this->classAllOperationGlobal.getCommandSudo(command);
    
    // Execute command : sudo a2dissite nome_projecto.conf
    this->classAllOperationGlobal.executeCommands(command);

    // Reload and Restart Apache
    this->reloadRestartApache(-1);
}

/**
 * Config Project on Apache
 */
void AllApache::configApache(string nameProject, string path, int port, int oldPort, bool isChangePort) {
    string portoString, fullPath;

    // Disable Site and unset port
	if (isChangePort){
		this->disableSiteApache(nameProject, oldPort);
	}

    // Copy virtual conf file
    this->copyVirtualConfApache(nameProject);

	// Set Port
    portoString = this->classAllOperationGlobal.intToString(port);
	this->setPortApache(portoString, nameProject);

	// Set Path
	this->setPathApache(path, nameProject);

    // Set Group apache
    fullPath = "\"" + path + "/" + nameProject + "\"";
    this->setGroupApache(fullPath);

	// Activa o site sudo a2ensite nome_projecto.conf
	this->enableSiteApache(nameProject);
}