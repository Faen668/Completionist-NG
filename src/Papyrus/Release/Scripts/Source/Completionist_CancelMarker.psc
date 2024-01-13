ScriptName Completionist_CancelMarker extends ObjectReference

Completionist_MCMScript Property CompMCM Auto
Completionist_MCMScript2 Property CompMCM2 Auto
Quest Property Completionist_Marker Auto

Event OnUpdate()
	
	if (Game.GetPlayer().GetDistance(Self) <= CompMCM.State_MarkerDetectionVal)
		Self.MoveToMyEditorLocation()
		Completionist_Marker.SetObjectiveCompleted(10, True)
		Completionist_Marker.SetObjectiveDisplayed(10, False, True)	
		CompMCM2.CurMarker = ""
		CompMCM2.CurLocation = None		
	else
		RegisterForSingleUpdate(0.1)
	endif
endEvent

