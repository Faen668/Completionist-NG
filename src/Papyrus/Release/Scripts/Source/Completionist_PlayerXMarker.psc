Scriptname Completionist_PlayerXMarker extends ObjectReference  
Import Completionist_Native

Completionist_MCMScript Property CompMCM Auto

ObjectReference Property PlayerRef Auto
Formlist Property Completionist_ExcludedCells Auto
	
Event OnCellDetach()
	Utility.Wait(0.1) ;maybe not necessary
	MoveTo(playerRef)
	
	if (Completionist_ExcludedCells.HasForm(PlayerRef.GetParentCell()))
		return;
	endif
	
	Utility.Wait(5)
	Completionist_Native.CheckForReferences(PlayerRef.GetParentCell(), CompMCM.bDebug)
EndEvent
