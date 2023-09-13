#include "Serialization.hpp"
#include "CFramework_3DC.hpp"
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
		/*23*/ {"3DNPC_Misc_Quest23", CFlagEnum::kSide, CCompEnum::kStand, "DialogueThriceBitten"},
		/*24*/ {"3DNPC_Misc_Quest24", CFlagEnum::kSide, CCompEnum::kStand, "Terynnequest"},
	};

	CArrayData ArrayData1{ &Quest1_IdenArray, &Quest1_NameArray, &Quest1_TextArray, &Quest1_BoolArray, &Quest1_RadiArray, &Quest1_KeysArray };
	CArrayData ArrayData2{ &Quest2_IdenArray, &Quest2_NameArray, &Quest2_TextArray, &Quest2_BoolArray, &Quest2_RadiArray, &Quest2_KeysArray };
	CArrayData ArrayData3{ &Quest3_IdenArray, &Quest3_NameArray, &Quest3_TextArray, &Quest3_BoolArray, &Quest3_RadiArray, &Quest3_KeysArray };
	CArrayData ArrayData4{ &Quest4_IdenArray, &Quest4_NameArray, &Quest4_TextArray, &Quest4_BoolArray, &Quest4_RadiArray, &Quest4_KeysArray };

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

		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
		CHandler::InstallQuestFramework();
		CHandler::InstallSearchTerms();

		FrameworkAPI::AddUpdateFoundForms(CHandler::UpdateFoundForms);
		FrameworkAPI::AddMapMarkerDiscovery(ProcessHookedMarker);
		PatchesInstalled += 1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallQuestFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(Quest1_Data)>; i++)
		{
			Quest1_Data[i].init()
				->initQuestData(&ArrayData1);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&Quest1_Data[i], Quest1_Data[i].GetName(), 44, Quest1_Data[i].unique_identifier));
		}
		Quest1_BoolArray = std::vector<bool>(CArraySize, false);

		for (auto i = 0; i < std::extent_v<decltype(Quest2_Data)>; i++)
		{
			Quest2_Data[i].init()
				->initQuestData(&ArrayData2);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&Quest2_Data[i], Quest2_Data[i].GetName(), 45, Quest2_Data[i].unique_identifier));
		}
		Quest2_BoolArray = std::vector<bool>(CArraySize, false);

		for (auto i = 0; i < std::extent_v<decltype(Quest3_Data)>; i++)
		{
			Quest3_Data[i].init()
				->initQuestData(&ArrayData3);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&Quest3_Data[i], Quest3_Data[i].GetName(), 46, Quest3_Data[i].unique_identifier));
		}
		Quest3_BoolArray = std::vector<bool>(CArraySize, false);

		for (auto i = 0; i < std::extent_v<decltype(Quest4_Data)>; i++)
		{
			Quest4_Data[i].init()
				->initQuestData(&ArrayData4);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&Quest4_Data[i], Quest4_Data[i].GetName(), 47, Quest4_Data[i].unique_identifier));
		}
		Quest4_BoolArray = std::vector<bool>(CArraySize, false);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() {
		RE::BooksRead::GetEventSource()->AddEventSink(CHandler::GetSingleton());

		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));

		auto ESourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESContainerChangedEvent>*>(CHandler::GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {

		if (!a_event || a_event->newContainer != 0x00014 || !CPatch_3DC_Items::Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = CPatch_3DC_Items::Data.GetBase(a_event->baseObj) ? CPatch_3DC_Items::Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, "NotifyItems");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) {

		if (!a_event || !CPatch_3DC_Books::Data.HasForm(a_event->book->GetFormID())) { return RE::BSEventNotifyControl::kContinue; }

		auto base = CPatch_3DC_Books::Data.GetBase(a_event->book->GetFormID()) ? CPatch_3DC_Books::Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), "NotifyBooks");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && CPatch_3DC_Books::Data.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = CPatch_3DC_Books::Data.GetBase(target) ? CPatch_3DC_Books::Data.GetBase(target) : target;
				CHandler::ProcessFoundForm(base, target, "NotifyBooks");
				return EventResult::kContinue;
			}
			return EventResult::kContinue;
		}

		if (a_event->menuName == RE::MapMenu::MENU_NAME && a_event->opening) {

			for (auto i = 0; i < MapMa_FormArray.size(); i++) {
				CHandler::ProcessMapMarker(MapMa_FormArray[i], i);
			}
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, std::string a_variable) {

		if (a_variable == "NotifyBooks") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_3DC_Books::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
				if (auto* book = static_cast<RE::TESObjectBOOK*>(CPatch_3DC_Books::Data.GetForm(a_eventID)); book && book->GetSpell()) {
					FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kTome, book->GetName());
				}
				else {
					FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kBook, CPatch_3DC_Books::Data.GetForm(a_eventID)->GetName());
				}
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_3DC_Books::Data.GetAllVariations()) {
				if (CPatch_3DC_Books::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Books_FormArray, CPatch_3DC_Books::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Books_FormArray.begin(), t_pos);
			Books_BoolArray[b_pos] = true;

			Books_EntriesFound = std::ranges::count(Books_BoolArray, true);
			return;
		}

		if (a_variable == "NotifyItems") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_3DC_Items::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kCollected, CPatch_3DC_Items::Data.GetForm(a_eventID)->GetName());
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_3DC_Items::Data.GetAllVariations()) {
				if (CPatch_3DC_Items::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Items_FormArray, CPatch_3DC_Items::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Items_FormArray.begin(), t_pos);
			Items_BoolArray[b_pos] = true;

			Items_EntriesFound = std::ranges::count(Items_BoolArray, true);
			return;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Map Marker ) -----
	//---------------------------------------------------

	void CHandler::ProcessMapMarker(RE::TESForm* a_form, std::int32_t a_pos) {

		auto* a_marker = static_cast<RE::TESObjectREFR*>(a_form);
		if (a_marker && !FoundItemData_NoShow.HasForm(a_form)) {
			if (auto extraMapMarker = Serialization::CompletionistData::GetMapMarkerInternal(a_marker); extraMapMarker && extraMapMarker->mapData) {
				if (extraMapMarker->mapData->flags.all(RE::MapMarkerData::Flag::kVisible, RE::MapMarkerData::Flag::kCanTravelTo) && !a_marker->IsDisabled()) {
					MapMa_BoolArray[a_pos] = true;
					FoundItemData_NoShow.AddForm(a_form);
					auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, MapMa_NameArray[a_pos]);
					FrameworkAPI::SendNotification(msg, "NotifySpecial");
					FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kDiscovered, MapMa_NameArray[a_pos]);
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
				CHandler::ProcessMapMarker(MapMa_FormArray[i], i);
				return;
			}
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		CPatch_3DC_Items::Data.CompileFormArray(CPatch_3DC::Items, modname);
		CPatch_3DC_Books::Data.CompileFormArray(CPatch_3DC::Books, modname);
		CPatch_3DC_MapMa::Data.CompileFormArray(CPatch_3DC::MapMa, modname);

		CPatch_3DC_Items::Data.MergeAsCollectable();
		CPatch_3DC_Books::Data.MergeAsCollectable();

		CPatch_3DC_Items::Data.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);
		CPatch_3DC_Books::Data.Populate(Books_NameArray, Books_FormArray, Books_BoolArray, Books_TextArray, false, 1);
		CPatch_3DC_MapMa::Data.Populate(MapMa_NameArray, MapMa_FormArray, MapMa_BoolArray, MapMa_TextArray, false, 2);

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		Books_EntriesTotal = Books_FormArray.size();
		Books_EntriesFound = std::ranges::count(Books_BoolArray, true);

		MapMa_EntriesTotal = MapMa_FormArray.size();
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	}

	void CHandler::InstallSearchTerms()
	{
		for (auto& name : Items_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageInterestingNPCs", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < Books_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Books_NameArray[i], "$MCMPageInterestingNPCs", FrameworkAPI::GetBookCategoryType(Books_FormArray[i])));
		}
		for (auto& name : MapMa_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageInterestingNPCs", std::to_underlying(EntryCategory::kMapM)));
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i = 0; i < Items_FormArray.size(); i++) {
			Items_BoolArray[i] = FrameworkAPI::IsItemKnown(Items_FormArray[i], &CPatch_3DC_Items::Data);
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