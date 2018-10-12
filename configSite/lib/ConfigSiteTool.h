/* Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
*/
#include "../lib/includes.h"

/* Definition to project */
class ConfigSiteTool : public AllApache, public AllCakePHP, public AllCommands {
    public:
        string nomeProjecto;
	    int porto;
	    int oldPorto;
	    string pathWWW;

        /* Methods */
        ConfigSiteTool();
        ~ConfigSiteTool();
        void clearCin();
        void printMenu();
        int getOlyInteger(string);
        string intToString(int);
        bool verifyPort(int);
        void getInfoProject(int, bool);
        void configApache(bool);
        void executeAllNecessaryCommand(int);
        void saveInfo(string, int, bool);
        void disableProject();
        void createNewProject();
        void executeOptionSelected(int);
};