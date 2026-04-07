Scriptname Completionist_MCMScript3 extends SKI_ConfigBase  
Import Completionist_Native

;-- Variables ---------------------------------------

Completionist_MCMScript Property CompMCM Auto

GlobalVariable Property Completionist_Busy Auto

Int PagePIndex = 0
int ActivePage = 0

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

bool Property SearchActive Auto Hidden

String[] OptionKeys
String[] OptionIden
Int[] OptionRadi

String[] Property NameArray auto hidden
String[] Property TextArray auto hidden
String[] Property IdenArray auto hidden
String[] Property KeysArray auto hidden
Int[] Property RadiArray auto hidden
	
String[] Y_Completed_Main_Name
String[] Y_Completed_Main_Text
String[] Y_Completed_Main_Iden
String[] Y_Completed_Main_Keys

String[] Y_Completed_Side_Name
String[] Y_Completed_Side_Text
String[] Y_Completed_Side_Iden
String[] Y_Completed_Side_Keys

String[] Y_Completed_Radi_Name
String[] Y_Completed_Radi_Text
String[] Y_Completed_Radi_Iden
String[] Y_Completed_Radi_Keys
		
Int Y_Completed_Main_Val = 0
Int Y_Completed_Side_Val = 0
Int Y_Completed_Radi_Val = 0
	
String[] N_Completed_Main_Name
String[] N_Completed_Main_Text
String[] N_Completed_Main_Iden
String[] N_Completed_Main_Keys

String[] N_Completed_Side_Name
String[] N_Completed_Side_Text
String[] N_Completed_Side_Iden
String[] N_Completed_Side_Keys

String[] N_Completed_Radi_Name
String[] N_Completed_Radi_Text
String[] N_Completed_Radi_Iden
String[] N_Completed_Radi_Keys

Int N_Completed_Main_Val = 0
Int N_Completed_Side_Val = 0
Int N_Completed_Radi_Val = 0

Bool ShowMainQuests
Bool ShowSideQuests
Bool ShowRadiQuests

Int posRight
Int posLeft
Int posSectionLeft
Int posSectionRight
Int CurrentQuestID

int State_SearchHistory
String[] SearchHistory
int SearchHistoryChoice
Int OID_SearhTerm

int State_UserOptions
String[] UserOptions
int UserOptionsChoice

Bool ManualPageChange = false

;---------------------------------------------------
;-- START OF CODE ----------------------------------
;---------------------------------------------------

Event OnConfigInit()	
	pages = GetMCMPages(1)
endevent

Event OnConfigOpen()
	if (CompMCM.bShortCutActive) && (CompMCM.UnofficialJumpPage != "")
		if (ShowMessage("$JumpMessage{" + CompMCM.UnofficialJumpPage + "}", true, "$ConfirmYes", "$ConfirmNo"))
			Jump(CompMCM.UnofficialJumpPage, 0)
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
	
	ManualPageChange = True
	if (jumpType == 0)
		SendModEvent("SKICP_pageSelected", CompMCM.UnofficialJumpPage, pages.Find(CompMCM.UnofficialJumpPage))
	endif

	if (jumpType == 2)
		SendModEvent("SKICP_pageSelected", currentpage, pages.Find(currentpage))
	endif
endFunction

;---------------------------------------------------
;-- Events -----------------------------------------
;---------------------------------------------------

event OnPageReset(string page)
	
	pages = GetMCMPages(2)
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
	
	SetInfoText(GetHighlightText(val))
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
		if (a_names[idx] != "" && IsFormVisible(CurrentPage, a_forms[idx], UserOptionsChoice))
			SetCursorPosition(PagePIndex)

			if (a_names[idx] == CompMCM.SearchedEntry || (a_type == 4 && IsMatchingShout(a_names[idx], CompMCM.SearchedEntry)))
				CompMCM.SearchedEntry = ""
				CompMCM.SearchJumpPage = ""
				CompMCM.SearchedMCMMenu = ""				 
				
				if (CompMCM.b_SearchHighlightQuest)
					if (!a_bools[idx] || a_type == 4)
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

			if (a_type == 2)
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

Function AddToPages(String sString, form a_form, int a_type, bool a_bool = false)
	
	if (a_type == 2 && MapMarkerIsCleared(a_form)) ;Location
		sString = sString + " " + GetLocStringByKeyExt("MapMarkerCleared")
	endif
				
	if (!a_bool)	
		OptionSlot[OptionIndx] = AddTextOption(sString, "", 0)
		return
	endif

	if (a_type == 4) ;Shout
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

Function AddMultiPageOptions()
	SetCursorPosition(0)
	AddSliderOptionST("StatePageManager", GetActivePageName(ActivePage, CurrentPage), ActivePage, "{0}", 0)

	SetCursorPosition(1)
	if ActivePage == 0
		AddTextOption("$BrowsingPage0", "", 1)
	else
		AddTextOption("$BrowsingPagex{" + ActivePage + "}{" + (GetMultiPageCount(currentpage) - 1) + "}", "", 1)
	endif
	PagePIndex = 2
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function DisplayPatchPage(string[] config)

	AddMultiPageOptions()

	Int Idx = 1
	Int Edx = 0

	While Edx < (config[0] as int)
		Int f = GetEntries_FoundByID(config[Idx] as int)
		Int t = GetEntries_TotalByID(config[Idx] as int)
	
		SetCursorPosition(PagePIndex)
		AddHeaderOption(config[Idx + 1])
		PagePIndex += 1

		AddHeaderOption(f + "/" + t + " " + config[Idx + 2])
		PagePIndex += 1	
		
		PopulatePage(GetFormArrayByID(config[Idx] as int), GetNameArrayByID(config[Idx] as int), GetTextArrayByID(config[Idx] as int), GetBoolArrayByID(config[Idx] as int), config[Idx] as int, config[Idx + 3] as int)
		InsertPageBreak()

		Idx += 4
		Edx += 1
	endWhile

	Int title_f = GetTotalEntriesFoundForPage(CurrentPage)
	Int title_t = GetTotalEntriesForPage(CurrentPage)
	SetTitleText("~ " + title_f + "/" + title_t + " - " + (((title_f as Float / title_t as Float * 100 as Float)  as Int) as String) + "% Completed ~")
endfunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Event OnOptionInputOpen(Int val)
	if (val == OID_SearhTerm)
		SetInputDialogStartText("$State_SearchTermString")
	endif
EndEvent

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Event OnOptionInputAccept(Int val, String a_searchTerm)
	if (val == OID_SearhTerm)
		AddSearchTerm(CurrentPage, a_searchTerm)
		ProcessSearchResults(a_searchTerm)
		return
	endIf
EndEvent

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function ProcessSearchResults(String term)
	
	if (GetStringLength(term) < 3)
		ShowMessage("$ResultsMessageErrorOnLength", false)
		RefreshPage();
		return
	endif
	
	string[] results = SearchMultiPage(CurrentPage, term, CompMCM.b_SearchIgnoreCompleted, CompMCM.i_SearchMaxResults, CompMCM.i_SearchTypeChoice);
	
	if (!results.length)
		ShowMessage("$ResultsMessage{" + term + "}", false)
		RefreshPage();
	endif
	
	int x = 0
	while (x < results.length)
		if (ShowMessage(results[x], true, "$GoToPage", "$Continue"))
			CompMCM.SearchJumpPage = results[x + 1]
			CompMCM.SearchedEntry  = results[x + 2]
			CompMCM.SearchedMCMMenu = results[x + 3]
			SearchActive = True
			RefreshPage()
			return
		endif
		x += 4
	endWhile
	RefreshPage()
endFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function DisplayMultiPageSplashScreen()
	GoToState("MultiPageSplash")
	
	Int title_f = GetTotalEntriesFoundForPage(CurrentPage)
	Int title_t = GetTotalEntriesForPage(CurrentPage)
	SearchHistory = GetSearchHistory(CurrentPage)
	SearchHistoryChoice = 0

	SetTitleText("~ " + title_f + "/" + title_t + " - " + (((title_f as Float / title_t as Float * 100 as Float)  as Int) as String) + "% Completed ~")

	AddMultiPageOptions()

	AddEmptyOption()
	AddEmptyOption()

	AddHeaderOption("$Settings{" + CurrentPage + "}")
	AddHeaderOption("")

	AddTextOptionST("StateDefaultPageToggle", "$StateDefaultPageToggle_Text", GetEnabledStatus(GetUseDefaultPage(CurrentPage)), 0)
	AddSliderOptionST("StateDefaultPageSlider", "$StateDefaultPageSlider_Text", GetDefaultPage(CurrentPage), "{0}", 0)

	OID_SearhTerm = AddInputOption("$State_SearchTerm_Text", "$State_SearchTermString", 0)
	AddMenuOptionST("State_SearchHistory", "$State_SearchHistory_Text", SearchHistory[SearchHistoryChoice], 0)
		
	if HasDropDownMenu(CurrentPage)
		UserOptions = GetDropDownMenuOptions(CurrentPage)
		UserOptionsChoice = GetCurrentUserOption(CurrentPage)		
		AddMenuOptionST("State_UserOptions", GetDropDownMenuName(CurrentPage), UserOptions[UserOptionsChoice], 0)
	else
		AddEmptyOption()
	endif
	
	AddEmptyOption()

	AddHeaderOption("$Sections{" + CurrentPage + "}")
	AddHeaderOption("$Completion")

	Int Idx = 0
	string[] list = GetMultiPageSplashScreenConfig(CurrentPage)
	while Idx < list.length
		OptionSlot[OptionIndx] = AddTextOption(list[Idx], "", 0)
		AddTextOption(list[Idx + 1], "", 0)
		
		OptionName[OptionIndx] = list[Idx]
		Idx += 2
		OptionIndx += 1
	endwhile
endfunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

function Select_Page()
	
	PagePIndex = 0
	SetCursorFillMode(LEFT_TO_RIGHT)
	GoToState("Framework_TrackingState")

	if (!IsSettingsPage(CurrentPage))
		
		if (!CanLoadPage(CurrentPage))
			ShowMessage("$PageValidationError", false, "$ConfirmY")
			return
		endif
		
		ActivePage = GetActivePage(CurrentPage)	

		int page_count = GetMultiPageCount(currentpage) - 1
		if (GetActivePage(CurrentPage) > page_count || GetDefaultPage(CurrentPage) > page_count )
			ResetPageSettings(CurrentPage)
			RefreshPage()
		endif

		if (SearchActive)
			ActivePage = GetPageNumberForForm(CompMCM.SearchJumpPage, CompMCM.SearchedEntry)
			SetActivePage(CurrentPage, ActivePage)
			SearchActive = false
		else
			if (GetUseDefaultPage(CurrentPage) && !ManualPageChange)
				ActivePage = GetDefaultPage(CurrentPage)
				SetActivePage(CurrentPage, ActivePage)
			endif
		endif

		if (GetUseDefaultPage(CurrentPage))
			ManualPageChange = false
		endif
		
		CurrentQuestID = GetQuestID(CurrentPage, ActivePage)

		if(CurrentQuestID > 0)
			LoadArrays()

			NameArray = qGetMiscQuestNameArrayByID(CurrentQuestID)
			TextArray = qGetMiscQuestTextArrayByID(CurrentQuestID)
			IdenArray = qGetMiscQuestIdenArrayByID(CurrentQuestID)
			RadiArray = qGetMiscQuestRadiArrayByID(CurrentQuestID)
			KeysArray = qGetMiscQuestKeysArrayByID(CurrentQuestID)
			OptionIndx = KeysArray.length
			LoadPage()
			return
		endif

		string[] config = GetPageConfiguration(CurrentPage, ActivePage)

		if(ActivePage == 0)
			DisplayMultiPageSplashScreen()
			return
		endif

		if config.length <= 0 || config[0] == 0
			return
		endif

		DisplayPatchPage(config)
	endif
EndFunction	

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function RefreshPage()
	ManualPageChange = True
	ForcePageReset()
endFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

String function GetEnabledStatus(Bool bValue)

	if (bValue)
		Return "$Enabled"
	endif

	Return "$Disabled"
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

State MultiPageSplash	
	Event OnOptionSelect(Int val)
		
		string section = Get_Option_Name(val)
		if section != ""
			if ShowMessage("$JumpToSection{" + Get_Option_Name(val) + "}", True, "$ConfirmJ", "$ConfirmN")
				ActivePage = GetPageNumberForSection(CurrentPage, section)
				SetActivePage(CurrentPage, ActivePage)
				RefreshPage()
			endif
		endif
	EndEvent

	Event OnOptionHighlight(Int val)
		if (val == OID_SearhTerm)
			SetInfoText("$State_SearchTerm_Info")
		else
			SetInfoText(GetHighlightText(val))
		endif
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state StateDefaultPageSlider ; SLIDER

	event OnSliderOpenST()
		SetSliderDialogStartValue(GetDefaultPage(CurrentPage))
		SetSliderDialogDefaultValue(0)
		SetSliderDialogRange(0, GetMultiPageCount(CurrentPage) - 1)
		SetSliderDialogInterval(1)
	endEvent

	event OnSliderAcceptST(float a_value)
		SetDefaultPage(CurrentPage, a_value as int)
		SetSliderOptionValueST(a_value, "{0}")
	endEvent

	event OnDefaultST()
		SetDefaultPage(CurrentPage, 0)
		self.SetSliderOptionValueST(0 as Float, "{0}")
	endEvent

	event OnHighlightST()
		self.SetInfoText("$StateDefaultPageSlider_Info{" + GetActivePageName(GetDefaultPage(CurrentPage), CurrentPage) + "}")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state StatePageManager ; SLIDER

	event OnSliderOpenST()
		SetSliderDialogStartValue(GetActivePage(CurrentPage))
		SetSliderDialogDefaultValue(0)
		SetSliderDialogRange(0, GetMultiPageCount(CurrentPage) - 1)
		SetSliderDialogInterval(1)
	endEvent

	event OnSliderAcceptST(float a_value)
		ActivePage = a_value as int
		SetActivePage(CurrentPage, ActivePage)
		SetSliderOptionValueST(ActivePage, "{0}")
		RefreshPage()
	endEvent

	event OnDefaultST()
		ActivePage = 0
		SetActivePage(CurrentPage, ActivePage)
		self.SetSliderOptionValueST(0 as Float, "{0}")
		RefreshPage()
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state StateDefaultPageToggle

	Event OnSelectST()
		bool default = GetUseDefaultPage(CurrentPage)
		SetUseDefaultPage(CurrentPage, !default)
		SetTextOptionValueST(GetEnabledStatus(default))
		RefreshPage()
	EndEvent
	
	Event OnDefaultST()
		SetUseDefaultPage(CurrentPage, false)
		SetTextOptionValueST(GetEnabledStatus(false))
		RefreshPage()
	EndEvent

	Event OnHighlightST()
		SetInfoText("$StateDefaultPageToggle_Info{"+ CurrentPage +"}")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State State_SearchHistory ; MENU

	Event OnMenuOpenST()
		SetMenuDialogStartIndex(0)
		SetMenuDialogDefaultIndex(0)
		SetMenuDialogOptions(SearchHistory)
	EndEvent
					
	Event OnMenuAcceptST(Int index)
		SearchHistoryChoice = Index
		SetMenuOptionValueST(State_SearchHistory, SearchHistory[SearchHistoryChoice])
		
		if (SearchHistory[SearchHistoryChoice] == "$NoSearchHistory" || SearchHistory[SearchHistoryChoice] == "")
			RefreshPage()
			return
		endif

		if (SearchHistory[SearchHistoryChoice] == "$ClearSearchHistory")
			ClearSearchHistory(CurrentPage)
			RefreshPage()
			return
		endif
		
		if ShowMessage("$State_rerunSearch{" + SearchHistory[SearchHistoryChoice] + "}", True, "$ConfirmY", "$ConfirmN")
			AddSearchTerm(CurrentPage, SearchHistory[SearchHistoryChoice])
			ProcessSearchResults(SearchHistory[SearchHistoryChoice])
		endif
		RefreshPage()
	EndEvent

	Event OnDefaultST()
		SearchHistoryChoice = 0
		SetMenuOptionValueST(State_SearchHistory, SearchHistory[SearchHistoryChoice])
	EndEvent

	Event OnHighlightST()
		SetInfoText("$State_SearchHistory_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State State_UserOptions ; MENU

	Event OnMenuOpenST()
		SetMenuDialogStartIndex(UserOptionsChoice)
		SetMenuDialogDefaultIndex(0)
		SetMenuDialogOptions(UserOptions)
	EndEvent
					
	Event OnMenuAcceptST(Int index)
		UserOptionsChoice = Index
		SetCurrentUserOption(CurrentPage, UserOptionsChoice)
		SetMenuOptionValueST(State_UserOptions, UserOptions[UserOptionsChoice])
		RefreshPage()
		return
	EndEvent

	Event OnDefaultST()
		UserOptionsChoice = 0
		SetCurrentUserOption(CurrentPage, UserOptionsChoice)
		SetMenuOptionValueST(State_UserOptions, UserOptions[UserOptionsChoice])
	EndEvent

	Event OnHighlightST()
		SetInfoText(GetDropDownMenuHighlight(CurrentPage))
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
				if ShowMessage("$RemoveCompleted{" + OptnName + "}", True, "$ConfirmY", "$ConfirmN")
					SetTitleText("$ProcessingTitle")	
					SetOptionCompleted(FrameworkID, OptnForm)
					Jump("$ProcessingTitle", 2)
				endIf
			elseif (status == 0)
				if ShowMessage("$ConfirmComplete{" + OptnName + "}", True, "$ConfirmY", "$ConfirmN")
					SetTitleText("$ProcessingTitle")	
					SetOptionCompleted(FrameworkID, OptnForm)
					Jump("$ProcessingTitle", 2)
				endIf
			elseif (status == -1)
				ShowMessage("Unable to find entry in array", false, "Ok")
			endif
		endif		
		
	endEvent

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
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State Quest_TrackingState
	Event OnOptionSelect(Int val)

		String quest_keys = Get_QuestOptionKeys(val)
		String quest_name = Get_QuestOptionName(val)
		if (quest_keys == "")
			return
		endif	
	
		Int status = qIsOptionCompleted(CurrentQuestID, quest_keys)

		if (status == -1)
			ShowMessage("Unable to find entry in array", false, "Ok")
		endif 

		if (status == 0)
			if ShowMessage("$ConfirmCompleteQ{" + quest_name + "}", True, "$ConfirmY", "$ConfirmN")
				qSetOptionCompleted(CurrentQuestID, quest_keys, true)
				Jump("$ProcessingTitle", 2)
			endIf
		endif 

		if (status == 1)
			if ShowMessage("$RemoveCompletedQ{" + quest_name + "}", True, "$ConfirmY", "$ConfirmN")
				qSetOptionCompleted(CurrentQuestID, quest_keys, false)
				Jump("$ProcessingTitle", 2)
			endIf
		endif 

		if (status == 2)
			ShowMessage("$DebugNoMove", false, "$ConfirmY")
		endif		
	endEvent
	
	Event OnOptionHighlight(Int val)
		if (CompMCM.bDebug)
			SetInfoText(Get_QuestOptionDebug(val))
			return
		endif
		SetInfoText(Get_QuestOptionText(val))
	endEvent
endState

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function GetColourStatus(String quest_name, String quest_key = "", bool search_quest = false)
	
	if (search_quest)
		CompMCM.SearchedEntry = ""
		CompMCM.SearchJumpPage = ""
		CompMCM.SearchedMCMMenu = ""			  
		OptionSlot[OptionIndx] = AddTextOption("$SearchResult{" + "#FFD966" + "}{" + quest_name + "}", "", 0)
		return
	endif
	
	if (qIsOptionToggled(CurrentQuestID, quest_key))		
		if (CompMCM.State_ManualCompletionView_Choice == 0)
		
			OptionSlot[OptionIndx] = AddTextOption("<font color='" + CompMCM.State_ManualCompletionColourString + "'>" + quest_name + "</font>", "(M)", 0)
			
		elseif (CompMCM.State_ManualCompletionView_Choice == 1)
			OptionSlot[OptionIndx] = AddTextOption(quest_name, "(M)", 0)
		
		elseif (CompMCM.State_ManualCompletionView_Choice == 2)
			OptionSlot[OptionIndx] = AddTextOption("<font color='" + CompMCM.State_ManualCompletionColourString + "'>" + quest_name + "</font>", "", 0)
		endif
	else
		if (CompMCM.State_AutomaticCompletionView_Choice == 0)
			OptionSlot[OptionIndx] = AddToggleOption("<font color='" + CompMCM.State_AutomaticCompletionColourString + "'>" + quest_name + "</font>", True, 0)
			
		elseif (CompMCM.State_AutomaticCompletionView_Choice == 1)
			OptionSlot[OptionIndx] = AddToggleOption(quest_name, True, 0)
		
		elseif (CompMCM.State_AutomaticCompletionView_Choice == 2)
			OptionSlot[OptionIndx] = AddTextOption("<font color='" + CompMCM.State_AutomaticCompletionColourString + "'>" + quest_name + "</font>", "", 0)
		endif
	endif
endFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

function buildpageLayout()

	posLeft = 0
	posRight = 1	

	AddMultiPageOptions()

	posLeft = 2
	posRight = 3

	SetCursorFillMode(LEFT_TO_RIGHT)
	posSectionLeft = 0
	posSectionRight = 0

	buildSection(True,  "$BuildPage01", posSectionLeft, N_Completed_Main_Name, N_Completed_Main_Text, N_Completed_Main_Iden, N_Completed_Main_Keys, "AM", ShowMainQuests)
	
	if (ShowMainQuests)
		posSectionLeft = 1
	endIf	
	buildSection(True,  "$BuildPage02", posSectionLeft, N_Completed_Side_Name, N_Completed_Side_Text, N_Completed_Side_Iden, N_Completed_Side_Keys, "AS", ShowSideQuests)

	if (ShowMainQuests) || (ShowSideQuests)
		posSectionLeft = 1
	endIf	
	buildSection(True,  "$BuildPage03", posSectionLeft, N_Completed_Radi_Name, N_Completed_Radi_Text, N_Completed_Radi_Iden, N_Completed_Radi_Keys, "AR", ShowRadiQuests)

	
	buildSection(False, "$BuildPage04", posSectionRight, Y_Completed_Main_Name, Y_Completed_Main_Text, Y_Completed_Main_Iden, Y_Completed_Main_Keys, "CM", ShowMainQuests)
	
	if (ShowMainQuests)
		posSectionRight = 1
	endIf	
	buildSection(False, "$BuildPage05", posSectionRight, Y_Completed_Side_Name, Y_Completed_Side_Text, Y_Completed_Side_Iden, Y_Completed_Side_Keys, "CS", ShowSideQuests)

	if (ShowMainQuests) || (ShowSideQuests)
		posSectionRight = 1
	endIf	
	buildSection(False, "$BuildPage06", posSectionRight, Y_Completed_Radi_Name, Y_Completed_Radi_Text, Y_Completed_Radi_Iden, Y_Completed_Radi_Keys, "CR", ShowRadiQuests)
	
	
	Int title_f = GetTotalEntriesFoundForPage(CurrentPage)
	Int title_t = GetTotalEntriesForPage(CurrentPage)
	SetTitleText("~ " + title_f + "/" + title_t + " - " + (((title_f as Float / title_t as Float * 100 as Float)  as Int) as String) + "% Completed ~")

	GoToState("Quest_TrackingState")
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

function buildSection(bool pageLeft, String headerString, Int intPos, String[] NameArr, String[] TextArr, String[] IdenArr, String[] KeysArr, String Section, Bool ShowSection)
	
	if (!ShowSection)
		return
	endIf
	
	Int Index = 0
	if pageLeft
		posLeft += intPos * 2
		SetCursorPosition(posLeft)
		AddHeaderOption(headerString)
		posLeft += 2
		
		if (NameArr[Index] == "")
			if (Section == "AM")
				SetCursorPosition(posLeft)
				AddTextOption("$BuildPage07", "", 0)
				posLeft += 2
			elseif (Section == "AS")
				SetCursorPosition(posLeft)
				AddTextOption("$BuildPage08", "", 0)
				posLeft += 2
			elseif (Section == "AR")
				SetCursorPosition(posLeft)
				AddTextOption("$BuildPage09", "", 0)
				posLeft += 2
			endif
		else
			While Index < NameArr.length && NameArr[Index] != ""
				SetCursorPosition(posLeft)
				
				if IsQuestVisible(CurrentPage, IdenArr[Index], UserOptionsChoice)
					if (NameArr[Index] == CompMCM.SearchedEntry && CompMCM.b_SearchHighlightQuest)
						GetColourStatus(NameArr[Index], KeysArr[Index], true)
					else
						OptionSlot[OptionIndx] = AddTextOption(NameArr[Index], "", 0)
					endif

					OptionName[OptionIndx] = NameArr[Index]
					OptionHigh[OptionIndx] = TextArr[Index]
					OptionIden[OptionIndx] = IdenArr[Index]
					OptionKeys[OptionIndx] = KeysArr[Index]
					OptionIndx += 1
					posLeft += 2
				endif
				Index += 1
			EndWhile
		endif
	Else
		posRight += intPos * 2
		SetCursorPosition(posRight)
		AddHeaderOption(headerString)
		posRight += 2

		if (NameArr[Index] == "")
			if (Section == "CM")
				SetCursorPosition(posRight)
				AddTextOption("$BuildPage10", "", 0)
				posRight += 2
			elseif (Section == "CS")
				SetCursorPosition(posRight)
				AddTextOption("$BuildPage10", "", 0)
				posRight += 2
			elseif (Section == "CR")
				SetCursorPosition(posRight)
				AddTextOption("$BuildPage10", "", 0)
				posRight += 2
			endif
		else
			While Index < NameArr.length && NameArr[Index] != ""
				SetCursorPosition(posRight)
				
				if IsQuestVisible(CurrentPage, IdenArr[Index], UserOptionsChoice)
					GetColourStatus(NameArr[Index], KeysArr[Index])
					OptionName[OptionIndx] = NameArr[Index]
					OptionHigh[OptionIndx] = TextArr[Index]
					OptionIden[OptionIndx] = IdenArr[Index]
					OptionKeys[OptionIndx] = KeysArr[Index]
					OptionIndx += 1
					posRight += 2
				endif
				Index += 1
			EndWhile
		endif
	endif
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function LoadArrays()

	OptionIndx = 0
	OptionSlot = New Int[128]
	OptionRadi = New Int[128]
	
	OptionIden = New String[128]
	OptionName = New String[128]
	OptionHigh = New String[128]
	OptionKeys = New String[128]

	NameArray = New String[128]
	TextArray = New String[128]
	IdenArray = New String[128]
	KeysArray = New String[128]
	RadiArray = New Int[128]
	
	Y_Completed_Main_Name = New String[128]
	Y_Completed_Main_Text = New String[128]
	Y_Completed_Main_Iden = New String[128]
	Y_Completed_Main_Keys = New String[128]
	
	Y_Completed_Side_Name = New String[128]
	Y_Completed_Side_Text = New String[128]
	Y_Completed_Side_Iden = New String[128]
	Y_Completed_Side_Keys = New String[128]

	Y_Completed_Radi_Name = New String[128]
	Y_Completed_Radi_Text = New String[128]
	Y_Completed_Radi_Iden = New String[128]
	Y_Completed_Radi_Keys = New String[128]
	
	Y_Completed_Main_Val = 0
	Y_Completed_Side_Val = 0
	Y_Completed_Radi_Val = 0
	
	N_Completed_Main_Name = New String[128]
	N_Completed_Main_Text = New String[128]
	N_Completed_Main_Iden = New String[128]
	N_Completed_Main_Keys = New String[128]
	
	N_Completed_Side_Name = New String[128]
	N_Completed_Side_Text = New String[128]
	N_Completed_Side_Iden = New String[128]
	N_Completed_Side_Keys = New String[128]
	
	N_Completed_Radi_Name = New String[128]
	N_Completed_Radi_Text = New String[128]
	N_Completed_Radi_Iden = New String[128]
	N_Completed_Radi_Keys = New String[128]
	
	N_Completed_Main_Val = 0
	N_Completed_Side_Val = 0
	N_Completed_Radi_Val = 0
endFunction
	
;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

function LoadPage()

	ShowMainQuests = False
	ShowSideQuests = False
	ShowRadiQuests = False
	
	Int Index = 0
	While Index < NameArray.length && (NameArray[Index] != "")
		
		if (qIsOptionCompleted(CurrentQuestID, KeysArray[Index]) > 0 || qIsOptionToggled(CurrentQuestID, KeysArray[Index]))

			if (RadiArray[Index] == 0)
				
				Y_Completed_Main_Name[Y_Completed_Main_Val] = NameArray[Index]
				Y_Completed_Main_Text[Y_Completed_Main_Val] = TextArray[Index]
				Y_Completed_Main_Iden[Y_Completed_Main_Val] = IdenArray[Index]
				Y_Completed_Main_Keys[Y_Completed_Main_Val] = KeysArray[Index]
				
				ShowMainQuests = True
				Y_Completed_Main_Val += 1
				
			elseif (RadiArray[Index] == 1)
				
				Y_Completed_Side_Name[Y_Completed_Side_Val] = NameArray[Index]
				Y_Completed_Side_Text[Y_Completed_Side_Val] = TextArray[Index]
				Y_Completed_Side_Iden[Y_Completed_Side_Val] = IdenArray[Index]
				Y_Completed_Side_Keys[Y_Completed_Side_Val] = KeysArray[Index]
				
				ShowSideQuests = True
				Y_Completed_Side_Val += 1

			elseif (RadiArray[Index] == 2)
				
				Y_Completed_Radi_Name[Y_Completed_Radi_Val] = NameArray[Index]
				Y_Completed_Radi_Text[Y_Completed_Radi_Val] = TextArray[Index] + "\n" + qGetTimesCompletedVsTimesRequiredText(KeysArray[Index])
				Y_Completed_Radi_Iden[Y_Completed_Radi_Val] = IdenArray[Index]
				Y_Completed_Radi_Keys[Y_Completed_Radi_Val] = KeysArray[Index]
				
				ShowRadiQuests = True
				Y_Completed_Radi_Val += 1				
			endif
			
		else
		
			if (RadiArray[Index] == 0)
				
				N_Completed_Main_Name[N_Completed_Main_Val] = NameArray[Index]
				N_Completed_Main_Text[N_Completed_Main_Val] = TextArray[Index]
				N_Completed_Main_Iden[N_Completed_Main_Val] = IdenArray[Index]
				N_Completed_Main_Keys[N_Completed_Main_Val] = KeysArray[Index]
				
				ShowMainQuests = True
				N_Completed_Main_Val += 1
				
			elseif (RadiArray[Index] == 1)
			
				N_Completed_Side_Name[N_Completed_Side_Val] = NameArray[Index]
				N_Completed_Side_Text[N_Completed_Side_Val] = TextArray[Index]
				N_Completed_Side_Iden[N_Completed_Side_Val] = IdenArray[Index]
				N_Completed_Side_Keys[N_Completed_Side_Val] = KeysArray[Index]
				
				ShowSideQuests = True
				N_Completed_Side_Val += 1
				
			elseif (RadiArray[Index] == 2)
			
				N_Completed_Radi_Name[N_Completed_Radi_Val] = NameArray[Index]
				N_Completed_Radi_Text[N_Completed_Radi_Val] = TextArray[Index] + "\n" + qGetTimesCompletedVsTimesRequiredText(KeysArray[Index])
				N_Completed_Radi_Iden[N_Completed_Radi_Val] = IdenArray[Index]
				N_Completed_Radi_Keys[N_Completed_Radi_Val] = KeysArray[Index]
				
				ShowRadiQuests = True
				N_Completed_Radi_Val += 1				
			endif
		endif
		
		Index += 1
	EndWhile	
	
	buildpageLayout()	
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

String function Get_QuestOptionKeys(Int val)
	
	Int Index = OptionSlot.Find(val)
	if (Index != -1)
		Return OptionKeys[Index]
	endif
		
	Return ""
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

String function Get_QuestOptionName(Int val)
	
	Int Index = OptionSlot.Find(val)
	if (Index != -1)
		Return OptionName[Index]
	endif
		
	Return ""
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

String function Get_QuestOptionText(Int val)
	
	Int Index = OptionSlot.Find(val)
	if (Index != -1)
		Return OptionHigh[Index]
	endif
		
	Return ""
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

String function Get_QuestOptionDebug(Int val)
	
	Int Index = OptionSlot.Find(val)
	if (Index != -1)
		Return "EditorID - [" + OptionIden[Index] + "] \n Name Set - [" + OptionName[index] + "] \n Data Set - [" + OptionHigh[index] + "] \n Key - [" + OptionKeys[index] + "]"
	endif
		
	Return ""
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

String function Get_QuestOptionIden(Int val)
	
	Int Index = OptionSlot.Find(val)
	if (Index != -1)
		Return OptionIden[Index]
	endif
		
	Return ""
EndFunction
