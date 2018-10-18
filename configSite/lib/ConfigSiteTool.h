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
        int idServerSelected;
        int idFrameworkSelected;
        int idDirectorySelected;
        int portSelected;
        
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
        void printMenu(int);
        void getInfoProject(int, bool);
        void configApache(bool);
        void executeAllNecessaryCommand(int);
        void saveInfo(string, int, bool);
        void disableProject();
        void createNewProject();
        void executeOptionSelected(int);
};

#endif // CONFIGSITETOOL_H