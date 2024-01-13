#include "Serialization.hpp"
#include "Internal Utility/Events.hpp"
#include "CFramework_REQ.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_REQ {
	using namespace CFramework_Master;

	// clang-format off
	
	// Armor
	constexpr Serialization::FormArray Armor = {
		0x000D6B,0x003E01,0x003E02,0x003E03,0x0A1167,0x0A1168,0x0A1169,0x0A116A,0x0BED17,0x43FE4C,
		0x45D3AB,0xAD36E1,0xAD3AFF,0xAD3B00,0xAD3B3E,0xAD8CA9,0xAD8CAA,0xAD8CAB,0xAD8CAD,0xADDD76,
		0xADDD77,0xADDD78,0xADDD81,0xADDD82,0xADDD83,0xADDD84,0xADDD85,0xADDD86,0xADDD87,0xADDD88,
		0xADDD89,0xADDD8A,0xADDD8B,0xADDD8C,0xADDD8D,0xADDD8E,0xADDD9A,0xADDD9B,0xADDD9C,0xADDD9D,
		0xADDDA5,0xADDDA6,0xADDDA7,0xADDDA8,0xADDDB1,0xADDDB2,0xADDDB3,0xADDDB4,0xADDDC0,0xADDDC1,
		0xADDDC2,0xADDDC3,0xADDDE0,0xADE426,0xADE427,0xADE428,0xADE434,0xADE435,0xADE436,0xADE437,
	};

	// Misc
	constexpr Serialization::FormArray Misc = {
		0x04FEBC, 0x04FEC0,0x21025A,
	};

	// Weapons
	constexpr Serialization::FormArray Weapon = {
		0x044BFD,0x0731D3,0x0801CC,0x094B3F,0x0AB005,0x169B46,0x1AA25A,0x1AA25B,0x1AA25D,0x1AA260,
		0x1AA261,0x1AA262,0x1AA263,0x352150,0x372455,0x447573,0x5CBCA7,0xADDD6D,0xADDD6E,
	};

	// General Books
	constexpr Serialization::FormArray Book = {
		0x05CBC7,0x05CBCA,0x05CBD4,0x05CBD7,0x05CBDA,0x05CBDC,0x05CBDE,0x05CBE0,0x069947,0x06BA1A,
		0x06BA1D,0x06BA21,0x085D6C,0x08850D,0x0A8863,0x0AB007,0x0C6ADC,0x35B997,0x4BBD05,0x4CAABF,
		0xAD36E8,0xAD3748,0xAD374B,0xAD38D2,
	};

	// Skill Books
	constexpr Serialization::FormArray SkillBook = {
		0x4BBCC5,0x4BBCCB,
	};

	// Spell Tomes
	constexpr Serialization::FormArray SpellTome = {
		0x02D142,0x02D143,0x02D145,0x02D147,0x02D148,0x02FC87,0x02FC88,0x02FC89,0x02FC8A,0x02FC8B,
		0x02FC8C,0x02FC8D,0x02FC8E,0x02FC8F,0x02FC90,0x02FC91,0x02FC92,0x02FC93,0x02FC94,0x02FC95,
		0x02FC96,0x02FC97,0x02FC98,0x02FC99,0x02FC9A,0x02FC9B,0x02FC9C,0x02FC9D,0x02FC9E,0x02FC9F,
		0x02FCA0,0x02FCA1,0x02FCA2,0x02FCA3,0x02FCA4,0x02FCA5,0x02FCA6,0x02FCA7,0x02FCA8,0x02FCA9,
		0x02FCAA,0x02FCAB,0x02FCAC,0x02FCAD,0x02FCAE,0x02FCAF,0x02FCB0,0x033DD8,0x033DDA,0x03CA22,
		0x03DA6C,0x04675B,0x04675C,0x04675D,0x04675E,0x04675F,0x046760,0x06F5B6,0x06F5D0,0x06F5D2,
		0x06F5D3,0x07373B,0x11D598,0x11D59A,0x11D59B,0x11D59C,0x129549,0x153D12,0x15B3E4,0x225F3C,
		0x26DCA5,0x26DCA6,0x270460,0x270461,0x270462,0x270463,0x27CA84,0x27CA86,0x331E47,0x458468,
		0x61FF5B,0x61FF5C,0x61FF5D,0x61FF5E,0x61FF5F,0x61FF60,0x61FF61,0x61FF62,0x61FF64,0x61FF67,
		0xA552E3,0xA552E4,0xA552E5,0xA552E6,0xA552E7,0xA552E8,0xAD38BC,0xAD38BD,0xAD38E0,
	};

	// clang-format on

	constexpr std::string_view modname = "Requiem.esp";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		CHandler::InjectAndCompileData();
		CHandler::InstallSearchTerms();
		FrameworkAPI::AddUpdateFoundForms(CHandler::UpdateFoundForms);
		CEvents::EventHandler::RegisterForEvent_OnBooksReadEvent(CHandler::OnBooksReadEvent);
		CEvents::EventHandler::RegisterForEvent_OnMenuOpenCloseEvent(CHandler::OnMenuOpenCloseEvent);
		CEvents::EventHandler::RegisterForEvent_OnContainerChangedEvent(CHandler::OnContainerChangedEvent);
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	void CHandler::OnContainerChangedEvent(RE::TESContainerChangedEvent const* a_event) {
		using log = Serialization::CompletionistLog::logType;

		if (a_event->newContainer != 0x00014)
		{
			if (ItemDataA.HasForm(a_event->baseObj)) {
				auto base = ItemDataA.GetBase(a_event->baseObj) ? ItemDataA.GetBase(a_event->baseObj) : a_event->baseObj;
				CHandler::ProcessFoundForm(base, a_event->baseObj, ItemDataA, Armor_FormArray, &Armor_BoolArray, &Armor_EntriesFound, log::kCollected, "NotifyItems");
				return;
			}

			if (ItemDataW.HasForm(a_event->baseObj)) {
				auto base = ItemDataW.GetBase(a_event->baseObj) ? ItemDataW.GetBase(a_event->baseObj) : a_event->baseObj;
				CHandler::ProcessFoundForm(base, a_event->baseObj, ItemDataW, Weapon_FormArray, &Weapon_BoolArray, &Weapon_EntriesFound, log::kCollected, "NotifyItems");
				return;
			}

			if (ItemDataM.HasForm(a_event->baseObj)) {
				auto base = ItemDataM.GetBase(a_event->baseObj) ? ItemDataM.GetBase(a_event->baseObj) : a_event->baseObj;
				CHandler::ProcessFoundForm(base, a_event->baseObj, ItemDataM, Misc_FormArray, &Misc_BoolArray, &Misc_EntriesFound, log::kCollected, "NotifyItems");
				return;
			}

		}
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	void CHandler::OnBooksReadEvent(RE::BooksRead::Event const* a_event) {
		using log = Serialization::CompletionistLog::logType;

		if (ItemDataB.HasForm(a_event->book->GetFormID())) {
			auto base = ItemDataB.GetBase(a_event->book->GetFormID()) ? ItemDataB.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
			CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), ItemDataB, Book_FormArray, &Book_BoolArray, &Book_EntriesFound, FrameworkAPI::GetBookLogType(a_event->book), "NotifyBooks");
			return;
		}

		if (ItemDataS.HasForm(a_event->book->GetFormID())) {
			auto base = ItemDataS.GetBase(a_event->book->GetFormID()) ? ItemDataS.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
			CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), ItemDataS, SkillBook_FormArray, &SkillBook_BoolArray, &SkillBook_EntriesFound, FrameworkAPI::GetBookLogType(a_event->book), "NotifyBooks");
			return;
		}

		if (ItemDataT.HasForm(a_event->book->GetFormID())) {
			auto base = ItemDataT.GetBase(a_event->book->GetFormID()) ? ItemDataT.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
			CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), ItemDataT, SpellTome_FormArray, &SpellTome_BoolArray, &SpellTome_EntriesFound, FrameworkAPI::GetBookLogType(a_event->book), "NotifyBooks");
			return;
		}
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	void CHandler::OnMenuOpenCloseEvent(RE::MenuOpenCloseEvent const* a_event) {
		using log = Serialization::CompletionistLog::logType;

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening)
		{
			if (ItemDataB.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) 
			{
				if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
					auto base = ItemDataB.GetBase(target) ? ItemDataB.GetBase(target) : target;
					CHandler::ProcessFoundForm(base, target, ItemDataB, Book_FormArray, &Book_BoolArray, &Book_EntriesFound, log::kBook, "NotifyBooks");
					return;
				}
			}

			if (ItemDataS.HasForm(RE::BookMenu::GetTargetForm()->GetFormID()))
			{
				if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
					auto base = ItemDataS.GetBase(target) ? ItemDataS.GetBase(target) : target;
					CHandler::ProcessFoundForm(base, target, ItemDataS, SkillBook_FormArray, &SkillBook_BoolArray, &SkillBook_EntriesFound, log::kBook, "NotifyBooks");
					return;
				}
			}
			return;
		}
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
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		ItemDataA.CompileFormArray(CPatch_REQ::Armor, modname);
		ItemDataM.CompileFormArray(CPatch_REQ::Misc, modname);

		ItemDataS.CompileFormArray(CPatch_REQ::SkillBook, modname);
		ItemDataB.CompileFormArray(CPatch_REQ::Book, modname);

		ItemDataW.CompileFormArray(CPatch_REQ::Weapon, modname);
		ItemDataT.CompileFormArray(CPatch_REQ::SpellTome, modname);

		ItemDataA.MergeAsCollectable();
		ItemDataM.MergeAsCollectable();

		ItemDataS.MergeAsCollectable();
		ItemDataB.MergeAsCollectable();

		ItemDataW.MergeAsCollectable();
		ItemDataT.MergeAsCollectable();

		ItemDataA.Populate(Armor_NameArray, Armor_FormArray, Armor_BoolArray, Armor_TextArray);
		ItemDataM.Populate(Misc_NameArray, Misc_FormArray, Misc_BoolArray, Misc_TextArray);

		ItemDataS.Populate(SkillBook_NameArray, SkillBook_FormArray, SkillBook_BoolArray, SkillBook_TextArray, false, 1);
		ItemDataB.Populate(Book_NameArray, Book_FormArray, Book_BoolArray, Book_TextArray, false, 1);

		ItemDataW.Populate(Weapon_NameArray, Weapon_FormArray, Weapon_BoolArray, Weapon_TextArray);
		ItemDataT.Populate(SpellTome_NameArray, SpellTome_FormArray, SpellTome_BoolArray, SpellTome_TextArray, false, 1);

		Armor_EntriesTotal = Armor_FormArray.size();
		Armor_EntriesFound = std::ranges::count(Armor_BoolArray, true);

		Misc_EntriesTotal = Misc_FormArray.size();
		Misc_EntriesFound = std::ranges::count(Misc_BoolArray, true);

		SkillBook_EntriesTotal = SkillBook_FormArray.size();
		SkillBook_EntriesFound = std::ranges::count(SkillBook_BoolArray, true);

		Book_EntriesTotal = Book_FormArray.size();
		Book_EntriesFound = std::ranges::count(Book_BoolArray, true);

		Weapon_EntriesTotal = Weapon_FormArray.size();
		Weapon_EntriesFound = std::ranges::count(Weapon_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Search Terms ) ---
	//---------------------------------------------------

	void CHandler::InstallSearchTerms()
	{
		for (auto i = 0; i < Armor_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Armor_FormArray[i], Armor_NameArray[i], "$MCMPageRequiem1", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < Misc_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Misc_FormArray[i], Misc_NameArray[i], "$MCMPageRequiem1", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < SkillBook_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(SkillBook_FormArray[i], SkillBook_NameArray[i], "$MCMPageRequiem2", FrameworkAPI::GetBookCategoryType(SkillBook_FormArray[i])));
		}
		for (auto i = 0; i < Book_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Book_FormArray[i], Book_NameArray[i], "$MCMPageRequiem2", FrameworkAPI::GetBookCategoryType(Book_FormArray[i])));
		}
		for (auto i = 0; i < SpellTome_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(SpellTome_FormArray[i], SpellTome_NameArray[i], "$MCMPageRequiem3", FrameworkAPI::GetBookCategoryType(SpellTome_FormArray[i])));
		}
		for (auto i = 0; i < Weapon_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Weapon_FormArray[i], Weapon_NameArray[i], "$MCMPageRequiem4", std::to_underlying(EntryCategory::kItem)));
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i = 0; i < Armor_FormArray.size(); i++) {
			Armor_BoolArray[i] = FrameworkAPI::IsItemKnown(Armor_FormArray[i], &ItemDataA);
		}

		for (auto i = 0; i < Misc_FormArray.size(); i++) {
			Misc_BoolArray[i] = FrameworkAPI::IsItemKnown(Misc_FormArray[i], &ItemDataM);
		}

		for (auto i = 0; i < SkillBook_FormArray.size(); i++) {
			SkillBook_BoolArray[i] = FrameworkAPI::IsBookKnown(SkillBook_FormArray[i]);
		}

		for (auto i = 0; i < Book_FormArray.size(); i++) {
			Book_BoolArray[i] = FrameworkAPI::IsBookKnown(Book_FormArray[i]);
		}

		for (auto i = 0; i < Weapon_FormArray.size(); i++) {
			Weapon_BoolArray[i] = FrameworkAPI::IsItemKnown(Weapon_FormArray[i], &ItemDataW);
		}

		for (auto i = 0; i < SpellTome_FormArray.size(); i++) {
			SpellTome_BoolArray[i] = FrameworkAPI::IsBookKnown(SpellTome_FormArray[i]);
		}

		Armor_EntriesTotal = Armor_FormArray.size();
		Armor_EntriesFound = std::ranges::count(Armor_BoolArray, true);

		Misc_EntriesTotal = Misc_FormArray.size();
		Misc_EntriesFound = std::ranges::count(Misc_BoolArray, true);

		SkillBook_EntriesTotal = SkillBook_FormArray.size();
		SkillBook_EntriesFound = std::ranges::count(SkillBook_BoolArray, true);

		Book_EntriesTotal = Book_FormArray.size();
		Book_EntriesFound = std::ranges::count(Book_BoolArray, true);

		Weapon_EntriesTotal = Weapon_FormArray.size();
		Weapon_EntriesFound = std::ranges::count(Weapon_BoolArray, true);

		SpellTome_EntriesTotal = SpellTome_FormArray.size();
		SpellTome_EntriesFound = std::ranges::count(SpellTome_BoolArray, true);
	}
}