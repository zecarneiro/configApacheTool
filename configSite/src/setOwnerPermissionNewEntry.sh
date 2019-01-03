#!/bin/bash
# Author: José M. C. Noronha

declare nameDir
declare -i isDir="0"
declare isDirKey="ISDIR"
declare wwwPath="$(echo $_WWW_PATH)"
if [ ! -z $1 ]; then
    wwwPath="$1"
fi
echo "$wwwPath"
if [ ! -z $wwwPath ]; then
	inotifywait -m -e create "$wwwPath" | while read DIR; do
		isDir="$(echo "$DIR" | grep -ci $isDirKey)"
		if [ $isDir -gt 0 ]; then
			nameDir="$wwwPath/$(echo "$DIR" | awk '{print $3}')"
			echo "$nameDir"
			sudo chmod -R 755 "$nameDir"
			sudo chown -R :www-data "$nameDir"
		fi
	done
fi