#include "MuseumAPI.hpp"
#include "Events.hpp"
#include "ScriptObject.hpp"
#include "Serialization.hpp"

#undef GetObject

namespace Completionist
{
	void MuseumAPI::Register()
	{
		if (!Serialization::CompletionistData::IsModInstalled("LegacyoftheDragonborn.esm"))
			return;

		_handler = RE::TESDataHandler::GetSingleton();
		_vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
		_policy = _vm ? _vm->GetObjectHandlePolicy() : nullptr;
		if (!_handler || !_vm || !_policy) {
			INFO("MuseumAPI: Unable to retrieve the data handler, VM or VM policy");
			return;
		}

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
		FindAndTrackMuseumAltItems();
		BuildAllVariations();
		VerifyAltLists();
	}

	//---------------------------------------------------
	//-- Set Displayable Functions ----------------------
	//---------------------------------------------------

	static bool IsExcludedFormType(RE::FormType a_type)
	{
		using ft = RE::FormType;
		return a_type == ft::Activator || a_type == ft::MovableStatic || a_type == ft::NPC;
	}

	static std::string_view FileOf(RE::TESForm* a_form)
	{
		auto* file = a_form ? a_form->GetFile(0) : nullptr;
		return file ? file->GetFilename() : "Unknown Filename";
	}

	// "Snow Prince's Boots (Replica)" -> "Snow Prince's Boots"
	static std::string OriginalName(std::string_view a_name)
	{
		constexpr std::string_view suffix = " (replica)";
		if (a_name.size() <= suffix.size())
			return {};

		const auto tail = a_name.substr(a_name.size() - suffix.size());
		const auto same = [](char a, char b) { return std::tolower(static_cast<unsigned char>(a)) == b; };
		if (!std::ranges::equal(tail, suffix, same))
			return {};

		return std::string(a_name.substr(0, a_name.size() - suffix.size()));
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
						if (variation)
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

		const bool toMuseum = _museumContainers.contains(a_event->newContainer);
		const bool fromMuseum = _museumContainers.contains(a_event->oldContainer);

		// Moved between two museum containers, so the count doesn't change
		if (toMuseum && fromMuseum)
			return;

		if (toMuseum)
		{
			_museumInventory[a_event->baseObj] += a_event->itemCount;
			return;
		}

		if (fromMuseum)
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

		// Groups that share an item become one group
		std::unordered_map<RE::FormID, RE::FormID> parent;
		const auto root = [&parent](RE::FormID a_id) {
			parent.try_emplace(a_id, a_id);
			while (parent[a_id] != a_id) {
				parent[a_id] = parent[parent[a_id]];
				a_id = parent[a_id];
			}
			return a_id;
		};

		for (const auto& [baseID, variations] : _DisplayableFormIDs) {
			const auto baseRoot = root(baseID);
			for (const auto& id : variations) {
				const auto other = root(id);
				if (other != baseRoot)
					parent[other] = baseRoot;
			}
		}

		std::unordered_map<RE::FormID, std::unordered_set<RE::FormID>> groups;
		for (const auto& [id, _] : parent)
			groups[root(id)].insert(id);

		for (const auto& [_, group] : groups) {
			for (const auto& id : group) {
				auto& others = _AllVariations[id];
				others = group;
				others.erase(id);
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
					INFO("  - {}", FileOf(form));
				}
				else
				{
					INFO("Variation group for [{}] {:08X} [{}] {} includes the following:", isTracked ? "Tracked Item" : "Untracked Item", id, FileOf(form), name);
					for (const auto& variationID : variations) {
						auto* varForm = RE::TESForm::LookupByID(variationID);
						std::string varName = varForm ? varForm->GetName() : "<unknown>";
						INFO("  - {:08X} - {}", variationID, varName);
						INFO("    - {}", FileOf(varForm));
					}
				}
			}
		}
		INFO("MuseumAPI variations map built with {} items.", _AllVariations.size());

		// Items the museum displays that Completionist doesn't track
		std::vector<std::pair<RE::TESForm*, bool>> untracked;
		for (const auto& [id, variations] : _AllVariations)
		{
			auto* form = RE::TESForm::LookupByID(id);
			if (!form || Serialization::CompletionistData::CheckIsCollectable(form))
				continue;

			const bool variationTracked = std::ranges::any_of(variations, [](RE::FormID a_id) {
				return Serialization::CompletionistData::CheckIsCollectable(a_id);
			});
			untracked.emplace_back(form, variationTracked);
		}

		std::ranges::sort(untracked, [](const auto& a, const auto& b) {
			return a.second != b.second ? !a.second : std::string_view(a.first->GetName()) < b.first->GetName();
		});

		INFO("Museum check: {} displayable items aren't tracked", untracked.size());
		for (const auto& [form, variationTracked] : untracked) {
			INFO("  - {:08X} [{}] {}{}", form->GetFormID(), FileOf(form), form->GetName(), variationTracked ? " (another version is tracked)" : "");
		}
	}

	//---------------------------------------------------
	//-- Functions --------------------------------------
	//---------------------------------------------------

	void MuseumAPI::BuildMuseumInventory()
	{
		auto* armStorage = _handler->LookupForm<RE::TESObjectREFR>(0x6C22C2, "LegacyoftheDragonborn.esm");
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
				if (!element) continue;

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
							if (!element) continue;

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

	// Alt lists hold items a display also takes that the replica lists don't link,
	// such as the Snow Prince armour replicas and the empty Bloodstone Chalice
	void MuseumAPI::FindAndTrackMuseumAltItems()
	{
		if (!_handler) return;

		const auto& replicaBaseItems = _handler->LookupForm<RE::BGSListForm>(0x138795, "LegacyoftheDragonborn.esm");
		const auto& replicaItems = _handler->LookupForm<RE::BGSListForm>(0x138796, "LegacyoftheDragonborn.esm");
		if (!replicaBaseItems || !replicaItems) {
			if (_logging)
				INFO("Unable to get replicaBaseItems or replicaItems formlists.");

			return;
		}

		// Everything tracked so far, and by name for replicas
		std::unordered_set<RE::FormID> tracked;
		std::unordered_map<std::string, RE::FormID> trackedByName;
		const auto track = [&](RE::FormID a_id) {
			if (!tracked.insert(a_id).second)
				return;
			auto* form = RE::TESForm::LookupByID(a_id);
			if (form && *form->GetName())
				trackedByName.try_emplace(form->GetName(), a_id);
		};

		for (const auto& [baseID, variations] : _DisplayableFormIDs) {
			track(baseID);
			for (const auto& id : variations)
				track(id);
		}

		// A replica's recipe checks the player has the original
		std::unordered_map<RE::FormID, RE::FormID> originalOf;
		std::unordered_map<RE::FormID, RE::FormID> replicaOf;
		for (auto* recipe : _handler->GetFormArray<RE::BGSConstructibleObject>())
		{
			if (!recipe || !recipe->createdItem)
				continue;

			for (auto* condition = recipe->conditions.head; condition; condition = condition->next)
			{
				if (condition->data.functionData.function != RE::FUNCTION_DATA::FunctionID::kGetItemCount)
					continue;

				if (auto* original = static_cast<RE::TESForm*>(condition->data.functionData.params[0])) {
					originalOf.try_emplace(recipe->createdItem->GetFormID(), original->GetFormID());
					replicaOf.try_emplace(original->GetFormID(), recipe->createdItem->GetFormID());
					break;
				}
			}
		}

		// Tracked item to link to: one already in the group, the other end of a replica recipe, or the name without "(Replica)"
		const auto findAnchor = [&](const std::vector<RE::TESForm*>& a_group) -> RE::TESForm* {
			for (auto* form : a_group) {
				if (tracked.contains(form->GetFormID()))
					return form;
			}
			for (const auto* links : { &originalOf, &replicaOf }) {
				for (auto* form : a_group) {
					auto it = links->find(form->GetFormID());
					if (it != links->end() && tracked.contains(it->second))
						return RE::TESForm::LookupByID(it->second);
				}
			}
			for (auto* form : a_group) {
				auto it = trackedByName.find(OriginalName(form->GetName()));
				if (it != trackedByName.end())
					return RE::TESForm::LookupByID(it->second);
			}
			return nullptr;
		};

		for (const auto& [mappedFormID, mappedFileName, formListName] : museumItemAltListsMap)
		{
			const auto& formlist = _handler->LookupForm<RE::BGSListForm>(mappedFormID, mappedFileName);
			if (!formlist) { continue; }

			for (const auto& element : formlist->forms)
			{
				if (!element)
					continue;

				std::vector<RE::TESForm*> members;
				if (auto nestedList = element->As<RE::BGSListForm>()) {
					for (const auto& form : nestedList->forms) {
						if (form)
							members.push_back(form);
					}
				}
				else {
					members.push_back(element);
				}

				std::vector<RE::TESForm*> group;
				for (auto* form : members) {
					group.push_back(form);
					for (auto* replica : GetReplicaItems(form, replicaBaseItems, replicaItems))
						group.push_back(replica);
				}

				if (group.empty())
					continue;

				auto* anchor = findAnchor(group);
				if (!anchor)
					anchor = group.front();

				std::unordered_set<RE::TESForm*> variations(group.begin(), group.end());
				variations.erase(anchor);

				SetDisplayable(anchor, std::move(variations));

				track(anchor->GetFormID());
				for (auto* form : group)
					track(form->GetFormID());
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
				if (!element) continue;

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
