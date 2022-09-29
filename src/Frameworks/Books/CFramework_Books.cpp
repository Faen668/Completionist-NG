#include "Serialization.hpp"
#include "CFramework_Books.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"

#undef AddForm

namespace CFramework_BooksManager {

	RE::Actor* Player;

	void CHandler::InstallFramework() {

		Player = static_cast<RE::Actor*>(RE::TESForm::LookupByID(0x00000014));

		CFramework_Books_AG::BookHandler::InstallFramework();
		CFramework_Books_HS::BookHandler::InstallFramework();
		CFramework_Books_TY::BookHandler::InstallFramework();
		CFramework_Books_SB::BookHandler::InstallFramework();
		CFramework_Books_ST::BookHandler::InstallFramework();
		
		CFramework_Books_DG::BookHandler::InstallFramework();
		CFramework_Books_DGS::BookHandler::InstallFramework();

		CFramework_Books_DB::BookHandler::InstallFramework();
		CFramework_Books_DBS::BookHandler::InstallFramework();
	}

	void CHandler::UpdateFoundForms() {

		Player = static_cast<RE::Actor*>(RE::TESForm::LookupByID(0x00000014));

		CFramework_Books_AG::BookHandler::UpdateFoundForms();
		CFramework_Books_HS::BookHandler::UpdateFoundForms();
		CFramework_Books_TY::BookHandler::UpdateFoundForms();
		CFramework_Books_SB::BookHandler::UpdateFoundForms();
		CFramework_Books_ST::BookHandler::UpdateFoundForms();

		CFramework_Books_DG::BookHandler::UpdateFoundForms();
		CFramework_Books_DGS::BookHandler::UpdateFoundForms();

		CFramework_Books_DB::BookHandler::UpdateFoundForms();
		CFramework_Books_DBS::BookHandler::UpdateFoundForms();
	}
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------ A - G
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

namespace CFramework_Books_AG {

	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray SkyrimForms = {
	0x001AFB2,0x001AFB3,0x001AFB1,0x001ACE5,0x001ACE3,0x001ACE4,0x001ACDF,0x00ED047,
	0x001AFC0,0x001AFD4,0x001AFC1,0x001B006,0x00ED02F,0x00ED02E,0x00A0322,0x00F1AB3,
	0x00F6843,0x00403AF,0x001B22C,0x001AFF3,0x00EA5B0,0x00ED03F,0x00ED03A,0x0083B38,
	0x001B22D,0x003636A,0x001B22F,0x001ACFE,0x001AFD7,0x001ACE7,0x001AFFC,0x001ACE8,
	0x0080D63,0x0080D64,0x0080D65,0x0080D66,0x00FF227,0x001ACE9,0x001ACC7,0x001ACB5,
	0x001ACB6,0x001ACB7,0x001B233,0x0032E72,0x001ACB9,0x001ACBA,0x001ACBB,0x001ACBC,
	0x001ACFF,0x00ED605,0x001B237,0x001B238,0x001B239,0x001B23A,0x001B23B,0x00ED032,
	0x001B23C,0x001AD04,0x001ACC9,0x00ED040,0x00EF2C0,0x00E2FC5,0x0083168,0x0083169,
	0x008316A,0x001ACEA,0x00E7F31,0x00E7F33,0x00E7F34,0x003010B,0x002D513,0x0048782,
	0x00ED033,0x001ACF7,0x001ACEB,0x001ACEC,0x001ACED,0x001B267,0x001AD05,0x00ED04E,
	0x00E3E69,0x0072841,0x001ACCB,0x001ACCD,0x00ED031,0x001AD07,0x001ACD5,
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
		Data.CompileFormArray(CFramework_Books_AG::SkyrimForms, "Skyrim.esm");
		Data.MergeAsCollectable();

		CFramework_Books_AG::Data.Populate(NameArray, FormArray, BoolArray, TextArray, false, 1);
		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void BookHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {
			if (auto* book = static_cast<RE::TESObjectBOOK*>(FormArray[i]); book) {
				if (book->IsRead() || (book->GetSpell() && CFramework_BooksManager::Player->HasSpell(book->GetSpell())) || FoundItemData.HasForm(book->GetFormID())) {
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
//------------------------------------------------------------------------------------------------------------------------------ H - S
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

namespace CFramework_Books_HS {

	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray SkyrimForms = {
	0x001AD08,0x00ED603,0x00ED60C,0x00ED60B,0x00D6F0B,0x00FBA57,0x00ED604,
	0x001AFF1,0x00EDA90,0x001B245,0x00EDD35,0x00AE324,0x00EF53E,0x001AD09,
	0x001ACDC,0x00ED608,0x00ED042,0x001ACF1,0x0028ADC,0x00F1445,0x001ACD1,
	0x001ACD4,0x001AFC3,0x001ACF5,0x0086EF8,0x0086EF9,0x0086EFA,0x0086EFB,
	0x001ACB8,0x001AD0E,0x001AD0D,0x004D249,0x00ED039,0x00ED161,0x00E0D66,
	0x0063A0F,0x00B6426,0x00ED607,0x00ED035,0x00B64B1,0x001A332,0x00EB090,
	0x001ACF3,0x00ED037,0x00EDA8E,0x001AF40,0x001AFFE,0x001ACF4,0x003010A,
	0x00ED041,0x00E7EF0,0x00F37D0,0x001AD14,0x001ACC6,0x00F11B7,0x00ED5F4,
	0x00ED5F5,0x00ED5F6,0x00ED5F7,0x001ACDB,0x0032785,0x00ED03B,0x00F84A1,
	0x001AF93,0x001B22B,0x00ED04C,0x00F1447,0x00E7F37,0x00ED062,
	0x00F11D5,0x00ED046,0x00ED045,0x00ED03D,0x00ED048,0x00ED044,0x001AD0C,
	0x00E2FC6,0x001B25A,0x001AD15,
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
		Data.CompileFormArray(CFramework_Books_HS::SkyrimForms, "Skyrim.esm");
		Data.MergeAsCollectable();

		CFramework_Books_HS::Data.Populate(NameArray, FormArray, BoolArray, TextArray, false, 1);
		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void BookHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {
			if (auto* book = static_cast<RE::TESObjectBOOK*>(FormArray[i]); book) {
				if (book->IsRead() || (book->GetSpell() && CFramework_BooksManager::Player->HasSpell(book->GetSpell())) || FoundItemData.HasForm(book->GetFormID())) {
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
//------------------------------------------------------------------------------------------------------------------------------ T - Y
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

namespace CFramework_Books_TY {

	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray SkyrimForms = {
	0x007EB03,0x001AF94,0x00ED04B,0x001ACE1,0x00EDA8F,0x001B25E,0x007EB9E,
	0x001AFC2,0x001ACC8,0x0105A52,0x00F86FE,0x00EF638,0x001B262,0x007EBC2,
	0x007EBC9,0x001ACF2,0x001AFCA,0x00EDDD5,0x009DE3D,0x00E0D68,0x001ACD2,
	0x001ACD3,0x0068B5A,0x00F456D,0x00ED030,0x001B26A,0x0072840,0x00C1771,
	0x001AD0A,0x001ACEF,0x00F699D,0x001ACF0,0x001B26E,0x00E0D67,0x00F68AC,
	0x00F68AD,0x010BEDF,0x0037DEA,0x001AD0F,0x001AD11,0x001AD12,0x001AFD3,
	0x001ACD6,0x001ACD7,0x001ACD8,0x001ACD9,0x001ACDA,0x001AD13,0x00F4530,
	0x00DB0C7,0x001ACFA,0x001AF8A,0x001AF8B,0x001AF8C,0x001AF8D,0x001AF8E,
	0x001AF8F,0x001AF90,0x001AF91,0x00F03E3,0x00ED04D,0x001ACFB,0x00ED04F,
	0x00F683F,0x001AD16,0x001AD17,0x001AD18,0x00ED03C,0x0083B3B,0x001AFF2,
	0x001AFFB,0x001B00B,0x001B024,0x001B01E,0x001B026,0x001ACFD,0x00ED601,
	0x00ED5F8,0x001ACDD,0x001ADB4,0x001ACE2,0x00ED606,0x00E1640,0x009F267,
	0x006851B,0x001ACFC,0x001AFBA,0x00ED061,0x001B272,0x00ED63F,
	0x00F11B6,0x00F1446,0x001B27D,0x001B273,0x001B274,0x002A563,
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
		Data.CompileFormArray(CFramework_Books_TY::SkyrimForms, "Skyrim.esm");
		Data.MergeAsCollectable();

		CFramework_Books_TY::Data.Populate(NameArray, FormArray, BoolArray, TextArray, false, 1);
		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void BookHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {
			if (auto* book = static_cast<RE::TESObjectBOOK*>(FormArray[i]); book) {
				if (book->IsRead() || (book->GetSpell() && CFramework_BooksManager::Player->HasSpell(book->GetSpell())) || FoundItemData.HasForm(book->GetFormID())) {
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
//------------------------------------------------------------------------------------------------------------------------------ Skill Books
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

namespace CFramework_Books_SB {

	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray SkyrimForms = {
	0x001AFEA,0x001AFE9,0x001B01F,0x001AFF7,0x001AFD9,0x001B017,0x001B025,0x001B010,
	0x001AFDF,0x001B00D,0x001B00E,0x001AFC4,0x001AFEE,0x002F838,0x001B01C,0x001ACE6,
	0x002F83C,0x001AFDC,0x001B012,0x001AFD6,0x001B023,0x001B236,0x002F83B,0x002F83A,
	0x001AFD1,0x001AFF8,0x001AFC9,0x001AFC7,0x001AFDD,0x002F837,0x001B008,0x001AFDA,
	0x002F836,0x001AFF6,0x001AFD2,0x001AFC8,0x001AFEC,0x001B001,0x001B00F,0x001B002,
	0x001AFE5,0x001AFCF,0x001B021,0x001AFD0,0x001AFE8,0x001AFE6,0x001AFC5,0x001B018,
	0x001AFF0,0x001B013,0x001AFE4,0x001AFF9,0x001B01B,0x001B022,0x001B015,0x001AFCC,
	0x001AFED,0x001B004,0x001B020,0x001AFCB,0x001AFDB,0x001AFC6,0x001B01D,0x001AFCE,
	0x001AFEF,0x001B009,0x001B011,0x001B00A,0x001AFE7,0x001B016,0x001B005,0x001AFE3,
	0x001AFFA,0x001AFE2,0x001B019,0x001AFCD,0x001B26D,0x001AFDE,0x001B000,0x001AFD5,
	0x001B003,0x001AFEB,0x001B01A,0x001AFBF,0x001B276,0x002F839,0x001B007,0x001AFE0,
	0x001B014,0x001AFD8,
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
		Data.CompileFormArray(CFramework_Books_SB::SkyrimForms, "Skyrim.esm");
		Data.MergeAsCollectable();

		CFramework_Books_SB::Data.Populate(NameArray, FormArray, BoolArray, TextArray, false, 1);
		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void BookHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {
			if (auto* book = static_cast<RE::TESObjectBOOK*>(FormArray[i]); book) {
				if (book->IsRead() || (book->GetSpell() && CFramework_BooksManager::Player->HasSpell(book->GetSpell())) || FoundItemData.HasForm(book->GetFormID())) {
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

namespace CFramework_Books_ST {

	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray SkyrimForms = {
	0x00DD647,0x00A26EE,0x00A270D,0x00A26ED,0x00A26F1,0x009E2A9,0x00A271B,0x00A2711,
	0x009E2A7,0x00A2708,0x00A2728,0x00FF7D1,0x010F64D,0x00A26F6,0x010FD60,0x009E2AB,
	0x00A26EC,0x00B45F7,0x00A26EF,0x00A26F0,0x009E2AD,0x00A26F9,0x00A26E7,0x00A26EA,
	0x00D2B4E,0x00A26F7,0x00A26E9,0x00F4997,0x00A26F8,0x00A271D,0x00A2712,0x00A2706,
	0x00A26FD,0x00A2700,0x00A270C,0x00A2703,0x009CD51,0x00A26FA,0x00A2714,0x009CD52,
	0x00A2704,0x00A2701,0x00A26FB,0x009E2AC,0x00DD643,0x00A2722,0x00FDE7B,0x00A271A,
	0x009E2AF,0x00A271E,0x00A2727,0x00A271C,0x00A26FE,0x00A2707,0x010F7F3,0x010F7F4,
	0x00A2715,0x00A26E4,0x009E2AE,0x00A26FF,0x00A2705,0x00A270E,0x00A26E2,0x00DD646,
	0x00A2719,0x00A270F,0x009E2A8,0x00A2717,0x00A26E8,0x009E2AA,0x00A2713,0x00A26EB,
	0x00A2725,0x00A2726,0x00A26F2,0x00A2718,0x00A2702,0x009CD54,0x009CD53,0x00B3165,
	0x00A2720,0x00A26E3,0x00A26FC,0x00A26E5,0x010F7F5,0x0109112,0x00A2729,0x00A271F,
	0x00A2721,0x00A2709,0x00A270A,0x00A270B,0x00A26E6,
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

	RE::Actor* Player;

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
		
		Data.CompileFormArray(CFramework_Books_ST::SkyrimForms, "Skyrim.esm");

		Data.MergeAsCollectable();

		CFramework_Books_ST::Data.Populate(NameArray, FormArray, BoolArray, TextArray, false, 1);
		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void BookHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {

			if (auto* book = static_cast<RE::TESObjectBOOK*>(FormArray[i]); book) {
				if (book->IsRead() || (book->GetSpell() && CFramework_BooksManager::Player->HasSpell(book->GetSpell())) || FoundItemData.HasForm(book->GetFormID())) {
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
//------------------------------------------------------------------------------------------------------------------------------ Dawnguard Books
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

namespace CFramework_Books_DG {

	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray BookForms = {
	0x016692,0x01A3E7,0x014010,0x014011,0x014012,0x014013,0x014014,0x014015,
	0x014016,0x014017,0x014018,0x014019,0x01A3E6,0x00D070,0x003F79,0x004D5B,
	0x01A3E5,0x00FAC2,0x006925,0x01A3E8,0x01A3E0,0x01A3E1,0x01A3E2,0x01A3E3,
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

	RE::Actor* Player;

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

		Data.CompileFormArray(CFramework_Books_DG::BookForms, "Dawnguard.esm");
		Data.MergeAsCollectable();

		CFramework_Books_DG::Data.Populate(NameArray, FormArray, BoolArray, TextArray, false, 1);
		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void BookHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {

			if (auto* book = static_cast<RE::TESObjectBOOK*>(FormArray[i]); book) {
				if (book->IsRead() || (book->GetSpell() && CFramework_BooksManager::Player->HasSpell(book->GetSpell())) || FoundItemData.HasForm(book->GetFormID())) {
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
//------------------------------------------------------------------------------------------------------------------------------ Dawnguard Spell Tomes
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

namespace CFramework_Books_DGS {

	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray BookForms = {
	0x0045B0,0x0045B1,0x0045B2,0x00E8D6,0x00E8D5,0x003F4E,0x003F51,0x003F4D,
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

	RE::Actor* Player;

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

		Data.CompileFormArray(CFramework_Books_DGS::BookForms, "Dawnguard.esm");

		Data.MergeAsCollectable();

		CFramework_Books_DGS::Data.Populate(NameArray, FormArray, BoolArray, TextArray, false, 1);
		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void BookHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {

			if (auto* book = static_cast<RE::TESObjectBOOK*>(FormArray[i]); book) {
				if (book->IsRead() || (book->GetSpell() && CFramework_BooksManager::Player->HasSpell(book->GetSpell())) || FoundItemData.HasForm(book->GetFormID())) {
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
//------------------------------------------------------------------------------------------------------------------------------ Dragonborn Books
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

namespace CFramework_Books_DB {

	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray BookForms = {
	0x033BD8,0x016E2C,0x01E99E,0x01E99F,0x01E99D,0x01E99C,0x016E2D,0x016E22,
	0x028261,0x028262,0x028263,0x03ABCC,0x028264,0x03661A,0x03AF1B,0x03AF1C,
	0x03AF1D,0x028269,0x02BAC7,0x02826A,0x02826B,0x03A35F,0x03A360,0x03A361,
	0x03A362,0x02826C,0x02826D,0x028265,0x028267,0x03ABCD,0x028268,0x02826E,
	0x02826F,0x028270,0x028271,0x028272,0x028273,0x028274,0x03B052,0x03B063,
	0x03B064,0x028275,0x03B3A5,0x029102,0x028276,0x028266,0x01D8D0,0x028277,
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

	RE::Actor* Player;

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

		Data.CompileFormArray(CFramework_Books_DB::BookForms, "Dragonborn.esm");
		Data.MergeAsCollectable();

		CFramework_Books_DB::Data.Populate(NameArray, FormArray, BoolArray, TextArray, false, 1);
		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void BookHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {

			if (auto* book = static_cast<RE::TESObjectBOOK*>(FormArray[i]); book) {
				if (book->IsRead() || (book->GetSpell() && CFramework_BooksManager::Player->HasSpell(book->GetSpell())) || FoundItemData.HasForm(book->GetFormID())) {
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
//------------------------------------------------------------------------------------------------------------------------------ Dragonborn Spell Tomes
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

namespace CFramework_Books_DBS {

	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray BookForms = {
	0x0177B5,0x0177AC,0x01CE07,0x01AABA,0x01E2B1,0x033C67,0x0177BD,0x020685,
	0x0177AD,
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

	RE::Actor* Player;

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

		Data.CompileFormArray(CFramework_Books_DBS::BookForms, "Dragonborn.esm");

		Data.MergeAsCollectable();

		CFramework_Books_DBS::Data.Populate(NameArray, FormArray, BoolArray, TextArray, false, 1);
		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void BookHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {

			if (auto* book = static_cast<RE::TESObjectBOOK*>(FormArray[i]); book) {
				if (book->IsRead() || (book->GetSpell() && CFramework_BooksManager::Player->HasSpell(book->GetSpell())) || FoundItemData.HasForm(book->GetFormID())) {
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