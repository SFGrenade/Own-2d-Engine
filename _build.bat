@ECHO OFF

VERIFY OTHER 2>nul
SETLOCAL ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION
IF NOT ERRORLEVEL 0 (
  echo Unable to enable extensions
)

FOR /F "delims=" %%A IN ('cd') DO SET "ORIGINAL_DIR=%%A"
ECHO orig dir: %ORIGINAL_DIR%

SET "logFolder=.\_build_logs"

GOTO :main

:doCommand
SET "logFile=%logFolder%\%~1.log"
SET "command=%~2"
ECHO %command%>"%logFile%" 2>&1
%command%>>"%logFile%" 2>&1
EXIT /B %ERRORLEVEL%

:main

RMDIR /S /Q %logFolder%

MKDIR %logFolder%

CALL :doCommand "00_made_build_logs" "echo we did it" && cd>NUL || Goto :END

CALL :doCommand "01_xmake_set_theme" "xmake global --theme=plain" && cd>NUL || Goto :END

REM CALL :doCommand "02_xmake_configure" "xmake config --plat=windows --arch=x64 --kind=shared --mode=debug --runtimes=MD --policies=package.precompiled:n -vD -y" && cd>NUL || Goto :END
REM CALL :doCommand "02_xmake_configure" "xmake config --plat=windows --arch=x64 --kind=shared --mode=debug --runtimes=MT --policies=package.precompiled:n -vD -y" && cd>NUL || Goto :END
REM CALL :doCommand "02_xmake_configure" "xmake config --plat=windows --arch=x64 --kind=shared --mode=release --runtimes=MD --policies=package.precompiled:n -vD -y" && cd>NUL || Goto :END
REM CALL :doCommand "02_xmake_configure" "xmake config --plat=windows --arch=x64 --kind=shared --mode=release --runtimes=MT --policies=package.precompiled:n -vD -y" && cd>NUL || Goto :END
REM CALL :doCommand "02_xmake_configure" "xmake config --plat=windows --arch=x64 --kind=static --mode=debug --runtimes=MD --policies=package.precompiled:n -vD -y" && cd>NUL || Goto :END
REM CALL :doCommand "02_xmake_configure" "xmake config --plat=windows --arch=x64 --kind=static --mode=debug --runtimes=MT --policies=package.precompiled:n -vD -y" && cd>NUL || Goto :END
REM CALL :doCommand "02_xmake_configure" "xmake config --plat=windows --arch=x64 --kind=static --mode=release --runtimes=MD --policies=package.precompiled:n -vD -y" && cd>NUL || Goto :END
CALL :doCommand "02_xmake_configure" "xmake config --plat=windows --arch=x64 --kind=static --mode=release --runtimes=MT --policies=package.precompiled:n -vD -y" && cd>NUL || Goto :END

CALL :doCommand "03_xmake_build" "xmake build -a -vD" && cd>NUL || Goto :END

CALL :doCommand "05_xmake_test_Configuration-Test" "xmake run -vD Configuration-Test" && cd>NUL || Goto :END
CALL :doCommand "05_xmake_test_Engine-Test" "xmake run -vD Engine-Test" && cd>NUL || Goto :END
CALL :doCommand "05_xmake_test_Logger-Test" "xmake run -vD Logger-Test" && cd>NUL || Goto :END
CALL :doCommand "05_xmake_test_Network-Messages-Test" "xmake run -vD Network-Messages-Test" && cd>NUL || Goto :END
CALL :doCommand "05_xmake_test_Utils-Test" "xmake run -vD Utils-Test" && cd>NUL || Goto :END

:END
cd %ORIGINAL_DIR%
ENDLOCAL
EXIT /B %ERRORLEVEL%
