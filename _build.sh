#!/bin/bash

ORIGINAL_DIR=$(pwd)
SCRIPT_DIR=$(dirname "${BASH_SOURCE[0]}")

cd "$SCRIPT_DIR"

logFolder="./_build_logs"

doCommand () {
    logFile="$logFolder/$1.log"
    command="$2"
    touch "$logFile"
    echo "$(pwd)$ $command" >"$logFile" 2>&1
    eval "$command >>\"$logFile\" 2>&1"

    retCode=$?
    if [[ ! $retCode -eq 0 ]]; then
        cd "$ORIGINAL_DIR"
        exit $retCode
    fi
}

rm -r ./_build_logs

mkdir _build_logs

doCommand "00_made_build_logs" "echo \"we did it\""

doCommand "01_xmake_set_theme" "xmake global --theme=plain"

#doCommand "02_xmake_configure" "xmake config -p linux -a x86_64 -m release -k shared -vD -y"
doCommand "02_xmake_configure" "xmake config --import=.vscode/xmakeConfig.release.conf -vD -y"

doCommand "03_xmake_build" "xmake build -a -vD"

#doCommand "05_xmake_test_Configuration-Test" "xmake run -vD Configuration-Test"
#doCommand "05_xmake_test_Logger-Test" "xmake run -vD Logger-Test"
#doCommand "05_xmake_test_Network-Messages-Test" "xmake run -vD Network-Messages-Test"
#doCommand "05_xmake_test_Sdl-Wrapper-Test" "xmake run -vD Sdl-Wrapper-Test"

cd "$ORIGINAL_DIR"
