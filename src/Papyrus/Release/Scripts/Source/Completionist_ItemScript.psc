Scriptname Completionist_ItemScript extends ReferenceAlias

ReferenceAlias Property name Auto
ReferenceAlias Property type Auto

Event OnActivate(ObjectReference akActionRef)
	
	if (akActionRef)
		ClearReferences(false)
	endif
EndEvent

Event OnCellDetach()
	ClearReferences(true)
EndEvent

Function ClearReferences(bool abFailed)

	if (abFailed)
		GetOwningQuest().SetObjectiveFailed(10, true)
	else
		GetOwningQuest().SetObjectiveCompleted(10, true)
	endif
	
	GetOwningQuest().SetObjectiveDisplayed(10, False, True)

	Clear()
	name.Clear()
	type.Clear()
EndFunction
