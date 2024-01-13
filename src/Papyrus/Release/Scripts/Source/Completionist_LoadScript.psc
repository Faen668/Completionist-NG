Scriptname Completionist_LoadScript extends ReferenceAlias  

Completionist_APIScript Property CompAPI Auto
GlobalVariable Property Completionist_Busy Auto

;---------------------------------------------------
;-- Events -----------------------------------------
;---------------------------------------------------

Event OnPlayerLoadGame()
	
	Completionist_Busy.SetValue(1)
	CompAPI.RegisterForSingleUpdate(0.1)
endEvent

;---------------------------------------------------
;-- END OF CODE ------------------------------------
;---------------------------------------------------