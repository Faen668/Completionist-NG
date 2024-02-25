Scriptname Completionist_PinnedItemScript extends ReferenceAlias

ReferenceAlias Property name Auto
ReferenceAlias Property type Auto
EffectShader Property Completionist_PinnedItemShader Auto
Formlist Property Completionist_PinnedItemSoundList Auto

;---------------------------------------------------
;-- Function ---------------------------------------
;---------------------------------------------------

Event OnActivate(ObjectReference akActionRef)
	
	if (akActionRef)
		;ClearReferences(false)
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
	RemoveAllInventoryEventFilters()
	
	GetOwningQuest().SetObjectiveDisplayed(10, False, True)
    if (GetReference() && GetReference().Is3DLoaded())
		Completionist_PinnedItemShader.Stop(GetReference())
    endIf
	
	Clear()
	name.Clear()
	type.Clear()
EndFunction

;---------------------------------------------------
;-- Function ---------------------------------------
;---------------------------------------------------

Bool Function IsFilled()
	return (GetReference() && GetReference().Is3DLoaded())
EndFunction

;---------------------------------------------------
;-- Function ---------------------------------------
;---------------------------------------------------

Function SetDisplayparamters(form a_form, Bool b_playEffect, Int i_soundChoice)
	RemoveAllInventoryEventFilters()
	
	if (a_form)
		AddInventoryEventFilter(a_form)
		StartGlow(b_playEffect)
		playSound(i_soundChoice)
    endIf	
EndFunction

;---------------------------------------------------
;-- Function ---------------------------------------
;---------------------------------------------------

Function StartGlow(Bool b_playEffect)
    if (b_playEffect && IsFilled())
		Completionist_PinnedItemShader.Play(GetReference())
    endIf	
EndFunction

;---------------------------------------------------
;-- Function ---------------------------------------
;---------------------------------------------------

Function StopGlow()
    if (IsFilled())
		Completionist_PinnedItemShader.Stop(GetReference())
    endIf	
EndFunction

;---------------------------------------------------
;-- Function ---------------------------------------
;---------------------------------------------------

Function UpdateGlowParameters(Bool b_playEffect)
    if (IsFilled())
		if (b_playEffect)
			Completionist_PinnedItemShader.Play(GetReference())
		else
			Completionist_PinnedItemShader.Stop(GetReference())
		endif
    endIf	
EndFunction

;---------------------------------------------------
;-- Function ---------------------------------------
;---------------------------------------------------

Function playSound(Int i_soundChoice)
    if (i_soundChoice > 0 && IsFilled())
		((Completionist_PinnedItemSoundList.GetAt(i_soundChoice)) as sound).PlayAndWait(Game.GetPlayer())
    endIf	
EndFunction

