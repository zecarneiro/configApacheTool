/* Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
*/
#ifndef ALLAPACHE_H
#define ALLAPACHE_H

#include "../lib/includes.h"
#include "../lib/AllOperationGlobal.h"
#include "../lib/AllDataBase.h"

/* Definition to all necessary apache config */
class AllApache {
    private:
        /*********************************************************************************************
         * VARIABLES
         ********************************************************************************************/    
        // Commands
        string enableSiteApacheCmd, disableSiteApacheCmd, restartApacheCmd, reloadApacheCmd;

        // Apache variables
        string pathApache, portConfFileApache, sitesAvailablePathApache, virtualConfTemplateApache;
        string groupApache, nameOnDBApache;

        // Project variables
        string nameProjectApache, pathProjectApache, portProjectApache;

        // Data base variables
        vector<vector<string>> resultDbApache;

        // Other variables
        string extensionVirtualConfApache;

        /*********************************************************************************************
         * VARIABLES FOR OTHER CLASS
         ********************************************************************************************/
        AllOperationGlobal classAllOperationGlobal;
        AllDataBase classAllDataBase;
        
        /*********************************************************************************************
         * DATA BASE METHODS
         ********************************************************************************************/
        void insertApacheInfoDB();
        bool getInfoProjectDBApache(string);

        /*********************************************************************************************
         * GET AND SET GROUP/PERMISSION
         ********************************************************************************************/
        void setGroupApache(string);
        string getGroupApache();
        void setPermissionApache(string);

        /*********************************************************************************************
         * SET/UNSET PORT AND PATH
         ********************************************************************************************/
        void setPortApache(string, string);
        void unsetPortApache(string);
        void setPathApache(string, string);

        /*********************************************************************************************
         * COPY/DELETE AND CHECK VIRTUAL CONF
         ********************************************************************************************/
        bool checkVirtualConfExistApache(string);
        void copyVirtualConfApache(string);
        void deleteVirtualConfFileApache(string);

        /*********************************************************************************************
         * ENABLE AND DISABLE SITE
         ********************************************************************************************/
        void enableSiteApache();
        void disableSiteApache();

        /*********************************************************************************************
         * OTHER METHODS
         ********************************************************************************************/
        string getFullPath();

    protected:
        /*********************************************************************************************
         * CHECK PORT AND PATH
         ********************************************************************************************/
        bool checkPortsUsedApache(string, bool);

    public:
        AllApache();
        ~AllApache();

        /*********************************************************************************************
         * CONFIG/RESTART/RELOAD APACHE
         ********************************************************************************************/
        void configApache(string, string, string);
        void reloadRestartApache(int);
};

#endif // ALLAPACHE_H