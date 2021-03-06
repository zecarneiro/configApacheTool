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
        int lengthName;
        int lengthPath;
        sqlite3 *dataBase;
        AllOperationGlobal classAllOperationGlobal;

    public:
        AllDataBase();
        ~AllDataBase();

        void openDataBase();
        void closeDataBase();
        void execQuery(const char* query);
        vector<vector<string> > execQueryReturnData(const char*);
        void createDataBase();
        void savePrintError();
};

#endif // ALLAPACHE_H