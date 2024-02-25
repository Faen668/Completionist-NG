Scriptname Completionist_MCMScript extends SKI_ConfigBase Conditional

Import UI
Import Debug
Import Utility
Import Input
Import StringUtil
Import Completionist_Native

; Translators (These are the only script lines that require translating. The rest can be found in 'Completionist_english.txt'
String Property notificationText = "Completionist: Entry Complete - " Auto Hidden
String Property notificationTextBarenziah = "Completionist: Entry Complete - Stone of Barenziah!" Auto Hidden

String Property notificationTextUpdate0 = "Completionist: Running Updates..." Auto Hidden
String Property notificationTextUpdate1 = "Completionist: Update Failed" Auto Hidden
String Property notificationTextUpdate2 = "Completionist: Updated To Version: "  Auto Hidden

String Property notificationTextStartup0 = "Completionist: Start Up Tasks Complete" Auto Hidden
String Property notificationTextStartup1 = "Completionist: Version: " Auto Hidden
String Property notificationTextStartup2 = " - Ready To Use" Auto Hidden

String Property notificationTextShouts0 = "Completionist: " Auto Hidden
String Property notificationTextShouts1 = " - Word 1 (" Auto Hidden
String Property notificationTextShouts2 = " - Word 2 (" Auto Hidden
String Property notificationTextShouts3 = ") - Learnt!" Auto Hidden

String Property notificationTextCellScan0 = " Has " Auto Hidden
String Property notificationTextCellScan1 = " Collectable Items!" Auto Hidden
String Property notificationTextCellScan2 = " Has Collectable Items!" Auto Hidden
String Property notificationTextCellScan3 = "Collectables" Auto Hidden 
String Property notificationTextCellScan4 = "Loose Items" Auto Hidden
String Property notificationTextCellScan5 = "Items On NPC's" Auto Hidden
String Property notificationTextCellScan6 = "Items In Containers" Auto Hidden

;-- Variables ---------------------------------------

Completionist_MCMScript2 Property CompMCM2 Auto
Completionist_MCMScript3 Property COmpMCM3 Auto
Completionist_UpdateScript Property CompVer Auto
Completionist_QuestsScript Property CompQst Auto

GlobalVariable Property Completionist_FishingEnabled Auto
ObjectReference Property Completionist_FishingSpot_Marker Auto
Bool Property FishingSpotMarkers = True Auto Hidden

GlobalVariable Property Completionist_Busy Auto
Quest Property Completionist_Marker Auto
Int Property State_MarkerDetectionVal = 1500 Auto Hidden
ObjectReference Property LocMarker Auto

Int Property State_RadiantCounterVal = 5 Auto Hidden
Int Property State_BountyCounterVal = 5 Auto Hidden
Int Property State_DLC1CounterVal = 5 Auto Hidden
Int Property State_TGRCounterVal = 5 Auto Hidden
Int Property State_LOTDCounterVal = 5 Auto Hidden
Int Property State_VigilantCounterVal = 5 Auto Hidden
Int Property State_CompanionsCounterVal = 5 Auto Hidden
Int Property State_CollegeCounterVal = 5 Auto Hidden
Int Property State_DBCounterVal = 5 Auto Hidden
Int Property State_FishingCounterVal = 4 Auto Hidden
Int Property State_BladesCounterVal = 5 Auto Hidden

Int Property IndexColour = 14905890 Auto Hidden
String Property ColourString = "#00E37222" Auto Hidden
Bool Property NotificationColourEnabled = True Auto Hidden

Int Property State_AutomaticCompletionColourVal = 10398469 Auto Hidden
String Property State_AutomaticCompletionColourString = "#9EAB05" Auto Hidden

Int Property State_ManualCompletionColourVal = 10398469 Auto Hidden
String Property State_ManualCompletionColourString = "#9EAB05" Auto Hidden

Int Property State_MiscCompletionColourVal = 10398469 Auto Hidden
String Property State_MiscCompletionColourString = "#9EAB05" Auto Hidden

Int Property StateShout_NotColourVal = 10027059 Auto Hidden
String Property StateShout_NotColourString Auto Hidden

Int Property StateShout_GotColourVal = 10092543 Auto Hidden
String Property StateShout_GotColourString Auto Hidden

;-----------------------------------------------------------------------------

Bool Property CompletionLog_PrefixEnabled = True Auto Hidden
Bool Property CompletionLog_ColoursEnabled = True Auto Hidden

Int Property CompletionLog_QuestsTextColourVal = 9202896 Auto Hidden
String Property CompletionLog_QuestsTextColourString = "#8C6CD0" Auto Hidden

Int Property CompletionLog_ItemsTextColourVal = 12406553 Auto Hidden
String Property CompletionLog_ItemsTextColourString = "#BD4F19" Auto Hidden

Int Property CompletionLog_BooksTextColourVal = 15379200 Auto Hidden
String Property CompletionLog_BooksTextColourString = "#EAAB00" Auto Hidden

Int Property CompletionLog_SpecialTextColourVal = 10092543 Auto Hidden
String Property CompletionLog_SpecialTextColourString = "#99FFFF" Auto Hidden

;-----------------------------------------------------------------------------

String Property State_OverRide_G_Name_String = "Got It!" Auto Hidden
String Property State_OverRide_N_Name_String = "Need It!" Auto Hidden
Int OID_OverRide_G_Name
Int OID_OverRide_N_Name
Int OID_SearhTerm

Int Property State_ColourVal_G_HUD_Crosshair = 1288220 Auto Hidden
Int Property State_ColourVal_N_HUD_Crosshair = 4430046 Auto Hidden
String Property State_ColourString_G_HUD_Crosshair = "#13a81c" Auto Hidden
String Property State_ColourString_N_HUD_Crosshair = "#4398de" Auto Hidden

Int Property State_ColourVal_G_HUD_Menus = 1288220 Auto Hidden
Int Property State_ColourVal_N_HUD_Menus = 4430046 Auto Hidden
String Property State_ColourString_G_HUD_Menus = "#13a81c" Auto Hidden
String Property State_ColourString_N_HUD_Menus = "#4398de" Auto Hidden

Int OID_CustomColour_G_HUD_Crosshair
Bool Property b_CustomColour_G_HUD_Crosshair Auto Hidden
Int Property State_CustomColourVal_G_HUD_Crosshair = -1 Auto Hidden
String Property State_CustomColourString_G_HUD_Crosshair = "Enter Decimal" Auto Hidden

Int OID_CustomColour_N_HUD_Crosshair
Bool Property b_CustomColour_N_HUD_Crosshair Auto Hidden
Int Property State_CustomColourVal_N_HUD_Crosshair = -1 Auto Hidden
String Property State_CustomColourString_N_HUD_Crosshair = "Enter Decimal" Auto Hidden

Int OID_CustomColour_G_HUD_Menus
Bool Property b_CustomColour_G_HUD_Menus Auto Hidden
Int Property State_CustomColourVal_G_HUD_Menus = -1 Auto Hidden
String Property State_CustomColourString_G_HUD_Menus = "Enter Decimal" Auto Hidden

Int OID_CustomColour_N_HUD_Menus
Bool Property b_CustomColour_N_HUD_Menus Auto Hidden
Int Property State_CustomColourVal_N_HUD_Menus = -1 Auto Hidden
String Property State_CustomColourString_N_HUD_Menus = "Enter Decimal" Auto Hidden

String Property State_SearchTermString = "Enter Search Term..." Auto Hidden
bool property b_SearchIgnoreCompleted Auto Hidden
bool property b_SearchHighlightQuest Auto Hidden
String Property SearchJumpPage Auto Hidden
String Property SearchedMCMMenu Auto Hidden										   
String Property SearchedEntry Auto Hidden
Int property i_SearchMaxResults = 30 Auto Hidden

String[] SearchType
Int State_SearchQueryType
Int Property i_SearchTypeChoice = 0 Auto Hidden
String s_SearchType = "$SearchTypeChoice01"

;-----------------------------------------------------------------------------

String[] Dawnguard_Faction
Int State_Menu_Faction1
Int Property DG_Faction_Choice = 0 Auto Hidden

String[] HelgenReborn_Faction
int State_Menu_Faction3
int property HR_Faction_Choice = 0 Auto Hidden

String[] Legacy_Faction
int State_Menu_Faction4
Int Property Legacy_Faction_Choice = 0 Auto Hidden

String[] Radiant_Quests
int State_Radiant_Quests
Int Property Radiant_Quests_Choice = 0 Auto Hidden
String Radiant_Quests_String = "$RadiantChoice01"

Int State_ShortcutsMenu1
String MainJumpPage
Int Property MainMCMPagesChoice = 0 Auto Hidden

int State_ShortcutsMenu2
String Property MiscJumpPage Auto Hidden
Int Property MiscMCMPagesChoice = 0 Auto Hidden

int State_ShortcutsMenu3
String Property UnofficialJumpPage Auto Hidden
Int Property UnofficialMCMPagesChoice = 0 Auto Hidden

string[] State_AutomaticCompletionView_List
int State_CompletionView
int property State_AutomaticCompletionView_Choice = 0 Auto Hidden

string[] State_ManualCompletionView_List
int State_ManualCompletionView
int property State_ManualCompletionView_Choice = 0 Auto Hidden

string[] State_MiscCompletionView_List
int State_MiscCompletionView
int property State_MiscCompletionView_Choice = 0 Auto Hidden

Bool Property bDebug = False Auto Hidden
Bool Property NotifyItems = True Auto Hidden
Bool Property NotifyBooks = True Auto Hidden
Bool Property NotifySpecial = True Auto Hidden
Bool Property NotifyStartup = True Auto Hidden
Bool Property NotifyMissable = True Auto Hidden

Bool Property TreatBooksAsItems = False Auto Hidden

Bool bGivenProcessingInfo
Bool AutoLoaded
Bool Property bShortCutActive = False Auto Hidden

Bool ShowMainQuests
Bool ShowSideQuests
Bool ShowRadiQuests

Int posRight
Int posLeft
Int posSectionLeft
Int posSectionRight

Int iFound
Int iTotal

;Tagging Options -----------------------------------

String[] InventoryMode_PrAp_List_G
Int InventoryModeOptions_PrAp_G

String[] InventoryMode_PrAp_List_N
Int InventoryModeOptions_PrAp_N

String[] InventoryMode_PrFx_List_G
Int InventoryModeOptions_PrFx_G

String[] InventoryMode_PrFx_List_N
Int InventoryModeOptions_PrFx_N

Int Property InventoryMode_PrAp_Choice_G = 4 Auto Hidden ;;Hooked By DLL
Int Property InventoryMode_PrAp_Choice_N = 4 Auto Hidden ;;Hooked By DLL

Int Property InventoryMode_PrFx_Choice_G = 0 Auto Hidden ;;Hooked By DLL
Int Property InventoryMode_PrFx_Choice_N = 0 Auto Hidden ;;Hooked By DLL

Bool Property b_moreHUDEnabled_Crosshair = True Auto Hidden ;;Hooked By DLL
Bool Property b_moreHUDEnabled_Menus = True Auto Hidden ;;Hooked By DLL
Bool Property b_quickLoot_Enabled = True Auto Hidden ;;Hooked By DLL

String[] OptionName
String[] OptionText
String[] OptionKeys
String[] OptionIden
Int[] OptionSlot
Int[] OptionRadi
Int Property OptionIndx = 0 Auto Hidden

String[] Property NameArray Auto Hidden
String[] Property TextArray Auto Hidden
String[] Property IdenArray Auto Hidden
String[] Property KeysArray Auto Hidden
Int[] Property RadiArray Auto Hidden
	
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

;Cell Scanner Options -----------------------------------
Bool Property bCellScanner_REFS = True Auto Hidden
Bool Property bCellScanner_CONT = True Auto Hidden
Bool Property bCellScanner_NPCS = False Auto Hidden
Bool Property bCellScanner_DETA = False Auto Hidden
Bool Property bCellScanner_NUMB = True Auto Hidden

Bool Property bCellScanner_Pinning_Enabled = True Auto Hidden
Bool Property bCellScanner_Pinning_Marker = True Auto Hidden
Bool Property bCellScanner_Pinning_Effect = True Auto Hidden

Int State_CellScanner_Pinning_Sound
Int Property CellScanner_Pinning_Sound_Choice = 0 Auto Hidden
string[] CellScanner_Pinning_Sound_List

Bool Property bCellScanner_ExcludeAlchemy = False Auto Hidden
Bool Property bCellScanner_ExcludeArmor = False Auto Hidden
Bool Property bCellScanner_ExcludeBooks = False Auto Hidden
Bool Property bCellScanner_ExcludeMiscItems = False Auto Hidden
Bool Property bCellScanner_ExcludeWeapons = False Auto Hidden
Int Property iCellScanner_Range = 100 Auto Hidden
Message Property CompletionistExcludedCellsMessage Auto
Formlist Property Completionist_ExcludedCells Auto

ObjectReference Property PlayerRef Auto
Int[] ExcPos
Int[] ExcRef
Form[] ExcForms

Int Property ReferenceKey = -1 Auto Hidden
Int Property ExcludeKey = -1 Auto Hidden
Int Property ExcludeReferenceKey = -1 Auto Hidden

Int Log_DateSelection = 0
String[] LoggingDates
Int Property LoggingValue = 0 Auto Hidden ;;Hooked By DLL

Quest Property Completionist_ItemFinder_Quest Auto
ReferenceAlias Property Completionist_ItemFinder_Item Auto
ReferenceAlias Property Completionist_ItemFinder_Name Auto
ReferenceAlias Property Completionist_ItemFinder_Type Auto
ObjectReference Property CompletionistItemString Auto
ObjectReference Property CompletionistTypeString Auto

String Property TargetName Auto Hidden
String Property TargetType Auto Hidden
Form Property TargetForm Auto Hidden
Form Property LastTargettedForm Auto Hidden

Quest Property Completionist_PinnedItemFinder_Quest Auto
ReferenceAlias Property Completionist_PinnedItemFinder_Item Auto
ReferenceAlias Property Completionist_PinnedItemFinder_Name Auto
ReferenceAlias Property Completionist_PinnedItemFinder_Type Auto
ObjectReference Property CompletionistPinnedItemString Auto
ObjectReference Property CompletionistPinnedTypeString Auto

ObjectReference Property PinnedRefr Auto Hidden
String Property PinnedName Auto Hidden
String Property PinnedType Auto Hidden
Form Property PinnedForm Auto Hidden

Bool Property bCellScanner_MARK Auto Hidden
Formlist Property Completionist_PinnedItemSoundList Auto

float keyHoldDuration

ObjectReference Property lastReference Auto Hidden
Bool Property bCellScanner_UseClosestReference = True Auto Hidden

;---------------------------------------------------
;-- START OF CODE ----------------------------------
;---------------------------------------------------
	
Event OnConfigInit()
	
	if (!AutoLoaded)
		AutoLoadConfig()
	endIf
	
	UnRegisterForAllKeys()
EndEvent

;---------------------------------------------------
;-- Events -----------------------------------------
;---------------------------------------------------

Bool Function IsMenuModeActive()
	return \
		IsMenuOpen("Dialogue Menu") 	|| \
		IsMenuOpen("Console") 			|| \
		IsMenuOpen("MessageBoxMenu") 	|| \
		IsMenuOpen("MagicMenu") 		|| \
		IsMenuOpen("TweenMenu")			|| \
		IsMenuOpen("BarterMenu")		|| \
		IsMenuOpen("GiftMenu")			|| \
		IsMenuOpen("MapMenu")			|| \
		IsMenuOpen("Lockpicking Menu")	|| \
		IsMenuOpen("Quantity Menu")		|| \
		IsMenuOpen("StatsMenu")			|| \
		IsMenuOpen("ContainerMenu")		|| \
		IsMenuOpen("Sleep/Wait Menu")	|| \
		IsMenuOpen("Journal Menu")		|| \
		IsMenuOpen("Book Menu")			|| \
		IsMenuOpen("FavoritesMenu")		|| \
		IsMenuOpen("RaceSex Menu")		|| \
		IsMenuOpen("Crafting Menu")		|| \
		IsMenuOpen("Training Menu")
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function ClearQuestTarget()
	Completionist_ItemFinder_Item.Clear()
	Completionist_ItemFinder_Name.Clear()
	Completionist_ItemFinder_Type.Clear()
	TargetName = ""
	TargetType = ""
	TargetForm = None
endfunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function ClearPinnedTarget(Bool a_userDisabled)
	(Completionist_PinnedItemFinder_Item as Completionist_PinnedItemScript).StopGlow()
	Completionist_PinnedItemFinder_Item.Clear()
	Completionist_PinnedItemFinder_Name.Clear()
	Completionist_PinnedItemFinder_Type.Clear()
	PinnedName = ""
	PinnedType = ""
	
	if (a_userDisabled)
		PinnedForm = None
	endIf
endfunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

String Function GetPinnedItemName()
	if (!PinnedForm || !bCellScanner_Pinning_Enabled)
		return "No Active Pin..."
	endIf
	
	return PinnedForm.GetName()
endFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function SwitchPinnedTarget(Form a_form, String a_name)
	ClearPinnedTarget(true)
	
	if (!a_form)
		Return
	endIf
	
	PinnedForm = a_form
	PinnedName = a_name
	
	if (HasPinnedFormInCell(PlayerRef.GetParentCell(), PinnedForm))
		PinnedRefr = GetPinnedReferenceRefr(PinnedForm)
		PinnedName = GetPinnedReferenceName(PinnedForm)
		PinnedType = GetPinnedReferenceType(PinnedForm)

		CompletionistPinnedItemString.GetBaseObject().SetName(PinnedName)
		CompletionistPinnedTypeString.GetBaseObject().SetName(PinnedType)
	
		Completionist_PinnedItemFinder_Item.ForceRefTo(PinnedRefr)
		Completionist_PinnedItemFinder_Name.ForceRefTo(CompletionistPinnedItemString)
		Completionist_PinnedItemFinder_Type.ForceRefTo(CompletionistPinnedTypeString)
		(Completionist_PinnedItemFinder_Item as Completionist_PinnedItemScript).SetDisplayparamters(PinnedForm, bCellScanner_Pinning_Effect, CellScanner_Pinning_Sound_Choice)
		
		if (bCellScanner_Pinning_Marker)
			Completionist_PinnedItemFinder_Quest.SetObjectiveDisplayed(10, True, True)
		endIf
	endIf
endfunction

;---------------------------------------------------
;-- Events -----------------------------------------
;---------------------------------------------------

Event OnKeyDown(Int KeyCode)
	
	if (IsInMenuMode() || IsInActualMenuMode() || IsMenuModeActive() )
		return
	endIf 

	if (KeyCode == ExcludeReferenceKey)
		ObjectReference curRef = Completionist_ItemFinder_Item.GetReference() as ObjectReference
		
		if (!curRef)
			SendNotification(GetLocStringByKeyExt("CellScanner_NoReference"), ColourString, NotificationColourEnabled)
			return
		endIf

		SendNotification(GetLocStringByKeyExt("CellScanner_ExcludeSuccess"), ColourString, NotificationColourEnabled)
		ExcludeReference(curRef, curRef.GetParentCell())
		ClearQuestTarget()
		Completionist_ItemFinder_Quest.SetObjectiveDisplayed(10, False, True)
		return
	endIf

	if (KeyCode == ReferenceKey)
		ObjectReference curRef = Completionist_ItemFinder_Item.GetReference() as ObjectReference
		ObjectReference pinRef = Completionist_PinnedItemFinder_Item.GetReference() as ObjectReference
		if (curRef || pinRef)
			if (curRef)
				ClearQuestTarget()
				Completionist_ItemFinder_Quest.SetObjectiveDisplayed(10, False, False)
			endIf

			if (pinRef)
				ClearPinnedTarget(false)
				Completionist_PinnedItemFinder_Quest.SetObjectiveDisplayed(10, False, False)
			endIf
			return
		endIf

		cell curCell = PlayerRef.GetParentCell()

		if (!curCell)
			return
		endIf

		if (Completionist_ExcludedCells.HasForm(curCell) || isCellExcluded(curCell))
			SendNotification(GetLocStringByKeyExt("CellScanner_ExcludedCell"), ColourString, NotificationColourEnabled)
			return
		endIf
			
		if (!bCellScanner_MARK)
			CheckForReferences(curCell, false, true)
		Else
			ClearQuestTarget()
			ClearPinnedTarget(false)

			Completionist_ItemFinder_Quest.SetObjectiveDisplayed(10, False, True)
			Completionist_PinnedItemFinder_Quest.SetObjectiveDisplayed(10, False, True)
			
			curRef = GetTargetReferenceRefr(curCell, LastTargettedForm)
			if (!curRef)
				if (curCell.IsInterior())
					SendNotification(curCell.GetName() + GetLocStringByKeyExt("CellScanner_NoCollectables"), ColourString, NotificationColourEnabled)
				else
					SendNotification(GetLocStringByKeyExt("CellScanner_ExteriorCellPrefix") + GetLocStringByKeyExt("CellScanner_NoCollectables"), ColourString, NotificationColourEnabled)
				endIf
				return
			endIf

			TargetForm = GetTargetReferenceForm()
			TargetName = GetTargetReferenceName()
			TargetType = GetTargetReferenceType()
			LastTargettedForm = TargetForm

			CompletionistItemString.GetBaseObject().SetName(TargetName)
			CompletionistTypeString.GetBaseObject().SetName(TargetType)
			
			Completionist_ItemFinder_Item.ForceRefTo(curRef)
			Completionist_ItemFinder_Name.ForceRefTo(CompletionistItemString)
			Completionist_ItemFinder_Type.ForceRefTo(CompletionistTypeString)
			Completionist_ItemFinder_Quest.SetObjectiveDisplayed(10, True, True)
		endIf
		
		pinRef = Completionist_PinnedItemFinder_Item.GetReference() as ObjectReference
		if (!pinRef)
			if (HasPinnedFormInCell(curCell, PinnedForm))
				PinnedRefr = GetPinnedReferenceRefr(PinnedForm)
				PinnedName = GetPinnedReferenceName(PinnedForm)
				PinnedType = GetPinnedReferenceType(PinnedForm)

				CompletionistPinnedItemString.GetBaseObject().SetName(PinnedName)
				CompletionistPinnedTypeString.GetBaseObject().SetName(PinnedType)
			
				Completionist_PinnedItemFinder_Item.ForceRefTo(PinnedRefr)
				Completionist_PinnedItemFinder_Name.ForceRefTo(CompletionistPinnedItemString)
				Completionist_PinnedItemFinder_Type.ForceRefTo(CompletionistPinnedTypeString)

				if (bCellScanner_Pinning_Marker)
					Completionist_PinnedItemFinder_Quest.SetObjectiveDisplayed(10, True, True)
				endIf
				(Completionist_PinnedItemFinder_Item as Completionist_PinnedItemScript).SetDisplayparamters(PinnedForm, bCellScanner_Pinning_Effect, CellScanner_Pinning_Sound_Choice)
			endIf
		endIf
	endIf
	
	if (KeyCode == ExcludeKey)
		Cell curCell = PlayerRef.GetParentCell()		
		if (!Completionist_ExcludedCells.HasForm(curCell))
			Int x = CompletionistExcludedCellsMessage.Show()
			if (x == 0)
				Completionist_ExcludedCells.AddForm(curCell as Cell)
			endIf
		endIf
	endIf
EndEvent

;---------------------------------------------------
;-- Events -----------------------------------------
;---------------------------------------------------

Function ResetMarkerOnLoad()
	ObjectReference curRef = Completionist_ItemFinder_Item.GetReference() as ObjectReference
	if (curRef)
		Completionist_ItemFinder_Name.Clear()
		Completionist_ItemFinder_Type.Clear()

		CompletionistItemString.GetBaseObject().SetName(TargetName)
		CompletionistTypeString.GetBaseObject().SetName(TargetType)

		Completionist_ItemFinder_Name.ForceRefTo(CompletionistItemString)
		Completionist_ItemFinder_Type.ForceRefTo(CompletionistTypeString)
		return
	endIf

	ObjectReference pinRef = Completionist_PinnedItemFinder_Item.GetReference() as ObjectReference
	if (pinRef)
		Completionist_PinnedItemFinder_Name.Clear()
		Completionist_PinnedItemFinder_Type.Clear()

		CompletionistPinnedItemString.GetBaseObject().SetName(PinnedName)
		CompletionistPinnedTypeString.GetBaseObject().SetName(PinnedType)

		Completionist_PinnedItemFinder_Name.ForceRefTo(CompletionistPinnedItemString)
		Completionist_PinnedItemFinder_Type.ForceRefTo(CompletionistPinnedTypeString)
		(Completionist_PinnedItemFinder_Item as Completionist_PinnedItemScript).SetDisplayparamters(PinnedForm, bCellScanner_Pinning_Effect, CellScanner_Pinning_Sound_Choice)
		return
	endIf
endFunction

;---------------------------------------------------
;-- Events -----------------------------------------
;---------------------------------------------------

Event OnConfigOpen()

	pages = GetMCMPages(0)
	CompMCM2.pages = GetMCMPages(1)
	CompMCM3.pages = GetMCMPages(2)
	
	if (bShortCutActive) && (MainJumpPage != "")
		if (ShowMessage("$JumpMessage{" + MainJumpPage + "}", true, "$ConfirmYes", "$ConfirmNo"))
			Jump(MainJumpPage, 0)
		endIf
	endIf
EndEvent

;---------------------------------------------------
;-- Events -----------------------------------------
;---------------------------------------------------

Event OnPageReset(String page)
	
	pages = GetMCMPages(0)
	Build_Menus()	  
		
	if (CurrentPage == "$MCMPageSettings")
		GoToState("")
		Build_Page_Settings()

	elseif (CurrentPage == "$MCMPageSettings2")
		GoToState("")
		Build_Page_Settings2()

	elseif (CurrentPage == "$MCMPageSettings3")
		GoToState("")
		Build_Page_Settings3()

	elseif (CurrentPage == "$MCMPageSettings4")
		GoToState("ExcludedCellState")
		Build_Page_Settings4()

	elseif (CurrentPage == "$MCMPageSettings5")
		GoToState("")
		Build_Page_Settings5()
		
	else
		if (!Completionist_Busy.GetValue())
			GoToState("Quest_TrackingState")
			CompQst._Build_Quests(CurrentPage)
		else
			self.ShowMessage("$BusyMessage", false, "$ConfirmY")
		endIf
	endIf
EndEvent

;---------------------------------------------------
;-- Events -----------------------------------------
;---------------------------------------------------

Event OnOptionHighlight(Int val)
	
	if (val == OID_OverRide_G_Name)
		SetInfoText("$State_OverRide_G_Name_Info")	
		return
	endIf

	if (val == OID_OverRide_N_Name)
		SetInfoText("$State_OverRide_N_Name_Info")	
		return
	endIf

	if (val == OID_CustomColour_G_HUD_Crosshair)
		SetInfoText("$ModNotificationsCustomInfoG_Crosshair")	
		return
	endIf

	if (val == OID_CustomColour_N_HUD_Crosshair)
		SetInfoText("$ModNotificationsCustomInfoN_Crosshair")	
		return
	endIf

	if (val == OID_CustomColour_G_HUD_Menus)
		SetInfoText("$ModNotificationsCustomInfoG_Menus")	
		return
	endIf

	if (val == OID_CustomColour_N_HUD_Menus)
		SetInfoText("$ModNotificationsCustomInfoN_Menus")	
		return
	endIf

	if (val == OID_SearhTerm)
		SetInfoText("$State_SearchTerm_Info")
	endIf
EndEvent

;---------------------------------------------------
;-- Events -----------------------------------------
;---------------------------------------------------

Event OnOptionInputOpen(Int val)
	
	if (val == OID_OverRide_G_Name)
		SetInputDialogStartText(State_OverRide_G_Name_String)
		return
	endIf

	if (val == OID_OverRide_N_Name)
		SetInputDialogStartText(State_OverRide_N_Name_String)
		return
	endIf

	if (val == OID_CustomColour_G_HUD_Crosshair)
		SetInputDialogStartText(State_CustomColourString_G_HUD_Crosshair)
		return
	endIf

	if (val == OID_CustomColour_N_HUD_Crosshair)
		SetInputDialogStartText(State_CustomColourString_N_HUD_Crosshair)
		return
	endIf

	if (val == OID_CustomColour_G_HUD_Menus)
		SetInputDialogStartText(State_CustomColourString_G_HUD_Menus)
		return
	endIf

	if (val == OID_CustomColour_N_HUD_Menus)
		SetInputDialogStartText(State_CustomColourString_N_HUD_Menus)
		return
	endIf

	if (val == OID_SearhTerm)
		SetInputDialogStartText(State_SearchTermString)
	endIf
EndEvent

;---------------------------------------------------
;-- Events -----------------------------------------
;---------------------------------------------------

Event OnOptionInputAccept(Int val, String HexString)

	if (val == OID_OverRide_G_Name)
		OverRide_G_Name(HexString)
		return
	endIf

	if (val == OID_OverRide_N_Name)
		OverRide_N_Name(HexString)
		return
	endIf

	if (val == OID_CustomColour_G_HUD_Crosshair)
		OverRide_G_Colour_Crosshair(HexString)
		return
	endIf

	if (val == OID_CustomColour_N_HUD_Crosshair)
		OverRide_N_Colour_Crosshair(HexString)
		return
	endIf

	if (val == OID_CustomColour_G_HUD_Menus)
		OverRide_G_Colour_Menus(HexString)
		return
	endIf

	if (val == OID_CustomColour_N_HUD_Menus)
		OverRide_N_Colour_Menus(HexString)
		return
	endIf

	if (val == OID_SearhTerm)
		ProcessSearchResults(HexString)
		return
	endIf
EndEvent

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function Jump(string sString = "", int jumpType = 0)

	if (sString != "")
		SetTitleText(sString)
	endIf
	
	if (jumpType == 0)
		SendModEvent("SKICP_pageSelected", pages[pages.Find(MainJumpPage)], pages.Find(MainJumpPage))
		return
	endIf

	if (jumpType == 1)
		if (SearchedMCMMenu == "Misc" && CompMCM2.pages.Find(SearchJumpPage) != -1)
			SendModEvent("SKICP_modSelected", 0, GetSkyUIMCMPositionalIndex("Completionist: Tracker (Misc)"))
			SendModEvent("SKICP_pageSelected", SearchJumpPage, CompMCM2.pages.Find(SearchJumpPage))
			return
		endIf

		if (SearchedMCMMenu == "Misc" && CompMCM3.pages.Find(SearchJumpPage) != -1)
			COmpMCM3.SearchActive = true
			SendModEvent("SKICP_modSelected", 0, GetSkyUIMCMPositionalIndex("Completionist: Tracker (Patches)"))
			SendModEvent("SKICP_pageSelected", SearchJumpPage, CompMCM3.pages.Find(SearchJumpPage))
			return
		endIf

		SendModEvent("SKICP_pageSelected", SearchJumpPage, pages.Find(SearchJumpPage))
		return
	endIf

	if (jumpType == 2)
		SendModEvent("SKICP_pageSelected", pages[pages.Find(currentpage)], pages.Find(currentpage))
	endIf
endFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------													
Function ProcessSearchResults(String term)
	
	if (GetStringLength(term) < 3)
		ShowMessage("$ResultsMessageErrorOnLength", false)
		return
	endIf
	
	string[] results = SearchAndReportPage(term, b_SearchIgnoreCompleted, i_SearchMaxResults, i_SearchTypeChoice);
	
	if (!results.length)
		ShowMessage("$ResultsMessage{" + term + "}", false)
	endIf
	
	int x = 0
	while (x < results.length)
		if (ShowMessage(results[x], true, "$GoToPage", "$Continue"))
			SearchJumpPage = results[x + 1]
			SearchedEntry  = results[x + 2]
			SearchedMCMMenu = results[x + 3]					   
			Jump(SearchJumpPage, 1)
			return
		endIf
		x += 4
	endWhile
endFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

function Build_Menus()

	InventoryMode_PrAp_List_G = new string[8]
	InventoryMode_PrAp_List_G[0] = "$InventoryMode_PrAp_Menu0"
	InventoryMode_PrAp_List_G[1] = "$InventoryMode_PrAp_Menu1"
	InventoryMode_PrAp_List_G[2] = "$InventoryMode_PrAp_Menu2"
	InventoryMode_PrAp_List_G[3] = "$InventoryMode_PrAp_Menu3"
	InventoryMode_PrAp_List_G[4] = "$InventoryMode_PrAp_Menu4"
	InventoryMode_PrAp_List_G[5] = "$InventoryMode_PrAp_Menu5"
	InventoryMode_PrAp_List_G[6] = "$InventoryMode_PrAp_Menu6"
	InventoryMode_PrAp_List_G[7] = "$InventoryMode_PrAp_Menu7"

	InventoryMode_PrAp_List_N = new string[8]
	InventoryMode_PrAp_List_N[0] = "$InventoryMode_PrAp_Menu0"
	InventoryMode_PrAp_List_N[1] = "$InventoryMode_PrAp_Menu1"
	InventoryMode_PrAp_List_N[2] = "$InventoryMode_PrAp_Menu2"
	InventoryMode_PrAp_List_N[3] = "$InventoryMode_PrAp_Menu3"
	InventoryMode_PrAp_List_N[4] = "$InventoryMode_PrAp_Menu4"
	InventoryMode_PrAp_List_N[5] = "$InventoryMode_PrAp_Menu5"
	InventoryMode_PrAp_List_N[6] = "$InventoryMode_PrAp_Menu6"
	InventoryMode_PrAp_List_N[7] = "$InventoryMode_PrAp_Menu7"

	InventoryMode_PrFx_List_G = new string[3]
	InventoryMode_PrFx_List_G[0] = "$InventoryMode_PrFx_Menu0"
	InventoryMode_PrFx_List_G[1] = "$InventoryMode_PrFx_Menu1"
	InventoryMode_PrFx_List_G[2] = "$InventoryMode_PrFx_Menu2"

	InventoryMode_PrFx_List_N = new string[3]
	InventoryMode_PrFx_List_N[0] = "$InventoryMode_PrFx_Menu0"
	InventoryMode_PrFx_List_N[1] = "$InventoryMode_PrFx_Menu1"
	InventoryMode_PrFx_List_N[2] = "$InventoryMode_PrFx_Menu2"
	
;---------------------------------------------------
	
	State_AutomaticCompletionView_List = new string[3]
	State_AutomaticCompletionView_List[0] = "$QuestCompletionViewString0"
	State_AutomaticCompletionView_List[1] = "$QuestCompletionViewString1"
	State_AutomaticCompletionView_List[2] = "$QuestCompletionViewString2"
	
;---------------------------------------------------
	
	State_ManualCompletionView_List = new string[3]
	State_ManualCompletionView_List[0] = "$QuestCompletionViewString3"
	State_ManualCompletionView_List[1] = "$QuestCompletionViewString4"
	State_ManualCompletionView_List[2] = "$QuestCompletionViewString2"

;---------------------------------------------------
	
	State_MiscCompletionView_List = new string[3]
	State_MiscCompletionView_List[0] = "$QuestCompletionViewString0"
	State_MiscCompletionView_List[1] = "$QuestCompletionViewString1"
	State_MiscCompletionView_List[2] = "$QuestCompletionViewString2"

;---------------------------------------------------
	
	Dawnguard_Faction = new String[3]
	Dawnguard_Faction[0] = "$MenuChoiceDefault"
	Dawnguard_Faction[1] = "$MenuChoice03"
	Dawnguard_Faction[2] = "$MenuChoice04"
	
;---------------------------------------------------

	SearchType = new String[3]
	SearchType[0] = "$SearchTypeChoice01"
	SearchType[1] = "$SearchTypeChoice02"
	SearchType[2] = "$SearchTypeChoice03"

;---------------------------------------------------

	CellScanner_Pinning_Sound_List = New String[3]
	CellScanner_Pinning_Sound_List[0] = "$CellScannerSoundList0"
	CellScanner_Pinning_Sound_List[1] = "$CellScannerSoundList1"
	CellScanner_Pinning_Sound_List[2] = "$CellScannerSoundList2"
;---------------------------------------------------
	
	Int x = 0	
	Radiant_Quests = new String[10]
	Radiant_Quests[x] = "$RadiantChoice01"
	x += 1
	Radiant_Quests[x] = "$RadiantChoice02"
	x += 1
	Radiant_Quests[x] = "$RadiantChoice08"
	x += 1
	Radiant_Quests[x] = "$RadiantChoice07"
	x += 1	
	Radiant_Quests[x] = "$RadiantChoice09"
	x += 1	
	Radiant_Quests[x] = "$RadiantChoice03"
	x += 1
	Radiant_Quests[x] = "$RadiantChoice04"
	x += 1
	Radiant_Quests[x] = "$RadiantChoice11"
	x += 1
	if (Game.GetModByName("ccbgssse001-fish.esm") != 255)
		Radiant_Quests[x] = "$RadiantChoice10"
		x += 1
	endIf
	
	if (Game.GetModByName("LegacyoftheDragonborn.esm") != 255)
		Radiant_Quests[x] = "$RadiantChoice05"
		x += 1
	endIf
	
	if (Game.GetModByName("Vigilant.esm") != 255)
		Radiant_Quests[x] = "$RadiantChoice06"
		x += 1
	endIf
	
;---------------------------------------------------
	
	Legacy_Faction = new string[4]
	Legacy_Faction[0] = "$MenuChoiceDefault"
	Legacy_Faction[1] = "$MenuChoice05"
	if (Game.GetModByName("Alternate Start - Live Another Life.esp") != 255) || (Game.GetModByName("AlternatePerspective.esp") != 255)
		Legacy_Faction[2] = "$MenuChoice06"
		if Game.GetModByName("DBM_RelicHunter.esp") != 255
			Legacy_Faction[3] = "$MenuChoice07"
		endIf
	endIf

;---------------------------------------------------
	
	HelgenReborn_Faction = new string[4]
	HelgenReborn_Faction[0] = "$MenuChoiceDefault"
	HelgenReborn_Faction[1] = "$MenuChoice08"
	HelgenReborn_Faction[2] = "$MenuChoice10"
	HelgenReborn_Faction[3] = "$MenuChoice09"
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

String function GetEnabledStatus(Bool bValue)

	if (bValue)
		Return "$Enabled"
	endIf

	Return "$Disabled"
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

String function GetClosestReferenceEnabledStatus(Bool bValue)

	if (bValue)
		Return "$Closest"
	endIf

	Return "$Random"
EndFunction


;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

function Build_Page_Settings()
	
	if (CurrentPage == "$MCMPageSettings")
		SetCursorFillMode(TOP_TO_BOTTOM)
		SetCursorPosition(0)			
		AddHeaderOption("$MCMPageSettingsHeader")
		AddMenuOptionST("State_Radiant_Quests", 			"$State_RadiantQuestsText",   		Radiant_Quests[Radiant_Quests_Choice])
		if (Radiant_Quests_String == "$RadiantChoice01")
			AddSliderOptionST("State_RadiantCounter", 	   	"$State_RadiantCounterText",		State_RadiantCounterVal, "{0}", 0)
			
		elseif (Radiant_Quests_String == "$RadiantChoice02")
			AddSliderOptionST("State_BountyCounter", 	   	"$State_BountyCounterText",			State_BountyCounterVal, "{0}", 0)
			
		elseif (Radiant_Quests_String == "$RadiantChoice08")
			AddSliderOptionST("State_CollegeCounter", 	"$State_CollegeCounterText",			State_CollegeCounterVal, "{0}", 0)
			
		elseif (Radiant_Quests_String == "$RadiantChoice07")
			AddSliderOptionST("State_CompanionsCounter", 	"$State_CompanionsCounterText",		State_CompanionsCounterVal, "{0}", 0)

		elseif (Radiant_Quests_String == "$RadiantChoice09")
			AddSliderOptionST("State_DBCounter", 			"$State_DBCounterText",				State_DBCounterVal, "{0}", 0)
			
		elseif (Radiant_Quests_String == "$RadiantChoice03")
			AddSliderOptionST("State_TGRCounter", 	   		"$State_TGRCounterText",			State_TGRCounterVal, "{0}", 0)
			
		elseif (Radiant_Quests_String == "$RadiantChoice04")
			AddSliderOptionST("State_DLC1Counter", 	   		"$State_DLC1CounterText",			State_DLC1CounterVal, "{0}", 0)

		elseif (Radiant_Quests_String == "$RadiantChoice10")
			AddSliderOptionST("State_FishingCounter", 	   	"$State_FishingCounterText",			State_FishingCounterVal, "{0}", 0)
			
		elseif (Radiant_Quests_String == "$RadiantChoice05")
			AddSliderOptionST("State_LOTDCounter", 	   		"$State_LOTDCounterText",			State_LOTDCounterVal, "{0}", 0)
			
		elseif (Radiant_Quests_String == "$RadiantChoice06")
			AddSliderOptionST("State_VigilantCounter", 		"$State_VigilantCounterText",		State_VigilantCounterVal, "{0}", 0)

		elseif (Radiant_Quests_String == "$RadiantChoice11")
			AddSliderOptionST("State_TheBladesCounter", 	"$State_TheBladesCounterText",		State_BladesCounterVal, "{0}", 0)
		endIf
		
		AddEmptyOption()
		OID_SearhTerm = AddInputOption("$State_SearchTerm_Text", State_SearchTermString, 0)
		AddTextOptionST("TreatBooksAsItemsState", "$TreatBooksAsItemsState_Text", GetEnabledStatus(TreatBooksAsItems), 0)
		AddTextOptionST("State_Debugging", "$State_DebuggingText", GetEnabledStatus(bDebug), 0)
		
		AddEmptyOption()
		AddHeaderOption("$MCMPageSettingsHeader2")
		AddMenuOptionST("State_Menu_Faction1",  		 "$DGFaction",   				Dawnguard_Faction[DG_Faction_Choice])
		if Game.GetModByName("Helgen Reborn.esp") != 255
			AddMenuOptionST("State_Menu_Faction3", 	"$HRFaction", 			HelgenReborn_Faction[HR_Faction_Choice])
		else
			AddTextOption("$HRFaction", "$NOFaction", 0)
		endIf
		if (Game.GetModByName("LegacyoftheDragonborn.esm") != 255)
			AddMenuOptionST("State_Menu_Faction4", "$LDFaction", 			Legacy_Faction[Legacy_Faction_Choice])
		else
			AddTextOption("$LDFaction", "$NOFaction", 0)
		endIf	
		
		if (bDebug)
			AddEmptyOption()
			AddHeaderOption("Developer Debug")
			AddTextOptionST("DumpQuestData", "Dump Quest Data", "Dump", 0)
			AddTextOption("(Thunderchild Stamina State: (" + (((Game.GetPlayer().GetActorValue("Stamina") as Float / Game.GetPlayer().GetBaseActorValue("Stamina") as Float * 100 as Float)  as Int) as String) + "%)", "", 0)
			AddTextOption("(Colours = Hex: " + ColourString + " Int: " + IndexColour + ")", "", 0)
		endIf
		
		SetCursorPosition(1)		
		AddHeaderOption("")
		AddTextOption("$AuthorThanks1", "", 0)
		AddTextOption("$AuthorThanks2", "", 0)
		AddEmptyOption()
		AddTextOption("", "$ModVersion{ " + CompVer.ModVersion + "}", 0)
		AddTextOption("", "$DLLVersion{ " + Completionist_Native.GetVersion() + "}", 0)
		AddTextOption("", "$PatchesInstalled{ " + Completionist_Native.GetPatchCount() + "}", 0)
		AddEmptyOption()
		AddHeaderOption("$State_ProfileHead")
		AddTextOptionST("State_ProfileSave", 			"$State_ProfileText1", 			GetConfigSaveString(), 0)
		AddTextOptionST("State_ProfileLoad", 			"$State_ProfileText1", 			GetConfigLoadString(), 0)
		AddTextOptionST("State_ProfileReset",			"$State_ProfileText2", 			"$State_ProfileText3", 0)	

	endIf
EndFunction
			
;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

function Build_Page_Settings2()

	if (CurrentPage == "$MCMPageSettings2")
		SetCursorFillMode(TOP_TO_BOTTOM)
		
		SetCursorPosition(0)
		AddHeaderOption("$NotificationSettingsText")
		AddTextOptionST("ModNotificationsItems", 	 		"$ModNotificationsItems", 			GetEnabledStatus(NotifyItems),  	0)
		AddTextOptionST("ModNotificationsBooks", 	 		"$ModNotificationsBooks", 			GetEnabledStatus(NotifyBooks), 		0)
		AddTextOptionST("ModNotificationsColourEnabled",	"$ModNotificationsColourEnabled", 	GetEnabledStatus(NotificationColourEnabled),  0)
		AddEmptyOption()

		AddHeaderOption("$State_MapMarkerSettingsHead")
		AddSliderOptionST("State_MarkerDetection", 	   	"$State_MarkerDetectionText",		State_MarkerDetectionVal, "{0}", 0)
		AddTextOptionST("State_MarkerDetectionCancel", 	"$State_MarkerDetectionCancelText", "$ConfirmN",  0)
		
		if (!Completionist_FishingEnabled.GetValue())
			AddTextOptionST("State_FishingSpotMarkers",		"$State_FishingSpotMarkers_Text", 	GetEnabledStatus(FishingSpotMarkers),  1)
		else
			AddTextOptionST("State_FishingSpotMarkers",		"$State_FishingSpotMarkers_Text", 	GetEnabledStatus(FishingSpotMarkers),  0)
		endIf
		
		AddEmptyOption()
		AddEmptyOption()
		AddHeaderOption("$MCMPageSettingsHeader3")
		AddTextOptionST("State_SearchIgnoreCompleted", "$State_SearchIgnoreCompleted_Text", GetEnabledStatus(b_SearchIgnoreCompleted), 0)
		AddTextOptionST("State_SearchHighlightQuest",  "$State_SearchHighlightQuest_Text",  GetEnabledStatus(b_SearchHighlightQuest), 0)
		
		AddEmptyOption()
		AddHeaderOption("$State_PageSettingsHead_QuestPage")
		AddMenuOptionST("State_CompletionView", 	 		"$State_AutomaticCompletionView_Text", 	State_AutomaticCompletionView_List[State_AutomaticCompletionView_Choice], 0)
		AddColorOptionST("State_CompletionColour",  		"$State_AutomaticCompletionView_Text", 	State_AutomaticCompletionColourVal, 0)

		AddEmptyOption()
		AddHeaderOption("$State_PageSettingsHead_MiscPage")
		AddMenuOptionST("State_MiscCompletionView", 	 	"$State_MiscCompletionView_Text", 	State_MiscCompletionView_List[State_MiscCompletionView_Choice], 0)
		AddColorOptionST("State_MiscCompletionColour",  	"$State_MiscCompletionView_Text", 	State_MiscCompletionColourVal, 0)		

		AddEmptyOption()
		AddHeaderOption("$MCMPageSettingsHeader4")
		AddTextOptionST("CompletionLog_ColoursEnabledState", 		"$CompletionLog_ColoursEnabledText", 	GetEnabledStatus(CompletionLog_ColoursEnabled), 0)
		if (!CompletionLog_ColoursEnabled)
			AddColorOptionST("CompletionLog_QuestsTextColourState", 	"$CompletionLog_QuestsTextColourText", 	CompletionLog_QuestsTextColourVal, 1)
			AddColorOptionST("CompletionLog_ItemsTextColourState",  	"$CompletionLog_ItemsTextColourText", 	CompletionLog_ItemsTextColourVal, 1)
		Else
			AddColorOptionST("CompletionLog_QuestsTextColourState", 	"$CompletionLog_QuestsTextColourText", 	CompletionLog_QuestsTextColourVal, 0)
			AddColorOptionST("CompletionLog_ItemsTextColourState",  	"$CompletionLog_ItemsTextColourText", 	CompletionLog_ItemsTextColourVal, 0)
		endIf
		SetCursorPosition(1)
		
		AddHeaderOption("")
		AddTextOptionST("ModNotificationsSpecial", 	 		"$ModNotificationsSpecial", 		GetEnabledStatus(NotifySpecial),  	0)
		AddTextOptionST("ModNotificationsStartup", 	 		"$ModNotificationsStartup", 		GetEnabledStatus(NotifyStartup), 	0)
		if (NotificationColourEnabled)
			AddColorOptionST("ModNotificationsColour",  "$ModNotificationsColour", 			IndexColour, 0)
		else
			AddColorOptionST("ModNotificationsColour",  "$ModNotificationsColour", 			IndexColour, 1)
		endIf
		AddEmptyOption()
		
		AddHeaderOption("$State_ShortcutsHead")
		AddTextOptionST("State_Shortcuts", 	 			"$State_ShortcutsText",			GetEnabledStatus(bShortCutActive), 	0)
		if (bShortCutActive)
			AddMenuOptionST("State_ShortcutsMenu1",  	"$State_ShortcutsMainText",			pages[MainMCMPagesChoice], 	0)
			AddMenuOptionST("State_ShortcutsMenu2", 	"$State_ShortcutsMiscText",			CompMCM2.pages[MiscMCMPagesChoice], 	0)
			AddMenuOptionST("State_ShortcutsMenu3", 	"$State_ShortcutsUnofficialText",	CompMCM3.pages[UnofficialMCMPagesChoice], 	0)
		else			
			AddMenuOptionST("State_ShortcutsMenu1",  	"$State_ShortcutsMainText",			pages[MainMCMPagesChoice], 	1)
			AddMenuOptionST("State_ShortcutsMenu2", 	"$State_ShortcutsMiscText",			CompMCM2.pages[MiscMCMPagesChoice], 	1)
			AddMenuOptionST("State_ShortcutsMenu3", 	"$State_ShortcutsUnofficialText",	CompMCM3.pages[UnofficialMCMPagesChoice], 	1)
		endIf
		
		AddEmptyOption()
		AddHeaderOption("")
		AddMenuOptionST("State_SearchQueryType", 	"$State_SearchQueryType_Text",   	SearchType[i_SearchTypeChoice])
		AddSliderOptionST("State_SearchMaxResults", "$State_SearchMaxResults_Text",	i_SearchMaxResults, "{0}", 0)

		AddEmptyOption()
		AddHeaderOption("")
		AddMenuOptionST("State_ManualCompletionView", 	 	"$State_ManualCompletionView_Text", State_ManualCompletionView_List[State_ManualCompletionView_Choice], 0)
		AddColorOptionST("State_ManualCompletionColour",  	"$State_ManualCompletionView_Text", State_ManualCompletionColourVal, 0)

		AddEmptyOption()
		AddHeaderOption("$State_PageSettingsHead_ShoutsPage")				
		AddColorOptionST("StateShout_GotColour",   "$StateShout_GotColourText", StateShout_GotColourVal, 0)		
		AddColorOptionST("StateShout_NotColour",   "$StateShout_NotColourText",	StateShout_NotColourVal, 0)

		AddEmptyOption()
		AddHeaderOption("")
		AddTextOptionST("CompletionLog_PrefixEnabledState", 			"$CompletionLog_PrefixEnabledText", 		GetEnabledStatus(CompletionLog_PrefixEnabled), 0)
		if (!CompletionLog_ColoursEnabled)
			AddColorOptionST("CompletionLog_BooksTextColourState", 		"$CompletionLog_BooksTextColourText", 		CompletionLog_BooksTextColourVal, 1)
			AddColorOptionST("CompletionLog_SpecialTextColourState",  	"$CompletionLog_SpecialTextColourText", 	CompletionLog_SpecialTextColourVal, 1)
		Else
			AddColorOptionST("CompletionLog_BooksTextColourState", 		"$CompletionLog_BooksTextColourText", 		CompletionLog_BooksTextColourVal, 0)
			AddColorOptionST("CompletionLog_SpecialTextColourState",  	"$CompletionLog_SpecialTextColourText", 	CompletionLog_SpecialTextColourVal, 0)
		endIf
	endIf
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

function Build_Page_Settings3()

	if (CurrentPage == "$MCMPageSettings3")
		SetCursorFillMode(TOP_TO_BOTTOM)
		SetCursorPosition(0)

		AddHeaderOption("$MainHUDMode_Header_N")
		OID_OverRide_N_Name = AddInputOption("$State_OverRide_N_Name_Text", 						State_OverRide_N_Name_String, 0)
		AddColorOptionST("ColourState_N_HUD_Crosshair", "$ColourState_N_Generic_Text_Crosshair", State_ColourVal_N_HUD_Crosshair, (b_CustomColour_N_HUD_Crosshair as Bool) as Int)
		OID_CustomColour_N_HUD_Crosshair = AddInputOption("$ModNotificationsCustomTextN_Crosshair", GetFontOption(State_CustomColourString_N_HUD_Crosshair, b_CustomColour_N_HUD_Crosshair))

		AddEmptyOption()
		AddHeaderOption("$MenuHUDMode_Header_N")
		AddMenuOptionST("InventoryModeOptions_PrAp_N", 	"$InventoryModeOptions_PrAp_Text_N", 			InventoryMode_PrAp_List_N[InventoryMode_PrAp_Choice_N], 0)
		AddMenuOptionST("InventoryModeOptions_PrFx_N", 	"$InventoryModeOptions_PrFx_Text_N", 			InventoryMode_PrFx_List_N[InventoryMode_PrFx_Choice_N], 0)
		AddColorOptionST("ColourState_N_HUD_Menus", "$ColourState_N_Generic_Text_Menus", State_ColourVal_N_HUD_Menus, (b_CustomColour_N_HUD_Menus as Bool) as Int)
		OID_CustomColour_N_HUD_Menus = AddInputOption("$ModNotificationsCustomTextN_Menus", GetFontOption(State_CustomColourString_N_HUD_Menus, b_CustomColour_N_HUD_Menus))

		AddEmptyOption()
		AddHeaderOption("$moreHUD_Header")
		AddTextOptionST("State_moreHUDEnabled_Crosshair", 	 	"$State_moreHUDEnabled_Crosshair_Text", GetEnabledStatus(b_moreHUDEnabled_Crosshair),  	0)
		AddTextOptionST("State_moreHUDEnabled_Menus", 	 		"$State_moreHUDEnabled_Menus_Text", 	GetEnabledStatus(b_moreHUDEnabled_Menus),  	0)
		
		SetCursorPosition(1)

		AddHeaderOption("$MainHUDMode_Header_G")		
		OID_OverRide_G_Name = AddInputOption("$State_OverRide_G_Name_Text", 						State_OverRide_G_Name_String, 0)
		AddColorOptionST("ColourState_G_HUD_Crosshair", "$ColourState_G_Generic_Text_Crosshair", State_ColourVal_G_HUD_Crosshair, (b_CustomColour_G_HUD_Crosshair as Bool) as Int)
		OID_CustomColour_G_HUD_Crosshair = AddInputOption("$ModNotificationsCustomTextG_Crosshair", GetFontOption(State_CustomColourString_G_HUD_Crosshair, b_CustomColour_G_HUD_Crosshair))
		
		AddEmptyOption()
		AddHeaderOption("$MenuHUDMode_Header_G")
		AddMenuOptionST("InventoryModeOptions_PrAp_G", 	"$InventoryModeOptions_PrAp_Text_G", 			InventoryMode_PrAp_List_G[InventoryMode_PrAp_Choice_G], 0)
		AddMenuOptionST("InventoryModeOptions_PrFx_G", 	"$InventoryModeOptions_PrFx_Text_G", 			InventoryMode_PrFx_List_G[InventoryMode_PrFx_Choice_G], 0)
		AddColorOptionST("ColourState_G_HUD_Menus", "$ColourState_G_Generic_Text_Menus", State_ColourVal_G_HUD_Menus, (b_CustomColour_G_HUD_Menus as Bool) as Int)
		OID_CustomColour_G_HUD_Menus = AddInputOption("$ModNotificationsCustomTextG_Menus", GetFontOption(State_CustomColourString_G_HUD_Menus, b_CustomColour_G_HUD_Menus))	

		AddEmptyOption()
		AddHeaderOption("")
		AddTextOptionST("State_quickLootEnabled", 	 	"$State_quickLootEnabled_Text", GetEnabledStatus(b_quickLoot_Enabled),  	0)
		AddEmptyOption()
	endIf
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

function Build_Page_Settings4()
	
	int curPos = 0
	ExcPos = New Int[128]
	ExcRef = New Int[128]
	ExcForms = New Form[128]

	int arrPos = 0
	int idx = 0
	
	if (CurrentPage == "$MCMPageSettings4")
		ObjectReference QuestMarkerReference = Completionist_ItemFinder_Item.GetReference() as ObjectReference
		
		SetCursorFillMode(TOP_TO_BOTTOM)
		
		SetCursorPosition(curPos)
		AddHeaderOption("$CellScanner_SettingsHeader1")
		curPos += 2

		SetCursorPosition(curPos)
		AddKeyMapOptionST("CheckForReferences_State1", "$CheckForReferences_Text1", ReferenceKey, OPTION_FLAG_WITH_UNMAP)
		curPos += 2
		
		SetCursorPosition(curPos)
		AddKeyMapOptionST("CheckForReferences_State2", "$CheckForReferences_Text2", ExcludeKey, OPTION_FLAG_WITH_UNMAP)
		curPos += 2

		SetCursorPosition(curPos)
		AddKeyMapOptionST("CheckForReferences_State3", "$CheckForReferences_Text3", ExcludeReferenceKey, OPTION_FLAG_WITH_UNMAP)
		curPos += 2

		SetCursorPosition(curPos)
		AddEmptyOption()
		curPos += 2		

		SetCursorPosition(curPos)
		AddHeaderOption("$CellScanner_SettingsHeader5")
		curPos += 2
		
		SetCursorPosition(curPos)
		AddTextOptionST("CellScanner_Setting_Pinning_Enabled", "$CellScanner_Setting_Pinning_Enabled_Text", GetEnabledStatus(bCellScanner_Pinning_Enabled), 0)
		curPos += 2	

		SetCursorPosition(curPos)
		AddTextOptionST("CellScanner_Setting_Pinning_Marker", 	"$CellScanner_Setting_Pinning_Marker_Text", GetEnabledStatus(bCellScanner_Pinning_Marker), ((!bCellScanner_Pinning_Enabled)) as int)
		curPos += 2	

		SetCursorPosition(curPos)
		AddTextOption("$CellScanner_CurrentPin", GetPinnedItemName(), ((!bCellScanner_Pinning_Enabled)) as int)
		curPos += 2	
		
		SetCursorPosition(curPos)
		AddEmptyOption()
		curPos += 2	

		SetCursorPosition(curPos)
		AddHeaderOption("$CellScanner_SettingsHeader2")
		curPos += 2		

		SetCursorPosition(curPos)
		AddTextOptionST("CellScanner_Setting_REFS", 	"$CellScanner_Setting_REFS_Text",		GetEnabledStatus(bCellScanner_REFS), 0)
		curPos += 2		

		SetCursorPosition(curPos)
		AddTextOptionST("CellScanner_Setting_CONT", 	"$CellScanner_Setting_CONT_Text",		GetEnabledStatus(bCellScanner_CONT), 0)
		curPos += 2		

		SetCursorPosition(curPos)
		AddTextOptionST("CellScanner_Setting_NPCS", 	"$CellScanner_Setting_NPCS_Text",		GetEnabledStatus(bCellScanner_NPCS), 0)
		curPos += 2		

		SetCursorPosition(curPos)
		AddSliderOptionST("CellScanner_Setting_Range", 	 "$CellScanner_Setting_Range_Text",		iCellScanner_Range, "{0}", 0)
		curPos += 2

		SetCursorPosition(curPos)
		AddEmptyOption()
		curPos += 2		

		SetCursorPosition(curPos)
		AddHeaderOption("$CellScanner_SettingsHeader4")
		curPos += 2		

		SetCursorPosition(curPos)
		AddTextOptionST("CellScanner_Setting_ExcludeAlchemy", 	"$CellScanner_Setting_ExcludeAlchemy_Text",		GetEnabledStatus(bCellScanner_ExcludeAlchemy), 0)
		curPos += 2	
		
		SetCursorPosition(curPos)
		AddTextOptionST("CellScanner_Setting_ExcludeArmor", 	"$CellScanner_Setting_ExcludeArmor_Text",		GetEnabledStatus(bCellScanner_ExcludeArmor), 0)
		curPos += 2	
		
		SetCursorPosition(curPos)
		AddTextOptionST("CellScanner_Setting_ExcludeBooks", 	"$CellScanner_Setting_ExcludeBooks_Text",		GetEnabledStatus(bCellScanner_ExcludeBooks), 0)
		curPos += 2	
		
		SetCursorPosition(curPos)
		AddEmptyOption()
		curPos += 2	
		
		SetCursorPosition(curPos)
		AddHeaderOption("$CellScanner_ExcludedHeader")
		curPos += 2				

		if (Completionist_ExcludedCells.GetSize() < 1)
			SetCursorPosition(curPos)
			AddTextOption("$CellScanner_Setting_EXCL_None", "", 1)
			curPos += 2	
		else
			idx = 0
			SetCursorFillMode(LEFT_TO_RIGHT)
			while idx < Completionist_ExcludedCells.GetSize()
				SetCursorPosition(curPos)
				ExcPos[arrPos] = AddTextOption(Completionist_ExcludedCells.GetAt(idx).Getname(), "", 0)
				ExcRef[arrPos] = 0
				ExcForms[arrPos] = Completionist_ExcludedCells.GetAt(idx) as form
				idx += 1
				curPos += 1
				arrPos += 1
			endWhile

			if (curPos % 2 != 0)
				curPos += 1
			endIf
		endIf
		SetCursorFillMode(TOP_TO_BOTTOM)
		
		SetCursorPosition(curPos)
		AddEmptyOption()
		curPos += 2

		SetCursorPosition(curPos)
		AddHeaderOption("$CellScanner_ExcludedReferencesHeader")

		Cell currentCell = PlayerRef.GetParentCell()
		string[] formIDs = GetReferenceFormIDs(currentCell)
		string[] names = GetReferenceNames(currentCell)
		ObjectReference[] references = GetObjectReferences(currentCell)
		
		if (formIDs.length == 0)
			AddTextOption("$CellScanner_Setting_EXRF_None", "", 1)
		else
			idx = 0
			SetCursorFillMode(LEFT_TO_RIGHT)

			while idx < Min(formIDs.length, 126)
				ExcPos[arrPos] = AddTextOption(formIDs[idx] + " - " + names[idx], "", 0)
				ExcRef[arrPos] = 1
				ExcForms[arrPos] = references[idx] as form
				
				idx += 1
				arrPos += 1
			endWhile
		endIf
		
		SetCursorFillMode(TOP_TO_BOTTOM)
		SetCursorPosition(1)
		AddHeaderOption("$CellScanner_SettingsHeader3")
		AddTextOption("FormID:", GetQuestMarkerReferenceFormID(QuestMarkerReference), 0)
		AddTextOption("Ref In:", GetQuestMarkerReferenceOwner(QuestMarkerReference), 0)
		if (QuestMarkerReference && Substring(GetQuestMarkerReferenceFormID(QuestMarkerReference), 0, 2) != "FF")
			AddTextOptionST("CellScanner_Setting_EXRF", 	"$CellScanner_Setting_EXRF_Text", "", 0)
		else
			AddTextOption("$CellScanner_Setting_EXRF_TextERROR", "", 1)
		endIf
		AddEmptyOption()

		AddHeaderOption("")
		AddMenuOptionST("State_CellScanner_Pinning_Sound", "$CellScanner_Pinning_Sound_Text", CellScanner_Pinning_Sound_List[CellScanner_Pinning_Sound_Choice], ((!bCellScanner_Pinning_Enabled)) as int)
		AddTextOptionST("CellScanner_Setting_Pinning_Effect", 	"$CellScanner_Setting_Pinning_Effect_Text", GetEnabledStatus(bCellScanner_Pinning_Effect), ((!bCellScanner_Pinning_Enabled)) as int)
		AddTextOptionST("CellScanner_Setting_Pinning_ClearPin", "", "$CellScanner_Setting_Pinning_ClearPin_Text", ((!bCellScanner_Pinning_Enabled)) as int)
		AddEmptyOption()

		AddHeaderOption("")
		AddTextOptionST("CellScanner_Setting_NUMB", 	"$CellScanner_Setting_NUMB_Text",		GetEnabledStatus(bCellScanner_NUMB), 0)
		AddTextOptionST("CellScanner_Setting_DETA", 	"$CellScanner_Setting_DETA_Text",		GetEnabledStatus(bCellScanner_DETA), 0)
		AddTextOptionST("CellScanner_Setting_Quest", 	"$CellScanner_Setting_Quest_Text",		GetEnabledStatus(bCellScanner_MARK), 0)
		AddTextOptionST("CellScanner_UseClosestReferenceState", "$CellScanner_USeClosestReferenceState_Text", GetClosestReferenceEnabledStatus(bCellScanner_UseClosestReference), ((!bCellScanner_MARK)) as int)
		AddEmptyOption()
		
		AddHeaderOption("")
		AddTextOptionST("CellScanner_Setting_ExcludeMiscItems", 	"$CellScanner_Setting_ExcludeMiscItems_Text",		GetEnabledStatus(bCellScanner_ExcludeMiscItems), 0)
		AddTextOptionST("CellScanner_Setting_ExcludeWeapons", 	"$CellScanner_Setting_ExcludeWeapons_Text",		GetEnabledStatus(bCellScanner_ExcludeWeapons), 0)
		if (PlayerRef.GetParentCell().IsInterior())
			AddTextOptionST("CellScanner_Setting_EXCL", 	"$CellScanner_Setting_EXCL_Text{" + PlayerRef.GetParentCell().GetName() + "}", "", 0)
		else
			AddTextOption("$CellScanner_Setting_EXCL_TextERROR", "", 1)
		endIf
		AddEmptyOption()
		
		AddHeaderOption("")
		
		SetCursorPosition(curPos + 1)
		AddHeaderOption("")
	endIf
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

int function Min(int first, int second)

	if first < second
		return first
	endIf

	return second
endfunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

function Build_Page_Settings5()

	if (CurrentPage == "$MCMPageSettings5")
		LoggingDates = GetLoggingDates()
		
		SetCursorFillMode(TOP_TO_BOTTOM)
		SetCursorPosition(0)

		AddHeaderOption("$Log_Header")
		SetCursorPosition(1)
		AddMenuOptionST("Log_DateSelection", 	"$Log_DateSelection_Text", LoggingDates[LoggingValue], 0)
		
		String[] events = GetLoggedEventsForDate(LoggingDates[LoggingValue], CompletionLog_PrefixEnabled, CompletionLog_ColoursEnabled, CompletionLog_QuestsTextColourString, CompletionLog_ItemsTextColourString, CompletionLog_BooksTextColourString, CompletionLog_SpecialTextColourString)
		
		int y = 2
		int x = 0
		if events.length < 1
			SetCursorPosition(y)
			AddTextOption("$EmptyLogView", "", 1)
		Else
			while (x < Min(events.length, 126))
				SetCursorPosition(y)
				AddTextOption(events[x], "", 0)
				x += 1
				y += 1
			endWhile
		endIf
	endIf
EndFunction

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state Log_DateSelection

	event OnMenuOpenST()
		SetMenuDialogStartIndex(LoggingValue)
		SetMenuDialogDefaultIndex(0)
		SetMenuDialogOptions(LoggingDates)
	endEvent

	event OnMenuAcceptST(int index)
		LoggingValue = Index
		SetMenuOptionValueST(Log_DateSelection, LoggingDates[LoggingValue])				
		ForcePageReset()
	endEvent

	event OnDefaultST()
		LoggingValue = 0
		SetMenuOptionValueST(LoggingDates[LoggingValue])
	endEvent

	event OnHighlightST()
		SetInfoText("$Log_DateSelection_Info")
	endEvent
endState

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function GetColourStatus(String quest_name, String quest_key = "", bool search_quest = false)
	
	if (search_quest)
		SearchedEntry = ""
		SearchJumpPage = ""
		SearchedMCMMenu = ""			  
		OptionSlot[OptionIndx] = AddTextOption("$SearchResult{" + "#FFD966" + "}{" + quest_name + "}", "", 0)
		return
	endIf
	
	if (qIsOptionToggled(CompQst.CurrentQuestID, quest_key))		
		if (State_ManualCompletionView_Choice == 0)
		
			OptionSlot[OptionIndx] = AddTextOption("<font color='" + State_ManualCompletionColourString + "'>" + quest_name + "</font>", "(M)", 0)
			
		elseif (State_ManualCompletionView_Choice == 1)
			OptionSlot[OptionIndx] = AddTextOption(quest_name, "(M)", 0)
		
		elseif (State_ManualCompletionView_Choice == 2)
			OptionSlot[OptionIndx] = AddTextOption("<font color='" + State_ManualCompletionColourString + "'>" + quest_name + "</font>", "", 0)
		endIf
	else
		if (State_AutomaticCompletionView_Choice == 0)
			OptionSlot[OptionIndx] = AddToggleOption("<font color='" + State_AutomaticCompletionColourString + "'>" + quest_name + "</font>", True, 0)
			
		elseif (State_AutomaticCompletionView_Choice == 1)
			OptionSlot[OptionIndx] = AddToggleOption(quest_name, True, 0)
		
		elseif (State_AutomaticCompletionView_Choice == 2)
			OptionSlot[OptionIndx] = AddTextOption("<font color='" + State_AutomaticCompletionColourString + "'>" + quest_name + "</font>", "", 0)
		endIf
	endIf
endFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

function buildpageLayout()
		
	SetCursorFillMode(LEFT_TO_RIGHT)
	posLeft = 0
	posRight = 1	
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
	
	
	SetTitleText("~ "  + iFound + "/" + iTotal + " - " + (((iFound as Float / iTotal as Float * 100 as Float)  as Int) as String) + "% Completed ~")
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
			endIf
		else
			While Index < NameArr.length && NameArr[Index] != ""
				SetCursorPosition(posLeft)
				
				if (NameArr[Index] == SearchedEntry && b_SearchHighlightQuest)
					GetColourStatus(NameArr[Index], KeysArr[Index], true)
				else
					OptionSlot[OptionIndx] = AddTextOption(NameArr[Index], "", 0)
				endIf
				
				OptionName[OptionIndx] = NameArr[Index]
				OptionText[OptionIndx] = TextArr[Index]
				OptionIden[OptionIndx] = IdenArr[Index]
				OptionKeys[OptionIndx] = KeysArr[Index]
				OptionIndx += 1
				posLeft += 2
				Index += 1
			EndWhile
		endIf
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
			endIf
		else
			While Index < NameArr.length && NameArr[Index] != ""
				SetCursorPosition(posRight)
				GetColourStatus(NameArr[Index], KeysArr[Index])
				OptionName[OptionIndx] = NameArr[Index]
				OptionText[OptionIndx] = TextArr[Index]
				OptionIden[OptionIndx] = IdenArr[Index]
				OptionKeys[OptionIndx] = KeysArr[Index]
				OptionIndx += 1
				posRight += 2
				Index += 1
			EndWhile
		endIf
	endIf
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
	OptionText = New String[128]
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
	iTotal = 0
	iFound = 0
	
	Int Index = 0
	While Index < NameArray.length && (NameArray[Index] != "")
		
		if (qIsOptionCompleted(CompQst.CurrentQuestID, KeysArray[Index]) > 0 || qIsOptionToggled(CompQst.CurrentQuestID, KeysArray[Index]))

			if (RadiArray[Index] == 0)
				
				Y_Completed_Main_Name[Y_Completed_Main_Val] = NameArray[Index]
				Y_Completed_Main_Text[Y_Completed_Main_Val] = TextArray[Index]
				Y_Completed_Main_Iden[Y_Completed_Main_Val] = IdenArray[Index]
				Y_Completed_Main_Keys[Y_Completed_Main_Val] = KeysArray[Index]
				
				ShowMainQuests = True
				Y_Completed_Main_Val += 1
				iTotal += 1
				iFound += 1
				
			elseif (RadiArray[Index] == 1)
				
				Y_Completed_Side_Name[Y_Completed_Side_Val] = NameArray[Index]
				Y_Completed_Side_Text[Y_Completed_Side_Val] = TextArray[Index]
				Y_Completed_Side_Iden[Y_Completed_Side_Val] = IdenArray[Index]
				Y_Completed_Side_Keys[Y_Completed_Side_Val] = KeysArray[Index]
				
				ShowSideQuests = True
				Y_Completed_Side_Val += 1
				iTotal += 1
				iFound += 1
				
			elseif (RadiArray[Index] == 2)
				
				Y_Completed_Radi_Name[Y_Completed_Radi_Val] = NameArray[Index]
				Y_Completed_Radi_Text[Y_Completed_Radi_Val] = TextArray[Index] + "\n" + qGetTimesCompletedVsTimesRequiredText(KeysArray[Index])
				Y_Completed_Radi_Iden[Y_Completed_Radi_Val] = IdenArray[Index]
				Y_Completed_Radi_Keys[Y_Completed_Radi_Val] = KeysArray[Index]
				
				ShowRadiQuests = True
				Y_Completed_Radi_Val += 1
				iTotal += 1
				iFound += 1
				
			endIf
			
		else
		
			if (RadiArray[Index] == 0)
				
				N_Completed_Main_Name[N_Completed_Main_Val] = NameArray[Index]
				N_Completed_Main_Text[N_Completed_Main_Val] = TextArray[Index]
				N_Completed_Main_Iden[N_Completed_Main_Val] = IdenArray[Index]
				N_Completed_Main_Keys[N_Completed_Main_Val] = KeysArray[Index]
				
				ShowMainQuests = True
				N_Completed_Main_Val += 1
				iTotal += 1
				
			elseif (RadiArray[Index] == 1)
			
				N_Completed_Side_Name[N_Completed_Side_Val] = NameArray[Index]
				N_Completed_Side_Text[N_Completed_Side_Val] = TextArray[Index]
				N_Completed_Side_Iden[N_Completed_Side_Val] = IdenArray[Index]
				N_Completed_Side_Keys[N_Completed_Side_Val] = KeysArray[Index]
				
				ShowSideQuests = True
				N_Completed_Side_Val += 1
				iTotal += 1
				
			elseif (RadiArray[Index] == 2)
			
				N_Completed_Radi_Name[N_Completed_Radi_Val] = NameArray[Index]
				N_Completed_Radi_Text[N_Completed_Radi_Val] = TextArray[Index] + "\n" + qGetTimesCompletedVsTimesRequiredText(KeysArray[Index])
				N_Completed_Radi_Iden[N_Completed_Radi_Val] = IdenArray[Index]
				N_Completed_Radi_Keys[N_Completed_Radi_Val] = KeysArray[Index]
				
				ShowRadiQuests = True
				N_Completed_Radi_Val += 1
				iTotal += 1
				
			endIf
		endIf
		
		Index += 1
	EndWhile	
	
	buildpageLayout()	
EndFunction		

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

String function Get_OptionKeys(Int val)
	
	Int Index = OptionSlot.Find(val)
	if (Index != -1)
		Return OptionKeys[Index]
	endIf
		
	Return ""
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

String function Get_OptionName(Int val)
	
	Int Index = OptionSlot.Find(val)
	if (Index != -1)
		Return OptionName[Index]
	endIf
		
	Return ""
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

String function Get_OptionText(Int val)
	
	Int Index = OptionSlot.Find(val)
	if (Index != -1)
		Return OptionText[Index]
	endIf
		
	Return ""
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

String function Get_OptionDebug(Int val)
	
	Int Index = OptionSlot.Find(val)
	if (Index != -1)
		Return "EditorID - [" + OptionIden[Index] + "] \n Name Set - [" + OptionName[index] + "] \n Data Set - [" + OptionText[index] + "] \n Key - [" + OptionKeys[index] + "]"
	endIf
		
	Return ""
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

String function Get_OptionIden(Int val)
	
	Int Index = OptionSlot.Find(val)
	if (Index != -1)
		Return OptionIden[Index]
	endIf
		
	Return ""
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

String function GetConfigSaveString()
	
	if papyrusutil.GetScriptVersion() > 31
		return "$SavePreset"
	endIf
	
	return "$PapUtilError"
endFunction	

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

String function GetConfigLoadString()

	if papyrusutil.GetScriptVersion() > 31
		return "$LoadPreset"
	endIf
	
	return "$PapUtilError"
endFunction	

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function Begin_Config_Save()

	if papyrusutil.GetScriptVersion() > 31
		
		;;Search Settings
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_SearchIgnoreCompleted", b_SearchIgnoreCompleted as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_SearchHighlightQuest", b_SearchHighlightQuest as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!i_SearchMaxResults", i_SearchMaxResults)	
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!i_SearchTypeChoice", i_SearchTypeChoice)	
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!s_SearchType", s_SearchType)	
		
		;;Factions
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!DG_Faction_Choice", DG_Faction_Choice)	
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!HR_Faction_Choice", HR_Faction_Choice)	
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!Legacy_Faction_Choice", Legacy_Faction_Choice)	
		
		;;Shortcuts
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bShortCutActive", bShortCutActive as Int)
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!MainJumpPage", MainJumpPage)	
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!MiscJumpPage", MiscJumpPage)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!MainMCMPagesChoice", MainMCMPagesChoice)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!MiscMCMPagesChoice", MiscMCMPagesChoice)
		
		;;General Settings
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!NotifyItems", NotifyItems as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!NotifyBooks", NotifyBooks as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!NotifySpecial", NotifySpecial as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!NotifyStartup", NotifyStartup as Int)
		
		;;Misc Settings
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_MarkerDetectionVal", State_MarkerDetectionVal)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_RadiantCounterVal", State_RadiantCounterVal)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_BountyCounterVal", State_BountyCounterVal)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_TGRCounterVal", State_TGRCounterVal)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_DLC1CounterVal", State_DLC1CounterVal)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_LOTDCounterVal", State_LOTDCounterVal)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_VigilantCounterVal", State_VigilantCounterVal)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_CompanionsCounterVal", State_CompanionsCounterVal)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_CollegeCounterVal", State_CollegeCounterVal)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_DBCounterVal", State_DBCounterVal)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_FishingCounterVal", State_FishingCounterVal)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_BladesCounterVal", State_BladesCounterVal)

		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!FishingSpotMarkers", FishingSpotMarkers as Int)	
		
		;;Colour Options
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!NotificationColourEnabled", NotificationColourEnabled as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!IndexColour", IndexColour)
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!ColourString", ColourString)	

		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_AutomaticCompletionColourVal", State_AutomaticCompletionColourVal)
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_AutomaticCompletionColourString", State_AutomaticCompletionColourString)	

		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!StateShout_NotColourVal", StateShout_NotColourVal)
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!StateShout_NotColourString", StateShout_NotColourString)	

		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!StateShout_GotColourVal", StateShout_GotColourVal)
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!StateShout_GotColourString", StateShout_GotColourString)	

		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_BooksTextColourVal", CompletionLog_BooksTextColourVal)
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_BooksTextColourString", CompletionLog_BooksTextColourString)	

		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_ItemsTextColourVal", CompletionLog_BooksTextColourVal)
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_ItemsTextColourString", CompletionLog_BooksTextColourString)	

		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_SpecialTextColourVal", CompletionLog_BooksTextColourVal)
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_SpecialTextColourString", CompletionLog_BooksTextColourString)	

		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_QuestsTextColourVal", CompletionLog_BooksTextColourVal)
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_QuestsTextColourString", CompletionLog_BooksTextColourString)	
		
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_ColoursEnabled", CompletionLog_ColoursEnabled as Int)	
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_PrefixEnabled", CompletionLog_PrefixEnabled as Int)	
		
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourVal_G_HUD_Crosshair", State_ColourVal_G_HUD_Crosshair)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourVal_N_HUD_Crosshair", State_ColourVal_N_HUD_Crosshair)
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourString_G_HUD_Crosshair", State_ColourString_G_HUD_Crosshair)	
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourString_N_HUD_Crosshair", State_ColourString_N_HUD_Crosshair)	

		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourVal_G_HUD_Menus", State_ColourVal_G_HUD_Menus)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourVal_N_HUD_Menus", State_ColourVal_N_HUD_Menus)
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourString_G_HUD_Menus", State_ColourString_G_HUD_Menus)	
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourString_N_HUD_Menus", State_ColourString_N_HUD_Menus)	
		
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_OverRide_G_Name_String", State_OverRide_G_Name_String)	
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_OverRide_N_Name_String", State_OverRide_N_Name_String)	
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!InventoryMode_PrAp_Choice_N", InventoryMode_PrAp_Choice_N)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!InventoryMode_PrAp_Choice_G", InventoryMode_PrAp_Choice_G)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!InventoryMode_PrFx_Choice_N", InventoryMode_PrFx_Choice_N)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!InventoryMode_PrFx_Choice_G", InventoryMode_PrFx_Choice_G)
		
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_CustomColour_G_HUD_Crosshair", b_CustomColour_G_HUD_Crosshair as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_CustomColour_N_HUD_Crosshair", b_CustomColour_N_HUD_Crosshair as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourVal_G_HUD_Crosshair", State_CustomColourVal_G_HUD_Crosshair)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourVal_N_HUD_Crosshair", State_CustomColourVal_N_HUD_Crosshair)
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourString_G_HUD_Crosshair", State_CustomColourString_G_HUD_Crosshair)	
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourString_N_HUD_Crosshair", State_CustomColourString_N_HUD_Crosshair)	

		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_CustomColour_G_HUD_Menus", b_CustomColour_G_HUD_Menus as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_CustomColour_N_HUD_Menus", b_CustomColour_N_HUD_Menus as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourVal_G_HUD_Menus", State_CustomColourVal_G_HUD_Menus)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourVal_N_HUD_Menus", State_CustomColourVal_N_HUD_Menus)
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourString_G_HUD_Menus", State_CustomColourString_G_HUD_Menus)	
		jsonutil.SetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourString_N_HUD_Menus", State_CustomColourString_N_HUD_Menus)	
		
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_CONT", bCellScanner_CONT as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_DETA", bCellScanner_DETA as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_NPCS", bCellScanner_NPCS as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_NUMB", bCellScanner_NUMB as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_REFS", bCellScanner_REFS as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_REFS", bCellScanner_MARK as Int)
		
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_ExcludeAlchemy", bCellScanner_ExcludeAlchemy as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_ExcludeArmor", bCellScanner_ExcludeArmor as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_ExcludeBooks", bCellScanner_ExcludeBooks as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_ExcludeMiscItems", bCellScanner_ExcludeMiscItems as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_ExcludeWeapons", bCellScanner_ExcludeWeapons as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!iCellScanner_Range", iCellScanner_Range)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_UseClosestReference", bCellScanner_UseClosestReference as Int)

		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_moreHUDEnabled_Crosshair", b_moreHUDEnabled_Crosshair as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_moreHUDEnabled_Menus", b_moreHUDEnabled_Menus as Int)
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_quickLoot_Enabled", b_quickLoot_Enabled as Int)
		
		jsonutil.SetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!TreatBooksAsItems", TreatBooksAsItems as Int)

		jsonutil.Save("../CompletionistData/Profiles/CompConfig", false)
		if IsInMenuMode()
			ShowMessage("$ProfileSaveSuccessMenu")
			ForcePageReset()
		endIf
	else
		ShowMessage("$PapUtilError")
		ForcePageReset()
	endIf
EndFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function Begin_Config_Load()

	if papyrusutil.GetScriptVersion() > 31
		if jsonutil.JsonExists("../CompletionistData/Profiles/CompConfig")
			if !jsonutil.IsGood("../CompletionistData/Profiles/CompConfig")
				if IsInMenuMode()
					ShowMessage("$ProfileLoadDamaged{" + jsonutil.GetErrors("../CompletionistData/Profiles/CompConfig") + "}", false, "$ConfirmY", "$ConfirmN")
					return
				else
					SendNotification("$ProfileLoadCorrupt", ColourString, NotificationColourEnabled)
					Begin_Config_Default()
					return 
				endIf
			endIf
			
			;;Search Settings
			b_SearchIgnoreCompleted = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_SearchIgnoreCompleted", b_SearchIgnoreCompleted as Int))
			b_SearchHighlightQuest = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_SearchHighlightQuest", b_SearchHighlightQuest as Int))
			i_SearchMaxResults = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!i_SearchMaxResults", i_SearchMaxResults))
			i_SearchTypeChoice = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!i_SearchTypeChoice", i_SearchTypeChoice))
			s_SearchType = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!s_SearchType", s_SearchType))
			
			;;Factions
			HR_Faction_Choice = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!HR_Faction_Choice", HR_Faction_Choice))
			DG_Faction_Choice = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!DG_Faction_Choice", DG_Faction_Choice))
			Legacy_Faction_Choice = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!Legacy_Faction_Choice", Legacy_Faction_Choice))

			;;Shortcuts
			bShortCutActive = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bShortCutActive", bShortCutActive as Int))
			MainJumpPage = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!MainJumpPage", MainJumpPage))
			MiscJumpPage = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!MiscJumpPage", MiscJumpPage))
			MainMCMPagesChoice = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!MainMCMPagesChoice", MainMCMPagesChoice))
			MiscMCMPagesChoice = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!MiscMCMPagesChoice", MiscMCMPagesChoice))
			if (!bShortCutActive)
				MainJumpPage = ""
				MiscJumpPage = ""
				MainMCMPagesChoice = 0
				MiscMCMPagesChoice = 0
			endIf
			
			;;General Settings
			NotifySpecial = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!NotifySpecial", NotifySpecial as Int))
			NotifyStartup = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!NotifyStartup", NotifyStartup as Int))
			NotifyBooks = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!NotifyBooks", NotifyBooks as Int))
			NotifyItems = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!NotifyItems", NotifyItems as Int))

			;;Misc Settings
			State_MarkerDetectionVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_MarkerDetectionVal", State_MarkerDetectionVal))
			State_RadiantCounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_RadiantCounterVal", State_RadiantCounterVal))
			State_BountyCounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_BountyCounterVal", State_BountyCounterVal))
			State_TGRCounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_TGRCounterVal", State_TGRCounterVal))
			State_DLC1CounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_DLC1CounterVal", State_DLC1CounterVal))
			State_LOTDCounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_LOTDCounterVal", State_LOTDCounterVal))
			State_VigilantCounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_VigilantCounterVal", State_VigilantCounterVal))
			State_CompanionsCounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_CompanionsCounterVal", State_CompanionsCounterVal))
			State_CollegeCounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_CollegeCounterVal", State_CollegeCounterVal))
			State_DBCounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_DBCounterVal", State_DBCounterVal))
			State_FishingCounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_FishingCounterVal", State_FishingCounterVal))
			State_BladesCounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_BladesCounterVal", State_BladesCounterVal))

			FishingSpotMarkers = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!FishingSpotMarkers", FishingSpotMarkers as Int))
			if (FishingSpotMarkers && Completionist_FishingEnabled.GetValue())
				Completionist_FishingSpot_Marker.Enable()
			else
				Completionist_FishingSpot_Marker.Disable()
				FishingSpotMarkers = false		
			endIf
				
			;;Notification Colours
			NotificationColourEnabled = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!NotificationColourEnabled", NotificationColourEnabled as Int))
			IndexColour = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!IndexColour", IndexColour))
			ColourString = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!ColourString", ColourString))

			StateShout_GotColourVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!StateShout_GotColourVal", StateShout_GotColourVal))
			StateShout_NotColourVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!StateShout_NotColourVal", StateShout_NotColourVal))
			StateShout_GotColourString = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!StateShout_GotColourString", StateShout_GotColourString))
			StateShout_NotColourString = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!StateShout_NotColourString", StateShout_NotColourString))
			Framework_UpdateShouts()

			CompletionLog_BooksTextColourVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_BooksTextColourVal", CompletionLog_BooksTextColourVal))
			CompletionLog_BooksTextColourString = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_BooksTextColourString", CompletionLog_BooksTextColourString))	

			CompletionLog_ItemsTextColourVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_ItemsTextColourVal", CompletionLog_ItemsTextColourVal))
			CompletionLog_ItemsTextColourString = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_ItemsTextColourString", CompletionLog_ItemsTextColourString))	

			CompletionLog_QuestsTextColourVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_QuestsTextColourVal", CompletionLog_QuestsTextColourVal))
			CompletionLog_QuestsTextColourString = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_QuestsTextColourString", CompletionLog_QuestsTextColourString))	

			CompletionLog_SpecialTextColourVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_SpecialTextColourVal", CompletionLog_SpecialTextColourVal))
			CompletionLog_SpecialTextColourString = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_SpecialTextColourString", CompletionLog_SpecialTextColourString))	
			
			State_AutomaticCompletionColourVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_AutomaticCompletionColourVal", State_AutomaticCompletionColourVal))
			State_AutomaticCompletionColourString = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_AutomaticCompletionColourString", State_AutomaticCompletionColourString))	
			
			CompletionLog_ColoursEnabled = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_ColoursEnabled", CompletionLog_ColoursEnabled as Int))
			CompletionLog_PrefixEnabled = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_PrefixEnabled", CompletionLog_PrefixEnabled as Int))
		
			State_ColourVal_G_HUD_Crosshair = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourVal_G_HUD_Crosshair", State_ColourVal_G_HUD_Crosshair))
			State_ColourVal_N_HUD_Crosshair = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourVal_N_HUD_Crosshair", State_ColourVal_N_HUD_Crosshair))
			State_ColourString_G_HUD_Crosshair = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourString_G_HUD_Crosshair", State_ColourString_G_HUD_Crosshair))
			State_ColourString_N_HUD_Crosshair = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourString_N_HUD_Crosshair", State_ColourString_N_HUD_Crosshair))

			State_ColourVal_G_HUD_Menus = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourVal_G_HUD_Menus", State_ColourVal_G_HUD_Menus))
			State_ColourVal_N_HUD_Menus = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourVal_N_HUD_Menus", State_ColourVal_N_HUD_Menus))
			State_ColourString_G_HUD_Menus = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourString_G_HUD_Menus", State_ColourString_G_HUD_Menus))
			State_ColourString_N_HUD_Menus = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourString_N_HUD_Menus", State_ColourString_N_HUD_Menus))

			b_CustomColour_G_HUD_Crosshair = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_CustomColour_G_HUD_Crosshair", b_CustomColour_G_HUD_Crosshair as Int))
			b_CustomColour_N_HUD_Crosshair = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_CustomColour_N_HUD_Crosshair", b_CustomColour_N_HUD_Crosshair as Int))
			State_CustomColourVal_G_HUD_Crosshair = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourVal_G_HUD_Crosshair", State_CustomColourVal_G_HUD_Crosshair))
			State_CustomColourVal_N_HUD_Crosshair = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourVal_N_HUD_Crosshair", State_CustomColourVal_N_HUD_Crosshair))
			State_CustomColourString_G_HUD_Crosshair = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourString_G_HUD_Crosshair", State_CustomColourString_G_HUD_Crosshair))
			State_CustomColourString_N_HUD_Crosshair = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourString_N_HUD_Crosshair", State_CustomColourString_N_HUD_Crosshair))

			b_CustomColour_G_HUD_Menus = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_CustomColour_G_HUD_Menus", b_CustomColour_G_HUD_Menus as Int))
			b_CustomColour_N_HUD_Menus = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_CustomColour_N_HUD_Menus", b_CustomColour_N_HUD_Menus as Int))
			State_CustomColourVal_G_HUD_Menus = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourVal_G_HUD_Menus", State_CustomColourVal_G_HUD_Menus))
			State_CustomColourVal_N_HUD_Menus = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourVal_N_HUD_Menus", State_CustomColourVal_N_HUD_Menus))
			State_CustomColourString_G_HUD_Menus = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourString_G_HUD_Menus", State_CustomColourString_G_HUD_Menus))
			State_CustomColourString_N_HUD_Menus = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourString_N_HUD_Menus", State_CustomColourString_N_HUD_Menus))
		
			State_OverRide_G_Name_String = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_OverRide_G_Name_String", State_OverRide_G_Name_String))
			State_OverRide_N_Name_String = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_OverRide_N_Name_String", State_OverRide_N_Name_String))
			InventoryMode_PrAp_Choice_N = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!InventoryMode_PrAp_Choice_N", InventoryMode_PrAp_Choice_N as Int))
			InventoryMode_PrAp_Choice_G = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!InventoryMode_PrAp_Choice_G", InventoryMode_PrAp_Choice_G as Int))
			InventoryMode_PrFx_Choice_N = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!InventoryMode_PrFx_Choice_N", InventoryMode_PrFx_Choice_N as Int))
			InventoryMode_PrFx_Choice_G = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!InventoryMode_PrFx_Choice_G", InventoryMode_PrFx_Choice_G as Int))
			
			bCellScanner_CONT = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_CONT", bCellScanner_CONT as Int))
			bCellScanner_DETA = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_DETA", bCellScanner_DETA as Int))
			bCellScanner_NPCS = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_NPCS", bCellScanner_NPCS as Int))
			bCellScanner_NUMB = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_NUMB", bCellScanner_NUMB as Int))
			bCellScanner_REFS = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_REFS", bCellScanner_REFS as Int))
			bCellScanner_MARK = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_MARK", bCellScanner_MARK as Int))
			iCellScanner_Range = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!iCellScanner_Range", iCellScanner_Range))
			bCellScanner_UseClosestReference = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_UseClosestReference", bCellScanner_UseClosestReference as Int))

			bCellScanner_ExcludeAlchemy = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_ExcludeAlchemy", bCellScanner_ExcludeAlchemy as Int))
			bCellScanner_ExcludeArmor = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_ExcludeArmor", bCellScanner_ExcludeArmor as Int))
			bCellScanner_ExcludeBooks = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_ExcludeBooks", bCellScanner_ExcludeBooks as Int))
			bCellScanner_ExcludeMiscItems = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_ExcludeMiscItems", bCellScanner_ExcludeMiscItems as Int))
			bCellScanner_ExcludeWeapons = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_ExcludeWeapons", bCellScanner_ExcludeWeapons as Int))
		
			b_moreHUDEnabled_Crosshair = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_moreHUDEnabled_Crosshair", b_moreHUDEnabled_Crosshair as Int))
			b_moreHUDEnabled_Menus = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_moreHUDEnabled_Menus", b_moreHUDEnabled_Menus as Int))
			b_quickLoot_Enabled = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_quickLoot_Enabled", b_quickLoot_Enabled as Int))

			TreatBooksAsItems = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!TreatBooksAsItems", TreatBooksAsItems as Int))
	
			jsonutil.Load("../CompletionistData/Profiles/CompConfig")
			if IsInMenuMode()
				ShowMessage("$ProfileLoadSuccessMenu")
				ForcePageReset()
			endIf
		else
			if IsInMenuMode()
				ShowMessage("$ProfileLoadMissingMenu")
				ForcePageReset()
			else
				Begin_Config_Default()
			endIf
		endIf
	else
		if IsInMenuMode()
			ShowMessage("$PapUtilError")
			ForcePageReset()
		endIf
	endIf
EndFunction	

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

function AutoLoadConfig()
		
	if (papyrusutil.GetScriptVersion() > 31) && (jsonutil.JsonExists("../CompletionistData/Profiles/CompConfig")) && (jsonutil.IsGood("../CompletionistData/Profiles/CompConfig"))
			
		;;Search Settings
		b_SearchIgnoreCompleted = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_SearchIgnoreCompleted", b_SearchIgnoreCompleted as Int))
		b_SearchHighlightQuest = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_SearchHighlightQuest", b_SearchHighlightQuest as Int))
		i_SearchMaxResults = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!i_SearchMaxResults", i_SearchMaxResults))
		i_SearchTypeChoice = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!i_SearchTypeChoice", i_SearchTypeChoice))
		s_SearchType = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!s_SearchType", s_SearchType))

		;;Factions
		HR_Faction_Choice = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!HR_Faction_Choice", HR_Faction_Choice))
		DG_Faction_Choice = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!DG_Faction_Choice", DG_Faction_Choice))
		Legacy_Faction_Choice = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!Legacy_Faction_Choice", Legacy_Faction_Choice))

		;;Shortcuts
		bShortCutActive = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bShortCutActive", bShortCutActive as Int))
		MainJumpPage = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!MainJumpPage", MainJumpPage))
		MiscJumpPage = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!MiscJumpPage", MiscJumpPage))
		MainMCMPagesChoice = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!MainMCMPagesChoice", MainMCMPagesChoice))
		MiscMCMPagesChoice = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!MiscMCMPagesChoice", MiscMCMPagesChoice))
		if (!bShortCutActive)
			MainJumpPage = ""
			MiscJumpPage = ""
			MainMCMPagesChoice = 0
			MiscMCMPagesChoice = 0
		endIf
		
		;;General Settings
		NotifySpecial = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!NotifySpecial", NotifySpecial as Int))
		NotifyStartup = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!NotifyStartup", NotifyStartup as Int))
		NotifyBooks = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!NotifyBooks", NotifyBooks as Int))
		NotifyItems = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!NotifyItems", NotifyItems as Int))

		;;Misc Settings
		State_MarkerDetectionVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_MarkerDetectionVal", State_MarkerDetectionVal))
		State_RadiantCounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_RadiantCounterVal", State_RadiantCounterVal))
		State_BountyCounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_BountyCounterVal", State_BountyCounterVal))
		State_TGRCounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_TGRCounterVal", State_TGRCounterVal))
		State_DLC1CounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_DLC1CounterVal", State_DLC1CounterVal))
		State_LOTDCounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_LOTDCounterVal", State_LOTDCounterVal))
		State_VigilantCounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_VigilantCounterVal", State_VigilantCounterVal))
		State_CompanionsCounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_CompanionsCounterVal", State_CompanionsCounterVal))
		State_CollegeCounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_CollegeCounterVal", State_CollegeCounterVal))
		State_DBCounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_DBCounterVal", State_DBCounterVal))
		State_FishingCounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_FishingCounterVal", State_FishingCounterVal))
		State_BladesCounterVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_BladesCounterVal", State_BladesCounterVal))
		
		FishingSpotMarkers = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!FishingSpotMarkers", FishingSpotMarkers as Int))
		if (FishingSpotMarkers && Completionist_FishingEnabled.GetValue())
			Completionist_FishingSpot_Marker.Enable()
		else
			Completionist_FishingSpot_Marker.Disable()
			FishingSpotMarkers = false		
		endIf
			
		;;Notification Colours
		NotificationColourEnabled = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!NotificationColourEnabled", NotificationColourEnabled as Int))
		IndexColour = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!IndexColour", IndexColour))
		ColourString = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!ColourString", ColourString))				

		StateShout_GotColourVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!StateShout_GotColourVal", StateShout_GotColourVal))
		StateShout_NotColourVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!StateShout_NotColourVal", StateShout_NotColourVal))
		StateShout_GotColourString = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!StateShout_GotColourString", StateShout_GotColourString))
		StateShout_NotColourString = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!StateShout_NotColourString", StateShout_NotColourString))
		Framework_UpdateShouts()

		CompletionLog_BooksTextColourVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_BooksTextColourVal", CompletionLog_BooksTextColourVal))
		CompletionLog_BooksTextColourString = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_BooksTextColourString", CompletionLog_BooksTextColourString))	

		CompletionLog_ItemsTextColourVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_ItemsTextColourVal", CompletionLog_ItemsTextColourVal))
		CompletionLog_ItemsTextColourString = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_ItemsTextColourString", CompletionLog_ItemsTextColourString))	

		CompletionLog_QuestsTextColourVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_QuestsTextColourVal", CompletionLog_QuestsTextColourVal))
		CompletionLog_QuestsTextColourString = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_QuestsTextColourString", CompletionLog_QuestsTextColourString))	

		CompletionLog_SpecialTextColourVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_SpecialTextColourVal", CompletionLog_SpecialTextColourVal))
		CompletionLog_SpecialTextColourString = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_SpecialTextColourString", CompletionLog_SpecialTextColourString))	
			
		State_AutomaticCompletionColourVal = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_AutomaticCompletionColourVal", State_AutomaticCompletionColourVal))
		State_AutomaticCompletionColourString = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_AutomaticCompletionColourString", State_AutomaticCompletionColourString))

		CompletionLog_ColoursEnabled = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_ColoursEnabled", CompletionLog_ColoursEnabled as Int))
		CompletionLog_PrefixEnabled = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!CompletionLog_PrefixEnabled", CompletionLog_PrefixEnabled as Int))
			
		State_ColourVal_G_HUD_Crosshair = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourVal_G_HUD_Crosshair", State_ColourVal_G_HUD_Crosshair))
		State_ColourVal_N_HUD_Crosshair = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourVal_N_HUD_Crosshair", State_ColourVal_N_HUD_Crosshair))
		State_ColourString_G_HUD_Crosshair = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourString_G_HUD_Crosshair", State_ColourString_G_HUD_Crosshair))
		State_ColourString_N_HUD_Crosshair = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourString_N_HUD_Crosshair", State_ColourString_N_HUD_Crosshair))

		State_ColourVal_G_HUD_Menus = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourVal_G_HUD_Menus", State_ColourVal_G_HUD_Menus))
		State_ColourVal_N_HUD_Menus = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourVal_N_HUD_Menus", State_ColourVal_N_HUD_Menus))
		State_ColourString_G_HUD_Menus = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourString_G_HUD_Menus", State_ColourString_G_HUD_Menus))
		State_ColourString_N_HUD_Menus = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_ColourString_N_HUD_Menus", State_ColourString_N_HUD_Menus))

		b_CustomColour_G_HUD_Crosshair = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_CustomColour_G_HUD_Crosshair", b_CustomColour_G_HUD_Crosshair as Int))
		b_CustomColour_N_HUD_Crosshair = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_CustomColour_N_HUD_Crosshair", b_CustomColour_N_HUD_Crosshair as Int))
		State_CustomColourVal_G_HUD_Crosshair = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourVal_G_HUD_Crosshair", State_CustomColourVal_G_HUD_Crosshair))
		State_CustomColourVal_N_HUD_Crosshair = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourVal_N_HUD_Crosshair", State_CustomColourVal_N_HUD_Crosshair))
		State_CustomColourString_G_HUD_Crosshair = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourString_G_HUD_Crosshair", State_CustomColourString_G_HUD_Crosshair))
		State_CustomColourString_N_HUD_Crosshair = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourString_N_HUD_Crosshair", State_CustomColourString_N_HUD_Crosshair))

		b_CustomColour_G_HUD_Menus = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_CustomColour_G_HUD_Menus", b_CustomColour_G_HUD_Menus as Int))
		b_CustomColour_N_HUD_Menus = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_CustomColour_N_HUD_Menus", b_CustomColour_N_HUD_Menus as Int))
		State_CustomColourVal_G_HUD_Menus = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourVal_G_HUD_Menus", State_CustomColourVal_G_HUD_Menus))
		State_CustomColourVal_N_HUD_Menus = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourVal_N_HUD_Menus", State_CustomColourVal_N_HUD_Menus))
		State_CustomColourString_G_HUD_Menus = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourString_G_HUD_Menus", State_CustomColourString_G_HUD_Menus))
		State_CustomColourString_N_HUD_Menus = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_CustomColourString_N_HUD_Menus", State_CustomColourString_N_HUD_Menus))
			
		State_OverRide_G_Name_String = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_OverRide_G_Name_String", State_OverRide_G_Name_String))
		State_OverRide_N_Name_String = (jsonutil.GetPathStringValue("../CompletionistData/Profiles/CompConfig", ".!State_OverRide_N_Name_String", State_OverRide_N_Name_String))
		InventoryMode_PrAp_Choice_N = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!InventoryMode_PrAp_Choice_N", InventoryMode_PrAp_Choice_N as Int))
		InventoryMode_PrAp_Choice_G = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!InventoryMode_PrAp_Choice_G", InventoryMode_PrAp_Choice_G as Int))
		InventoryMode_PrFx_Choice_N = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!InventoryMode_PrFx_Choice_N", InventoryMode_PrFx_Choice_N as Int))
		InventoryMode_PrFx_Choice_G = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!InventoryMode_PrFx_Choice_G", InventoryMode_PrFx_Choice_G as Int))

		bCellScanner_CONT = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_CONT", bCellScanner_CONT as Int))
		bCellScanner_DETA = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_DETA", bCellScanner_DETA as Int))
		bCellScanner_NPCS = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_NPCS", bCellScanner_NPCS as Int))
		bCellScanner_NUMB = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_NUMB", bCellScanner_NUMB as Int))
		bCellScanner_REFS = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_REFS", bCellScanner_REFS as Int))
		bCellScanner_MARK = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_MARK", bCellScanner_MARK as Int))
		iCellScanner_Range = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!iCellScanner_Range", iCellScanner_Range))
		bCellScanner_UseClosestReference = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_UseClosestReference", bCellScanner_UseClosestReference as Int))

		bCellScanner_ExcludeAlchemy = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_ExcludeAlchemy", bCellScanner_ExcludeAlchemy as Int))
		bCellScanner_ExcludeArmor = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_ExcludeArmor", bCellScanner_ExcludeArmor as Int))
		bCellScanner_ExcludeBooks = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_ExcludeBooks", bCellScanner_ExcludeBooks as Int))
		bCellScanner_ExcludeMiscItems = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_ExcludeMiscItems", bCellScanner_ExcludeMiscItems as Int))
		bCellScanner_ExcludeWeapons = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!bCellScanner_ExcludeWeapons", bCellScanner_ExcludeWeapons as Int))
			
		b_moreHUDEnabled_Crosshair = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_moreHUDEnabled_Crosshair", b_moreHUDEnabled_Crosshair as Int))
		b_moreHUDEnabled_Menus = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_moreHUDEnabled_Menus", b_moreHUDEnabled_Menus as Int))
		b_quickLoot_Enabled = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!b_quickLoot_Enabled", b_quickLoot_Enabled as Int))
		
		TreatBooksAsItems = (jsonutil.GetPathIntValue("../CompletionistData/Profiles/CompConfig", ".!TreatBooksAsItems", TreatBooksAsItems as Int))

		jsonutil.Load("../CompletionistData/Profiles/CompConfig")
		SendNotification("$ProfileLoadSuccess", ColourString, NotificationColourEnabled)
	else
		Begin_Config_Default()
		SendNotification("$ProfileLoadMissing", ColourString, NotificationColourEnabled)
	endIf
	AutoLoaded = True
endFunction

;---------------------------------------------------
;-- Functions --------------------------------------
;---------------------------------------------------

Function Begin_Config_Default()

	b_SearchIgnoreCompleted = false
	b_SearchHighlightQuest = true
	i_SearchMaxResults = 30
	i_SearchTypeChoice = 0
	s_SearchType = "$SearchTypeChoice01"

	HR_Faction_Choice = 0
	DG_Faction_Choice = 0
	Legacy_Faction_Choice = 0
	
	bShortCutActive = False
	MainMCMPagesChoice = 0
	MiscMCMPagesChoice = 0
	MainJumpPage = ""
	MiscJumpPage = ""
	SearchJumpPage = ""
	
	NotifyBooks = True
	NotifyItems = True
	NotifySpecial = True
	NotifyStartup = True
	NotifyMissable = True
	TreatBooksAsItems = False
	
	State_MarkerDetectionVal = 1500
	State_RadiantCounterVal = 5
	State_BountyCounterVal = 5
	State_TGRCounterVal = 5
	State_DLC1CounterVal = 5
	State_LOTDCounterVal = 5
	State_VigilantCounterVal = 5
	State_CompanionsCounterVal = 5
	State_CollegeCounterVal = 5
	State_DBCounterVal = 5
	State_FishingCounterVal = 4
	State_BladesCounterVal = 5

	iCellScanner_Range = 100
	
	NotificationColourEnabled = True
	IndexColour = 14905890
	ColourString = GetHexValue(IndexColour)
	
	StateShout_GotColourVal = 10092543
	StateShout_NotColourVal = 10027059
	StateShout_GotColourString = GetHexValue(StateShout_GotColourVal)
	StateShout_NotColourString = GetHexValue(StateShout_NotColourVal)
	Framework_UpdateShouts()

	CompletionLog_BooksTextColourVal = 15379200
	CompletionLog_BooksTextColourString = GetHexValue(CompletionLog_BooksTextColourVal)

	CompletionLog_ItemsTextColourVal = 12406553
	CompletionLog_ItemsTextColourString = GetHexValue(CompletionLog_ItemsTextColourVal)

	CompletionLog_QuestsTextColourVal = 9202896
	CompletionLog_QuestsTextColourString = GetHexValue(CompletionLog_QuestsTextColourVal)

	CompletionLog_SpecialTextColourVal = 10092543
	CompletionLog_SpecialTextColourString = GetHexValue(CompletionLog_SpecialTextColourVal)

	CompletionLog_ColoursEnabled = True
	CompletionLog_PrefixEnabled = True
			
	State_AutomaticCompletionColourVal = 10398469
	State_AutomaticCompletionColourString = "#9EAB05"

	State_ManualCompletionColourVal = 10398469
	State_ManualCompletionColourString = "#9EAB05"

	State_MiscCompletionColourVal = 10398469
	State_MiscCompletionColourString = "#9EAB05"
	
	State_ColourVal_G_HUD_Crosshair = 1288220
	State_ColourVal_N_HUD_Crosshair = 4430046
	State_ColourString_G_HUD_Crosshair = GetHexValue(State_ColourVal_G_HUD_Crosshair)
	State_ColourString_N_HUD_Crosshair = GetHexValue(State_ColourVal_N_HUD_Crosshair)

	State_ColourVal_G_HUD_Menus = 1288220
	State_ColourVal_N_HUD_Menus = 4430046
	State_ColourString_G_HUD_Menus = GetHexValue(State_ColourVal_G_HUD_Menus)
	State_ColourString_N_HUD_Menus = GetHexValue(State_ColourVal_N_HUD_Menus)
	
	b_CustomColour_G_HUD_Crosshair = False
	b_CustomColour_N_HUD_Crosshair = False

	State_CustomColourVal_G_HUD_Crosshair = -1
	State_CustomColourString_G_HUD_Crosshair = "Enter Decimal"

	State_CustomColourVal_N_HUD_Crosshair = -1
	State_CustomColourString_N_HUD_Crosshair = "Enter Decimal"

	b_CustomColour_G_HUD_Menus = False
	b_CustomColour_N_HUD_Menus = False
	
	State_CustomColourVal_G_HUD_Menus = -1
	State_CustomColourString_G_HUD_Menus = "Enter Decimal"

	State_CustomColourVal_N_HUD_Menus = -1
	State_CustomColourString_N_HUD_Menus = "Enter Decimal"
	
	InventoryMode_PrAp_Choice_N = 4
	InventoryMode_PrAp_Choice_G = 4

	InventoryMode_PrFx_Choice_N = 0
	InventoryMode_PrFx_Choice_G = 0
	
	State_OverRide_G_Name_String = "Got It!"
	State_OverRide_N_Name_String = "Need It!"
	State_SearchTermString = "Enter Search Term..."
	
	if (!Completionist_FishingEnabled.GetValue())
		FishingSpotMarkers = False
		Completionist_FishingSpot_Marker.Disable()
	else
		FishingSpotMarkers = True
		Completionist_FishingSpot_Marker.Enable()
	endIf
	
	bCellScanner_CONT = True
	bCellScanner_DETA = False
	bCellScanner_NPCS = False
	bCellScanner_NUMB = True
	bCellScanner_REFS = True
	bCellScanner_MARK = False

	bCellScanner_ExcludeAlchemy = False
	bCellScanner_ExcludeArmor = False
	bCellScanner_ExcludeBooks = False
	bCellScanner_ExcludeMiscItems = False
	bCellScanner_ExcludeWeapons = False
	bCellScanner_UseClosestReference = True

	b_moreHUDEnabled_Crosshair = True
	b_moreHUDEnabled_Menus = True
	b_quickLoot_Enabled = True
			
	if IsInMenuMode()
		ForcePageReset()
	endIf
endFunction

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_ProfileReset

	Event OnSelectST()
		
		 if ShowMessage("$LoadDefaultTitleMessage", true, "$Restore", "$ConfirmN")
			SetTitleText("$LoadDefaultTitleText") 
			Begin_Config_Default()
			UpdateVariables()
		endIf
	EndEvent

	Event OnHighlightST()

		SetInfoText("$LoadDefaultTitleInfo")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_ProfileSave

	Event OnSelectST()
		
		SetTitleText("$SaveProfileTitleText") 
		Begin_Config_Save()
	EndEvent

	Event OnHighlightST()
		
		SetInfoText("$SaveProfileTitleInfo")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_ProfileLoad

	Event OnSelectST()
		
		SetTitleText("$LoadProfileTitleText") 
		Begin_Config_Load()
		UpdateVariables()
	EndEvent

	Event OnHighlightST()

		SetInfoText("$LoadProfileTitleInfo")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State State_Radiant_Quests ; MENU

	Event OnMenuOpenST()
		SetMenuDialogStartIndex(Radiant_Quests_Choice)
		SetMenuDialogDefaultIndex(0)
		SetMenuDialogOptions(Radiant_Quests)
	EndEvent
					
	Event OnMenuAcceptST(Int index)
		Radiant_Quests_Choice = Index
		Radiant_Quests_String = Radiant_Quests[Radiant_Quests_Choice]
		SetMenuOptionValueST(State_Radiant_Quests, Radiant_Quests[Radiant_Quests_Choice])
		ForcePageReset()
	EndEvent

	Event OnDefaultST()
		Radiant_Quests_Choice = 0
		Radiant_Quests_String = Radiant_Quests[Radiant_Quests_Choice]
		SetMenuOptionValueST(Radiant_Quests[Radiant_Quests_Choice])
	EndEvent

	Event OnHighlightST()
		SetInfoText("$State_RadiantQuestsInfo")
	EndEvent
	
endState
	
;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State State_Menu_Faction1 ; MENU

	Event OnMenuOpenST()
		SetMenuDialogStartIndex(DG_Faction_Choice)
		SetMenuDialogDefaultIndex(0)
		SetMenuDialogOptions(Dawnguard_Faction)
	EndEvent
					
	Event OnMenuAcceptST(Int index)
		DG_Faction_Choice = Index
		SetMenuOptionValueST(State_Menu_Faction1, Dawnguard_Faction[DG_Faction_Choice])
		ForcePageReset()
	EndEvent

	Event OnDefaultST()
		DG_Faction_Choice = 0
		SetMenuOptionValueST(Dawnguard_Faction[DG_Faction_Choice])
	EndEvent

	Event OnHighlightST()
		SetInfoText("$DGFactionInfo")
	EndEvent
	
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State State_Menu_Faction3 ; MENU

	event OnMenuOpenST()
		SetMenuDialogStartIndex(HR_Faction_Choice)
		SetMenuDialogDefaultIndex(0)
		SetMenuDialogOptions(HelgenReborn_Faction)
	endevent
					
	event OnMenuAcceptST(int index)
		HR_Faction_Choice = Index
		SetMenuOptionValueST(State_Menu_Faction3, HelgenReborn_Faction[HR_Faction_Choice])
		ForcePageReset()
	endevent

	event OnDefaultST()
		HR_Faction_Choice = 0
		SetMenuOptionValueST(HelgenReborn_Faction[HR_Faction_Choice])
	endevent

	event OnHighlightST()
		SetInfoText("$HRFactionInfo")
	endevent
	
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State State_Menu_Faction4 ; MENU

	event OnMenuOpenST()
		SetMenuDialogStartIndex(Legacy_Faction_Choice)
		SetMenuDialogDefaultIndex(0)
		SetMenuDialogOptions(Legacy_Faction)
	endevent
					
	event OnMenuAcceptST(int index)
		Legacy_Faction_Choice = Index
		SetMenuOptionValueST(State_Menu_Faction4, Legacy_Faction[Legacy_Faction_Choice])
		ForcePageReset()
	endevent

	event OnDefaultST()
		Legacy_Faction_Choice = 0
		SetMenuOptionValueST(Legacy_Faction[Legacy_Faction_Choice])
	endevent

	event OnHighlightST()
		SetInfoText("$LDFactionInfo")
	endevent
	
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State State_ShortcutsMenu1 ; MENU

	Event OnMenuOpenST()
		SetMenuDialogStartIndex(MainMCMPagesChoice)
		SetMenuDialogDefaultIndex(0)
		SetMenuDialogOptions(pages)
	EndEvent
					
	Event OnMenuAcceptST(Int index)
		MainMCMPagesChoice = Index
		SetMenuOptionValueST(State_ShortcutsMenu1, pages[MainMCMPagesChoice])
		MainJumpPage = pages[MainMCMPagesChoice]
		ForcePageReset()
	EndEvent

	Event OnDefaultST()
		MainMCMPagesChoice = 0
		SetMenuOptionValueST(pages[MainMCMPagesChoice])
		MainJumpPage = pages[MainMCMPagesChoice]
	EndEvent

	Event OnHighlightST()
		SetInfoText("$State_ShortcutsMainInfo")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State State_ShortcutsMenu2 ; MENU

	Event OnMenuOpenST()
		SetMenuDialogStartIndex(MiscMCMPagesChoice)
		SetMenuDialogDefaultIndex(0)
		SetMenuDialogOptions(CompMCM2.pages)
	EndEvent
					
	Event OnMenuAcceptST(Int Index)
		MiscMCMPagesChoice = Index
		SetMenuOptionValueST(State_ShortcutsMenu2, CompMCM2.pages[MiscMCMPagesChoice])
		MiscJumpPage = CompMCM2.pages[MiscMCMPagesChoice]
		ForcePageReset()
	EndEvent

	Event OnDefaultST()
		MiscMCMPagesChoice = 0
		SetMenuOptionValueST(CompMCM2.pages[MiscMCMPagesChoice])
		MiscJumpPage = CompMCM2.pages[MiscMCMPagesChoice]
	EndEvent

	Event OnHighlightST()
		SetInfoText("$State_ShortcutsMiscInfo")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State State_ShortcutsMenu3 ; MENU

	Event OnMenuOpenST()
		SetMenuDialogStartIndex(UnofficialMCMPagesChoice)
		SetMenuDialogDefaultIndex(0)
		SetMenuDialogOptions(CompMCM3.pages)
	EndEvent
					
	Event OnMenuAcceptST(Int Index)
		UnofficialMCMPagesChoice = Index
		SetMenuOptionValueST(State_ShortcutsMenu2, CompMCM3.pages[UnofficialMCMPagesChoice])
		UnofficialJumpPage = CompMCM3.pages[UnofficialMCMPagesChoice]
		ForcePageReset()
	EndEvent

	Event OnDefaultST()
		UnofficialMCMPagesChoice = 0
		SetMenuOptionValueST(CompMCM3.pages[UnofficialMCMPagesChoice])
		UnofficialJumpPage = CompMCM3.pages[UnofficialMCMPagesChoice]
	EndEvent

	Event OnHighlightST()
		SetInfoText("$State_ShortcutsUnofficialInfo")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State State_CompletionView ; MENU

	Event OnMenuOpenST()
		SetMenuDialogStartIndex(State_AutomaticCompletionView_Choice)
		SetMenuDialogDefaultIndex(0)
		SetMenuDialogOptions(State_AutomaticCompletionView_List)
	EndEvent
					
	Event OnMenuAcceptST(Int index)
		State_AutomaticCompletionView_Choice = Index
		SetMenuOptionValueST(State_CompletionView, State_AutomaticCompletionView_List[State_AutomaticCompletionView_Choice])
		ForcePageReset()
	EndEvent

	Event OnDefaultST()
		State_AutomaticCompletionView_Choice = 0
		SetMenuOptionValueST(State_AutomaticCompletionView_List[State_AutomaticCompletionView_Choice])
	EndEvent

	Event OnHighlightST()
		SetInfoText("$State_AutomaticCompletionView_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State State_ManualCompletionView ; MENU

	Event OnMenuOpenST()
		SetMenuDialogStartIndex(State_ManualCompletionView_Choice)
		SetMenuDialogDefaultIndex(0)
		SetMenuDialogOptions(State_ManualCompletionView_List)
	EndEvent
					
	Event OnMenuAcceptST(Int index)
		State_ManualCompletionView_Choice = Index
		SetMenuOptionValueST(State_ManualCompletionView, State_ManualCompletionView_List[State_ManualCompletionView_Choice])
		ForcePageReset()
	EndEvent

	Event OnDefaultST()
		State_ManualCompletionView_Choice = 0
		SetMenuOptionValueST(State_ManualCompletionView_List[State_ManualCompletionView_Choice])
	EndEvent

	Event OnHighlightST()
		SetInfoText("$State_ManualCompletionView_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State State_MiscCompletionView ; MENU

	Event OnMenuOpenST()
		SetMenuDialogStartIndex(State_MiscCompletionView_Choice)
		SetMenuDialogDefaultIndex(0)
		SetMenuDialogOptions(State_MiscCompletionView_List)
	EndEvent
					
	Event OnMenuAcceptST(Int index)
		State_MiscCompletionView_Choice = Index
		SetMenuOptionValueST(State_MiscCompletionView, State_MiscCompletionView_List[State_MiscCompletionView_Choice])
		ForcePageReset()
	EndEvent

	Event OnDefaultST()
		State_MiscCompletionView_Choice = 0
		SetMenuOptionValueST(State_MiscCompletionView_List[State_MiscCompletionView_Choice])
	EndEvent

	Event OnHighlightST()
		SetInfoText("$State_MiscCompletionView_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_SearchHighlightQuest

	Event OnSelectST()
		b_SearchHighlightQuest = !b_SearchHighlightQuest
		SetTextOptionValueST(GetEnabledStatus(b_SearchHighlightQuest))
		ForcePageReset()
	EndEvent
	
	Event OnDefaultST()
		b_SearchHighlightQuest = True
		SetTextOptionValueST(GetEnabledStatus(b_SearchHighlightQuest))
		ForcePageReset()
	EndEvent

	Event OnHighlightST()

		SetInfoText("$State_SearchHighlightQuest_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_SearchIgnoreCompleted

	Event OnSelectST()
		b_SearchIgnoreCompleted = !b_SearchIgnoreCompleted
		SetTextOptionValueST(GetEnabledStatus(b_SearchIgnoreCompleted))
		ForcePageReset()
	EndEvent
	
	Event OnDefaultST()
		b_SearchIgnoreCompleted = False
		SetTextOptionValueST(GetEnabledStatus(b_SearchIgnoreCompleted))
		ForcePageReset()
	EndEvent

	Event OnHighlightST()

		SetInfoText("$State_SearchIgnoreCompleted_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_SearchMaxResults ; SLIDER

	event OnSliderOpenST()
		SetSliderDialogStartValue(i_SearchMaxResults)
		SetSliderDialogDefaultValue(30)
		SetSliderDialogRange(1, 300)
		SetSliderDialogInterval(1)
	endEvent

	event OnSliderAcceptST(float a_value)
		i_SearchMaxResults = a_value as int
		SetSliderOptionValueST(i_SearchMaxResults, "{0}")
	endEvent

	event OnDefaultST()
		i_SearchMaxResults = 30
		SetSliderOptionValueST(i_SearchMaxResults)
		self.SetSliderOptionValueST(30 as Float, "{0}")
	endEvent

	event OnHighlightST()
		SetInfoText("$State_SearchMaxResults_Info")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State State_SearchQueryType ; MENU

	Event OnMenuOpenST()
		SetMenuDialogStartIndex(i_SearchTypeChoice)
		SetMenuDialogDefaultIndex(0)
		SetMenuDialogOptions(SearchType)
	EndEvent
					
	Event OnMenuAcceptST(Int index)
		i_SearchTypeChoice = Index
		s_SearchType = SearchType[i_SearchTypeChoice]
		SetMenuOptionValueST(State_SearchQueryType, SearchType[i_SearchTypeChoice])
		ForcePageReset()
	EndEvent

	Event OnDefaultST()
		i_SearchTypeChoice = 0
		s_SearchType = SearchType[i_SearchTypeChoice]
		SetMenuOptionValueST(SearchType[i_SearchTypeChoice])
	EndEvent

	Event OnHighlightST()
		SetInfoText("$State_SearchQueryType_Info")
	EndEvent
	
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_Debugging

	Event OnSelectST()
		bDebug = !bDebug
		SetTextOptionValueST(GetEnabledStatus(bDebug))
		ForcePageReset()
	EndEvent
	
	Event OnDefaultST()
		bDebug = False
		SetTextOptionValueST(GetEnabledStatus(bDebug))
		ForcePageReset()
	EndEvent

	Event OnHighlightST()

		SetInfoText("$State_DebuggingInfo")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State DumpQuestData
	Event OnSelectST()
		qDumpQuestData()
		ShowMessage("Quest Data dumped to Completionist.log", false, "Ok")
	EndEvent

	Event OnHighlightST()

		SetInfoText("$DumpToLog")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state TreatBooksAsItemsState

	Event OnSelectST()
		TreatBooksAsItems = !TreatBooksAsItems
		SetTextOptionValueST(GetEnabledStatus(TreatBooksAsItems))
	EndEvent
	
	Event OnDefaultST()
		TreatBooksAsItems = False
		SetTextOptionValueST(GetEnabledStatus(TreatBooksAsItems))
	EndEvent

	Event OnHighlightST()

		SetInfoText("$TreatBooksAsItemsState_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state ModNotificationsItems

	Event OnSelectST()
		NotifyItems = !NotifyItems
		SetTextOptionValueST(GetEnabledStatus(NotifyItems))
	EndEvent
	
	Event OnDefaultST()
		NotifyItems = True
		SetTextOptionValueST(GetEnabledStatus(NotifyItems))
	EndEvent

	Event OnHighlightST()

		SetInfoText("$ModNotificationsItemsInfo")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state ModNotificationsBooks

	Event OnSelectST()
		NotifyBooks = !NotifyBooks
		SetTextOptionValueST(GetEnabledStatus(NotifyBooks))
	EndEvent
	
	Event OnDefaultST()
		NotifyBooks = True
		SetTextOptionValueST(GetEnabledStatus(NotifyBooks))
	EndEvent

	Event OnHighlightST()

		SetInfoText("$ModNotificationsBooksInfo")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state ModNotificationsSpecial

	Event OnSelectST()
		NotifySpecial = !NotifySpecial
		SetTextOptionValueST(GetEnabledStatus(NotifySpecial))
	EndEvent
	
	Event OnDefaultST()
		NotifySpecial = True
		SetTextOptionValueST(GetEnabledStatus(NotifySpecial))
	EndEvent

	Event OnHighlightST()

		SetInfoText("$ModNotificationsSpecialInfo")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state ModNotificationsStartup

	Event OnSelectST()
		NotifyStartup = !NotifyStartup
		SetTextOptionValueST(GetEnabledStatus(NotifyStartup))
	EndEvent
	
	Event OnDefaultST()
		NotifyStartup = True
		SetTextOptionValueST(GetEnabledStatus(NotifyStartup))
	EndEvent

	Event OnHighlightST()

		SetInfoText("$ModNotificationsStartupInfo")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_FishingSpotMarkers

	Event OnSelectST()
		FishingSpotMarkers = !FishingSpotMarkers
		
		if (FishingSpotMarkers)
			Completionist_FishingSpot_Marker.Enable()
		else
		Completionist_FishingSpot_Marker.Disable()
		endIf

		SetTextOptionValueST(GetEnabledStatus(FishingSpotMarkers))
		UpdateVariables()
		BuildMCMPages()
	EndEvent
	
	Event OnDefaultST()
		if (!Completionist_FishingEnabled.GetValue())
			Completionist_FishingSpot_Marker.Disable()
			FishingSpotMarkers = False
		else
			Completionist_FishingSpot_Marker.Enable()
			FishingSpotMarkers = True
		endIf
		SetTextOptionValueST(GetEnabledStatus(FishingSpotMarkers))
		UpdateVariables()
		BuildMCMPages()
	EndEvent

	Event OnHighlightST()
		SetInfoText("$State_FishingSpotMarkers_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state ModNotificationsColour
	
	event OnColorOpenST()
	
		SetColorDialogStartColor(IndexColour)
		SetColorDialogDefaultColor(14905890)
	endEvent
	
	event OnColorAcceptST(Int Index)
	
		IndexColour = Index
		ColourString = GetHexValue(IndexColour)
		SetColorOptionValueST(IndexColour, false)
		Debug.Trace("Colour Picker = " + IndexColour)
	endEvent

	event OnDefaultST()
	
		IndexColour = 14905890
		ColourString = GetHexValue(IndexColour)
		SetColorOptionValueST(IndexColour, false)
	endEvent

	event OnHighlightST()
		SetInfoText("$ModNotificationsColourInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state ModNotificationsColourEnabled

	Event OnSelectST()
		NotificationColourEnabled = !NotificationColourEnabled
		SetTextOptionValueST(GetEnabledStatus(NotificationColourEnabled))
		ForcePageReset()
	EndEvent
	
	Event OnDefaultST()
		NotificationColourEnabled = True
		SetTextOptionValueST(GetEnabledStatus(NotificationColourEnabled))
		ForcePageReset()
	EndEvent

	Event OnHighlightST()

		SetInfoText("$ModNotificationsColourEnabledInfo")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state StateShout_NotColour
	
	event OnColorOpenST()
	
		SetColorDialogStartColor(StateShout_NotColourVal)
		SetColorDialogDefaultColor(10027059)
	endEvent
	
	event OnColorAcceptST(Int Index)
	
	
		StateShout_NotColourVal = Index
		ShowMessage(StateShout_NotColourVal)
		StateShout_NotColourString = GetHexValue(StateShout_NotColourVal)
		SetColorOptionValueST(StateShout_NotColourVal, false)
		Framework_UpdateShouts()
	endEvent

	event OnDefaultST()
	
		StateShout_NotColourVal = 10027059
		StateShout_NotColourString = GetHexValue(StateShout_NotColourVal)
		SetColorOptionValueST(StateShout_NotColourVal, false)
		Framework_UpdateShouts()
	endEvent

	event OnHighlightST()
		SetInfoText("$StateShout_NotColourInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CompletionLog_ColoursEnabledState

	Event OnSelectST()
		CompletionLog_ColoursEnabled = !CompletionLog_ColoursEnabled
		SetTextOptionValueST(GetEnabledStatus(CompletionLog_ColoursEnabled))
		ForcePageReset()
	EndEvent
	
	Event OnDefaultST()
		CompletionLog_ColoursEnabled = True
		SetTextOptionValueST(GetEnabledStatus(CompletionLog_ColoursEnabled))
		ForcePageReset()
	EndEvent

	Event OnHighlightST()

		SetInfoText("$CompletionLog_ColoursEnabledInfo")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CellScanner_USeClosestReferenceState

	Event OnSelectST()
		bCellScanner_UseClosestReference = !bCellScanner_UseClosestReference
		SetTextOptionValueST(GetClosestReferenceEnabledStatus(bCellScanner_UseClosestReference))
		ForcePageReset()
	EndEvent
	
	Event OnDefaultST()
		bCellScanner_UseClosestReference = True
		SetTextOptionValueST(GetClosestReferenceEnabledStatus(bCellScanner_UseClosestReference))
		ForcePageReset()
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CompletionLog_PrefixEnabledState

	Event OnSelectST()
		CompletionLog_PrefixEnabled = !CompletionLog_PrefixEnabled
		SetTextOptionValueST(GetEnabledStatus(CompletionLog_PrefixEnabled))
		ForcePageReset()
	EndEvent
	
	Event OnDefaultST()
		CompletionLog_PrefixEnabled = True
		SetTextOptionValueST(GetEnabledStatus(CompletionLog_PrefixEnabled))
		ForcePageReset()
	EndEvent

	Event OnHighlightST()

		SetInfoText("$CompletionLog_PrefixEnabledInfo")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CompletionLog_BooksTextColourState
	
	event OnColorOpenST()
	
		SetColorDialogStartColor(CompletionLog_BooksTextColourVal)
		SetColorDialogDefaultColor(15379200)
	endEvent
	
	event OnColorAcceptST(Int Index)
	
		CompletionLog_BooksTextColourVal = Index
		CompletionLog_BooksTextColourString = GetHexValue(CompletionLog_BooksTextColourVal)
		SetColorOptionValueST(CompletionLog_BooksTextColourVal, false)
	endEvent

	event OnDefaultST()
	
		CompletionLog_BooksTextColourVal = 15379200
		CompletionLog_BooksTextColourString = GetHexValue(CompletionLog_BooksTextColourVal)
		SetColorOptionValueST(CompletionLog_BooksTextColourVal, false)
	endEvent

	event OnHighlightST()
		if (bDebug)
			SetInfoText("$CompletionLog_BooksTextColourInfo" + "\n" + CompletionLog_BooksTextColourVal + "\n" + CompletionLog_BooksTextColourString)
			Return
		endIf
		SetInfoText("$CompletionLog_BooksTextColourInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CompletionLog_ItemsTextColourState

	event OnColorOpenST()
	
		SetColorDialogStartColor(CompletionLog_ItemsTextColourVal)
		SetColorDialogDefaultColor(12406553)
	endEvent
	
	event OnColorAcceptST(Int Index)
	
		CompletionLog_ItemsTextColourVal = Index
		CompletionLog_ItemsTextColourString = GetHexValue(CompletionLog_ItemsTextColourVal)
		SetColorOptionValueST(CompletionLog_ItemsTextColourVal, false)
	endEvent

	event OnDefaultST()
	
		CompletionLog_ItemsTextColourVal = 12406553
		CompletionLog_ItemsTextColourString = GetHexValue(CompletionLog_ItemsTextColourVal)
		SetColorOptionValueST(CompletionLog_ItemsTextColourVal, false)
	endEvent

	event OnHighlightST()
		if (bDebug)
			SetInfoText("$CompletionLog_ItemsTextColourInfo" + "\n" + CompletionLog_ItemsTextColourVal + "\n" + CompletionLog_ItemsTextColourString)
			Return
		endIf
		SetInfoText("$CompletionLog_ItemsTextColourInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CompletionLog_QuestsTextColourState

	event OnColorOpenST()
	
		SetColorDialogStartColor(CompletionLog_QuestsTextColourVal)
		SetColorDialogDefaultColor(9202896)
	endEvent
	
	event OnColorAcceptST(Int Index)
	
		CompletionLog_QuestsTextColourVal = Index
		CompletionLog_QuestsTextColourString = GetHexValue(CompletionLog_QuestsTextColourVal)
		SetColorOptionValueST(CompletionLog_QuestsTextColourVal, false)
	endEvent

	event OnDefaultST()
	
		CompletionLog_QuestsTextColourVal = 9202896
		CompletionLog_QuestsTextColourString = GetHexValue(CompletionLog_QuestsTextColourVal)
		SetColorOptionValueST(CompletionLog_QuestsTextColourVal, false)
	endEvent

	event OnHighlightST()
		if (bDebug)
			SetInfoText("$CompletionLog_QuestsTextColourInfo" + "\n" + CompletionLog_QuestsTextColourVal + "\n" + CompletionLog_QuestsTextColourString)
			Return
		endIf
		SetInfoText("$CompletionLog_QuestsTextColourInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CompletionLog_SpecialTextColourState

	event OnColorOpenST()
	
		SetColorDialogStartColor(CompletionLog_SpecialTextColourVal)
		SetColorDialogDefaultColor(10092543)
	endEvent
	
	event OnColorAcceptST(Int Index)
	
		CompletionLog_SpecialTextColourVal = Index
		CompletionLog_SpecialTextColourString = GetHexValue(CompletionLog_SpecialTextColourVal)
		SetColorOptionValueST(CompletionLog_SpecialTextColourVal, false)
	endEvent

	event OnDefaultST()
	
		CompletionLog_SpecialTextColourVal = 10092543
		CompletionLog_SpecialTextColourString = GetHexValue(CompletionLog_SpecialTextColourVal)
		SetColorOptionValueST(CompletionLog_SpecialTextColourVal, false)
	endEvent

	event OnHighlightST()
		if (bDebug)
			SetInfoText("$CompletionLog_SpecialTextColourInfo" + "\n" + CompletionLog_SpecialTextColourVal + "\n" + CompletionLog_SpecialTextColourString)
			Return
		endIf
		SetInfoText("$CompletionLog_SpecialTextColourInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state StateShout_GotColour
	
	event OnColorOpenST()
	
		SetColorDialogStartColor(StateShout_GotColourVal)
		SetColorDialogDefaultColor(10092543)
	endEvent
	
	event OnColorAcceptST(Int Index)
	
		StateShout_GotColourVal = Index
		ShowMessage(StateShout_GotColourVal)
		StateShout_GotColourString = GetHexValue(StateShout_GotColourVal)
		SetColorOptionValueST(StateShout_GotColourVal, false)
		Framework_UpdateShouts()
	endEvent

	event OnDefaultST()
	
		StateShout_GotColourVal = 10092543
		StateShout_GotColourString = GetHexValue(StateShout_GotColourVal)
		SetColorOptionValueST(StateShout_GotColourVal, false)
		Framework_UpdateShouts()
	endEvent

	event OnHighlightST()
		SetInfoText("$StateShout_GotColourInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_CompletionColour
	
	event OnColorOpenST()
	
		SetColorDialogStartColor(State_AutomaticCompletionColourVal)
		SetColorDialogDefaultColor(10398469)
	endEvent
	
	event OnColorAcceptST(Int Index)
	
		State_AutomaticCompletionColourVal = Index
		State_AutomaticCompletionColourString = GetHexValue(State_AutomaticCompletionColourVal)
		SetColorOptionValueST(State_AutomaticCompletionColourVal, false)
	endEvent

	event OnDefaultST()
	
		State_AutomaticCompletionColourVal = 10398469
		State_AutomaticCompletionColourString = GetHexValue(State_AutomaticCompletionColourVal)
		SetColorOptionValueST(State_AutomaticCompletionColourVal, false)
	endEvent

	event OnHighlightST()
		SetInfoText("$State_AutomaticCompletionColourInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_ManualCompletionColour
	
	event OnColorOpenST()
	
		SetColorDialogStartColor(State_ManualCompletionColourVal)
		SetColorDialogDefaultColor(10398469)
	endEvent
	
	event OnColorAcceptST(Int Index)
	
		State_ManualCompletionColourVal = Index
		State_ManualCompletionColourString = GetHexValue(State_ManualCompletionColourVal)
		SetColorOptionValueST(State_ManualCompletionColourVal, false)
	endEvent

	event OnDefaultST()
	
		State_ManualCompletionColourVal = 10398469
		State_ManualCompletionColourString = GetHexValue(State_ManualCompletionColourVal)
		SetColorOptionValueST(State_ManualCompletionColourVal, false)
	endEvent

	event OnHighlightST()
		SetInfoText("$State_ManualCompletionColourInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_MiscCompletionColour
	
	event OnColorOpenST()
	
		SetColorDialogStartColor(State_MiscCompletionColourVal)
		SetColorDialogDefaultColor(10398469)
	endEvent
	
	event OnColorAcceptST(Int Index)
	
		State_MiscCompletionColourVal = Index
		State_MiscCompletionColourString = GetHexValue(State_MiscCompletionColourVal)
		SetColorOptionValueST(State_MiscCompletionColourVal, false)
	endEvent

	event OnDefaultST()
	
		State_MiscCompletionColourVal = 10398469
		State_MiscCompletionColourString = GetHexValue(State_MiscCompletionColourVal)
		SetColorOptionValueST(State_MiscCompletionColourVal, false)
	endEvent

	event OnHighlightST()
		SetInfoText("$State_MiscCompletionColourInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state InventoryModeOptions_PrAp_N

	event OnMenuOpenST()
		SetMenuDialogStartIndex(InventoryMode_PrAp_Choice_N)
		SetMenuDialogDefaultIndex(0)
		SetMenuDialogOptions(InventoryMode_PrAp_List_N)
	endEvent

	event OnMenuAcceptST(int index)
		InventoryMode_PrAp_Choice_N = Index
		SetMenuOptionValueST(InventoryModeOptions_PrAp_N, InventoryMode_PrAp_List_N[InventoryMode_PrAp_Choice_N])				
		ForcePageReset()
	endEvent

	event OnDefaultST()
		InventoryMode_PrAp_Choice_N = 4
		SetMenuOptionValueST(InventoryMode_PrAp_List_N[InventoryMode_PrAp_Choice_N])
	endEvent

	event OnHighlightST()
		SetInfoText("$InventoryModeOptions_PrAp_Info_N")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state InventoryModeOptions_PrAp_G

	event OnMenuOpenST()
		SetMenuDialogStartIndex(InventoryMode_PrAp_Choice_G)
		SetMenuDialogDefaultIndex(0)
		SetMenuDialogOptions(InventoryMode_PrAp_List_G)
	endEvent

	event OnMenuAcceptST(int index)
		InventoryMode_PrAp_Choice_G = Index
		SetMenuOptionValueST(InventoryModeOptions_PrAp_G, InventoryMode_PrAp_List_G[InventoryMode_PrAp_Choice_G])				
		ForcePageReset()
	endEvent

	event OnDefaultST()
		InventoryMode_PrAp_Choice_G = 4
		SetMenuOptionValueST(InventoryMode_PrAp_List_G[InventoryMode_PrAp_Choice_G])
	endEvent

	event OnHighlightST()
		SetInfoText("$InventoryModeOptions_PrAp_Info_G")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state InventoryModeOptions_PrFx_N

	event OnMenuOpenST()
		SetMenuDialogStartIndex(InventoryMode_PrFx_Choice_N)
		SetMenuDialogDefaultIndex(0)
		SetMenuDialogOptions(InventoryMode_PrFx_List_N)
	endEvent

	event OnMenuAcceptST(int index)
		InventoryMode_PrFx_Choice_N = Index
		SetMenuOptionValueST(InventoryModeOptions_PrFx_N, InventoryMode_PrFx_List_N[InventoryMode_PrFx_Choice_N])				
		ForcePageReset()
	endEvent

	event OnDefaultST()
		InventoryMode_PrFx_Choice_N = 0
		SetMenuOptionValueST(InventoryMode_PrFx_List_N[InventoryMode_PrFx_Choice_N])
	endEvent

	event OnHighlightST()
		SetInfoText("$InventoryModeOptions_PrFx_Info_N")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state InventoryModeOptions_PrFx_G

	event OnMenuOpenST()
		SetMenuDialogStartIndex(InventoryMode_PrFx_Choice_G)
		SetMenuDialogDefaultIndex(0)
		SetMenuDialogOptions(InventoryMode_PrFx_List_G)
	endEvent

	event OnMenuAcceptST(int index)
		InventoryMode_PrFx_Choice_G = Index
		SetMenuOptionValueST(InventoryModeOptions_PrFx_G, InventoryMode_PrFx_List_G[InventoryMode_PrFx_Choice_G])				
		ForcePageReset()
	endEvent

	event OnDefaultST()
		InventoryMode_PrFx_Choice_G = 0
		SetMenuOptionValueST(InventoryMode_PrFx_List_G[InventoryMode_PrFx_Choice_G])
	endEvent

	event OnHighlightST()
		SetInfoText("$InventoryModeOptions_PrFx_Info_G")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_MarkerDetection ; SLIDER

	event OnSliderOpenST()
		SetSliderDialogStartValue(State_MarkerDetectionVal)
		SetSliderDialogDefaultValue(1500)
		SetSliderDialogRange(100, 3000)
		SetSliderDialogInterval(100)
	endEvent

	event OnSliderAcceptST(float a_value)
		State_MarkerDetectionVal = a_value as int
		SetSliderOptionValueST(State_MarkerDetectionVal, "{0}")
	endEvent

	event OnDefaultST()
		State_MarkerDetectionVal = 1500
		SetSliderOptionValueST(State_MarkerDetectionVal)
		self.SetSliderOptionValueST(10 as Float, "{0}")
	endEvent

	event OnHighlightST()
		SetInfoText("$State_MarkerDetectionInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_CompanionsCounter ; SLIDER

	event OnSliderOpenST()
		SetSliderDialogStartValue(State_CompanionsCounterVal)
		SetSliderDialogDefaultValue(5)
		SetSliderDialogRange(1, 20)
		SetSliderDialogInterval(1)
	endEvent

	event OnSliderAcceptST(float a_value)
		State_CompanionsCounterVal = a_value as int
		SetSliderOptionValueST(State_CompanionsCounterVal, "{0}")
		UpdateVariables()
	endEvent

	event OnDefaultST()
		State_CompanionsCounterVal = 5
		SetSliderOptionValueST(State_CompanionsCounterVal)
		self.SetSliderOptionValueST(5 as Float, "{0}")
	endEvent

	event OnHighlightST()
		SetInfoText("$State_CompanionsCounterInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_CollegeCounter ; SLIDER

	event OnSliderOpenST()
		SetSliderDialogStartValue(State_CollegeCounterVal)
		SetSliderDialogDefaultValue(5)
		SetSliderDialogRange(1, 20)
		SetSliderDialogInterval(1)
	endEvent

	event OnSliderAcceptST(float a_value)
		State_CollegeCounterVal = a_value as int
		SetSliderOptionValueST(State_CollegeCounterVal, "{0}")
		UpdateVariables()
	endEvent

	event OnDefaultST()
		State_CollegeCounterVal = 5
		SetSliderOptionValueST(State_CollegeCounterVal)
		self.SetSliderOptionValueST(5 as Float, "{0}")
	endEvent

	event OnHighlightST()
		SetInfoText("$State_CollegeCounterInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_DBCounter ; SLIDER

	event OnSliderOpenST()
		SetSliderDialogStartValue(State_DBCounterVal)
		SetSliderDialogDefaultValue(5)
		SetSliderDialogRange(1, 20)
		SetSliderDialogInterval(1)
	endEvent

	event OnSliderAcceptST(float a_value)
		State_DBCounterVal = a_value as int
		SetSliderOptionValueST(State_DBCounterVal, "{0}")
		UpdateVariables()
	endEvent

	event OnDefaultST()
		State_DBCounterVal = 5
		SetSliderOptionValueST(State_DBCounterVal)
		self.SetSliderOptionValueST(5 as Float, "{0}")
	endEvent

	event OnHighlightST()
		SetInfoText("$State_DBCounterInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_TheBladesCounter ; SLIDER

	event OnSliderOpenST()
		SetSliderDialogStartValue(State_BladesCounterVal)
		SetSliderDialogDefaultValue(5)
		SetSliderDialogRange(1, 20)
		SetSliderDialogInterval(1)
	endEvent

	event OnSliderAcceptST(float a_value)
		State_BladesCounterVal = a_value as int
		SetSliderOptionValueST(State_BladesCounterVal, "{0}")
		UpdateVariables()
	endEvent

	event OnDefaultST()
		State_BladesCounterVal = 5
		SetSliderOptionValueST(State_BladesCounterVal)
		self.SetSliderOptionValueST(5 as Float, "{0}")
	endEvent

	event OnHighlightST()
		SetInfoText("$State_TheBladesCounterInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_RadiantCounter ; SLIDER

	event OnSliderOpenST()
		SetSliderDialogStartValue(State_RadiantCounterVal)
		SetSliderDialogDefaultValue(5)
		SetSliderDialogRange(1, 20)
		SetSliderDialogInterval(1)
	endEvent

	event OnSliderAcceptST(float a_value)
		State_RadiantCounterVal = a_value as int
		SetSliderOptionValueST(State_RadiantCounterVal, "{0}")
		UpdateVariables()
	endEvent

	event OnDefaultST()
		State_RadiantCounterVal = 5
		SetSliderOptionValueST(State_RadiantCounterVal)
		self.SetSliderOptionValueST(5 as Float, "{0}")
	endEvent

	event OnHighlightST()
		SetInfoText("$State_RadiantCounterInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_BountyCounter ; SLIDER

	event OnSliderOpenST()
		SetSliderDialogStartValue(State_BountyCounterVal)
		SetSliderDialogDefaultValue(5)
		SetSliderDialogRange(1, 20)
		SetSliderDialogInterval(1)
	endEvent

	event OnSliderAcceptST(float a_value)
		State_BountyCounterVal = a_value as int
		SetSliderOptionValueST(State_BountyCounterVal, "{0}")
		UpdateVariables()
	endEvent

	event OnDefaultST()
		State_BountyCounterVal = 5
		SetSliderOptionValueST(State_BountyCounterVal)
		self.SetSliderOptionValueST(5 as Float, "{0}")
	endEvent

	event OnHighlightST()
		SetInfoText("$State_BountyCounterInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_TGRCounter ; SLIDER

	event OnSliderOpenST()
		SetSliderDialogStartValue(State_TGRCounterVal)
		SetSliderDialogDefaultValue(5)
		SetSliderDialogRange(1, 20)
		SetSliderDialogInterval(1)
	endEvent

	event OnSliderAcceptST(float a_value)
		State_TGRCounterVal = a_value as int
		SetSliderOptionValueST(State_TGRCounterVal, "{0}")
		UpdateVariables()
	endEvent

	event OnDefaultST()
		State_TGRCounterVal = 5
		SetSliderOptionValueST(State_TGRCounterVal)
		self.SetSliderOptionValueST(5 as Float, "{0}")
	endEvent

	event OnHighlightST()
		SetInfoText("$State_TGRCounterInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_DLC1Counter ; SLIDER

	event OnSliderOpenST()
		SetSliderDialogStartValue(State_DLC1CounterVal)
		SetSliderDialogDefaultValue(5)
		SetSliderDialogRange(1, 20)
		SetSliderDialogInterval(1)
	endEvent

	event OnSliderAcceptST(float a_value)
		State_DLC1CounterVal = a_value as int
		SetSliderOptionValueST(State_DLC1CounterVal, "{0}")
		UpdateVariables()
	endEvent

	event OnDefaultST()
		State_DLC1CounterVal = 5
		SetSliderOptionValueST(State_DLC1CounterVal)
		self.SetSliderOptionValueST(5 as Float, "{0}")
	endEvent

	event OnHighlightST()
		SetInfoText("$State_DLC1CounterInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_LOTDCounter ; SLIDER

	event OnSliderOpenST()
		SetSliderDialogStartValue(State_LOTDCounterVal)
		SetSliderDialogDefaultValue(5)
		SetSliderDialogRange(1, 20)
		SetSliderDialogInterval(1)
	endEvent

	event OnSliderAcceptST(float a_value)
		State_LOTDCounterVal = a_value as int
		SetSliderOptionValueST(State_LOTDCounterVal, "{0}")
		UpdateVariables()
	endEvent

	event OnDefaultST()
		State_LOTDCounterVal = 5
		SetSliderOptionValueST(State_LOTDCounterVal)
		self.SetSliderOptionValueST(5 as Float, "{0}")
	endEvent

	event OnHighlightST()
		SetInfoText("$State_LOTDCounterInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_VigilantCounter ; SLIDER

	event OnSliderOpenST()
		SetSliderDialogStartValue(State_VigilantCounterVal)
		SetSliderDialogDefaultValue(5)
		SetSliderDialogRange(1, 20)
		SetSliderDialogInterval(1)
	endEvent

	event OnSliderAcceptST(float a_value)
		State_VigilantCounterVal = a_value as int
		SetSliderOptionValueST(State_VigilantCounterVal, "{0}")
		UpdateVariables()
	endEvent

	event OnDefaultST()
		State_VigilantCounterVal = 5
		SetSliderOptionValueST(State_VigilantCounterVal)
		self.SetSliderOptionValueST(5 as Float, "{0}")
	endEvent

	event OnHighlightST()
		SetInfoText("$State_VigilantCounterInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_FishingCounter ; SLIDER

	event OnSliderOpenST()
		SetSliderDialogStartValue(State_FishingCounterVal)
		SetSliderDialogDefaultValue(4)
		SetSliderDialogRange(1, 4)
		SetSliderDialogInterval(1)
	endEvent

	event OnSliderAcceptST(float a_value)
		State_FishingCounterVal = a_value as int
		SetSliderOptionValueST(State_FishingCounterVal, "{0}")
		UpdateVariables()
	endEvent

	event OnDefaultST()
		State_FishingCounterVal = 4
		SetSliderOptionValueST(State_FishingCounterVal)
		self.SetSliderOptionValueST(4 as Float, "{0}")
	endEvent

	event OnHighlightST()
		SetInfoText("$State_FishingCounterInfo")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State State_MarkerDetectionCancel

	Event OnSelectST()
		Completionist_Marker.SetObjectiveDisplayed(10, False, True)	
		(LocMarker as Completionist_CancelMarker).UnregisterForUpdate()
		LocMarker.MoveToMyEditorLocation()
		CompMCM2.CurMarker = ""
		CompMCM2.CurLocation = None	
		CompMCM3.CurMarker = ""
		CompMCM3.CurLocation = None	
	EndEvent

	Event OnHighlightST()

		SetInfoText("$State_MarkerDetectionCancelInfo")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_Shortcuts

	Event OnSelectST()
		bShortCutActive = !bShortCutActive
		
		SetTextOptionValueST(GetEnabledStatus(bShortCutActive))
		ForcePageReset()
	EndEvent
	
	Event OnDefaultST()
		bShortCutActive = False
		MainJumpPage = ""
		MiscJumpPage = ""
		MainMCMPagesChoice = 0
		MiscMCMPagesChoice = 0
		SetTextOptionValueST(GetEnabledStatus(bShortCutActive))
	EndEvent

	Event OnHighlightST()

		SetInfoText("$State_ShortcutsInfo")
	EndEvent
endState

;---------------------------------------------------
;-- Text Input Events ------------------------------
;---------------------------------------------------

String Function GetFontOption(String s, bool O)
	
	if (O)
		return "<font color='" + s + "'>" + s + "</font>"
	endIf
	
	return s
endFunction

;---------------------------------------------------
;-- END OF CODE ------------------------------------
;---------------------------------------------------

Function OverRide_G_Name(String InputString)
		
	if (InputString != "") 
		State_OverRide_G_Name_String = InputString
		SetInputOptionValue(OID_OverRide_G_Name, InputString)
	else
		State_OverRide_G_Name_String = "Got It!"
		SetInputOptionValue(OID_OverRide_G_Name, InputString)
	endIf
endFunction

Function OverRide_N_Name(String InputString)
		
	if (InputString != "") 
		State_OverRide_N_Name_String = InputString
		SetInputOptionValue(OID_OverRide_N_Name, InputString)
	else
		State_OverRide_N_Name_String = "Need It!"
		SetInputOptionValue(OID_OverRide_N_Name, InputString)
	endIf
endFunction

Function Search(String InputString)
		
	if (InputString != "") 
		State_SearchTermString = InputString
		SetInputOptionValue(OID_SearhTerm, InputString)
	else
		State_SearchTermString = "Enter Search Term..."
		SetInputOptionValue(OID_SearhTerm, InputString)
	endIf
endFunction

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state ColourState_G_HUD_Crosshair
	
	event OnColorOpenST()
	
		SetColorDialogStartColor(State_ColourVal_G_HUD_Crosshair)
		SetColorDialogDefaultColor(1288220)
	endEvent
	
	event OnColorAcceptST(Int Index)
	
		State_ColourVal_G_HUD_Crosshair = Index
		State_ColourString_G_HUD_Crosshair = GetHexValue(State_ColourVal_G_HUD_Crosshair)
		SetColorOptionValueST(State_ColourVal_G_HUD_Crosshair, false)
	endEvent

	event OnDefaultST()
	
		State_ColourVal_G_HUD_Crosshair = 1288220
		State_ColourString_G_HUD_Crosshair = GetHexValue(State_ColourVal_G_HUD_Crosshair)
		SetColorOptionValueST(State_ColourVal_G_HUD_Crosshair, false)
	endEvent

	event OnHighlightST()
		SetInfoText("$ColourState_G_Generic_Info_Crosshair")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state ColourState_N_HUD_Crosshair
	
	event OnColorOpenST()
	
		SetColorDialogStartColor(State_ColourVal_N_HUD_Crosshair)
		SetColorDialogDefaultColor(4430046)
	endEvent
	
	event OnColorAcceptST(Int Index)
	
		State_ColourVal_N_HUD_Crosshair = Index
		State_ColourString_N_HUD_Crosshair = GetHexValue(State_ColourVal_N_HUD_Crosshair)
		SetColorOptionValueST(State_ColourVal_N_HUD_Crosshair, false)
	endEvent

	event OnDefaultST()
	
		State_ColourVal_N_HUD_Crosshair = 4430046
		State_ColourString_N_HUD_Crosshair = GetHexValue(State_ColourVal_N_HUD_Crosshair)
		SetColorOptionValueST(State_ColourVal_N_HUD_Crosshair, false)
	endEvent

	event OnHighlightST()
		SetInfoText("$ColourState_N_Generic_Info_Crosshair")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state ColourState_G_HUD_Menus
	
	event OnColorOpenST()
	
		SetColorDialogStartColor(State_ColourVal_G_HUD_Menus)
		SetColorDialogDefaultColor(1288220)
	endEvent
	
	event OnColorAcceptST(Int Index)
	
		State_ColourVal_G_HUD_Menus = Index
		State_ColourString_G_HUD_Menus = GetHexValue(State_ColourVal_G_HUD_Menus)
		SetColorOptionValueST(State_ColourVal_G_HUD_Menus, false)
	endEvent

	event OnDefaultST()
	
		State_ColourVal_G_HUD_Menus = 1288220
		State_ColourString_G_HUD_Menus = GetHexValue(State_ColourVal_G_HUD_Menus)
		SetColorOptionValueST(State_ColourVal_G_HUD_Menus, false)
	endEvent

	event OnHighlightST()
		SetInfoText("$ColourState_G_Generic_Info_Menus")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state ColourState_N_HUD_Menus
	
	event OnColorOpenST()
	
		SetColorDialogStartColor(State_ColourVal_N_HUD_Menus)
		SetColorDialogDefaultColor(4430046)
	endEvent
	
	event OnColorAcceptST(Int Index)
	
		State_ColourVal_N_HUD_Menus = Index
		State_ColourString_N_HUD_Menus = GetHexValue(State_ColourVal_N_HUD_Menus)
		SetColorOptionValueST(State_ColourVal_N_HUD_Menus, false)
	endEvent

	event OnDefaultST()
	
		State_ColourVal_N_HUD_Menus = 4430046
		State_ColourString_N_HUD_Menus = GetHexValue(State_ColourVal_N_HUD_Menus)
		SetColorOptionValueST(State_ColourVal_N_HUD_Menus, false)
	endEvent

	event OnHighlightST()
		SetInfoText("$ColourState_N_Generic_Info_Menus")
	endEvent
endState

;---------------------------------------------------
;-- Text Input Events ------------------------------
;---------------------------------------------------

Function OverRide_G_Colour_Crosshair(String HexString)

	if HexString == "Clear"
		if (b_CustomColour_G_HUD_Crosshair)
		
			State_CustomColourString_G_HUD_Crosshair = "Enter Decimal"
			State_CustomColourVal_G_HUD_Crosshair = -1
			
			b_CustomColour_G_HUD_Crosshair = False
			ShowMessage("$ColourClear1")
			ForcePageReset()
		else
			ShowMessage("$ColourClear2")
			b_CustomColour_G_HUD_Crosshair = False
		endIf
	endIf
		
	if (HexString != "Enter Decimal") && (HexString != "Clear")
		
		if ((HexString as Int) == 0)
			ShowMessage("$ColourError")
			return
		endIf
		
		b_CustomColour_G_HUD_Crosshair = True
		
		State_CustomColourVal_G_HUD_Crosshair = (HexString as Int)
		State_CustomColourString_G_HUD_Crosshair = GetHexValue(State_CustomColourVal_G_HUD_Crosshair)
		
		SetInputOptionValue(OID_CustomColour_G_HUD_Crosshair, GetFontOption(State_CustomColourString_G_HUD_Crosshair, b_CustomColour_G_HUD_Crosshair))
		ForcePageReset()
		
	endIf
endFunction

;---------------------------------------------------
;-- Text Input Events ------------------------------
;---------------------------------------------------

Function OverRide_N_Colour_Crosshair(String HexString)

	if HexString == "Clear"
		if (b_CustomColour_N_HUD_Crosshair)
		
			State_CustomColourString_N_HUD_Crosshair = "Enter Decimal"
			State_CustomColourVal_N_HUD_Crosshair = -1
			
			b_CustomColour_N_HUD_Crosshair = False
			ShowMessage("$ColourClear1")
			ForcePageReset()
		else
			ShowMessage("$ColourClear2")
			b_CustomColour_N_HUD_Crosshair = False
		endIf
	endIf
		
	if (HexString != "Enter Decimal") && (HexString != "Clear")

		if ((HexString as Int) == 0)
			ShowMessage("$ColourError")
			return
		endIf
		
		b_CustomColour_N_HUD_Crosshair = True
		
		State_CustomColourVal_N_HUD_Crosshair = (HexString as Int)
		State_CustomColourString_N_HUD_Crosshair = GetHexValue(State_CustomColourVal_N_HUD_Crosshair)
		
		SetInputOptionValue(OID_CustomColour_N_HUD_Crosshair, GetFontOption(State_CustomColourString_N_HUD_Crosshair, b_CustomColour_N_HUD_Crosshair))
		ForcePageReset()
		
	endIf
endFunction

;---------------------------------------------------
;-- Text Input Events ------------------------------
;---------------------------------------------------

Function OverRide_G_Colour_Menus(String HexString)

	if HexString == "Clear"
		if (b_CustomColour_G_HUD_Menus)
		
			State_CustomColourString_G_HUD_Menus = "Enter Decimal"
			State_CustomColourVal_G_HUD_Menus = -1
			
			b_CustomColour_G_HUD_Menus = False
			ShowMessage("$ColourClear1")
			ForcePageReset()
		else
			ShowMessage("$ColourClear2")
			b_CustomColour_G_HUD_Menus = False
		endIf
	endIf
		
	if (HexString != "Enter Decimal") && (HexString != "Clear")
		
		if ((HexString as Int) == 0)
			ShowMessage("$ColourError")
			return
		endIf
		
		b_CustomColour_G_HUD_Menus = True
		
		State_CustomColourVal_G_HUD_Menus = (HexString as Int)
		State_CustomColourString_G_HUD_Menus = GetHexValue(State_CustomColourVal_G_HUD_Menus)
		
		SetInputOptionValue(OID_CustomColour_G_HUD_Menus, GetFontOption(State_CustomColourString_G_HUD_Menus, b_CustomColour_G_HUD_Menus))
		ForcePageReset()
		
	endIf
endFunction

;---------------------------------------------------
;-- Text Input Events ------------------------------
;---------------------------------------------------

Function OverRide_N_Colour_Menus(String HexString)

	if HexString == "Clear"
		if (b_CustomColour_N_HUD_Menus)
		
			State_CustomColourString_N_HUD_Menus = "Enter Decimal"
			State_CustomColourVal_N_HUD_Menus = -1
			
			b_CustomColour_N_HUD_Menus = False
			ShowMessage("$ColourClear1")
			ForcePageReset()
		else
			ShowMessage("$ColourClear2")
			b_CustomColour_N_HUD_Menus = False
		endIf
	endIf
		
	if (HexString != "Enter Decimal") && (HexString != "Clear")

		if ((HexString as Int) == 0)
			ShowMessage("$ColourError")
			return
		endIf
		
		b_CustomColour_N_HUD_Menus = True
		
		State_CustomColourVal_N_HUD_Menus = (HexString as Int)
		State_CustomColourString_N_HUD_Menus = GetHexValue(State_CustomColourVal_N_HUD_Menus)
		
		SetInputOptionValue(OID_CustomColour_N_HUD_Menus, GetFontOption(State_CustomColourString_N_HUD_Menus, b_CustomColour_N_HUD_Menus))
		ForcePageReset()
		
	endIf
endFunction

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_quickLootEnabled

	Event OnSelectST()
		b_quickLoot_Enabled = !b_quickLoot_Enabled
		SetTextOptionValueST(GetEnabledStatus(b_quickLoot_Enabled))
	EndEvent
	
	Event OnDefaultST()
		b_quickLoot_Enabled = True
		SetTextOptionValueST(GetEnabledStatus(b_quickLoot_Enabled))
	EndEvent

	Event OnHighlightST()

		SetInfoText("$State_quickLootEnabled_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_moreHUDEnabled_Crosshair

	Event OnSelectST()
		b_moreHUDEnabled_Crosshair = !b_moreHUDEnabled_Crosshair
		SetTextOptionValueST(GetEnabledStatus(b_moreHUDEnabled_Crosshair))
	EndEvent
	
	Event OnDefaultST()
		b_moreHUDEnabled_Crosshair = True
		SetTextOptionValueST(GetEnabledStatus(b_moreHUDEnabled_Crosshair))
	EndEvent

	Event OnHighlightST()

		SetInfoText("$State_moreHUDEnabled_Crosshair_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_moreHUDEnabled_Menus

	Event OnSelectST()
		b_moreHUDEnabled_Menus = !b_moreHUDEnabled_Menus
		SetTextOptionValueST(GetEnabledStatus(b_moreHUDEnabled_Menus))
	EndEvent
	
	Event OnDefaultST()
		b_moreHUDEnabled_Menus = True
		SetTextOptionValueST(GetEnabledStatus(b_moreHUDEnabled_Menus))
	EndEvent

	Event OnHighlightST()

		SetInfoText("$State_moreHUDEnabled_Menus_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CellScanner_Setting_REFS

	Event OnSelectST()
		bCellScanner_REFS = !bCellScanner_REFS
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_REFS))
	EndEvent
	
	Event OnDefaultST()
		bCellScanner_REFS = True
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_REFS))
	EndEvent

	Event OnHighlightST()

		SetInfoText("$CellScanner_Setting_REFS_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CellScanner_Setting_Quest

	Event OnSelectST()
		bCellScanner_MARK = !bCellScanner_MARK
		
		if (!bCellScanner_MARK)
			Completionist_ItemFinder_Item.Clear()
			Completionist_ItemFinder_Name.Clear()
			Completionist_ItemFinder_Type.Clear()
			Completionist_ItemFinder_Quest.SetObjectiveDisplayed(10, False, True)
		endIf
		
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_MARK))
		ForcePageReset()
	EndEvent
	
	Event OnDefaultST()
		bCellScanner_MARK = True
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_MARK))
		ForcePageReset()
	EndEvent

	Event OnHighlightST()
		SetInfoText("$CellScanner_Setting_Quest_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CellScanner_Setting_ExcludeAlchemy

	Event OnSelectST()
		bCellScanner_ExcludeAlchemy = !bCellScanner_ExcludeAlchemy		
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_ExcludeAlchemy))
	EndEvent
	
	Event OnDefaultST()
		bCellScanner_ExcludeAlchemy = True
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_ExcludeAlchemy))
	EndEvent

	Event OnHighlightST()

		SetInfoText("$CellScanner_Setting_ExcludeAlchemy_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CellScanner_Setting_Range ; SLIDER

	event OnSliderOpenST()
		SetSliderDialogStartValue(iCellScanner_Range)
		SetSliderDialogDefaultValue(100)
		SetSliderDialogRange(10, 500)
		SetSliderDialogInterval(1)
	endEvent

	event OnSliderAcceptST(float a_value)
		iCellScanner_Range = a_value as int
		SetSliderOptionValueST(iCellScanner_Range, "{0}")
	endEvent

	event OnDefaultST()
		iCellScanner_Range = 100
		SetSliderOptionValueST(iCellScanner_Range)
		self.SetSliderOptionValueST(5000 as Float, "{0}")
	endEvent

	event OnHighlightST()
		SetInfoText("$CellScanner_Setting_Range_Info")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CellScanner_Setting_ExcludeArmor

	Event OnSelectST()
		bCellScanner_ExcludeArmor = !bCellScanner_ExcludeArmor		
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_ExcludeArmor))
	EndEvent
	
	Event OnDefaultST()
		bCellScanner_ExcludeArmor = True
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_ExcludeArmor))
	EndEvent

	Event OnHighlightST()

		SetInfoText("$CellScanner_Setting_ExcludeArmor_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CellScanner_Setting_ExcludeWeapons

	Event OnSelectST()
		bCellScanner_ExcludeWeapons = !bCellScanner_ExcludeWeapons		
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_ExcludeWeapons))
	EndEvent
	
	Event OnDefaultST()
		bCellScanner_ExcludeWeapons = True
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_ExcludeWeapons))
	EndEvent

	Event OnHighlightST()

		SetInfoText("$CellScanner_Setting_ExcludeWeapons_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CellScanner_Setting_ExcludeBooks

	Event OnSelectST()
		bCellScanner_ExcludeBooks = !bCellScanner_ExcludeBooks		
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_ExcludeBooks))
	EndEvent
	
	Event OnDefaultST()
		bCellScanner_ExcludeBooks = True
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_ExcludeBooks))
	EndEvent

	Event OnHighlightST()

		SetInfoText("$CellScanner_Setting_ExcludeBooks_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CellScanner_Setting_ExcludeMiscItems

	Event OnSelectST()
		bCellScanner_ExcludeMiscItems = !bCellScanner_ExcludeMiscItems		
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_ExcludeMiscItems))
	EndEvent
	
	Event OnDefaultST()
		bCellScanner_ExcludeMiscItems = True
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_ExcludeMiscItems))
	EndEvent

	Event OnHighlightST()

		SetInfoText("$CellScanner_Setting_ExcludeMiscItems_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CellScanner_Setting_CONT

	Event OnSelectST()
		bCellScanner_CONT = !bCellScanner_CONT
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_CONT))
	EndEvent
	
	Event OnDefaultST()
		bCellScanner_CONT = True
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_CONT))
	EndEvent

	Event OnHighlightST()

		SetInfoText("$CellScanner_Setting_CONT_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CellScanner_Setting_NPCS

	Event OnSelectST()
		bCellScanner_NPCS = !bCellScanner_NPCS
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_NPCS))
	EndEvent
	
	Event OnDefaultST()
		bCellScanner_NPCS = True
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_NPCS))
	EndEvent

	Event OnHighlightST()

		SetInfoText("$CellScanner_Setting_NPCS_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CellScanner_Setting_DETA

	Event OnSelectST()
		bCellScanner_DETA = !bCellScanner_DETA
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_DETA))
	EndEvent
	
	Event OnDefaultST()
		bCellScanner_DETA = True
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_DETA))
	EndEvent

	Event OnHighlightST()

		SetInfoText("$CellScanner_Setting_DETA_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CellScanner_Setting_Pinning_Enabled

	Event OnSelectST()
		bCellScanner_Pinning_Enabled = !bCellScanner_Pinning_Enabled

		if (!bCellScanner_Pinning_Enabled)
			ClearPinnedTarget(true)
		else
			(Completionist_PinnedItemFinder_Item as Completionist_PinnedItemScript).SetDisplayparamters(PinnedForm, bCellScanner_Pinning_Effect, CellScanner_Pinning_Sound_Choice)
		endIf
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_Pinning_Enabled))
		ForcePageReset()
	EndEvent
	
	Event OnDefaultST()
		bCellScanner_Pinning_Enabled = True
		(Completionist_PinnedItemFinder_Item as Completionist_PinnedItemScript).SetDisplayparamters(PinnedForm, bCellScanner_Pinning_Effect, CellScanner_Pinning_Sound_Choice)
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_Pinning_Enabled))
		ForcePageReset()
	EndEvent

	Event OnHighlightST()
		SetInfoText("$CellScanner_Setting_Pinning_Enabled_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CellScanner_Setting_Pinning_ClearPin

	Event OnSelectST()
		ClearPinnedTarget(true)
		ForcePageReset()
	EndEvent

	Event OnHighlightST()
		SetInfoText("$CellScanner_Setting_Pinning_ClearPin_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CellScanner_Setting_Pinning_Marker

	Event OnSelectST()
		bCellScanner_Pinning_Marker = !bCellScanner_Pinning_Marker

		if (bCellScanner_Pinning_Enabled && bCellScanner_Pinning_Marker)
			if (Completionist_PinnedItemFinder_Item.GetReference())
				Completionist_PinnedItemFinder_Quest.SetObjectiveDisplayed(10, True, True)
			endIf
		else
			if (Completionist_PinnedItemFinder_Item.GetReference())
				Completionist_PinnedItemFinder_Quest.SetObjectiveDisplayed(10, False, False)
			endIf
		endIf		
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_Pinning_Marker))
	EndEvent
	
	Event OnDefaultST()
		bCellScanner_Pinning_Marker = True
		if (Completionist_PinnedItemFinder_Item.GetReference())
			Completionist_PinnedItemFinder_Quest.SetObjectiveDisplayed(10, True, True)
		endIf		
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_Pinning_Marker))
	EndEvent

	Event OnHighlightST()

		SetInfoText("$CellScanner_Setting_Pinning_Marker_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CellScanner_Setting_Pinning_Effect

	Event OnSelectST()
		bCellScanner_Pinning_Effect = !bCellScanner_Pinning_Effect
		(Completionist_PinnedItemFinder_Item as Completionist_PinnedItemScript).UpdateGlowParameters(bCellScanner_Pinning_Effect)
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_Pinning_Effect))
	EndEvent
	
	Event OnDefaultST()
		bCellScanner_Pinning_Effect = True
		(Completionist_PinnedItemFinder_Item as Completionist_PinnedItemScript).UpdateGlowParameters(bCellScanner_Pinning_Effect)
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_Pinning_Effect))
	EndEvent

	Event OnHighlightST()
		SetInfoText("$CellScanner_Setting_Pinning_Effect_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CellScanner_Setting_NUMB

	Event OnSelectST()
		bCellScanner_NUMB = !bCellScanner_NUMB
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_NUMB))
	EndEvent
	
	Event OnDefaultST()
		bCellScanner_NUMB = True
		SetTextOptionValueST(GetEnabledStatus(bCellScanner_NUMB))
	EndEvent

	Event OnHighlightST()

		SetInfoText("$CellScanner_Setting_NUMB_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state State_CellScanner_Pinning_Sound

	event OnMenuOpenST()
		SetMenuDialogStartIndex(CellScanner_Pinning_Sound_Choice)
		SetMenuDialogDefaultIndex(0)
		SetMenuDialogOptions(CellScanner_Pinning_Sound_List)
	endEvent

	event OnMenuAcceptST(int index)
		CellScanner_Pinning_Sound_Choice = Index
		SetMenuOptionValueST(State_CellScanner_Pinning_Sound, CellScanner_Pinning_Sound_List[CellScanner_Pinning_Sound_Choice])		
		;/if (CellScanner_Pinning_Sound_Choice > 0)
			((Completionist_PinnedItemSoundList.GetAt(CellScanner_Pinning_Sound_Choice)) as sound).PlayAndWait(Game.GetPlayer())
		endIf/;	
		ForcePageReset()
	endEvent

	event OnDefaultST()
		CellScanner_Pinning_Sound_Choice = 0
		SetMenuOptionValueST(State_CellScanner_Pinning_Sound, CellScanner_Pinning_Sound_List[CellScanner_Pinning_Sound_Choice])
	endEvent

	event OnHighlightST()
		SetInfoText("$CellScanner_Pinning_Sound_Info")
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CellScanner_Setting_EXCL

	Event OnSelectST()
		Cell curCell = PlayerRef.GetParentCell()
		
		if (!Completionist_ExcludedCells.HasForm(curCell))
			if (ShowMessage("$CellScanner_Setting_EXCL_Message1{" + curCell.GetName() + "}", true, "$ConfirmYes", "$ConfirmNo"))
				Completionist_ExcludedCells.AddForm(curCell as Cell)
				ForcePageReset()
			endIf
		endIf
	EndEvent
	
	Event OnHighlightST()

		SetInfoText("$CellScanner_Setting_EXCL_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State ExcludedCellState
	Event OnOptionSelect(Int val)
		string a_message
		
		Int Index = ExcPos.Find(val)
		if (Index != -1)
			if ExcRef[Index] == 0
				If (Completionist_ExcludedCells.GetAt(Index) as Cell)
					if (ShowMessage("$CellScanner_Setting_EXCL_Message2{" + (Completionist_ExcludedCells.GetAt(Index) as Cell).GetName() + "}", true, "$ConfirmYes", "$ConfirmNo"))
						Completionist_ExcludedCells.RemoveAddedForm(Completionist_ExcludedCells.GetAt(Index))
						ForcePageReset()
						return
					endIf
				endIf
			endIf

			if ExcRef[Index] == 1			
				If (ExcForms[Index] as ObjectReference)					
					if (ShowMessage("$CellScanner_Setting_EXRF_Message2", true, "$ConfirmYes", "$ConfirmNo"))
						RemoveExcludedReference(ExcForms[Index] as ObjectReference)
						ForcePageReset()
						return
					endIf
				endIf
			endIf		 
		endIf
	endEvent
	
	Event OnOptionHighlight(Int val)
		Int Index = ExcPos.Find(val)
		if (Index != -1)
			if ExcRef[Index] == 0
				SetInfoText("$CellScanner_Setting_EXCL_Highlight")
			endIf

			if ExcRef[Index] == 1
				SetInfoText("$CellScanner_Setting_EXRF_Highlight")
			endIf
		endIf
	endEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

state CellScanner_Setting_EXRF

	Event OnSelectST()
		ObjectReference curRef = Completionist_ItemFinder_Item.GetReference() as ObjectReference
		
		String a_message
		If (curRef.GetBaseObject() as Container)
			a_message = "$CellScanner_Setting_EXRF_Message3"
		Elseif (curRef as Actor)
			a_message = "$CellScanner_Setting_EXRF_Message4{" + curRef.GetDisplayName() + "}{" + curRef.GetDisplayName() + "}{" + curRef.GetDisplayName() + "}"
		Else
			a_message = "$CellScanner_Setting_EXRF_Message1{" + curRef.GetDisplayName() + "}"
		endIf 
					
		if (ShowMessage(a_message, true, "$ConfirmYes", "$ConfirmNo"))
			ExcludeReference(curRef, curRef.GetParentCell())
			Completionist_ItemFinder_Item.Clear()
			Completionist_ItemFinder_Name.Clear()
			Completionist_ItemFinder_Type.Clear()
			Completionist_ItemFinder_Quest.SetObjectiveDisplayed(10, False, True)
			ForcePageReset()
		endIf
	EndEvent
	
	Event OnHighlightST()
		SetInfoText("$CellScanner_Setting_EXRF_Info")
	EndEvent
endState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State CheckForReferences_State1

	Event OnKeyMapChangeST(Int KeyCode, String ConflictControl, String ConflictName)

		if (ConflictName != "")
			if (ShowMessage("$CheckForReferences_Msg2{" + ConflictControl + "}{" + conflictName + "}", true, "$ConfirmYes", "$ConfirmNo"))
				UnregisterForKey(ReferenceKey)
				ReferenceKey = KeyCode
				RegisterForKey(ReferenceKey)
				
				SetKeyMapOptionValueST(ReferenceKey)
				return
			endIf
			
			return
		endIf

		if (ConflictControl != "")
			if (ShowMessage("$CheckForReferences_Msg1{" + ConflictControl + "}", true, "$ConfirmYes", "$ConfirmNo"))
				UnregisterForKey(ReferenceKey)
				ReferenceKey = KeyCode
				RegisterForKey(ReferenceKey)
				
				SetKeyMapOptionValueST(ReferenceKey)
				return
			endIf
			
			return
		endIf


		UnregisterForKey(ReferenceKey)
		ReferenceKey = KeyCode
		RegisterForKey(ReferenceKey)
		
		SetKeyMapOptionValueST(ReferenceKey)
	EndEvent
	
	Event OnHighlightST()
		SetInfoText("$CheckForReferences_Info1")
	EndEvent
EndState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State CheckForReferences_State2

	Event OnKeyMapChangeST(Int KeyCode, String ConflictControl, String ConflictName)

		if (ConflictName != "")
			if (ShowMessage("$CheckForReferences_Msg2{" + ConflictControl + "}{" + conflictName + "}", true, "$ConfirmYes", "$ConfirmNo"))
				UnregisterForKey(ExcludeKey)
				ExcludeKey = KeyCode
				RegisterForKey(ExcludeKey)
				
				SetKeyMapOptionValueST(ExcludeKey)
				return
			endIf
			
			return
		endIf

		if (ConflictControl != "")
			if (ShowMessage("$CheckForReferences_Msg1{" + ConflictControl + "}", true, "$ConfirmYes", "$ConfirmNo"))
				UnregisterForKey(ExcludeKey)
				ExcludeKey = KeyCode
				RegisterForKey(ExcludeKey)
				
				SetKeyMapOptionValueST(ExcludeKey)
				return
			endIf
			
			return
		endIf


		UnregisterForKey(ExcludeKey)
		ExcludeKey = KeyCode
		RegisterForKey(ExcludeKey)
		
		SetKeyMapOptionValueST(ExcludeKey)
	EndEvent
	
	Event OnHighlightST()
		SetInfoText("$CheckForReferences_Info2")
	EndEvent
EndState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State CheckForReferences_State3

	Event OnKeyMapChangeST(Int KeyCode, String ConflictControl, String ConflictName)

		if (ConflictName != "")
			if (ShowMessage("$CheckForReferences_Msg2{" + ConflictControl + "}{" + conflictName + "}", true, "$ConfirmYes", "$ConfirmNo"))
				UnregisterForKey(ExcludeReferenceKey)
				ExcludeReferenceKey = KeyCode
				RegisterForKey(ExcludeReferenceKey)
				
				SetKeyMapOptionValueST(ExcludeReferenceKey)
				return
			endIf
			
			return
		endIf

		if (ConflictControl != "")
			if (ShowMessage("$CheckForReferences_Msg1{" + ConflictControl + "}", true, "$ConfirmYes", "$ConfirmNo"))
				UnregisterForKey(ExcludeReferenceKey)
				ExcludeReferenceKey = KeyCode
				RegisterForKey(ExcludeReferenceKey)
				
				SetKeyMapOptionValueST(ExcludeReferenceKey)
				return
			endIf
			
			return
		endIf


		UnregisterForKey(ExcludeReferenceKey)
		ExcludeReferenceKey = KeyCode
		RegisterForKey(ExcludeReferenceKey)
		
		SetKeyMapOptionValueST(ExcludeReferenceKey)
	EndEvent
	
	Event OnHighlightST()
		SetInfoText("$CheckForReferences_Info3")
	EndEvent
EndState

;---------------------------------------------------
;-- States -----------------------------------------
;---------------------------------------------------

State Quest_TrackingState
	Event OnOptionSelect(Int val)
		
		String quest_keys = Get_OptionKeys(val)
		String quest_name = Get_OptionName(val)
		if (quest_keys == "")
			return
		endIf	
	
		Int status = qIsOptionCompleted(CompQst.CurrentQuestID, quest_keys)

		if (status == -1)
			ShowMessage("Unable to find entry in array", false, "Ok")
		endIf 

		if (status == 0)
			if ShowMessage("$ConfirmCompleteQ{" + quest_name + "}", True, "$ConfirmY", "$ConfirmN")
				qSetOptionCompleted(CompQst.CurrentQuestID, quest_keys, true)
				Jump("$ProcessingTitle", 2)
			endIf
		endIf 

		if (status == 1)
			if ShowMessage("$RemoveCompletedQ{" + quest_name + "}", True, "$ConfirmY", "$ConfirmN")
				qSetOptionCompleted(CompQst.CurrentQuestID, quest_keys, false)
				Jump("$ProcessingTitle", 2)
			endIf
		endIf 

		if (status == 2)
			ShowMessage("$DebugNoMove", false, "$ConfirmY")
		endIf		
	endEvent
	
	Event OnOptionHighlight(Int val)
		if (bDebug)
			SetInfoText(Get_OptionDebug(val))
			return
		endIf
		SetInfoText(Get_OptionText(val))
	endEvent
endState
