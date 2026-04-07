ScriptName Completionist_PetListener Extends Quest

Import Completionist_Native

ReferenceAlias Property _Critter Auto
Actor AdoptedCritter

Event OnInit()
	
	RegisterForSingleUpdate(3)
endEvent

Event OnUpdate()
	
	if (_Critter.GetActorReference() && _Critter.GetActorReference() != AdoptedCritter)
		AdoptedCritter = _Critter.GetActorReference()
		Framework_UpdatePetOwnership(AdoptedCritter)
	endif
	
	RegisterForSingleUpdate(3)
endEvent
