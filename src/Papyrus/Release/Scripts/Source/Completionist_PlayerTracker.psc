Scriptname Completionist_PlayerTracker extends activemagiceffect  

Import Completionist_Native

ObjectReference Property PlayerRef Auto
ObjectReference property Completionist_PlayerXMarkera auto
 
Event OnEffectStart(Actor akTarget, Actor akCaster)
    Utility.Wait(0.1) ; Required.
    Completionist_PlayerXMarkera.MoveTo(PlayerRef)
EndEvent
