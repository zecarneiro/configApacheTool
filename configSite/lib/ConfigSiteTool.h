/* Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
*/
#ifndef CONFIGSITETOOL_H
#define CONFIGSITETOOL_H

#include "../lib/includes.h"
#include "../lib/AllOperationGlobal.h"
#include "../lib/AllApache.h"
#include "../lib/AllCakePHP.h"
#include "../lib/AllDataBase.h"

/* Definition to project */
class ConfigSiteTool: public AllDataBase, public AllOperationGlobal, private AllApache, public AllCakePHP {
    private:
        string createConfigFolderCmd;
        int serverSelected;
        int frameworkSelected;
        int portSelected;
        vector<vector<string>> resultDbConfig;
        
    public:
        string nomeProjecto;
	    int porto;
	    int oldPorto;
	    string pathWWW;

        /* Methods */
        ConfigSiteTool();
        ~ConfigSiteTool();

        void initExecution();
        void selectServer();
        void selectFramework(bool &);
        void printMenu();
        void executeOptionSelected(int);
        void getPathProject();
        bool getPortProject(bool);
        bool getNameProject();
        void executeAllNecessaryCommand(int);
        bool getInfoProjectDB();
        void configServer(bool, bool);
        void saveDeleteInfo(bool);

        void createNewProject();
        void printSelectedServer();
};

#endif // CONFIGSITETOOL_H