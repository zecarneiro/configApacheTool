/* Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
*/
#ifndef ALLAPACHE_H
#define ALLAPACHE_H

#include "../lib/includes.h"
#include "../lib/AllCommands.h"

/* Definition to all necessary apache config */
class AllApache {
    protected:
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
        AllCommands classAllCommands;

    public:
        AllApache();
        ~AllApache();

        void setGroupApache(string);
        void reloadRestart(int);
        void setPort(string, string);
        void unsetPort(string);
        void setPath(string, string);
        void deleteVirtualConfFile(string);
        bool checkPortsUsed(string);
        void enableSite(string);
        void disableSite(string);
        void copyVirtualConf(string);
        bool checkVirtualConfExist(string);
};

#endif // ALLAPACHE_H