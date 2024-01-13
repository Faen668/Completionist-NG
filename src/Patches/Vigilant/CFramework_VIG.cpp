#include "Serialization.hpp"
#include "CFramework_VIG.hpp"
#include "Internal Utility/Events.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

#undef AddForm

namespace CPatch_VIG 
{
	using namespace CFramework_Master;

	CQuestData Quest1_Data[]
	{
		{"Vigilant_Main_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "zzzAoMMq00"},
		{"Vigilant_Main_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "zzzAoMMq01"},
		{"Vigilant_Main_Quest02", CFlagEnum::kMain, CCompEnum::kStand, "zzzAoMMq02"},
		{"Vigilant_Main_Quest03", CFlagEnum::kMain, CCompEnum::kStand, "zzzAoMMq03"},
		{"Vigilant_Main_Quest04", CFlagEnum::kMain, CCompEnum::kStand, "zzzAoMMq04"},
		{"Vigilant_Main_Quest05", CFlagEnum::kMain, CCompEnum::kStand, "zzzAoMMq05"},
		{"Vigilant_Main_Quest06", CFlagEnum::kMain, CCompEnum::kStand, "zzzAoMMq06"},
		{"Vigilant_Main_Quest07", CFlagEnum::kMain, CCompEnum::kStand, "zzzAoMMq07"},
		{"Vigilant_Main_Quest08", CFlagEnum::kMain, CCompEnum::kStand, "zzzAoMMq08"},
		{"Vigilant_Main_Quest09", CFlagEnum::kMain, CCompEnum::kStand, "zzzAoMMqGoodEnd"},
		{"Vigilant_Main_Quest10", CFlagEnum::kMain, CCompEnum::kStand, "zzzAoMMq09"},
		{"Vigilant_Main_Quest11", CFlagEnum::kMain, CCompEnum::kStand, "zzzAoMMq10"},
		{"Vigilant_Main_Quest12", CFlagEnum::kMain, CCompEnum::kStand, "zzzAoMSubQ01"},
		{"Vigilant_Main_Quest13", CFlagEnum::kMain, CCompEnum::kStand, "zzzBMMq01"},
		{"Vigilant_Main_Quest14", CFlagEnum::kMain, CCompEnum::kStand, "zzzBMMq02"},
		{"Vigilant_Main_Quest15", CFlagEnum::kMain, CCompEnum::kStand, "zzzBMMq03"},
		{"Vigilant_Main_Quest16", CFlagEnum::kMain, CCompEnum::kStand, "zzzCOMq01"},
		{"Vigilant_Main_Quest17", CFlagEnum::kMain, CCompEnum::kStand, "zzzCHMQ00"},
		{"Vigilant_Main_Quest18", CFlagEnum::kMain, CCompEnum::kStand, "zzzCHMQ01"},
		{"Vigilant_Main_Quest19", CFlagEnum::kMain, CCompEnum::kStage, "zzzCHMQ02"},
		{"Vigilant_Main_Quest20", CFlagEnum::kMain, CCompEnum::kStage, "zzzCHEpilogueQuest01"},
		{"Vigilant_Main_Quest21", CFlagEnum::kMain, CCompEnum::kStand, "zzzAoMSubQ02"},
	};

	CStageData Quest1_StageData[]{
		{"Vigilant_Main_Quest19", CStageEnum::kPast, 50, 0},
		{"Vigilant_Main_Quest20", CStageEnum::kPast, 20, 0},
	};

	CQuestData Quest2_Data[]
	{
		{"Vigilant_Side_Quest00", CFlagEnum::kSide, CCompEnum::kStage, "zzzCHSubQuest02"},
		{"Vigilant_Side_Quest01", CFlagEnum::kSide, CCompEnum::kStage, "zzzCHSubQuest03"},
		{"Vigilant_Side_Quest02", CFlagEnum::kSide, CCompEnum::kStage, "zzzCHSubQuest04"},
		{"Vigilant_Side_Quest03", CFlagEnum::kSide, CCompEnum::kStage, "zzzCHSubQuest01"},
		{"Vigilant_Side_Quest04", CFlagEnum::kSide, CCompEnum::kStage, "zzzCHSubQuest09"},
		{"Vigilant_Side_Quest05", CFlagEnum::kSide, CCompEnum::kStage, "zzzCHSubQuest10"},
		{"Vigilant_Side_Quest06", CFlagEnum::kSide, CCompEnum::kStand, "zzzCHSubQuest08"},
		{"Vigilant_Side_Quest07", CFlagEnum::kSide, CCompEnum::kStand, "zzzCHSubQuest07"},
		{"Vigilant_Side_Quest08", CFlagEnum::kSide, CCompEnum::kStage, "zzzCHSubQuest12"},
		{"Vigilant_Side_Quest09", CFlagEnum::kSide, CCompEnum::kStage, "zzzCHSubQuest05"},
		{"Vigilant_Side_Quest10", CFlagEnum::kSide, CCompEnum::kStage, "zzzCHSubQuest06"},
		{"Vigilant_Side_Quest11", CFlagEnum::kSide, CCompEnum::kStand, "zzzCHSubQuest11"},
		{"Vigilant_Side_Quest12", CFlagEnum::kSide, CCompEnum::kStage, "zzzCHGreymarchQuest"},
	};

	CStageData Quest2_StageData[]{
		{"Vigilant_Side_Quest00", CStageEnum::kPast, 255, 0},
		{"Vigilant_Side_Quest01", CStageEnum::kPast, 255, 0},
		{"Vigilant_Side_Quest02", CStageEnum::kPast, 255, 0},
		{"Vigilant_Side_Quest03", CStageEnum::kPast, 255, 0},
		{"Vigilant_Side_Quest04", CStageEnum::kPast, 20,  0},
		{"Vigilant_Side_Quest05", CStageEnum::kPast, 20,  0},
		{"Vigilant_Side_Quest08", CStageEnum::kPast, 50,  0},
		{"Vigilant_Side_Quest09", CStageEnum::kPast, 60,  0},
		{"Vigilant_Side_Quest10", CStageEnum::kPast, 30,  0},
		{"Vigilant_Side_Quest12", CStageEnum::kPast, 20,  0},
	};

	CQuestData Quest3_Data[]
	{
		{"Vigilant_Memory_Quest00", CFlagEnum::kSide, CCompEnum::kStand, "zzzCHMemoryQuest03"},
		{"Vigilant_Memory_Quest01", CFlagEnum::kSide, CCompEnum::kStand, "zzzCHMemoryQuest04"},
		{"Vigilant_Memory_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "zzzCHMemoryQuest10"},
		{"Vigilant_Memory_Quest03", CFlagEnum::kSide, CCompEnum::kStand, "zzzCHMemoryQuest01"},
		{"Vigilant_Memory_Quest04", CFlagEnum::kSide, CCompEnum::kStand, "zzzCHMemoryQuest02"},
		{"Vigilant_Memory_Quest05", CFlagEnum::kSide, CCompEnum::kStand, "zzzCHMemoryQuest05"},
		{"Vigilant_Memory_Quest06", CFlagEnum::kSide, CCompEnum::kStand, "zzzCHMemoryQuest11"},
		{"Vigilant_Memory_Quest07", CFlagEnum::kSide, CCompEnum::kStand, "zzzCHMemoryQuest12"},
		{"Vigilant_Memory_Quest08", CFlagEnum::kSide, CCompEnum::kStand, "zzzCHMemoryQuest06"},
		{"Vigilant_Memory_Quest09", CFlagEnum::kSide, CCompEnum::kStand, "zzzCHMemoryQuest07"},
		{"Vigilant_Memory_Quest10", CFlagEnum::kSide, CCompEnum::kStand, "zzzCHMemoryQuest08"},
		{"Vigilant_Memory_Quest11", CFlagEnum::kSide, CCompEnum::kStand, "zzzCHMemoryQuest09"},
	};

	CQuestData Quest4_Data[]
	{
		{"Vigilant_Radiant_Quest00", CFlagEnum::kRadi, CCompEnum::kGlobl, "zzzAomBountyBook"},
		{"Vigilant_Radiant_Quest01", CFlagEnum::kRadi, CCompEnum::kGlobl, "zzzAomBountyChickTrader"},
		{"Vigilant_Radiant_Quest02", CFlagEnum::kRadi, CCompEnum::kGlobl, "zzzAomBountyRelic"},
		{"Vigilant_Radiant_Quest03", CFlagEnum::kRadi, CCompEnum::kGlobl, "zzzAoMBountyRelicHorn"},
		{"Vigilant_Radiant_Quest04", CFlagEnum::kRadi, CCompEnum::kGlobl, "zzzAoMBountyPiper"},
		{"Vigilant_Radiant_Quest05", CFlagEnum::kRadi, CCompEnum::kGlobl, "zzzAomBountySummoner"},
		{"Vigilant_Radiant_Quest06", CFlagEnum::kRadi, CCompEnum::kGlobl, "zzzAomBountyVampire"},
		{"Vigilant_Radiant_Quest07", CFlagEnum::kRadi, CCompEnum::kGlobl, "zzzAomBountyWitch"},
		{"Vigilant_Radiant_Quest08", CFlagEnum::kRadi, CCompEnum::kGlobl, "zzzAoMRadDead"},
		{"Vigilant_Radiant_Quest09", CFlagEnum::kRadi, CCompEnum::kGlobl, "zzzAoMRadDoppler"},
		{"Vigilant_Radiant_Quest10", CFlagEnum::kRadi, CCompEnum::kGlobl, "zzzAoMRadMolag"},
		{"Vigilant_Radiant_Quest11", CFlagEnum::kRadi, CCompEnum::kGlobl, "zzzAoMRadVampire"},
		{"Vigilant_Radiant_Quest12", CFlagEnum::kRadi, CCompEnum::kGlobl, "zzzAoMRadWitchHunter"},
	};

	CRadiantData Quest4_RadiantData[]{
		{"Vigilant_Radiant_Quest00", CRadiantEnum::kRadiant_VIG, 0x4E1318, 0, 20},
		{"Vigilant_Radiant_Quest01", CRadiantEnum::kRadiant_VIG, 0x4E372E, 0, 20},
		{"Vigilant_Radiant_Quest02", CRadiantEnum::kRadiant_VIG, 0x4E011B, 0, 20},
		{"Vigilant_Radiant_Quest03", CRadiantEnum::kRadiant_VIG, 0x4E0121, 0, 20},
		{"Vigilant_Radiant_Quest04", CRadiantEnum::kRadiant_VIG, 0x4E0116, 0, 10},
		{"Vigilant_Radiant_Quest05", CRadiantEnum::kRadiant_VIG, 0x4E0102, 0, 10},
		{"Vigilant_Radiant_Quest06", CRadiantEnum::kRadiant_VIG, 0x4E0108, 0, 10},
		{"Vigilant_Radiant_Quest07", CRadiantEnum::kRadiant_VIG, 0x4E010E, 0, 10},
		{"Vigilant_Radiant_Quest08", CRadiantEnum::kRadiant_VIG, 0x4EA3A8, 0, 10},
		{"Vigilant_Radiant_Quest09", CRadiantEnum::kRadiant_VIG, 0x4EA3D1, 0, 10},
		{"Vigilant_Radiant_Quest10", CRadiantEnum::kRadiant_VIG, 0x4EA3AC, 0, 10},
		{"Vigilant_Radiant_Quest11", CRadiantEnum::kRadiant_VIG, 0x4E91B0, 0, 10},
		{"Vigilant_Radiant_Quest12", CRadiantEnum::kRadiant_VIG, 0x4EA3C8, 0, 10},
	};

	// clang-format off

	constexpr Serialization::FormArray Items = {
	0x1353DF,0x3B3500,0x148177,0x03D213,0x119E3C,0x11B7D0,0x0EF0B6,
	0x12942F,0x0D7360,0x0B6D80,0x0DA99B,0x0B0EAA,0x0C7E35,0x0B0EAB,
	0x0B0EAC,0x0B0EAD,0x142BF4,0x29F2EF,0x0CE832,0x00A962,0x03D783,
	0x1D19BD,0x071CE2,0x0D957B,0x1418A9,0x0C847B,0x1418A8,0x1280CB,
	0x0CE835,0x13F43D,0x12A7A3,0x0CB0AE,0x15CE2D,0x11880C,0x323B2F,
	0x0DFD65,0x0DFD66,0x0DFD67,0x0DFD68,0x1804E9,0x12B2F9,0x130E6B,
	0x187829,0x18782A,0x18782B,0x18782C,0x130E6A,0x119E3B,0x130E69,
	0x22B38C,0x12A7A2,0x142BF5,0x0EB828,0x0EB827,0x1418AB,0x1280CA,
	0x0CB0AB,0x1450A0,0x1418AA,0x1280CC,0x100E9F,0x0EA4C2,0x2699FB,
	0x2699FC,0x2699FD,0x2699FE,0x296938,0x13F43C,0x0D428C,0x11B7CF,
	0x0D428B,0x0CFCB6,0x12942E,0x1804E7,0x0CAA70,
	};

	constexpr Serialization::Variation ItmsV[] = {
	{ 0x23007E, { 0x230080,0x23007C,0x23007D,0x23007F,0x230081 } },
	{ 0x144CDD, { 0x144CDE } },
	{ 0x0BBF35, { 0x0BD352 } },
	{ 0x0D428A, { 0x0D568D } },
	{ 0x323B2F, { 0x43CBAD } },
	{ 0x0C3DA4, { 0x0B828B } },
	{ 0x1AABC2, { 0x1AABC3 } },
	{ 0x0C68B9, { 0x0C68B8 } },
	};
	
	constexpr Serialization::FormArray Books = {
	0x4A8AFA,0x054ED0,0x054ED1,0x4A8AFD,0x4A8B03,0x4A8AFE,0x4A8B05,
	0x4A8B04,0x4A8AFC,0x0D69FC,0x0D69FA,0x0D69FD,0x10832C,0x108323,
	0x108325,0x108322,0x0DE93B,0x0CDE74,0x1258B1,0x0CDE76,0x1404FF,
	0x108324,0x4A8AFB,0x1CB871,0x1CB872,0x1CB873,0x0DB22D,0x4E1317,
	0x13B4F9,0x13B4FA,0x13B4FB,0x24724A,0x13B4FC,0x13B4FD,0x13B501,
	0x13B506,0x13B4F7,0x054ED3,0x4A8B07,0x4A8B02,0x3F7CFC,0x13BAB0,
	0x3FB795,0x12905B,0x18B1BA,0x4A8AF6,0x4A8B00,0x4A8AFF,0x0CB0DF,
	0x12905E,0x114FF7,0x12905D,0x0D429E,0x12905F,0x4A8AF9,0x0DB22E,
	0x054ED2,0x12905C,0x4A8B06,0x18B1B9,0x4A8AF8,0x4A8B01,0x4A8AF7,
	0x054ED5,0x054ED6,0x054ED4,0x038AAE,
	};

	constexpr Serialization::FormArray MapMa = {
	0x088308,0x0801A8,0x093F3A,0x08ABE1,0x1388FA,0x089143,0x0A2517,
	0x0896EC,0x06A6D8,0x0A0C88,0x0A578D,0x091910,0x204817,0x128D5A,
	0x144AD5,0x128D54,0x09205C,0x09205D,0x092058,0x088312,0x21C52E,
	0x128D57,0x25B236,0x08773A,0x093F39,0x07FB64,0x09F7FB,0x145667,
	0x09BFF0,0x0A3E75,0x089DAA,0x0801A9,0x08E83E,0x08773F,0x4C3D8B,
	0x0A95A4,0x0A578F,0x128D51,0x091911,0x09D2B6,0x025045,0x0A87F4,
	0x128D55,0x0AFEBA,0x07FB65,0x09C65C,0x171D71,
	};

	// clang-format on

	constexpr std::string_view modname = "Vigilant.esm";

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
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallQuestFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(Quest1_Data)>; i++)
		{
			Quest1_Data[i].init()->initStageData(Quest1_StageData)->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&Quest1_Data[i], Quest1_Data[i].GetName(), 48, Quest1_Data[i].unique_identifier));
		}

		for (auto i = 0; i < std::extent_v<decltype(Quest2_Data)>; i++)
		{
			Quest2_Data[i].init()->initStageData(Quest2_StageData)->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&Quest2_Data[i], Quest2_Data[i].GetName(), 49, Quest2_Data[i].unique_identifier));
		}

		for (auto i = 0; i < std::extent_v<decltype(Quest3_Data)>; i++)
		{
			Quest3_Data[i].init()->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&Quest3_Data[i], Quest3_Data[i].GetName(), 50, Quest3_Data[i].unique_identifier));
		}

		for (auto i = 0; i < std::extent_v<decltype(Quest4_Data)>; i++)
		{
			Quest4_Data[i].init()->initRadiantData(Quest4_RadiantData)->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&Quest4_Data[i], Quest4_Data[i].GetName(), 51, Quest4_Data[i].unique_identifier));
		}
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	void CHandler::OnContainerChangedEvent(RE::TESContainerChangedEvent const* a_event) {
		using log = Serialization::CompletionistLog::logType;

		if (a_event->newContainer != 0x00014 || !ItemData.HasForm(a_event->baseObj)) { return; }

		auto base = ItemData.GetBase(a_event->baseObj) ? ItemData.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData, Items_FormArray, &Items_BoolArray, &Items_EntriesFound, log::kCollected, "NotifyItems");
		return;
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	void CHandler::OnBooksReadEvent(RE::BooksRead::Event const* a_event) {
		using log = Serialization::CompletionistLog::logType;

		if (!BookData.HasForm(a_event->book->GetFormID())) { return; }

		auto base = BookData.GetBase(a_event->book->GetFormID()) ? BookData.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), BookData, Books_FormArray, &Books_BoolArray, &Books_EntriesFound, FrameworkAPI::GetBookLogType(a_event->book), "NotifyBooks");
		return;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	void CHandler::OnMenuOpenCloseEvent(RE::MenuOpenCloseEvent const* a_event) {
		using log = Serialization::CompletionistLog::logType;

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && BookData.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = BookData.GetBase(target) ? BookData.GetBase(target) : target;
				CHandler::ProcessFoundForm(base, target, BookData, Books_FormArray, &Books_BoolArray, &Books_EntriesFound, log::kBook, "NotifyBooks");
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

		ItemData.CompileFormArray(CPatch_VIG::Items, modname);
		ItemData.CompileVariation(CPatch_VIG::ItmsV, modname);
		
		BookData.CompileFormArray(CPatch_VIG::Books, modname);
		MapsData.CompileFormArray(CPatch_VIG::MapMa, modname);
		
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
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Items_FormArray[i], Items_NameArray[i], "$MCMPageVIG1", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < Books_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Books_FormArray[i], Books_NameArray[i], "$MCMPageVIG2", FrameworkAPI::GetBookCategoryType(Books_FormArray[i])));
		}
		for (auto i = 0; i < MapMa_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(MapMa_FormArray[i], MapMa_NameArray[i], "$MCMPageVIG3", std::to_underlying(EntryCategory::kMapM)));
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