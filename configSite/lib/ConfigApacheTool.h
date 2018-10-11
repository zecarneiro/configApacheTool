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

/* Definition to all necessary commands */
class AllCommands {
    public:
        const string sudo = "sudo";
        const string copy = "cp";
        const string cd = "cd";
        const string mkdir = "mkdir";
        const string del = "rm";
        const string grep = "grep";
        const string mv = "mv";
        const string cat = "cat";
        const string sed = "sed";
        const string chown = "chown";
        const string chmod = "chmod";

        AllCommands(){}
        ~AllCommands(){}
};

/* Definition to all necessary apache config */
class AllApacheConfig : private AllCommands {
    public:
        const string enableSite = "a2ensite";
        const string disableSite = "a2dissite";
        const string restartApache = AllCommands::sudo + " service apache2 restart";
        const string reloadApache = AllCommands::sudo + " service apache2 reload";
        const string verifyPort = "/etc/apache2/";
        const string apachePath = "ports.conf";
        const string portsConf = this->apachePath + "sites-available/";
        const string sitesEnable = AllCommands::cat + " " + this->apachePath + this->portsConf + " | " + AllCommands::grep;;

        AllApacheConfig(){}
        ~AllApacheConfig(){}

        /**
         * Reload/Restart Apache
         */
        void reloadRestart(int option){
            switch (option) {
                case 0:
                    system(this->reloadApache.c_str());
                    break;
                case 1:
                    system(this->restartApache.c_str());
                    break;
                default:
                    system(this->reloadApache.c_str());
                    system(this->restartApache.c_str());
            }
        }
};

/* Definition to project */
class ConfigApacheTool {
    public:
        AllCommands commands;
        AllApacheConfig apacheConfig;
        const string APPFOLDER = "/opt/configSite/";
        const string APACHEVIRTUALCONF = this->APPFOLDER + "virtualConfApacheTemplate.conf";
        
        string copyTemplate;
        string nomeProjecto;
	    int porto;
	    int oldPorto;
	    string pathWWW;

        /* Methods */
        ConfigApacheTool();
        ~ConfigApacheTool();
        void clearCin();
        void printMenu();
        int getOlyInteger(string);
        string intToString(int);
        bool verifyPort(int);
        void getInfoProject(bool);
        void configApache(bool);
        void executeAllNecessaryCommand(int);
        void saveInfo(string, int, bool);
        void disableProject();
};