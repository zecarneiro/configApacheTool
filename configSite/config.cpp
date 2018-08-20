/* Create By José M. C. Noronha
 * Lisbon 10 May 2018
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
*/
#include <iostream>
#include <cstdlib>	//Para system()
#include <stdlib.h>	//Para system()
#include <string>
#include <sstream>	//Para converter inteiros em strings
#include <cstdio>	//Para popen()
using namespace std;

// Define o nome do template
string APPFOLDER = "/opt/configSite/";
string APACHEVIRTUALCONF = APPFOLDER + "virtualConfApacheTemplate.conf";

// Info to project
struct projectDef {
	string nomeProjecto;
	int porto;
	int oldPorto;
	string pathWWW;
};

// All necessary path
struct allPath{
	string apache;
	string sitesEnable;
}path;

// All necessary files
struct allFiles{
	string portConfApache;
}files;

// All necessary command
struct allCommand{
	string sudo;
	string copy;
	string cd;
	string mkdir;
	string del;
	string grep;
	string mv;
	string cat;
	string sed;
	string chown;
	string chmod;
	string enableSite;
	string disableSite;
	string restartApache;
	string reloadApache;
	string verifyPort;
	string copyTemplate;
}cmd;

/**
 * Clear cin
 */
void clearCin() {
	cin.clear();
    cin.ignore(256,'\n');
}

/**
 * Set default value all struct
 */
void initialise(projectDef &project) {
	project.nomeProjecto = "novoProjectoCake";
	project.porto = 9000;
	project.oldPorto = -1;
	project.pathWWW  = "PATH_WWW";

	// Path
	path.apache = "/etc/apache2/";
	path.sitesEnable = path.apache + "sites-available/";

	// Files
	files.portConfApache = "ports.conf";

	// Command
	cmd.sudo = "sudo";
	cmd.copy = "cp";
	cmd.cd = "cd";
	cmd.mkdir = "mkdir";
	cmd.del = "rm";
	cmd.grep = "grep";
	cmd.mv = "mv";
	cmd.cat = "cat";
	cmd.sed = "sed";
	cmd.chown = "chown";
	cmd.chmod = "chmod";
	cmd.enableSite = "a2ensite";
	cmd.disableSite = "a2dissite";
	cmd.restartApache = "sudo service apache2 restart";
	cmd.reloadApache = "sudo service apache2 reload";
	cmd.verifyPort = cmd.cat + " " + path.apache + files.portConfApache + " | " + cmd.grep;
	cmd.copyTemplate = cmd.sudo + " " + cmd.copy + " " + APACHEVIRTUALCONF;
}

/**
 * Print Menu
 */
void printMenu() {
	cout << "\n Options 1, 2, 3 and 6 also activate project on apache !!!\n\n";
	cout << "1 - Create new project CakePHP" << endl;
	cout << "2 - Config existing CakePHP project(Run composer install)" << endl;
 	cout << "3 - Change port for an project(For activated project)" << endl;
 	cout << "4 - Disable an existing project(Not delete project folder)" << endl;
 	cout << "5 - Update existing project" << endl;
 	cout << "6 - Only activate project(On apache)" << endl;
	cout << "7 - Show all active project" << endl;
	cout << "8 - Exit" << endl;
	cout << "Introduz a opcao: ";
}

/**
 * Get only number from cin
 */
int getOlyInteger(string errorMessage) {
	int integer;
	cin >> integer;

	// ask when user not introduce integer
    while(cin.fail()) {
        cout << errorMessage;
        clearCin();
        cin >> integer;
    }
    return integer;
}

/**
 * Convert intinger to string
 */
string intToString(int integerToString) {
	// get Int to convert to string
	stringstream ssInt;
	ssInt << integerToString;
	return ssInt.str();
}

/**
 * Verify if port introduced by user is defined o port.conf
 */
bool verifyPort(int port) {
	// Define Variable
	string portConverted = intToString(port);
	bool portIsUsed = false;
	string outputToCompared = "Listen " + portConverted;
	string output;

	// Construct command
	string command = cmd.verifyPort + " -w \"" + portConverted + "\"";

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
 * Get all necessary info for the project
 */
void getInfoProject(projectDef &info, bool isNewProject) {
	string errorPortoMessage = "\nInsert valid port: ";
	string printMessagePorto;
	string confirm;

	// clear cin
	clearCin();

    /**
     * Get info
     */
    // Pede o nome do projecto
	cout << "Insert project name: ";
	getline(cin, info.nomeProjecto);

	// Pede o número do porto
	printMessagePorto = (isNewProject) ? "Insert port: " : "Insert new port: ";
	cout << printMessagePorto;
	info.porto = getOlyInteger(errorPortoMessage);
	clearCin();

	// Pede o porto enquanto o portio indicado já esta a ser usado
	while(verifyPort(info.porto)) {
		cout << "Port inserted is used by other project." << endl;
		cout << "Continue? (S/N): ";
		getline(cin, confirm);

		if (confirm == "S" || confirm == "s"){
			break;
		} else if (confirm == "N" || confirm == "n") {
			cout << printMessagePorto;
			info.porto = getOlyInteger(errorPortoMessage);
		}
		clearCin();
	}
}

/**
 * Configura, activa e reinicia as novas configurações no apache
 */
void configApache(projectDef allinfo, bool isChangePort) {
	string cmdToExec;
	string porto = intToString(allinfo.porto);
	string pathProjecto = allinfo.pathWWW + "/" + allinfo.nomeProjecto;
	string webrootFolder = "webroot/";

	// Virtual Conf Porto(Comando a executar) : sudo sed -i 's/PORTO/porto/' /etc/apache2/sites-available/nomeProjecto.conf
	cmdToExec = cmd.sudo + " " + cmd.sed + " -i " + "'s/PORTO/" + porto + "/' ";
	cmdToExec += path.sitesEnable + allinfo.nomeProjecto + ".conf";
	system(cmdToExec.c_str());

	// Virtual Conf Pasta do Projecto(Comando a executar) : sudo sed -i 's/FULL_PATH_PROJECT/nomeProjecto/' /etc/apache2/sites-available/nomeProjecto.conf
	cmdToExec = cmd.sudo + " " + cmd.sed + " -i " + "\"s#FULL_PATH_PROJECT#" + pathProjecto + "/" + webrootFolder + "#\" ";
	cmdToExec += path.sitesEnable + allinfo.nomeProjecto + ".conf";
	system(cmdToExec.c_str());

	// Se o porto ainda não não foi definido no ficheiro ports.conf então é inserido este porto
	if (!verifyPort(allinfo.porto)) {
		// Ports conf : sudo sed -i "/Listen 80/a\\Listen PORTOPROJECTO" ports.conf
		cmdToExec = cmd.sudo + " " + cmd.sed + " -i " + "\"/Listen 80/a\\\\Listen " + porto + "\" ";
		cmdToExec += path.apache + files.portConfApache;
		system(cmdToExec.c_str());
	}

	// Desactiva o site sudo a2dissite nome_projecto.conf
	if (isChangePort){
		cmdToExec = cmd.sudo + " " + cmd.disableSite + " " + allinfo.nomeProjecto + ".conf";
		system(cmdToExec.c_str());

		// Reload Apache
		system(cmd.reloadApache.c_str());

		// Restart Apache
		system(cmd.restartApache.c_str());
	}

	// Activa o site sudo a2ensite nome_projecto.conf
	cmdToExec = cmd.sudo + " " + cmd.enableSite + " " + allinfo.nomeProjecto + ".conf";
	system(cmdToExec.c_str());

	// Reload Apache
	system(cmd.reloadApache.c_str());

	// Restart Apache
	system(cmd.restartApache.c_str());
}

/**
 * Execute all necessary command. Call apache command here
 */
void executeAllNecessaryCommand(projectDef allinfo, int option) {
	string cmdToExec;
	string oldPorto;
	string groupApache = "www-data";

	// Copy tamplate
	cmd.copyTemplate += " " + path.sitesEnable + allinfo.nomeProjecto + ".conf";
	system(cmd.copyTemplate.c_str());

	// Config Apache
	if (option == 3){
		configApache(allinfo, true);
	} else {
		configApache(allinfo, false);
	}

	switch (option) {
		case 1:
			// Construct command
			cmdToExec = "composer create-project --prefer-dist cakephp/app";
			cmdToExec += " " + allinfo.pathWWW + "/" + allinfo.nomeProjecto;
			system(cmdToExec.c_str());
			break;
		case 2:
			// Contruct command
			cmdToExec = "composer install -d " + allinfo.pathWWW + "/" + allinfo.nomeProjecto;
			system(cmdToExec.c_str());
			break;
		case 3:
			oldPorto = intToString(allinfo.oldPorto);
			// Ports conf : sed -i '/Listen OLD_PORTO/d' ports.conf
			cmdToExec = cmd.sudo + " " + cmd.sed + " -i " + "\"/Listen " + oldPorto + "/d\" ";
			cmdToExec += path.apache + files.portConfApache;
			system(cmdToExec.c_str());
			break;			
	}

	if(option != 3) {
		// Set group to new project folder
		cmdToExec = cmd.sudo + " " + cmd.chown + " -R :" + groupApache + " " + allinfo.pathWWW + "/" + allinfo.nomeProjecto;
		system(cmdToExec.c_str());

		// Set permission on logs folder
		cmdToExec = cmd.sudo + " " + cmd.chmod + " -R 777 " + allinfo.pathWWW + "/" + allinfo.nomeProjecto;
		system(cmdToExec.c_str());
	}
}

/**
 * Save info on file
 */
void saveInfo(string name, int porto, bool isDelete) {
	string operation = (isDelete) ? " delete " : " save ";
	string sPorto = intToString(porto);
	string cmdToExec = "bash " + APPFOLDER + "infoListProject.sh" + operation + name + " " + sPorto;
	system(cmdToExec.c_str());
}

/**
 * Main
 */
int main(){
	// Definições das vairiáveis
	projectDef project;
	bool showMenu = true, firstRun=true;
	int  opcao;
	string porto, cmdToExec;

	while(1) {

		// initialise struct
		initialise(project);

		if (!firstRun){
			cout << "\n\n\n### Operation finished! ###\n" << endl;
		}

		do{
			// Se o menu não foi imprimido, então faz o print do mesmo
	    	if (showMenu) {
	    		printMenu();
	    		showMenu = false;
	    	}
	    	// Espera pela opção escolhida pelo utilizador
	      	opcao = getOlyInteger("\nInsert valid option: ");
	      	
	      	// Apresenta uma mensagem se o utilizador não introduzir uma opção correcta
	      	if(opcao < 1 || opcao > 8){
				cout << "\nInsert valid option only!!!"<<endl;
	  			cout<<"Insert an option: ";
	      	}
		}while (opcao < 1 || opcao > 8);

		// Exit
		if (opcao == 8) {
			break;
		}

		// Consoante a opção do utilizador, a acção é realizada
	  	switch (opcao) {
		    case 1:
		    	getInfoProject(project, true);
				break;
		    case 2:
		    	getInfoProject(project, true);
				break;
		    case 3:
		    	getInfoProject(project, false);

		    	// Pede o número do antigo porto
		    	cout << "Insert old port: ";
				project.oldPorto = getOlyInteger("\nInsert valid port: ");
				break;
			case 4:
		    	getInfoProject(project, true);
		    	porto = intToString(project.porto);
		    	cmdToExec = "sh " + APPFOLDER + "/disableProject.sh " + porto + " " + project.nomeProjecto;
		    	system(cmdToExec.c_str());
		    	saveInfo(project.nomeProjecto, project.porto, true);
				break;
			case 5:
				// clear cin
				clearCin();

				// Pede o nome do projecto
				cout << "Insert project name: ";
				getline(cin, project.nomeProjecto);

				// Contruct command
				cmdToExec = "composer update -d " + project.pathWWW + "/" + project.nomeProjecto;
				system(cmdToExec.c_str());

				// Set permission on logs folder
				cmdToExec = cmd.sudo + " " + cmd.chmod + " -R 777 " + project.pathWWW + "/" + project.nomeProjecto;
				system(cmdToExec.c_str());
				break;
			case 6:
				getInfoProject(project, true);
				break;
			case 7:
				cmdToExec = "bash " + APPFOLDER + "infoListProject.sh" + " show";
				system(cmdToExec.c_str());
				break;
	  	}

	  	// Executa se a opcao não fôr sair
	  	if (opcao != 4 && opcao != 5 && opcao != 7 && opcao != 8) {
  			executeAllNecessaryCommand(project, opcao);
  			saveInfo(project.nomeProjecto, project.porto, false);
	  	}

	  	// Change config for variable
	  	showMenu = true;
	  	opcao = -1;
	  	firstRun = false;
	}
  	
  	// exit
  	return 0;
}
