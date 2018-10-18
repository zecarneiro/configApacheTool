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
    this->lengthName = 50;
    this->lengthPath = 200;

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

void AllDataBase::execQuery(const char* query, bool useCallBack) {
    int rc;
    if (useCallBack) {
        rc = sqlite3_exec(this->dataBase, query, callback, NULL, NULL);
    } else {
        rc = sqlite3_exec(this->dataBase, query, NULL, NULL, NULL);
    }

    // ERROR
    if( rc != SQLITE_OK ) {
        cout<<"SQL error: "<<sqlite3_errmsg(this->dataBase)<<"\n";

        // Save error
        string error = sqlite3_errmsg(this->dataBase);
        error = "SQL error: " + error;
        this->classAllOperationGlobal.logError(error);

        sqlite3_close(this->dataBase);
        exit(1);
    }
}



/**
 * Create Data Base
 */
void AllDataBase::createDataBase(){
    this->openDataBase();
    const char* query;

    // Create Data Base Server
    string queryServer = "CREATE TABLE IF NOT EXISTS Server("
        "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
        "name_server VARCHAR(" + to_string(this->lengthName) + ") NOT NULL,"
        "UNIQUE (name_server COLLATE NOCASE))"; // COLLATE NOCASE : case insensitive
    query = queryServer.c_str();
    this->execQuery(query, false);

    // Create Data Base Framework
    string queryFramework = "CREATE TABLE IF NOT EXISTS Framework("
        "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
        "name_framework VARCHAR(" + to_string(this->lengthName) + ") NOT NULL,"
        "UNIQUE (name_framework COLLATE NOCASE))"; // COLLATE NOCASE : case insensitive
    query = queryFramework.c_str();
    this->execQuery(query, false);

    // Create Data Base Framework
    string queryDirectory = "CREATE TABLE IF NOT EXISTS Directory("
        "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
        "directory VARCHAR(" + to_string(this->lengthPath) + "),"
        "UNIQUE (directory COLLATE NOCASE))"; // COLLATE NOCASE : case insensitive
    query = queryFramework.c_str();
    this->execQuery(query, false);

    // Create Data Base config site tool
    string queryCreateConfigSite = "CREATE TABLE IF NOT EXISTS ConfigSite("
        "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
        "name_project VARCHAR(" + to_string(this->lengthName) + ") NOT NULL,"
        "port INTEGER NOT NULL,"
        "framework_id INTEGER NOT NULL,"
        "directory_id INTEGER NOT NULL,"
        "server_id INTEGER NOT NULL,"
        "FOREIGN KEY(framework_id) REFERENCES Framework(id)"
        "FOREIGN KEY(directory_id) REFERENCES Directory_id(id)"
        "FOREIGN KEY(server_id) REFERENCES Server(id))";
    query = queryCreateConfigSite.c_str();
    this->execQuery(query, false);

    // Insert Server Value
    string serverValues = "INSERT INTO Server(name_server) VALUES ('Apache2');";
    serverValues += "INSERT INTO Server(name_server) VALUES ('NGinx');";
    query = serverValues.c_str();
    this->execQuery(query, true);

    this->closeDataBase();
}

