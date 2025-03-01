;BEGIN FRAGMENT CODE - Do not edit anything between this and the end comment
;NEXT FRAGMENT INDEX 2
Scriptname PRKF_Completionist_MerchantH_05005995 Extends Perk Hidden

Import Completionist_Native

;BEGIN FRAGMENT Fragment_0
Function Fragment_0(ObjectReference akTargetRef, Actor akActor)
;BEGIN CODE
if (!akTargetRef)
	return
endif

if (akTargetRef.IsInDialogueWithPlayer())

	RegisterMerchant(akTargetRef as Actor)

	While akTargetRef.IsInDialogueWithPlayer()
		Utility.Wait(3)
	endwhile
	
	UnRegisterMerchant(akTargetRef as Actor)
endif
;END CODE
EndFunction
;END FRAGMENT

;END FRAGMENT CODE - Do not edit anything between this and the begin comment
