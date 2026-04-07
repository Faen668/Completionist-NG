
scriptName ccBGSSSE001_FishingSystemScript extends Quest
{The system that handles all fishing gameplay mechanics.
  Interacts with Fishing Supplies (ccBGSSSE001_FishingActScript), which maintain some local state.}

Import Completionist_Native
Import TrueDirectionalMovement

;-- Properties --------------------------------------
Float property RUMBLE_STRENGTH_HOOKED_RIGHTCONSTANT
	Float function get()

		return 0.000000
	endFunction
endproperty
Float property DURATION_SUCCESSVIEW
	Float function get()

		return 1.80000
	endFunction
endproperty
globalvariable property ccBGSSSE001_CatchTypeSmallFish auto
String property LINETUG_FISH_ANIM
	String function get()

		return "BiteFish"
	endFunction
endproperty
String property CATCH_FAILURE_ANIM
	String function get()

		return "CatchFail"
	endFunction
endproperty
message property ccBGSSSE001_CatchPrompt auto
Int property RARITY_LIST_UNCOMMON_INDEX
	Int function get()

		return 1
	endFunction
endproperty
Int property SYSTEMSTATE_NIBBLE
	Int function get()

		return 3
	endFunction
endproperty
Float property MORNING_EVENING_COMMONFISH_THRESHOLD_ADJUST
	Float function get()

		return 0.100000
	endFunction
endproperty
formlist property ccBGSSSE001_FishCatchDataListArctic auto
{The arctic fish list to draw from.}
Float property RUMBLE_DURATION_SUCCESS
	Float function get()

		return 0.400000
	endFunction
endproperty
Float property RUMBLE_STRENGTH_CAST_LEFT
	Float function get()

		return 0.0900000
	endFunction
endproperty
Float property RUMBLE_STRENGTH_HOOKEDSMALLFISH_LEFT
	Float function get()

		return 1.00000
	endFunction
endproperty
Float property DURATION_JUNKITEMCATCHTIME
	Float function get()

		return 3.50000
	endFunction
endproperty
Int property RAIN_BONUS_MIN
	Int function get()

		return 1
	endFunction
endproperty
Int property POPULATION_COUNT_FULL
	Int function get()

		return 4
	endFunction
endproperty
Float property DURATION_SHOWPOPULATION
	Float function get()

		return 3.00000
	endFunction
endproperty
Int property RODTYPE_DWARVEN
	Int function get()

		return 3
	endFunction
endproperty
String property POPULATION_EMPTY_ANIM
	String function get()

		return "IdleEmpty"
	endFunction
endproperty
Float property RUMBLE_STRENGTH_HOOKEDLARGEFISH_LEFT
	Float function get()

		return 0.200000
	endFunction
endproperty
formlist property ccBGSSSE001_FishingRodFXActivators auto
{A formlist of all fishing rod FX activators.}
Float property DURATION_FADETOBLACKCROSSFADE
	Float function get()

		return 0.300000
	endFunction
endproperty
String property EXIT_ANIM
	String function get()

		return "Exit"
	endFunction
endproperty
formlist property ccBGSSSE001_FishCatchDataListTemperateLakeRain auto
{The temperate lake (rain weather) fish list to draw from.}
Float property RUMBLE_STRENGTH_HOOKEDLARGEFISH_LEFTCONSTANT
	Float function get()

		return 0.600000
	endFunction
endproperty
Float property RUMBLE_DURATION_HOOKEDCONSTANT
	Float function get()

		return 2.00000
	endFunction
endproperty
Int property SIZE_LIST_SMALLFISH_INDEX
	Int function get()

		return 0
	endFunction
endproperty
Float property RUMBLE_STRENGTH_FAILURE_RIGHT
	Float function get()

		return 0.000000
	endFunction
endproperty
Int property RODTYPE_NONE
	Int function get()

		return -1
	endFunction
endproperty
objectreference property ReelLineRef auto
{A reference to ccBGSSSE001ReelLineAct in aaaMarkers cell. Trigger volume for player activation.}
formlist property ccBGSSSE001_FishingRods auto
{A formlist of all fishing rod weapons.}
Float property LINETUG_TYPE_TUGFISH
	Float function get()

		return 3.00000
	endFunction
endproperty
Int property SYSTEMSTATE_CATCH_RESOLVE
	Int function get()

		return 5
	endFunction
endproperty
String property CAST_ANIM
	String function get()

		return "Cast"
	endFunction
endproperty
Float property RUMBLE_STRENGTH_FAILURE_LEFT
	Float function get()

		return 1.00000
	endFunction
endproperty
String property POPULATION_FULL_ANIM
	String function get()

		return "IdleFull"
	endFunction
endproperty
actor property PlayerRef auto
sound property ccBGSSSE001_CatchSuccessSM auto
{Catch success sound.}
Int property BIOME_TYPE_ARCTIC
	Int function get()

		return 2
	endFunction
endproperty
Int property UPDATETYPE_SEQUENCE
	Int function get()

		return 2
	endFunction
endproperty
Float property BASE_CATCH_THRESHOLD_COMMONFISH
	Float function get()

		return 0.250000
	endFunction
endproperty
Float property DURATION_EXIT
	Float function get()

		return 1.50000
	endFunction
endproperty
formlist property ccBGSSSE001_FishCatchDataListTemperateStreamRain auto
{The temperate stream (rain weather) fish list to draw from.}
ccbgssse001_dialoguedetectscript property DialogueQuest auto
{The dialogue detection system quest.}
String property POPULATION_ANIMVAR
	String function get()

		return "iFishPopulation"
	endFunction
endproperty
Float property SPECIAL_JUNK_RODS_UNCOMMONJUNK_BONUS
	Float function get()

		return 0.0700000
	endFunction
endproperty
Float property DURATION_RODLOADTIME
	Float function get()

		return 0.150000
	endFunction
endproperty
formlist property ccBGSSSE001_FishCatchDataListTemperateLakeClear auto
{The temperate lake (clear weather) fish list to draw from.}
Int property SYSTEMSTATE_HOOKED
	Int function get()

		return 4
	endFunction
endproperty
Quest property ccBGSSSE001_FishingFollowerIdleQuest auto
{The follower idle quest to help keep followers out of the way when the player is fishing.}
light property Torch01 auto
String property CATCH_SUCCESS_ANIM
	String function get()

		return "CatchSuccess"
	endFunction
endproperty
formlist property ccBGSSSE001_JunkCatchDataListDefault auto
{The junk list to draw from.}
Float property RUMBLE_STRENGTH_HOOKEDLARGEFISH_RIGHT
	Float function get()

		return 0.650000
	endFunction
endproperty
Float property RUMBLE_STRENGTH_SUCCESSOBJECT_RIGHT
	Float function get()

		return 0.100000
	endFunction
endproperty
message property ccBGSSSE001_FishingTutorial auto
Float property BASE_CATCH_THRESHOLD_UNCOMMONFISH
	Float function get()

		return 0.0300000
	endFunction
endproperty
Float property RUMBLE_STRENGTH_HOOKEDLARGEFISH_RIGHTCONSTANT
	Float function get()

		return 0.100000
	endFunction
endproperty
Float property GAMETIME_LATEEVENING
	Float function get()

		return 21.0000
	endFunction
endproperty
Float property DURATION_NIBBLE
	Float function get()

		return 0.500000
	endFunction
endproperty
Float property DURATION_CATCHTIMEOUT
	Float function get()

		return 5.00000
	endFunction
endproperty
keyword property ccBGSSSE001_SummonsRain auto
{The equipment keyword that will cause rainstorms when fishing if wearing that equipment.}
String property POPULATION_SPARSE_ANIM
	String function get()

		return "IdleSparse"
	endFunction
endproperty
Int property BASE_POPULATION
	Int function get()

		return 4
	endFunction
endproperty
Int property RARITY_LIST_RARE_INDEX
	Int function get()

		return 2
	endFunction
endproperty
Int property RARITY_LIST_COMMON_INDEX
	Int function get()

		return 0
	endFunction
endproperty
message property ccBGSSSE001_ErrorNoFishMounted auto
Float property RUMBLE_STRENGTH_HOOKEDSMALLFISH_RIGHT
	Float function get()

		return 0.0500000
	endFunction
endproperty
Int property SYSTEMSTATE_FISHING
	Int function get()

		return 2
	endFunction
endproperty
Float property DURATION_INITIAL_WAITING_PERIOD
	Float function get()

		return 5.00000
	endFunction
endproperty
globalvariable property GameHour auto
Int property RODTYPE_ARGONIAN
	Int function get()

		return 2
	endFunction
endproperty
Float property RUMBLE_DURATION_NIBBLE
	Float function get()

		return 0.450000
	endFunction
endproperty
Float property RUMBLE_STRENGTH_NIBBLELARGE_LEFT
	Float function get()

		return 0.000000
	endFunction
endproperty
Float property SPECIAL_FISH_RODS_COMMONFISH_THRESHOLD_ADJUST
	Float function get()

		return 0.200000
	endFunction
endproperty
String property FASTEXIT_ANIM
	String function get()

		return "FastExit"
	endFunction
endproperty
Float property RUMBLE_DURATION_HOOKED
	Float function get()

		return 0.450000
	endFunction
endproperty
message property ccBGSSSE001_ErrorNoFishCombat auto
weather property SkyrimStormRain auto
{The weather to force to when wearing equipment that summons rain.}
imagespacemodifier property ccBGSSSE001_FadeToBlackBackImod auto
Float property RUMBLE_STRENGTH_HOOKEDOBJECT_RIGHT
	Float function get()

		return 0.000000
	endFunction
endproperty
formlist property ccBGSSSE001_OneTimeCaughtList auto
{FormList of Catch Data that are flagged as one-time-only and have already been caught.}
formlist property ccBGSSSE001_FishCatchDataListTemperateStreamClear auto
{The temperate stream (clear weather) fish list to draw from.}
message property ccBGSSSE001_fishingEarlyReel auto
Int property UPDATETYPE_SETQUESTSTAGE
	Int function get()

		return 4
	endFunction
endproperty
String property RESET_ANIM
	String function get()

		return "Reset"
	endFunction
endproperty
objectreference property ccBGSSSE001_NavBlockerRef auto
{A reference to ccBGSSSE001_NavBlockerRef in aaaMarkers cell. Collision navcut volume to help prevent NPCs pushing the player.}
referencealias property DogAlias auto
{The dog alias from the idle quest.}
globalvariable property ccBGSSSE001_HasCaughtFishAtLeastOnce auto
{The global that tracks whether or not the player has caught a fish at least once.}
Quest property ccBGSSSE001_Start_MQ2 auto
{The quest to start after the player has caught a fish at least once.}
static property XMarker auto
ccbgssse001_movedetectscript property MoveDetectRef auto
{A reference to ccBGSSSE001MoveDetectAct in aaaMarkers cell. Trigger volume for detecting player movement.}
String property LINETUG_OBJECT_ANIM
	String function get()

		return "BiteObject"
	endFunction
endproperty
Int property BIOME_TYPE_STREAM
	Int function get()

		return 0
	endFunction
endproperty
imagespacemodifier property ccBGSSSE001_FadeToBlackImod auto
Float property GAMETIME_LATEMORNING
	Float function get()

		return 9.00000
	endFunction
endproperty
message property ccBGSSSE001_ErrorNoFishSitting auto
Int property MORNINGEVENING_BONUS_MAX
	Int function get()

		return 3
	endFunction
endproperty
Float property GAMETIME_MORNING
	Float function get()

		return 6.00000
	endFunction
endproperty
Float property RUMBLE_DURATION_FAILURE
	Float function get()

		return 0.400000
	endFunction
endproperty
Float property DURATION_CAST
	Float function get()

		return 3.00000
	endFunction
endproperty
Float property DURATION_SHEATHEWEAPON
	Float function get()

		return 1.10000
	endFunction
endproperty
message property ccBGSSSE001_ErrorRodRequired auto
sound property ccBGSSSE001_RareCatchSM auto
{Rare catch success sound.}
sound property ccBGSSSE001_ITMFishUpSM auto
{Item fanfare sound.}
message property ccBGSSSE001_fishingLostCatch auto
Float property RUMBLE_STRENGTH_SUCCESSLARGEFISH_LEFT
	Float function get()

		return 0.100000
	endFunction
endproperty
message property ccBGSSSE001_fishingHooked auto
Int property BASE_BONUS_MAX
	Int function get()

		return 2
	endFunction
endproperty
Float property RUMBLE_DURATION_CAST
	Float function get()

		return 0.600000
	endFunction
endproperty
message property ccBGSSSE001_ReelLinePrompt auto
Int property RODTYPE_STANDARD
	Int function get()

		return 0
	endFunction
endproperty
message property ccBGSSSE001_ErrorNoFishJumping auto
globalvariable property ccBGSSSE001_FishingDebugEnabled auto
globalvariable property ccBGSSSE001_FishingTutorialDisplayed auto
globalvariable property ccBGSSSE001_ShowedCatchPromptThisSession auto
Int property BASE_BONUS_MIN
	Int function get()

		return 1
	endFunction
endproperty
Float property RUMBLE_STRENGTH_NIBBLESMALL_LEFT
	Float function get()

		return 0.500000
	endFunction
endproperty
globalvariable property ccBGSSSE001_ShowedReelPromptThisSession auto
light property ccBGSSSE001_CatchSuccessLight auto
{The light to display when an object is caught.}
Float property GAMETIME_EVENING
	Float function get()

		return 18.0000
	endFunction
endproperty
formlist property ccBGSSSE001_FishCatchDataListCave auto
{The cave fish list to draw from.}
Float property LINETUG_TYPE_NIBBLE
	Float function get()

		return 2.00000
	endFunction
endproperty
Int property SYSTEMSTATE_CLEANUP
	Int function get()

		return 6
	endFunction
endproperty
imagespacemodifier property ccBGSSSE001_FadeToBlackHoldImod auto
Float property RUMBLE_STRENGTH_CAST_RIGHT
	Float function get()

		return 0.000000
	endFunction
endproperty
Float property DURATION_FASTEXIT
	Float function get()

		return 0.500000
	endFunction
endproperty
Int property UPDATETYPE_CATCHTIMEOUT
	Int function get()

		return 3
	endFunction
endproperty
globalvariable property ccBGSSSE001_CatchTypeObject auto
Float property SPECIAL_JUNK_RODS_COMMONJUNK_BONUS
	Float function get()

		return 0.200000
	endFunction
endproperty
referencealias property FollowerAlias auto
{The follower alias from the idle quest.}
Float property BASE_CATCH_THRESHOLD_COMMONJUNK
	Float function get()

		return 0.350000
	endFunction
endproperty
Int property UPDATETYPE_START
	Int function get()

		return 1
	endFunction
endproperty
Float property RUMBLE_STRENGTH_HOOKEDOBJECT_LEFT
	Float function get()

		return 0.500000
	endFunction
endproperty
Float property LINETUG_TYPE_TUGOBJECT
	Float function get()

		return 4.00000
	endFunction
endproperty
Float property RUMBLE_STRENGTH_SUCCESSOBJECT_LEFT
	Float function get()

		return 0.600000
	endFunction
endproperty
Int property BIOME_TYPE_LAKE
	Int function get()

		return 1
	endFunction
endproperty
Int property RODTYPE_ALIKRI
	Int function get()

		return 1
	endFunction
endproperty
Float property POPULATION_TYPE_FULL
	Float function get()

		return 3.00000
	endFunction
endproperty
Float property SPECIAL_FISH_RODS_UNCOMMONFISH_THRESHOLD_ADJUST
	Float function get()

		return 0.0700000
	endFunction
endproperty
Float property DURATION_HOOKED_ANIM_WAIT
	Float function get()

		return 0.350000
	endFunction
endproperty
Float property DURATION_CATCH
	Float function get()

		return 1.00000
	endFunction
endproperty
Int property SIZE_LIST_LARGEFISH_INDEX
	Int function get()

		return 1
	endFunction
endproperty
Float property DURATION_INITIAL_WAITING_PERIOD_VARIANCE
	Float function get()

		return 2.00000
	endFunction
endproperty
Float property RUMBLE_STRENGTH_SUCCESSSMALLFISH_LEFT
	Float function get()

		return 1.00000
	endFunction
endproperty
Float property RUMBLE_STRENGTH_SUCCESSSMALLFISH_RIGHT
	Float function get()

		return 0.0800000
	endFunction
endproperty
Float property RUMBLE_STRENGTH_SUCCESSLARGEFISH_RIGHT
	Float function get()

		return 1.00000
	endFunction
endproperty
message property ccBGSSSE001_fishingEarlyReelNibble auto
Int property BIOME_TYPE_CAVE
	Int function get()

		return 3
	endFunction
endproperty
Float property SPECIAL_JUNK_RODS_UNCOMMONJUNK_THRESHOLD_ADJUST
	Float function get()

		return 0.0600000
	endFunction
endproperty
Float property BASE_CATCH_THRESHOLD_UNCOMMONJUNK
	Float function get()

		return 0.0300000
	endFunction
endproperty
Int property MORNINGEVENING_BONUS_MIN
	Int function get()

		return 2
	endFunction
endproperty
Float property RUMBLE_STRENGTH_NIBBLELARGE_RIGHT
	Float function get()

		return 0.600000
	endFunction
endproperty
Float property POPULATION_TYPE_SPARSE
	Float function get()

		return 2.00000
	endFunction
endproperty
Int property SYSTEMSTATE_IDLE
	Int function get()

		return 0
	endFunction
endproperty
Int property RAIN_BONUS_MAX
	Int function get()

		return 3
	endFunction
endproperty
imagespacemodifier property ccBGSSSE001_CatchSuccessDOF auto
Float property RUMBLE_STRENGTH_NIBBLESMALL_RIGHT
	Float function get()

		return 0.000000
	endFunction
endproperty
Float property BASE_CATCH_THRESHOLD_IS_FISH
	Float function get()

		return 0.100000
	endFunction
endproperty
String property TIMEOUT_ANIM
	String function get()

		return "Timeout"
	endFunction
endproperty
Float property POPULATION_TYPE_EMPTY
	Float function get()

		return 1.00000
	endFunction
endproperty
Float property MORNING_EVENING_UNCOMMONFISH_THRESHOLD_ADJUST
	Float function get()

		return 0.0500000
	endFunction
endproperty
Float property SPECIAL_JUNK_RODS_COMMONJUNK_THRESHOLD_ADJUST
	Float function get()

		return 0.300000
	endFunction
endproperty
String property NIBBLE_ANIM
	String function get()

		return "Nibble"
	endFunction
endproperty
Float property LINETUG_TYPE_NONE
	Float function get()

		return 1.00000
	endFunction
endproperty
String property LINETUG_ANIMVAR
	String function get()

		return "iFishBite"
	endFunction
endproperty
Float property BASE_CATCH_THRESHOLD_SMALL
{50% chance of Small fish catch. Rods modify this by 25% up (Alik'ri) or down (Argonian, so, greater chance to catch Large fish).}
	Float function get()

		return 0.500000
	endFunction
endproperty
Int property SYSTEMSTATE_SETTINGUP
	Int function get()

		return 1
	endFunction
endproperty
globalvariable property ccBGSSSE001_CatchTypeLargeFish auto
Float property RUMBLE_STRENGTH_HOOKED_LEFTCONSTANT
	Float function get()

		return 0.600000
	endFunction
endproperty

;-- Variables ---------------------------------------
Int nextUpdateType = 0
Bool handlingInputOrUpdate = false
Bool isQuestItemCatch = false
ccBGSSSE001_CatchData nextCatchData
ccBGSSSE001_FishingActScript lastFishingSupplies
Bool lastCatchWasRare = false
objectreference fishingRodActivator
weather previousWeather
Bool startedInFirstPerson = false
Int currentCatchSequenceIndex = 0
ccBGSSSE001_FishingActScript currentFishingSupplies
Bool forcedRainWeather = false
Int currentSystemState = 0
ccBGSSSE001_RadiantFishEventListener RadiantFishEventListener
Bool debugEnabled = false
Float currentGameHour = 0.000000
formlist nextCatchDataSourceList
Int currentFishingRodType = -1
Bool startedWithTorch = false

;-- Functions ---------------------------------------

function ReturnSurroundingVolumes()

	objectreference returnRef = ReelLineRef.GetLinkedRef(none)
	ReelLineRef.MoveTo(returnRef, 0.000000, 0.000000, 0.000000, true)
	MoveDetectRef.MoveTo(returnRef, 0.000000, 0.000000, 0.000000, true)
	ccBGSSSE001_NavBlockerRef.MoveTo(returnRef, 0.000000, 0.000000, 0.000000, true)
endFunction

function SetVisualPopulation()

	Int currentPopulation = currentFishingSupplies.GetCurrentFishPopulation()
	self.FishingDebug("Current population: " + currentPopulation as String)
	if currentPopulation >= self.POPULATION_COUNT_FULL
		self.FishingDebug("Setting full population animation var")
		fishingRodActivator.SetAnimationVariableFloat(self.POPULATION_ANIMVAR, self.POPULATION_TYPE_FULL)
	elseIf currentPopulation > 0
		self.FishingDebug("Setting sparse population animation var")
		fishingRodActivator.SetAnimationVariableFloat(self.POPULATION_ANIMVAR, self.POPULATION_TYPE_SPARSE)
	else
		self.FishingDebug("Setting empty population animation var")
		fishingRodActivator.SetAnimationVariableFloat(self.POPULATION_ANIMVAR, self.POPULATION_TYPE_EMPTY)
	endIf
endFunction

function PlayResetAnimation()

	fishingRodActivator.PlayAnimation(self.RESET_ANIM)
endFunction

ccBGSSSE001_CatchData function GetNextJunkCatchData(formlist akCatchDataList)

	Int rarityListIndex
	Float rarityRoll = utility.RandomFloat(0.000000, 1.00000)
	self.FishingDebug("Rarity Roll: " + rarityRoll as String)
	Float catchThresholdCommonJunk = self.BASE_CATCH_THRESHOLD_COMMONJUNK
	Float catchThresholdUncommonJunk = self.BASE_CATCH_THRESHOLD_UNCOMMONJUNK
	if currentFishingRodType == self.RODTYPE_DWARVEN
		catchThresholdCommonJunk += self.SPECIAL_JUNK_RODS_COMMONJUNK_THRESHOLD_ADJUST
		catchThresholdUncommonJunk += self.SPECIAL_JUNK_RODS_UNCOMMONJUNK_THRESHOLD_ADJUST
	endIf
	if rarityRoll >= catchThresholdCommonJunk
		rarityListIndex = self.RARITY_LIST_COMMON_INDEX
	elseIf rarityRoll >= catchThresholdUncommonJunk
		rarityListIndex = self.RARITY_LIST_UNCOMMON_INDEX
	else
		rarityListIndex = self.RARITY_LIST_RARE_INDEX
	endIf
	formlist raritySubList = akCatchDataList.GetAt(rarityListIndex) as formlist
	Int raritySubListSize = raritySubList.GetSize()
	Int resultRoll = utility.RandomInt(0, raritySubListSize - 1)
	self.FishingDebug("Result Roll: " + resultRoll as String)
	ccBGSSSE001_CatchData catchData = raritySubList.GetAt(resultRoll) as ccBGSSSE001_CatchData
	self.FishingDebug("Catch Data: " + catchData as String)
	if rarityListIndex == self.RARITY_LIST_RARE_INDEX
		lastCatchWasRare = true
	endIf
	nextCatchDataSourceList = akCatchDataList
	return catchData
endFunction

Bool function GetInRain()

	weather theWeather
	if forcedRainWeather
		return true
	endIf
	if weather.GetCurrentWeatherTransition() >= 0.500000
		theWeather = weather.GetCurrentWeather()
	else
		theWeather = weather.GetOutgoingWeather()
	endIf
	return theWeather.GetClassification() == 2
endFunction

ccBGSSSE001_CatchData function FindAllowedCatchDataInList(formlist akCatchDataList)

	Int size = akCatchDataList.GetSize()
	Int i = 0
	while i < size
		ccBGSSSE001_CatchData catchData = akCatchDataList.GetAt(i) as ccBGSSSE001_CatchData
		if !catchData.isOneTimeCatch || catchData.isOneTimeCatch && ccBGSSSE001_OneTimeCaughtList.Find(catchData as form) == -1
			return catchData
		endIf
		i += 1
	endWhile
	self.FishingDebug("There were no allowed CatchData in list " + akCatchDataList as String + ", a non-exclusive result should be added!")
	return none
endFunction

function SetupCameraAndPosition(Bool abContinueFishing)

	startedInFirstPerson = PlayerRef.GetAnimationVariableBool("IsFirstPerson")
	startedWithTorch = PlayerRef.GetEquippedItemType(0) == 11
	while self.IsPlayerDrawingWeapon()
		utility.Wait(0.250000)
	endWhile
	Bool hasWeaponDrawn = PlayerRef.IsWeaponDrawn()
	game.DisablePlayerControls(true, true, false, false, true, true, false, true, 0)
	Bool resetViewAndPlayerState = true
	if abContinueFishing
		objectreference fishingmarker = currentFishingSupplies.GetFishingMarker()
		if PlayerRef.GetAngleX() == 0.000000 && PlayerRef.GetAngleZ() == fishingmarker.GetAngleZ() && math.Floor(PlayerRef.GetPositionX()) == math.Floor(fishingmarker.GetPositionX()) && math.Floor(PlayerRef.GetPositionY()) == math.Floor(fishingmarker.GetPositionY()) && startedInFirstPerson as Bool && !hasWeaponDrawn
			resetViewAndPlayerState = false
		endIf
	endIf


	if resetViewAndPlayerState
		ccBGSSSE001_FadeToBlackImod.ApplyCrossFade(self.DURATION_FADETOBLACKCROSSFADE)
		utility.Wait(self.DURATION_FADETOBLACKCROSSFADE - 0.100000)
		ccBGSSSE001_FadeToBlackImod.PopTo(ccBGSSSE001_FadeToBlackHoldImod, 1.00000)
		ccBGSSSE001_FishingFollowerIdleQuest.Start()
		if hasWeaponDrawn
			utility.Wait(self.DURATION_SHEATHEWEAPON)
		endIf
		if startedWithTorch
			PlayerRef.UnequipItem(Torch01 as form, false, true)
		endIf
;		if !startedInFirstPerson
;			game.ForceFirstPerson()
;		endIf
		self.MovePlayerToFishingMarker()
	endIf
	utility.Wait(self.DURATION_RODLOADTIME)
;	fishingRodActivator.TranslateToRef(currentFishingSupplies.GetFishingMarker(), 2000.00, 2000.00)
fishingmarker = currentFishingSupplies.GetFishingMarker()
fishingRodActivator.TranslateTo(fishingmarker.GetPositionX(), fishingmarker.GetPositionY(), fishingmarker.GetPositionZ(), fishingmarker.GetAngleX(), fishingmarker.GetAngleY(), fishingmarker.GetAngleZ(), 2000.00, 2000.00)

	MoveDetectRef.IgnoreTriggerEvents(true)
	objectreference fishingMarker = currentFishingSupplies.GetFishingMarker()
	ccBGSSSE001_NavBlockerRef.MoveTo(fishingMarker, 0.000000, 0.000000, 0.000000, true)
	ReelLineRef.MoveTo(fishingMarker, 0.000000, 0.000000, 0.000000, true)
	MoveDetectRef.MoveTo(fishingMarker, 0.000000, 0.000000, 0.000000, true)
	MoveDetectRef.IgnoreTriggerEvents(false)
	if resetViewAndPlayerState
		ccBGSSSE001_FadeToBlackHoldImod.PopTo(ccBGSSSE001_FadeToBlackBackImod, 1.00000)
		utility.Wait(self.DURATION_FADETOBLACKCROSSFADE - 0.100000)
	endIf
endFunction

function ShowFanfareScreenAndAddCaughtItem(form akCaughtObject)
    Form FishingPreviewQuest = Game.GetFormFromFile(0x800, "FishingPreview.esp")
    If FishingPreviewQuest
        if currentSystemState != self.SYSTEMSTATE_CATCH_RESOLVE
            game.EnablePlayerControls(true, true, true, true, true, true, true, true, 0)
        endIf
        (FishingPreviewQuest As FishingPreviewQuestScript).ShowPreview(akCaughtObject)
        Return
    EndIf

game.DisablePlayerControls(true, true, true, true, true, true, true, true, 0)

WasInThird = False

If !PlayerRef.GetAnimationVariableBool("IsFirstPerson") && FishingCamera.GetValue() < 2
		ccBGSSSE001_FadeToBlackImod.ApplyCrossFade(0.4)
		utility.Wait(0.2)
		ccBGSSSE001_FadeToBlackHoldImod.PopTo(ccBGSSSE001_FadeToBlackBackImod, 1.20000)
	WasInThird = True
	game.ForceFirstPerson()
endif

If FishingCamera.GetValue() < 2
PlayerRef.SetAngle(fishingRodActivator.GetAngleX(), fishingRodActivator.GetAngleY(), fishingRodActivator.GetAngleZ())
endif

ccBGSSSE001_CatchSuccessDOF.Apply(1.0)
	objectreference catchRef = currentFishingSupplies.PlaceAtMe(akCaughtObject, 1, false, false)
	while !catchRef.Is3DLoaded()
		utility.Wait(0.200000)
	endWhile
	catchRef.SetMotionType(catchRef.Motion_Keyframed, true)
	catchRef.Disable(false)
	objectreference fishingMarker = currentFishingSupplies.GetFishingMarker()
	objectreference fanfareLight = currentFishingSupplies.PlaceAtMe(ccBGSSSE001_CatchSuccessLight as form, 1, false, true)
	fanfareLight.MoveToNode(fishingMarker, "LightNode")
	catchRef.MoveToNode(fishingMarker, nextCatchData.successNodeName)
	fanfareLight.EnableNoWait(false)
	catchRef.EnableNoWait(true)
	PlayerRef.AddItem(catchRef.GetBaseObject(), 1, false)
	utility.Wait(self.DURATION_SUCCESSVIEW)
	fanfareLight.DisableNoWait(false)
	catchRef.DisableNoWait(false)
	fanfareLight.Delete()
	catchRef.Delete()
	ccBGSSSE001_CatchSuccessDOF.Remove()
	if currentSystemState != self.SYSTEMSTATE_CATCH_RESOLVE
		game.EnablePlayerControls(true, true, true, true, true, true, true, true, 0)
	endIf
endFunction

function RecalculateFishPopulation()

	Int basePopulation = 0
	Int morningEveningBonus = 0
	Bool shouldSetPopulation = false
	if !currentFishingSupplies.GetCalculatedPopulationRecently()
		currentFishingSupplies.SetCalculatedPopulationRecently(true)
		self.RegisterForUpdateNextMorning()
		shouldSetPopulation = true
		basePopulation = self.GetFishBasePopulation()
	else
		basePopulation = currentFishingSupplies.GetCurrentFishPopulation()
	endIf
	if !currentFishingSupplies.GetWasMorningEvening() && self.GetIsMorningEvening()
		currentFishingSupplies.SetWasMorningEvening(true)
		shouldSetPopulation = true
		morningEveningBonus = self.GetFishPopulationMorningEveningBonus()
	endIf
	if shouldSetPopulation
		self.FishingDebug("Setting population with basePopulation " + basePopulation as String + " and morningEveningBonus " + morningEveningBonus as String)
		currentFishingSupplies.SetFishPopulation(basePopulation + morningEveningBonus)
	endIf
endFunction

Float function GetSmallCatchThresholdModifier()

	if currentFishingRodType == 0 || currentFishingRodType == self.RODTYPE_DWARVEN
		return 1.00000
	elseIf currentFishingRodType == self.RODTYPE_ALIKRI
		return 0.400000
	elseIf currentFishingRodType == self.RODTYPE_ARGONIAN
		return 1.60000
	endIf
endFunction

function PlayCatchSuccessAnimation()

	fishingRodActivator.PlayAnimation(self.CATCH_SUCCESS_ANIM)
	Int catchType = nextCatchData.getCatchType()
	if catchType == ccBGSSSE001_CatchTypeSmallFish.GetValueInt()
		game.ShakeController(self.RUMBLE_STRENGTH_SUCCESSSMALLFISH_LEFT, self.RUMBLE_STRENGTH_SUCCESSSMALLFISH_RIGHT, self.RUMBLE_DURATION_SUCCESS)
	elseIf catchType == ccBGSSSE001_CatchTypeLargeFish.GetValueInt()
		game.ShakeController(self.RUMBLE_STRENGTH_SUCCESSLARGEFISH_LEFT, self.RUMBLE_STRENGTH_SUCCESSLARGEFISH_RIGHT, self.RUMBLE_DURATION_SUCCESS)
	else
		game.ShakeController(self.RUMBLE_STRENGTH_SUCCESSLARGEFISH_LEFT, self.RUMBLE_STRENGTH_SUCCESSLARGEFISH_RIGHT, self.RUMBLE_DURATION_SUCCESS)
	endIf
endFunction

function RegisterRadiantFishEventListener(ccBGSSSE001_RadiantFishEventListener listener)

	RadiantFishEventListener = listener
endFunction

Float function GetFishCatchThresholdModifier()

	if currentFishingRodType == self.RODTYPE_DWARVEN
		return 4.00000
	else
		return 1.00000
	endIf
endFunction

function CatchFail(Bool abFastExit, Bool abReduceFishPopulation)

	self.FishingDebug("Catch failure, exit!")
	if abFastExit
		self.PlayFastExitAnimation()
		utility.Wait(self.DURATION_FASTEXIT)
	else
		self.PlayCatchFailureAnimation()
		utility.Wait(self.DURATION_CATCH)
	endIf
	if abReduceFishPopulation
		currentFishingSupplies.ReduceFishPopulation(1)
	endIf
	self.CleanUp(false)
endFunction

function CleanUpFishingRodActivator(Bool abFastExit)

	if fishingRodActivator
		if abFastExit
			fishingRodActivator.PlayAnimation(self.FASTEXIT_ANIM)
			utility.Wait(self.DURATION_FASTEXIT)
		endIf
		fishingRodActivator.Disable(false)
		fishingRodActivator.Delete()
		fishingRodActivator = none
	endIf
endFunction

Bool function IsCatchSuccessful()

	if self.IsFishCatchType(nextCatchData.getCatchType())
		weapon requiredRod = nextCatchData.getRequiredRod()
		if requiredRod as Bool && currentFishingRodType != ccBGSSSE001_FishingRods.Find(requiredRod as form)
			return false
		endIf
	endIf
	return true
endFunction

function PlayCatchFailureAnimation()

	fishingRodActivator.PlayAnimation(self.CATCH_FAILURE_ANIM)
	game.ShakeController(self.RUMBLE_STRENGTH_FAILURE_LEFT, self.RUMBLE_STRENGTH_FAILURE_RIGHT, self.RUMBLE_DURATION_FAILURE)
endFunction

Bool function GetIsMorningEvening()

	return currentGameHour >= self.GAMETIME_MORNING && currentGameHour < self.GAMETIME_LATEMORNING || currentGameHour >= self.GAMETIME_EVENING && currentGameHour < self.GAMETIME_LATEEVENING
endFunction

function OnPlayerInDialogue()

	self.DoCleanupTasks()
endFunction

function ResumeFollowerBehavior()

	actor follower = FollowerAlias.GetActorRef()
	actor dog = DogAlias.GetActorRef()
	ccBGSSSE001_FishingFollowerIdleQuest.Stop()
	if follower
		follower.EvaluatePackage()
	endIf
	if dog
		dog.EvaluatePackage()
	endIf
endFunction

function StartPlayerInteraction(ccBGSSSE001_FishingActScript akFishingSupplies, Bool abContinueFishing)

	currentSystemState = self.SYSTEMSTATE_SETTINGUP
	self.CheckEnableDebug()
	currentFishingSupplies = akFishingSupplies
	self.ResetSystem()
	currentFishingRodType = self.GetCurrentFishingRodType()
	self.FishingDebug("Current fishing rod type: " + currentFishingRodType as String)
	if !self.IsFishingAllowed(currentFishingRodType)
		currentSystemState = self.SYSTEMSTATE_IDLE
		return 
	endIf
	currentGameHour = GameHour.GetValue()
	self.RecalculateFishPopulation()
	self.TryToApplyRainWeather()
	self.Fish(abContinueFishing)
endFunction

function ClearFishingAttemptVariables()

	nextCatchData = none
	currentCatchSequenceIndex = 0
	lastCatchWasRare = false
	isQuestItemCatch = false
	forcedRainWeather = false
endFunction

function OnPlayerMoveAway()

	self.DoCleanupTasks()
endFunction

function CatchSuccess()

	self.FishingDebug("Catch success!")
	self.UnregisterForUpdate()
	ccBGSSSE001_CatchSuccessSM.Play(PlayerRef as objectreference)
	self.PlayCatchSuccessAnimation()
	utility.Wait(self.DURATION_CATCH)
	form caughtObject = nextCatchData.getCaughtObject()
	Int catchType = nextCatchData.getCatchType()
	ccBGSSSE001_ITMFishUpSM.Play(PlayerRef as objectreference)
	if lastCatchWasRare
		ccBGSSSE001_RareCatchSM.Play(PlayerRef as objectreference)
	endIf
	if nextCatchData.isOneTimeCatch
		ccBGSSSE001_OneTimeCaughtList.AddForm(nextCatchData as form)
	endIf

	self.ShowFanfareScreenAndAddCaughtItem(caughtObject)
	SetFishCaught(caughtObject) ;Completionist
	if self.IsFishCatchType(catchType)
		self.TryToStartQuestAfterFirstCatch()
		currentFishingSupplies.UpdateFishCatchSuccess()
		currentFishingSupplies.ReduceFishPopulation(1)
	endIf
	if isQuestItemCatch as Bool && currentFishingSupplies.myQuestStageToSet != -1
		currentFishingSupplies.myQuest.SetStage(currentFishingSupplies.myQuestStageToSet)
	endIf
	if RadiantFishEventListener as Bool && RadiantFishEventListener.FishingSpot.GetRef() == currentFishingSupplies as objectreference
		RadiantFishEventListener.CatchEvent(caughtObject, catchType)
	endIf
	self.CleanUp(false)
endFunction

function OnUpdate()

	while handlingInputOrUpdate
		utility.Wait(0.250000)
	endWhile
	handlingInputOrUpdate = true
	self.FishingDebug("Got update...")
	if !self.IsValidUpdateSystemState()
		self.FishingDebug("System state not valid for updating, exiting.")
		handlingInputOrUpdate = false
		return 
	endIf
	if nextUpdateType == self.UPDATETYPE_START
		self.FishingDebug("    ...start")
		self.RegisterForNextUpdate(self.UPDATETYPE_SEQUENCE)
	elseIf nextUpdateType == self.UPDATETYPE_SEQUENCE
		self.FishingDebug("    ...sequence")
		if !nextCatchData
			self.FishingDebug("    ...did not have catch data. Abort.")
			self.CatchFail(true, false)
		endIf
		if self.IsFishCatchType(nextCatchData.getCatchType())
			Float[] catchSequence = nextCatchData.getCatchSequence()
			if catchSequence
				if currentCatchSequenceIndex > catchSequence.length - 1 || catchSequence[currentCatchSequenceIndex] == 0.000000
					currentSystemState = self.SYSTEMSTATE_HOOKED
					self.FishingDebug("    ...caught a fish!")
					self.PlayHookedFishAnimation()
					self.ShowCatchPrompt()
					self.RegisterForNextUpdate(self.UPDATETYPE_CATCHTIMEOUT)
				else
					currentSystemState = self.SYSTEMSTATE_NIBBLE
					self.FishingDebug("        ...nibble!")
					self.PlayNibbleAnimation()
					self.RegisterForNextUpdate(self.UPDATETYPE_SEQUENCE)
					currentCatchSequenceIndex += 1
				endIf
			else
				self.FishingDebug("    ...failed to obtain a valid catch sequence. Abort.")
				self.CatchFail(true, false)
			endIf
		else
			currentSystemState = self.SYSTEMSTATE_HOOKED
			self.FishingDebug("            ...caught an object!")
			self.ShowCatchPrompt()
			self.PlayHookedObjectAnimation()
			self.RegisterForNextUpdate(self.UPDATETYPE_CATCHTIMEOUT)
		endIf
	elseIf nextUpdateType == self.UPDATETYPE_CATCHTIMEOUT && self.IsInExitableSystemState()
		currentSystemState = self.SYSTEMSTATE_CATCH_RESOLVE
		self.FishingDebug("        ...catch timeout!")
		ccBGSSSE001_fishingLostCatch.Show(0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000)
		self.CatchFail(false, self.IsFishCatchType(nextCatchData.getCatchType()))
	endIf
	handlingInputOrUpdate = false
endFunction

Bool function IsFishCatchType(Int aiCatchType)

	return aiCatchType <= ccBGSSSE001_CatchTypeLargeFish.GetValueInt()
endFunction

ccBGSSSE001_CatchData function GetNextCatchData()

	ccBGSSSE001_CatchData catchData
	Float catchChanceFish = self.BASE_CATCH_THRESHOLD_IS_FISH * self.GetFishCatchThresholdModifier() * self.GetFishPopulationJunkModifier()
	if currentFishingSupplies.CanCatchQuestItem()
		isQuestItemCatch = true
		formlist myQuestCatchDataList = currentFishingSupplies.myQuestCatchDataList
		Int resultRoll = utility.RandomInt(0, myQuestCatchDataList.GetSize() - 1)
		self.FishingDebug("Result roll " + resultRoll as String + " from myQuestCatchDataList " + myQuestCatchDataList as String)
		return myQuestCatchDataList.GetAt(resultRoll) as ccBGSSSE001_CatchData
	endIf
	Float catchTypeRoll = 1.00000 - utility.RandomFloat(0.000000, 1.00000)
	self.FishingDebug("Catch Type Roll: " + catchTypeRoll as String)
	if !self.PlayerHasCaughtFishBefore()
		catchTypeRoll = catchChanceFish
	endIf
	if catchTypeRoll >= catchChanceFish
		formlist catchDataList
		Int biomeType = currentFishingSupplies.biomeType
		if biomeType == self.BIOME_TYPE_STREAM
			if self.GetInRain()
				catchDataList = ccBGSSSE001_FishCatchDataListTemperateStreamRain
			else
				catchDataList = ccBGSSSE001_FishCatchDataListTemperateStreamClear
			endIf
		elseIf biomeType == self.BIOME_TYPE_LAKE
			if self.GetInRain()
				catchDataList = ccBGSSSE001_FishCatchDataListTemperateLakeRain
			else
				catchDataList = ccBGSSSE001_FishCatchDataListTemperateLakeClear
			endIf
		elseIf biomeType == self.BIOME_TYPE_ARCTIC
			catchDataList = ccBGSSSE001_FishCatchDataListArctic
		elseIf biomeType == self.BIOME_TYPE_CAVE
			catchDataList = ccBGSSSE001_FishCatchDataListCave
		endIf
		catchData = self.GetNextFishCatchData(catchDataList)
	else
		catchData = self.GetNextJunkCatchData(self.GetJunkCatchDataList())
	endIf
	if catchData.isOneTimeCatch && ccBGSSSE001_OneTimeCaughtList.Find(catchData as form) > -1
		catchData = self.FindAllowedCatchDataInList(nextCatchDataSourceList)
	endIf
	return catchData
endFunction

; Skipped compiler generated GotoState

ccBGSSSE001_FishingActScript function GetCurrentFishingSupplies()

	return currentFishingSupplies
endFunction

function DoCleanupTasks()

	if currentSystemState != self.SYSTEMSTATE_IDLE && self.IsInExitableSystemState()
		self.CleanUp(true)
	elseIf currentSystemState == self.SYSTEMSTATE_IDLE
		self.ReturnSurroundingVolumes()
		self.ResumeFollowerBehavior()
		self.RestoreWeather()
	endIf
endFunction

Int function GetFishPopulationMorningEveningBonus()

	return utility.RandomInt(self.MORNINGEVENING_BONUS_MIN, self.MORNINGEVENING_BONUS_MAX)
endFunction

function ShowCatchPrompt()

	if ccBGSSSE001_ShowedCatchPromptThisSession.GetValueInt() == 0
		ccBGSSSE001_ShowedCatchPromptThisSession.SetValueInt(1)
		message.ResetHelpMessage("Activate")
		ccBGSSSE001_CatchPrompt.ShowAsHelpMessage("Activate", 5 as Float, 30 as Float, 1)
	endIf
endFunction

objectreference function PlaceFishingRodActivator(Int aiRodType)

	objectreference fishingRodPlacementMarker = PlayerRef.PlaceAtMe(XMarker as form, 1, false, false)
	fishingRodPlacementMarker.MoveTo(fishingRodPlacementMarker, 0.000000, 0.000000, -600.000, true)
	objectreference fishingRodRef = fishingRodPlacementMarker.PlaceAtMe(ccBGSSSE001_FishingRodFXActivators.GetAt(aiRodType), 1, false, false)
	fishingRodPlacementMarker.Disable(false)
	fishingRodPlacementMarker.Delete()
	return fishingRodRef
endFunction

Int function GetFishBasePopulation()

	return self.BASE_POPULATION + utility.RandomInt(self.BASE_BONUS_MIN, self.BASE_BONUS_MAX)
endFunction

function PlayFastExitAnimation()

	fishingRodActivator.PlayAnimation(self.FASTEXIT_ANIM)
endFunction

function OnFishingTriggerActivated()

	self.FishingDebug("OnFishingTriggerActivated, currentSystemState " + currentSystemState as String)
	if currentSystemState == self.SYSTEMSTATE_FISHING || currentSystemState == self.SYSTEMSTATE_NIBBLE || currentSystemState == self.SYSTEMSTATE_HOOKED
		self.ReelLine()
	elseIf currentSystemState == self.SYSTEMSTATE_IDLE
		self.StartPlayerInteraction(currentFishingSupplies, true)
	endIf
endFunction

function MovePlayerToFishingMarker()
PlayerRef.MoveTo(currentFishingSupplies.GetFishingMarker(), 0.000000, 0.000000, 0.000000, true)
endFunction

function CleanUp(Bool abFastExit)

	self.FishingDebug("Cleaning up...")
	currentSystemState = self.SYSTEMSTATE_CLEANUP
	self.UnregisterForUpdate()
	DialogueQuest.StopUpdating()
	self.CleanUpFishingRodActivator(abFastExit)
	if abFastExit
		self.ReturnSurroundingVolumes()
		self.ResumeFollowerBehavior()
		self.RestoreWeather()
	endIf
	lastFishingSupplies = currentFishingSupplies
	if startedWithTorch
		PlayerRef.EquipItem(Torch01 as form, false, true)
	endIf
	self.RestoreCameraAndControls(abFastExit)
	self.ClearFishingAttemptVariables()
	currentSystemState = self.SYSTEMSTATE_IDLE
	self.FishingDebug("Done!")
endFunction

function PlayHookedFishAnimation()

AnimatedFishing_Global.SetValue(2)
Utility.Wait(0.1)

If PlayerRef.GetAnimationVariableBool("IsFirstPerson")
else
playerref.playidle(idlesearchingchest)
endif
	fishingRodActivator.SetAnimationVariableFloat(self.LINETUG_ANIMVAR, self.LINETUG_TYPE_TUGFISH)
	fishingRodActivator.PlayAnimation(self.NIBBLE_ANIM)
	utility.Wait(self.DURATION_HOOKED_ANIM_WAIT)
	Int catchType = nextCatchData.getCatchType()
	fishingRodActivator.PlayAnimation(self.LINETUG_FISH_ANIM)
	if catchType == ccBGSSSE001_CatchTypeSmallFish.GetValueInt()
		game.ShakeController(self.RUMBLE_STRENGTH_HOOKEDSMALLFISH_LEFT, self.RUMBLE_STRENGTH_HOOKEDSMALLFISH_RIGHT, self.RUMBLE_DURATION_HOOKED)
		utility.Wait(self.RUMBLE_DURATION_HOOKED - 0.100000)
		game.ShakeController(self.RUMBLE_STRENGTH_HOOKED_LEFTCONSTANT, self.RUMBLE_STRENGTH_HOOKED_RIGHTCONSTANT, self.RUMBLE_DURATION_HOOKEDCONSTANT)
	elseIf catchType == ccBGSSSE001_CatchTypeLargeFish.GetValueInt()
		game.ShakeController(self.RUMBLE_STRENGTH_HOOKEDLARGEFISH_LEFT, self.RUMBLE_STRENGTH_HOOKEDLARGEFISH_RIGHT, self.RUMBLE_DURATION_HOOKED)
		utility.Wait(self.RUMBLE_DURATION_HOOKED - 0.100000)
		game.ShakeController(self.RUMBLE_STRENGTH_HOOKEDLARGEFISH_LEFTCONSTANT, self.RUMBLE_STRENGTH_HOOKEDLARGEFISH_RIGHTCONSTANT, self.RUMBLE_DURATION_HOOKEDCONSTANT)
	endIf
endFunction

function ClearFishingSessionVariables()

	startedInFirstPerson = false
	startedWithTorch = false
	fishingRodActivator = none
	nextUpdateType = 0
	currentFishingRodType = -1
endFunction

Float function GetFishPopulationJunkModifier()

	Int currentPopulation = currentFishingSupplies.GetCurrentFishPopulation()
	if currentPopulation >= self.POPULATION_COUNT_FULL
		return 1.00000
	elseIf currentPopulation < self.POPULATION_COUNT_FULL && currentPopulation > 0
		return 2.00000
	else
		return 100.000
	endIf
endFunction

function RestoreWeather()

	if forcedRainWeather
		weather.ReleaseOverride()
		if previousWeather
			previousWeather.SetActive(false, true)
			previousWeather = none
		endIf
	endIf
endFunction

function RegisterForNextUpdate(Int aiUpdateType)

	nextUpdateType = aiUpdateType
	self.FishingDebug("Registering for next update...")
	if aiUpdateType == self.UPDATETYPE_SETQUESTSTAGE
		self.FishingDebug("    ...set quest stage")
		self.RegisterForSingleUpdate(0.0100000)
	elseIf aiUpdateType == self.UPDATETYPE_START
		self.FishingDebug("    ...start")
		self.RegisterForSingleUpdate(self.GetInitialWaitingPeriod())
	elseIf aiUpdateType == self.UPDATETYPE_SEQUENCE
		self.FishingDebug("    ...sequence")
		if nextCatchData as Bool && self.IsFishCatchType(nextCatchData.getCatchType())
			self.RegisterForSingleUpdate(nextCatchData.getCatchSequence()[currentCatchSequenceIndex])
		else
			self.RegisterForSingleUpdate(self.DURATION_JUNKITEMCATCHTIME)
		endIf
	elseIf aiUpdateType == self.UPDATETYPE_CATCHTIMEOUT
		self.FishingDebug("    ...catch timeout")
		self.RegisterForSingleUpdate(self.DURATION_CATCHTIMEOUT)
	endIf
endFunction

function FishingDebug(String asMessage)

	if debugEnabled
		debug.trace(asMessage, 0)
	endIf
endFunction

function CheckEnableDebug()

	if ccBGSSSE001_FishingDebugEnabled.GetValueInt() == 1
		debugEnabled = true
	else
		debugEnabled = false
	endIf
endFunction

Bool function IsItemCatchType(Int aiCatchType)

	return aiCatchType == ccBGSSSE001_CatchTypeObject.GetValueInt()
endFunction

Bool function IsInExitableSystemState()

	return currentSystemState != self.SYSTEMSTATE_CATCH_RESOLVE && currentSystemState != self.SYSTEMSTATE_CLEANUP
endFunction

Float function GetInitialWaitingPeriod()

	return self.DURATION_INITIAL_WAITING_PERIOD + utility.RandomFloat(-self.DURATION_INITIAL_WAITING_PERIOD_VARIANCE, self.DURATION_INITIAL_WAITING_PERIOD_VARIANCE)
endFunction

formlist function GetJunkCatchDataList()

	formlist overrideJunkList = currentFishingSupplies.myOverrideJunkCatchDataList
	if overrideJunkList
		return overrideJunkList
	else
		return ccBGSSSE001_JunkCatchDataListDefault
	endIf
endFunction

Bool function IsValidUpdateSystemState()

	return currentSystemState >= self.SYSTEMSTATE_FISHING && currentSystemState <= self.SYSTEMSTATE_HOOKED
endFunction

Bool function IsPlayerDrawingWeapon()

	return PlayerRef.GetAnimationVariableBool("IsEquipping")
endFunction

function ReelLine()
	if !handlingInputOrUpdate
		handlingInputOrUpdate = true
		if currentSystemState == self.SYSTEMSTATE_NIBBLE
			currentSystemState = self.SYSTEMSTATE_CATCH_RESOLVE
			ccBGSSSE001_fishingEarlyReelNibble.Show(0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000)
			self.CatchFail(true, true)
		elseIf currentSystemState == self.SYSTEMSTATE_HOOKED
			currentSystemState = self.SYSTEMSTATE_CATCH_RESOLVE
			if self.IsCatchSuccessful()
				self.CatchSuccess()
			else
				ccBGSSSE001_fishingLostCatch.Show(0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000)
				self.CatchFail(false, self.IsFishCatchType(nextCatchData.getCatchType()))
			endIf
		elseIf currentSystemState == self.SYSTEMSTATE_CATCH_RESOLVE
			
		else
			currentSystemState = self.SYSTEMSTATE_CATCH_RESOLVE
			ccBGSSSE001_fishingEarlyReel.Show(0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000)
			self.CatchFail(true, false)
		endIf
		handlingInputOrUpdate = false
	endIf
endFunction

function RegisterForUpdateNextMorning()

	Float hoursUntilMorning
	self.FishingDebug("Fishing Supplies " + self as String + " RegisterForUpdateNextMorning()")
	if currentGameHour <= self.GAMETIME_MORNING
		hoursUntilMorning = self.GAMETIME_MORNING - currentGameHour
	else
		hoursUntilMorning = 24.0000 - currentGameHour + self.GAMETIME_MORNING
	endIf
	currentFishingSupplies.RegisterForPopulationUpdate(hoursUntilMorning)
endFunction

function TryToStartQuestAfterFirstCatch()

	if !self.PlayerHasCaughtFishBefore()
		ccBGSSSE001_HasCaughtFishAtLeastOnce.SetValueInt(1)
		ccBGSSSE001_Start_MQ2.Start()
	endIf
endFunction

Bool function IsFishingAllowed(Int aiFishingRodType)

	if aiFishingRodType == self.RODTYPE_NONE
		ccBGSSSE001_ErrorRodRequired.Show(0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000)
		return false
	endIf
	if PlayerRef.GetAnimationVariableBool("bInJumpState")
		ccBGSSSE001_ErrorNoFishJumping.Show(0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000)
		return false
	endIf
	if PlayerRef.IsOnMount()
		ccBGSSSE001_ErrorNoFishMounted.Show(0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000)
		return false
	endIf
	if PlayerRef.GetSitState() != 0
		ccBGSSSE001_ErrorNoFishSitting.Show(0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000)
		return false
	endIf
	if PlayerRef.GetCombatState() != 0
		ccBGSSSE001_ErrorNoFishCombat.Show(0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000)
		return false
	endIf


;;;;;;;;;;;;;;;; This is the bit for the bait stuff ;;;;;;;;;;;;;;;;;;;;

if BaitReq.GetValue() == 1
	if PlayerRef.GetItemCount(AnimatedFishing_Bait) == 0
		AnimatedFishing_NoBait.Show()
		return false
	else

        int ListSize = AnimatedFishing_Bait.GetSize()
        int CurrentItems = 0
        int ItemsLeft = 1

        while CurrentItems <= ListSize && ItemsLeft > 0
                Form CurrentItem1 = AnimatedFishing_Bait.GetAt(CurrentItems)
                int ItemCount = PlayerREF.GetItemCount(CurrentItem1)

                PlayerREF.RemoveItem(CurrentItem1, ItemsLeft)
                ItemsLeft -= ItemCount
                CurrentItems += 1
        endwhile
	endif
endif
;;;;;;;;;;;;;;;;;; This is where the bit for the bait stuff ends ;;;;;;;;;;;;;;;

	return true
endFunction


function TryToApplyRainWeather()

	Int biomeType = currentFishingSupplies.biomeType
	if PlayerRef.WornHasKeyword(ccBGSSSE001_SummonsRain) && (biomeType == self.BIOME_TYPE_STREAM || biomeType == self.BIOME_TYPE_LAKE)
		forcedRainWeather = true
		weather currentWeather = weather.GetCurrentWeather()
		if currentWeather.GetClassification() != 2
			previousWeather = currentWeather
			SkyrimStormRain.SetActive(true, true)
		endIf
	endIf
endFunction

Int function GetCurrentFishingRodType()

	weapon equippedWeaponRightHand = PlayerRef.GetEquippedWeapon(false)
	if equippedWeaponRightHand
		Int rodType = ccBGSSSE001_FishingRods.Find(equippedWeaponRightHand as form)
		if rodType > -1
			return rodType
		endIf
	endIf
	weapon equippedWeaponLeftHand = PlayerRef.GetEquippedWeapon(true)
	if equippedWeaponLeftHand
		Int rodtype = ccBGSSSE001_FishingRods.Find(equippedWeaponLeftHand as form)
		if rodtype > -1
			return rodtype
		endIf
	endIf
	return self.RODTYPE_NONE
endFunction

function OnPlayerHit()

	self.DoCleanupTasks()
endFunction

ccBGSSSE001_CatchData function GetNextFishCatchData(formlist akCatchDataList)

	Int sizeListIndex
	Int rarityListIndex
	ccBGSSSE001_CatchData catchData
	Float rarityRoll = utility.RandomFloat(0.000000, 1.00000)
	self.FishingDebug("Rarity Roll: " + rarityRoll as String)
	Float catchThresholdCommonFish = self.BASE_CATCH_THRESHOLD_COMMONFISH
	Float catchThresholdUncommonFish = self.BASE_CATCH_THRESHOLD_UNCOMMONFISH
	if currentFishingRodType == self.RODTYPE_ALIKRI || currentFishingRodType == self.RODTYPE_ARGONIAN
		catchThresholdCommonFish += self.SPECIAL_FISH_RODS_COMMONFISH_THRESHOLD_ADJUST
		catchThresholdUncommonFish += self.SPECIAL_FISH_RODS_UNCOMMONFISH_THRESHOLD_ADJUST
	endIf
	if self.GetIsMorningEvening()
		catchThresholdCommonFish += self.MORNING_EVENING_COMMONFISH_THRESHOLD_ADJUST
		catchThresholdUncommonFish += self.MORNING_EVENING_UNCOMMONFISH_THRESHOLD_ADJUST
	endIf
	if rarityRoll >= catchThresholdCommonFish
		rarityListIndex = self.RARITY_LIST_COMMON_INDEX
	elseIf rarityRoll >= catchThresholdUncommonFish
		rarityListIndex = self.RARITY_LIST_UNCOMMON_INDEX
	else
		rarityListIndex = self.RARITY_LIST_RARE_INDEX
	endIf
	formlist raritySubList = akCatchDataList.GetAt(rarityListIndex) as formlist
	Float catchChanceSmallFish = self.BASE_CATCH_THRESHOLD_SMALL * self.GetSmallCatchThresholdModifier()
	Float sizeRoll = utility.RandomFloat(0.000000, 1.00000)
	self.FishingDebug("Size Roll: " + sizeRoll as String)
	if sizeRoll >= catchChanceSmallFish
		sizeListIndex = self.SIZE_LIST_SMALLFISH_INDEX
	else
		sizeListIndex = self.SIZE_LIST_LARGEFISH_INDEX
	endIf
	formlist sizeSubList = raritySubList.GetAt(sizeListIndex) as formlist
	Int sizeSubListSize = sizeSubList.GetSize()
	if sizeSubListSize > 0
		Int resultRoll = utility.RandomInt(0, sizeSubListSize - 1)
		self.FishingDebug("Result Roll: " + resultRoll as String)
		catchData = sizeSubList.GetAt(resultRoll) as ccBGSSSE001_CatchData
		self.FishingDebug("Catch Data: " + catchData as String)
	else
		return self.GetNextJunkCatchData(self.GetJunkCatchDataList())
	endIf
	if rarityListIndex == self.RARITY_LIST_RARE_INDEX
		lastCatchWasRare = true
	endIf
	nextCatchDataSourceList = sizeSubList
	return catchData
endFunction

function PlayCastAnimation()

Weapon1 = PlayerRef.GetEquippedWeapon()
Weapon2 = PlayerRef.GetEquippedWeapon(True)
PlayerRef.UnequipItem(Weapon1, false, true)
PlayerRef.UnequipItem(Weapon2, false, true)

AnimatedFishing_Global.SetValue(1)
		if !startedInFirstPerson
Utility.Wait(0.1)
PlayerRef.PlayIdle(IdleCoweringLoose)
			Utility.Wait(0.7)
else
PlayerRef.PlayIdle(IdleCoweringLoose)
endif
utility.wait(0.1)
	fishingRodActivator.PlayAnimation(self.CAST_ANIM)
	game.ShakeController(self.RUMBLE_STRENGTH_CAST_LEFT, self.RUMBLE_STRENGTH_CAST_RIGHT, self.RUMBLE_DURATION_CAST)

Utility.wait(0.2)
float height = PlayerRef.GetPositionZ() 
fishingRodActivator.TranslateTo(fishingRodActivator.GetPositionX(), fishingRodActivator.GetPositionY(), (height - RodHeight.GetValue()), fishingRodActivator.GetAngleX(), fishingRodActivator.GetAngleY(), fishingRodActivator.GetAngleZ(), 2000.00, 2000.00)

endFunction

function PlayHookedObjectAnimation()

AnimatedFishing_Global.SetValue(2)
Utility.Wait(0.1)

If PlayerRef.GetAnimationVariableBool("IsFirstPerson")
;Debug.SendanimationEvent(PlayerRef, "Idleboundkneesenterinstant")
else
playerref.playidle(idlesearchingchest)
endif
	fishingRodActivator.SetAnimationVariableFloat(self.LINETUG_ANIMVAR, self.LINETUG_TYPE_TUGOBJECT)
	fishingRodActivator.PlayAnimation(self.NIBBLE_ANIM)
	utility.Wait(self.DURATION_HOOKED_ANIM_WAIT)
	fishingRodActivator.PlayAnimation(self.LINETUG_OBJECT_ANIM)
	game.ShakeController(self.RUMBLE_STRENGTH_HOOKEDOBJECT_LEFT, self.RUMBLE_STRENGTH_HOOKEDOBJECT_RIGHT, self.RUMBLE_DURATION_HOOKED)
	utility.Wait(self.RUMBLE_DURATION_HOOKED - 0.100000)
	game.ShakeController(self.RUMBLE_STRENGTH_HOOKED_LEFTCONSTANT, self.RUMBLE_STRENGTH_HOOKED_RIGHTCONSTANT, self.RUMBLE_DURATION_HOOKEDCONSTANT)
endFunction

function PlayVisualPopulationAnimation()

	Int currentPopulation = currentFishingSupplies.GetCurrentFishPopulation()
	if currentPopulation >= self.POPULATION_COUNT_FULL
		fishingRodActivator.PlayAnimation(self.POPULATION_FULL_ANIM)
	elseIf currentPopulation > 0
		fishingRodActivator.PlayAnimation(self.POPULATION_SPARSE_ANIM)
	else
		fishingRodActivator.PlayAnimation(self.POPULATION_EMPTY_ANIM)
	endIf
endFunction

function ResetSystem()

	self.ClearFishingSessionVariables()
	self.ClearFishingAttemptVariables()
endFunction

function ShowFishingTutorial()

	if ccBGSSSE001_FishingTutorialDisplayed.GetValueInt() == 0
		ccBGSSSE001_FishingTutorialDisplayed.SetValueInt(1)
		ccBGSSSE001_FishingTutorial.Show(0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000)
	endIf
endFunction

function UnregisterRadiantFishEventListener()

	RadiantFishEventListener = none
endFunction

function ShowReelLinePrompt()

	if ccBGSSSE001_ShowedReelPromptThisSession.GetValueInt() == 0
		ccBGSSSE001_ShowedReelPromptThisSession.SetValueInt(1)
		message.ResetHelpMessage("Activate")
		ccBGSSSE001_ReelLinePrompt.ShowAsHelpMessage("Activate", 5 as Float, 30 as Float, 1)
	endIf
endFunction

function RestoreCameraAndControls(Bool abFastExit)

PlayerRef.PlayIdle(idlestop_loose)

game.EnablePlayerControls(true, true, true, true, true, true, true, true, 0)

If FishingCamera.GetValue() == 1 && WasInThird == True
	;game.ForceThirdPerson()
	Input.TapKey(Input.GetMappedKey("Toggle POV"))
	WasInThird = False
endIf

AnimatedFishing_Global.SetValue(0)
Utility.wait(0.25)
PlayerRef.EquipItemEx(Weapon1, 1, false, false)
PlayerRef.EquipItemEx(Weapon2, 1, false, false)
ToggleDisableHeadtracking(False, False)
endFunction

Bool function PlayerHasCaughtFishBefore()

	return ccBGSSSE001_HasCaughtFishAtLeastOnce.GetValueInt() != 0
endFunction

function PlayNibbleAnimation()

	currentFishingSupplies.UpdateNibble()
	fishingRodActivator.PlayAnimation(self.NIBBLE_ANIM)
	Int catchType = nextCatchData.getCatchType()
	if catchType == ccBGSSSE001_CatchTypeSmallFish.GetValueInt()
		game.ShakeController(self.RUMBLE_STRENGTH_NIBBLESMALL_LEFT, self.RUMBLE_STRENGTH_NIBBLESMALL_RIGHT, self.RUMBLE_DURATION_NIBBLE)
	elseIf catchType == ccBGSSSE001_CatchTypeLargeFish.GetValueInt()
		game.ShakeController(self.RUMBLE_STRENGTH_NIBBLELARGE_LEFT, self.RUMBLE_STRENGTH_NIBBLELARGE_RIGHT, self.RUMBLE_DURATION_NIBBLE)
	endIf
endFunction

function Fish(Bool abContinueFishing)

ToggleDisableHeadtracking(False, True)
	currentFishingSupplies.UpdateFish()
	DialogueQuest.StartUpdating()
	fishingRodActivator = self.PlaceFishingRodActivator(currentFishingRodType)
	self.FishingDebug("Placing fishing rod activator " + fishingRodActivator as String)
	self.SetupCameraAndPosition(abContinueFishing)
	self.SetVisualPopulation()
	self.ShowFishingTutorial()
	self.ShowReelLinePrompt()
	nextCatchData = self.GetNextCatchData()
	self.FishingDebug("Catch data for next catch")
	self.FishingDebug("  === Catch: " + nextCatchData.getCaughtObject() as String)
	self.FishingDebug("  === Catch Type: " + nextCatchData.getCatchType() as String)
	self.PlayCastAnimation()
	utility.Wait(self.DURATION_CAST)
	self.PlayVisualPopulationAnimation()
	self.RegisterForNextUpdate(self.UPDATETYPE_START)
	currentSystemState = self.SYSTEMSTATE_FISHING
endFunction

Idle Property IdleStop_Loose  Auto  

Idle Property IdleSearchingChest Auto

Idle Property idlecowerexit Auto

idle property idlecoweringloose auto

GlobalVariable Property AnimatedFishing_Global Auto

GlobalVariable Property RodHeight Auto

GlobalVariable Property BaitReq Auto

GlobalVariable Property FishingCamera Auto

weapon weapon1

weapon weapon2

Message Property AnimatedFishing_NoBait Auto

Formlist Property AnimatedFishing_Bait Auto

Bool WasInThird