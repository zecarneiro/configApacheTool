/* Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
*/
#ifndef ALLDATABASE_H
#define ALLDATABASE_H

#include "../lib/includes.h"
#include "../lib/AllOperationGlobal.h"

/* Definition to all necessary apache config */
class AllDataBase {
    private:
        string nameFileDataBase;
        string pathDataBase;
        string fileDataBase;
        sqlite3 *dataBase;
        AllOperationGlobal classAllOperationGlobal;

    public:
        AllDataBase();
        ~AllDataBase();

        void openDataBase();
        void closeDataBase();
        void createDataBase();
        void printActiveProject();
        void getProject(string &);
        bool changePort(string nameProject, string server);
        bool deleteActiveProject(string nameProject, string server);
};

#endif // ALLAPACHE_H