/* Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
*/
#ifndef INCLUDE_H
#define INCLUDE_H

#include <iostream>
#include <cstdlib>	//Para system()
#include <stdlib.h>	//Para system()
#include <string>
#include <sstream>	//Para converter inteiros em strings
#include <cstdio>	//Para popen()
#include <sqlite3.h> // Data Base Manager
#include <vector>
using namespace std;

/**
 * Include All Variables
 */
const string APPFOLDER = "/opt/configSite/";
const string APACHEVIRTUALCONF = APPFOLDER + "virtualConfApacheTemplate.conf";
const string PORTOIDENTIFY = "PORTO";
const string PATHIDENTIFY = "FULL_PATH_PROJECT";
const string PATHWWW = "PATH_WWW";
const string LEVELPERMISSION = "777";
const string INFOPROJECTAPACHE = "infoProjectApache.sh";
const string FUNCTIONSHELL = APPFOLDER + "src/functions.sh";
const string CONFIGFOLDER = "HOME_DIR/.config/configSite";

#endif // INCLUDE_H