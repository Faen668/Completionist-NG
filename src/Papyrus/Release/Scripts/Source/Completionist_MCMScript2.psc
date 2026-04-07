Scriptname Completionist_MCMScript2 extends SKI_ConfigBase  
Import Completionist_Native

;-- Variables ---------------------------------------

Completionist_MCMScript Property CompMCM Auto

GlobalVariable Property Completionist_Busy Auto

Int PagePIndex = 0

Int OptionIndx
Int[] OptionSlot
Int[] OptionfrID
Form[] OptionForm

String[] OptionName
String[] OptionHigh
ObjectReference[] OptionObjR

ObjectReference Property CompletionistLocationString Auto
ReferenceAlias Property LocAliasName Auto

ObjectReference Property LocMarker Auto
ReferenceAlias Property LocAlias Auto
Quest Property LocQuest Auto

Int FrameworkLocationID = 355685442

String Property CurMarker Auto Hidden
ObjectReference Property CurLocation Auto Hidden

String[] NameArray_A
String[] TextArray_A
Form[] FormArray_A
Bool[] BoolArray_A
Int EntriesFound_A
Int EntriesTotal_A

String[] NameArray_B
String[] TextArray_B
Form[] FormArray_B
Bool[] BoolArray_B
Int EntriesFound_B
Int EntriesTotal_B

String[] NameArray_C
String[] TextArray_C
Form[] FormArray_C
Bool[] BoolArray_C
Int EntriesFound_C
Int EntriesTotal_C

String[] NameArray_D
String[] TextArray_D
Form[] FormArray_D
Bool[] BoolArray_D
Int EntriesFound_D
Int EntriesTotal_D

String[] NameArray_E
String[] TextArray_E
Form[] FormArray_E
Bool[] BoolArray_E
Int EntriesFound_E
Int EntriesTotal_E

String[] NameArray_F
String[] TextArray_F
Form[] FormArray_F
Bool[] BoolArray_F
Int EntriesFound_F
Int EntriesTotal_F

;---------------------------------------------------
;-- START OF CODE ----------------------------------
;---------------------------------------------------

Event OnConfigInit()	
	pages = GetMCMPages(1)
endevent

Event OnConfigOpen()
	
	if (CompMCM.bShortCutActive) && (CompMCM.MiscJumpPage != "")
		if (ShowMessage("$JumpMessage{" + CompMCM.MiscJumpPage + "}", true, "$ConfirmYes", "$ConfirmNo"))
			Jump(CompMCM.MiscJumpPage, 0)
		endif
	endif	
EndEvent

;---------------------------------------------------
;-- Events -----------------------------------------
;---------------------------------------------------

Function Jump(string sString = "", int jumpType = 0)

	if (sString != "")
		SetTitleText(sString)
	endif
	
	if (jumpType == 0)
		SendModEvent("SKICP_pageSelected", CompMCM.MiscJumpPage, pages.Find(CompMCM.MiscJumpPage))
	endif

	if (jumpType == 1)
		SendModEvent("SKICP_pageSelected", CompMCM.SearchJumpPage, pages.Find(CompMCM.SearchJumpPage))
	endif
	
	if (jumpType == 2)
		SendModEvent("SKICP_pageSelected", currentpage, pages.Find(currentpage))
	endif
endFunction

;---------------------------------------------------
;-- Events -----------------------------------------
;---------------------------------------------------

event OnPageReset(string page)
	
	pages = GetMCMPages(1)
	
	OptionIndx = 0
	OptionSlot = New Int[128]
	OptionfrID = New Int[128]
	OptionForm = New Form[128]
	OptionName = New String[128]
	OptionHigh = New String[128]
	OptionObjR = New ObjectReference[128]

	if (!Completionist_Busy.GetValue())

		if (CurrentPage == "$MCMPageKillCounts")
			GoToState("KillCounts")
			BuildKillCountPage()
			return
		endif

		Select_Page()
	else
		self.ShowMessage("$BusyMessage", false, "$ConfirmY")
	endif
endevent
	
;---------------------------------------------------
;-- Events -----------------------------------------
;---------------------------------------------------

Event OnOptionHighlight(Int val)
	string baseText = GetHighlightText(val)
	string displayStatus = GetMuseumDisplayStatus(Get_Option_Form(val))

	if (CompMCM.MuseumModeEnabled && displayStatus != "")
		if (baseText != "")
			SetInfoText(baseText + "\nMuseum Display Status: " + displayStatus)
		else
			SetInfoText("~~~ " + Get_Option_Name(val) + " ~~~" + "\nMuseum Display Status: " + displayStatus)
		endif
	else
		SetInfoText(baseText)
	endif
endEvent

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function BuildKillCountPage()
	SetCursorFillMode(LEFT_TO_RIGHT)
	OptionIndx = 0

	AddHeaderOption("Kill Counts")
	AddHeaderOption("")

	string[] kills = GetPlayerKillNames()

	if (kills.length == 0)
		AddTextOption("Nothing to see yet...", "", 0)
		AddTextOption("Go murder something!", "", 0)
		return
	endif

	int Idx = 0
	while Idx < kills.length
		OptionSlot[OptionIndx] = AddTextOption(kills[Idx], GetCombinedKillString(kills[Idx]), 0)
		OptionName[OptionIndx] = kills[Idx]
		Idx += 1
		OptionIndx += 1
	endwhile
endFunction

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State KillCounts

	Event OnOptionSelect(Int val)
		String Name = Get_Option_Name(val)
		
		if (Name == "")
			return
		endif

		if (GetPlayerKillCount(Name) > 0)
			if ShowMessage("$ResetKill{" + Name + "}", True, "$ConfirmY", "$ConfirmN")
				ResetPlayerKill(Name)
				ForcePageReset()
			endif
		else
			if ShowMessage("$RemoveKill{" + Name + "}", True, "$ConfirmY", "$ConfirmN")
				RemovePlayerKill(Name)
				ForcePageReset()
			endif
		endif
	endEvent

	Event OnOptionHighlight(Int val)
		String Name = Get_Option_Name(val)
	
		if (Name == "")
			return
		endif
		SetInfoText(GetDeathString(Name))
	endevent
endState

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function InsertPageBreak()

	if (PagePIndex % 2 != 0)
		PagePIndex += 1
	endif
	
	AddEmptyOption()
	PagePIndex += 1
	AddEmptyOption()
	PagePIndex += 1
endFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function SetLocationMarker()
	
	if (CurMarker != "") && (CurLocation)
		LocAlias.Clear()
		LocAliasName.Clear()
		
		CompletionistLocationString.GetBaseObject().SetName(CurMarker)
		LocAliasName.ForceRefTo(CompletionistLocationString)
		LocAlias.ForceRefTo(LocMarker)
		(LocMarker as Completionist_CancelMarker).RegisterForSingleUpdate(1)
	endif
endfunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

String function GetHighlightText(Int val)
	
	Int Index = OptionSlot.Find(val)
	if (Index != -1)
		Return OptionHigh[Index]
	endif
		
	Return ""
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

String function Get_Option_Name(Int val)
	
	Int Index = OptionSlot.Find(val)
	if Index != -1
		Return OptionName[Index]
	endif
		
	Return ""
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Form function Get_Option_Form(Int val)
	
	Int Index = OptionSlot.Find(val)
	if Index != -1
		Return OptionForm[Index]
	endif
		
	Return None
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Int function Get_Option_frID(Int val)
	
	Int Index = OptionSlot.Find(val)
	if (Index != -1)
		Return OptionfrID[Index]
	endif
		
	Return -1
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

ObjectReference function Get_Option_Location(Int val)
	
	Int Index = OptionSlot.Find(val)
	if (Index != -1) && (OptionObjR[Index])
		Return OptionObjR[Index]
	endif
		
	Return None
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function PopulatePage(form[] a_forms, string[] a_names, string[] a_texts, bool[] a_bools, int a_ida, int a_type)

	Int idx = 0
				
	while idx < a_names.Length
		if (a_names[idx] != "")
			SetCursorPosition(PagePIndex)

			if (a_names[idx] == CompMCM.SearchedEntry || (CompMCM.SearchJumpPage == "$MCMPageShouts" && IsMatchingShout(a_names[idx], CompMCM.SearchedEntry)))
				CompMCM.SearchedEntry = ""
				CompMCM.SearchJumpPage = ""
				CompMCM.SearchedMCMMenu = ""				 
				
				if (CompMCM.b_SearchHighlightQuest)

					if (!a_bools[idx] || CurrentPage == "$MCMPageShouts")
						OptionSlot[OptionIndx] = AddTextOption("$SearchResult{" + "#FFD966" + "}{" + a_names[idx] + "}", "", 0)
					else
						OptionSlot[OptionIndx] = AddToggleOption("$SearchResult{" + "#FFD966" + "}{" + a_names[idx] + "}", True, 0)
					endif
				else
					AddToPages(a_names[idx], a_forms[idx], a_type, a_bools[idx])
				endif
			else
				AddToPages(a_names[idx], a_forms[idx], a_type, a_bools[idx])
			endif

			OptionName[OptionIndx] = a_names[idx]
			OptionForm[OptionIndx] = a_forms[idx]
			OptionfrID[OptionIndx] = a_ida
			
			if (a_texts[idx] != "NO_HIGHLIGHT")
				OptionHigh[OptionIndx] = a_texts[idx]
			endif

			if (a_type == 1)
				OptionObjR[OptionIndx] = (a_forms[idx] as ObjectReference)
				OptionfrID[OptionIndx] = FrameworkLocationID
			endif
			
			OptionIndx += 1
			PagePIndex += 1
		endif
		idx += 1
	endWhile
endFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function AddToPages(String sString, form a_form, int isLocation, bool a_bool = false)
	
	if ( isLocation == 1 && MapMarkerIsCleared(a_form) )
		sString = sString + " " + GetLocStringByKeyExt("MapMarkerCleared")
	endif
				
	if (!a_bool)	
		OptionSlot[OptionIndx] = AddTextOption(sString, "", 0)
		return
	endif
	
	if (CurrentPage == "$MCMPageShouts")
		OptionSlot[OptionIndx] = AddToggleOption(sString, True, 1)
		
	elseif (CompMCM.State_MiscCompletionView_Choice == 0)
		OptionSlot[OptionIndx] = AddToggleOption("<font color='" + CompMCM.State_MiscCompletionColourString + "'>" + sString + "</font>", True, 0)

	elseif (CompMCM.State_MiscCompletionView_Choice == 1)
		OptionSlot[OptionIndx] = AddToggleOption(sString, True, 0)
		
	elseif (CompMCM.State_MiscCompletionView_Choice == 2)
		OptionSlot[OptionIndx] = AddTextOption("<font color='" + CompMCM.State_MiscCompletionColourString + "'>" + sString + "</font>", "", 0)
	endif
endFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function LoadArrays(int a_frameworkID_a = -1, int a_frameworkID_b = -1, int a_frameworkID_c = -1, int a_frameworkID_d = -1, int a_frameworkID_e = -1, int a_frameworkID_f = -1)	
	
	if (a_frameworkID_a != -1)
		FormArray_A = GetFormArrayByID(a_frameworkID_a)
		NameArray_A = GetNameArrayByID(a_frameworkID_a)
		TextArray_A = GetTextArrayByID(a_frameworkID_a)
		BoolArray_A = GetBoolArrayByID(a_frameworkID_a)
		EntriesFound_A = GetEntries_FoundByID(a_frameworkID_a)
		EntriesTotal_A = GetEntries_TotalByID(a_frameworkID_a)
	endif

	if (a_frameworkID_b != -1)
		FormArray_B = GetFormArrayByID(a_frameworkID_b)
		NameArray_B = GetNameArrayByID(a_frameworkID_b)
		TextArray_B = GetTextArrayByID(a_frameworkID_b)
		BoolArray_B = GetBoolArrayByID(a_frameworkID_b)
		EntriesFound_B = GetEntries_FoundByID(a_frameworkID_b)
		EntriesTotal_B = GetEntries_TotalByID(a_frameworkID_b)
	endif

	if (a_frameworkID_c != -1)
		FormArray_C = GetFormArrayByID(a_frameworkID_c)
		NameArray_C = GetNameArrayByID(a_frameworkID_c)
		TextArray_C = GetTextArrayByID(a_frameworkID_c)
		BoolArray_C = GetBoolArrayByID(a_frameworkID_c)
		EntriesFound_C = GetEntries_FoundByID(a_frameworkID_c)
		EntriesTotal_C = GetEntries_TotalByID(a_frameworkID_c)
	endif

	if (a_frameworkID_d != -1)
		FormArray_D = GetFormArrayByID(a_frameworkID_d)
		NameArray_D = GetNameArrayByID(a_frameworkID_d)
		TextArray_D = GetTextArrayByID(a_frameworkID_d)
		BoolArray_D = GetBoolArrayByID(a_frameworkID_d)
		EntriesFound_D = GetEntries_FoundByID(a_frameworkID_d)
		EntriesTotal_D = GetEntries_TotalByID(a_frameworkID_d)
	endif

	if (a_frameworkID_e != -1)
		FormArray_E = GetFormArrayByID(a_frameworkID_e)
		NameArray_E = GetNameArrayByID(a_frameworkID_e)
		TextArray_E = GetTextArrayByID(a_frameworkID_e)
		BoolArray_E = GetBoolArrayByID(a_frameworkID_e)
		EntriesFound_E = GetEntries_FoundByID(a_frameworkID_e)
		EntriesTotal_E = GetEntries_TotalByID(a_frameworkID_e)
	endif

	if (a_frameworkID_f != -1)
		FormArray_F = GetFormArrayByID(a_frameworkID_f)
		NameArray_F = GetNameArrayByID(a_frameworkID_f)
		TextArray_F = GetTextArrayByID(a_frameworkID_f)
		BoolArray_F = GetBoolArrayByID(a_frameworkID_f)
		EntriesFound_F = GetEntries_FoundByID(a_frameworkID_f)
		EntriesTotal_F = GetEntries_TotalByID(a_frameworkID_f)
	endif
endFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function DisplayPage_1Entry(int a_frameworkID_a, string a_header_a, string a_header_b, int a_loc_a = 0, bool _local = true)
	
	LoadArrays(a_frameworkID_a)

	SetTitleText("~ "  + EntriesFound_A + "/" + EntriesTotal_A + " - " + (((EntriesFound_A as Float / EntriesTotal_A as Float * 100 as Float)  as Int) as String) + "% Completed ~")
	SetCursorPosition(PagePIndex)
	AddHeaderOption(a_header_a)
	PagePIndex += 1
	if (!_local)
		AddHeaderOption(EntriesFound_A + "/" + EntriesTotal_A + " " + a_header_b)
	else
		AddHeaderOption(a_header_b + "{" + EntriesFound_A + "}{" + EntriesTotal_A + "}")
	endif
	PagePIndex += 1	
	
	PopulatePage(FormArray_A, NameArray_A, TextArray_A, BoolArray_A, a_frameworkID_a, a_loc_a)
endfunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function DisplayPage_2Entry(int a_frameworkID_a, int a_frameworkID_b, string a_header_a, string a_header_b, string a_header_c, string a_header_d, int a_loc_a = 0, int a_loc_b = 0, bool _local = true)
	
	LoadArrays(a_frameworkID_a, a_frameworkID_b)
	
	SetTitleText("~ "  + (EntriesFound_A + EntriesFound_B)  + "/" + (EntriesTotal_A + EntriesTotal_B) + " - " + ((((EntriesFound_A + EntriesFound_B) as Float / (EntriesTotal_A + EntriesTotal_B) as Float * 100 as Float)  as Int) as String) + "% Completed ~")
	SetCursorPosition(PagePIndex)
	
	if (FormArray_A) && (FormArray_A[0] != "")
		AddHeaderOption(a_header_a)
		PagePIndex += 1
		if (!_local)
			AddHeaderOption(EntriesFound_A + "/" + EntriesTotal_A + " " + a_header_c)
		else
			AddHeaderOption(a_header_c + "{" + EntriesFound_A + "}{" + EntriesTotal_A + "}")
		endif
		PagePIndex += 1	
		PopulatePage(FormArray_A, NameArray_A, TextArray_A, BoolArray_A, a_frameworkID_a, a_loc_a)
		InsertPageBreak()
	endif

	if (FormArray_B) && (FormArray_B[0] != "")
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_b)
		PagePIndex += 1
		if (!_local)
			AddHeaderOption(EntriesFound_B + "/" + EntriesTotal_B + " " + a_header_d)
		else
			AddHeaderOption(a_header_d + "{" + EntriesFound_B + "}{" + EntriesTotal_B + "}")
		endif
		PagePIndex += 1
		PopulatePage(FormArray_B, NameArray_B, TextArray_B, BoolArray_B, a_frameworkID_b, a_loc_b)
		InsertPageBreak()
	endif
endfunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function DisplayPage_3Entry(int a_frameworkID_a, int a_frameworkID_b, int a_frameworkID_c, string a_header_a, string a_header_b, string a_header_c, string a_header_d, string a_header_e, string a_header_f, int a_loc_a = 0, int a_loc_b = 0, int a_loc_c = 0, bool _local = true)
	
	LoadArrays(a_frameworkID_a, a_frameworkID_b, a_frameworkID_c)
	
	SetTitleText("~ "  + (EntriesFound_A + EntriesFound_B + EntriesFound_C)  + "/" + (EntriesTotal_A + EntriesTotal_B + EntriesTotal_C) + " - " + ((((EntriesFound_A + EntriesFound_B + EntriesFound_C) as Float / (EntriesTotal_A + EntriesTotal_B + EntriesTotal_C) as Float * 100 as Float)  as Int) as String) + "% Completed ~")
	SetCursorPosition(PagePIndex)

	if (FormArray_A) && (FormArray_A[0])
		AddHeaderOption(a_header_a)
		PagePIndex += 1
		if (!_local)
			AddHeaderOption(EntriesFound_A + "/" + EntriesTotal_A + " " + a_header_d)
		else
			AddHeaderOption(a_header_d + "{" + EntriesFound_A + "}{" + EntriesTotal_A + "}")
		endif
		PagePIndex += 1	
		PopulatePage(FormArray_A, NameArray_A, TextArray_A, BoolArray_A, a_frameworkID_a, a_loc_a)
		InsertPageBreak()
	endif

	if (FormArray_B) && (FormArray_B[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_b)
		PagePIndex += 1
		if (!_local)
			AddHeaderOption(EntriesFound_B + "/" + EntriesTotal_B + " " + a_header_e)
		else
			AddHeaderOption(a_header_e + "{" + EntriesFound_B + "}{" + EntriesTotal_B + "}")
		endif
		PagePIndex += 1
		PopulatePage(FormArray_B, NameArray_B, TextArray_B, BoolArray_B, a_frameworkID_b, a_loc_b)
		InsertPageBreak()
	endif

	if (FormArray_C) && (FormArray_C[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_c)
		PagePIndex += 1
		if (!_local)
			AddHeaderOption(EntriesFound_C + "/" + EntriesTotal_C + " " + a_header_f)
		else
			AddHeaderOption(a_header_f + "{" + EntriesFound_C + "}{" + EntriesTotal_C + "}")
		endif
		PagePIndex += 1	
		PopulatePage(FormArray_C, NameArray_C, TextArray_C, BoolArray_C, a_frameworkID_c, a_loc_c)
	endif
endfunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function DisplayPage_4Entry(int a_frameworkID_a, int a_frameworkID_b, int a_frameworkID_c, int a_frameworkID_d, string a_header_a, string a_header_b, string a_header_c, string a_header_d, string a_header_e, string a_header_f, string a_header_g, string a_header_h, int a_loc_a = 0, int a_loc_b = 0, int a_loc_c = 0, int a_loc_d = 0, bool _local = true)
	
	LoadArrays(a_frameworkID_a, a_frameworkID_b, a_frameworkID_c, a_frameworkID_d)
	
	SetTitleText("~ "  + (EntriesFound_A + EntriesFound_B + EntriesFound_C + EntriesFound_D)  + "/" + (EntriesTotal_A + EntriesTotal_B + EntriesTotal_C + EntriesTotal_D) + " - " + ((((EntriesFound_A + EntriesFound_B + EntriesFound_C + EntriesFound_D) as Float / (EntriesTotal_A + EntriesTotal_B + EntriesTotal_C + EntriesTotal_D) as Float * 100 as Float)  as Int) as String) + "% Completed ~")
	SetCursorPosition(PagePIndex)

	if (FormArray_A) && (FormArray_A[0])
		AddHeaderOption(a_header_a)
		PagePIndex += 1
		if (!_local)
			AddHeaderOption(EntriesFound_A + "/" + EntriesTotal_A + " " + a_header_e)
		else
			AddHeaderOption(a_header_e + "{" + EntriesFound_A + "}{" + EntriesTotal_A + "}")
		endif
		PagePIndex += 1	
		PopulatePage(FormArray_A, NameArray_A, TextArray_A, BoolArray_A, a_frameworkID_a, a_loc_a)
		InsertPageBreak()
	endif

	if (FormArray_B) && (FormArray_B[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_b)
		PagePIndex += 1
		if (!_local)
			AddHeaderOption(EntriesFound_B + "/" + EntriesTotal_B + " " + a_header_f)
		else
			AddHeaderOption(a_header_f + "{" + EntriesFound_B + "}{" + EntriesTotal_B + "}")
		endif
		PagePIndex += 1
		PopulatePage(FormArray_B, NameArray_B, TextArray_B, BoolArray_B, a_frameworkID_b, a_loc_b)
		InsertPageBreak()
	endif

	if (FormArray_C) && (FormArray_C[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_c)
		PagePIndex += 1
		if (!_local)
			AddHeaderOption(EntriesFound_C + "/" + EntriesTotal_C + " " + a_header_g)
		else
			AddHeaderOption(a_header_g + "{" + EntriesFound_C + "}{" + EntriesTotal_C + "}")
		endif
		PagePIndex += 1	
		PopulatePage(FormArray_C, NameArray_C, TextArray_C, BoolArray_C, a_frameworkID_c, a_loc_c)
		InsertPageBreak()
	endif

	if (FormArray_D) && (FormArray_D[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_d)
		PagePIndex += 1
		if (!_local)
			AddHeaderOption(EntriesFound_D + "/" + EntriesTotal_D + " " + a_header_h)
		else
			AddHeaderOption(a_header_h + "{" + EntriesFound_D + "}{" + EntriesTotal_D + "}")
		endif
		PagePIndex += 1	
		PopulatePage(FormArray_D, NameArray_D, TextArray_D, BoolArray_D, a_frameworkID_d, a_loc_d)
	endif
endfunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function DisplayPage_5Entry(int a_frameworkID_a, int a_frameworkID_b, int a_frameworkID_c, int a_frameworkID_d, int a_frameworkID_e, string a_header_a, string a_header_b, string a_header_c, string a_header_d, string a_header_e, string a_header_f, string a_header_g, string a_header_h, string a_header_i, string a_header_j, int a_loc_a = 0, int a_loc_b = 0, int a_loc_c = 0, int a_loc_d = 0, int a_loc_e = 0, bool _local = true)
	
	LoadArrays(a_frameworkID_a, a_frameworkID_b, a_frameworkID_c, a_frameworkID_d, a_frameworkID_e)
	
	SetTitleText("~ "  + (EntriesFound_A + EntriesFound_B + EntriesFound_C + EntriesFound_D + EntriesFound_E)  + "/" + (EntriesTotal_A + EntriesTotal_B + EntriesTotal_C + EntriesTotal_D + EntriesTotal_E) + " - " + ((((EntriesFound_A + EntriesFound_B + EntriesFound_C + EntriesFound_D + EntriesFound_E) as Float / (EntriesTotal_A + EntriesTotal_B + EntriesTotal_C + EntriesTotal_D + EntriesTotal_E) as Float * 100 as Float)  as Int) as String) + "% Completed ~")
	SetCursorPosition(PagePIndex)

	if (FormArray_A) && (FormArray_A[0])
		AddHeaderOption(a_header_a)
		PagePIndex += 1
		if (!_local)
			AddHeaderOption(EntriesFound_A + "/" + EntriesTotal_A + " " + a_header_f)
		else
			AddHeaderOption(a_header_f + "{" + EntriesFound_A + "}{" + EntriesTotal_A + "}")
		endif
		PagePIndex += 1	
		PopulatePage(FormArray_A, NameArray_A, TextArray_A, BoolArray_A, a_frameworkID_a, a_loc_a)
		InsertPageBreak()
	endif

	if (FormArray_B) && (FormArray_B[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_b)
		PagePIndex += 1
		if (!_local)
			AddHeaderOption(EntriesFound_B + "/" + EntriesTotal_B + " " + a_header_g)
		else
			AddHeaderOption(a_header_g + "{" + EntriesFound_B + "}{" + EntriesTotal_B + "}")
		endif
		PagePIndex += 1
		PopulatePage(FormArray_B, NameArray_B, TextArray_B, BoolArray_B, a_frameworkID_b, a_loc_b)
		InsertPageBreak()
	endif

	if (FormArray_C) && (FormArray_C[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_c)
		PagePIndex += 1
		if (!_local)
			AddHeaderOption(EntriesFound_C + "/" + EntriesTotal_C + " " + a_header_h)
		else
			AddHeaderOption(a_header_h + "{" + EntriesFound_C + "}{" + EntriesTotal_C + "}")
		endif
		PagePIndex += 1	
		PopulatePage(FormArray_C, NameArray_C, TextArray_C, BoolArray_C, a_frameworkID_c, a_loc_c)
		InsertPageBreak()
	endif

	if (FormArray_D) && (FormArray_D[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_d)
		PagePIndex += 1
		if (!_local)
			AddHeaderOption(EntriesFound_D + "/" + EntriesTotal_D + " " + a_header_i)
		else
			AddHeaderOption(a_header_i + "{" + EntriesFound_D + "}{" + EntriesTotal_D + "}")
		endif
		PagePIndex += 1	
		PopulatePage(FormArray_D, NameArray_D, TextArray_D, BoolArray_D, a_frameworkID_d, a_loc_d)
		InsertPageBreak()
	endif

	if (FormArray_E) && (FormArray_E[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_e)
		PagePIndex += 1
		if (!_local)
			AddHeaderOption(EntriesFound_E + "/" + EntriesTotal_E + " " + a_header_j)
		else
			AddHeaderOption(a_header_j + "{" + EntriesFound_E + "}{" + EntriesTotal_E + "}")
		endif
		PagePIndex += 1	
		PopulatePage(FormArray_E, NameArray_E, TextArray_E, BoolArray_E, a_frameworkID_e, a_loc_e)
	endif
endfunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function DisplayPage_6Entry(int a_frameworkID_a, int a_frameworkID_b, int a_frameworkID_c, int a_frameworkID_d, int a_frameworkID_e, int a_frameworkID_f, string a_header_a, string a_header_b, string a_header_c, string a_header_d, string a_header_e, string a_header_f, string a_header_g, string a_header_h, string a_header_i, string a_header_j, string a_header_k, string a_header_l, int a_loc_a = 0, int a_loc_b = 0, int a_loc_c = 0, int a_loc_d = 0, int a_loc_e = 0, int a_loc_f = 0, bool _local = true)
	
	LoadArrays(a_frameworkID_a, a_frameworkID_b, a_frameworkID_c, a_frameworkID_d, a_frameworkID_e, a_frameworkID_f)
	
	SetTitleText("~ "  + (EntriesFound_A + EntriesFound_B + EntriesFound_C + EntriesFound_D + EntriesFound_E + EntriesFound_F)  + "/" + (EntriesTotal_A + EntriesTotal_B + EntriesTotal_C + EntriesTotal_D + EntriesTotal_E + EntriesTotal_F) + " - " + ((((EntriesFound_A + EntriesFound_B + EntriesFound_C + EntriesFound_D + EntriesFound_E + EntriesFound_F) as Float / (EntriesTotal_A + EntriesTotal_B + EntriesTotal_C + EntriesTotal_D + EntriesTotal_E + EntriesTotal_F) as Float * 100 as Float)  as Int) as String) + "% Completed ~")
	SetCursorPosition(PagePIndex)

	if (FormArray_A) && (FormArray_A[0])
		AddHeaderOption(a_header_a)
		PagePIndex += 1
		if (!_local)
			AddHeaderOption(EntriesFound_A + "/" + EntriesTotal_A + " " + a_header_g)
		else
			AddHeaderOption(a_header_g + "{" + EntriesFound_A + "}{" + EntriesTotal_A + "}")
		endif
		PagePIndex += 1	
		PopulatePage(FormArray_A, NameArray_A, TextArray_A, BoolArray_A, a_frameworkID_a, a_loc_a)
		InsertPageBreak()
	endif

	if (FormArray_B) && (FormArray_B[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_b)
		PagePIndex += 1
		if (!_local)
			AddHeaderOption(EntriesFound_B + "/" + EntriesTotal_B + " " + a_header_h)
		else
			AddHeaderOption(a_header_h + "{" + EntriesFound_B + "}{" + EntriesTotal_B + "}")
		endif
		PagePIndex += 1
		PopulatePage(FormArray_B, NameArray_B, TextArray_B, BoolArray_B, a_frameworkID_b, a_loc_b)
		InsertPageBreak()
	endif

	if (FormArray_C) && (FormArray_C[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_c)
		PagePIndex += 1
		if (!_local)
			AddHeaderOption(EntriesFound_C + "/" + EntriesTotal_C + " " + a_header_i)
		else
			AddHeaderOption(a_header_i + "{" + EntriesFound_C + "}{" + EntriesTotal_C + "}")
		endif
		PagePIndex += 1	
		PopulatePage(FormArray_C, NameArray_C, TextArray_C, BoolArray_C, a_frameworkID_c, a_loc_c)
		InsertPageBreak()
	endif

	if (FormArray_D) && (FormArray_D[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_d)
		PagePIndex += 1
		if (!_local)
			AddHeaderOption(EntriesFound_D + "/" + EntriesTotal_D + " " + a_header_j)
		else
			AddHeaderOption(a_header_j + "{" + EntriesFound_D + "}{" + EntriesTotal_D + "}")
		endif
		PagePIndex += 1	
		PopulatePage(FormArray_D, NameArray_D, TextArray_D, BoolArray_D, a_frameworkID_d, a_loc_d)
		InsertPageBreak()
	endif

	if (FormArray_E) && (FormArray_E[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_e)
		PagePIndex += 1
		if (!_local)
			AddHeaderOption(EntriesFound_E + "/" + EntriesTotal_E + " " + a_header_k)
		else
			AddHeaderOption(a_header_k + "{" + EntriesFound_E + "}{" + EntriesTotal_E + "}")
		endif
		PagePIndex += 1	
		PopulatePage(FormArray_E, NameArray_E, TextArray_E, BoolArray_E, a_frameworkID_e, a_loc_e)
		InsertPageBreak()
	endif

	if (FormArray_F) && (FormArray_F[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_f)
		PagePIndex += 1
		if (!_local)
			AddHeaderOption(EntriesFound_F + "/" + EntriesTotal_F + " " + a_header_l)
		else
			AddHeaderOption(a_header_l + "{" + EntriesFound_F + "}{" + EntriesTotal_F + "}")
		endif
		PagePIndex += 1	
		PopulatePage(FormArray_F, NameArray_F, TextArray_F, BoolArray_F, a_frameworkID_f, a_loc_f)
		InsertPageBreak()
	endif
endfunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

function Select_Page()
	
	PagePIndex = 0
	SetCursorFillMode(LEFT_TO_RIGHT)
	GoToState("Framework_TrackingState")
;---------------------------------------------------
;---------------------------------------------------
		
	if (CurrentPage == "$MCMPageBarenziah")
		DisplayPage_1Entry(0, "$PageHeaderBarenziah1", "$PageHeaderGenericFoundI")
		
;---------------------------------------------------
;---------------------------------------------------
		
	elseif (CurrentPage == "$MCMPageLocations1")
		DisplayPage_1Entry(1, "$PageHeaderPlaces1", "$PageHeaderGenericFoundL", 1)
		
;---------------------------------------------------
;---------------------------------------------------
		
	elseif (CurrentPage == "$MCMPageLocations2")
		DisplayPage_1Entry(2, "$PageHeaderPlaces1", "$PageHeaderGenericFoundL", 1)

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageLocations3")
		DisplayPage_1Entry(3, "$PageHeaderPlaces1", "$PageHeaderGenericFoundL", 1)
	
;---------------------------------------------------
;---------------------------------------------------
		
	elseif (CurrentPage == "$MCMPageLocations4")
		DisplayPage_1Entry(4, "$PageHeaderPlaces1", "$PageHeaderGenericFoundL", 1)
		
;---------------------------------------------------
;---------------------------------------------------
		
	elseif (CurrentPage == "$MCMPageLocations5")
		DisplayPage_1Entry(5, "$PageHeaderPlaces1", "$PageHeaderGenericFoundL", 1)

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageDoomstone")
		DisplayPage_1Entry(6, "$PageHeaderDoomstone1", "$PageHeaderGenericFoundL")
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageShrines")
		DisplayPage_2Entry(7, 8, "$PageHeaderShrines1", "$PageHeaderShrines2", "$PageHeaderGenericFoundL", "$PageHeaderGenericFoundL")	

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageAEnchantments")
		DisplayPage_2Entry(9, 10, "$PageHeaderAEnchantments", "$PageHeaderSEnchantments", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS")
	
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageWEnchantments")
		DisplayPage_2Entry(11, 12, "$PageHeaderWEnchantments", "$PageHeaderSEnchantments", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPagePets")
		DisplayPage_1Entry(13, "$PageHeaderPets1", "$PageHeaderGenericFoundO")
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageHouses")
		DisplayPage_1Entry(14, "$PageHeaderHouses1", "$PageHeaderGenericFoundO")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageShouts")
		GoToState("State_NoSelect")
		DisplayPage_3Entry(15, 16, 17, "$PageHeaderShouts1", "$PageHeaderShouts2","$PageHeaderShouts3", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS")	
	endif
EndFunction	

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

int Function GetIntValue(string value)
	if value == "true"
		return 1
	endif

	return 0
endfunction

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State State_NoSelect	
	Event OnOptionSelect(Int val)
	EndEvent	
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State Framework_TrackingState
	Event OnOptionSelect(Int val)
		
		Form OptnForm = Get_Option_Form(val)
		String OptnName = Get_Option_Name(val)
		Int FrameworkID = Get_Option_frID(val)

		if (FrameworkID == FrameworkLocationID)
		
			if ShowMessage("$PlaceMarkerMessage{" + Get_Option_Name(val) + "}", True, "$ConfirmP", "$ConfirmN")
				LocQuest.SetObjectiveDisplayed(10, False, True)	
				CurMarker = Get_Option_Name(val)
				CurLocation = Get_Option_Location(val)
				SetLocationMarker()
				LocMarker.MoveTo(CurLocation)
				LocQuest.SetObjectiveCompleted(10, False)	
				LocQuest.SetObjectiveDisplayed(10, True, True)	
			endif
			
			return
		endIf
		
		if CompMCM.bCellScanner_Pinning_Enabled && IsItemPinnable(OptnForm) && ShowMessage("$SetPinned{" + OptnName + "}", True, "$ConfirmY", "$ConfirmN")
			CompMCM.SwitchPinnedTarget(OptnForm, OptnName)
		else
			Int status = IsOptionCompleted(FrameworkID, OptnForm)
			if (status == 1)
				if ShowMessage("$RemoveCompleted{" + OptnForm.GetName() + "}", True, "$ConfirmY", "$ConfirmN")
					SetTitleText("$ProcessingTitle")	
					SetOptionCompleted(FrameworkID, OptnForm)
					Jump("$ProcessingTitle", 2)
				endIf
			elseif (status == 0)
				if ShowMessage("$ConfirmComplete{" + OptnForm.GetName() + "}", True, "$ConfirmY", "$ConfirmN")
					SetTitleText("$ProcessingTitle")	
					SetOptionCompleted(FrameworkID, OptnForm)
					Jump("$ProcessingTitle", 2)
				endIf
			elseif (status == -1)
				ShowMessage("Unable to find entry in array", false, "Ok")
			endif
		endif
		
	endEvent
endState

;---------------------------------------------------
;-- END OF CODE ------------------------------------
;---------------------------------------------------