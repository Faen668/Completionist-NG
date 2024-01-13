ScriptName Completionist_PetListener Extends Quest

Import Completionist_Native

ReferenceAlias Property _Critter Auto
ActorBase AdoptedCritter

Event OnInit()
	
	RegisterForSingleUpdate(3)
endEvent

Event OnUpdate()
	
	if (_Critter.GetActorReference() && _Critter.GetActorReference().GetActorBase() != AdoptedCritter)
		AdoptedCritter = _Critter.GetActorReference().GetActorBase()
		Framework_UpdatePetOwnership(AdoptedCritter.GetName())
	endif
	
	RegisterForSingleUpdate(3)
endEvent
