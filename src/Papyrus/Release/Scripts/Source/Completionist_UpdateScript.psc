Scriptname Completionist_UpdateScript extends Quest

Import Debug
Import Utility
Import Completionist_Native

Completionist_MCMScript Property CompMCM Auto
Completionist_APIScript Property CompAPI Auto

Float Property fSKSE Auto Hidden
 
Int Property fVersion Auto Hidden 
Int Property fVMajor Auto Hidden 
Int Property fVMinor Auto Hidden
Int Property fVPatch Auto Hidden
Int Property fVTweak Auto Hidden

Int curVersion
Int curVMajor
Int curVMinor
Int curVPatch
Int curVTweak

String Property ModVersion Auto Hidden

Bool bVersioningDone
Bool bUpdated

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

function GetSKSEVersion()

	fSKSE = (SKSE.GetVersion() * 10000 + SKSE.GetVersionMinor() * 100 + SKSE.GetVersionBeta())
EndFunction	

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function DoVersioning() ;Initial versioning on a new game.
	
	if (bVersioningDone)
		return
	endif
	
	GetSKSEVersion()
	fVMajor = 4
	fVMinor = 1
	fVPatch = 0
	fVTweak = 8
	fVersion = (fVMajor * 1000) + (fVMinor * 100) + (fVPatch * 10) + (fVTweak)
	ModVersion = (fVMajor + "." + fVMinor + "." + fVPatch + "." + fVTweak)
	
	bVersioningDone = True
	LogWithPlugin("Completionist UDS - Initial Versioning Completed")	
endFunction	

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function CheckVersioning() ;Versioning ran from OnPlayerLoadGame()
	
	GetSKSEVersion()
	curVMajor = 4
	curVMinor = 1
	curVPatch = 0
	curVTweak = 8
	curVersion = (curVMajor * 1000) + (curVMinor * 100) + (curVPatch * 10) + (curVTweak)
	
	While IsInMenuMode()
		Wait(0.1)
	endWhile

	if (fVersion < curVersion)
		SendNotification(CompMCM.notificationTextUpdate0, CompMCM.ColourString, CompMCM.NotificationColourEnabled)
		UpdateKicker()
	endif
endFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function UpdateKicker()
	
	bUpdated = False

	if (fVersion < 4102)
		MessageBox("=== Completionist ===\nVersion 4.1.0.2 has undergone extensive changes to the patch framework\nPlease start a new game or alternatively make a manual save, exit the game and clean your save file with 'Resaver' to ensure the mod still works correctly.")
	endif

	if (fVersion < 4108)
		fVMajor = curVMajor
		fVMinor = curVMinor
		fVPatch = curVPatch
		fVTweak = curVTweak
		fVersion = curVersion
		bUpdated = True
	endif
	
	if (!bUpdated)
		Trace("=============== Completionist Update Handler - Update Failed ===============")	
		SendNotification(CompMCM.notificationTextUpdate1, CompMCM.ColourString, CompMCM.NotificationColourEnabled)
		return
	endif

	ModVersion = (fVMajor + "." + fVMinor + "." + fVPatch + "." + fVTweak)
	Trace("=============== Completionist Update Handler - Updated to Version " + fVMajor + "." + fVMinor + "." + fVPatch + "." + fVTweak + " ===============")	
	SendNotification(CompMCM.notificationTextUpdate2 + ModVersion, CompMCM.ColourString, CompMCM.NotificationColourEnabled)
endFunction
	
;---------------------------------------------------
;-- END OF CODE ------------------------------------
;---------------------------------------------------		