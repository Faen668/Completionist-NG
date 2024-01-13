@echo off

call variables.cmd

::Delete 'Backup' folder and create a new one.
rmdir "%modpath%\backup" /s /q
mkdir "%modpath%\backup"
mkdir "%modpath%\backup\Release"
mkdir "%modpath%\backup\Project"

::Copy over the project files
XCOPY "%modpath%\.vscode" "%modpath%\backup\Project\.vscode\" /e /s /y
XCOPY "%modpath%\Utility" "%modpath%\backup\Project\Utility\" /e /s /y
XCOPY "%modpath%\SkyrimSE.code-workspace" "%modpath%\backup\Project\" /y
XCOPY "%modpath%\skyrimse.ppj" "%modpath%\backup\Project\" /y

::Copy over the mod files
XCOPY "%modpath%\Interface" "%modpath%\backup\release\Interface\" /e /s /y
XCOPY "%modpath%\MapMarkers" "%modpath%\backup\release\MapMarkers\" /e /s /y
XCOPY "%modpath%\Scripts" "%modpath%\backup\release\Scripts\" /e /s /y
XCOPY "%modpath%\SKSE" "%modpath%\backup\release\SKSE\" /e /s /y
XCOPY "%modpath%\Completionist.esp" "%modpath%\backup\release\" /y

::Delete Local Papyrus Folder
rmdir "%backupPath%\Papyrus" /s /q
mkdir "%backupPath%\Papyrus"

::Copy backup then remove
XCOPY "%modpath%\backup" "%backupPath%\Papyrus" /e /s /y
rmdir "%modpath%\backup" /s /q