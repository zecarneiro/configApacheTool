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
declare executableAPP="$appInstalationPath$appPath/bin/$appPath"
declare iconAPP="$appInstalationPath$appPath/icons/$appPath.png"
declare compileCommand="make -C $appInstalationPath$appPath/ -f $appInstalationPath$appPath/Makefile"

# Print Messages
function printMessages(){
	local message="$1"

	echo
	echo
	echo "#### $message ####"
	echo
	echo
}

# Install Other Apps
function installOtherApps(){
	local allPPAs="ppa:git-core/ppa"
	local allApps="g++ make git curl"

	printMessages "Install APPs..."
	eval "$functionsFile -i \"$allApps\" \"$allPPAs\""
	printMessages "Instalation of APPs done..."
}

# Install ConfigSite
function installConfigSite(){
	local permission="755"
	local atalhoFile="$pathHome/.local/share/applications/$appPath.desktop"

	printMessages "Install ConfigSite..."
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

	printMessages "Uninstall ConfigSite..."
	if [ -f $atalhoFile ]; then
		sudo rm $atalhoFile
	fi

	if [ -d "$dirInstalationApp" ]; then
		sudo rm -r "$dirInstalationApp"
	fi
	printMessages "Uninstall of ConfigSite done..."
}

# Set Default path
function setDefaultPath(){
	local permission="777"
	local fileToSetDefaultPath="$appInstalationPath$appPath/lib/includes.h"

	printMessages "Set Deafult Path..."
	echo "Default full path for projects: $pathWWW" 
	read -p "Insert full path for projects(Press ENTER for default path): " path

	# Verify if folder exist
	if [ ! -z "$path" ]; then
		pathWWW="$path"
	fi

	# Create pathWWW and set permission
	sudo mkdir -p "$pathWWW"
	sudo chmod -R $permission "$pathWWW"

	# Set Default path www on app
	# sudo sed -i 's#PATH_WWW#localizacao_www#' /opt/configSite/includes.h
	sudo sed -i "s#PATH_WWW#$pathWWW#" "$fileToSetDefaultPath"
	printMessages "Set Default Path Done..."
}

# Instalation of servers
function installServer(){
	local allServerApp="apache2 nginx"

	printMessages "Install Servers..."
	eval "$functionsFile -i \"$allServerApp\""
	printMessages "Instalation of servers done..."
}

# Install PHP apps
function installPhp(){
	local allPhpApp="snmp-mibs-downloader php libapache2-mod-php php-mysql php-intl php-mbstring php-xml php-curl php-gd"
	allPhpApp="$allPhpApp php-pear php-imagick php-imap php-memcache php-pspell php-recode php-snmp"
	allPhpApp="$allPhpApp php-tidy php-xmlrpc php-sqlite3"

	printMessages "Install PHP APPs..."
	eval "$functionsFile -i \"$allPhpApp\""
	printMessages "Instalation of PHP APPs done..."
}

# Install Composer
function installComposer(){
	local -i forceInstall
	local pathComposerInstalation="/usr/bin/composer"

	printMessages "Install Composer..."
	if [ ! -z $forceInstallComposer ]; then
		forceInstall=$forceInstallComposer
	else
		forceInstall="0"
	fi

	if [ ! -f $pathComposerInstalation ]||[ $forceInstall -eq 1 ]; then
		curl -sS https://getcomposer.org/installer | sudo php -- --install-dir=/usr/bin --filename=composer
	fi
	printMessages "Instalation of Composer done..."
}

# Install Other App defined by user
function installAppByUser(){
	printMessages "### Install Apps Defined by user ###"
	./otherAppByUser.sh
	printMessages "### Install Apps Defined by user: Finish ###"
}

# Config Apache
function configApache(){
	printMessages "Config Apache2"
	sudo a2enmod rewrite

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

	printMessages "Config NGinx"
	# Change default port nginx because apache port: 80. Change to port 81
	sudo sed -i "s#$lineToChange#$newLineToChange#" $fileConfig
	sudo sed -i "s#$lineToChange1#$newLineToChange1#" $fileConfig

	# Reload and Restart
	sudo service nginx start
	sudo service nginx reload
	sudo service nginx restart
	printMessages "Config NGinx Done..."
}

# Main
function main(){
	case "$installUninstall" in
		"-i")
			installConfigSite
			installOtherApps

			# Compile
			eval "$compileCommand"

			setDefaultPath
			installServer
			installPhp
			installComposer
			installAppByUser
			configApache
			configNGinx
			;;
		"-u")
			uninstallConfigSite
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