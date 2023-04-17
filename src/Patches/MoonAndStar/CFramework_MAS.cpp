#include "Serialization.hpp"
#include "CFramework_MAS.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

#undef AddForm

namespace CPatch_MAS 
{
	using namespace CFramework_Master;

	CQuestData QuestData[]
	{
		/*00*/ {"MoonAndStar_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "MASMainQuest"},
		/*01*/ {"MoonAndStar_Quest01", CFlagEnum::kSide, CCompEnum::kStand, "MASAlbertDialog"},
		/*02*/ {"MoonAndStar_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "MASBalRanDialog"},
		/*03*/ {"MoonAndStar_Quest03", CFlagEnum::kSide, CCompEnum::kStand, "MASBelvadyrFavor"},
		/*04*/ {"MoonAndStar_Quest04", CFlagEnum::kSide, CCompEnum::kStand, "MASSelyseDialog"},
		/*05*/ {"MoonAndStar_Quest05", CFlagEnum::kSide, CCompEnum::kStand, "MASTorenDialog"},
	};

	CArrayData ArrayData{ &Quest_IdenArray, &Quest_NameArray, &Quest_TextArray, &Quest_BoolArray, &Quest_RadiArray };

	// clang-format off

	constexpr Serialization::FormArray Items = {
	0x005E4D,0x013F74,0x01033F,0x01033E,0x025EB3,		
	0x00E242,0x014FBA,0x010337,0x014FC0,0x00E243,		
	0x015023,0x00E240,		
	};

	constexpr Serialization::Variation ItmsV[] = {
		{ 0x00E285, { 0x00E284 } },

	};
	
	constexpr Serialization::FormArray Books = {
	0x011908,0x0170D1,0x011909,
	};

	constexpr Serialization::FormArray MapMa = {
	0x012ECE,0x012ED0,0x012ED9,
	};

	// clang-format on

	constexpr std::string_view modname = "MoonAndStar_MAS.esp";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
		CHandler::InstallQuestFramework();
		PatchesInstalled += 1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallQuestFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()
				->initQuestData(&ArrayData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 36));
		}
		Quest_BoolArray = std::vector<bool>(CArraySize, false);
	};

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() 
	{
		RE::BooksRead::GetEventSource()->AddEventSink(CHandler::GetSingleton());

		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));

		auto ESourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESContainerChangedEvent>*>(CHandler::GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) 
	{
		if (!a_event || a_event->newContainer != 0x00014 || !CPatch_MAS_Items::Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = CPatch_MAS_Items::Data.GetBase(a_event->baseObj) ? CPatch_MAS_Items::Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, "NotifyItems");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) 
	{
		if (!a_event || !CPatch_MAS_Books::Data.HasForm(a_event->book->GetFormID())) { return RE::BSEventNotifyControl::kContinue; }

		auto base = CPatch_MAS_Books::Data.GetBase(a_event->book->GetFormID()) ? CPatch_MAS_Books::Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), "NotifyBooks");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && CPatch_MAS_Books::Data.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = CPatch_MAS_Books::Data.GetBase(target) ? CPatch_MAS_Books::Data.GetBase(target) : target;
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
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_MAS_Books::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_MAS_Books::Data.GetAllVariations()) {
				if (CPatch_MAS_Books::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Books_FormArray, CPatch_MAS_Books::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Books_FormArray.begin(), t_pos);
			Books_BoolArray[b_pos] = true;

			Books_EntriesFound = std::ranges::count(Books_BoolArray, true);
			return;
		}

		if (a_variable == "NotifyItems") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_MAS_Items::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_MAS_Items::Data.GetAllVariations()) {
				if (CPatch_MAS_Items::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Items_FormArray, CPatch_MAS_Items::Data.GetForm(a_baseID));
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

		if (a_marker) {
			if (auto extraMapMarker = Serialization::CompletionistData::GetMapMarkerInternal(a_marker); extraMapMarker && extraMapMarker->mapData) {
				if (extraMapMarker->mapData->flags.all(RE::MapMarkerData::Flag::kVisible, RE::MapMarkerData::Flag::kCanTravelTo) && !a_marker->IsDisabled()) {
					MapMa_BoolArray[a_pos] = true;
					FoundItemData_NoShow.AddForm(a_marker);
				}
			}
		}
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		CPatch_MAS_Items::Data.CompileFormArray(CPatch_MAS::Items, modname);
		CPatch_MAS_Items::Data.CompileVariation(CPatch_MAS::ItmsV, modname);
		
		CPatch_MAS_Books::Data.CompileFormArray(CPatch_MAS::Books, modname);
		CPatch_MAS_MapMa::Data.CompileFormArray(CPatch_MAS::MapMa, modname);
		
		CPatch_MAS_Items::Data.MergeAsCollectable();
		CPatch_MAS_Books::Data.MergeAsCollectable();

		CPatch_MAS_Items::Data.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);
		CPatch_MAS_Books::Data.Populate(Books_NameArray, Books_FormArray, Books_BoolArray, Books_TextArray, false, 1);
		CPatch_MAS_MapMa::Data.Populate(MapMa_NameArray, MapMa_FormArray, MapMa_BoolArray, MapMa_TextArray, false, 2);

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		Books_EntriesTotal = Books_FormArray.size();
		Books_EntriesFound = std::ranges::count(Books_BoolArray, true);

		MapMa_EntriesTotal = MapMa_FormArray.size();
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i = 0; i < Items_FormArray.size(); i++) {
			Items_BoolArray[i] = FrameworkAPI::IsItemKnown(Items_FormArray[i], &CPatch_MAS_Items::Data);
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
};