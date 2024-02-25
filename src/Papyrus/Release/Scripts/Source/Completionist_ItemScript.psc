Scriptname Completionist_ItemScript extends ReferenceAlias

ReferenceAlias Property name Auto
ReferenceAlias Property type Auto

;---------------------------------------------------
;-- Function ---------------------------------------
;---------------------------------------------------

Event OnActivate(ObjectReference akActionRef)
	
	if (akActionRef)
		ClearReferences(false)
	endif
EndEvent

;---------------------------------------------------
;-- Function ---------------------------------------
;---------------------------------------------------

Event OnCellDetach()
	ClearReferences(true)
EndEvent

;---------------------------------------------------
;-- Function ---------------------------------------
;---------------------------------------------------

Function ClearReferences(bool abFailed)
	
	GetOwningQuest().SetObjectiveDisplayed(10, False, True)
	Clear()
	name.Clear()
	type.Clear()
EndFunction
