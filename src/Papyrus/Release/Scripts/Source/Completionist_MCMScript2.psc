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
		Select_Page()
	else
		self.ShowMessage("$BusyMessage", false, "$ConfirmY")
	endif
endevent
	
;---------------------------------------------------
;-- Events -----------------------------------------
;---------------------------------------------------

Event OnOptionHighlight(Int val)
	
	SetInfoText(GetHighlightText(val, Get_Option_Name(val)))
EndEvent

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

String function GetHighlightText(Int val, String sOption)
	
	Int Index = OptionName.Find(sOption)
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

Function DisplayPage_1Entry(int a_frameworkID_a, string a_header_a, string a_header_b, int a_loc_a = 0)
	
	LoadArrays(a_frameworkID_a)

	SetTitleText("~ "  + EntriesFound_A + "/" + EntriesTotal_A + " - " + (((EntriesFound_A as Float / EntriesTotal_A as Float * 100 as Float)  as Int) as String) + "% Completed ~")
	SetCursorPosition(PagePIndex)
	AddHeaderOption(a_header_a)
	PagePIndex += 1
	AddHeaderOption(a_header_b + "{" + EntriesFound_A + "}{" + EntriesTotal_A + "}")
	PagePIndex += 1	
	
	PopulatePage(FormArray_A, NameArray_A, TextArray_A, BoolArray_A, a_frameworkID_a, a_loc_a)
endfunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function DisplayPage_2Entry(int a_frameworkID_a, int a_frameworkID_b, string a_header_a, string a_header_b, string a_header_c, string a_header_d, int a_loc_a = 0, int a_loc_b = 0)
	
	LoadArrays(a_frameworkID_a, a_frameworkID_b)
	
	SetTitleText("~ "  + (EntriesFound_A + EntriesFound_B)  + "/" + (EntriesTotal_A + EntriesTotal_B) + " - " + ((((EntriesFound_A + EntriesFound_B) as Float / (EntriesTotal_A + EntriesTotal_B) as Float * 100 as Float)  as Int) as String) + "% Completed ~")
	SetCursorPosition(PagePIndex)
	
	if (FormArray_A) && (FormArray_A[0] != "")
		AddHeaderOption(a_header_a)
		PagePIndex += 1
		AddHeaderOption(a_header_c + "{" + EntriesFound_A + "}{" + EntriesTotal_A + "}")
		PagePIndex += 1	
		PopulatePage(FormArray_A, NameArray_A, TextArray_A, BoolArray_A, a_frameworkID_a, a_loc_a)
		InsertPageBreak()
	endif

	if (FormArray_B) && (FormArray_B[0] != "")
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_b)
		PagePIndex += 1
		AddHeaderOption(a_header_d + "{" + EntriesFound_B + "}{" + EntriesTotal_B + "}")
		PagePIndex += 1
		PopulatePage(FormArray_B, NameArray_B, TextArray_B, BoolArray_B, a_frameworkID_b, a_loc_b)
		InsertPageBreak()
	endif
endfunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function DisplayPage_3Entry(int a_frameworkID_a, int a_frameworkID_b, int a_frameworkID_c, string a_header_a, string a_header_b, string a_header_c, string a_header_d, string a_header_e, string a_header_f, int a_loc_a = 0, int a_loc_b = 0, int a_loc_c = 0)
	
	LoadArrays(a_frameworkID_a, a_frameworkID_b, a_frameworkID_c)
	
	SetTitleText("~ "  + (EntriesFound_A + EntriesFound_B + EntriesFound_C)  + "/" + (EntriesTotal_A + EntriesTotal_B + EntriesTotal_C) + " - " + ((((EntriesFound_A + EntriesFound_B + EntriesFound_C) as Float / (EntriesTotal_A + EntriesTotal_B + EntriesTotal_C) as Float * 100 as Float)  as Int) as String) + "% Completed ~")
	SetCursorPosition(PagePIndex)

	if (FormArray_A) && (FormArray_A[0])
		AddHeaderOption(a_header_a)
		PagePIndex += 1
		AddHeaderOption(a_header_d + "{" + EntriesFound_A + "}{" + EntriesTotal_A + "}")
		PagePIndex += 1	
		PopulatePage(FormArray_A, NameArray_A, TextArray_A, BoolArray_A, a_frameworkID_a, a_loc_a)
		InsertPageBreak()
	endif

	if (FormArray_B) && (FormArray_B[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_b)
		PagePIndex += 1
		AddHeaderOption(a_header_e + "{" + EntriesFound_B + "}{" + EntriesTotal_B + "}")
		PagePIndex += 1
		PopulatePage(FormArray_B, NameArray_B, TextArray_B, BoolArray_B, a_frameworkID_b, a_loc_b)
		InsertPageBreak()
	endif

	if (FormArray_C) && (FormArray_C[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_c)
		PagePIndex += 1
		AddHeaderOption(a_header_f + "{" + EntriesFound_C + "}{" + EntriesTotal_C + "}")
		PagePIndex += 1	
		PopulatePage(FormArray_C, NameArray_C, TextArray_C, BoolArray_C, a_frameworkID_c, a_loc_c)
	endif
endfunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function DisplayPage_4Entry(int a_frameworkID_a, int a_frameworkID_b, int a_frameworkID_c, int a_frameworkID_d, string a_header_a, string a_header_b, string a_header_c, string a_header_d, string a_header_e, string a_header_f, string a_header_g, string a_header_h, int a_loc_a = 0, int a_loc_b = 0, int a_loc_c = 0, int a_loc_d = 0)
	
	LoadArrays(a_frameworkID_a, a_frameworkID_b, a_frameworkID_c, a_frameworkID_d)
	
	SetTitleText("~ "  + (EntriesFound_A + EntriesFound_B + EntriesFound_C + EntriesFound_D)  + "/" + (EntriesTotal_A + EntriesTotal_B + EntriesTotal_C + EntriesTotal_D) + " - " + ((((EntriesFound_A + EntriesFound_B + EntriesFound_C + EntriesFound_D) as Float / (EntriesTotal_A + EntriesTotal_B + EntriesTotal_C + EntriesTotal_D) as Float * 100 as Float)  as Int) as String) + "% Completed ~")
	SetCursorPosition(PagePIndex)

	if (FormArray_A) && (FormArray_A[0])
		AddHeaderOption(a_header_a)
		PagePIndex += 1
		AddHeaderOption(a_header_e + "{" + EntriesFound_A + "}{" + EntriesTotal_A + "}")
		PagePIndex += 1	
		PopulatePage(FormArray_A, NameArray_A, TextArray_A, BoolArray_A, a_frameworkID_a, a_loc_a)
		InsertPageBreak()
	endif

	if (FormArray_B) && (FormArray_B[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_b)
		PagePIndex += 1
		AddHeaderOption(a_header_f + "{" + EntriesFound_B + "}{" + EntriesTotal_B + "}")
		PagePIndex += 1
		PopulatePage(FormArray_B, NameArray_B, TextArray_B, BoolArray_B, a_frameworkID_b, a_loc_b)
		InsertPageBreak()
	endif

	if (FormArray_C) && (FormArray_C[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_c)
		PagePIndex += 1
		AddHeaderOption(a_header_g + "{" + EntriesFound_C + "}{" + EntriesTotal_C + "}")
		PagePIndex += 1	
		PopulatePage(FormArray_C, NameArray_C, TextArray_C, BoolArray_C, a_frameworkID_c, a_loc_c)
		InsertPageBreak()
	endif

	if (FormArray_D) && (FormArray_D[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_d)
		PagePIndex += 1
		AddHeaderOption(a_header_h + "{" + EntriesFound_D + "}{" + EntriesTotal_D + "}")
		PagePIndex += 1	
		PopulatePage(FormArray_D, NameArray_D, TextArray_D, BoolArray_D, a_frameworkID_d, a_loc_d)
	endif
endfunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function DisplayPage_5Entry(int a_frameworkID_a, int a_frameworkID_b, int a_frameworkID_c, int a_frameworkID_d, int a_frameworkID_e, string a_header_a, string a_header_b, string a_header_c, string a_header_d, string a_header_e, string a_header_f, string a_header_g, string a_header_h, string a_header_i, string a_header_j, int a_loc_a = 0, int a_loc_b = 0, int a_loc_c = 0, int a_loc_d = 0, int a_loc_e = 0)
	
	LoadArrays(a_frameworkID_a, a_frameworkID_b, a_frameworkID_c, a_frameworkID_d, a_frameworkID_e)
	
	SetTitleText("~ "  + (EntriesFound_A + EntriesFound_B + EntriesFound_C + EntriesFound_D + EntriesFound_E)  + "/" + (EntriesTotal_A + EntriesTotal_B + EntriesTotal_C + EntriesTotal_D + EntriesTotal_E) + " - " + ((((EntriesFound_A + EntriesFound_B + EntriesFound_C + EntriesFound_D + EntriesFound_E) as Float / (EntriesTotal_A + EntriesTotal_B + EntriesTotal_C + EntriesTotal_D + EntriesTotal_E) as Float * 100 as Float)  as Int) as String) + "% Completed ~")
	SetCursorPosition(PagePIndex)

	if (FormArray_A) && (FormArray_A[0])
		AddHeaderOption(a_header_a)
		PagePIndex += 1
		AddHeaderOption(a_header_f + "{" + EntriesFound_A + "}{" + EntriesTotal_A + "}")
		PagePIndex += 1	
		PopulatePage(FormArray_A, NameArray_A, TextArray_A, BoolArray_A, a_frameworkID_a, a_loc_a)
		InsertPageBreak()
	endif

	if (FormArray_B) && (FormArray_B[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_b)
		PagePIndex += 1
		AddHeaderOption(a_header_g + "{" + EntriesFound_B + "}{" + EntriesTotal_B + "}")
		PagePIndex += 1
		PopulatePage(FormArray_B, NameArray_B, TextArray_B, BoolArray_B, a_frameworkID_b, a_loc_b)
		InsertPageBreak()
	endif

	if (FormArray_C) && (FormArray_C[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_c)
		PagePIndex += 1
		AddHeaderOption(a_header_h + "{" + EntriesFound_C + "}{" + EntriesTotal_C + "}")
		PagePIndex += 1	
		PopulatePage(FormArray_C, NameArray_C, TextArray_C, BoolArray_C, a_frameworkID_c, a_loc_c)
		InsertPageBreak()
	endif

	if (FormArray_D) && (FormArray_D[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_d)
		PagePIndex += 1
		AddHeaderOption(a_header_i + "{" + EntriesFound_D + "}{" + EntriesTotal_D + "}")
		PagePIndex += 1	
		PopulatePage(FormArray_D, NameArray_D, TextArray_D, BoolArray_D, a_frameworkID_d, a_loc_d)
		InsertPageBreak()
	endif

	if (FormArray_E) && (FormArray_E[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_e)
		PagePIndex += 1
		AddHeaderOption(a_header_j + "{" + EntriesFound_E + "}{" + EntriesTotal_E + "}")
		PagePIndex += 1	
		PopulatePage(FormArray_E, NameArray_E, TextArray_E, BoolArray_E, a_frameworkID_e, a_loc_e)
	endif
endfunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function DisplayPage_6Entry(int a_frameworkID_a, int a_frameworkID_b, int a_frameworkID_c, int a_frameworkID_d, int a_frameworkID_e, int a_frameworkID_f, string a_header_a, string a_header_b, string a_header_c, string a_header_d, string a_header_e, string a_header_f, string a_header_g, string a_header_h, string a_header_i, string a_header_j, string a_header_k, string a_header_l, int a_loc_a = 0, int a_loc_b = 0, int a_loc_c = 0, int a_loc_d = 0, int a_loc_e = 0, int a_loc_f = 0)
	
	LoadArrays(a_frameworkID_a, a_frameworkID_b, a_frameworkID_c, a_frameworkID_d, a_frameworkID_e, a_frameworkID_f)
	
	SetTitleText("~ "  + (EntriesFound_A + EntriesFound_B + EntriesFound_C + EntriesFound_D + EntriesFound_E + EntriesFound_F)  + "/" + (EntriesTotal_A + EntriesTotal_B + EntriesTotal_C + EntriesTotal_D + EntriesTotal_E + EntriesTotal_F) + " - " + ((((EntriesFound_A + EntriesFound_B + EntriesFound_C + EntriesFound_D + EntriesFound_E + EntriesFound_F) as Float / (EntriesTotal_A + EntriesTotal_B + EntriesTotal_C + EntriesTotal_D + EntriesTotal_E + EntriesTotal_F) as Float * 100 as Float)  as Int) as String) + "% Completed ~")
	SetCursorPosition(PagePIndex)

	if (FormArray_A) && (FormArray_A[0])
		AddHeaderOption(a_header_a)
		PagePIndex += 1
		AddHeaderOption(a_header_g + "{" + EntriesFound_A + "}{" + EntriesTotal_A + "}")
		PagePIndex += 1	
		PopulatePage(FormArray_A, NameArray_A, TextArray_A, BoolArray_A, a_frameworkID_a, a_loc_a)
		InsertPageBreak()
	endif

	if (FormArray_B) && (FormArray_B[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_b)
		PagePIndex += 1
		AddHeaderOption(a_header_h + "{" + EntriesFound_B + "}{" + EntriesTotal_B + "}")
		PagePIndex += 1
		PopulatePage(FormArray_B, NameArray_B, TextArray_B, BoolArray_B, a_frameworkID_b, a_loc_b)
		InsertPageBreak()
	endif

	if (FormArray_C) && (FormArray_C[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_c)
		PagePIndex += 1
		AddHeaderOption(a_header_i + "{" + EntriesFound_C + "}{" + EntriesTotal_C + "}")
		PagePIndex += 1	
		PopulatePage(FormArray_C, NameArray_C, TextArray_C, BoolArray_C, a_frameworkID_c, a_loc_c)
		InsertPageBreak()
	endif

	if (FormArray_D) && (FormArray_D[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_d)
		PagePIndex += 1
		AddHeaderOption(a_header_j + "{" + EntriesFound_D + "}{" + EntriesTotal_D + "}")
		PagePIndex += 1	
		PopulatePage(FormArray_D, NameArray_D, TextArray_D, BoolArray_D, a_frameworkID_d, a_loc_d)
		InsertPageBreak()
	endif

	if (FormArray_E) && (FormArray_E[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_e)
		PagePIndex += 1
		AddHeaderOption(a_header_k + "{" + EntriesFound_E + "}{" + EntriesTotal_E + "}")
		PagePIndex += 1	
		PopulatePage(FormArray_E, NameArray_E, TextArray_E, BoolArray_E, a_frameworkID_e, a_loc_e)
		InsertPageBreak()
	endif

	if (FormArray_F) && (FormArray_F[0])
		SetCursorPosition(PagePIndex)
		AddHeaderOption(a_header_f)
		PagePIndex += 1
		AddHeaderOption(a_header_l + "{" + EntriesFound_F + "}{" + EntriesTotal_F + "}")
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
				
	if (CurrentPage == "$MCMPageArmor")
		DisplayPage_1Entry(00, "$PageHeaderArmor1", "$PageHeaderGenericFoundI")

;---------------------------------------------------
;---------------------------------------------------
		
	elseif (CurrentPage == "$MCMPageWeapons")
		DisplayPage_1Entry(01, "$PageHeaderWeapon1", "$PageHeaderGenericFoundI")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageItems")
		DisplayPage_1Entry(02, "$PageHeaderItems1", "$PageHeaderGenericFoundI")
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageJewelry")
		DisplayPage_1Entry(03, "$PageHeaderJewelry1", "$PageHeaderGenericFoundI")

;---------------------------------------------------
;---------------------------------------------------
		
	elseif (CurrentPage == "$MCMPageLiquor")
		DisplayPage_2Entry(04, 325, "$PageHeaderLiquor1", "$PageHeaderLiquorSUDS", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI")

;---------------------------------------------------
;---------------------------------------------------
		
	elseif (CurrentPage == "$MCMPageConditionals")
		DisplayPage_1Entry(67, "$PageHeaderConditionals1", "$PageHeaderGenericFoundI")
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageCCItems")
		DisplayPage_3Entry(05, 06, 07, "$PageHeaderGenericA", "$PageHeaderGenericI", "$PageHeaderGenericW", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI")
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageClaws")
		DisplayPage_2Entry(08, 09, "$PageHeaderClaws1", "$PageHeaderClaws2", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI")
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageMasks")
		DisplayPage_2Entry(10, 11, "$PageHeaderMasks1", "$PageHeaderMasks2", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI")

;---------------------------------------------------
;---------------------------------------------------
		
	elseif (CurrentPage == "$MCMPageBarenziah")
		DisplayPage_1Entry(12, "$PageHeaderBarenziah1", "$PageHeaderGenericFoundI")
		
;---------------------------------------------------
;---------------------------------------------------
		
	elseif (CurrentPage == "$MCMPageBooks1")
		DisplayPage_1Entry(13, "$PageHeaderBooks1", "$PageHeaderGenericFoundB")
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageBooks2")  
		DisplayPage_1Entry(14, "$PageHeaderBooks1", "$PageHeaderGenericFoundB")
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageBooks3")
		DisplayPage_1Entry(15, "$PageHeaderBooks1", "$PageHeaderGenericFoundB")
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageBooks5") 
		DisplayPage_1Entry(16, "$PageHeaderBooks1", "$PageHeaderGenericFoundB")
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageBooks6")  
		DisplayPage_5Entry(17, 50, 51, 52, 53, "$PageHeaderBooksA", "$PageHeaderBooksC", "$PageHeaderBooksD", "$PageHeaderBooksI", "$PageHeaderBooksR", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS")
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageBooks7")  
		DisplayPage_6Entry(18, 19, 54, 55, 56, 57, "$PageHeaderBooks1", "$PageHeaderBooksA", "$PageHeaderBooksC", "$PageHeaderBooksD", "$PageHeaderBooksI", "$PageHeaderBooksR", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageBooks8") 
		DisplayPage_6Entry(20, 21, 58, 59, 60, 61, "$PageHeaderBooks1", "$PageHeaderBooksA", "$PageHeaderBooksC", "$PageHeaderBooksD", "$PageHeaderBooksI", "$PageHeaderBooksR", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageBooks4") 
		DisplayPage_3Entry(22, 23, 24, "$PageHeaderMaps1", "$PageHeaderMaps2", "$PageHeaderMaps3", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundB")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageBooksCC")
		DisplayPage_6Entry(25, 26, 62, 63, 64, 65, "$PageHeaderBooks1", "$PageHeaderBooksA", "$PageHeaderBooksC", "$PageHeaderBooksD", "$PageHeaderBooksI", "$PageHeaderBooksR", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS")

;---------------------------------------------------
;---------------------------------------------------
		
	elseif (CurrentPage == "$MCMPageLocations1")
		DisplayPage_1Entry(27, "$PageHeaderPlaces1", "$PageHeaderGenericFoundL", 1)
		
;---------------------------------------------------
;---------------------------------------------------
		
	elseif (CurrentPage == "$MCMPageLocations2")
		DisplayPage_1Entry(28, "$PageHeaderPlaces1", "$PageHeaderGenericFoundL", 1)

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageLocations3")
		DisplayPage_1Entry(29, "$PageHeaderPlaces1", "$PageHeaderGenericFoundL", 1)
	
;---------------------------------------------------
;---------------------------------------------------
		
	elseif (CurrentPage == "$MCMPageLocations4")
		DisplayPage_1Entry(30, "$PageHeaderPlaces1", "$PageHeaderGenericFoundL", 1)
		
;---------------------------------------------------
;---------------------------------------------------
		
	elseif (CurrentPage == "$MCMPageLocations5")
		DisplayPage_1Entry(31, "$PageHeaderPlaces1", "$PageHeaderGenericFoundL", 1)

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageLocations6")
		DisplayPage_1Entry(32, "$PageHeaderPlaces1", "$PageHeaderGenericFoundL", 1)
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageDoomstone")
		DisplayPage_1Entry(33, "$PageHeaderDoomstone1", "$PageHeaderGenericFoundL")
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageShrines")
		DisplayPage_2Entry(34, 35, "$PageHeaderShrines1", "$PageHeaderShrines2", "$PageHeaderGenericFoundL", "$PageHeaderGenericFoundL")	

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageAEnchantments")
		DisplayPage_3Entry(36, 66, 37, "$PageHeaderAEnchantments", "$PageHeaderNEnchantments", "$PageHeaderSEnchantments", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS")
	
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageWEnchantments")
		DisplayPage_2Entry(38, 39, "$PageHeaderWEnchantments", "$PageHeaderSEnchantments", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPagePets")
		DisplayPage_5Entry(40, 41, 42, 43, 339, "$PageHeaderPets1", "$PageHeaderPets2", "$PageHeaderPets3", "$PageHeaderPets4", "$PageHeaderPets5", "$PageHeaderGenericFoundO", "$PageHeaderGenericFoundO", "$PageHeaderGenericFoundO", "$PageHeaderGenericFoundO", "$PageHeaderGenericFoundO")
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageHouses")
		DisplayPage_3Entry(44, 45, 46, "$PageHeaderHouses1", "$PageHeaderHouses2","$PageHeaderHouses3", "$PageHeaderGenericFoundO", "$PageHeaderGenericFoundO", "$PageHeaderGenericFoundO")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageShouts")
		GoToState("State_NoSelect")
		DisplayPage_3Entry(47, 48, 49, "$PageHeaderShouts1", "$PageHeaderShouts2","$PageHeaderShouts3", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS")	

;---------------------------------------------------
;---------------------------------------------------
	
	elseif (CurrentPage == "$MCMPageAdditionalHearthfireDolls")
		DisplayPage_1Entry(200, "$PageHeaderGenericI", "$PageHeaderGenericFoundI")
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageBrotherhood")
		DisplayPage_3Entry(201, 202, 203, "$PageHeaderGenericI", "$PageHeaderGenericB", "$PageHeaderGenericL", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundL", 0, 0, 1)

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageClockwork")
		DisplayPage_3Entry(204, 205, 206, "$PageHeaderGenericI", "$PageHeaderGenericB", "$PageHeaderGenericL", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundL", 0, 0, 1)
	
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageFalskaar")
		DisplayPage_3Entry(207, 208, 209, "$PageHeaderGenericI", "$PageHeaderGenericB", "$PageHeaderGenericL", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundL", 0, 0, 1)
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageFossilMining1")
		DisplayPage_1Entry(210, "$PageHeaderFossilsC", "$PageHeaderGenericFoundF")
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageFossilMining2")
		DisplayPage_1Entry(211, "$PageHeaderFossilsU", "$PageHeaderGenericFoundF")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageFossilMining3")
		DisplayPage_1Entry(212, "$PageHeaderFossilsR", "$PageHeaderGenericFoundF")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageGrayCowl")
		DisplayPage_3Entry(213, 214, 215, "$PageHeaderGenericI", "$PageHeaderGenericB", "$PageHeaderGenericL", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundL", 0, 0, 1)
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageOblivionArtifacts")
		DisplayPage_3Entry(216, 217, 218, "$PageHeaderGenericI", "$PageHeaderGenericB", "$PageHeaderGenericL", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundL", 0, 0, 1)

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageHelgen")
		DisplayPage_3Entry(219, 220, 221, "$PageHeaderGenericI", "$PageHeaderGenericB", "$PageHeaderGenericL", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundL", 0, 0, 1)

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageInterestingNPCs")
		DisplayPage_3Entry(222, 223, 224, "$PageHeaderGenericI", "$PageHeaderGenericB", "$PageHeaderGenericL", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundL", 0, 0, 1)
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageMoonStar")
		DisplayPage_3Entry(225, 226, 227, "$PageHeaderGenericI", "$PageHeaderGenericB", "$PageHeaderGenericL", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundL", 0, 0, 1)
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageMoonpath")
		DisplayPage_2Entry(228, 229, "$PageHeaderGenericI", "$PageHeaderGenericB", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundB")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageProjectAHO")
		DisplayPage_3Entry(230, 231, 232, "$PageHeaderGenericI", "$PageHeaderGenericB", "$PageHeaderGenericL", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundL", 0, 0, 1)

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageSUT1")
		DisplayPage_4Entry(233, 234, 235, 236, "$PageHeaderSUTJA", "$PageHeaderSUTPL", "$PageHeaderSUTFI", "$PageHeaderSUTIN", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageSUT2")
		DisplayPage_4Entry(237, 238, 239, 240, "$PageHeaderSUTSK", "$PageHeaderSUTGL", "$PageHeaderSUTGE", "$PageHeaderSUTWI", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageSUT3")
		DisplayPage_4Entry(241, 242, 243, 244, "$PageHeaderSUTWE", "$PageHeaderSUTAL", "$PageHeaderSUTGO", "$PageHeaderSUTCO", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageFishF")
		DisplayPage_1Entry(245, "$PageHeaderFish", "$PageHeaderGenericFoundC")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageFishI")
		DisplayPage_1Entry(246, "$PageHeaderItems1", "$PageHeaderGenericFoundI")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageFishB")
		DisplayPage_1Entry(247, "$PageHeaderBooks1", "$PageHeaderGenericFoundB")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageFishL")
		DisplayPage_4Entry(248, 249, 250, 251, "$PageHeaderFishingA", "$PageHeaderFishingC", "$PageHeaderFishingL", "$PageHeaderFishingS", "$PageHeaderGenericFoundL", "$PageHeaderGenericFoundL", "$PageHeaderGenericFoundL", "$PageHeaderGenericFoundL", 1, 1, 1, 1)
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageTeldrynSerious")
		DisplayPage_3Entry(252, 253, 254, "$PageHeaderGenericI", "$PageHeaderGenericB", "$PageHeaderGenericL", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundL", 0, 0, 1)

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageThunderchild")
		DisplayPage_3Entry(255, 256, 257, "$PageHeaderGenericI", "$PageHeaderGenericB", "$PageHeaderGenericL", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundL", 0, 0, 1)

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageUndeath")
		DisplayPage_2Entry(258, 259, "$PageHeaderGenericB", "$PageHeaderGenericL", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundL", 0, 1)
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageLull")
		DisplayPage_3Entry(260, 261, 262, "$PageHeaderGenericI", "$PageHeaderGenericB", "$PageHeaderGenericL", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundL", 0, 0, 1)

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageWintersun")
		DisplayPage_3Entry(263, 264, 265, "$PageHeaderGenericI", "$PageHeaderGenericB", "$PageHeaderGenericL", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundL", 0, 0, 1)

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageWyrmstooth")
		DisplayPage_3Entry(266, 267, 268, "$PageHeaderGenericI", "$PageHeaderGenericB", "$PageHeaderGenericL", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundL", 0, 0, 1)	

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageVIG1")
		DisplayPage_1Entry(269, "$PageHeaderGenericI", "$PageHeaderGenericFoundI")
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageVIG2")
		DisplayPage_1Entry(270, "$PageHeaderGenericB", "$PageHeaderGenericFoundB")
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageVIG3")
		DisplayPage_1Entry(271, "$PageHeaderGenericL", "$PageHeaderGenericFoundL")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageTomes_Apocalypse1")
		DisplayPage_2Entry(300, 301, "$PageHeaderBooksA", "$PageHeaderBooksC", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageTomes_Apocalypse2")
		DisplayPage_3Entry(302, 303, 304, "$PageHeaderBooksD", "$PageHeaderBooksI", "$PageHeaderBooksR", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS")

;---------------------------------------------------
;---------------------------------------------------
		
	elseif (CurrentPage == "$MCMPageTomes_Odin1")
		DisplayPage_2Entry(305, 306, "$PageHeaderBooksA", "$PageHeaderBooksC", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS")

;---------------------------------------------------
;---------------------------------------------------
		
	elseif (CurrentPage == "$MCMPageTomes_Odin2")
		DisplayPage_3Entry(307, 308, 309, "$PageHeaderBooksD", "$PageHeaderBooksI", "$PageHeaderBooksR", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS")
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageTomes_Mysticism1")
		DisplayPage_2Entry(310, 311, "$PageHeaderBooksA", "$PageHeaderBooksC", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageTomes_Mysticism2")
		DisplayPage_3Entry(312, 313, 314, "$PageHeaderBooksD", "$PageHeaderBooksI", "$PageHeaderBooksR", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageTomes_ForgottenMagic1")
		DisplayPage_2Entry(315, 316, "$PageHeaderBooksA", "$PageHeaderBooksC", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageTomes_ForgottenMagic2")
		DisplayPage_3Entry(317, 318, 319, "$PageHeaderBooksD", "$PageHeaderBooksI", "$PageHeaderBooksR", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageTomes_Triumvirate1")
		DisplayPage_2Entry(320, 321, "$PageHeaderBooksA", "$PageHeaderBooksC", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS")
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageTomes_Triumvirate2")
		DisplayPage_3Entry(322, 323, 324, "$PageHeaderBooksD", "$PageHeaderBooksI", "$PageHeaderBooksR", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS", "$PageHeaderGenericFoundS")

;---------------------------------------------------
;---------------------------------------------------
	
	elseif (CurrentPage == "$MCMPageCheese1")
		DisplayPage_1Entry(327, "$PageHeaderGenericI", "$PageHeaderGenericFoundI")

	elseif (CurrentPage == "$MCMPageCheese2")
		DisplayPage_1Entry(328, "$PageHeaderGenericI", "$PageHeaderGenericFoundI")

	elseif (CurrentPage == "$MCMPageCheese3")
		DisplayPage_1Entry(329, "$PageHeaderGenericI", "$PageHeaderGenericFoundI")

	elseif (CurrentPage == "$MCMPageCheese4")
		DisplayPage_1Entry(326, "$PageHeaderGenericI", "$PageHeaderGenericFoundI")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageRequiem1")
		DisplayPage_2Entry(331, 330, "$PageHeaderGenericI", "$PageHeaderGenericA", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI")

	elseif (CurrentPage == "$MCMPageRequiem2")
		DisplayPage_2Entry(332, 333, "$PageHeaderGenericS", "$PageHeaderGenericB", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundB")
		
	elseif (CurrentPage == "$MCMPageRequiem3")
		DisplayPage_1Entry(334, "$PageHeaderGenericT", "$PageHeaderGenericFoundS")

	elseif (CurrentPage == "$MCMPageRequiem4")
		DisplayPage_1Entry(335, "$PageHeaderGenericW", "$PageHeaderGenericFoundI")
		
;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageInnSoaps")
		DisplayPage_1Entry(336, "$PageHeaderGenericI", "$PageHeaderGenericFoundI")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageJaysusSwords")
		DisplayPage_1Entry(337, "$PageHeaderGenericW", "$PageHeaderGenericFoundI")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPagRoyalArmory")
		DisplayPage_1Entry(338, "$PageHeaderGenericW", "$PageHeaderGenericFoundI")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageCloaksOfSkyrim")
		DisplayPage_2Entry(340, 341, "$PageHeaderCloaks1", "$PageHeaderCloaks2", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI")

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageICOW")
		DisplayPage_4Entry(342, 343, 344, 345, "$PageHeaderGenericI", "$PageHeaderGenericB", "$PageHeaderGenericS", "$PageHeaderGenericT", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundS")		

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageTTB")
		DisplayPage_3Entry(346, 347, 348, "$PageHeaderGenericI", "$PageHeaderGenericB", "$PageHeaderGenericL", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundL", 0, 0, 1)		

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageBadGremlins")
		DisplayPage_4Entry(349, 350, 351, 352, "$PageHeaderBGC1", "$PageHeaderBGC2", "$PageHeaderBGC3", "$PageHeaderBGC4", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI", 0, 0, 0, 0)		

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageAmuletsofSkyrim1")
		DisplayPage_3Entry(353, 354, 355, "$PageHeaderAOS1", "$PageHeaderAOS2", "$PageHeaderAOS3", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI", 0, 0, 0)		

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageAmuletsofSkyrim2")
		DisplayPage_4Entry(356, 357, 358, 359, "$PageHeaderAOS4", "$PageHeaderAOS5", "$PageHeaderAOS6", "$PageHeaderAOS7", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI", 0, 0, 0, 0)	

;---------------------------------------------------
;---------------------------------------------------

	elseif (CurrentPage == "$MCMPageAmuletsofSkyrim3")
		DisplayPage_5Entry(360, 361, 362 ,363 ,364, "$PageHeaderAOS8", "$PageHeaderAOS9", "$PageHeaderAOS10", "$PageHeaderAOS11", "$PageHeaderAOS12", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundI", 0, 0, 0, 0, 0)	

;---------------------------------------------------
;---------------------------------------------------

	else
		if (!IsSettingsPage(CurrentPage))
			
			if (GetHeaderRequired(CurrentPage, 0)) && (GetHeaderRequired(CurrentPage, 1)) && (GetHeaderRequired(CurrentPage, 2))
				DisplayPage_3Entry(GetValidMiscPatchPageIDForItems(CurrentPage), GetValidMiscPatchPageIDForBooks(CurrentPage), GetValidMiscPatchPageIDForMapMa(CurrentPage), "$PageHeaderGenericI", "$PageHeaderGenericB", "$PageHeaderGenericL", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundL", 0, 0, 1)
				Return
			endif

			if (GetHeaderRequired(CurrentPage, 0)) && (GetHeaderRequired(CurrentPage, 1))
				DisplayPage_2Entry(GetValidMiscPatchPageIDForItems(CurrentPage), GetValidMiscPatchPageIDForBooks(CurrentPage), "$PageHeaderGenericI", "$PageHeaderGenericB", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundB", 0, 0)
				Return
			endif

			if (GetHeaderRequired(CurrentPage, 0)) && (GetHeaderRequired(CurrentPage, 2))
				DisplayPage_2Entry(GetValidMiscPatchPageIDForItems(CurrentPage), GetValidMiscPatchPageIDForMapMa(CurrentPage), "$PageHeaderGenericI", "$PageHeaderGenericL", "$PageHeaderGenericFoundI", "$PageHeaderGenericFoundL", 0, 1)
				Return
			endif			

			if (GetHeaderRequired(CurrentPage, 1)) && (GetHeaderRequired(CurrentPage, 2))
				DisplayPage_2Entry(GetValidMiscPatchPageIDForBooks(CurrentPage), GetValidMiscPatchPageIDForMapMa(CurrentPage), "$PageHeaderGenericB", "$PageHeaderGenericL", "$PageHeaderGenericFoundB", "$PageHeaderGenericFoundL", 0, 1)
				Return
			endif

			if (GetHeaderRequired(CurrentPage, 0))
				DisplayPage_1Entry(GetValidMiscPatchPageIDForItems(CurrentPage), "$PageHeaderGenericI", "$PageHeaderGenericFoundI", 0)
				Return
			endif

			if (GetHeaderRequired(CurrentPage, 1))
				DisplayPage_1Entry(GetValidMiscPatchPageIDForBooks(CurrentPage), "$PageHeaderGenericB", "$PageHeaderGenericFoundB", 0)
				Return
			endif

			if (GetHeaderRequired(CurrentPage, 2))
				DisplayPage_1Entry(GetValidMiscPatchPageIDForMapMa(CurrentPage), "$PageHeaderGenericL", "$PageHeaderGenericFoundL", 1)
				Return
			endif			
		endif
	endif
EndFunction	

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
		
	endEvent
endState

;---------------------------------------------------
;-- END OF CODE ------------------------------------
;---------------------------------------------------