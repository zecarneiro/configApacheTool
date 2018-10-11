#!/bin/bash
# Author: José M. C. Noronha

### Global Variable ###
declare homeFolder="$( echo $HOME )"
declare fileToSave="$homeFolder/.infoProjectConfigSite"
declare projectName="$2"
declare -i port=$3
declare redirectNull="/dev/null"

### FUNCTIONS ###
# Verify if file exist
# Return true = 1 / false = 0
function existFile(){
	local -i response=0

	if [ -f $fileToSave ]; then
		response=1
	fi

	echo $response
}

# Verify if info exist
# Return true = 1 / false = 0
function existInfo(){
	local -i response=0
	local -i exist=$( cat $fileToSave | grep -c "ProjectName: $projectName" ) # Count x string exist
	
	if [ $exist -gt 0 ]; then
		response=1
	fi

	# Return true or false
	echo $response
}

# Save info on file
function saveInfo(){
	if [ $(existFile) -eq 1 ]; then
		deleteInfo
		echo "ProjectName: $projectName - Port: $port" | tee -a $fileToSave > $redirectNull
	else
		echo "ProjectName: $projectName - Port: $port" | tee $fileToSave > $redirectNull
	fi
}

# Save info of file
function deleteInfo(){
	if [ $(existFile) -eq 1 ]; then
		if [ $(existInfo) -eq 1 ]; then
			sed -i "/ProjectName: $projectName/d" $fileToSave
		fi
	fi
}

# Print file
function showAllInfo(){
	echo
	echo "List of active project:"
	echo
	if [ $(existFile) -eq 1 ]; then
		cat $fileToSave
	else
		echo "No info to show!"
	fi
	echo
}

### MAIN ###
case "$1" in
	"save")
		saveInfo
		;;
	"delete")
		deleteInfo
		;;
	"show")
		showAllInfo
		;;
esac
