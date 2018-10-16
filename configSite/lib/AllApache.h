/* Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
*/
#ifndef ALLAPACHE_H
#define ALLAPACHE_H

#include "../lib/includes.h"
#include "../lib/AllOperationGlobal.h"

/* Definition to all necessary apache config */
class AllApache {
    private:
        string enableSiteCmd;
        string disableSiteCmd;
        string restartApacheCmd;
        string reloadApacheCmd;
        string apachePort;
        string apachePath;
        string apacheSitesAvailable;
        string portsConf;
        string extensionVirtualConf;
        string groupApache;
        AllOperationGlobal classAllCommands;

    public:
        AllApache();
        ~AllApache();

        void setGroupApache(string);
        void reloadRestartApache(int);
        void setPortApache(string, string);
        void unsetPortApache(string);
        void setPathApache(string, string);
        void deleteVirtualConfFileApache(string);
        bool checkPortsUsedApache(string);
        void enableSiteApache(string);
        void disableSiteApache(string);
        void copyVirtualConfApache(string);
        bool checkVirtualConfExistApache(string);
};

#endif // ALLAPACHE_H