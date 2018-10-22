/* Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
*/
#ifndef ALLCAKEPHP_H
#define ALLCAKEPHP_H

#include "../lib/includes.h"
#include "../lib/AllApache.h"
#include "../lib/AllOperationGlobal.h"
#include "../lib/AllDataBase.h"

class AllCakePHP {
    private:
        string newProjectCmd;
        string updateProjectCmd;
        string configProjectCmd;
        vector<vector<string>> resultDbConfig;
        AllOperationGlobal classAllCommands;
        AllApache classAllApache;
        AllDataBase classAllDataBase;

    public:
        AllCakePHP();
        ~AllCakePHP();

        void insertCakePHPInfoDB();
        void newProjectCakePHP(string, string);
        void updateProjectCakePHP(string, string);
        void configProjectCakePHP(string, string);
        void printActiveProjectCakePHP();
};

#endif // ALLCAKEPHP_H
