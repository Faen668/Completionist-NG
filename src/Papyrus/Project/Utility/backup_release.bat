@echo off

call variables.cmd

::Delete 'Backup' folder and create a new one.
rmdir "%modpath%\backup" /s /q
mkdir "%modpath%\backup"
mkdir "%modpath%\backup\Release"
mkdir "%modpath%\backup\Project"

::Copy over the project files
XCOPY "%modpath%\Utility" "%modpath%\backup\Project\Utility\" /e /s /y

::Copy over the mod files
XCOPY "%modpath%\Interface\Translations\*.txt" "%modpath%\backup\release\Interface\Translations\" /e /s /y
XCOPY "%modpath%\Scripts\Source\" "%modpath%\backup\release\Scripts\Source\" /e /s /y
XCOPY "%modpath%\SKSE" "%modpath%\backup\release\SKSE\" /e /s /y /EXCLUDE:exclude.txt
XCOPY "%modpath%\Completionist.esp" "%modpath%\backup\release\" /y

::Delete Files that should not be backed up
rm 
::Delete Local Papyrus Folder
rmdir "%backupPath%\Papyrus" /s /q
mkdir "%backupPath%\Papyrus"

::Copy backup then remove
XCOPY "%modpath%\backup" "%backupPath%\Papyrus" /e /s /y
rmdir "%modpath%\backup" /s /q