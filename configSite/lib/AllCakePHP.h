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
        string newProjectCmd, updateProjectCmd, configProjectCmd;
        string nameOnDBCakePHP;
        vector<vector<string>> resultDbCakePHP;
        AllOperationGlobal classAllOperationGlobal;
        AllApache classAllApache;
        AllDataBase classAllDataBase;

        void insertCakePHPInfoDB();
        void setPermissionCakePHP(string, string);
        void newProjectCakePHP(string, string);
        void updateProjectCakePHP(string, string);
        void configProjectCakePHP(string, string);
        void printActiveProjectCakePHP();

    public:
        AllCakePHP();
        ~AllCakePHP();
        void setOperationCakePHP(string, string, string);
};

#endif // ALLCAKEPHP_H
