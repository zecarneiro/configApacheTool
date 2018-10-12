/* Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
*/
#include <iostream>
#include <cstdlib>	//Para system()
#include <stdlib.h>	//Para system()
#include <string>
#include <sstream>	//Para converter inteiros em strings
#include <cstdio>	//Para popen()
using namespace std;

/**
 * Include All Class Headers
 */
#ifndef CLASS_H
#define CLASS_H

#include "../lib/AllCommands.h"
#include "../lib/AllApache.h"
#include "../lib/AllCakePHP.h"

#endif

/**
 * Include All Variables
 */
#ifndef VARIABLES_H
#define VARIABLES_H

const string APPFOLDER = "/opt/configSite/";
const string APACHEVIRTUALCONF = APPFOLDER + "virtualConfApacheTemplate.conf";
const string PORTOIDENTIFY = "PORTO";
const string PATHIDENTIFY = "FULL_PATH_PROJECT";
const string PATHWWW = "PATH_WWW";
const string LEVELPERMISSION = "777";
const string INFOPROJECTAPACHE = "infoProjectApache.sh";

#endif