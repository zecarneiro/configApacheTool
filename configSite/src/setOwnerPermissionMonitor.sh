#!/bin/bash
# Author: José M. C. Noronha

declare wwwPath="$(echo $_WWW_PATH)"
declare pwdPath="$(echo $PWD)"

declare fileLogService=".logService"
declare nameOwnerServer="www-data"
declare isGitDir=".git"

# Main
function main(){
    local nameDirFile
    local pathDirFile

    # Get www Path
    if [ -z $wwwPath ]||[ "$wwwPath"="" ]; then
        wwwPath="$pwdPath"
    fi

    # Init
    if [ ! -z $wwwPath ]; then
        # Log working directory
        echo "Working Directory: $wwwPath" | sudo tee $wwwPath/$fileLogService > /dev/null

        # Monitor
        inotifywait -m -r -e create "$wwwPath" | while read DirFile; do
            isGitDir="$(echo "$DirFile" | grep -ci $isGitDir)"
            if [ $isGitDir -le 0 ]; then
                pathDirFile="$(echo "$DirFile" | awk '{print $1}')"
                nameDirFile="$pathDirFile$(echo "$DirFile" | awk '{print $3}')"
                echo "$nameDirFile" | tee -a $wwwPath/$fileLogService > /dev/null
                sudo chmod -R 755 "$nameDirFile"
                sudo chown -R :$nameOwnerServer "$nameDirFile"
            fi
        done
    fi
}
main