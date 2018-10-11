#!/bin/bash
# Create By José M. C. Noronha
# Lisbon 10 May 2018

# Define Path
pathHome=$( echo $HOME )
pathDestAtalho="$pathHome/.local/share/applications"
pathOpt="/opt/"
nameWWW="www"
namePathApp="configSite"
nameApp="configSite"
nameDisableProject="disableProject.sh"
pathWWW="$pathHome/$nameWWW"
pathApp="$namePathApp/"

# Define Files
fileConficpp="config.cpp"

# main
echo "Default full path for projects: $pathWWW" 
read -p "Insert full path for projects(Press ENTER for default path): " path


# Verify if folder exist
if [ -d "$path" ]; then
	pathWWW="$path/$nameWWW"
fi

# Create www folder
if [ ! -d "$pathWWW" ]; then
	mkdir "$pathWWW"
fi

# Create path to desktop file
./createFolder.sh "$pathHome" .local share applications

# Copy App Folder
sudo cp -r "$pathApp" "$pathOpt"

# Create Desktop File
echo "
[Desktop Entry]
Encoding=UTF-8
Name=configSite
Comment=configSite
Exec=\"$pathOpt$pathApp$nameApp\"
Terminal=true
Type=Application
Icon=$pathOpt$pathApp$nameApp.png
StartupNotify=true
Name[pt]=configSite" > "$pathDestAtalho/$nameApp.desktop"
chmod +x "$pathDestAtalho/$nameApp.desktop"

# Set Default path www on app
# sudo sed -i 's#PATH_WWW#localizacao_www#' /opt/configSite/config.cpp
sudo sed -i "s#PATH_WWW#$pathWWW#" "$pathOpt$pathApp$fileConficpp"

# Compile App 
sudo apt install g++ make git python3 -y
sudo g++ -Wall "$pathOpt$pathApp$fileConficpp" -o "$pathOpt$pathApp$nameApp.exe"

# Set Permission to execute app
sudo chmod +x "$pathOpt$pathApp$nameApp.exe"

# Set Permission to execute disable project
sudo chmod +x "$pathOpt$pathApp$nameDisableProject"

# Install Apache2
sudo apt install apache2 -y

# Install Other
sudo apt install snmp-mibs-downloader -y

# Install PHP
sudo apt install php libapache2-mod-php php-mysql php-intl php-mbstring php-xml php-curl php-gd -y

# Install PHP(Continue)
sudo apt install php-pear php-imagick php-imap php-memcache php-pspell php-recode php-snmp -y

# Install PHP(Continue)
sudo apt install php-tidy php-xmlrpc php-sqlite3 -y

# Install Git
sudo add-apt-repository ppa:git-core/ppa -y
sudo apt update
sudo apt install git -y

# Install Composer
sudo apt install curl -y
curl -sS https://getcomposer.org/installer | sudo php -- --install-dir=/usr/bin --filename=composer

# Install Other App defined by user
echo
echo
echo "### Install Apps Defined by user ###"
./otherAppByUser.sh
echo
echo
echo "### Install Apps Defined by user: Finish ###"
echo
echo

# Config Apache2
sudo a2enmod rewrite
sudo service apache2 restart
