Scriptname Completionist_LoadScript extends ReferenceAlias  

Completionist_MCMScript Property CompMCM Auto
Completionist_APIScript Property CompAPI Auto
Completionist_ItemScript Property CompITM Auto
Completionist_PinnedItemScript Property CompPIN Auto
GlobalVariable Property Completionist_Busy Auto

;---------------------------------------------------
;-- Events -----------------------------------------
;---------------------------------------------------

Event OnPlayerLoadGame()
	
	Completionist_Busy.SetValue(1)
	CompMCM = (Quest.GetQuest("Completionist_Main") as Completionist_MCMScript)
	CompITM = (Quest.GetQuest("Completionist_ItemFinder").GetAliasByName("Item") as Completionist_ItemScript)
	CompPIN = (Quest.GetQuest("Completionist_PinnedItem").GetAliasByName("Item") as Completionist_PinnedItemScript)
	CompAPI.RegisterForSingleUpdate(0.1)
	AddInventoryEventFilter(None)
endEvent

;---------------------------------------------------
;-- Events -----------------------------------------
;---------------------------------------------------

Event OnItemAdded(Form akBaseItem, int aiItemCount, ObjectReference akItemReference, ObjectReference akSourceContainer)
	if (CompMCM.TargetForm != none && akBaseItem == CompMCM.TargetForm)
		CompITM.ClearReferences(false)
		CompMCM.ClearQuestTarget()
	endif

	if (CompMCM.PinnedForm != none && akBaseItem == CompMCM.PinnedForm)
		CompPIN.ClearReferences(false)
		CompMCM.ClearPinnedTarget(true)
	endif
EndEvent

;---------------------------------------------------
;-- END OF CODE ------------------------------------
;---------------------------------------------------