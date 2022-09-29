#include "Serialization.hpp"
#include "CFramework_CreationClub.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"

#undef AddForm

namespace CFramework_CCManager {

	RE::Actor* Player;

	void CHandler::InstallFramework() {

		Player = static_cast<RE::Actor*>(RE::TESForm::LookupByID(0x00000014));

		CFramework_CreationClub_A::CHandler::InstallFramework();
		CFramework_CreationClub_I::CHandler::InstallFramework();
		CFramework_CreationClub_W::CHandler::InstallFramework();
		CFramework_CreationClub_L::CHandler::InstallFramework();
		CFramework_CreationClub_B::BookHandler::InstallFramework();
		CFramework_CreationClub_S::BookHandler::InstallFramework();
	}

	void CHandler::UpdateFoundForms() {

		Player = static_cast<RE::Actor*>(RE::TESForm::LookupByID(0x00000014));

		CFramework_CreationClub_A::CHandler::UpdateFoundForms();
		CFramework_CreationClub_I::CHandler::UpdateFoundForms();
		CFramework_CreationClub_W::CHandler::UpdateFoundForms();
		CFramework_CreationClub_L::CHandler::UpdateFoundForms();
		CFramework_CreationClub_B::BookHandler::InstallFramework();
		CFramework_CreationClub_S::BookHandler::InstallFramework();
	}
}


//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------ Locations
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------


namespace CFramework_CreationClub_L {

	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray Locations = {
	0x00000,
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

	void CHandler::InstallFramework() {

		CHandler::Sink();
		CHandler::InjectAndCompileData();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Get Singleton ) ----------
	//---------------------------------------------------

	auto CHandler::GetSingleton() -> CHandler* {
		static CHandler singleton;
		return std::addressof(singleton);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::Sink() {
		auto ui = RE::UI::GetSingleton();
		ui->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	auto CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) -> RE::BSEventNotifyControl {

		if (a_event->menuName == RE::MapMenu::MENU_NAME && a_event->opening) {

			for (auto i = 0; i < FormArray.size(); i++) {
				CHandler::ProcessMapMarker(FormArray[i], i);
			}
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Map Marker ) -----
	//---------------------------------------------------

	void CHandler::ProcessMapMarker(RE::TESForm* a_form, uint32_t a_pos) {

		auto* a_marker = static_cast<RE::TESObjectREFR*>(a_form);

		if (a_marker) {
			if (auto extraMapMarker = Serialization::CompletionistData::GetMapMarkerInternal(a_marker); extraMapMarker && extraMapMarker->mapData) {
				if (extraMapMarker->mapData->flags.all(RE::MapMarkerData::Flag::kVisible, RE::MapMarkerData::Flag::kCanTravelTo) && !a_marker->IsDisabled()) {
					BoolArray[a_pos] = true;
					FoundItemData.AddForm(a_marker);
				}
			}
		}

		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		auto handler = RE::TESDataHandler::GetSingleton();
		MCMScript = ScriptObject::FromForm(static_cast<RE::TESForm*>(handler->LookupForm(0x00800, "Completionist.esp")), "Completionist_MCMScript");

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse059-ba_dragonplate.esl"); Mod) {
			Data.AddForm(0x00085C, Mod->GetFilename());
		}

		if (const auto* Mod = handler->LookupLoadedModByName("cceejsse005-cave.esm"); Mod) {
			Data.AddForm(0x127259, Mod->GetFilename());
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccbgssse031-advcyrus.esm"); Mod) {
			Data.AddForm(0x000F74, Mod->GetFilename());
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccvsvsse004-beafarmer.esl"); Mod) {
			Data.AddForm(0x000B63, Mod->GetFilename());
		}

		if (const auto* Mod = handler->LookupLoadedModByName("cctwbsse001-puzzledungeon.esm"); Mod) {
			Data.AddForm(0x0038D6, Mod->GetFilename());
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccasvsse001-almsivi.esm"); Mod) {
			Data.AddForm(0x0BC826, Mod->GetFilename());
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse005-goldbrand.esl"); Mod) {
			Data.AddForm(0x000B86, Mod->GetFilename());
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("cceejsse004-hall.esl"); Mod) {
			Data.AddForm(0x000AFF, Mod->GetFilename());
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("cceejsse002-tower.esl"); Mod) {
			Data.AddForm(0x000FA2, Mod->GetFilename());
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccafdsse001-dwesanctuary.esm"); Mod) {
			Data.AddForm(0x070E0A, Mod->GetFilename());
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("cceejsse003-hollow.esl"); Mod) {
			Data.AddForm(0x000F72, Mod->GetFilename());
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccbgssse067-daedinv.esm"); Mod) {
			Data.AddForm(0x1D67E3, Mod->GetFilename());
			Data.AddForm(0x1D67E4, Mod->GetFilename());
			Data.AddForm(0x1D67E7, Mod->GetFilename());
		}

		if (const auto* Mod = handler->LookupLoadedModByName("cceejsse001-hstead.esm"); Mod) {
			Data.AddForm(0x000B8A, Mod->GetFilename());
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccbgssse016-umbra.esm"); Mod) {
			Data.AddForm(0x00CD93, Mod->GetFilename());
		}

		Data.CompileFormArray(CFramework_CreationClub_L::Locations, "");
		CFramework_CreationClub_L::Data.Populate(NameArray, FormArray, BoolArray, TextArray, false, 2);

		TextArray.clear();
		for (auto& name : NameArray) {
			TextArray.push_back("$AddLocationHighlight{" + name + "}");
		}

		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {
			if (FoundItemData.HasForm(FormArray[i]->GetFormID())) {
				BoolArray[i] = true;
			}
		}

		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}
}


//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------ Books
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------


namespace CFramework_CreationClub_B {

	using namespace CFramework_Master;

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

		//Add Forms

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse063-ba_ebony.esl"); Mod) {
			Data.AddForm(0x00085B, Mod->GetFilename()); // The Crimson Dirks, v1
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse056-ba_silver.esl"); Mod) {
			Data.AddForm(0x000824, Mod->GetFilename()); // The Crimson Dirks, v2
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse050-ba_daedric.esl"); Mod) {
			Data.AddForm(0x00081A, Mod->GetFilename()); // The Crimson Dirks, v3
			Data.AddForm(0x000819, Mod->GetFilename()); // Death of a Crimson Dirk
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse060-ba_dragonscale.esl"); Mod) {
			Data.AddForm(0x000819, Mod->GetFilename()); // The Crimson Dirks, v4
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse051-ba_daedricmail.esl"); Mod) {
			Data.AddForm(0x000847, Mod->GetFilename()); // The Crimson Dirks, v5
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse055-ba_orcishscaled.esl"); Mod) {
			Data.AddForm(0x000839, Mod->GetFilename()); // The Crimson Dirks, v6
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse064-ba_elven.esl"); Mod) {
			Data.AddForm(0x000828, Mod->GetFilename()); // The Crimson Dirks, v7
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse062-ba_dwarvenmail.esl"); Mod) {
			Data.AddForm(0x000821, Mod->GetFilename()); // The Crimson Dirks, v8
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse061-ba_dwarven.esl"); Mod) {
			Data.AddForm(0x00081A, Mod->GetFilename()); // The Crimson Dirks, v9
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccbgssse031-advcyrus.esm"); Mod) {
			Data.AddForm(0x000809, Mod->GetFilename()); // The Restless
		}

		if (const auto* Mod = handler->LookupLoadedModByName("cctwbsse001-puzzledungeon.esm"); Mod) {
			Data.AddForm(0x0008A2, Mod->GetFilename()); // Forgotten Seasons, v1
			Data.AddForm(0x0008AA, Mod->GetFilename()); // Log of Elberon the Great, Vol 2
			Data.AddForm(0x0008A0, Mod->GetFilename()); // Log of Elberon the Great, Vol 3
			Data.AddForm(0x0008AB, Mod->GetFilename()); // Log of Elberon the Great, Vol 4
			Data.AddForm(0x0008A1, Mod->GetFilename()); // Log of Elberon the Great, Vol 5
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccedhsse003-redguard.esl"); Mod) {
			Data.AddForm(0x00083D, Mod->GetFilename()); // The Alik'r
			Data.AddForm(0x0008D3, Mod->GetFilename()); // The Ebon Arm
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccbgssse025-advdsgs.esm"); Mod) {
			Data.AddForm(0x000AEC, Mod->GetFilename()); // Heretical Thoughts
			Data.AddForm(0x000AEE, Mod->GetFilename()); // Saints and Seducers
			Data.AddForm(0x000B97, Mod->GetFilename()); // The Blessings of Sheogorath
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse045-hasedoki.esl"); Mod) {
			Data.AddForm(0x000804, Mod->GetFilename()); // The Light and the Dark
			//Data.AddForm(0x000805, Mod->GetFilename()); // Arkay the Enemy
			Data.AddForm(0x000839, Mod->GetFilename()); // To Raise the Living
		}

		//Finish Adding Forms
		Data.MergeAsCollectable();
		CFramework_CreationClub_B::Data.Populate(NameArray, FormArray, BoolArray, TextArray, false, 1);
		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void BookHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {
			if (auto* book = static_cast<RE::TESObjectBOOK*>(FormArray[i]); book) {
				if (book->IsRead() || (book->GetSpell() && CFramework_CCManager::Player->HasSpell(book->GetSpell())) || FoundItemData.HasForm(book->GetFormID())) {
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
//------------------------------------------------------------------------------------------------------------------------------ Spell Tomes
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------


namespace CFramework_CreationClub_S {

	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray Books = {
	0x00000,
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

		//Add Forms

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse014-spellpack01.esl"); Mod) {
			Data.AddForm(0x000815, Mod->GetFilename()); // Spell Tome: Unbounded Flames
			Data.AddForm(0x000816, Mod->GetFilename()); // Spell Tome: Unbounded Freezing
			Data.AddForm(0x000817, Mod->GetFilename()); // Spell Tome: Unbounded Storms
			Data.AddForm(0x000818, Mod->GetFilename()); // Spell Tome: Paralysis Rune
			Data.AddForm(0x000861, Mod->GetFilename()); // Spell Tome: Pride of Hirstaang
			Data.AddForm(0x000862, Mod->GetFilename()); // Spell Tome: Elemental Flare
			Data.AddForm(0x000863, Mod->GetFilename()); // Spell Tome: Elemental Bolt
			Data.AddForm(0x000864, Mod->GetFilename()); // Spell Tome: Elemental Burst
			Data.AddForm(0x000865, Mod->GetFilename()); // Spell Tome: Elemental Blast
			Data.AddForm(0x000866, Mod->GetFilename()); // Spell Tome: Orum's Aquatic Escape
			Data.AddForm(0x000867, Mod->GetFilename()); // Spell Tome: Fenrik's Welcome
			Data.AddForm(0x000868, Mod->GetFilename()); // Spell Tome: Mara's Wrath
			Data.AddForm(0x000869, Mod->GetFilename()); // Spell Tome: Choking Grasp
			Data.AddForm(0x00086A, Mod->GetFilename()); // Spell Tome: Strangulation
			Data.AddForm(0x00086B, Mod->GetFilename()); // Spell Tome: Hangman's Noose
			Data.AddForm(0x00086C, Mod->GetFilename()); // Spell Tome: Touch of Death
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse002-exoticarrows.esl"); Mod) {
			Data.AddForm(0x000816, Mod->GetFilename()); // Spell Tome: Bound Quiver
			Data.AddForm(0x00082B, Mod->GetFilename()); // Spell Tome: Telekinesis Arrows
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccvsvsse003-necroarts.esl"); Mod) {
			Data.AddForm(0x000889, Mod->GetFilename()); // Spell Tome: Banish Undead
			Data.AddForm(0x00088A, Mod->GetFilename()); // Spell Tome: Conjure Undying Ghost
			Data.AddForm(0x00088B, Mod->GetFilename()); // Spell Tome: Conjure Ancient Deathpriest
			Data.AddForm(0x00088C, Mod->GetFilename()); // Spell Tome: Conjure Skeleton Minion
			Data.AddForm(0x00088D, Mod->GetFilename()); // Spell Tome: Necromancer's Ritual
			Data.AddForm(0x00088E, Mod->GetFilename()); // Spell Tome: Conjure Cursed Spectre
			Data.AddForm(0x00088F, Mod->GetFilename()); // Spell Tome: Conjure Haunting Spirit
			Data.AddForm(0x000890, Mod->GetFilename()); // Spell Tome: Conjure Tomb Guardian
			Data.AddForm(0x000891, Mod->GetFilename()); // Spell Tome: Conjure Bone Colossus
			Data.AddForm(0x000892, Mod->GetFilename()); // Spell Tome: Conjure Skeleton Warlock
			Data.AddForm(0x000893, Mod->GetFilename()); // Spell Tome: Conjure Skeleton Champion
			Data.AddForm(0x000894, Mod->GetFilename()); // Spell Tome: Conjure Skeleton Marksman
			Data.AddForm(0x000896, Mod->GetFilename()); // Spell Tome: Soul Split
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse003-zombies.esl"); Mod) {
			Data.AddForm(0x00086E, Mod->GetFilename()); // Spell Tome: Conjure Zombie
			Data.AddForm(0x00086F, Mod->GetFilename()); // Spell Tome: Conjure Foul Zombie
			Data.AddForm(0x000870, Mod->GetFilename()); // Spell Tome: Conjure Putrid Zombie
			Data.AddForm(0x000871, Mod->GetFilename()); // Spell Tome: Conjure Malignant Zombie
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccbgssse025-advdsgs.esm"); Mod) {
			Data.AddForm(0x000AF0, Mod->GetFilename()); // Spell Tome: Conjure Golden Saint Archer
			Data.AddForm(0x000AFD, Mod->GetFilename()); // Spell Tome: Conjure Golden Saint Warrior
			Data.AddForm(0x000B0C, Mod->GetFilename()); // Spell Tome: Conjure Dark Seducer Archer
			Data.AddForm(0x000B63, Mod->GetFilename()); // Spell Tome: Conjure Dark Seducer Warrior
			Data.AddForm(0x19D3F9, Mod->GetFilename()); // Spell Tome: Conjure Staada
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccbgssse067-daedinv.esm"); Mod) {
			Data.AddForm(0x000B64, Mod->GetFilename()); // Spell Tome: Conjure Daedric Horse
			Data.AddForm(0x147D93, Mod->GetFilename()); // Spell Tome: Conjure Ayleid Lich
		}

		//Finish Adding Forms

		Data.CompileFormArray(CFramework_CreationClub_S::Books, "");
		Data.MergeAsCollectable();

		CFramework_CreationClub_S::Data.Populate(NameArray, FormArray, BoolArray, TextArray, false, 1);
		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void BookHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {
			if (auto* book = static_cast<RE::TESObjectBOOK*>(FormArray[i]); book) {
				if (book->IsRead() || (book->GetSpell() && CFramework_CCManager::Player->HasSpell(book->GetSpell())) || FoundItemData.HasForm(book->GetFormID())) {
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
//------------------------------------------------------------------------------------------------------------------------------ Armor
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

namespace CFramework_CreationClub_A {

	using namespace CFramework_Master;

	// clang-format off
	constexpr Serialization::FormArray Armor = {
	0x00000,
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

	void CHandler::InstallFramework() {

		CHandler::RegisterEvents();
		CHandler::InjectAndCompileData();
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {

		if (!a_event || a_event->newContainer != 0x00014 || !Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = Data.GetBase(a_event->baseObj) ? Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj);
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID) {

		if (!FoundItemData.HasForm(a_eventID)) {
			auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, Data.GetForm(a_eventID)->GetName());
			FrameworkAPI::SendNotification(msg, "NotifyItems");
		}

		FoundItemData.AddForm(a_baseID);
		for (auto var : Data.GetAllVariations()) {
			if (Data.GetBase(var) == a_baseID) {
				FoundItemData.AddForm(var);
			}
		}

		auto t_pos = std::find(FormArray.begin(), FormArray.end(), Data.GetForm(a_baseID));
		auto b_pos = std::distance(FormArray.begin(), t_pos);
		BoolArray[b_pos] = true;

		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		auto handler = RE::TESDataHandler::GetSingleton();
		MCMScript = ScriptObject::FromForm(static_cast<RE::TESForm*>(handler->LookupForm(0x00800, "Completionist.esp")), "Completionist_MCMScript");

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse051-ba_daedricmail.esl"); Mod) {
			Data.AddForm(0x000823, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse059-ba_dragonplate.esl"); Mod) {
			Data.AddForm(0x00086B, Mod->GetFilename()); // 
			Data.AddForm(0x000869, Mod->GetFilename()); // 
			Data.AddForm(0x000867, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccpewsse002-armsofchaos.esl"); Mod) {
			Data.AddForm(0x000813, Mod->GetFilename()); // 
			Data.AddForm(0x00080F, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedModByName("cceejsse005-cave.esm"); Mod) {
			Data.AddForm(0x089EEF, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccffbsse001-imperialdragon.esl"); Mod) {
			Data.AddForm(0x000D62, Mod->GetFilename()); // 
			Data.AddForm(0x00084F, Mod->GetFilename()); // 
			Data.AddForm(0x000D61, Mod->GetFilename()); // 
			Data.AddForm(0x000850, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccbgssse031-advcyrus.esm"); Mod) {
			Data.AddForm(0x00089E, Mod->GetFilename()); // 
			Data.AddForm(0x00089B, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccmtysse001-knightsofthenine.esl"); Mod) {
			Data.AddForm(0x000800, Mod->GetFilename()); // 
			Data.AddForm(0x000801, Mod->GetFilename()); // 
			Data.AddForm(0x000802, Mod->GetFilename()); // 
			Data.AddForm(0x000803, Mod->GetFilename()); // 
			Data.AddForm(0x000804, Mod->GetFilename()); // 
			Data.AddForm(0x000805, Mod->GetFilename()); // 
			Data.AddForm(0x000806, Mod->GetFilename()); // 
			Data.AddForm(0x000807, Mod->GetFilename()); // 
			Data.AddForm(0x000808, Mod->GetFilename()); // 
			Data.AddForm(0x000809, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedModByName("cctwbsse001-puzzledungeon.esm"); Mod) {
			Data.AddForm(0x00088F, Mod->GetFilename()); // 
			Data.AddForm(0x000892, Mod->GetFilename()); // 
			Data.AddForm(0x00089F, Mod->GetFilename()); // 
			Data.AddForm(0x000890, Mod->GetFilename()); // 
			Data.AddForm(0x000894, Mod->GetFilename()); // 
			Data.AddForm(0x000895, Mod->GetFilename()); // 
			Data.AddForm(0x000893, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccrmssse001-necrohouse.esl"); Mod) {
			Data.AddForm(0x000CCF, Mod->GetFilename()); // 
			Data.AddForm(0x000D3D, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccasvsse001-almsivi.esm"); Mod) {
			Data.AddForm(0x15E949, Mod->GetFilename()); // 
			Data.AddForm(0x15E948, Mod->GetFilename()); // 
			Data.AddForm(0x00081F, Mod->GetFilename()); // 
			Data.AddForm(0x000821, Mod->GetFilename()); // 
			Data.AddForm(0x000820, Mod->GetFilename()); // 
			Data.AddForm(0x000822, Mod->GetFilename()); // 
			Data.AddForm(0x0C1929, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse005-goldbrand.esl"); Mod) {
			Data.AddForm(0x000807, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse021-lordsmail.esl"); Mod) {
			Data.AddForm(0x000D63, Mod->GetFilename()); // 
			Data.AddForm(0x000804, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse041-netchleather.esl"); Mod) {
			Data.AddForm(0x00092B, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccbgssse025-advdsgs.esm"); Mod) {
			Data.AddForm(0x000AE4, Mod->GetFilename()); // 
			Data.AddForm(0x183E63, Mod->GetFilename()); // 
			Data.AddForm(0x19D389, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse008-wraithguard.esl"); Mod) {
			Data.AddForm(0x000D63, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse069-contest.esl"); Mod) {
			Data.AddForm(0x000813, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse020-graycowl.esl"); Mod) {
			Data.AddForm(0x000D62, Mod->GetFilename()); // 
		}

		Data.CompileFormArray(CFramework_CreationClub_A::Armor, "");
		Data.MergeAsCollectable();

		CFramework_CreationClub_A::Data.Populate(NameArray, FormArray, BoolArray, TextArray);
		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {
			if (FoundItemData.HasForm(FormArray[i]->GetFormID())) {
				BoolArray[i] = true;
			}
		}

		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
//-- Framework Functions ( MCM is Entry Complete ) --
//---------------------------------------------------

	uint32_t CHandler::IsOptionCompleted(std::string a_name) {

		if (auto t_pos = std::find(NameArray.begin(), NameArray.end(), a_name); t_pos != NameArray.end()) {
			return uint32_t(BoolArray[std::distance(NameArray.begin(), t_pos)]);
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Set Entry Complete ) -
	//---------------------------------------------------

	void CHandler::SetOptionCompleted(std::string a_name) {

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
//------------------------------------------------------------------------------------------------------------------------------ Items
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

namespace CFramework_CreationClub_I {

	using namespace CFramework_Master;

	// clang-format off
	constexpr Serialization::FormArray Items = {
	0x00000,
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

	void CHandler::InstallFramework() {

		CHandler::RegisterEvents();
		CHandler::InjectAndCompileData();
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {

		if (!a_event || a_event->newContainer != 0x00014 || !Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = Data.GetBase(a_event->baseObj) ? Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj);
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID) {

		if (!FoundItemData.HasForm(a_eventID)) {
			auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, Data.GetForm(a_eventID)->GetName());
			FrameworkAPI::SendNotification(msg, "NotifyItems");
		}

		FoundItemData.AddForm(a_baseID);
		for (auto var : Data.GetAllVariations()) {
			if (Data.GetBase(var) == a_baseID) {
				FoundItemData.AddForm(var);
			}
		}

		auto t_pos = std::find(FormArray.begin(), FormArray.end(), Data.GetForm(a_baseID));
		auto b_pos = std::distance(FormArray.begin(), t_pos);
		BoolArray[b_pos] = true;

		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		auto handler = RE::TESDataHandler::GetSingleton();
		MCMScript = ScriptObject::FromForm(static_cast<RE::TESForm*>(handler->LookupForm(0x00800, "Completionist.esp")), "Completionist_MCMScript");

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccpewsse002-armsofchaos.esl"); Mod) {
			Data.AddForm(0x000814, Mod->GetFilename()); // 
			Data.AddForm(0x000815, Mod->GetFilename()); //
			Data.AddForm(0x000816, Mod->GetFilename()); //
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccffbsse001-imperialdragon.esl"); Mod) {
			Data.AddForm(0x000868, Mod->GetFilename()); // 
			Data.AddForm(0x000867, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedModByName("cctwbsse001-puzzledungeon.esm"); Mod) {
			Data.AddForm(0x03326A, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse005-goldbrand.esl"); Mod) {
			Data.AddForm(0x00080A, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccbgssse025-advdsgs.esm"); Mod) {
			Data.AddForm(0x12DD3A, Mod->GetFilename()); // 
			Data.AddForm(0x12DD3B, Mod->GetFilename()); // 
			Data.AddForm(0x12DD3C, Mod->GetFilename()); // 
			Data.AddForm(0x183E61, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse019-staffofsheogorath.esl"); Mod) {
			Data.AddForm(0x00080F, Mod->GetFilename()); // 
			Data.AddForm(0x00080E, Mod->GetFilename()); // 
		}

		Data.CompileFormArray(CFramework_CreationClub_I::Items, "");
		Data.MergeAsCollectable();

		CFramework_CreationClub_I::Data.Populate(NameArray, FormArray, BoolArray, TextArray);
		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {
			if (FoundItemData.HasForm(FormArray[i]->GetFormID())) {
				BoolArray[i] = true;
			}
		}

		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
//-- Framework Functions ( MCM is Entry Complete ) --
//---------------------------------------------------

	uint32_t CHandler::IsOptionCompleted(std::string a_name) {

		if (auto t_pos = std::find(NameArray.begin(), NameArray.end(), a_name); t_pos != NameArray.end()) {
			return uint32_t(BoolArray[std::distance(NameArray.begin(), t_pos)]);
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Set Entry Complete ) -
	//---------------------------------------------------

	void CHandler::SetOptionCompleted(std::string a_name) {

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
//------------------------------------------------------------------------------------------------------------------------------ Weapons
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

namespace CFramework_CreationClub_W {

	using namespace CFramework_Master;

	// clang-format off
	constexpr Serialization::FormArray Weapons = {
	0x00000,
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

	void CHandler::InstallFramework() {

		CHandler::RegisterEvents();
		CHandler::InjectAndCompileData();
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {

		if (!a_event || a_event->newContainer != 0x00014 || !Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = Data.GetBase(a_event->baseObj) ? Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj);
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID) {

		if (!FoundItemData.HasForm(a_eventID)) {
			auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, Data.GetForm(a_eventID)->GetName());
			FrameworkAPI::SendNotification(msg, "NotifyItems");
		}

		FoundItemData.AddForm(a_baseID);
		for (auto var : Data.GetAllVariations()) {
			if (Data.GetBase(var) == a_baseID) {
				FoundItemData.AddForm(var);
			}
		}

		auto t_pos = std::find(FormArray.begin(), FormArray.end(), Data.GetForm(a_baseID));
		auto b_pos = std::distance(FormArray.begin(), t_pos);
		BoolArray[b_pos] = true;

		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		auto handler = RE::TESDataHandler::GetSingleton();
		MCMScript = ScriptObject::FromForm(static_cast<RE::TESForm*>(handler->LookupForm(0x00800, "Completionist.esp")), "Completionist_MCMScript");

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccpewsse002-armsofchaos.esl"); Mod) {
			Data.AddForm(0x00080D, Mod->GetFilename()); // 
			Data.AddForm(0x00080B, Mod->GetFilename()); //
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse038-bowofshadows.esl"); Mod) {
			Data.AddForm(0x000807, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse007-chrysamere.esl"); Mod) {
			Data.AddForm(0x000D63, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse013-dawnfang.esl"); Mod) {
			Data.AddForm(0x000C92, Mod->GetFilename()); // 
			Data.AddForm(0x000D63, Mod->GetFilename()); //
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccbgssse031-advcyrus.esm"); Mod) {
			Data.AddForm(0x000A4B, Mod->GetFilename()); // 
			Data.AddForm(0x000D63, Mod->GetFilename()); //
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccmtysse001-knightsofthenine.esl"); Mod) {
			Data.AddForm(0x00080C, Mod->GetFilename()); // 
			Data.AddForm(0x00080D, Mod->GetFilename()); //
		}

		if (const auto* Mod = handler->LookupLoadedModByName("cctwbsse001-puzzledungeon.esm"); Mod) {
			Data.AddForm(0x0008DE, Mod->GetFilename()); // 
			Data.AddForm(0x0008DD, Mod->GetFilename()); //
			Data.AddForm(0x0008DF, Mod->GetFilename()); //
			Data.AddForm(0x0008E2, Mod->GetFilename()); //
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccrmssse001-necrohouse.esl"); Mod) {
			Data.AddForm(0x000CF2, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccasvsse001-almsivi.esm"); Mod) {
			Data.AddForm(0x057384, Mod->GetFilename()); // 
			Data.AddForm(0x000C28, Mod->GetFilename()); //
			Data.AddForm(0x13101E, Mod->GetFilename()); //
			Data.AddForm(0x13101B, Mod->GetFilename()); //
			Data.AddForm(0x11CBF0, Mod->GetFilename()); //
			Data.AddForm(0x00083A, Mod->GetFilename()); //
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse005-goldbrand.esl"); Mod) {
			Data.AddForm(0x000D61, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse068-bloodfall.esl"); Mod) {
			Data.AddForm(0x000803, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccedhsse003-redguard.esl"); Mod) {
			Data.AddForm(0x0008D6, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse004-ruinsedge.esl"); Mod) {
			Data.AddForm(0x000D63, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccbgssse025-advdsgs.esm"); Mod) {
			Data.AddForm(0x000BE3, Mod->GetFilename()); // 
			Data.AddForm(0x00083B, Mod->GetFilename()); //
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse018-shadowrend.esl"); Mod) {
			Data.AddForm(0x000D62, Mod->GetFilename(), 0x000D63); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse045-hasedoki.esl"); Mod) {
			Data.AddForm(0x000806, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse019-staffofsheogorath.esl"); Mod) {
			Data.AddForm(0x000801, Mod->GetFilename()); // 
			Data.AddForm(0x000D62, Mod->GetFilename()); //
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse006-stendarshammer.esl"); Mod) {
			Data.AddForm(0x000D62, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse008-wraithguard.esl"); Mod) {
			Data.AddForm(0x000801, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccbgssse067-daedinv.esm"); Mod) {
			Data.AddForm(0x000B5E, Mod->GetFilename()); // 
			Data.AddForm(0x147D9F, Mod->GetFilename()); //
			Data.AddForm(0x000B5F, Mod->GetFilename()); //
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse069-contest.esl"); Mod) {
			Data.AddForm(0x000815, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccbgssse020-graycowl.esl"); Mod) {
			Data.AddForm(0x000850, Mod->GetFilename()); // 
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccbgssse016-umbra.esm"); Mod) {
			Data.AddForm(0x000D62, Mod->GetFilename()); // 
		}

		Data.CompileFormArray(CFramework_CreationClub_W::Weapons, "");
		Data.MergeAsCollectable();

		CFramework_CreationClub_W::Data.Populate(NameArray, FormArray, BoolArray, TextArray);
		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {
			if (FoundItemData.HasForm(FormArray[i]->GetFormID())) {
				BoolArray[i] = true;
			}
		}

		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM is Entry Complete ) --
	//---------------------------------------------------

	uint32_t CHandler::IsOptionCompleted(std::string a_name) {

		if (auto t_pos = std::find(NameArray.begin(), NameArray.end(), a_name); t_pos != NameArray.end()) {
			return uint32_t(BoolArray[std::distance(NameArray.begin(), t_pos)]);
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Set Entry Complete ) -
	//---------------------------------------------------

	void CHandler::SetOptionCompleted(std::string a_name) {

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