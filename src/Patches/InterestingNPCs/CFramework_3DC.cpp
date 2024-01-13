#include "Serialization.hpp"
#include "CFramework_3DC.hpp"
#include "Internal Utility/Events.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

#undef AddForm

namespace CPatch_3DC 
{
	using namespace CFramework_Master;

	CQuestData Quest1_Data[]
	{
		/*00*/ {"3DNPC_Main_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "DJGQuest"},
		/*01*/ {"3DNPC_Main_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "ErevanQuest"},
		/*02*/ {"3DNPC_Main_Quest02", CFlagEnum::kMain, CCompEnum::kStand, "DJGQuest2"},
		/*03*/ {"3DNPC_Main_Quest03", CFlagEnum::kMain, CCompEnum::kStand, "AmaleeQuest"},
		/*04*/ {"3DNPC_Main_Quest04", CFlagEnum::kMain, CCompEnum::kStand, "hagquest"},
		/*05*/ {"3DNPC_Main_Quest05", CFlagEnum::kMain, CCompEnum::kStand, "RaynesGriffithQuest"},
		/*06*/ {"3DNPC_Main_Quest06", CFlagEnum::kMain, CCompEnum::kStand, "CWQQ2"},
		/*07*/ {"3DNPC_Main_Quest07", CFlagEnum::kMain, CCompEnum::kStand, "AnumLaQuest"},
		/*08*/ {"3DNPC_Main_Quest08", CFlagEnum::kMain, CCompEnum::kStand, "AmicusDialogue"},
		/*09*/ {"3DNPC_Main_Quest09", CFlagEnum::kMain, CCompEnum::kStand, "GorrQuest"},
		/*10*/ {"3DNPC_Main_Quest10", CFlagEnum::kMain, CCompEnum::kStand, "warrensquest"},
		/*11*/ {"3DNPC_Main_Quest11", CFlagEnum::kMain, CCompEnum::kStand, "DialogueNair"},
		/*12*/ {"3DNPC_Main_Quest12", CFlagEnum::kMain, CCompEnum::kStand, "ZoraQuest"},
		/*13*/ {"3DNPC_Main_Quest13", CFlagEnum::kMain, CCompEnum::kStand, "Herranquest"},
		/*14*/ {"3DNPC_Main_Quest14", CFlagEnum::kMain, CCompEnum::kStand, "ArilwaenQuest"},
		/*15*/ {"3DNPC_Main_Quest15", CFlagEnum::kMain, CCompEnum::kStand, "DialogueJasparGaerston"},
		/*16*/ {"3DNPC_Main_Quest16", CFlagEnum::kMain, CCompEnum::kStand, "RDQuest"},
		/*17*/ {"3DNPC_Main_Quest17", CFlagEnum::kMain, CCompEnum::kStand, "AsteriaQuest"},
		/*18*/ {"3DNPC_Main_Quest18", CFlagEnum::kMain, CCompEnum::kStand, "RumarinQuest2"},
		/*19*/ {"3DNPC_Main_Quest19", CFlagEnum::kMain, CCompEnum::kStand, "AldiQuest"},
		/*20*/ {"3DNPC_Main_Quest20", CFlagEnum::kMain, CCompEnum::kStand, "SvashniQuest"},
		/*21*/ {"3DNPC_Main_Quest21", CFlagEnum::kMain, CCompEnum::kStand, "HorkerQuest"},
		/*22*/ {"3DNPC_Main_Quest22", CFlagEnum::kMain, CCompEnum::kStand, "KjoliQuest"},
		/*23*/ {"3DNPC_Main_Quest23", CFlagEnum::kMain, CCompEnum::kStand, "ViranyaQuest"},
	};

	CQuestData Quest2_Data[]
	{
		/*00*/ {"3DNPC_BOK_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "3DNPCMQ1"},
		/*01*/ {"3DNPC_BOK_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "3DNPCMQ2"},
		/*02*/ {"3DNPC_BOK_Quest02", CFlagEnum::kMain, CCompEnum::kStand, "3DNPCMQ3"},
		/*03*/ {"3DNPC_BOK_Quest03", CFlagEnum::kMain, CCompEnum::kStand, "3DNPCMQ4"},
		/*04*/ {"3DNPC_BOK_Quest04", CFlagEnum::kMain, CCompEnum::kStand, "3DNPCMQ5"},
		/*05*/ {"3DNPC_BOK_Quest05", CFlagEnum::kMain, CCompEnum::kStand, "3DNPCMQ6"},
		/*06*/ {"3DNPC_BOK_Quest06", CFlagEnum::kMain, CCompEnum::kStand, "3DNPCMQ7"},
	};

	CQuestData Quest3_Data[]
	{
		/*00*/ {"3DNPC_DS_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "3dmcue"},
		/*01*/ {"3DNPC_DS_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "3dmcue2"},
		/*02*/ {"3DNPC_DS_Quest02", CFlagEnum::kMain, CCompEnum::kStand, "3dmcue3"},
		/*03*/ {"3DNPC_DS_Quest03", CFlagEnum::kMain, CCompEnum::kStand, "3dmcue4"},
		/*04*/ {"3DNPC_DS_Quest04", CFlagEnum::kMain, CCompEnum::kStand, "3dmcue5"},
		/*05*/ {"3DNPC_DS_Quest05", CFlagEnum::kMain, CCompEnum::kStand, "3dmcue6"},
	};

	CQuestData Quest4_Data[]
	{
		/*00*/ {"3DNPC_Misc_Quest00", CFlagEnum::kSide, CCompEnum::kStand, "Jurykquest"},
		/*01*/ {"3DNPC_Misc_Quest01", CFlagEnum::kSide, CCompEnum::kStand, "Rumarinquest"},
		/*02*/ {"3DNPC_Misc_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "Paintingquest"},
		/*03*/ {"3DNPC_Misc_Quest03", CFlagEnum::kSide, CCompEnum::kStand, "CarmellaSkjarnQuest"},
		/*04*/ {"3DNPC_Misc_Quest04", CFlagEnum::kSide, CCompEnum::kStand, "DialogueAzzarian"},
		/*05*/ {"3DNPC_Misc_Quest05", CFlagEnum::kSide, CCompEnum::kStand, "Lurgokquest"},
		/*06*/ {"3DNPC_Misc_Quest06", CFlagEnum::kSide, CCompEnum::kStand, "BlackRobesQuest"},
		/*07*/ {"3DNPC_Misc_Quest07", CFlagEnum::kSide, CCompEnum::kStand, "Arielequest"},
		/*08*/ {"3DNPC_Misc_Quest08", CFlagEnum::kSide, CCompEnum::kStand, "Morndasargonianquest"},
		/*09*/ {"3DNPC_Misc_Quest09", CFlagEnum::kSide, CCompEnum::kStand, "DialogueBrakh"},
		/*10*/ {"3DNPC_Misc_Quest10", CFlagEnum::kSide, CCompEnum::kStand, "Beatricequest"},
		/*11*/ {"3DNPC_Misc_Quest11", CFlagEnum::kSide, CCompEnum::kStand, "DialogueTheRelic"},
		/*12*/ {"3DNPC_Misc_Quest12", CFlagEnum::kSide, CCompEnum::kStand, "RobbersRefuge3DNPC"},
		/*13*/ {"3DNPC_Misc_Quest13", CFlagEnum::kSide, CCompEnum::kStand, "CassockQuest"},
		/*14*/ {"3DNPC_Misc_Quest14", CFlagEnum::kSide, CCompEnum::kStand, "Skjelquest"},
		/*15*/ {"3DNPC_Misc_Quest15", CFlagEnum::kSide, CCompEnum::kStand, "DialogueYtharil"},
		/*16*/ {"3DNPC_Misc_Quest16", CFlagEnum::kSide, CCompEnum::kStand, "DialogueDarrakki"},
		/*17*/ {"3DNPC_Misc_Quest17", CFlagEnum::kSide, CCompEnum::kStand, "Hagcursequest"},
		/*18*/ {"3DNPC_Misc_Quest18", CFlagEnum::kSide, CCompEnum::kStand, "Vartheimquest"},
		/*19*/ {"3DNPC_Misc_Quest19", CFlagEnum::kSide, CCompEnum::kStand, "HWSQuest"},
		/*20*/ {"3DNPC_Misc_Quest20", CFlagEnum::kSide, CCompEnum::kStand, "DialogueRaynes"},
		/*21*/ {"3DNPC_Misc_Quest21", CFlagEnum::kSide, CCompEnum::kStand, "DialogueJilkmar"},
		/*22*/ {"3DNPC_Misc_Quest22", CFlagEnum::kSide, CCompEnum::kStand, "Bookquest3dnpc"},
		/*25*/ {"3DNPC_Misc_Quest25", CFlagEnum::kSide, CCompEnum::kStand, "DialogueIorel"},
		/*23*/ {"3DNPC_Misc_Quest23", CFlagEnum::kSide, CCompEnum::kStand, "DialogueThriceBitten"},
		/*24*/ {"3DNPC_Misc_Quest24", CFlagEnum::kSide, CCompEnum::kStand, "Terynnequest"},
	};

	// clang-format off

	constexpr Serialization::FormArray Items = {
	0x186CD2,0x28493D,0x1B2920,0x0DF4B8,0x1067D8,0x0DF4BA,0x0DF4BB,
	0x1FF21B,0x19A951,0x0DF4B7,0x19853F,0x0E97FD,0x22B94F,0x12E113,
	0x1A6EF3,0x27F184,0x1D8052,0x18F49B,0x1A6EF7,0x2A25FB,0x200C93,
	0x1B018C,0x2BE76E,0x500F44,0x363BF7,0x363BF6,0x152283,0x16A25B,
	0x1C99CF,0x14E0DD,0x144638,0x19A970,0x148844,0x2CB194,0x12A149,
	0x18A9F9,0x1C3AC4,0x1C3AC3,0x1C3AC6,0x238527,0x1C5CDB,0x1ACFFA,
	0x0DE9E9,0x14E0DE,0x1915E0,0x205FD9,0x1F3B47,0x0CDC78,0x182E25,
	};

	constexpr Serialization::FormArray Books = {
	0x272F9B,0x02B06F,0x130790,0x1A37C7,0x1A118C,0x13A92F,0x058932,
	0x190A9B,0x191023,0x191024,0x19159D,0x130792,0x01652C,0x130D40,
	0x1B018E,0x12F1B5,0x1CED4E,0x1AE651,0x1B4638,
	};

	constexpr Serialization::FormArray MapMa = {
	0x21908B,0x30846C,0x19DBFB,0x2C5C97,0x29844D,0x2A269C,0x380979,
	0x28BD0E,0x1A7F98,0x1DBB14,0x1DA7F2,0x248CAF,0x1FD608,0x1C2283,
	0x245AA4,0x17CB26,0x1CC676,0x1D40FC,0x24E51F,0x1BD250,
	};

	// clang-format on

	constexpr std::string_view modname = "3DNPC.esp";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		CHandler::InjectAndCompileData();
		CHandler::InstallQuestFramework();
		CHandler::InstallSearchTerms();

		FrameworkAPI::AddUpdateFoundForms(CHandler::UpdateFoundForms);
		FrameworkAPI::AddMapMarkerDiscovery(ProcessHookedMarker);
		CEvents::EventHandler::RegisterForEvent_OnBooksReadEvent(CHandler::OnBooksReadEvent);
		CEvents::EventHandler::RegisterForEvent_OnMenuOpenCloseEvent(CHandler::OnMenuOpenCloseEvent);
		CEvents::EventHandler::RegisterForEvent_OnContainerChangedEvent(CHandler::OnContainerChangedEvent);
		PatchesInstalled += 1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallQuestFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(Quest1_Data)>; i++)
		{
			Quest1_Data[i].init()->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&Quest1_Data[i], Quest1_Data[i].GetName(), 44, Quest1_Data[i].unique_identifier));
		}

		for (auto i = 0; i < std::extent_v<decltype(Quest2_Data)>; i++)
		{
			Quest2_Data[i].init()->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&Quest2_Data[i], Quest2_Data[i].GetName(), 45, Quest2_Data[i].unique_identifier));
		}

		for (auto i = 0; i < std::extent_v<decltype(Quest3_Data)>; i++)
		{
			Quest3_Data[i].init()->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&Quest3_Data[i], Quest3_Data[i].GetName(), 46, Quest3_Data[i].unique_identifier));
		}

		for (auto i = 0; i < std::extent_v<decltype(Quest4_Data)>; i++)
		{
			Quest4_Data[i].init()->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&Quest4_Data[i], Quest4_Data[i].GetName(), 47, Quest4_Data[i].unique_identifier));
		}
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	void CHandler::OnContainerChangedEvent(RE::TESContainerChangedEvent const* a_event) {
		using cmd = Serialization::CompletionistLog::logType;

		if (a_event->newContainer != 0x00014 || !ItemData.HasForm(a_event->baseObj)) { return; }

		auto base = ItemData.GetBase(a_event->baseObj) ? ItemData.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData, Items_FormArray, &Items_BoolArray, &Items_EntriesFound, cmd::kCollected, "NotifyItems");
		return;
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	void CHandler::OnBooksReadEvent(RE::BooksRead::Event const* a_event) {
		using cmd = Serialization::CompletionistLog::logType;

		if (!BookData.HasForm(a_event->book->GetFormID())) { return; }

		auto base = BookData.GetBase(a_event->book->GetFormID()) ? BookData.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), BookData, Books_FormArray, &Books_BoolArray, &Books_EntriesFound, FrameworkAPI::GetBookLogType(a_event->book), "NotifyBooks");
		return;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	void CHandler::OnMenuOpenCloseEvent(RE::MenuOpenCloseEvent const* a_event) {
		using cmd = Serialization::CompletionistLog::logType;

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && BookData.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = BookData.GetBase(target) ? BookData.GetBase(target) : target;
				CHandler::ProcessFoundForm(base, target, BookData, Books_FormArray, &Books_BoolArray, &Books_EntriesFound, cmd::kBook, "NotifyBooks");
				return;
			}
			return;
		}

		if (a_event->menuName == RE::MapMenu::MENU_NAME && a_event->opening) {

			for (auto i = 0; i < MapMa_FormArray.size(); i++) {
				CHandler::ProcessMapMarker(MapMa_FormArray[i], i, false);
			}
		}
		return;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(ProcessFoundFormArgs, std::string a_section) {

		if (!FoundItemData.HasForm(a_eventID)) {
			auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, data.GetForm(a_eventID)->GetName());
			FrameworkAPI::SendNotification(msg, a_section);
			FrameworkAPI::AddNewEventToLog(eventHandle, data.GetForm(a_eventID)->GetName());
		}

		FoundItemData.AddForm(a_baseID);
		for (auto var : data.GetAllVariations()) {
			if (data.GetBase(var) == a_baseID) {
				FoundItemData.AddForm(var);
			}
		}

		bools->at(std::distance(forms.begin(), std::ranges::find(forms, data.GetForm(a_baseID)))) = true;
		*found = std::ranges::count(*bools, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Map Marker ) -----
	//---------------------------------------------------

	void CHandler::ProcessMapMarker(RE::TESForm* a_form, std::int32_t a_pos, bool from_hook) {

		auto* a_marker = static_cast<RE::TESObjectREFR*>(a_form);

		if (a_marker && !FoundItemData_NoShow.HasForm(a_form)) {
			if (auto extraMapMarker = Serialization::CompletionistData::GetMapMarkerInternal(a_marker); extraMapMarker && extraMapMarker->mapData) {
				if (extraMapMarker->mapData->flags.all(RE::MapMarkerData::Flag::kVisible, RE::MapMarkerData::Flag::kCanTravelTo) && !a_marker->IsDisabled()) {
					MapMa_BoolArray[a_pos] = true;
					FoundItemData_NoShow.AddForm(a_form);
					if (from_hook)
					{
						auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, MapMa_NameArray[a_pos]);
						FrameworkAPI::SendNotification(msg, "NotifySpecial");
						FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kDiscovered, MapMa_NameArray[a_pos]);
					}
				}
			}
		}
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Events ( Process Hooked Markers ) ----
	//---------------------------------------------------

	void CHandler::ProcessHookedMarker(const char* nam)
	{
		for (auto i = 0; i < MapMa_FormArray.size(); i++) {
			if (DKUtil::string::iequals(nam, MapMa_NameArray[i]) && !FoundItemData_NoShow.HasForm(MapMa_FormArray[i])) {
				CHandler::ProcessMapMarker(MapMa_FormArray[i], i, true);
				return;
			}
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		ItemData.CompileFormArray(CPatch_3DC::Items, modname);
		BookData.CompileFormArray(CPatch_3DC::Books, modname);
		MapsData.CompileFormArray(CPatch_3DC::MapMa, modname);

		ItemData.MergeAsCollectable();
		BookData.MergeAsCollectable();

		ItemData.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);
		BookData.Populate(Books_NameArray, Books_FormArray, Books_BoolArray, Books_TextArray, false, 1);
		MapsData.Populate(MapMa_NameArray, MapMa_FormArray, MapMa_BoolArray, MapMa_TextArray, false, 2);

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		Books_EntriesTotal = Books_FormArray.size();
		Books_EntriesFound = std::ranges::count(Books_BoolArray, true);

		MapMa_EntriesTotal = MapMa_FormArray.size();
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Search Terms ) ---
	//---------------------------------------------------

	void CHandler::InstallSearchTerms()
	{
		for (auto i = 0; i < Items_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Items_FormArray[i], Items_NameArray[i], "$MCMPageInterestingNPCs", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < Books_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Books_FormArray[i], Books_NameArray[i], "$MCMPageInterestingNPCs", FrameworkAPI::GetBookCategoryType(Books_FormArray[i])));
		}
		for (auto i = 0; i < MapMa_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(MapMa_FormArray[i], MapMa_NameArray[i], "$MCMPageInterestingNPCs", std::to_underlying(EntryCategory::kMapM)));
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i = 0; i < Items_FormArray.size(); i++) {
			Items_BoolArray[i] = FrameworkAPI::IsItemKnown(Items_FormArray[i], &ItemData);
		}

		for (auto i = 0; i < Books_FormArray.size(); i++) {
			Books_BoolArray[i] = FrameworkAPI::IsBookKnown(Books_FormArray[i]);
		}

		for (auto i = 0; i < MapMa_FormArray.size(); i++) {
			MapMa_BoolArray[i] = FoundItemData_NoShow.HasForm(MapMa_FormArray[i]->GetFormID());
		}

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		Books_EntriesTotal = Books_FormArray.size();
		Books_EntriesFound = std::ranges::count(Books_BoolArray, true);

		MapMa_EntriesTotal = MapMa_FormArray.size();
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	}
}