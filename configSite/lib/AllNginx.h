/* Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
*/
#ifndef ALLNGINX_H
#define ALLNGINX_H

#include "../lib/includes.h"
#include "../lib/AllOperationGlobal.h"
#include "../lib/AllDataBase.h"

/* Definition to all necessary nginx config */
class AllNginx {
    private:
        /*********************************************************************************************
         * VARIABLES
         ********************************************************************************************/    
        // Commands
        string restartNginxCmd, reloadNginxCmd;

        // Nginx variables
        string pathNginx, portConfFileNginx, sitesAvailablePathNginx, sitesEnablePathNginx, virtualConfTemplateNginx;
        string groupNginx, nameOnDBNginx;

        // Project variables
        string nameProjectNginx, pathProjectNginx, portProjectNginx;

        // Data base variables
        vector<vector<string>> resultDbNginx;

        /*********************************************************************************************
         * VARIABLES FOR OTHER CLASS
         ********************************************************************************************/
        AllOperationGlobal classAllOperationGlobal;
        AllDataBase classAllDataBase;
        
        /*********************************************************************************************
         * DATA BASE METHODS
         ********************************************************************************************/
        void insertNginxInfoDB();
        bool getInfoProjectDBNginx(string);

        /*********************************************************************************************
         * GET AND SET GROUP/PERMISSION
         ********************************************************************************************/
        void setGroupNginx(string);
        string getGroupNginx();
        void setPermissionNginx(string);

        /*********************************************************************************************
         * SET/UNSET PORT AND PATH
         ********************************************************************************************/
        void setPortNginx(string, string);
        void unsetPortNginx(string);
        void setPathNginx(string, string);

        /*********************************************************************************************
         * COPY/DELETE AND CHECK VIRTUAL CONF
         ********************************************************************************************/
        bool checkVirtualConfExistNginx(string);
        void copyVirtualConfNginx(string);
        void deleteVirtualConfFileNginx(string);

        /*********************************************************************************************
         * ENABLE AND DISABLE SITE
         ********************************************************************************************/
        void enableSiteNginx();
        void disableSiteNginx();

        /*********************************************************************************************
         * OTHER METHODS
         ********************************************************************************************/
        string getFullPath();

    protected:
        /*********************************************************************************************
         * CHECK PORT AND PATH
         ********************************************************************************************/
        bool checkPortsUsedNginx(string);

    public:
        AllNginx();
        ~AllNginx();

        /*********************************************************************************************
         * CONFIG/RESTART/RELOAD Nginx
         ********************************************************************************************/
        void configNginx(string, string, string);
        void reloadRestartNginx(int);
};

#endif // ALLNGINX_H