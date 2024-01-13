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

ReferenceAlias Property Pet_Spider Auto
ReferenceAlias Property Pet_Goat Auto
ReferenceAlias Property Pet_Skeever Auto
ReferenceAlias Property Pet_Fox Auto
ReferenceAlias Property Pet_Rabbit Auto
ReferenceAlias Property Pet_Dement Auto
ReferenceAlias Property Pet_Manic Auto
ReferenceAlias Property Pet_Mudcrab Auto
ReferenceAlias Property Pet_Wolf Auto
ReferenceAlias Property Pet_Hound Auto

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
		ForcePetRefs()
		LoadInjectedForms()
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
	
	ForcePetRefs()
	LoadInjectedForms()
	
	CompMCM2.SetLocationMarker()
	Completionist_Busy.SetValue(0)
	
	if (CompMCM.NotifyStartup)
		SendNotification(CompMCM.notificationTextStartup0, CompMCM.ColourString, CompMCM.NotificationColourEnabled)
	endif
endFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function ForcePetRefs()

	if (Game.GetModByName("ccvsvsse002-pets.esl") != 255) ; Pets of Skyrim
		Pet_Spider.ForceRefTo	(GetFormFromFile(0x001085E, "ccvsvsse002-pets.esl") as Actor)
		Pet_Goat.ForceRefTo		(GetFormFromFile(0x0010879, "ccvsvsse002-pets.esl") as Actor)
		Pet_Skeever.ForceRefTo	(GetFormFromFile(0x001083C, "ccvsvsse002-pets.esl") as Actor)
		Pet_Fox.ForceRefTo		(GetFormFromFile(0x001087E, "ccvsvsse002-pets.esl") as Actor)
		Pet_Rabbit.ForceRefTo	(GetFormFromFile(0x0010881, "ccvsvsse002-pets.esl") as Actor)
	endif

	if (Game.GetModByName("ccbgssse025-advdsgs.esm") != 255) ; Saints & Seducers
		Pet_Dement.ForceRefTo	(GetFormFromFile(0x001008, "ccbgssse025-advdsgs.esm") as Actor)
		Pet_Manic.ForceRefTo	(GetFormFromFile(0x001005, "ccbgssse025-advdsgs.esm") as Actor)
	endif

	if (Game.GetModByName("ccbgssse036-petbwolf.esl") != 255) ; Bone Wolf
		Pet_Wolf.ForceRefTo		(GetFormFromFile(0x000813, "ccbgssse036-petbwolf.esl") as Actor)
	endif
	
	if (Game.GetModByName("ccbgssse010-petdwarvenarmoredmudcrab.esl") != 255) ; Dwarven Armored Mudcrab
		Pet_Mudcrab.ForceRefTo	(GetFormFromFile(0x006801, "ccbgssse010-petdwarvenarmoredmudcrab.esl") as Actor)
	endif

	if (Game.GetModByName("ccbgssse035-petnhound.esl") != 255) ; Nix-Hound
		Pet_Hound.ForceRefTo	(GetFormFromFile(0x00E806, "ccbgssse035-petnhound.esl") as Actor)
	endif
endFunction

;---------------------------------------------------
;-- END OF CODE ------------------------------------
;---------------------------------------------------