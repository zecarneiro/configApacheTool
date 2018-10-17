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
        void execQuery(const char* query, bool useCallBack);
        void createDataBase();
        void printActiveProject();
        void getProject(string &);
        bool changePort(string nameProject, string server);
        bool deleteActiveProject(string nameProject, string server);
        static int callback(void *unused, int count, char **data, char **columns) {
            int idx;

            cout << "There are " << count << " column(s)\n";

            for (idx = 0; idx < count; idx++) {
                cout << "The data in column \"" << columns[idx] << "\" is: " << data[idx] << "\n";
            }

            cout << endl;
            return 0;
        }
};

#endif // ALLAPACHE_H