Scriptname Completionist_APIScript extends Quest

Import Game
Import Debug
Import Utility
Import Completionist_Native

Completionist_MCMScript Property CompMCM Auto
Completionist_MCMScript2 Property CompMCM2 Auto
Completionist_UpdateScript Property CompVer Auto

Perk Property Completionist_ShrineWatcher Auto

GlobalVariable Property Completionist_Busy Auto

;Notification Properties
Bool Property bSetupFinished Auto Hidden

;---------------------------------------------------
;-- Events -----------------------------------------
;---------------------------------------------------

Event OnInit()

	Completionist_Busy.SetValue(1)
	CompVer.DoVersioning()
	RegisterForSingleUpdate(1)
endEvent

;---------------------------------------------------
;-- Events -----------------------------------------
;---------------------------------------------------

Event OnUpdate()
		
	Game.GetPlayer().AddPerk(Completionist_ShrineWatcher)
	SetFrameworkQuest(CompMCM as quest)
	Framework_UpdateShouts()
	
	if (!bSetupFinished)
		bSetupFinished = True
		Completionist_Busy.SetValue(0)
		SendNotification(CompMCM.notificationTextStartup1 + CompVer.ModVersion + CompMCM.notificationTextStartup2, CompMCM.ColourString, CompMCM.NotificationColourEnabled)
		return
	endif
	
	Maintenance()
endEvent
		
;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function Maintenance()
	
	CompVer.CheckVersioning()
	
	CompMCM.ResetMarkerOnLoad()
	CompMCM2.SetLocationMarker()
	Completionist_Busy.SetValue(0)
	
	if (CompMCM.NotifyStartup)
		SendNotification(CompMCM.notificationTextStartup0, CompMCM.ColourString, CompMCM.NotificationColourEnabled)
	endif
endFunction

;---------------------------------------------------
;-- END OF CODE ------------------------------------
;---------------------------------------------------