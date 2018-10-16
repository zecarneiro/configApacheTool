/* Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
*/
#ifndef ALLCAKEPHP_H
#define ALLCAKEPHP_H

#include "../lib/includes.h"
#include "../lib/AllApache.h"
#include "../lib/AllOperationGlobal.h"

class AllCakePHP {
    private:
        string newProjectCmd;
        string updateProjectCmd;
        string configProjectCmd;
        AllOperationGlobal classAllCommands;
        AllApache classAllApache;

    public:
        AllCakePHP();
        ~AllCakePHP();

        void newProjectCakePHP(string, string);
        void updateProjectCakePHP(string, string);
        void configProjectCakePHP(string, string);
};

#endif // ALLCAKEPHP_H
