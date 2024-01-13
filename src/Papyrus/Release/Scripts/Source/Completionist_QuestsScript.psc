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

	elseif mcmpage == "$MCMPageHelgen" 			;--------------- Helgen Reborn Quests	

		if (CompMCM.HR_Faction_Choice == 0)
			CompMCM.ShowMessage("$PickFactionErrorMSG", False, "$ConfirmY", 0)
			return
		endif

		GetArrays(mcmpage)

		Index = 0
		While Index < 7
			FillArrays(TNA[Index], TTA[Index], TIA[Index], TRA[Index], TKA[Index])
			Index += 1
		endWhile
		
		If CompMCM.HR_Faction_Choice == 1
			FillArrays(TNA[07], TTA[07], TIA[07], TRA[07], TKA[07])
			FillArrays(TNA[08], TTA[08], TIA[08], TRA[08], TKA[08])
			FillArrays(TNA[09], TTA[09], TIA[09], TRA[09], TKA[09])
			FillArrays(TNA[10], TTA[10], TIA[10], TRA[10], TKA[10])
			FillArrays(TNA[11], TTA[11], TIA[11], TRA[11], TKA[11])
			FillArrays(TNA[12], TTA[12], TIA[12], TRA[12], TKA[12])

		elseif CompMCM.HR_Faction_Choice == 2    
			FillArrays(TNA[13], TTA[13], TIA[13], TRA[13], TKA[13])
			FillArrays(TNA[14], TTA[14], TIA[14], TRA[14], TKA[14])
			FillArrays(TNA[15], TTA[15], TIA[15], TRA[15], TKA[15])
			FillArrays(TNA[16], TTA[16], TIA[16], TRA[16], TKA[16])
			FillArrays(TNA[17], TTA[17], TIA[17], TRA[17], TKA[17])
			FillArrays(TNA[18], TTA[18], TIA[18], TRA[18], TKA[18])
			
		elseif CompMCM.HR_Faction_Choice == 3                                                                                                                     
			FillArrays(TNA[19], TTA[19], TIA[19], TRA[19], TKA[19])
			FillArrays(TNA[20], TTA[20], TIA[20], TRA[20], TKA[20])
			FillArrays(TNA[21], TTA[21], TIA[21], TRA[21], TKA[21])
			FillArrays(TNA[22], TTA[22], TIA[22], TRA[22], TKA[22])
			FillArrays(TNA[23], TTA[23], TIA[23], TRA[23], TKA[23])
			FillArrays(TNA[24], TTA[24], TIA[24], TRA[24], TKA[24])
		endIf

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