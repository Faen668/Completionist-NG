;BEGIN FRAGMENT CODE - Do not edit anything between this and the end comment
;NEXT FRAGMENT INDEX 5
Scriptname PRKF_Completionist_ShrineWatcher Extends Perk Hidden

Import Completionist_Native

;BEGIN FRAGMENT Fragment_3
Function Fragment_3(ObjectReference akTargetRef, Actor akActor)
;BEGIN CODE
if (Survival_ModeEnabledShared.GetValue())
	ActivateShrineByID(akTargetRef.GetBaseObject())
endif
;END CODE
EndFunction
;END FRAGMENT

;END FRAGMENT CODE - Do not edit anything between this and the begin comment

GlobalVariable Property Survival_ModeEnabledShared Auto

