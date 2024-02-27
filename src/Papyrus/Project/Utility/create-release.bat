@echo off

call variables.cmd

:: Delete 'Backup' folder and create a new one.
rmdir "%modpath%\release" /s /q
mkdir "%modpath%\release"

:: Copy over the release files
XCOPY "%modpath%\Interface" "%modpath%\release\Base\Interface\" /e /s /y || echo Copying Interface failed!
XCOPY "%modpath%\MapMarkers" "%modpath%\release\Base\MapMarkers\" /e /s /y || echo Copying MapMarkers failed!
XCOPY "%modpath%\Scripts" "%modpath%\release\Base\Scripts\" /e /s /y || echo Copying Scripts failed!
XCOPY "%modpath%\SKSE" "%modpath%\release\Base\SKSE\" /e /s /y || echo Copying SKSE failed!
XCOPY "%modpath%\Sound" "%modpath%\release\Base\Sound\" /e /s /y || echo Copying Sound failed!
XCOPY "%modpath%\Completionist.esp" "%modpath%\release\Base\" /y || echo Copying Completionist.esp failed!

:: Copy over the dev kit files
XCOPY "%modpath%\SKSE\Plugins\CompletionistData\Dev Kit" "%modpath%\release\Dev Kit\SKSE\Plugins\CompletionistData\Dev Kit\" /e /s /y || echo Copying Dev Kit failed!

:: Update Translations
copy "%modpath%\release\Base\Interface\Translations\completionist_english.txt" "%modpath%\release\Base\Interface\Translations\completionist_czech.txt"
copy "%modpath%\release\Base\Interface\Translations\completionist_english.txt" "%modpath%\release\Base\Interface\Translations\completionist_french.txt"
copy "%modpath%\release\Base\Interface\Translations\completionist_english.txt" "%modpath%\release\Base\Interface\Translations\completionist_german.txt"
copy "%modpath%\release\Base\Interface\Translations\completionist_english.txt" "%modpath%\release\Base\Interface\Translations\completionist_italian.txt"
copy "%modpath%\release\Base\Interface\Translations\completionist_english.txt" "%modpath%\release\Base\Interface\Translations\completionist_japanese.txt"
copy "%modpath%\release\Base\Interface\Translations\completionist_english.txt" "%modpath%\release\Base\Interface\Translations\completionist_polish.txt"
copy "%modpath%\release\Base\Interface\Translations\completionist_english.txt" "%modpath%\release\Base\Interface\Translations\completionist_russian.txt"
copy "%modpath%\release\Base\Interface\Translations\completionist_english.txt" "%modpath%\release\Base\Interface\Translations\completionist_spanish.txt"

:: Delete Unneeded Files
rmdir /s /q "%modpath%\release\Base\SKSE\Plugins\CompletionistData\Dev Kit"
del /s /q "%modpath%\release\Base\Interface\constructibleobjectmenu.swf"
del /s /q "%modpath%\release\Base\Interface\craftingmenu.swf"
del /s /q "%modpath%\release\Base\Interface\Translations\Update.bat"
del /s /q "%modpath%\release\Base\Scripts\ccbgssse001_fishingsystemscript.pex"
del /s /q "%modpath%\release\Base\Scripts\Source\ccbgssse001_fishingsystemscript.psc"