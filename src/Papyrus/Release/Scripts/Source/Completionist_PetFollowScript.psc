scriptname Completionist_PetFollowScript extends referencealias

Import Completionist_Native

Bool Property bAdopted Auto Hidden
Faction Property PetFramework_PetFollowingFaction Auto

Event OnActivate(ObjectReference akRef)
	
	RegisterForSingleUpdate(0.1)
endEvent

Event OnUpdate()

	if (!bAdopted && GetActorReference().GetFactionRank(PetFramework_PetFollowingFaction) == 1)
		Framework_UpdatePetOwnership(GetActorReference().GetActorBase().GetName())
		UnregisterForUpdate()
		bAdopted = True
		return;
	endif
	
	if (Game.GetPlayer().GetDistance(GetActorReference()) > 1000)
		UnregisterForUpdate()
		return
	endif
	
	RegisterForSingleUpdate(0.1)
endEvent
