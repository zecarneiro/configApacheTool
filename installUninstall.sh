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

# Print Messages
function printMessages(){
	local message="$1"

	echo "#### $message ####"
	echo
}

# Install Other Apps
function installOtherApps(){
	local allPPAs="ppa:git-core/ppa"
	local allApps="g++ make git curl libsqlite3-dev unoconv sendemail"

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

# Install Data Bases
function installDataBases(){
	local allServer=""

	echo "1 - MySQL"
	echo "2 - MariaDB"
	echo "3 - PostgreSQL"

	echo "All: a1|a2 (1-MySQL|2-MariaDB)"
	echo "None PRESS ENTER"
	read -p "Insert Option: " server

	# MySQL
	if [ "$server" = "1" ]||[ "$server" = "a1" ]; then
		allServer="mysql-server mysql-client"
	fi

	# MariaDB
	if [ "$server" = "2" ]||[ "$server" = "a2" ]; then
		allServer="mariadb-server mariadb-client"
	fi

	# PostgreSQL
	if [ "$server" = "3" ]||[ "$server" = "a1" ]||[ "$server" = "a2" ]; then
		allServer="$allServer postgresql postgresql-contrib"
	fi

	if [ ! -z $server ]; then
		# Install
		eval "$functionsFile -i \"$allServer\""

		printf "\n\n"
		echo "PRESS ENTER TO DEFULT"
		read -p "Insert User(Defult = root): " user
		read -p "Insert Password(Defult = root): " password

		if [ -z $password ]; then
			password="root"
		fi

		# Set user info on MySQL Or MariaDB
		if [ ! -z $user ]; then
			local createUser="CREATE USER '$user'@'localhost' IDENTIFIED BY '$password';"
			local setAllPrevileges="GRANT ALL PRIVILEGES ON * . * TO '$user'@'localhost';"
			local delRootUser="DELETE FROM mysql.user WHERE user = 'root';"
			local savePrevileges="FLUSH PRIVILEGES;"

			# Execute
			sudo mysql -u root -e "$createUser"
			sudo mysql -u root -e "$setAllPrevileges"
			sudo mysql -u root -e "$savePrevileges"
			sudo mysql -u $user -e "$delRootUser"
		else
			local setPassword="ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY '$password';"
			sudo mysql -u root -e "$setPassword"
		fi

		# Set user info on PostgreSQL
		if [ ! -z $user ]; then
			local createUser="sudo -u postgres createuser $user"
			local setAllPrevileges="grant all privileges on *.* to $user';"
			local delRootUser="sudo -u postgres dropuser $user"


			# Execute
			sudo mysql -u root -e "$cmdMySQLMariaDB"
			sudo mysql -u root -e "$setAllPrevileges"
			sudo mysql -u $user -e "$delRootUser"
		else
			local setPassword="ALTER USER postgres PASSWORD '$password';"
			sudo -u postgres psql -c "$setPassword"
		fi
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
	local aliasName="_WWW_PATH"
	local comment="# Command to go partition WWW Path"
	local -i isSet="$1"

	if [[ $isSet -eq 1 ]]; then
		echo "$comment" | tee -a ~/.bashrc > /dev/null
		echo "alias $aliasName='cd $pathWWW'" | tee -a ~/.bashrc > /dev/null
		printMessages "Created Alias: $aliasName"
	else
		sed -i "/$comment/d" ~/.bashrc
		sed -i "/alias $aliasName/d" ~/.bashrc
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
			;;
		"-u")
			uninstallConfigSite
			createAliasCmd 0
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
