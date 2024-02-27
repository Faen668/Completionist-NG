Scriptname Completionist_Native Hidden

; Gets the Current Plugin Version As A Full String e.g "1.0.1.2"
String Function GetVersion() Global Native

Function SetFrameworkQuest(Quest a_quest) Global Native

Function LogWithPlugin(string a_log) Global Native

; Returns the specified Form Array from the ID.
Form[] Function GetFormArrayByID(Int a_frameworkID) Global Native

; Returns the specified Bool Array from the ID.
Bool[] Function GetBoolArrayByID(Int a_frameworkID) Global Native

; Returns the specified Name Array from the ID.
String[] Function GetNameArrayByID(Int a_frameworkID) Global Native
String[] Function qGetNameArrayByName(String a_page) Global Native
String[] Function qGetKeysArrayByName(String a_page) Global Native

; Returns the specified Highlight Array from the ID.
String[] Function GetTextArrayByID(Int a_frameworkID) Global Native
String[] Function qGetTextArrayByName(String a_page) Global Native

; Returns the specified total integer from the ID.
Int Function GetEntries_TotalByID(Int a_frameworkID) Global Native

; Returns the specified total found integer Array from the ID.
Int Function GetEntries_FoundByID(Int a_frameworkID) Global Native

; Returns the specified completion status of the option from the ID.
Int Function IsOptionCompleted(Int a_frameworkID, Form a_form) Global Native

; Returns the specified completion status of the option from the ID.
Function SetOptionCompleted(Int a_frameworkID, Form a_form) Global Native
Function qSetOptionCompleted(Int a_frameworkID, string a_uniqueKey, bool b_completed) Global Native

Bool Function qIsOptionToggled(Int a_frameworkID, string a_uniqueKey) Global Native
Int Function qIsOptionCompleted(Int a_frameworkID, string a_uniqueKey) Global Native

; Returns the specified Unique Identifier Array from the ID.
String[] Function qGetIdenArrayByName(String a_page) Global Native

; Returns the specified Radiant Array from the ID.
Int[] Function qGetRadiArrayByName(String a_page) Global Native

String function qGetTimesCompletedVsTimesRequiredText(String a_key) Global Native

; Updates Pet Ownership and Shouut Completion in the DLL Frameworks.
Function Framework_UpdatePetOwnership(ActorBase a_actorBase) Global Native
Function Framework_UpdateShouts() Global Native

;Print Message from Plugin with colour formatting.
Function SendNotification(String a_message, String a_Colour, Bool a_enabled) Global Native

;Returns the Hex String of an Int.
String Function GetHexValue(Int a_decimal) Global Native

;Sends perk activated to the plugin for processing (only works when survival mode is active)
Function ActivateShrineByID(Form a_form) Global Native

;Checks the current cell for collectable items.
Function CheckForReferences(Cell a_cell, bool b_log, bool b_notify) Global Native
ObjectReference Function GetTargetReferenceRefr(Cell a_cell, Form a_lastForm) Global Native
String Function GetTargetReferenceType() Global Native
String Function GetTargetReferenceName() Global Native
Form Function GetTargetReferenceForm() Global Native

Bool Function HasPinnedFormInCell(Cell a_cell, Form a_form) Global Native
Bool Function IsItemPinnable(Form a_form) Global Native
ObjectReference Function GetPinnedReferenceRefr(Form a_form) Global Native
String Function GetPinnedReferenceType(Form a_form) Global Native
String Function GetPinnedReferenceName(Form a_form) Global Native

String Function GetQuestMarkerReferenceFormID(ObjectReference a_ref) Global Native
String Function GetQuestMarkerReferenceOwner(ObjectReference a_ref) Global Native
String Function GetQuestMarkerReferenceIndex(ObjectReference a_ref) Global Native

Function ExcludeReference(ObjectReference a_ref, Cell a_cell) Global Native
Function RemoveExcludedReference(ObjectReference a_ref) Global Native
Function IsFormExcludable(ObjectReference a_ref) Global Native

String[] Function GetReferenceFormIDs(Cell a_cell) Global Native
String[] Function GetReferenceNames(Cell a_cell) Global Native
ObjectReference[] Function GetObjectReferences(Cell a_cell) Global Native
bool Function isCellExcluded(cell a_cell) Global Native

;Forces the DLL to load changed varaibles.
Function UpdateVariables() Global Native

; Dumps all quest data to completionist log.
Function qDumpQuestData() Global Native

; Mercahnt registration functions for internal processing of favor quests.
Function RegisterMerchant(Actor a_actor) Global Native
Function UnRegisterMerchant(Actor a_actor) Global Native

; Returns search results from the Quests MCM.
String[] Function SearchAndReportPage(string query, bool ignoreCompleted = false, int maxResults, int searchType) Global Native

; Native localisation lookup from CVariables::Localised_Map.
String Function GetLocStringByKeyExt(string s_str) Global Native

; Returns the _modconfigs position for any given MCM page.
int Function GetSkyUIMCMPositionalIndex(string s_str) Global Native

; Returns the unique frameowrk identifier for any given MCM page.
Int Function GetMCMPageIdentifierFromName(string s_str) Global Native

Bool Function IsInActualMenuMode() Global Native

; Returns true if shout a begins with shout b when strippeed to substring and font removed.
Bool Function IsMatchingShout(string a, string b) Global Native

Function BuildMCMPages() Global Native
String[] Function GetMCMPages(int menu_identifier) Global Native
Int Function GetValidMainPatchPageID(string mcmpage) Global Native
Int Function GetValidMiscPatchPageIDForItems(string mcmpage) Global Native
Int Function GetValidMiscPatchPageIDForBooks(string mcmpage) Global Native
Int Function GetValidMiscPatchPageIDForMapMa(string mcmpage) Global Native
Int Function GetValidMiscPatchPageIDForEncha(string mcmpage) Global Native
String[] Function GetPageConfiguration(string mcmpage, int pageNumber) Global Native
String[] Function SearchMultiPage(string page, string query, bool ignoreCompleted = false, int maxResults, int searchType) Global Native

Int Function GetQuestID(string mcmpage, int activePage) Global Native
Int Function GetMultiPageCount(string mcmpage) Global Native
bool function IsMultiPage(string mcmpage) Global Native

Int Function GetTotalEntriesForPage(string mcmpage) Global Native
Int Function GetTotalEntriesFoundForPage(string mcmpage) Global Native
Int Function GetPageNumberForForm(string mcmpage, string formName) Global Native
Int FUnction GetPageNumberForSection(string mcmpage, string sectionName) Global Native

String[] Function qGetMiscQuestNameArrayByID(int patchID) Global Native
String[] Function qGetMiscQuestTextArrayByID(int patchID) Global Native
String[] Function qGetMiscQuestIdenArrayByID(int patchID) Global Native
Int[] Function qGetMiscQuestRadiArrayByID(int patchID) Global Native
String[] Function qGetMiscQuestKeysArrayByID(int patchID) Global Native

Int Function GetActivePage(string mcmpage) Global Native
Function SetActivePage(string mcmpage, int a_value) Global Native

Int Function GetDefaultPage(string mcmpage) Global Native
Function SetDefaultPage(string mcmpage, int a_value) Global Native

Bool Function GetUseDefaultPage(string mcmpage) Global Native
Function SetUseDefaultPage(string mcmpage, bool a_value) Global Native
Function ResetPageSettings(string mcmpage) Global Native

Function AddSearchTerm(string mcmpage, string a_value) Global Native
String[] Function GetSearchHistory(string mcmpage) Global Native
Function ClearSearchHistory(string mcmpage) Global Native

String[] Function GetMultiPageSplashScreenConfig(string mcmpage) Global Native

bool function IsSettingsPage(string mcmpage) Global Native
bool function GetHeaderRequired(string mcmpage, int header) Global Native
string function GetHeader(string mcmpage, int header, bool _left) Global Native
string function GetActivePageName(int activePage, string mcmpage) Global Native

; return the length of the string
int Function GetStringLength(string s) Global Native

; Logging Data.
String[] Function GetLoggingDates() Global Native
String[] Function GetLoggedEventsForDate(string s_date, bool b_prefix, bool b_colour, string qc, string ic, string bc, string sc) Global Native

bool Function MapMarkerIsCleared(form a_marker) Global Native
bool Function IsItemKnownExternal(form a_form) Global Native

Function SetFishCaught(form a_fish) Global Native

String[] Function GetPlayerKillNames()  Global Native
int Function GetPlayerKillCount(string a_name)  Global Native
Function ResetPlayerKill(string a_name)  Global Native
Function RemovePlayerKill(string a_name)  Global Native
string Function GetDeathString(string a_name)  Global Native
string Function GetCombinedKillString(string a_name)  Global Native

int function GetPatchCount() Global Native
