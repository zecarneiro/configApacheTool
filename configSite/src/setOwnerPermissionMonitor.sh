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

    # Delete Log File
    if [ -f "$wwwPath/$fileLogService" ]; then
        sudo rm "$wwwPath/$fileLogService"
    fi

    # Init
    if [ ! -z $wwwPath ]; then
        # Log working directory
        echo "Working Directory: $wwwPath" | sudo tee "$wwwPath/$fileLogService" > /dev/null

        if [ -f "$wwwPath/$fileLogService" ]; then
            sudo chmod 777 "$wwwPath/$fileLogService"
        fi

        # Monitor
        inotifywait -m -r -e create --format '%w%f' "$wwwPath" | while read DirFile; do
            isGitDir="$(echo "$DirFile" | grep -ci $isGitDir)"
            if [ $isGitDir -le 0 ]; then
                # Log to file
                # echo "$DirFile" | sudo tee -a "$wwwPath/$fileLogService" > /dev/null

                # Exec operation
                sudo chmod -R 755 "$DirFile"
                sudo chown -R :$nameOwnerServer "$DirFile"
            fi
        done
    fi
}
main