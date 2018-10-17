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
	this->nameFileDataBase = "configSiteTool.db";
    this->pathDataBase = CONFIGFOLDER;
    this->fileDataBase = this->pathDataBase + "/" + this->nameFileDataBase;

    // Create Data Base if not exist
    if (!this->classAllOperationGlobal.checkFile(CONFIGFOLDER, this->nameFileDataBase)) {
        this->createDataBase();
    }
}

/**
 * Destructor
 */
AllDataBase::~AllDataBase() {}

/**
 * Open Data Base
 */
void AllDataBase::openDataBase() {
    int status = sqlite3_open(this->fileDataBase.c_str(), &this->dataBase);
    if( status )
    {
	    cerr << "Can't open database: " << sqlite3_errmsg(this->dataBase) << endl;

        // Save error
        string error = sqlite3_errmsg(this->dataBase);
        error = "Can't open database: " + error;
        this->classAllOperationGlobal.logError(error);

	    sqlite3_close(this->dataBase);
        exit(1);
    }
}

/**
 * Close Data Base
 */
void AllDataBase::closeDataBase() {
    sqlite3_close(this->dataBase);
}

/**
 * Create Data Base
 */
void AllDataBase::createDataBase(){
    this->openDataBase();

    // Create Data Base
    const char* queryCreate = "CREATE TABLE configSiteTool("
        "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
        "name_project VARCHAR(50) NOT NULL,"
        "server VARCHAR(50) NOT NULL,"
        "port INTEGER NOT NULL,"
        "path VARCHAR(200),"
        "date DATETIME DEFAULT CURRENT_TIMESTAMP)";
    
    sqlite3_exec(this->dataBase, queryCreate, NULL, NULL, NULL);
    this->closeDataBase();
}

