/* Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
*/
#ifndef ALLCOMMANDS_H
#define ALLCOMMANDS_H

#include "../lib/includes.h"

/* Definition to all necessary commands */
class AllCommands {
    private:
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
        string otherFunctionsCommand;

    public:
        AllCommands();
        ~AllCommands();

        string getCommandSudo(string);
        string getCommandCopy(string);
        string getCommandCd(string);
        string getCommandMkdir(string);
        string getCommandDel(string);
        string getCommandGrep(string);
        string getCommandMv(string);
        string getCommandCat(string);
        string getCommandSed(string);
        string getCommandChown(string);
        string getCommandChmod(string);

        void executeCommands(string);
        string executeCommandsWithOutput(const char*);
        void setPermission(string);
        bool checkDirectory(string, string);
        bool checkFile(string, string);
};
#endif // ALLCOMMANDS_H