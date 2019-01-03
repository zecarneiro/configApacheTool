#!/bin/bash
# Create By José M. C. Noronha
# Lisbon 10 May 2018

# Args
declare installUninstall="$1"
declare forceInstallComposer="$2"

# System Path
pathHome=$( echo $HOME )
nameWWW="www"
pathWWW="$pathHome/$nameWWW"

# App Var
declare appPath="configSite"
declare appInstalationPath="/opt/"
declare functionsFile="$appInstalationPath$appPath/src/functions.sh"
declare executableAPP="$appInstalationPath$appPath/bin/$appPath -e"
declare iconAPP="$appInstalationPath$appPath/icons/$appPath.png"
declare compileCommand="make -C $appInstalationPath$appPath/ -f $appInstalationPath$appPath/Makefile"
declare phpVersion
declare virtualConfNginx="$appInstalationPath$appPath/virtualConfNginxTemplate"
declare sudoersPath="/etc/sudoers"

# Print Messages
function printMessages(){
	local message="$1"

	echo "#### $message ####"
	echo
}

# Install Other Apps
function installOtherApps(){
	local allPPAs="ppa:git-core/ppa"
	local allApps="g++ make git curl libsqlite3-dev unoconv sendemail inotify-tools"

	eval "$functionsFile -i \"$allApps\" \"$allPPAs\""
	printMessages "Instalation of APPs done..."
}

# Install ConfigSite
function installConfigSite(){
	local permission="755"
	local atalhoFile="$pathHome/.local/share/applications/$appPath.desktop"

	# Copy app path to instalation path
	sudo cp -r $appPath $appInstalationPath

	# Set Permission
	sudo chmod -R $permission "$appInstalationPath$appPath"

	# Create desktop file
	eval "$functionsFile -dFile \"$appPath\" \"$appPath.desktop\" \"$executableAPP\" \"$iconAPP\" 0"

	# Activate to execute on terminal
	sed -i "s#Terminal=false#Terminal=true#" $atalhoFile
	printMessages "Instalation of ConfigSite done..."
}

# Uninstall ConfigSite
function uninstallConfigSite(){
	local atalhoFile="$pathHome/.local/share/applications/$appPath.desktop"
	local dirInstalationApp="$appInstalationPath$appPath"

	if [ -f $atalhoFile ]; then
		sudo rm $atalhoFile
	fi

	if [ -d "$dirInstalationApp" ]; then
		sudo rm -r "$dirInstalationApp"
	fi
	printMessages "Uninstall of ConfigSite done..."
}

# Set Default path
function setPathAndOther(){
	local userServer="www-data"
	local fileToSetDefaultPath="$appInstalationPath$appPath/lib/includes.h"

	echo "Default full path for projects: $pathWWW"
	read -p "Insert full path for projects(Press ENTER for default path): " path

	# Verify if folder exist
	if [ ! -z "$path" ]; then
		pathWWW="$path"
	fi

	# Create pathWWW
	mkdir -p "$pathWWW"
	chmod -R 755 "$pathWWW"
	sudo chown -R :"$userServer" "$pathWWW"

	# Set Default path www on app
	# sudo sed -i 's#PATH_WWW#localizacao_www#' /opt/configSite/includes.h
	sudo sed -i "s#PATH_WWW#$pathWWW#" "$fileToSetDefaultPath"
	sudo sed -i "s#HOME_DIR#$pathHome#" "$fileToSetDefaultPath"
	printMessages "Set Path Done..."

	# Set PHP Version
	sudo sed -i "s#PHPVERSION#php$phpVersion#" $virtualConfNginx
}

# Instalation of servers
function installServer(){
	local allServerApp="apache2 nginx"

	eval "$functionsFile -i \"$allServerApp\""
	printMessages "Instalation of servers done..."
}

# Install PHP apps
function installPhp(){
	local allPhpApp="snmp-mibs-downloader php libapache2-mod-php php-mysql php-intl php-mbstring php-xml php-curl php-gd"
	allPhpApp="$allPhpApp php-pear php-imagick php-imap php-memcache php-pspell php-recode php-snmp"
	allPhpApp="$allPhpApp php-tidy php-xmlrpc php-sqlite3 php-fpm"

	eval "$functionsFile -i \"$allPhpApp\""

	phpVersion=$(php -v | grep -i php | cut -d ' ' -f2 | cut -d '.' -f1-2 | head -1)
	printMessages "Instalation of PHP APPs done..."
}

# Install Composer
function installComposer(){
	local -i forceInstall
	local pathComposerInstalation="/usr/bin/composer"

	if [ ! -z $forceInstallComposer ]; then
		forceInstall=$forceInstallComposer
	else
		forceInstall="0"
	fi

	if [ ! -f $pathComposerInstalation ]||[ $forceInstall -eq 1 ]; then
		curl -sS https://getcomposer.org/installer | sudo php -- --install-dir=/usr/bin --filename=composer
		printMessages "Instalation of Composer done..."
	fi
}

function defaultOptionsDBFirstConfig(){
	local -i option="$1"
	
	if [ $option -eq 0 ]||[ $option -eq 1 ]; then
		printf "\n### CONFIG OPTIONS FOR MariaDB ###\n"
		echo "Enter current password for root (enter for none): Just press the Enter"
		echo "Set root password? [Y/n]: Y"
		echo "New password: Enter password\n"
		echo "Re-enter new password: Repeat password"
		echo "Remove anonymous users? [Y/n]: Y"
		echo "Disallow root login remotely? [Y/n]: Y"
		echo "Remove test database and access to it? [Y/n]: Y"
		printf "Reload privilege tables now? [Y/n]: Y\n"
	elif [ $option -eq 2 ]; then
		echo "CONFIG OPTIONS FOR POSTGRESQL"
	fi
}

# Install Data Bases
function installDataBases(){
	echo "1 - MySQL"
	echo "2 - MariaDB"
	echo "3 - PostgreSQL"

	echo "None PRESS ENTER"
	read -p "Insert Option: " server

	case "$server" in
		"1") # MySQL
			eval "$functionsFile -i \"mysql-server mysql-client\""
			defaultOptionsDBFirstConfig 0
			read -p "Insert your password(Press to Default: root): " password
			if [ -z $password ]; then
				password="root"
			fi
			sudo mysql -u root -e "ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY '$password';"
			sudo systemctl restart mysql.service
			;;
		"2") # MariaDB
			eval "$functionsFile -i \"mariadb-server mariadb-client\""
			defaultOptionsDBFirstConfig 1
			sudo mysql_secure_installation
			sudo systemctl restart mariadb.service
			sudo mariadb -u root -p mysql -e "update user set plugin=' ' where User='root';"
			sudo systemctl restart mariadb.service
			;;
		"3") # PostgreSQL
			eval "$functionsFile -i \"postgresql postgresql-contrib\""
			defaultOptionsDBFirstConfig 2
	esac

	if [ ! -z $server ]; then
		printMessages "Instalation of Server done..."
	fi
}

# Install Other App defined by user
function installAppByUser(){
	printMessages "### Install Apps Defined by user ###"
	./otherAppByUser.sh
	printMessages "### Install Apps Defined by user: Finish ###"
}

# Config Apache
function configApache(){
	# Enable mod rewrite
	sudo a2enmod rewrite
	sudo service apache2 restart

	# Disable mod mpm_event
	sudo a2dismod mpm_event
	sudo service apache2 restart

	# Enable mod php
	sudo a2enmod php$phpVersion
	sudo service apache2 restart

	# Reload and Restart
	sudo service apache2 reload
	sudo service apache2 restart
	printMessages "Config Apache2 Done..."
}

# Config NGinx
function configNGinx(){
	local lineToChange="listen 80 default_server;"
	local newLineToChange="listen 81 default_server;"
	local lineToChange1="listen \[::\]:80 default_server;"
	local newLineToChange1="listen \[::\]:81 default_server;"
	local fileConfig="/etc/nginx/sites-available/default"

	# Change default port nginx because apache port: 80. Change to port 81
	sudo sed -i "s#$lineToChange#$newLineToChange#" $fileConfig
	sudo sed -i "s#$lineToChange1#$newLineToChange1#" $fileConfig

	# Reload and Restart
	sudo service nginx start
	sudo service nginx reload
	sudo service nginx restart
	printMessages "Config NGinx Done..."
}

function createAliasCmd(){
	local envName="_WWW_PATH"
	local comment="# Command to go partition _WWW_PATH"
	local -i isSet="$1"

	if [[ $isSet -eq 1 ]]; then
		echo "$comment" | tee -a ~/.bashrc > /dev/null
		echo "export $envName=$pathWWW" | tee -a ~/.bashrc > /dev/null
		printMessages "Created Envoriement Name: $envName"
	else
		sed -i "/$comment/d" ~/.bashrc
		sed -i "/export $envName/d" ~/.bashrc
	fi
}

function installScriptMonitor(){
	local -i isSet="$1"
	local scriptMonitorName="setOwnerPermissionNewEntry.sh"
	local scriptMonitorPath="$appInstalationPath$appPath/src/$scriptMonitorName"
	local commentMonitor="# ConfigSiteTool ScriptMonitor"
	local autostartMonitor="$pathHome/.config/autostart/$appPath.desktop"

	if [[ $isSet -eq 1 ]]; then
		echo "$commentMonitor" | sudo tee -a $sudoersPath > /dev/null
		echo "ALL ALL=(ALL) NOPASSWD:$scriptMonitorPath" | sudo tee -a $sudoersPath > /dev/null
		
		# Create desktop file
		eval "$functionsFile -dFile \"$appPath\" \"$appPath.desktop\" \"$scriptMonitorPath\" \"$iconAPP\" 1"

		# Execute ScriptMonitor
		eval "$scriptMonitorPath $pathWWW &"

		printMessages "Script Monitor Instaled.."
	else
		scriptMonitorPath="\/opt\/configSite\/src\/setOwnerPermissionNewEntry.sh"
		sudo sed -i "/$commentMonitor/d" $sudoersPath
		sudo sed -i "/ALL ALL=(ALL) NOPASSWD:$scriptMonitorPath/d" $sudoersPath
		if [ -d "$autostartMonitor" ]; then
			rm "$autostartMonitor"
		fi
	fi
}

# Main
function main(){
	case "$installUninstall" in
		"-i")
			installConfigSite
			installOtherApps
			installServer
			installPhp
			installComposer

			setPathAndOther

			# Compile
			eval "$compileCommand"
			echo

			installAppByUser
			configApache
			configNGinx
			installDataBases
			createAliasCmd 1
			installScriptMonitor 1
			;;
		"-u")
			uninstallConfigSite
			createAliasCmd 0
			installScriptMonitor 0
			;;
		*)
            echo "$0 (-i|-u) OPTIONAL(0|1)"
			echo "-i: Install."
			echo "-u: Uninstall"
			echo "OPTIONAL: If install(-i) so set Force install composer(1). Default is 0"
            ;;
	esac
}
main