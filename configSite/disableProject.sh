#!/bin/bash
# Create By José M. C. Noronha
# Lisbon 10 May 2018

# Define command
cmdSudo="sudo"
cmdRDel="rm"
cmdSed="sed"
cmdDisableProject="a2dissite"

# Define variable
portProject="$1"
nameProject="$2"

# Define file path
apachePath="/etc/apache2"
portConf="$apachePath/ports.conf"
virtualHost="$apachePath/sites-available/$nameProject.conf"

# Remove Port
$cmdSudo $cmdSed -i "/Listen $portProject/d" $portConf

# Remove VirtualHost
$cmdSudo $cmdRDel $virtualHost

# Disable Site
$cmdSudo $cmdDisableProject "$nameProject.conf"

# Restart and reload apache2
$cmdSudo service apache2 reload
$cmdSudo service apache2 restart