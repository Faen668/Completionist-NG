Scriptname Completionist_QuestsScript extends Quest  

Import Debug
Import Utility
Import Completionist_Native

Completionist_MCMScript Property CompMCM Auto
Completionist_APIScript Property CompAPI Auto

;Civil War Siege / Fort Quests - All Handled By Plugin.
GlobalVariable Property CWPlayerAllegiance Auto

Int Property CurrentQuestID = -1 Auto Hidden

String[] TNA
String[] TTA
String[] TIA
String[] TKA
Int[] TRA
	
;;---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
;;---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Function GetArrays(string mcmpage)
	TNA = qGetNameArrayByname(mcmpage)
	TTA = qGetTextArrayByname(mcmpage)
	TIA = qGetIdenArrayByname(mcmpage)
	TRA = qGetRadiArrayByname(mcmpage)
	TKA = qGetKeysArrayByname(mcmpage)
endFunction

;;---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
;;---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Function FillArrays(String a_name, String a_text, String a_edit, Int a_type, String a_keys)
	
	CompMCM.NameArray[CompMCM.OptionIndx] = a_name
	CompMCM.TextArray[CompMCM.OptionIndx] = a_text
	CompMCM.IdenArray[CompMCM.OptionIndx] = a_edit
	CompMCM.RadiArray[CompMCM.OptionIndx] = a_type
	CompMCM.KeysArray[CompMCM.OptionIndx] = a_keys
	CompMCM.OptionIndx += 1
endFunction
			
;;---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
;;---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Function _Build_Quests(string mcmpage)
	
	Int Index = 0
	CurrentQuestID = GetMCMPageIdentifierFromName(mcmpage)
	CompMCM.LoadArrays()

;;---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
;;---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	if mcmpage == "$MCMPageMainCW" 				;--------------- Civil War Quests	
		
		if (CWPlayerAllegiance.GetValue() == 0)
			CompMCM.ShowMessage("$PickFactionErrorMSGCW", False, "$ConfirmY", 0)
			return
		endif
		
		GetArrays(mcmpage)

		if (CWPlayerAllegiance.GetValue() == 1)
			Index = 0
			While Index < 16
				FillArrays(TNA[Index], TTA[Index], TIA[Index], TRA[Index], TKA[Index])
				Index += 1
			endWhile
		endIf

		if (CWPlayerAllegiance.GetValue() == 2)
			Index = 16
			While Index < TNA.length
				FillArrays(TNA[Index], TTA[Index], TIA[Index], TRA[Index], TKA[Index])
				Index += 1
			endWhile
		endIf		
		
		CompMCM.LoadPage()

;;---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
;;---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	elseif mcmpage == "$MCMPageMainDG" 			;--------------- Dawnguard Main Quests			

		if (CompMCM.DG_Faction_Choice == 0)
			CompMCM.ShowMessage("$PickFactionErrorMSG", False, "$ConfirmY", 0)
			return
		endif

		GetArrays(mcmpage)
		
		Index = 0
		While Index < TNA.length
			if Index == 3
				if CompMCM.DG_Faction_Choice == 1
					FillArrays(TNA[3], TTA[3], TIA[3], TRA[3], TKA[3])
					FillArrays(TNA[4], TTA[4], TIA[4], TRA[4], TKA[4])
					Index = 6
				elseif CompMCM.DG_Faction_Choice == 2
					FillArrays(TNA[5], TTA[5], TIA[5], TRA[5], TKA[5])
					FillArrays(TNA[6], TTA[6], TIA[6], TRA[6], TKA[6])
					Index = 6
				endif
			else
				FillArrays(TNA[Index], TTA[Index], TIA[Index], TRA[Index], TKA[Index])
			endif
			Index += 1
		endWhile
		
		CompMCM.LoadPage()

;;---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
;;---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	elseif mcmpage == "$MCMPageMuseum" 			;--------------- Museum Quests	
		
		if (CompMCM.Legacy_Faction_Choice == 0)
			CompMCM.ShowMessage("$PickFactionErrorMSG", False, "$ConfirmY", 0)
			return
		endif

		GetArrays(mcmpage)
		
		if CompMCM.Legacy_Faction_Choice == 1 || (Game.GetModByName("Alternate Start - Live Another Life.esp") == 255 && Game.GetModByName("AlternatePerspective.esp") == 255)
			FillArrays(TNA[00], TTA[00], TIA[00], TRA[00], TKA[00])
												  
		elseif CompMCM.Legacy_Faction_Choice == 2 
			FillArrays(TNA[01], TTA[01], TIA[01], TRA[01], TKA[01])
												  
		elseif CompMCM.Legacy_Faction_Choice == 3 
			FillArrays(TNA[02], TTA[02], TIA[02], TRA[02], TKA[02])
			FillArrays(TNA[03], TTA[03], TIA[03], TRA[03], TKA[03])
			FillArrays(TNA[04], TTA[04], TIA[04], TRA[04], TKA[04])
		endIf

		Index = TIA.Find("DBM_HauntedMuseumQuest")
		While Index < TNA.length
			FillArrays(TNA[Index], TTA[Index], TIA[Index], TRA[Index], TKA[Index])
			Index += 1
		endWhile

		CompMCM.LoadPage()

;;---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
;;---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	else
		if (!IsSettingsPage(mcmpage))
			CompMCM.NameArray = qGetNameArrayByname(mcmpage)
			CompMCM.TextArray = qGetTextArrayByname(mcmpage)
			CompMCM.IdenArray = qGetIdenArrayByname(mcmpage)
			CompMCM.RadiArray = qGetRadiArrayByname(mcmpage)
			CompMCM.KeysArray = qGetKeysArrayByname(mcmpage)
			CompMCM.OptionIndx = CompMCM.KeysArray.length
			CompMCM.LoadPage()
		endif
	endif
endFunction

;---------------------------------------------------
;-- END OF CODE ------------------------------------
;---------------------------------------------------/;