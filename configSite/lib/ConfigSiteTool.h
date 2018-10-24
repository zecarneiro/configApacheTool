/* Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
*/
#ifndef CONFIGSITETOOL_H
#define CONFIGSITETOOL_H

#include "../lib/includes.h"
#include "../lib/AllOperationGlobal.h"
#include "../lib/AllDataBase.h"
#include "../lib/AllApache.h"
#include "../lib/AllNginx.h"
#include "../lib/AllCakePHP.h"

/**
 * EXIT AND RETURN CODE
 * EXIT = -2
 * RETURN = -1
 */
#define _EXITCODE -2
#define _RETURNCODE -1

/* Definition to project */
class ConfigSiteTool: public AllOperationGlobal, public AllDataBase, private AllApache, private AllNginx, public AllCakePHP {
    protected:
        int serverSelected, frameworkSelected, portSelected;
        string nameServerSelected, nameFrameworkSelected;
        vector<vector<string>> resultDbConfig;

        // Project Info
        string nomeProjecto, oldPorto, porto, pathWWW;

        /*********************************************************************************************
         * DATA BASE METHODS
         ********************************************************************************************/
        void saveDeleteInfo(bool);
        bool getInfoProjectDB();
        void setOperationDB(int);
        void printActiveProject();

        /*********************************************************************************************
         * CONFIG SERVER AND FRAMEWORK
         ********************************************************************************************/
        void configServer(string);
        void configFramework(string);
        
    public:
        ConfigSiteTool();
        ~ConfigSiteTool();

        /*********************************************************************************************
         * CLI
         ********************************************************************************************/
        void helpCLI(string);
        void insertProjectCLI(int, string *);
        void readInputAndSelectCLI(int, string *);

        /*********************************************************************************************
         * PRINT MENU AND SELECT SERVER, FRAMEWORK AND OTHER OPERATION
         ********************************************************************************************/
        int selectServer();
        int selectFramework();
        int selectOperation();

        /*********************************************************************************************
         * GET PROJECT INFO
         ********************************************************************************************/
        bool getNameProject();
        void getPathProject();
        bool getPortProject(bool);
        bool executeOptionSelected(int);

        /*********************************************************************************************
         * OTHER
         ********************************************************************************************/
        void initExecution();
        void executeAllNecessaryCommand(int);
};

#endif // CONFIGSITETOOL_H