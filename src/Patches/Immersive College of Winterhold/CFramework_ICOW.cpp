#include "Serialization.hpp"
#include "CFramework_ICOW.hpp"
#include "Internal Utility/Events.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

#undef AddForm

namespace CPatch_ICOW 
{
	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray Items = {
	0x49A45D,0x49A45C,0x49A457,0x49A455,
	};

	constexpr Serialization::Variation Items_V[]{
	{ 0x49A45A, { 0x49A459 } },
	};

	constexpr Serialization::FormArray Books = {
	0x21C81F,0x3515AF,0x49A454,0x21C8A5,0x21C824,0x21C81E,
	0x26EBFC,0x21C825,0x21C823,0x26F15F,0x26F164,0x351610,
	0x21C8A7,
	};

	constexpr Serialization::FormArray Tomes = {
	0x4D710F,0x49A453,0x15CC13,
	};

	constexpr Serialization::FormArray Skill = {
	0x49A45E,0x2E307F,0x35BC11,0x29926C,0x21C822,0x220391,
	0x312E02,0x21C826,0x21C827,0x35BC14 
	};

	// clang-format on

	constexpr std::string_view modname = "CollegeOfWinterholdImmersive.esp";

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
		PatchesInstalled += 1;
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

		if (BookData.HasForm(a_event->book->GetFormID())) {
			auto base = BookData.GetBase(a_event->book->GetFormID()) ? BookData.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
			CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), BookData, Books_FormArray, &Books_BoolArray, &Books_EntriesFound, FrameworkAPI::GetBookLogType(a_event->book), "NotifyBooks");
			return;
		}

		if (SkilData.HasForm(a_event->book->GetFormID())) {
			auto base = SkilData.GetBase(a_event->book->GetFormID()) ? SkilData.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
			CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), SkilData, Skill_FormArray, &Skill_BoolArray, &Skill_EntriesFound, FrameworkAPI::GetBookLogType(a_event->book), "NotifyBooks");
			return;
		}

		if (TomeData.HasForm(a_event->book->GetFormID())) {
			auto base = TomeData.GetBase(a_event->book->GetFormID()) ? TomeData.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
			CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), TomeData, Tomes_FormArray, &Tomes_BoolArray, &Tomes_EntriesFound, FrameworkAPI::GetBookLogType(a_event->book), "NotifyBooks");
			return;
		}
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	void CHandler::OnMenuOpenCloseEvent(RE::MenuOpenCloseEvent const* a_event) {
		using cmd = Serialization::CompletionistLog::logType;

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening)
		{
			if (BookData.HasForm(RE::BookMenu::GetTargetForm()->GetFormID()))
			{
				if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
					auto base = BookData.GetBase(target) ? BookData.GetBase(target) : target;
					CHandler::ProcessFoundForm(base, target, BookData, Books_FormArray, &Books_BoolArray, &Books_EntriesFound, cmd::kBook, "NotifyBooks");
					return;
				}
			}

			if (SkilData.HasForm(RE::BookMenu::GetTargetForm()->GetFormID()))
			{
				if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
					auto base = SkilData.GetBase(target) ? SkilData.GetBase(target) : target;
					CHandler::ProcessFoundForm(base, target, SkilData, Skill_FormArray, &Skill_BoolArray, &Skill_EntriesFound, cmd::kBook, "NotifyBooks");
					return;
				}
			}

			if (TomeData.HasForm(RE::BookMenu::GetTargetForm()->GetFormID()))
			{
				if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
					auto base = TomeData.GetBase(target) ? TomeData.GetBase(target) : target;
					CHandler::ProcessFoundForm(base, target, TomeData, Tomes_FormArray, &Tomes_BoolArray, &Tomes_EntriesFound, cmd::kTome, "NotifyBooks");
					return;
				}
			}
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

		ItemData.CompileFormArray(CPatch_ICOW::Items, modname);
		ItemData.CompileVariation(CPatch_ICOW::Items_V, modname);

		BookData.CompileFormArray(CPatch_ICOW::Books, modname);
		SkilData.CompileFormArray(CPatch_ICOW::Skill, modname);
		TomeData.CompileFormArray(CPatch_ICOW::Tomes, modname);
		
		ItemData.MergeAsCollectable();
		BookData.MergeAsCollectable();
		TomeData.MergeAsCollectable();
		SkilData.MergeAsCollectable();

		ItemData.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);
		BookData.Populate(Books_NameArray, Books_FormArray, Books_BoolArray, Books_TextArray, false, 1);
		TomeData.Populate(Tomes_NameArray, Tomes_FormArray, Tomes_BoolArray, Tomes_TextArray, false, 1);
		SkilData.Populate(Skill_NameArray, Skill_FormArray, Skill_BoolArray, Skill_TextArray, false, 1);

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		Books_EntriesTotal = Books_FormArray.size();
		Books_EntriesFound = std::ranges::count(Books_BoolArray, true);

		Tomes_EntriesTotal = Tomes_FormArray.size();
		Tomes_EntriesFound = std::ranges::count(Tomes_BoolArray, true);

		Skill_EntriesTotal = Skill_FormArray.size();
		Skill_EntriesFound = std::ranges::count(Skill_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Search Terms ) ---
	//---------------------------------------------------

	void CHandler::InstallSearchTerms()
	{
		for (auto i = 0; i < Items_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Items_FormArray[i], Items_NameArray[i], "$MCMPageICOW", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < Books_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Books_FormArray[i], Books_NameArray[i], "$MCMPageICOW", FrameworkAPI::GetBookCategoryType(Books_FormArray[i])));
		}
		for (auto i = 0; i < Tomes_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_FormArray[i], Tomes_NameArray[i], "$MCMPageICOW", FrameworkAPI::GetBookCategoryType(Tomes_FormArray[i])));
		}
		for (auto i = 0; i < Skill_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Skill_FormArray[i], Skill_NameArray[i], "$MCMPageICOW", FrameworkAPI::GetBookCategoryType(Skill_FormArray[i])));
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

		for (auto i = 0; i < Tomes_FormArray.size(); i++) {
			Tomes_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_FormArray[i]);
		}

		for (auto i = 0; i < Skill_FormArray.size(); i++) {
			Skill_BoolArray[i] = FrameworkAPI::IsBookKnown(Skill_FormArray[i]);
		}

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		Books_EntriesTotal = Books_FormArray.size();
		Books_EntriesFound = std::ranges::count(Books_BoolArray, true);

		Tomes_EntriesTotal = Tomes_FormArray.size();
		Tomes_EntriesFound = std::ranges::count(Tomes_BoolArray, true);

		Skill_EntriesTotal = Skill_FormArray.size();
		Skill_EntriesFound = std::ranges::count(Skill_BoolArray, true);
	}
}