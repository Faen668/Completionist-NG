Scriptname Completionist_MerchantScript extends ReferenceAlias  
Import Completionist_Native

Event OnActivate(ObjectReference akRef)
	
	if (!akRef)
		return
	endif
	
	if (GetReference().IsInDialogueWithPlayer())
	
		RegisterMerchant(GetReference() as Actor)

		While GetReference().IsInDialogueWithPlayer()
			Utility.Wait(3)
		endwhile
		
		UnRegisterMerchant(GetReference() as Actor)
	endif
endEvent

;Add NPC to ChopWood Faction: addtofaction 000BF210 0