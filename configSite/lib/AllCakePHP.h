/* Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
*/
#include "../lib/includes.h"

class AllCakePHP {
    protected:
        string newProjectCmd;
        string updateProjectCmd;
        string configProjectCmd;
        AllApache classAllApache;
        AllCommands classAllCommands;

    public:
        AllCakePHP();
        ~AllCakePHP();

        void newProject(string, string);
        void updateProject(string, string);
        void configProject(string, string);
};
