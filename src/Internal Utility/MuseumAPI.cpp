#include "MuseumAPI.hpp"
#include "Events.hpp"
#include "ScriptObject.hpp"
#include "Serialization.hpp"

#undef GetObject

namespace Completionist
{
	void MuseumAPI::Register()
	{
		SKSE::GetPapyrusInterface()->Register(RegisterFunctions);

		if (!Serialization::CompletionistData::IsModInstalled("LegacyoftheDragonborn.esm"))
			return;

		_handler = RE::TESDataHandler::GetSingleton();
		if (!_handler) INFO("MuseumAPI: Unable to retrieve VM");

		_vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
		if (!_vm) INFO("MuseumAPI: Unable to retrieve VM");

		_policy = _vm->GetObjectHandlePolicy();
		if (!_policy) INFO("MuseumAPI: Unable to retrieve VM Policy");

		_logging = CVariables::VariablesAPI::IsMuseumAPILoggingEnabled();

		CEvents::EventHandler::RegisterForEvent_OnContainerChangedEvent(OnContainerChangedEvent);

		_ready = true;
	}

	void MuseumAPI::OnPlayerLoadGame()
	{
		if (!_ready)
			return;

		_museumContainers.clear();
		_museumInventory.clear();
		_AllVariations.clear();
		_DisplayableFormIDs.clear();

		BuildMuseumInventory();
		FindAndTrackMuseumItems();
		FindAndTrackMuseumPatchItems();
		InsertPatchReplacedItems();
		BuildAllVariations();
		VerifyAltLists();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Papyrus Registrations ) --
	//---------------------------------------------------

	auto MuseumAPI::RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm) -> bool
	{
		a_vm->RegisterFunction("GetMuseumDisplayStatus", "Completionist_Native", GetMuseumDisplayStatus);
		return true;
	};

	RE::BSFixedString MuseumAPI::GetMuseumDisplayStatus(RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!_ready || !a_form || !CVariables::V_MuseumModeEnabled || !IsMuseumDisplayable(a_form))
			return "";

		bool isVariationDisplayedInstead = false;
		return IsDisplayed(a_form, isVariationDisplayedInstead) ? "Displayed" : isVariationDisplayedInstead ? "Occupied" : "Displayable";
	}

	//---------------------------------------------------
	//-- Set Displayable Functions ----------------------
	//---------------------------------------------------

	static bool IsExcludedFormType(RE::FormType a_type)
	{
		using ft = RE::FormType;
		return a_type == ft::Activator || a_type == ft::MovableStatic || a_type == ft::NPC;
	}

	bool MuseumAPI::SetDisplayable(RE::FormID a_formID, std::unordered_set<RE::FormID> a_variations)
	{
		std::size_t originalSize = _DisplayableFormIDs.size();
		auto& existing = _DisplayableFormIDs[a_formID];
		
		std::size_t variationsSize = existing.size();
		existing.insert(a_variations.begin(), a_variations.end());

		return existing.size() > variationsSize || _DisplayableFormIDs.size() > originalSize;
	}

	void MuseumAPI::SetDisplayable(RE::TESForm* a_form, std::unordered_set<RE::TESForm*> a_variations)
	{
		if (!a_form || IsExcludedFormType(a_form->GetFormType()))
			return;

		std::unordered_set<RE::FormID> converted;
		for (auto* var : a_variations)
		{
			if (var)
				converted.insert(var->GetFormID());
		}
		if (SetDisplayable(a_form->GetFormID(), std::move(converted)))
		{
			if (_logging)
			{
				if (a_variations.size() == 0)
				{
					INFO("Set {:08X} - {} as displayable", a_form->GetFormID(), a_form->GetName());
				}
				else
				{
					INFO("Set {:08X} - {} as displayable with the following variations:", a_form->GetFormID(), a_form->GetName());

					for (const auto& variation : a_variations) {
						INFO("  - {:08X} - {}", variation->GetFormID(), variation->GetName());
					}
				}
			}
		}
	}

	//---------------------------------------------------
	//-- Is Displayable Functions -----------------------
	//---------------------------------------------------

	bool MuseumAPI::IsMuseumDisplayable(RE::TESForm* a_form)
	{
		return a_form && IsMuseumDisplayable(a_form->GetFormID());
	}

	bool MuseumAPI::IsMuseumDisplayable(RE::FormID a_formID)
	{
		return _AllVariations.contains(a_formID);
	}

	//---------------------------------------------------
	//-- Is Displayed Functions -------------------------
	//---------------------------------------------------

	bool MuseumAPI::IsDisplayed(RE::FormID a_formID)
	{
		return IsMuseumDisplayable(a_formID) && _museumInventory.contains(a_formID);
	}

	bool MuseumAPI::IsDisplayed(RE::TESForm* a_form)
	{
		return a_form && IsDisplayed(a_form->GetFormID());
	}

	bool MuseumAPI::IsDisplayed(RE::FormID a_formID, bool& matchedByVariation)
	{
		matchedByVariation = false;

		if (IsMuseumDisplayable(a_formID) && _museumInventory.contains(a_formID))
			return true;

		for (const auto& variation : GetVariations(a_formID)) {
			if (IsMuseumDisplayable(variation) && _museumInventory.contains(variation)) {
				matchedByVariation = true;
				return false;
			}
		}

		return false;
	}

	bool MuseumAPI::IsDisplayed(RE::TESForm* a_form, bool& matchedByVariation)
	{
		return a_form && IsDisplayed(a_form->GetFormID(), matchedByVariation);
	}

	//---------------------------------------------------
	//-- Variations Functions ---------------------------
	//---------------------------------------------------

	/// <summary>
	/// Returns true if the targetted formID has variations.
	/// </summary>
	bool MuseumAPI::HasVariations(RE::FormID a_formID)
	{
		auto it = _AllVariations.find(a_formID);
		return it != _AllVariations.end() && !it->second.empty();
	}

	/// <summary>
	/// Returns true if the targetted form has variations.
	/// </summary>
	bool MuseumAPI::HasVariations(RE::TESForm* a_form)
	{
		return a_form && HasVariations(a_form->GetFormID());
	}

	/// <summary>
	/// Gets all variations of the target formID including the base if the passed in formID is a variation itself.
	/// </summary>
	std::unordered_set<RE::FormID> MuseumAPI::GetVariations(RE::FormID a_formID)
	{
		auto it = _AllVariations.find(a_formID);
		return it != _AllVariations.end() ? it->second : std::unordered_set<RE::FormID>{};
	}

	//---------------------------------------------------
	//-- Events -----------------------------------------
	//---------------------------------------------------

	void MuseumAPI::OnContainerChangedEvent(RE::TESContainerChangedEvent const* a_event)
	{
		if (!a_event || a_event->itemCount <= 0)
			return;

		if (_museumContainers.contains(a_event->newContainer))
		{
			_museumInventory[a_event->baseObj] += a_event->itemCount;
			return;
		}

		if (_museumContainers.contains(a_event->oldContainer))
		{
			auto it = _museumInventory.find(a_event->baseObj);
			if (it != _museumInventory.end()) {
				it->second -= a_event->itemCount;

				if (it->second <= 0) {
					_museumInventory.erase(it);
				}
			}
		}
	}

	//---------------------------------------------------
	//-- Functions --------------------------------------
	//---------------------------------------------------

	void MuseumAPI::BuildAllVariations()
	{
		_AllVariations.clear();
		for (const auto& [baseID, variations] : _DisplayableFormIDs) {
			std::unordered_set<RE::FormID> group = variations;
			group.insert(baseID);

			for (const auto& id : group) {
				std::unordered_set<RE::FormID> others;
				for (const auto& otherID : group) {
					if (otherID != id)
						others.insert(otherID);
				}
				_AllVariations[id] = std::move(others);
			}
		}

		//No longer needed once _AllVariations is built.
		_DisplayableFormIDs.clear();

		if (_logging)
		{
			for (const auto& [id, variations] : _AllVariations)
			{
				auto* form = RE::TESForm::LookupByID(id);
				std::string name = form ? form->GetName() : "<unknown>";

				bool isTracked = Serialization::CompletionistData::CheckIsCollectable(form);

				if (variations.empty())
				{
					INFO("Variation group for [{}] {:08X} - {} has no other linked items", isTracked ? "Tracked Item" : "Untracked Item", id, name);
					INFO("  - {}", form ? form->GetFile(0)->GetFilename() : "Unknown Filename");
				}
				else
				{
					INFO("Variation group for [{}] {:08X} [{}] {} includes the following:", isTracked ? "Tracked Item" : "Untracked Item", id, form ? form->GetFile(0)->GetFilename() : "Unknown Filename", name);
					for (const auto& variationID : variations) {
						auto* varForm = RE::TESForm::LookupByID(variationID);
						std::string varName = varForm ? varForm->GetName() : "<unknown>";
						INFO("  - {:08X} - {}", variationID, varName);
						INFO("    - {}", varForm ? varForm->GetFile(0)->GetFilename() : "Unknown Filename");
					}
				}
			}
		}
		INFO("MuseumAPI variations map built with {} items.", _AllVariations.size());
	}

	//---------------------------------------------------
	//-- Functions --------------------------------------
	//---------------------------------------------------

	void MuseumAPI::BuildMuseumInventory()
	{
		auto* armStorage = _handler->LookupForm(0x6C22C2, "LegacyoftheDragonborn.esm");
		if (armStorage)
			_museumContainers.emplace(armStorage->AsReference()->GetFormID());

		auto* bookStorage = _handler->LookupForm<RE::TESObjectREFR>(0x2F8E89, "LegacyoftheDragonborn.esm");
		if (bookStorage)
			_museumContainers.emplace(bookStorage->AsReference()->GetFormID());

		auto* jewelryStorage = _handler->LookupForm<RE::TESObjectREFR>(0x2ACDD9, "LegacyoftheDragonborn.esm");
		if (jewelryStorage)
			_museumContainers.emplace(jewelryStorage->AsReference()->GetFormID());

		auto* mainStorage = _handler->LookupForm<RE::TESObjectREFR>(0x126087, "LegacyoftheDragonborn.esm");
		if (mainStorage)
			_museumContainers.emplace(mainStorage->AsReference()->GetFormID());

		INFO("Building museum inventory...");

		for (auto containerFormID : _museumContainers)
		{
			auto* container = RE::TESForm::LookupByID<RE::TESObjectREFR>(containerFormID);
			if (!container) {
				if (_logging)
					INFO("Failed to find container for FormID: {:08X}", containerFormID);
				continue;
			}

			const auto& containerInventory = container->GetInventory();
			if (containerInventory.empty()) {
				if (_logging)
					INFO("Container {:08X} has no inventory items.", containerFormID);
				continue;
			}

			if (_logging)
				INFO("Processing container {:08X} with {} items.", containerFormID, containerInventory.size());

			for (const auto& [boundObject, inventoryEntryData] : containerInventory)
			{
				if (boundObject) {
					RE::FormID formID = boundObject->GetFormID();
					std::int32_t count = inventoryEntryData.first;
					_museumInventory[formID] = count;
				}
			}
		}

		INFO("Museum Inventory built with {} items.", _museumInventory.size());
	}

	//---------------------------------------------------
	//-- Functions --------------------------------------
	//---------------------------------------------------

	std::unordered_set<RE::TESForm*> MuseumAPI::GetReplicaItems(const RE::TESForm* element, const RE::BGSListForm* replicaBaseItems, const RE::BGSListForm* replicaItems)
	{
		std::unordered_set<RE::TESForm*> result;

		if (!element || !replicaBaseItems || !replicaItems) {
			return result;
		}

		auto safeInsertList = [&result](const RE::BGSListForm* list) {
			if (!list) return;
			for (const auto& item : list->forms) {
				if (item) result.insert(item);
			}
			};

		auto safeInsertForm = [&result](RE::TESForm* form) {
			if (form) result.insert(form);
			};

		auto processPair = [&](RE::TESForm* baseForm, RE::TESForm* correspondingForm) -> bool {
			if (!baseForm || !correspondingForm) return false;

			if (auto baseList = baseForm->As<RE::BGSListForm>()) {
				for (const auto& nested : baseList->forms) {
					if (nested && nested->GetFormID() == element->GetFormID()) {
						for (const auto& other : baseList->forms) {
							if (other && other->GetFormID() != element->GetFormID()) {
								safeInsertForm(other);
							}
						}

						if (auto replicaList = correspondingForm->As<RE::BGSListForm>()) {
							safeInsertList(replicaList);
						}
						else {
							safeInsertForm(correspondingForm);
						}

						return true;
					}
				}
			}
			else if (baseForm->GetFormID() == element->GetFormID()) {
				if (auto replicaList = correspondingForm->As<RE::BGSListForm>()) {
					safeInsertList(replicaList);
				}
				else {
					safeInsertForm(correspondingForm);
				}

				return true;
			}

			return false;
			};

		// ---- Forward lookup ----
		const auto& baseForms = replicaBaseItems->forms;
		const auto& replicaForms = replicaItems->forms;
		auto size = std::min(baseForms.size(), replicaForms.size());

		for (std::size_t i = 0; i < size; ++i) {
			if (processPair(baseForms[i], replicaForms[i])) {
				return result;
			}
		}

		// ---- Reverse lookup ----
		size = std::min(replicaForms.size(), baseForms.size());
		for (std::size_t i = 0; i < size; ++i) {
			if (processPair(replicaForms[i], baseForms[i])) {
				return result;
			}
		}

		return result;
	}

	//---------------------------------------------------
	//-- Functions --------------------------------------
	//---------------------------------------------------

	void MuseumAPI::FindAndTrackMuseumItems()
	{
		if (!_handler) return;

		const auto& replicaBaseItems = _handler->LookupForm<RE::BGSListForm>(0x138795, "LegacyoftheDragonborn.esm");
		const auto& replicaItems = _handler->LookupForm<RE::BGSListForm>(0x138796, "LegacyoftheDragonborn.esm");
		if (!replicaBaseItems || !replicaItems) {
			if (_logging)
				INFO("Unable to get replicaBaseItems or replicaItems formlists.");

			return;
		}

		for (const auto& [mappedFormID, mappedFileName] : museumItemListsMap) 
		{
			const auto& formlist = _handler->LookupForm<RE::BGSListForm>(mappedFormID, mappedFileName);
			if (!formlist) { continue; }

			for (auto i = 0; i < formlist->forms.size(); i++)
			{
				const auto& element = formlist->forms[i];

				if (auto nestedList = element->As<RE::BGSListForm>())
				{
					if (!nestedList->forms.empty())
					{
						auto* mainForm = nestedList->forms[0];
						std::unordered_set<RE::TESForm*> allItems;

						// Add replicas of the main form first
						auto mainReplicas = GetReplicaItems(mainForm, replicaBaseItems, replicaItems);
						allItems.insert(mainReplicas.begin(), mainReplicas.end());

						for (const auto& form : nestedList->forms)
						{
							if (!form || form == mainForm)
								continue;

							allItems.insert(form);

							auto replicas = GetReplicaItems(form, replicaBaseItems, replicaItems);
							allItems.insert(replicas.begin(), replicas.end());
						}

						SetDisplayable(mainForm, std::move(allItems));
					}
				}
				else
				{
					SetDisplayable(element, GetReplicaItems(element, replicaBaseItems, replicaItems));
				}
			}
		}
	}

	//---------------------------------------------------
	//-- Functions --------------------------------------
	//---------------------------------------------------

	void MuseumAPI::FindAndTrackMuseumPatchItems()
	{
		if (!_handler) return;

		const auto& replicaBaseItems = _handler->LookupForm<RE::BGSListForm>(0x138795, "LegacyoftheDragonborn.esm");
		const auto& replicaItems = _handler->LookupForm<RE::BGSListForm>(0x138796, "LegacyoftheDragonborn.esm");
		if (!replicaBaseItems || !replicaItems) {
			if (_logging)
				INFO("Unable to get replicaBaseItems or replicaItems formlists.");

			return;
		}

		const auto& quests = _handler->GetFormArray<RE::TESQuest>();
		if (quests.empty()) return;

		for (auto& quest : quests)
		{
			auto script = ScriptObject::FromForm(quest, "DBMSupportedModScript", false);
			if (!script) continue;

			const auto& itemLists = script.get()->GetProperty("NewSectionItemLists");
			if (!itemLists) continue;

			if (itemLists->IsArray())
			{
				const auto& scriptArrayPointer = itemLists->GetArray().get();
				if (!scriptArrayPointer) continue;

				for (int i = 0; i < scriptArrayPointer->size(); i++)
				{
					const auto& elementPtr = (*scriptArrayPointer)[i].GetObject().get();
					if (!elementPtr) continue;

					const auto& element = _policy->GetObjectForHandle(RE::TESForm::FORMTYPE, elementPtr->GetHandle());
					if (!element) continue;

					if (auto formlist = element->As<RE::BGSListForm>())
					{
						for (auto i = 0; i < formlist->forms.size(); i++)
						{
							const auto& element = formlist->forms[i];

							if (auto nestedList = element->As<RE::BGSListForm>())
							{
								if (!nestedList->forms.empty())
								{
									auto* mainForm = nestedList->forms[0];
									std::unordered_set<RE::TESForm*> allItems;

									// Add replicas of the main form first
									auto mainReplicas = GetReplicaItems(mainForm, replicaBaseItems, replicaItems);
									allItems.insert(mainReplicas.begin(), mainReplicas.end());

									for (const auto& form : nestedList->forms)
									{
										if (!form || form == mainForm)
											continue;

										allItems.insert(form);

										auto replicas = GetReplicaItems(form, replicaBaseItems, replicaItems);
										allItems.insert(replicas.begin(), replicas.end());
									}

									SetDisplayable(mainForm, std::move(allItems));
								}
							}
							else
							{
								SetDisplayable(element, GetReplicaItems(element, replicaBaseItems, replicaItems));
							}
						}
					}
				}
			}
		}
	}

	//---------------------------------------------------
	//-- Functions --------------------------------------
	//---------------------------------------------------

	void MuseumAPI::InsertPatchReplacedItems()
	{
		for (const auto& [mappedFormID, mappedFileName, formVector] : patchedItemMappings)
		{
			const auto& form = _handler->LookupForm(mappedFormID, mappedFileName);
			if (!form) continue;

			if (_DisplayableFormIDs.contains(form->GetFormID()))
			{
				for (const auto& [variationFormID, variationFileName] : formVector)
				{
					const auto& variation = _handler->LookupForm(variationFormID, variationFileName);
					if (variation)
					{
						_DisplayableFormIDs[form->GetFormID()].insert(variation->GetFormID());
					}
				}
			}
		}
	}

	void MuseumAPI::VerifyAltLists()
	{
		if (!_handler) return;

		const auto& replicaBaseItems = _handler->LookupForm<RE::BGSListForm>(0x138795, "LegacyoftheDragonborn.esm");
		const auto& replicaItems = _handler->LookupForm<RE::BGSListForm>(0x138796, "LegacyoftheDragonborn.esm");
		if (!replicaBaseItems || !replicaItems) {
			if (_logging)
				INFO("Unable to get replicaBaseItems or replicaItems formlists.");

			return;
		}

		for (const auto& [mappedFormID, mappedFileName, formListName] : museumItemAltListsMap)
		{
			const auto& formlist = _handler->LookupForm<RE::BGSListForm>(mappedFormID, mappedFileName);
			if (!formlist) { continue; }

			for (auto i = 0; i < formlist->forms.size(); i++)
			{
				const auto& element = formlist->forms[i];

				if (auto nestedList = element->As<RE::BGSListForm>())
				{
					if (!nestedList->forms.empty())
					{
						auto* mainForm = nestedList->forms[0];
						std::unordered_set<RE::TESForm*> allItems;

						for (const auto& form : nestedList->forms)
						{
							if (!form)
								continue;

							allItems.insert(form);
							auto replicas = GetReplicaItems(form, replicaBaseItems, replicaItems);
							allItems.insert(replicas.begin(), replicas.end());
						}

						for (const auto& formToVerify : allItems)
						{
							if (!IsMuseumDisplayable(formToVerify))
								INFO("Critical Error: Undisplayed Form Found in nested list: {} - [{:08X}] in {}", formToVerify->GetName(), formToVerify->GetFormID(), formListName);
						}
					}
				}
				else
				{
					auto replicas = GetReplicaItems(element, replicaBaseItems, replicaItems);

					std::unordered_set<RE::TESForm*> allItems;
					allItems.insert(element);
					allItems.insert(replicas.begin(), replicas.end());

					for (const auto& formToVerify : allItems)
					{
						if (!IsMuseumDisplayable(formToVerify))
							INFO("Critical Error: Undisplayed Form Found: {} - [{:08X}] in {}", formToVerify->GetName(), formToVerify->GetFormID(), formListName);
					}
				}
			}
		}
	}
}
