/* Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
*/
#include "../lib/AllApache.h"

AllApache::AllApache() {
    this->enableSiteApacheCmd = "a2ensite";
    this->disableSiteApacheCmd = "a2dissite";
    this->restartApacheCmd = "service apache2 restart";
    this->reloadApacheCmd = "service apache2 reload";

    this->pathApache = "/etc/apache2/";
    this->portConfFileApache = this->pathApache + "ports.conf";
    this->sitesAvailablePathApache = this->pathApache + "sites-available/";
    this->virtualConfTemplateApache = APPFOLDER + "virtualConfApacheTemplate.conf";
    this->groupApache = this->getGroupApache();
    this->nameOnDBApache = "Apache2";

    this->extensionVirtualConfApache = ".conf";

    // Insert Default value on data base
    this->insertApacheInfoDB();
}

AllApache::~AllApache() {}

/*********************************************************************************************
 * DATA BASE METHODS
 ********************************************************************************************/
/**
 * Insert default value on Data Base
 */
void AllApache::insertApacheInfoDB() {
    string query = "INSERT INTO Server(name) "
        "SELECT '" + this->nameOnDBApache + "' "
        "WHERE NOT EXISTS(SELECT 1 FROM Server WHERE name = '" + this->nameOnDBApache + "');";
    
    // Save on Data Base
    this->classAllDataBase.execQuery(query.c_str());
}

/**
 * Get Project Info
 */
bool AllApache::getInfoProjectDBApache(string nameProject) {
    // string nameProject, string path, int port, int oldPort
    string query = 
        "SELECT ConfigSite.name, ConfigSite.port, ConfigSite.directory "
		"FROM ConfigSite "
        "INNER JOIN Server ON ConfigSite.server_id = Server.id "
		"WHERE ConfigSite.name = '" + nameProject + "' "
        "AND Server.name = '" + this->nameOnDBApache + "';";
    
    // Execute Query
	this->resultDbApache = this->classAllDataBase.execQueryReturnData(query.c_str());

    // Get Data
	if (this->resultDbApache.empty()) {
		cout << "Not exist " + this->nameProjectApache + " on data base" << endl;
		return false;
	} else {
		this->nameProjectApache = this->resultDbApache[0][0];
        this->portProjectApache = this->resultDbApache[0][1];
		this->pathProjectApache = this->resultDbApache[0][2];
		return true;
	}
}

/*********************************************************************************************
 * GET AND SET GROUP/PERMISSION
 ********************************************************************************************/
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
}

/**
 * Set Permission for group Apache
 */
void AllApache::setPermissionApache(string fullPath) {
    // Set All Permission
    this->classAllOperationGlobal.setPermission(fullPath, "775");
}

/*********************************************************************************************
 * SET/UNSET AND CHECK PORT AND PATH
 ********************************************************************************************/
/**
 * Check if Port is defined on port.conf
 */
bool AllApache::checkPortsUsedApache(string port, bool useDataBase) {
	bool portIsUsed = false;
	string outputToCompared = "Listen " + port;
	string output, command;

	// Construct command
	command = " -w \"" + port + "\"";
    command = this->portConfFileApache + " | " + this->classAllOperationGlobal.getCommandGrep(command);
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

    if (!portIsUsed && useDataBase) {
        // Check on Data Base
        string query = 
            "SELECT COUNT(*) "
            "FROM ConfigSite "
            "INNER JOIN Server ON ConfigSite.server_id = Server.id "
            "WHERE ConfigSite.port = " + port + ";";
        this->resultDbApache = this->classAllDataBase.execQueryReturnData(query.c_str());

        portIsUsed = (this->classAllOperationGlobal.stringToInt(this->resultDbApache[0][0]) > 0) ?
            true : false;
    }

	// Return true = Used or false
	return portIsUsed;
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
    command += this->sitesAvailablePathApache + nameProject + this->extensionVirtualConfApache;

    // if port is not defined, set the port
	if (!this->checkPortsUsedApache(port, false)) {
        // Construct command
        // Ports conf : sudo sed -i "/Listen 80/a\\Listen PORTOPROJECTO" ports.conf
        string commandPortConf = "-i \"/Listen 80/a\\\\Listen " + port + "\" ";
        commandPortConf = this->classAllOperationGlobal.getCommandSed(commandPortConf);
        commandPortConf = this->classAllOperationGlobal.getCommandSudo(commandPortConf);
        commandPortConf += this->portConfFileApache;
        command += " && " + commandPortConf;
	}

    // Execute command to insert port on virtual conf file
    this->classAllOperationGlobal.executeCommands(command);
}

/**
 * Unset Port, Remove port from port.conf
 */
void AllApache::unsetPortApache(string port) {
    string command = "-i '/Listen " + port + "/d'" + " " + this->portConfFileApache;
    command = this->classAllOperationGlobal.getCommandSed(command);
    command = this->classAllOperationGlobal.getCommandSudo(command);

    // Execute command
    this->classAllOperationGlobal.executeCommands(command);
}

/**
 * Set Path for apache on virtual conf
 */
void AllApache::setPathApache(string path, string nameProject) {
    string webrootFolder = "webroot/";
    string command = "-i \"s#" +  PATHIDENTIFY + "#" + path + "/" + nameProject;
    command += "/" + webrootFolder + "#\" ";
    
    // Virtual Conf Pasta do Projecto(Comando a executar)
    // sudo sed -i 's/FULL_PATH_PROJECT/nomeProjecto/' /etc/apache2/sites-available/nomeProjecto.conf
	command = this->classAllOperationGlobal.getCommandSed(command);
    command = this->classAllOperationGlobal.getCommandSudo(command);
    command += this->sitesAvailablePathApache + nameProject + this->extensionVirtualConfApache;

    // Execute command to insert port on virtual conf file
    this->classAllOperationGlobal.executeCommands(command);
}

/*********************************************************************************************
 * COPY/DELETE AND CHECK VIRTUAL CONF
 ********************************************************************************************/
/**
 * Check if virtual conf exist
 */
bool AllApache::checkVirtualConfExistApache(string nameProject) {
    string command, output;

    command = "ls " + this->sitesAvailablePathApache;
    command += " | grep \"" + nameProject + this->extensionVirtualConfApache + "\"";

    // Execute command
    output = this->classAllOperationGlobal.executeCommandsWithOutput(command.c_str());

    return (output.length() > 0 && output != "\n") ? true : false; 
}

/**
 * Copy virtual conf for apache
 */
void AllApache::copyVirtualConfApache(string nameProject) {
    string command = this->classAllOperationGlobal.getCommandCopy(this->virtualConfTemplateApache);
    command = this->classAllOperationGlobal.getCommandSudo(command);
    command += " " + this->sitesAvailablePathApache + nameProject + this->extensionVirtualConfApache;

    // Execute Command
    this->classAllOperationGlobal.executeCommands(command);
}

/**
 * Delete Virtual conf file
 */
void AllApache::deleteVirtualConfFileApache(string nameProject) {
    string command = this->sitesAvailablePathApache + nameProject + this->extensionVirtualConfApache;
    command = this->classAllOperationGlobal.getCommandDel(command);
    command = this->classAllOperationGlobal.getCommandSudo(command);

    // Execute command
    this->classAllOperationGlobal.executeCommands(command);
}

/*********************************************************************************************
 * ENABLE AND DISABLE SITE
 ********************************************************************************************/
/**
 * Enable Site
 */
void AllApache::enableSiteApache() {
    cout << "\n\nEnable Site...\n";

    // Copy virtual conf file
    this->copyVirtualConfApache(this->nameProjectApache);

	// Set Port
	this->setPortApache(this->portProjectApache, this->nameProjectApache);

	// Set Path
	this->setPathApache(this->pathProjectApache, this->nameProjectApache);

    // Set Group apache
    this->setGroupApache(this->getFullPath());

    // Enable Site Command
    string command = this->enableSiteApacheCmd + " " + this->nameProjectApache + this->extensionVirtualConfApache;
    command = this->classAllOperationGlobal.getCommandSudo(command);
    
    // Execute command
    this->classAllOperationGlobal.executeCommands(command);

    // Reload and Restart Apache
    this->reloadRestartApache(-1);
}

/**
 * Disable Site
 */
void AllApache::disableSiteApache() {
    cout << "\n\nDisable Site...\n";

	// Remove Port
	this->unsetPortApache(this->portProjectApache);

	// Remove Virtual Conf File
	this->deleteVirtualConfFileApache(this->nameProjectApache);

    // Disable Site Command
    string command = this->disableSiteApacheCmd + " " + this->nameProjectApache + this->extensionVirtualConfApache;
    command = this->classAllOperationGlobal.getCommandSudo(command);
    
    // Execute command : sudo a2dissite nome_projecto.conf
    this->classAllOperationGlobal.executeCommands(command);

    // Reload and Restart Apache
    this->reloadRestartApache(-1);
}

/*********************************************************************************************
 * OTHER METHODS
 ********************************************************************************************/
string AllApache::getFullPath() {
    return "\"" + this->pathProjectApache + "/" + this->nameProjectApache + "\"";
}

/*********************************************************************************************
 * CONFIG/RESTART/RELOAD APACHE
 ********************************************************************************************/
/**
 * Config Project on Apache
 */
void AllApache::configApache(string operation, string nameProject, string oldPort) {
    if (this->getInfoProjectDBApache(nameProject)) {
        if (operation.compare("enable") == 0) this->enableSiteApache();
        else if (operation.compare("disable") == 0) this->disableSiteApache();
        else if (operation.compare("changePort") == 0) {
            // Save new Port
            string newPort = this->portProjectApache;

            // Disable Site
            this->portProjectApache = oldPort;
            this->disableSiteApache();

            // Restore new port and enable
            this->portProjectApache = newPort;
            this->enableSiteApache();
        }
    }
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