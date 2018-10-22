/**
 * Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
 */
#include "../lib/AllDataBase.h"

/**
 * Constructor
 */
AllDataBase::AllDataBase() {
    //this->dataBase = NULL;
	this->nameFileDataBase = "configSiteTool.db";
    this->pathDataBase = CONFIGFOLDER;
    this->fileDataBase = this->pathDataBase + "/" + this->nameFileDataBase;
    this->lengthName = 50;
    this->lengthPath = 200;
    this->createDataBase();
}

/**
 * Destructor
 */
AllDataBase::~AllDataBase() {}

void AllDataBase::savePrintError() {
    string error = sqlite3_errmsg(this->dataBase);
    error = "Can't open database: " + error;

    // Print 
    cerr << error << endl;

    // Save Error
    this->classAllOperationGlobal.logError(error);

    // Close Data Base
    sqlite3_close(this->dataBase);

    // Exit
    exit(1);
}

/**
 * Open Data Base
 */
void AllDataBase::openDataBase() {
    int status = sqlite3_open(this->fileDataBase.c_str(), &this->dataBase);
    if(status) this->savePrintError();
}

/**
 * Close Data Base
 */
void AllDataBase::closeDataBase() {
    sqlite3_close(this->dataBase);
}

void AllDataBase::execQuery(const char* query) {
    this->openDataBase();
    int rc = sqlite3_exec(this->dataBase, query, NULL, NULL, NULL);

    // ERROR
    if( rc != SQLITE_OK ) this->savePrintError();
    this->closeDataBase();
}

vector<vector<string> > AllDataBase::execQueryReturnData(const char* query) {
    this->openDataBase();
    sqlite3_stmt *statement;
    vector<vector<string> > results;
    
    if(sqlite3_prepare_v2(this->dataBase, query, -1, &statement, 0) == SQLITE_OK) {

        int cols = sqlite3_column_count(statement);
        int result = 0;
        while(true)
        {
            result = sqlite3_step(statement);
                
            if(result == SQLITE_ROW)
            {
                vector<string> values;
                for(int col = 0; col < cols; col++)
                {
                    values.push_back((char*)sqlite3_column_text(statement, col));
                }
                results.push_back(values);
            }
            else
            {
                break;  
            }
        }
        
        sqlite3_finalize(statement);
    } else {
        this->savePrintError();
    } 
    return results; 
}

/**
 * Create Data Base
 */
void AllDataBase::createDataBase(){
    // Create Data Base if not exist
    if (!this->classAllOperationGlobal.checkFile(CONFIGFOLDER, this->nameFileDataBase)) {
        const char* query;

        // Create Data Base Server
        string queryServer = "CREATE TABLE IF NOT EXISTS Server("
            "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
            "name_server VARCHAR(" + to_string(this->lengthName) + ") NOT NULL,"
            "virtual_conf VARCHAR(" + to_string(this->lengthPath) + ") NOT NULL,"
            "UNIQUE (name_server COLLATE NOCASE))"; // COLLATE NOCASE : case insensitive
        query = queryServer.c_str();
        this->execQuery(query);

        // Create Data Base Framework
        string queryFramework = "CREATE TABLE IF NOT EXISTS Framework("
            "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
            "name_framework VARCHAR(" + to_string(this->lengthName) + ") NOT NULL,"
            "UNIQUE (name_framework COLLATE NOCASE))"; // COLLATE NOCASE : case insensitive
        query = queryFramework.c_str();
        this->execQuery(query);

        // Create Data Base config site tool
        string queryCreateConfigSite = "CREATE TABLE IF NOT EXISTS ConfigSite("
            "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
            "name_project VARCHAR(" + to_string(this->lengthName) + ") NOT NULL,"
            "port INTEGER UNIQUE,"
            "directory VARCHAR(" + to_string(this->lengthPath) + "),"
            "framework_id INTEGER NOT NULL,"
            "server_id INTEGER NOT NULL,"
            "UNIQUE (name_project COLLATE NOCASE)," // COLLATE NOCASE : case insensitive
            "FOREIGN KEY(framework_id) REFERENCES Framework(id),"
            "FOREIGN KEY(server_id) REFERENCES Server(id))";
        query = queryCreateConfigSite.c_str();
        this->execQuery(query);
    }
}

