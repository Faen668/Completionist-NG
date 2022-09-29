#include "Serialization.hpp"
#include "CFramework_Maps.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"

#undef AddForm

namespace CFramework_MapsManager {
	using namespace Serialization;

	void CHandler::InstallFramework() {

		CFramework_Maps_V::BookHandler::InstallFramework();
		if (CompletionistData::IsModInstalled("treasure_hunt.esp")) {
			CFramework_Maps_NTH::BookHandler::InstallFramework();
		}

		if (CompletionistData::IsModInstalled("TreasureHunter.esp")) {
			CFramework_Maps_TH::BookHandler::InstallFramework();
		}
	}

	void CHandler::UpdateFoundForms() {

		CFramework_Maps_V::BookHandler::UpdateFoundForms();
		if (CompletionistData::IsModInstalled("treasure_hunt.esp")) {
			CFramework_Maps_NTH::BookHandler::UpdateFoundForms();
		}

		if (CompletionistData::IsModInstalled("TreasureHunter.esp")) {
			CFramework_Maps_TH::BookHandler::UpdateFoundForms();

		}
	}
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------ Vanilla Maps
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

namespace CFramework_Maps_V {

	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray Maps = {
	0x0EF07A,0x0F33CE,0x0F33CF,0x0F33D1,0x0F33CD,0x0F33D4,
	0x0F33D0,0x0F33D5,0x0F33D3,0x0F33E0,0x0F33D2,0x0DDEFB,
	};

	// clang-format on

	Serialization::CompletionistData Data;

	std::vector<std::string> NameArray{};
	std::vector<std::string> TextArray{};
	std::vector<RE::TESForm*> FormArray{};
	std::vector<bool> BoolArray{};

	inline ScriptObjectPtr  MCMScript;

	inline uint32_t EntriesTotal;
	inline uint32_t EntriesFound;

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void BookHandler::InstallFramework() {

		BookHandler::Sink();
		MenuHandler::Sink();
		BookHandler::InjectAndCompileData();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Get Singleton ) ----------
	//---------------------------------------------------

	auto BookHandler::GetSingleton() -> BookHandler* {
		static BookHandler singleton;
		return std::addressof(singleton);
	}

	auto MenuHandler::GetSingleton() -> MenuHandler* {
		static MenuHandler singleton;
		return std::addressof(singleton);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void BookHandler::Sink() {
		RE::BooksRead::GetEventSource()->AddEventSink(BookHandler::GetSingleton());
	}

	void MenuHandler::Sink() {
		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(MenuHandler::GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	auto BookHandler::ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) -> RE::BSEventNotifyControl {

		if (!a_event || !Data.HasForm(a_event->book->GetFormID())) { return RE::BSEventNotifyControl::kContinue; }
			
		auto base = Data.GetBase(a_event->book->GetFormID()) ? Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		BookHandler::ProcessFoundForm(base, a_event->book->GetFormID(), false);
		return EventResult::kContinue;
	}

	auto MenuHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) -> RE::BSEventNotifyControl {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && Data.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = Data.GetBase(target) ? Data.GetBase(target) : target;
				BookHandler::ProcessFoundForm(base, target, false);
				return EventResult::kContinue;
			}
			return EventResult::kContinue;
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Map Marker ) -----
	//---------------------------------------------------

	void BookHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, bool a_supress) {

		if (!FoundItemData.HasForm(a_eventID) && !a_supress) {
			auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, Data.GetForm(a_eventID)->GetName());
			FrameworkAPI::SendNotification(msg, "NotifyBooks");
		}

		FoundItemData.AddForm(a_baseID);
		for (auto var : Data.GetAllVariations()) {
			if (Data.GetBase(var) == a_baseID) {
				FoundItemData.AddForm(var);
			}
		}

		auto t_pos = std::find(FormArray.begin(), FormArray.end(), Data.GetForm(a_baseID));
		auto b_pos = std::distance(FormArray.begin(), t_pos);
		BoolArray.at(b_pos) = true;

		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void BookHandler::InjectAndCompileData() {

		Data.CompileFormArray(CFramework_Maps_V::Maps, "Skyrim.esm");
		Data.MergeAsCollectable();

		CFramework_Maps_V::Data.Populate(NameArray, FormArray, BoolArray, TextArray, false, 1);

		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void BookHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {
			if (auto* book = static_cast<RE::TESObjectBOOK*>(FormArray[i]); book) {
				if (book->IsRead() || FoundItemData.HasForm(book->GetFormID())) {
					auto base = Data.GetBase(book->GetFormID()) ? Data.GetBase(book->GetFormID()) : book->GetFormID();
					BookHandler::ProcessFoundForm(base, FormArray[i]->GetFormID(), true);
				}
			}
		}

		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM is Entry Complete ) --
	//---------------------------------------------------

	uint32_t BookHandler::IsOptionCompleted(std::string a_name) {

		if (auto t_pos = std::find(NameArray.begin(), NameArray.end(), a_name); t_pos != NameArray.end()) {
			return uint32_t(BoolArray[std::distance(NameArray.begin(), t_pos)]);
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Set Entry Complete ) -
	//---------------------------------------------------

	void BookHandler::SetOptionCompleted(std::string a_name) {

		if (auto t_pos = std::find(NameArray.begin(), NameArray.end(), a_name); t_pos != NameArray.end()) {
			auto b_pos = std::distance(NameArray.begin(), t_pos);

			if (BoolArray.at(b_pos)) {
				BoolArray.at(b_pos) = false;

				FoundItemData.RemoveForm(FormArray.at(b_pos)->GetFormID());
				for (auto var : Data.GetAllVariations()) {
					if (Data.GetBase(var) == FormArray.at(b_pos)->GetFormID()) {
						FoundItemData.RemoveForm(var);
					}
				}
			}
			else {
				BoolArray.at(b_pos) = true;
				FoundItemData.AddForm(FormArray.at(b_pos)->GetFormID());
				for (auto var : Data.GetAllVariations()) {
					if (Data.GetBase(var) == FormArray.at(b_pos)->GetFormID()) {
						FoundItemData.AddForm(var);
					}
				}
			}

			EntriesTotal = FormArray.size();
			EntriesFound = std::ranges::count(BoolArray, true);
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------ New Treasure Hunt
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

namespace CFramework_Maps_NTH {

	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray Maps = {
	0x000D62,0x000D63,0x000803,0x000804,0x000809,0x000805,0x000806,
	0x000807,0x000808,0x00080B,0x00080C,0x00080D,0x00080E,
	};

	// clang-format on

	Serialization::CompletionistData Data;

	std::vector<std::string> NameArray{};
	std::vector<std::string> TextArray{};
	std::vector<RE::TESForm*> FormArray{};
	std::vector<bool> BoolArray{};

	inline ScriptObjectPtr  MCMScript;

	inline uint32_t EntriesTotal;
	inline uint32_t EntriesFound;

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void BookHandler::InstallFramework() {

		BookHandler::Sink();
		MenuHandler::Sink();

		BookHandler::InjectAndCompileData();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Get Singleton ) ----------
	//---------------------------------------------------

	auto BookHandler::GetSingleton() -> BookHandler* {
		static BookHandler singleton;
		return std::addressof(singleton);
	}

	auto MenuHandler::GetSingleton() -> MenuHandler* {
		static MenuHandler singleton;
		return std::addressof(singleton);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void BookHandler::Sink() {
		RE::BooksRead::GetEventSource()->AddEventSink(BookHandler::GetSingleton());
	}

	void MenuHandler::Sink() {
		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(MenuHandler::GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	auto BookHandler::ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) -> RE::BSEventNotifyControl {

		if (a_event == nullptr || !Data.HasForm(a_event->book->GetFormID())) { return RE::BSEventNotifyControl::kContinue; }

		auto base = Data.GetBase(a_event->book->GetFormID()) ? Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		BookHandler::ProcessFoundForm(base, a_event->book->GetFormID(), false);
		return EventResult::kContinue;
	}

	auto MenuHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) -> RE::BSEventNotifyControl {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && Data.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = Data.GetBase(target) ? Data.GetBase(target) : target;
				BookHandler::ProcessFoundForm(base, target, false);
				return EventResult::kContinue;
			}
			return EventResult::kContinue;
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Map Marker ) -----
	//---------------------------------------------------

	void BookHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, bool a_supress) {

		if (!FoundItemData.HasForm(a_eventID) && !a_supress) {
			auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, Data.GetForm(a_eventID)->GetName());
			FrameworkAPI::SendNotification(msg, "NotifyBooks");
		}

		FoundItemData.AddForm(a_baseID);
		for (auto var : Data.GetAllVariations()) {
			if (Data.GetBase(var) == a_baseID) {
				FoundItemData.AddForm(var);
			}
		}

		auto t_pos = std::find(FormArray.begin(), FormArray.end(), Data.GetForm(a_baseID));
		auto b_pos = std::distance(FormArray.begin(), t_pos);
		BoolArray.at(b_pos) = true;

		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void BookHandler::InjectAndCompileData() {

		auto handler = RE::TESDataHandler::GetSingleton();
		MCMScript = ScriptObject::FromForm(static_cast<RE::TESForm*>(handler->LookupForm(0x00800, "Completionist.esp")), "Completionist_MCMScript");
		Data.CompileFormArray(CFramework_Maps_NTH::Maps, "treasure_hunt.esp");
		Data.MergeAsCollectable();

		CFramework_Maps_NTH::Data.Populate(NameArray, FormArray, BoolArray, TextArray, false, 1);

		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void BookHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {
			if (auto* book = static_cast<RE::TESObjectBOOK*>(FormArray[i]); book) {
				if (book->IsRead() || FoundItemData.HasForm(book->GetFormID())) {
					auto base = Data.GetBase(book->GetFormID()) ? Data.GetBase(book->GetFormID()) : book->GetFormID();
					BookHandler::ProcessFoundForm(base, FormArray[i]->GetFormID(), true);
				}
			}
		}

		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM is Entry Complete ) --
	//---------------------------------------------------

	uint32_t BookHandler::IsOptionCompleted(std::string a_name) {

		if (auto t_pos = std::find(NameArray.begin(), NameArray.end(), a_name); t_pos != NameArray.end()) {
			return uint32_t(BoolArray[std::distance(NameArray.begin(), t_pos)]);
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Set Entry Complete ) -
	//---------------------------------------------------

	void BookHandler::SetOptionCompleted(std::string a_name) {

		if (auto t_pos = std::find(NameArray.begin(), NameArray.end(), a_name); t_pos != NameArray.end()) {
			auto b_pos = std::distance(NameArray.begin(), t_pos);

			if (BoolArray.at(b_pos)) {
				BoolArray.at(b_pos) = false;

				FoundItemData.RemoveForm(FormArray.at(b_pos)->GetFormID());
				for (auto var : Data.GetAllVariations()) {
					if (Data.GetBase(var) == FormArray.at(b_pos)->GetFormID()) {
						FoundItemData.RemoveForm(var);
					}
				}
			}
			else {
				BoolArray.at(b_pos) = true;
				FoundItemData.AddForm(FormArray.at(b_pos)->GetFormID());
				for (auto var : Data.GetAllVariations()) {
					if (Data.GetBase(var) == FormArray.at(b_pos)->GetFormID()) {
						FoundItemData.AddForm(var);
					}
				}
			}

			EntriesTotal = FormArray.size();
			EntriesFound = std::ranges::count(BoolArray, true);
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------ Treasure Hunter
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

namespace CFramework_Maps_TH {

	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray Maps = {
	0x000D63,0x006DA9,0x00787E,0x00834C,
	0x00834F,0x0095BA,0x021824,
	};

	// clang-format on

	Serialization::CompletionistData Data;

	std::vector<std::string> NameArray{};
	std::vector<std::string> TextArray{};
	std::vector<RE::TESForm*> FormArray{};
	std::vector<bool> BoolArray{};

	inline ScriptObjectPtr  MCMScript;

	inline uint32_t EntriesTotal;
	inline uint32_t EntriesFound;

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void BookHandler::InstallFramework() {

		BookHandler::Sink();
		MenuHandler::Sink();

		BookHandler::InjectAndCompileData();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Get Singleton ) ----------
	//---------------------------------------------------

	auto BookHandler::GetSingleton() -> BookHandler* {
		static BookHandler singleton;
		return std::addressof(singleton);
	}

	auto MenuHandler::GetSingleton() -> MenuHandler* {
		static MenuHandler singleton;
		return std::addressof(singleton);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void BookHandler::Sink() {
		RE::BooksRead::GetEventSource()->AddEventSink(BookHandler::GetSingleton());
	}

	void MenuHandler::Sink() {
		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(MenuHandler::GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	auto BookHandler::ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) -> RE::BSEventNotifyControl {

		if (a_event == nullptr || !Data.HasForm(a_event->book->GetFormID())) { return RE::BSEventNotifyControl::kContinue; }

		auto base = Data.GetBase(a_event->book->GetFormID()) ? Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		BookHandler::ProcessFoundForm(base, a_event->book->GetFormID(), false);
		return EventResult::kContinue;
	}

	auto MenuHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) -> RE::BSEventNotifyControl {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && Data.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = Data.GetBase(target) ? Data.GetBase(target) : target;
				BookHandler::ProcessFoundForm(base, target, false);
				return EventResult::kContinue;
			}
			return EventResult::kContinue;
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Map Marker ) -----
	//---------------------------------------------------

	void BookHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, bool a_supress) {

		if (!FoundItemData.HasForm(a_eventID) && !a_supress) {
			auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, Data.GetForm(a_eventID)->GetName());
			FrameworkAPI::SendNotification(msg, "NotifyBooks");
		}

		FoundItemData.AddForm(a_baseID);
		for (auto var : Data.GetAllVariations()) {
			if (Data.GetBase(var) == a_baseID) {
				FoundItemData.AddForm(var);
			}
		}

		auto t_pos = std::find(FormArray.begin(), FormArray.end(), Data.GetForm(a_baseID));
		auto b_pos = std::distance(FormArray.begin(), t_pos);
		BoolArray.at(b_pos) = true;

		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void BookHandler::InjectAndCompileData() {

		auto handler = RE::TESDataHandler::GetSingleton();
		MCMScript = ScriptObject::FromForm(static_cast<RE::TESForm*>(handler->LookupForm(0x00800, "Completionist.esp")), "Completionist_MCMScript");
		Data.CompileFormArray(CFramework_Maps_TH::Maps, "TreasureHunter.esp");
		Data.MergeAsCollectable();

		CFramework_Maps_TH::Data.Populate(NameArray, FormArray, BoolArray, TextArray, false, 1);

		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void BookHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {
			if (auto* book = static_cast<RE::TESObjectBOOK*>(FormArray[i]); book) {
				if (book->IsRead() || FoundItemData.HasForm(book->GetFormID())) {
					auto base = Data.GetBase(book->GetFormID()) ? Data.GetBase(book->GetFormID()) : book->GetFormID();
					BookHandler::ProcessFoundForm(base, FormArray[i]->GetFormID(), true);
				}
			}
		}

		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM is Entry Complete ) --
	//---------------------------------------------------

	uint32_t BookHandler::IsOptionCompleted(std::string a_name) {

		if (auto t_pos = std::find(NameArray.begin(), NameArray.end(), a_name); t_pos != NameArray.end()) {
			return uint32_t(BoolArray[std::distance(NameArray.begin(), t_pos)]);
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Set Entry Complete ) -
	//---------------------------------------------------

	void BookHandler::SetOptionCompleted(std::string a_name) {

		if (auto t_pos = std::find(NameArray.begin(), NameArray.end(), a_name); t_pos != NameArray.end()) {
			auto b_pos = std::distance(NameArray.begin(), t_pos);

			if (BoolArray.at(b_pos)) {
				BoolArray.at(b_pos) = false;

				FoundItemData.RemoveForm(FormArray.at(b_pos)->GetFormID());
				for (auto var : Data.GetAllVariations()) {
					if (Data.GetBase(var) == FormArray.at(b_pos)->GetFormID()) {
						FoundItemData.RemoveForm(var);
					}
				}
			}
			else {
				BoolArray.at(b_pos) = true;
				FoundItemData.AddForm(FormArray.at(b_pos)->GetFormID());
				for (auto var : Data.GetAllVariations()) {
					if (Data.GetBase(var) == FormArray.at(b_pos)->GetFormID()) {
						FoundItemData.AddForm(var);
					}
				}
			}

			EntriesTotal = FormArray.size();
			EntriesFound = std::ranges::count(BoolArray, true);
		}
	}
}