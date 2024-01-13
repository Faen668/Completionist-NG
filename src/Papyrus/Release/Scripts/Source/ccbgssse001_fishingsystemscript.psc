scriptName ccBGSSSE001_FishingSystemScript extends Quest
{The system that handles all fishing gameplay mechanics.
  Interacts with Fishing Supplies (ccBGSSSE001_FishingActScript), which maintain some local state.}

Import Completionist_Native

Int property UPDATETYPE_CATCHTIMEOUT
    Int function get()
        return 3
    endFunction
endproperty
String property LINETUG_ANIMVAR
    String function get()
        return "iFishBite"
    endFunction
endproperty
imagespacemodifier property ccBGSSSE001_FadeToBlackImod auto
message property ccBGSSSE001_CatchPrompt auto
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
Int property SYSTEMSTATE_NIBBLE
    Int function get()
        return 3
    endFunction
endproperty
String property TIMEOUT_ANIM
    String function get()
        return "Timeout"
    endFunction
endproperty
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
Float property MORNING_EVENING_COMMONFISH_THRESHOLD_ADJUST
    Float function get()
        return 0.100000
    endFunction
endproperty
formlist property ccBGSSSE001_FishCatchDataListArctic auto
{The arctic fish list to draw from.}
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
Float property RUMBLE_STRENGTH_HOOKEDSMALLFISH_LEFT
    Float function get()
        return 1.00000
    endFunction
endproperty
Float property LINETUG_TYPE_NONE
    Float function get()
        return 1.00000
    endFunction
endproperty
Float property RUMBLE_STRENGTH_HOOKEDLARGEFISH_LEFT
    Float function get()
        return 0.200000
    endFunction
endproperty
Int property BIOME_TYPE_LAKE
    Int function get()
        return 1
    endFunction
endproperty
Int property BIOME_TYPE_CAVE
    Int function get()
        return 3
    endFunction
endproperty
Float property DURATION_SHOWPOPULATION
    Float function get()
        return 3.00000
    endFunction
endproperty
Float property DURATION_FASTEXIT
    Float function get()
        return 0.500000
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
formlist property ccBGSSSE001_FishingRodFXActivators auto
{A formlist of all fishing rod FX activators.}
Float property DURATION_FADETOBLACKCROSSFADE
    Float function get()
        return 0.300000
    endFunction
endproperty
Float property RUMBLE_STRENGTH_FAILURE_RIGHT
    Float function get()
        return 0.000000
    endFunction
endproperty
Int property SIZE_LIST_SMALLFISH_INDEX
    Int function get()
        return 0
    endFunction
endproperty
objectreference property ReelLineRef auto
{A reference to ccBGSSSE001ReelLineAct in aaaMarkers cell. Trigger volume for player activation.}
Int property RODTYPE_NONE
    Int function get()
        return -1
    endFunction
endproperty
Int property SYSTEMSTATE_CATCH_RESOLVE
    Int function get()
        return 5
    endFunction
endproperty
Float property RUMBLE_STRENGTH_SUCCESSSMALLFISH_RIGHT
    Float function get()
        return 0.0800000
    endFunction
endproperty
Float property RUMBLE_STRENGTH_FAILURE_LEFT
    Float function get()
        return 1.00000
    endFunction
endproperty
formlist property ccBGSSSE001_FishingRods auto
{A formlist of all fishing rod weapons.}
Float property LINETUG_TYPE_TUGFISH
    Float function get()
        return 3.00000
    endFunction
endproperty
Int property UPDATETYPE_SEQUENCE
    Int function get()
        return 2
    endFunction
endproperty
actor property PlayerRef auto
sound property ccBGSSSE001_CatchSuccessSM auto
{Catch success sound.}
formlist property ccBGSSSE001_FishCatchDataListTemperateStreamRain auto
{The temperate stream (rain weather) fish list to draw from.}
String property RESET_ANIM
    String function get()
        return "Reset"
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
Int property SYSTEMSTATE_HOOKED
    Int function get()
        return 4
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
Float property RUMBLE_STRENGTH_HOOKEDLARGEFISH_RIGHT
    Float function get()
        return 0.650000
    endFunction
endproperty
String property FASTEXIT_ANIM
    String function get()
        return "FastExit"
    endFunction
endproperty
message property ccBGSSSE001_FishingTutorial auto
Quest property ccBGSSSE001_FishingFollowerIdleQuest auto
{The follower idle quest to help keep followers out of the way when the player is fishing.}
light property Torch01 auto
formlist property ccBGSSSE001_JunkCatchDataListDefault auto
{The junk list to draw from.}
Float property DURATION_CATCH
    Float function get()
        return 1.00000
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
Float property BASE_CATCH_THRESHOLD_UNCOMMONFISH
    Float function get()
        return 0.0300000
    endFunction
endproperty
Int property BASE_POPULATION
    Int function get()
        return 4
    endFunction
endproperty
Float property DURATION_CATCHTIMEOUT
    Float function get()
        return 5.00000
    endFunction
endproperty
keyword property ccBGSSSE001_SummonsRain auto
{The equipment keyword that will cause rainstorms when fishing if wearing that equipment.}
message property ccBGSSSE001_ErrorNoFishMounted auto
Float property RUMBLE_STRENGTH_HOOKEDSMALLFISH_RIGHT
    Float function get()
        return 0.0500000
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
globalvariable property GameHour auto
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
Float property RUMBLE_DURATION_NIBBLE
    Float function get()
        return 0.450000
    endFunction
endproperty
String property POPULATION_SPARSE_ANIM
    String function get()
        return "IdleSparse"
    endFunction
endproperty
Float property SPECIAL_FISH_RODS_COMMONFISH_THRESHOLD_ADJUST
    Float function get()
        return 0.200000
    endFunction
endproperty
Float property RUMBLE_STRENGTH_NIBBLELARGE_LEFT
    Float function get()
        return 0.000000
    endFunction
endproperty
String property CATCH_SUCCESS_ANIM
    String function get()
        return "CatchSuccess"
    endFunction
endproperty
weather property SkyrimStormRain auto
{The weather to force to when wearing equipment that summons rain.}
globalvariable property ccBGSSSE001_ShowedReelPromptThisSession auto
objectreference property ccBGSSSE001_NavBlockerRef auto
{A reference to ccBGSSSE001_NavBlockerRef in aaaMarkers cell. Collision navcut volume to help prevent NPCs pushing the player.}
imagespacemodifier property ccBGSSSE001_FadeToBlackBackImod auto
Float property RUMBLE_STRENGTH_HOOKEDOBJECT_RIGHT
    Float function get()
        return 0.000000
    endFunction
endproperty
sound property ccBGSSSE001_ITMFishUpSM auto
{Item fanfare sound.}
globalvariable property ccBGSSSE001_HasCaughtFishAtLeastOnce auto
{The global that tracks whether or not the player has caught a fish at least once.}
Quest property ccBGSSSE001_Start_MQ2 auto
{The quest to start after the player has caught a fish at least once.}
Float property RUMBLE_DURATION_HOOKED
    Float function get()
        return 0.450000
    endFunction
endproperty
message property ccBGSSSE001_ErrorNoFishCombat auto
ccbgssse001_dialoguedetectscript property DialogueQuest auto
{The dialogue detection system quest.}
static property XMarker auto
referencealias property DogAlias auto
{The dog alias from the idle quest.}
Float property LINETUG_TYPE_NIBBLE
    Float function get()
        return 2.00000
    endFunction
endproperty
formlist property ccBGSSSE001_FishCatchDataListTemperateStreamClear auto
{The temperate stream (clear weather) fish list to draw from.}
String property POPULATION_ANIMVAR
    String function get()
        return "iFishPopulation"
    endFunction
endproperty
Int property UPDATETYPE_SETQUESTSTAGE
    Int function get()
        return 4
    endFunction
endproperty
Float property RUMBLE_STRENGTH_CAST_RIGHT
    Float function get()
        return 0.000000
    endFunction
endproperty
Float property SPECIAL_FISH_RODS_UNCOMMONFISH_THRESHOLD_ADJUST
    Float function get()
        return 0.0700000
    endFunction
endproperty
Float property GAMETIME_LATEMORNING
    Float function get()
        return 9.00000
    endFunction
endproperty
String property POPULATION_FULL_ANIM
    String function get()
        return "IdleFull"
    endFunction
endproperty
Int property MORNINGEVENING_BONUS_MAX
    Int function get()
        return 3
    endFunction
endproperty
sound property ccBGSSSE001_RareCatchSM auto
{Rare catch success sound.}
ccbgssse001_movedetectscript property MoveDetectRef auto
{A reference to ccBGSSSE001MoveDetectAct in aaaMarkers cell. Trigger volume for detecting player movement.}
message property ccBGSSSE001_fishingLostCatch auto
Int property BIOME_TYPE_ARCTIC
    Int function get()
        return 2
    endFunction
endproperty
message property ccBGSSSE001_ErrorRodRequired auto
message property ccBGSSSE001_fishingHooked auto
Float property POPULATION_TYPE_SPARSE
    Float function get()
        return 2.00000
    endFunction
endproperty
message property ccBGSSSE001_fishingEarlyReel auto
String property CAST_ANIM
    String function get()
        return "Cast"
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
message property ccBGSSSE001_ReelLinePrompt auto
Int property RODTYPE_STANDARD
    Int function get()
        return 0
    endFunction
endproperty
message property ccBGSSSE001_ErrorNoFishSitting auto
message property ccBGSSSE001_ErrorNoFishJumping auto
Float property RUMBLE_STRENGTH_SUCCESSLARGEFISH_LEFT
    Float function get()
        return 0.100000
    endFunction
endproperty
globalvariable property ccBGSSSE001_FishingDebugEnabled auto
globalvariable property ccBGSSSE001_FishingTutorialDisplayed auto
Float property RUMBLE_DURATION_CAST
    Float function get()
        return 0.600000
    endFunction
endproperty
globalvariable property ccBGSSSE001_ShowedCatchPromptThisSession auto
light property ccBGSSSE001_CatchSuccessLight auto
{The light to display when an object is caught.}
Float property GAMETIME_EVENING
    Float function get()
        return 18.0000
    endFunction
endproperty
formlist property ccBGSSSE001_OneTimeCaughtList auto
{FormList of Catch Data that are flagged as one-time-only and have already been caught.}
Float property DURATION_INITIAL_WAITING_PERIOD_VARIANCE
    Float function get()
        return 2.00000
    endFunction
endproperty
Int property SYSTEMSTATE_CLEANUP
    Int function get()
        return 6
    endFunction
endproperty
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
String property EXIT_ANIM
    String function get()
        return "Exit"
    endFunction
endproperty
Int property RODTYPE_DWARVEN
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
String property NIBBLE_ANIM
    String function get()
        return "Nibble"
    endFunction
endproperty
Int property RODTYPE_ARGONIAN
    Int function get()
        return 2
    endFunction
endproperty
imagespacemodifier property ccBGSSSE001_FadeToBlackHoldImod auto
Float property RUMBLE_STRENGTH_NIBBLESMALL_RIGHT
    Float function get()
        return 0.000000
    endFunction
endproperty
Int property BIOME_TYPE_STREAM
    Int function get()
        return 0
    endFunction
endproperty
Float property RUMBLE_STRENGTH_SUCCESSOBJECT_LEFT
    Float function get()
        return 0.600000
    endFunction
endproperty
Float property BASE_CATCH_THRESHOLD_COMMONJUNK
    Float function get()
        return 0.350000
    endFunction
endproperty
Int property RODTYPE_ALIKRI
    Int function get()
        return 1
    endFunction
endproperty
referencealias property FollowerAlias auto
{The follower alias from the idle quest.}
Float property GAMETIME_MORNING
    Float function get()
        return 6.00000
    endFunction
endproperty
String property POPULATION_EMPTY_ANIM
    String function get()
        return "IdleEmpty"
    endFunction
endproperty
Float property RUMBLE_STRENGTH_HOOKEDOBJECT_LEFT
    Float function get()
        return 0.500000
    endFunction
endproperty
Int property SIZE_LIST_LARGEFISH_INDEX
    Int function get()
        return 1
    endFunction
endproperty
Float property RUMBLE_STRENGTH_HOOKEDLARGEFISH_RIGHTCONSTANT
    Float function get()
        return 0.100000
    endFunction
endproperty
Float property RUMBLE_STRENGTH_SUCCESSSMALLFISH_LEFT
    Float function get()
        return 1.00000
    endFunction
endproperty
Float property RUMBLE_STRENGTH_SUCCESSOBJECT_RIGHT
    Float function get()
        return 0.100000
    endFunction
endproperty
Int property POPULATION_COUNT_FULL
    Int function get()
        return 4
    endFunction
endproperty
Float property DURATION_HOOKED_ANIM_WAIT
    Float function get()
        return 0.350000
    endFunction
endproperty
Int property BASE_BONUS_MAX
    Int function get()
        return 2
    endFunction
endproperty
Float property LINETUG_TYPE_TUGOBJECT
    Float function get()
        return 4.00000
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
Int property UPDATETYPE_START
    Int function get()
        return 1
    endFunction
endproperty
Float property POPULATION_TYPE_FULL
    Float function get()
        return 3.00000
    endFunction
endproperty
message property ccBGSSSE001_fishingEarlyReelNibble auto
Float property RUMBLE_STRENGTH_SUCCESSLARGEFISH_RIGHT
    Float function get()
        return 1.00000
    endFunction
endproperty
Float property SPECIAL_JUNK_RODS_UNCOMMONJUNK_THRESHOLD_ADJUST
    Float function get()
        return 0.0600000
    endFunction
endproperty
Float property BASE_CATCH_THRESHOLD_IS_FISH
    Float function get()
        return 0.100000
    endFunction
endproperty
String property LINETUG_OBJECT_ANIM
    String function get()
        return "BiteObject"
    endFunction
endproperty
Float property POPULATION_TYPE_EMPTY
    Float function get()
        return 1.00000
    endFunction
endproperty
Float property BASE_CATCH_THRESHOLD_SMALL
{50% chance of Small fish catch. Rods modify this by 25% up (Alik'ri) or down (Argonian, so, greater chance to catch Large fish).}
    Float function get()
        return 0.500000
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
Int property RARITY_LIST_UNCOMMON_INDEX
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
String property CATCH_FAILURE_ANIM
    String function get()
        return "CatchFail"
    endFunction
endproperty
Int property SYSTEMSTATE_SETTINGUP
    Int function get()
        return 1
    endFunction
endproperty
String property LINETUG_FISH_ANIM
    String function get()
        return "BiteFish"
    endFunction
endproperty
formlist property ccBGSSSE001_FishCatchDataListCave auto
{The cave fish list to draw from.}
Int currentFishingRodType = -1
formlist nextCatchDataSourceList
Int currentSystemState = 0
Bool forcedRainWeather = false
ccBGSSSE001_FishingActScript currentFishingSupplies
Int currentCatchSequenceIndex = 0
weather previousWeather
Bool startedInFirstPerson = false
Bool lastCatchWasRare = false
objectreference fishingRodActivator
ccBGSSSE001_FishingActScript lastFishingSupplies
ccBGSSSE001_CatchData nextCatchData
Bool isQuestItemCatch = false
Bool handlingInputOrUpdate = false
ccBGSSSE001_RadiantFishEventListener RadiantFishEventListener
Bool debugEnabled = false
Int nextUpdateType = 0
Float currentGameHour = 0.000000
Bool startedWithTorch = false

; DINO: modified to control rarity chance by global variable
ccBGSSSE001_CatchData function GetNextJunkCatchData(formlist akCatchDataList)
    Int rarityListIndex
    Float rarityRoll = utility.RandomFloat(0.000000, 1.00000)
    FishingDebug("Rarity Roll: " + rarityRoll as String)
    ;Float catchThresholdCommonJunk = BASE_CATCH_THRESHOLD_COMMONJUNK
    ;Float catchThresholdUncommonJunk = BASE_CATCH_THRESHOLD_UNCOMMONJUNK
    float catchThresholdUncommonJunk = __FancyFishingVarChanceJunkRareBase.GetValue()
    float catchThresholdCommonJunk = catchThresholdUncommonJunk + __FancyFishingVarChanceJunkUncommonBase.GetValue()
    if currentFishingRodType == RODTYPE_DWARVEN
        catchThresholdCommonJunk += SPECIAL_JUNK_RODS_COMMONJUNK_THRESHOLD_ADJUST
        catchThresholdUncommonJunk += SPECIAL_JUNK_RODS_UNCOMMONJUNK_THRESHOLD_ADJUST
    endIf
    if rarityRoll >= catchThresholdCommonJunk
        rarityListIndex = RARITY_LIST_COMMON_INDEX
    elseIf rarityRoll >= catchThresholdUncommonJunk
        rarityListIndex = RARITY_LIST_UNCOMMON_INDEX
    else
        rarityListIndex = RARITY_LIST_RARE_INDEX
    endIf
    formlist raritySubList = akCatchDataList.GetAt(rarityListIndex) as formlist
    Int raritySubListSize = raritySubList.GetSize()
    Int resultRoll = utility.RandomInt(0, raritySubListSize - 1)
    FishingDebug("Result Roll: " + resultRoll as String)
    ccBGSSSE001_CatchData catchData = raritySubList.GetAt(resultRoll) as ccBGSSSE001_CatchData
    FishingDebug("Catch Data: " + catchData as String)
    if rarityListIndex == RARITY_LIST_RARE_INDEX
        lastCatchWasRare = true
    endIf
    nextCatchDataSourceList = akCatchDataList
    return catchData
endFunction

function PlayVisualPopulationAnimation()
    Int currentPopulation = currentFishingSupplies.GetCurrentFishPopulation()
    if currentPopulation >= POPULATION_COUNT_FULL
        fishingRodActivator.PlayAnimation(POPULATION_FULL_ANIM)
    elseIf currentPopulation > 0
        fishingRodActivator.PlayAnimation(POPULATION_SPARSE_ANIM)
    else
        fishingRodActivator.PlayAnimation(POPULATION_EMPTY_ANIM)
    endIf
endFunction

; DINO: for compat with:
;   - Simple Fishing Overhaul
; originally part of IsFishingAllowed; separated to prevent issues when IsFishingAllowed is called externally
; (e.g. by Fish Anywhere)
Bool function CheckForBait()
    ;;;;;;;;;;;;;;;; This is the bit for the bait stuff ;;;;;;;;;;;;;;;;;;;;
    if AnimatedFishing_Global && BaitReq.GetValue() == 1
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

Bool function IsFishingAllowed(Int aiFishingRodType)
    if aiFishingRodType == RODTYPE_NONE
        ccBGSSSE001_ErrorRodRequired.Show()
        return false
    endIf
    if PlayerRef.GetAnimationVariableBool("bInJumpState")
        ccBGSSSE001_ErrorNoFishJumping.Show()
        return false
    endIf
    if PlayerRef.IsOnMount()
        ccBGSSSE001_ErrorNoFishMounted.Show()
        return false
    endIf
    if PlayerRef.GetSitState() != 0
        ccBGSSSE001_ErrorNoFishSitting.Show()
        return false
    endIf
    if PlayerRef.GetCombatState() != 0
        ccBGSSSE001_ErrorNoFishCombat.Show()
        return false
    endIf

    return true
endFunction

function ReturnSurroundingVolumes()
    objectreference returnRef = ReelLineRef.GetLinkedRef()
    ReelLineRef.MoveTo(returnRef)
    MoveDetectRef.MoveTo(returnRef)
    ccBGSSSE001_NavBlockerRef.MoveTo(returnRef)
endFunction

function SetVisualPopulation()
    Int currentPopulation = currentFishingSupplies.GetCurrentFishPopulation()
    FishingDebug("Current population: " + currentPopulation as String)
    if currentPopulation >= POPULATION_COUNT_FULL
        FishingDebug("Setting full population animation var")
        fishingRodActivator.SetAnimationVariableFloat(POPULATION_ANIMVAR, POPULATION_TYPE_FULL)
    elseIf currentPopulation > 0
        FishingDebug("Setting sparse population animation var")
        fishingRodActivator.SetAnimationVariableFloat(POPULATION_ANIMVAR, POPULATION_TYPE_SPARSE)
    else
        FishingDebug("Setting empty population animation var")
        fishingRodActivator.SetAnimationVariableFloat(POPULATION_ANIMVAR, POPULATION_TYPE_EMPTY)
    endIf
endFunction

function PlayResetAnimation()
    fishingRodActivator.PlayAnimation(RESET_ANIM)
endFunction

Bool function IsPlayerDrawingWeapon()
    return PlayerRef.GetAnimationVariableBool("IsEquipping")
endFunction

Float function GetFishCatchThresholdModifier()
    if currentFishingRodType == RODTYPE_DWARVEN
        return 4.00000
    else
        return 1.00000
    endIf
endFunction

function RecalculateFishPopulation()
    Int basePopulation = 0
    Int morningEveningBonus = 0
    Bool shouldSetPopulation = false
    if !currentFishingSupplies.GetCalculatedPopulationRecently()
        currentFishingSupplies.SetCalculatedPopulationRecently(true)
        RegisterForUpdateNextMorning()
        shouldSetPopulation = true
        basePopulation = GetFishBasePopulation()
    else
        basePopulation = currentFishingSupplies.GetCurrentFishPopulation()
    endIf
    if !currentFishingSupplies.GetWasMorningEvening() && GetIsMorningEvening()
        currentFishingSupplies.SetWasMorningEvening(true)
        shouldSetPopulation = true
        morningEveningBonus = GetFishPopulationMorningEveningBonus()
    endIf
    if shouldSetPopulation
        FishingDebug("Setting population with basePopulation " + basePopulation as String + " and morningEveningBonus " + morningEveningBonus as String)
        currentFishingSupplies.SetFishPopulation(basePopulation + morningEveningBonus)
    endIf
endFunction

Float function GetSmallCatchThresholdModifier()
    if currentFishingRodType == 0 || currentFishingRodType == RODTYPE_DWARVEN
        return 1.00000
    elseIf currentFishingRodType == RODTYPE_ALIKRI
        return 0.400000
    elseIf currentFishingRodType == RODTYPE_ARGONIAN
        return 1.60000
    endIf
endFunction

function PlayCatchSuccessAnimation()
    fishingRodActivator.PlayAnimation(CATCH_SUCCESS_ANIM)
    Int catchType = nextCatchData.getCatchType()
    if catchType == ccBGSSSE001_CatchTypeSmallFish.GetValueInt()
        game.ShakeController(RUMBLE_STRENGTH_SUCCESSSMALLFISH_LEFT, RUMBLE_STRENGTH_SUCCESSSMALLFISH_RIGHT, RUMBLE_DURATION_SUCCESS)
    elseIf catchType == ccBGSSSE001_CatchTypeLargeFish.GetValueInt()
        game.ShakeController(RUMBLE_STRENGTH_SUCCESSLARGEFISH_LEFT, RUMBLE_STRENGTH_SUCCESSLARGEFISH_RIGHT, RUMBLE_DURATION_SUCCESS)
    else
        game.ShakeController(RUMBLE_STRENGTH_SUCCESSLARGEFISH_LEFT, RUMBLE_STRENGTH_SUCCESSLARGEFISH_RIGHT, RUMBLE_DURATION_SUCCESS)
    endIf
endFunction

function RegisterRadiantFishEventListener(ccBGSSSE001_RadiantFishEventListener listener)
    RadiantFishEventListener = listener
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
    FishingDebug("There were no allowed CatchData in list " + akCatchDataList as String + ", a non-exclusive result should be added!")
    return none
endFunction

; DINO: edited to allow skipping fade to black
; plus compat:
;   - Simple Fishing Overhaul
;   - Fishing Preview
;   - Proper Mod-Added Torch Support
function SetupCameraAndPosition(Bool abContinueFishing)
    bool do_fade = __FancyFishingVarDoFade.GetValue() > 0
    startedInFirstPerson = PlayerRef.GetAnimationVariableBool("IsFirstPerson")

    playerTorch = PlayerRef.GetEquippedObject(0) as Light
    startedWithTorch = playerTorch != none

    while IsPlayerDrawingWeapon()
        utility.Wait(0.250000)
    endWhile
    Bool hasWeaponDrawn = PlayerRef.IsWeaponDrawn()

    bool disable_camera = __FancyFishingVarAllowCameraMovement.GetValue() == 0
    game.DisablePlayerControls(true, true, disable_camera, disable_camera, true, true, false, true, 0)

    Bool resetViewAndPlayerState = true
    ObjectReference fishingMarker = currentFishingSupplies.GetFishingMarker()
    if abContinueFishing
        if PlayerRef.GetAngleX() == 0.000000 && PlayerRef.GetAngleZ() == fishingMarker.GetAngleZ() && math.Floor(PlayerRef.GetPositionX()) == math.Floor(fishingMarker.GetPositionX()) && math.Floor(PlayerRef.GetPositionY()) == math.Floor(fishingMarker.GetPositionY()) && startedInFirstPerson as Bool && !hasWeaponDrawn
            resetViewAndPlayerState = false
        endIf
    endIf
    if resetViewAndPlayerState
        if do_fade
            ccBGSSSE001_FadeToBlackImod.ApplyCrossFade(DURATION_FADETOBLACKCROSSFADE)
            utility.Wait(DURATION_FADETOBLACKCROSSFADE - 0.100000)
            ccBGSSSE001_FadeToBlackImod.PopTo(ccBGSSSE001_FadeToBlackHoldImod)
        endif
        ccBGSSSE001_FishingFollowerIdleQuest.Start()
        if hasWeaponDrawn
            utility.Wait(DURATION_SHEATHEWEAPON)
        endIf
        if startedWithTorch
            PlayerRef.UnequipItemEx(playerTorch, 0)
        endIf
        if !(AnimatedFishing_Global) && !startedInFirstPerson
            game.ForceFirstPerson()
        endIf
        MovePlayerToFishingMarker()
    endIf
    utility.Wait(DURATION_RODLOADTIME)

    fishingRodActivator.TranslateTo(fishingMarker.GetPositionX(), fishingMarker.GetPositionY(), fishingMarker.GetPositionZ(), fishingMarker.GetAngleX(), fishingMarker.GetAngleY(), fishingMarker.GetAngleZ(), 2000.00, 2000.00)

    MoveDetectRef.IgnoreTriggerEvents(true)
    ccBGSSSE001_NavBlockerRef.MoveTo(fishingMarker)
    ReelLineRef.MoveTo(fishingMarker)
    MoveDetectRef.MoveTo(fishingMarker)
    MoveDetectRef.IgnoreTriggerEvents(false)
    if resetViewAndPlayerState && do_fade
        ccBGSSSE001_FadeToBlackHoldImod.PopTo(ccBGSSSE001_FadeToBlackBackImod)
        utility.Wait(DURATION_FADETOBLACKCROSSFADE - 0.100000)
    endIf
endFunction

function OnPlayerMoveAway()
    DoCleanupTasks()
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

; DINO: entry point, is called by both base CC fishing init & Fish Anywhere With Water
; DINO: modified for Simple Fishing Overhaul compat:
;   - to set RequiredBaitCount (normally 0, set locally to 1 to allow checking & removal of bait if needed)
;   - also grab properties via GetFormFromFile (needed before CheckForBait)
function StartPlayerInteraction(ccBGSSSE001_FishingActScript akFishingSupplies, Bool abContinueFishing)
    currentSystemState = SYSTEMSTATE_SETTINGUP
    CheckEnableDebug()
    currentFishingSupplies = akFishingSupplies
    ResetSystem()
    currentFishingRodType = GetCurrentFishingRodType()
    FishingDebug("Current fishing rod type: " + currentFishingRodType as String)

    ; SFO settings
    ; idea is to avoid touching this quest record in the Fancy Fishing + SFO patch itself
    AnimatedFishing_Global = Game.GetFormFromFile(0x800, "Simple Fishing Overhaul.esp") as GlobalVariable
    if AnimatedFishing_Global
        RodHeight = Game.GetFormFromFile(0x801, "Simple Fishing Overhaul.esp") as GlobalVariable
        BaitReq = Game.GetFormFromFile(0x819, "Simple Fishing Overhaul.esp") as GlobalVariable
        FishingCamera = Game.GetFormFromFile(0x86F, "Simple Fishing Overhaul.esp") as GlobalVariable
        AnimatedFishing_NoBait = Game.GetFormFromFile(0x818, "Simple Fishing Overhaul.esp") as Message
        AnimatedFishing_Bait = Game.GetFormFromFile(0x817, "Simple Fishing Overhaul.esp") as FormList

        TrueDirectionalMovement.ToggleDisableHeadtracking(False, True)
    endif

    if !IsFishingAllowed(currentFishingRodType) || !CheckForBait()
        currentSystemState = SYSTEMSTATE_IDLE
        return 
    endIf

    currentGameHour = GameHour.GetValue()
    RecalculateFishPopulation()
    TryToApplyRainWeather()
    Fish(abContinueFishing)
endFunction

function PlayCatchFailureAnimation()
    fishingRodActivator.PlayAnimation(CATCH_FAILURE_ANIM)
    game.ShakeController(RUMBLE_STRENGTH_FAILURE_LEFT, RUMBLE_STRENGTH_FAILURE_RIGHT, RUMBLE_DURATION_FAILURE)
endFunction

Bool function GetIsMorningEvening()
    return currentGameHour >= GAMETIME_MORNING && currentGameHour < GAMETIME_LATEMORNING || currentGameHour >= GAMETIME_EVENING && currentGameHour < GAMETIME_LATEEVENING
endFunction

function OnPlayerInDialogue()
    DoCleanupTasks()
endFunction

Bool function IsCatchSuccessful()
    if IsFishCatchType(nextCatchData.getCatchType())
        weapon requiredRod = nextCatchData.getRequiredRod()
        if requiredRod as Bool && currentFishingRodType != ccBGSSSE001_FishingRods.Find(requiredRod as form)
            return false
        endIf
    endIf
    return true
endFunction

; DINO: modified to enable configuration of junk base & max chances by global variable
ccBGSSSE001_CatchData function GetNextCatchData()
    ccBGSSSE001_CatchData catchData
    ; misleading name?  looks like this actually translates to the chance of getting junk?
    ;Float catchChanceFish = BASE_CATCH_THRESHOLD_IS_FISH * GetFishCatchThresholdModifier() * GetFishPopulationJunkModifier()
    float catchThresholdFish = __FancyFishingVarChanceJunkBase.GetValue() * GetFishCatchThresholdModifier() * GetFishPopulationJunkModifier()
    float max_junk_chance = __FancyFishingVarChanceJunkMax.GetValue()
    if catchThresholdFish > max_junk_chance
        catchThresholdFish = max_junk_chance
    endif
    if currentFishingSupplies.CanCatchQuestItem()
        isQuestItemCatch = true
        formlist myQuestCatchDataList = currentFishingSupplies.myQuestCatchDataList
        Int resultRoll = utility.RandomInt(0, myQuestCatchDataList.GetSize() - 1)
        FishingDebug("Result roll " + resultRoll as String + " from myQuestCatchDataList " + myQuestCatchDataList as String)
        return myQuestCatchDataList.GetAt(resultRoll) as ccBGSSSE001_CatchData
    endIf
    Float catchTypeRoll = 1.00000 - utility.RandomFloat(0.000000, 1.00000)
    FishingDebug("Catch Type Roll: " + catchTypeRoll as String)
    if !PlayerHasCaughtFishBefore()
        catchTypeRoll = catchThresholdFish
    endIf
    if catchTypeRoll >= catchThresholdFish
        formlist catchDataList
        Int biomeType = currentFishingSupplies.biomeType
        if biomeType == BIOME_TYPE_STREAM
            if GetInRain()
                catchDataList = ccBGSSSE001_FishCatchDataListTemperateStreamRain
            else
                catchDataList = ccBGSSSE001_FishCatchDataListTemperateStreamClear
            endIf
        elseIf biomeType == BIOME_TYPE_LAKE
            if GetInRain()
                catchDataList = ccBGSSSE001_FishCatchDataListTemperateLakeRain
            else
                catchDataList = ccBGSSSE001_FishCatchDataListTemperateLakeClear
            endIf
        elseIf biomeType == BIOME_TYPE_ARCTIC
            catchDataList = ccBGSSSE001_FishCatchDataListArctic
        elseIf biomeType == BIOME_TYPE_CAVE
            catchDataList = ccBGSSSE001_FishCatchDataListCave
        endIf
        catchData = GetNextFishCatchData(catchDataList)
    else
        catchData = GetNextJunkCatchData(GetJunkCatchDataList())
    endIf
    if catchData.isOneTimeCatch && ccBGSSSE001_OneTimeCaughtList.Find(catchData as form) > -1
        catchData = FindAllowedCatchDataInList(nextCatchDataSourceList)
    endIf
    return catchData
endFunction


; DINO: OnUpdate is responsible for moving through stages of the fishing event
; DINO: modified to launch new minigames instead of default behavior when the appropriate global is set
Event OnUpdate()
    while handlingInputOrUpdate
        utility.Wait(0.250000)
    endWhile
    handlingInputOrUpdate = true
    FishingDebug("Got update...")
    if !IsValidUpdateSystemState()
        FishingDebug("System state not valid for updating, exiting.")
        handlingInputOrUpdate = false
        return 
    endIf
    if nextUpdateType == UPDATETYPE_START
        FishingDebug("    ...start")
        RegisterForNextUpdate(UPDATETYPE_SEQUENCE)
    elseIf nextUpdateType == UPDATETYPE_SEQUENCE
        FishingDebug("    ...sequence")
        if !nextCatchData
            FishingDebug("    ...did not have catch data. Abort.")
            CatchFail(true, false)
        endIf
        if IsFishCatchType(nextCatchData.getCatchType())
            Float[] catchSequence = nextCatchData.getCatchSequence()
            if catchSequence
                if currentCatchSequenceIndex > catchSequence.length - 1 || catchSequence[currentCatchSequenceIndex] == 0.000000
                    currentSystemState = SYSTEMSTATE_HOOKED
                    FishingDebug("    ...caught a fish!")
                    PlayHookedFishAnimation()

                    ; DINO: launch new minigames or original catch fish prompt, depending on settings
                    ; DINO: this happens after the nibble sequence for the fish, so before the hook all is the same
                    if minigame_type != 0 && auto_reel ; DINO: minigame
                        currentSystemState = SYSTEMSTATE_CATCH_RESOLVE
                        Utility.Wait(0.5)
                        __FancyFishingManagerQuest.init_minigame(nextCatchData, self)
                    elseif auto_reel
                        currentSystemState = SYSTEMSTATE_CATCH_RESOLVE
                        if (__FancyFishingVarRodAllowCheckBypass.GetValue() > 0) || IsCatchSuccessful()
                            CatchSuccess()
                        else
                            ccBGSSSE001_fishingLostCatch.Show()
                            CatchFail(false, IsFishCatchType(nextCatchData.getCatchType()))
                        endIf
                    else
                        ShowCatchPrompt()
                        RegisterForNextUpdate(UPDATETYPE_CATCHTIMEOUT)
                    endif
                else
                    currentSystemState = SYSTEMSTATE_NIBBLE
                    FishingDebug("        ...nibble!")
                    PlayNibbleAnimation()
                    RegisterForNextUpdate(UPDATETYPE_SEQUENCE)
                    currentCatchSequenceIndex += 1
                endIf
            else
                FishingDebug("    ...failed to obtain a valid catch sequence. Abort.")
                CatchFail(true, false)
            endIf
        else
            currentSystemState = SYSTEMSTATE_HOOKED
            FishingDebug("            ...caught an object!")
            PlayHookedObjectAnimation()

            ; DINO: launch new minigame for junk catch
            if minigame_type != 0 && auto_reel ; DINO: minigame
                currentSystemState = SYSTEMSTATE_CATCH_RESOLVE
                Utility.Wait(0.5)
                __FancyFishingManagerQuest.init_minigame(nextCatchData, self)
            elseif auto_reel
                currentSystemState = SYSTEMSTATE_CATCH_RESOLVE
                if (__FancyFishingVarRodAllowCheckBypass.GetValue() > 0) || IsCatchSuccessful()
                    CatchSuccess()
                else
                    ccBGSSSE001_fishingLostCatch.Show()
                    CatchFail(false, IsFishCatchType(nextCatchData.getCatchType()))
                endIf
            else
                ShowCatchPrompt()
                RegisterForNextUpdate(UPDATETYPE_CATCHTIMEOUT)
            endif
        endIf
    elseIf nextUpdateType == UPDATETYPE_CATCHTIMEOUT && IsInExitableSystemState()
        currentSystemState = SYSTEMSTATE_CATCH_RESOLVE
        FishingDebug("        ...catch timeout!")
        ccBGSSSE001_fishingLostCatch.Show()
        CatchFail(false, IsFishCatchType(nextCatchData.getCatchType()))
    endIf
    handlingInputOrUpdate = false
endEvent

Bool function IsFishCatchType(Int aiCatchType)
    return aiCatchType <= ccBGSSSE001_CatchTypeLargeFish.GetValueInt()
endFunction

ccBGSSSE001_FishingActScript function GetCurrentFishingSupplies()
    return currentFishingSupplies
endFunction

function DoCleanupTasks()
    if currentSystemState != SYSTEMSTATE_IDLE && IsInExitableSystemState()
        CleanUp(true)
    elseIf currentSystemState == SYSTEMSTATE_IDLE
        ReturnSurroundingVolumes()
        ResumeFollowerBehavior()
        RestoreWeather()
    endIf
endFunction

Int function GetFishPopulationMorningEveningBonus()
    return utility.RandomInt(MORNINGEVENING_BONUS_MIN, MORNINGEVENING_BONUS_MAX)
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
    fishingRodPlacementMarker.Disable()
    fishingRodPlacementMarker.Delete()
    return fishingRodRef
endFunction

ccBGSSSE001_CatchData function GetNextFishCatchData(formlist akCatchDataList)
    Int sizeListIndex
    Int rarityListIndex
    ccBGSSSE001_CatchData catchData
    Float rarityRoll = utility.RandomFloat(0.000000, 1.00000)
    FishingDebug("Rarity Roll: " + rarityRoll as String)
    ;Float catchThresholdCommonFish = BASE_CATCH_THRESHOLD_COMMONFISH
    ;Float catchThresholdUncommonFish = BASE_CATCH_THRESHOLD_UNCOMMONFISH
    float catchThresholdUncommonFish = __FancyFishingVarChanceRareBase.GetValue()
    float catchThresholdCommonFish = catchThresholdUncommonFish + __FancyFishingVarChanceUncommonBase.GetValue()
    if currentFishingRodType == RODTYPE_ALIKRI || currentFishingRodType == RODTYPE_ARGONIAN
        catchThresholdCommonFish += SPECIAL_FISH_RODS_COMMONFISH_THRESHOLD_ADJUST
        catchThresholdUncommonFish += SPECIAL_FISH_RODS_UNCOMMONFISH_THRESHOLD_ADJUST
    endIf
    if GetIsMorningEvening()
        catchThresholdCommonFish += MORNING_EVENING_COMMONFISH_THRESHOLD_ADJUST
        catchThresholdUncommonFish += MORNING_EVENING_UNCOMMONFISH_THRESHOLD_ADJUST
    endIf
    if rarityRoll >= catchThresholdCommonFish
        rarityListIndex = RARITY_LIST_COMMON_INDEX
    elseIf rarityRoll >= catchThresholdUncommonFish
        rarityListIndex = RARITY_LIST_UNCOMMON_INDEX
    else
        rarityListIndex = RARITY_LIST_RARE_INDEX
    endIf
    formlist raritySubList = akCatchDataList.GetAt(rarityListIndex) as formlist
    Float catchChanceSmallFish = BASE_CATCH_THRESHOLD_SMALL * GetSmallCatchThresholdModifier()
    Float sizeRoll = utility.RandomFloat(0.000000, 1.00000)
    FishingDebug("Size Roll: " + sizeRoll as String)
    if sizeRoll >= catchChanceSmallFish
        sizeListIndex = SIZE_LIST_SMALLFISH_INDEX
    else
        sizeListIndex = SIZE_LIST_LARGEFISH_INDEX
    endIf
    formlist sizeSubList = raritySubList.GetAt(sizeListIndex) as formlist
    Int sizeSubListSize = sizeSubList.GetSize()
    if sizeSubListSize > 0
        Int resultRoll = utility.RandomInt(0, sizeSubListSize - 1)
        FishingDebug("Result Roll: " + resultRoll as String)
        catchData = sizeSubList.GetAt(resultRoll) as ccBGSSSE001_CatchData
        FishingDebug("Catch Data: " + catchData as String)
    else
        return GetNextJunkCatchData(GetJunkCatchDataList())
    endIf
    if rarityListIndex == RARITY_LIST_RARE_INDEX
        lastCatchWasRare = true
    endIf
    nextCatchDataSourceList = sizeSubList
    return catchData
endFunction


; === DINO: added variables
GlobalVariable property __FancyFishingVarMinigameType auto
GlobalVariable property __FancyFishingVarAutoReel auto
GlobalVariable property __FancyFishingVarPopReduceChance auto
GlobalVariable property __FancyFishingVarPopReduceChanceFailed auto
GlobalVariable property __FancyFishingVarRodAllowCheckBypass auto
GlobalVariable property __FancyFishingVarChanceJunkBase auto
GlobalVariable property __FancyFishingVarChanceJunkMax auto
GlobalVariable property __FancyFishingVarChanceJunkRareBase auto
GlobalVariable property __FancyFishingVarChanceJunkUncommonBase auto
GlobalVariable property __FancyFishingVarChanceRareBase auto
GlobalVariable property __FancyFishingVarChanceUncommonBase auto
GlobalVariable property __FancyFishingVarPerfectExtraJunkChance auto
GlobalVariable property __FancyFishingVarPerfectExtraJunkUseAltList auto
GlobalVariable property __FancyFishingVarBiteTimeMult auto
GlobalVariable property __FancyFishingVarDoFade auto
GlobalVariable property __FancyFishingVarAllowCameraMovement auto
LeveledItem property __FancyFishingLItemPerfectExtra auto
LeveledItem property __FancyFishingLItemDummyForFanfare auto
__FancyFishingManagerScript property __FancyFishingManagerQuest auto
int minigame_type = 0
bool auto_reel = false

; === Simple Fishing Overhaul
; base-game properties, can be filled directly
Idle Property IdleStop_Loose  Auto
Idle Property IdleSearchingChest Auto
Idle Property idlecowerexit Auto
idle property idlecoweringloose auto
; properties from SimpleFishingOverhaul.esp; leave undefined, will be filled below
GlobalVariable Property AnimatedFishing_Global Auto
GlobalVariable Property RodHeight Auto
GlobalVariable Property BaitReq Auto
GlobalVariable Property FishingCamera Auto
Message Property AnimatedFishing_NoBait Auto
Formlist Property AnimatedFishing_Bait Auto
; etc
weapon weapon1
weapon weapon2
Bool WasInThird

; - Proper Mod-Added Torch Support
Light playerTorch = none

; DINO: function called by StartPlayerInteraction to initiate the fishing process
; DINO: edited to allow switching to alternate minigames depending on global variable setting
function Fish(Bool abContinueFishing)
    ; DINO: retrieve setting for minigame style (recorded this early for purposes of skipping incorrect tutorial messages)
    minigame_type = __FancyFishingVarMinigameType.GetValueInt()
    auto_reel = (__FancyFishingVarAutoReel.GetValue() > 0)

    currentFishingSupplies.UpdateFish()
    DialogueQuest.StartUpdating()
    fishingRodActivator = PlaceFishingRodActivator(currentFishingRodType)
    FishingDebug("Placing fishing rod activator " + fishingRodActivator as String)
    SetupCameraAndPosition(abContinueFishing)
    SetVisualPopulation()
    if !auto_reel ; DINO: skip tutorial messages when automatically reeling
        self.ShowFishingTutorial()
        self.ShowReelLinePrompt()
    endif
    nextCatchData = GetNextCatchData()
    FishingDebug("Catch data for next catch")
    FishingDebug("  === Catch: " + nextCatchData.getCaughtObject() as String)
    FishingDebug("  === Catch Type: " + nextCatchData.getCatchType() as String)
    PlayCastAnimation()
    utility.Wait(DURATION_CAST)
    PlayVisualPopulationAnimation()
    RegisterForNextUpdate(UPDATETYPE_START)
    currentSystemState = SYSTEMSTATE_FISHING
endFunction

; DINO: edited to allow chance of skipping population reduce and chance of extra junk on perfect catch
function CatchSuccess()
    FishingDebug("Catch success!")
    UnregisterForUpdate()
    ccBGSSSE001_CatchSuccessSM.Play(PlayerRef as objectreference)
    PlayCatchSuccessAnimation()
    utility.Wait(DURATION_CATCH)
    form caughtObject = nextCatchData.getCaughtObject()
    Int catchType = nextCatchData.getCatchType()
    ccBGSSSE001_ITMFishUpSM.Play(PlayerRef as objectreference)
    if lastCatchWasRare
        ccBGSSSE001_RareCatchSM.Play(PlayerRef as objectreference)
    endIf
    if nextCatchData.isOneTimeCatch
        ccBGSSSE001_OneTimeCaughtList.AddForm(nextCatchData as form)
    endIf

    FanfareCaughtObject02 = none
    FanfareNodeName02 = ""

    bool is_fish = IsFishCatchType(catchType)

    if is_fish
        ; DINO: roll for extra junk if desired
        float extra_junk_chance = __FancyFishingVarPerfectExtraJunkChance.GetValue()
        if catch_was_perfect && ((extra_junk_chance >= 1.0) || ((extra_junk_chance > 0) && (Utility.RandomFloat() <= extra_junk_chance)))
            if (__FancyFishingVarPerfectExtraJunkUseAltList.GetValue() == 0)
                ccBGSSSE001_CatchData junkCatchData = GetNextJunkCatchData(GetJunkCatchDataList())
                FanfareCaughtObject02 = junkCatchData.getCaughtObject()
                FanfareNodeName02 = junkCatchData.successNodeName
            else
                FanfareCaughtObject02 = __FancyFishingLItemPerfectExtra
                FanfareNodeName02 = "SuccessNodeItemJunkSmall01" ; all items in this list are pretty small
            endif
        endif
    endIf

    ShowFanfareScreenAndAddCaughtItem(caughtObject)
    FanfareCaughtObject02 = none ; DINO: reset these in case it's called elsewhere by someone else
    FanfareNodeName02 = ""

    if is_fish
        TryToStartQuestAfterFirstCatch()
        currentFishingSupplies.UpdateFishCatchSuccess()

        ; DINO: population reduction as chance instead of always
        if should_reduce_pop(false)
            currentFishingSupplies.ReduceFishPopulation(1)
        endif
    endif

    if isQuestItemCatch as Bool && currentFishingSupplies.myQuestStageToSet != -1
        currentFishingSupplies.myQuest.SetStage(currentFishingSupplies.myQuestStageToSet)
    endIf
    if RadiantFishEventListener as Bool && RadiantFishEventListener.FishingSpot.GetReference() == currentFishingSupplies as objectreference
        RadiantFishEventListener.CatchEvent(caughtObject, catchType)
    endIf
    CleanUp(false)
endFunction

; DINO: reduction chance helper
bool Function should_reduce_pop(bool catch_failed)
    float reduce_chance
    if catch_failed
        reduce_chance = __FancyFishingVarPopReduceChanceFailed.GetValue()
    else
        reduce_chance = __FancyFishingVarPopReduceChance.GetValue()
    endif
    return (reduce_chance >= 1.0) || ((reduce_chance > 0) && (reduce_chance > Utility.RandomFloat()))
endFunction

Float function GetFishPopulationJunkModifier()
    Int currentPopulation = currentFishingSupplies.GetCurrentFishPopulation()
    if currentPopulation >= POPULATION_COUNT_FULL
        return 1.00000
    elseIf currentPopulation < POPULATION_COUNT_FULL && currentPopulation > 0
        return 2.00000
    else
        return 100.000
    endIf
endFunction

; DINO: edited for compat with:
;   - Simple Fishing Overhaul
function PlayHookedFishAnimation()
    if AnimatedFishing_Global
        AnimatedFishing_Global.SetValue(2)
        Utility.Wait(0.1)

        If PlayerRef.GetAnimationVariableBool("IsFirstPerson")
        else
            playerref.playidle(idlesearchingchest)
        endif
    endif

    fishingRodActivator.SetAnimationVariableFloat(LINETUG_ANIMVAR, LINETUG_TYPE_TUGFISH)
    fishingRodActivator.PlayAnimation(NIBBLE_ANIM)
    utility.Wait(DURATION_HOOKED_ANIM_WAIT)
    Int catchType = nextCatchData.getCatchType()
    fishingRodActivator.PlayAnimation(LINETUG_FISH_ANIM)
    if catchType == ccBGSSSE001_CatchTypeSmallFish.GetValueInt()
        game.ShakeController(RUMBLE_STRENGTH_HOOKEDSMALLFISH_LEFT, RUMBLE_STRENGTH_HOOKEDSMALLFISH_RIGHT, RUMBLE_DURATION_HOOKED)
        utility.Wait(RUMBLE_DURATION_HOOKED - 0.100000)
        game.ShakeController(RUMBLE_STRENGTH_HOOKED_LEFTCONSTANT, RUMBLE_STRENGTH_HOOKED_RIGHTCONSTANT, RUMBLE_DURATION_HOOKEDCONSTANT)
    elseIf catchType == ccBGSSSE001_CatchTypeLargeFish.GetValueInt()
        game.ShakeController(RUMBLE_STRENGTH_HOOKEDLARGEFISH_LEFT, RUMBLE_STRENGTH_HOOKEDLARGEFISH_RIGHT, RUMBLE_DURATION_HOOKED)
        utility.Wait(RUMBLE_DURATION_HOOKED - 0.100000)
        game.ShakeController(RUMBLE_STRENGTH_HOOKEDLARGEFISH_LEFTCONSTANT, RUMBLE_STRENGTH_HOOKEDLARGEFISH_RIGHTCONSTANT, RUMBLE_DURATION_HOOKEDCONSTANT)
    endIf
endFunction

function OnFishingTriggerActivated()
    FishingDebug("OnFishingTriggerActivated, currentSystemState " + currentSystemState as String)
    if currentSystemState == SYSTEMSTATE_FISHING || currentSystemState == SYSTEMSTATE_NIBBLE || currentSystemState == SYSTEMSTATE_HOOKED
        ReelLine()
    elseIf currentSystemState == SYSTEMSTATE_IDLE
        StartPlayerInteraction(currentFishingSupplies, true)
    endIf
endFunction

; DINO: edited to allow chance of skipping population reduce
function CatchFail(Bool abFastExit, Bool abReduceFishPopulation)
    FishingDebug("Catch failure, exit!")
    if abFastExit
        PlayFastExitAnimation()
        utility.Wait(DURATION_FASTEXIT)
    else
        PlayCatchFailureAnimation()
        utility.Wait(DURATION_CATCH)
    endIf

    ; DINO: population reduction as chance instead of always
    if abReduceFishPopulation && should_reduce_pop(true)
        currentFishingSupplies.ReduceFishPopulation(1)
    endIf
    CleanUp(false)
endFunction

; Proper Mod-Added Torch Support: edited to re-equip mod-added torch if used
function CleanUp(Bool abFastExit)
    FishingDebug("Cleaning up...")
    currentSystemState = SYSTEMSTATE_CLEANUP
    UnregisterForUpdate()
    DialogueQuest.StopUpdating()
    CleanUpFishingRodActivator(abFastExit)
    if abFastExit
        ReturnSurroundingVolumes()
        ResumeFollowerBehavior()
        RestoreWeather()
    endIf
    lastFishingSupplies = currentFishingSupplies
    if startedWithTorch && PlayerRef.GetItemCount(playerTorch)
        PlayerRef.EquipItemEx(playerTorch, 0, false, true)
    endIf
    RestoreCameraAndControls(abFastExit)
    ClearFishingAttemptVariables()
    currentSystemState = SYSTEMSTATE_IDLE
    FishingDebug("Done!")
endFunction

Bool function PlayerHasCaughtFishBefore()
    return ccBGSSSE001_HasCaughtFishAtLeastOnce.GetValueInt() != 0
endFunction

function ClearFishingSessionVariables()
    startedInFirstPerson = false
    startedWithTorch = false
    playerTorch = none
    fishingRodActivator = none
    nextUpdateType = 0
    currentFishingRodType = -1
endFunction

function FishingDebug(String asMessage)
    if debugEnabled
        debug.trace(asMessage, 0)
    endIf
endFunction

function RegisterForUpdateNextMorning()
    Float hoursUntilMorning
    FishingDebug("Fishing Supplies " + self as String + " RegisterForUpdateNextMorning()")
    if currentGameHour <= GAMETIME_MORNING
        hoursUntilMorning = GAMETIME_MORNING - currentGameHour
    else
        hoursUntilMorning = 24.0000 - currentGameHour + GAMETIME_MORNING
    endIf
    currentFishingSupplies.RegisterForPopulationUpdate(hoursUntilMorning)
endFunction

function TryToStartQuestAfterFirstCatch()
    if !PlayerHasCaughtFishBefore()
        ccBGSSSE001_HasCaughtFishAtLeastOnce.SetValueInt(1)
        ccBGSSSE001_Start_MQ2.Start()
    endIf
endFunction

function CheckEnableDebug()
    if ccBGSSSE001_FishingDebugEnabled.GetValueInt() == 1
        debugEnabled = true
    else
        debugEnabled = false
    endIf
endFunction

Float function GetInitialWaitingPeriod()
    return DURATION_INITIAL_WAITING_PERIOD + utility.RandomFloat(-DURATION_INITIAL_WAITING_PERIOD_VARIANCE, DURATION_INITIAL_WAITING_PERIOD_VARIANCE)
endFunction

formlist function GetJunkCatchDataList()
    formlist overrideJunkList = currentFishingSupplies.myOverrideJunkCatchDataList
    if overrideJunkList
        return overrideJunkList
    else
        return ccBGSSSE001_JunkCatchDataListDefault
    endIf
endFunction

Bool function IsInExitableSystemState()
    return currentSystemState != SYSTEMSTATE_CATCH_RESOLVE && currentSystemState != SYSTEMSTATE_CLEANUP
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

Bool function IsValidUpdateSystemState()
    return currentSystemState >= SYSTEMSTATE_FISHING && currentSystemState <= SYSTEMSTATE_HOOKED
endFunction

function MovePlayerToFishingMarker()
    PlayerRef.MoveTo(currentFishingSupplies.GetFishingMarker(), 0.000000, 0.000000, 0.000000, true)
endFunction

Int function GetFishBasePopulation()
    return BASE_POPULATION + utility.RandomInt(BASE_BONUS_MIN, BASE_BONUS_MAX)
endFunction

function UnregisterRadiantFishEventListener()
    RadiantFishEventListener = none
endFunction

function OnPlayerHit()
    DoCleanupTasks()
endFunction

; DINO: modified to add a multiplier to wait time
function RegisterForNextUpdate(Int aiUpdateType)
    nextUpdateType = aiUpdateType
    FishingDebug("Registering for next update...")
    if aiUpdateType == UPDATETYPE_SETQUESTSTAGE
        FishingDebug("    ...set quest stage")
        RegisterForSingleUpdate(0.0100000)
    elseIf aiUpdateType == UPDATETYPE_START
        FishingDebug("    ...start")
        RegisterForSingleUpdate(GetInitialWaitingPeriod()*__FancyFishingVarBiteTimeMult.GetValue())
    elseIf aiUpdateType == UPDATETYPE_SEQUENCE
        FishingDebug("    ...sequence")
        if nextCatchData as Bool && IsFishCatchType(nextCatchData.getCatchType())
            RegisterForSingleUpdate(nextCatchData.getCatchSequence()[currentCatchSequenceIndex]*__FancyFishingVarBiteTimeMult.GetValue())
        else
            RegisterForSingleUpdate(DURATION_JUNKITEMCATCHTIME*__FancyFishingVarBiteTimeMult.GetValue())
        endIf
    elseIf aiUpdateType == UPDATETYPE_CATCHTIMEOUT
        FishingDebug("    ...catch timeout")
        RegisterForSingleUpdate(DURATION_CATCHTIMEOUT)
    endIf
endFunction

; DINO: edit to launch minigame as necessary & bypass rod checks if desired
function ReelLine()
    if !handlingInputOrUpdate
        handlingInputOrUpdate = true
        if currentSystemState == SYSTEMSTATE_NIBBLE
            currentSystemState = SYSTEMSTATE_CATCH_RESOLVE
            ccBGSSSE001_fishingEarlyReelNibble.Show()
            CatchFail(true, true)
        elseIf currentSystemState == SYSTEMSTATE_HOOKED
            currentSystemState = SYSTEMSTATE_CATCH_RESOLVE

            ; DINO - handle minigame if auto-reel isn't on
            if minigame_type != 0 ; DINO: minigame
                __FancyFishingManagerQuest.init_minigame(nextCatchData, self)
            else
                if (__FancyFishingVarRodAllowCheckBypass.GetValue() > 0) || IsCatchSuccessful()
                    CatchSuccess()
                else
                    ccBGSSSE001_fishingLostCatch.Show()
                    CatchFail(false, IsFishCatchType(nextCatchData.getCatchType()))
                endIf
            endif
        elseIf currentSystemState == SYSTEMSTATE_CATCH_RESOLVE
            
        else
            currentSystemState = SYSTEMSTATE_CATCH_RESOLVE
            ccBGSSSE001_fishingEarlyReel.Show()
            CatchFail(true, false)
        endIf
        handlingInputOrUpdate = false
    endIf
endFunction

; DINO: new function for minigames to report result
; DINO: not checking handlingInputOrUpdate here because this function will be called once only.
bool catch_was_perfect
Function reel_line_known_result(bool success, bool terminated_early, bool perfect, bool is_fish)
    handlingInputOrUpdate = true
    currentSystemState = self.SYSTEMSTATE_CATCH_RESOLVE
    catch_was_perfect = perfect
    if success
        CatchSuccess()
    elseif terminated_early
        ccBGSSSE001_fishingEarlyReelNibble.Show()
        CatchFail(true, is_fish)
    else
        ccBGSSSE001_fishingLostCatch.Show()
        CatchFail(true, is_fish)
    endif
    catch_was_perfect = false ; DINO: reset, to avoid impacting things if this function is called from elsewhere
    handlingInputOrUpdate = false
endFunction

function CleanUpFishingRodActivator(Bool abFastExit)
    if fishingRodActivator
        if abFastExit
            fishingRodActivator.PlayAnimation(FASTEXIT_ANIM)
            utility.Wait(DURATION_FASTEXIT)
        endIf
        fishingRodActivator.Disable()
        fishingRodActivator.Delete()
        fishingRodActivator = none
    endIf
endFunction

function PlayNibbleAnimation()
    currentFishingSupplies.UpdateNibble()
    fishingRodActivator.PlayAnimation(NIBBLE_ANIM)
    Int catchType = nextCatchData.getCatchType()
    if catchType == ccBGSSSE001_CatchTypeSmallFish.GetValueInt()
        game.ShakeController(RUMBLE_STRENGTH_NIBBLESMALL_LEFT, RUMBLE_STRENGTH_NIBBLESMALL_RIGHT, RUMBLE_DURATION_NIBBLE)
    elseIf catchType == ccBGSSSE001_CatchTypeLargeFish.GetValueInt()
        game.ShakeController(RUMBLE_STRENGTH_NIBBLELARGE_LEFT, RUMBLE_STRENGTH_NIBBLELARGE_RIGHT, RUMBLE_DURATION_NIBBLE)
    endIf
endFunction

function ClearFishingAttemptVariables()
    nextCatchData = none
    currentCatchSequenceIndex = 0
    lastCatchWasRare = false
    isQuestItemCatch = false
    forcedRainWeather = false
endFunction

function TryToApplyRainWeather()
    Int biomeType = currentFishingSupplies.biomeType
    if PlayerRef.WornHasKeyword(ccBGSSSE001_SummonsRain) && (biomeType == BIOME_TYPE_STREAM || biomeType == BIOME_TYPE_LAKE)
        forcedRainWeather = true
        weather currentWeather = weather.GetCurrentWeather()
        if currentWeather.GetClassification() != 2
            previousWeather = currentWeather
            SkyrimStormRain.SetActive(true, true)
        endIf
    endIf
endFunction

; DINO: edited for compat with:
;   - Simple Fishing Overhaul
function PlayHookedObjectAnimation()
    if AnimatedFishing_Global
        AnimatedFishing_Global.SetValue(2)
        Utility.Wait(0.1)

        If PlayerRef.GetAnimationVariableBool("IsFirstPerson")
            ;Debug.SendanimationEvent(PlayerRef, "Idleboundkneesenterinstant")
        else
            playerref.playidle(idlesearchingchest)
        endif
    endif

    fishingRodActivator.SetAnimationVariableFloat(LINETUG_ANIMVAR, LINETUG_TYPE_TUGOBJECT)
    fishingRodActivator.PlayAnimation(NIBBLE_ANIM)
    utility.Wait(DURATION_HOOKED_ANIM_WAIT)
    fishingRodActivator.PlayAnimation(LINETUG_OBJECT_ANIM)
    game.ShakeController(RUMBLE_STRENGTH_HOOKEDOBJECT_LEFT, RUMBLE_STRENGTH_HOOKEDOBJECT_RIGHT, RUMBLE_DURATION_HOOKED)
    utility.Wait(RUMBLE_DURATION_HOOKED - 0.100000)
    game.ShakeController(RUMBLE_STRENGTH_HOOKED_LEFTCONSTANT, RUMBLE_STRENGTH_HOOKED_RIGHTCONSTANT, RUMBLE_DURATION_HOOKEDCONSTANT)
endFunction

; DINO: edited for compat with:
;   - Simple Fishing Overhaul
function PlayCastAnimation()
    if AnimatedFishing_Global
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
    endif

    fishingRodActivator.PlayAnimation(CAST_ANIM)
    game.ShakeController(RUMBLE_STRENGTH_CAST_LEFT, RUMBLE_STRENGTH_CAST_RIGHT, RUMBLE_DURATION_CAST)

    if AnimatedFishing_Global
        Utility.wait(0.2)
        float height = PlayerRef.GetPositionZ()
        fishingRodActivator.TranslateTo(fishingRodActivator.GetPositionX(), fishingRodActivator.GetPositionY(), (height - RodHeight.GetValue()), fishingRodActivator.GetAngleX(), fishingRodActivator.GetAngleY(), fishingRodActivator.GetAngleZ(), 2000.00, 2000.00)
    endif
endFunction

function ShowReelLinePrompt()
    if ccBGSSSE001_ShowedReelPromptThisSession.GetValueInt() == 0
        ccBGSSSE001_ShowedReelPromptThisSession.SetValueInt(1)
        message.ResetHelpMessage("Activate")
        ccBGSSSE001_ReelLinePrompt.ShowAsHelpMessage("Activate", 5 as Float, 30 as Float, 1)
    endIf
endFunction

; DINO: edited for compat with:
;   - Simple Fishing Overhaul
function RestoreCameraAndControls(Bool abFastExit)
    if AnimatedFishing_Global
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
        TrueDirectionalMovement.ToggleDisableHeadtracking(False, False)
    else
        game.EnablePlayerControls()
        if !startedInFirstPerson
            game.ForceThirdPerson()
        endIf
    endif
endFunction

function ResetSystem()
    ClearFishingSessionVariables()
    ClearFishingAttemptVariables()
endFunction

function ShowFishingTutorial()
    if ccBGSSSE001_FishingTutorialDisplayed.GetValueInt() == 0
        ccBGSSSE001_FishingTutorialDisplayed.SetValueInt(1)
        ccBGSSSE001_FishingTutorial.Show()
    endIf
endFunction

Bool function IsItemCatchType(Int aiCatchType)
    return aiCatchType == ccBGSSSE001_CatchTypeObject.GetValueInt()
endFunction

function PlayFastExitAnimation()
    fishingRodActivator.PlayAnimation(FASTEXIT_ANIM)
endFunction

; DINO: edited to allow showing a second fanfare screen on adding extra junk w/ perfect catch
; and to prevent getting stuck on the fanfare screen if the base item is empty 
;   (such as if a mod gives chances for items to be pulled from vanilla leveled lists, and then another mod changes those lists to have a nonzero chance none-- saw this reported for From Rods To Riches, probably in combination with Scarcity)
;   it'll still be weird for the player, with fishing ending abruptly and nothing being added, but it'll no longer lock on the wait loop and require a reload.
;Message property __FancyFishingMessageNoCatch auto
; also for compat with:
;   - Simple Fishing Overhaul
;   - Fishing Preview
; optional second item stored here in place of args to avoid changing function interface
; (in case someone calls this from elsewhere for another mod)
Form FanfareCaughtObject02
string FanfareNodeName02
function ShowFanfareScreenAndAddCaughtItem(form akCaughtObject)
    
    SetFishCaught(akCaughtObject)
    Form FishingPreviewQuest = Game.GetFormFromFile(0x800, "FishingPreview.esp")
    If FishingPreviewQuest
        if currentSystemState != SYSTEMSTATE_CATCH_RESOLVE
            game.EnablePlayerControls(true, true, true, true, true, true, true, true, 0)
        endIf
        if FanfareCaughtObject02
            ; this is kind of an inelegant workaround but should get things working
            __FancyFishingLItemDummyForFanfare.Revert()
            __FancyFishingLItemDummyForFanfare.AddForm(akCaughtObject, 1, 1)
            __FancyFishingLItemDummyForFanfare.AddForm(FanfareCaughtObject02, 1, 1)
            (FishingPreviewQuest As FishingPreviewQuestScript).ShowPreview(__FancyFishingLItemDummyForFanfare)
        else
            (FishingPreviewQuest As FishingPreviewQuestScript).ShowPreview(akCaughtObject)
        endif
        Return
    EndIf

    game.DisablePlayerControls(true, true, true, true, true, true, true, true, 0)

    if AnimatedFishing_Global
        WasInThird = False

        If !PlayerRef.GetAnimationVariableBool("IsFirstPerson") && FishingCamera.GetValue() < 2
            if __FancyFishingVarDoFade.GetValue() > 0
                ccBGSSSE001_FadeToBlackImod.ApplyCrossFade(0.4)
                utility.Wait(0.2)
                ccBGSSSE001_FadeToBlackHoldImod.PopTo(ccBGSSSE001_FadeToBlackBackImod, 1.20000)
            endif
            WasInThird = True
            game.ForceFirstPerson()
        endif

        If FishingCamera.GetValue() < 2
            PlayerRef.SetAngle(fishingRodActivator.GetAngleX(), fishingRodActivator.GetAngleY(), fishingRodActivator.GetAngleZ())
        endif
    endif

    ccBGSSSE001_CatchSuccessDOF.Apply(1.00000)

    objectreference catchRef = currentFishingSupplies.PlaceAtMe(akCaughtObject, 1, false, false)
    objectreference catchRef02
    if FanfareCaughtObject02
        catchRef02 = currentFishingSupplies.PlaceAtMe(FanfareCaughtObject02, 1, false, false)
    endif

    while (catchRef && !catchRef.Is3DLoaded()) || (catchRef02 && !catchRef02.Is3DLoaded())
        utility.Wait(0.200000)
    endWhile

    if catchRef
        catchRef.SetMotionType(catchRef.Motion_Keyframed, true)
        catchRef.Disable()
    endif

    if catchRef02
        catchRef02.SetMotionType(catchRef02.Motion_Keyframed, true)
        catchRef02.Disable()
    endif

    objectreference fishingMarker = currentFishingSupplies.GetFishingMarker()
    objectreference fanfareLight = currentFishingSupplies.PlaceAtMe(ccBGSSSE001_CatchSuccessLight as form, 1, false, true)
    fanfareLight.MoveToNode(fishingMarker, "LightNode")
    fanfareLight.EnableNoWait()
    if catchRef
        catchRef.MoveToNode(fishingMarker, nextCatchData.successNodeName)
        catchRef.EnableNoWait(true)
        PlayerRef.AddItem(catchRef.GetBaseObject(), 1, false)
        utility.Wait(DURATION_SUCCESSVIEW)
        catchRef.DisableNoWait()
        catchRef.Delete()
    endif

    if catchRef02
        catchRef02.MoveToNode(fishingMarker, FanfareNodeName02)
        catchRef02.EnableNoWait(true)
        PlayerRef.AddItem(catchRef02.GetBaseObject(), 1, false)
        Utility.Wait(DURATION_SUCCESSVIEW)
        catchRef02.DisableNoWait()
        catchRef02.Delete()
    endif

    fanfareLight.DisableNoWait()
    fanfareLight.Delete()
    ccBGSSSE001_CatchSuccessDOF.Remove()

    if !catchRef && !catchRef02
        ccBGSSSE001_fishingEarlyReel.Show()
        ;__FancyFishingMessageNoCatch.Show()
    endif

    if currentSystemState != SYSTEMSTATE_CATCH_RESOLVE
        game.EnablePlayerControls()
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
    return RODTYPE_NONE
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
