/* Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
*/
#ifndef ALLDATABASE_H
#define ALLDATABASE_H

#include "../lib/includes.h"
#include <sqlite3.h>
#include "../lib/AllOperationGlobal.h"

/* Definition to all necessary apache config */
class AllDataBase {
    private:
        string nameFileDataBase;
        string pathDataBase;
        sqlite3 *dataBase;
        AllOperationGlobal classAllOperationGlobal;
        int createDataBase();

    public:
        AllDataBase(string path);
        ~AllDataBase();

        void printActiveProject();
        bool changePort(string nameProject, string server);
        bool deleteActiveProject(string nameProject, string server);
};

#endif // ALLAPACHE_H