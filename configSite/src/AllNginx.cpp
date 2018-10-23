/* Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
*/
#include "../lib/AllNginx.h"

AllNginx::AllNginx() {
    this->enableSiteNginxCmd = "sudo ln -s /etc/nginx/sites-available/example.com /etc/nginx/sites-enabled/";
    this->restartNginxCmd = "systemctl restart nginx";

    this->pathNginx = "/etc/nginx/";
    this->sitesAvailablePathNginx = this->pathNginx + "sites-available/";
    this->virtualConfTemplateNginx = APPFOLDER + "virtualConfNginxTemplate";
    this->groupNginx = this->getGroupNginx();
    this->nameOnDBNginx = "Nginx";

    // Insert Default value on data base
    this->insertNginxInfoDB();

    // ps aux | egrep '([n|N][g|G]inx|[h|H]ttpd)' | awk '{ print $1}' | uniq | tail -1
}

AllNginx::~AllNginx() {}

/*********************************************************************************************
 * DATA BASE METHODS
 ********************************************************************************************/
/**
 * Insert default value on Data Base
 */
void AllNginx::insertNginxInfoDB() {
    string query = "INSERT INTO Server(name) "
        "SELECT '" + this->nameOnDBNginx + "' "
        "WHERE NOT EXISTS(SELECT 1 FROM Server WHERE name = '" + this->nameOnDBNginx + "');";
    
    // Save on Data Base
    this->classAllDataBase.execQuery(query.c_str());
}

/**
 * Get Project Info
 */
bool AllNginx::getInfoProjectDBNginx(string nameProject) {
    // string nameProject, string path, int port, int oldPort
    string query = 
        "SELECT ConfigSite.name, ConfigSite.port, ConfigSite.directory "
		"FROM ConfigSite "
        "INNER JOIN Server ON ConfigSite.server_id = Server.id "
		"WHERE ConfigSite.name = '" + nameProject + "' "
        "AND Server.name = '" + this->nameOnDBNginx + "';";
    
    // Execute Query
	this->resultDbNginx = this->classAllDataBase.execQueryReturnData(query.c_str());

    // Get Data
	if (this->resultDbNginx.empty()) {
		cout << "Not exist " + this->nameProjectNginx + " on data base" << endl;
		return false;
	} else {
		this->nameProjectNginx = this->resultDbNginx[0][0];
        this->portProjectNginx = this->resultDbNginx[0][1];
		this->pathProjectNginx = this->resultDbNginx[0][2];
		return true;
	}
}

/*********************************************************************************************
 * GET AND SET GROUP/PERMISSION
 ********************************************************************************************/
/**
 * Return Group of Nginx2
 */
string AllNginx::getGroupNginx() {
    string command = "ps aux | egrep '([n|N][g|G]inx|[h|H]ttpd)' | awk '{ print $1}' | uniq | tail -1";
    string group = this->classAllOperationGlobal.executeCommandsWithOutput(command.c_str());
    group.erase(remove(group.begin(), group.end(), '\n'), group.end());
    return group;
}

/**
 * Set Permission
 */
void AllNginx::setGroupNginx(string fullPath) {
    // Set group to new project folder
    string command = "-R :" + this->groupNginx + " " + fullPath;
    command = this->classAllOperationGlobal.getCommandChown(command);
    command = this->classAllOperationGlobal.getCommandSudo(command);

    // Execute command
    this->classAllOperationGlobal.executeCommands(command);

    // Set Permission
    this->setPermissionNginx(fullPath);
}

/**
 * Set Permission for group Nginx
 */
void AllNginx::setPermissionNginx(string fullPath) {
    // Set All Permission
    this->classAllOperationGlobal.setPermission(fullPath, "775");
}

/*********************************************************************************************
 * SET/UNSET AND CHECK PORT AND PATH
 ********************************************************************************************/
/**
 * Check if Port is defined on port.conf
 */
bool AllNginx::checkPortsUsedNginx(string port) {
    // Check by terminal
    // sudo lsof -i -n | grep nginx | awk '{print $9}' | cut -d ':' -f2 | grep -c port
    string command = "-c " + port;
    command = "-d ':' -f2 | " + this->classAllOperationGlobal.getCommandGrep(command);
    command = "nginx | awk '{print $9}' | " + this->classAllOperationGlobal.getCommandCat(command);
    command = "lsof -i -n | " + this->classAllOperationGlobal.getCommandGrep(command);
    command = this->classAllOperationGlobal.getCommandSudo(command);
    string exist = this->classAllOperationGlobal.executeCommandsWithOutput()

    // Check on Data Base
    string query = 
        "SELECT COUNT(*) "
        "FROM ConfigSite "
        "INNER JOIN Server ON ConfigSite.server_id = Server.id "
        "WHERE ConfigSite.port = " + port + ";";

    this->resultDbNginx = this->classAllDataBase.execQueryReturnData(query.c_str());
    
    if (this->resultDbNginx[0][0] == '1' && )
    
	bool portIsUsed = false;
	string outputToCompared = "Listen " + port;
	string output, command;

	// Construct command
	command = " -w \"" + port + "\"";
    command = this->portConfFileNginx + " | " + this->classAllOperationGlobal.getCommandGrep(command);
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

/**
 * Set Port for Nginx
 */
void AllNginx::setPortNginx(string port, string nameProject) {
    string command = "-i 's/" + PORTOIDENTIFY + "/" + port + "/' ";

    // Get sed command
    command = this->classAllOperationGlobal.getCommandSed(command);

    // Get sudo command
    command = this->classAllOperationGlobal.getCommandSudo(command);

    /* Create command to insert port virtual conf file
     * sudo sed -i 's/PORTO/porto/' /etc/Nginx2/sites-available/nomeProjecto.conf
     */
    command += this->sitesAvailablePathNginx + nameProject + this->extensionVirtualConfNginx;

    // if port is not defined, set the port
	if (!this->checkPortsUsedNginx(port)) {
        // Construct command
        // Ports conf : sudo sed -i "/Listen 80/a\\Listen PORTOPROJECTO" ports.conf
        string commandPortConf = "-i \"/Listen 80/a\\\\Listen " + port + "\" ";
        commandPortConf = this->classAllOperationGlobal.getCommandSed(commandPortConf);
        commandPortConf = this->classAllOperationGlobal.getCommandSudo(commandPortConf);
        commandPortConf += this->portConfFileNginx;
        command += " && " + commandPortConf;
	}

    // Execute command to insert port on virtual conf file
    this->classAllOperationGlobal.executeCommands(command);
}

/**
 * Unset Port, Remove port from port.conf
 */
void AllNginx::unsetPortNginx(string port) {
    string command = "-i '/Listen " + port + "/d'" + " " + this->portConfFileNginx;
    command = this->classAllOperationGlobal.getCommandSed(command);
    command = this->classAllOperationGlobal.getCommandSudo(command);

    // Execute command
    this->classAllOperationGlobal.executeCommands(command);
}

/**
 * Set Path for Nginx on virtual conf
 */
void AllNginx::setPathNginx(string path, string nameProject) {
    string webrootFolder = "webroot/";
    string command = "-i \"s#" +  PATHIDENTIFY + "#" + path + "/" + nameProject;
    command += "/" + webrootFolder + "#\" ";
    
    // Virtual Conf Pasta do Projecto(Comando a executar)
    // sudo sed -i 's/FULL_PATH_PROJECT/nomeProjecto/' /etc/Nginx2/sites-available/nomeProjecto.conf
	command = this->classAllOperationGlobal.getCommandSed(command);
    command = this->classAllOperationGlobal.getCommandSudo(command);
    command += this->sitesAvailablePathNginx + nameProject + this->extensionVirtualConfNginx;

    // Execute command to insert port on virtual conf file
    this->classAllOperationGlobal.executeCommands(command);
}

/*********************************************************************************************
 * COPY/DELETE AND CHECK VIRTUAL CONF
 ********************************************************************************************/
/**
 * Check if virtual conf exist
 */
bool AllNginx::checkVirtualConfExistNginx(string nameProject) {
    string command, output;

    command = "ls " + this->sitesAvailablePathNginx;
    command += " | grep \"" + nameProject + this->extensionVirtualConfNginx + "\"";

    // Execute command
    output = this->classAllOperationGlobal.executeCommandsWithOutput(command.c_str());

    return (output.length() > 0 && output != "\n") ? true : false; 
}

/**
 * Copy virtual conf for Nginx
 */
void AllNginx::copyVirtualConfNginx(string nameProject) {
    string command = this->classAllOperationGlobal.getCommandCopy(this->virtualConfTemplateNginx);
    command = this->classAllOperationGlobal.getCommandSudo(command);
    command += " " + this->sitesAvailablePathNginx + nameProject + this->extensionVirtualConfNginx;

    // Execute Command
    this->classAllOperationGlobal.executeCommands(command);
}

/**
 * Delete Virtual conf file
 */
void AllNginx::deleteVirtualConfFileNginx(string nameProject) {
    string command = this->sitesAvailablePathNginx + nameProject + this->extensionVirtualConfNginx;
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
void AllNginx::enableSiteNginx() {
    cout << "\n\nEnable Site...\n";

    // Copy virtual conf file
    this->copyVirtualConfNginx(this->nameProjectNginx);

	// Set Port
	this->setPortNginx(this->portProjectNginx, this->nameProjectNginx);

	// Set Path
	this->setPathNginx(this->pathProjectNginx, this->nameProjectNginx);

    // Set Group Nginx
    this->setGroupNginx(this->getFullPath());

    // Enable Site Command
    string command = this->enableSiteNginxCmd + " " + this->nameProjectNginx + this->extensionVirtualConfNginx;
    command = this->classAllOperationGlobal.getCommandSudo(command);
    
    // Execute command
    this->classAllOperationGlobal.executeCommands(command);

    // Reload and Restart Nginx
    this->reloadRestartNginx(-1);
}

/**
 * Disable Site
 */
void AllNginx::disableSiteNginx() {
    cout << "\n\nDisable Site...\n";

	// Remove Port
	this->unsetPortNginx(this->portProjectNginx);

	// Remove Virtual Conf File
	this->deleteVirtualConfFileNginx(this->nameProjectNginx);

    // Disable Site Command
    string command = this->disableSiteNginxCmd + " " + this->nameProjectNginx + this->extensionVirtualConfNginx;
    command = this->classAllOperationGlobal.getCommandSudo(command);
    
    // Execute command : sudo a2dissite nome_projecto.conf
    this->classAllOperationGlobal.executeCommands(command);

    // Reload and Restart Nginx
    this->reloadRestartNginx(-1);
}

/*********************************************************************************************
 * OTHER METHODS
 ********************************************************************************************/
string AllNginx::getFullPath() {
    return "\"" + this->pathProjectNginx + "/" + this->nameProjectNginx + "\"";
}

/*********************************************************************************************
 * CONFIG/RESTART/RELOAD Nginx
 ********************************************************************************************/
/**
 * Config Project on Nginx
 */
void AllNginx::configNginx(string operation, string nameProject, string oldPort) {
    if (this->getInfoProjectDBNginx(nameProject)) {
        if (operation.compare("enable") == 0) this->enableSiteNginx();
        else if (operation.compare("disable") == 0) this->disableSiteNginx();
        else if (operation.compare("changePort") == 0) {
            // Save new Port
            string newPort = this->portProjectNginx;

            // Disable Site
            this->portProjectNginx = oldPort;
            this->disableSiteNginx();

            // Restore new port and enable
            this->portProjectNginx = newPort;
            this->enableSiteNginx();
        }
    }
}

/**
 * Reload/Restart Nginx
 */
void AllNginx::reloadRestartNginx(int option) {
    string command;
    switch (option) {
        case 0:
            command = this->classAllOperationGlobal.getCommandSudo(this->reloadNginxCmd);
            break;
        case 1:
            command = this->classAllOperationGlobal.getCommandSudo(this->restartNginxCmd);
            break;
        default:
            command = this->classAllOperationGlobal.getCommandSudo(this->reloadNginxCmd);
            command += " && " + this->classAllOperationGlobal.getCommandSudo(this->restartNginxCmd);
    }

    // Execute command
    this->classAllOperationGlobal.executeCommands(command);
}