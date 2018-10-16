/**
 * Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
 */
#include "../lib/AllDataBase.h"

/**
 * Constructor
 */
AllDataBase::AllDataBase(string path) {
	this->nameFileDataBase = "configSiteTool.db";
    this->pathDataBase = path;
}

/**
 * Destructor
 */
AllDataBase::~AllDataBase() {}