#include "Ini File Management/Cell Scanner Exclusions.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Frameworks/Items/CFramework_Others.hpp"
#include "Internal Utility/PatchListener.hpp"
#include "CellScanner.hpp"
#include "Variables.hpp"

namespace Completionist
{
	using namespace CFramework_Master;
	using namespace Serialization;
	using namespace CVariables;

	static std::vector<std::tuple<RE::ObjectRefHandle, std::string, std::string, RE::TESForm*, CellScanner::Place, bool>> refsWithNames;
	static std::unordered_map<RE::FormID, RE::FormID> ExcludedVendorChests;
	static std::vector<std::pair<RE::FormID, RE::FormID>> trackedTargets;

	using cmd = Serialization::CompletionistData;

	static RE::TESObjectREFR* GetReference(const RE::ObjectRefHandle& a_handle)
	{
		return a_handle.get().get();
	}

	static auto FindPinnedReferenceEntry(RE::TESForm* a_form)
	{
		return std::find_if(refsWithNames.begin(), refsWithNames.end(),
			[a_form](const auto& tuple) {
				return a_form && std::get<3>(tuple) && std::get<3>(tuple)->GetFormID() == a_form->GetFormID();
			}
		);
	}

	void CellScanner::init()
	{
		CompletionistExclusions::ExclusionHandler::init();
		ExcludeAllVendorChests();
	}

	//---------------------------------------------------
	//-- Serialisation Interface ------------------------
	//---------------------------------------------------

	void CellScanner::ExcludeReference(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESObjectCELL* a_cell)
	{
		if (!a_ref || !a_cell) { return; }
		CFramework_Master::ExcludedCellScannerRefs.AddReference(a_ref->GetFormID(), a_ref->GetDisplayFullName(), a_cell->GetFormID());
	};

	void CellScanner::RemoveExcludedReference(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) { return; }
		CFramework_Master::ExcludedCellScannerRefs.RemoveReference(a_ref->GetFormID());
	};

	std::vector<std::string> CellScanner::GetReferenceFormIDs(RE::StaticFunctionTag*, RE::TESObjectCELL* a_cell)
	{
		if (!a_cell) { return {}; }
		return CFramework_Master::ExcludedCellScannerRefs.GetReferenceFormIDs(a_cell->GetFormID());
	}

	std::vector<std::string> CellScanner::GetReferenceNames(RE::StaticFunctionTag*, RE::TESObjectCELL* a_cell)
	{
		if (!a_cell) { return {}; }
		return CFramework_Master::ExcludedCellScannerRefs.GetReferenceNames(a_cell->GetFormID());
	}

	std::vector<RE::TESObjectREFR*> CellScanner::GetObjectReferences(RE::StaticFunctionTag*, RE::TESObjectCELL* a_cell)
	{
		if (!a_cell) { return {}; }
		return CFramework_Master::ExcludedCellScannerRefs.GetReferenceForms(a_cell->GetFormID());
	}

	//---------------------------------------------------
	//-- Vendor Chest Functions -------------------------
	//---------------------------------------------------

	void CellScanner::ExcludeAllVendorChests() {

		const auto& [allForms, lock] = RE::TESForm::GetAllForms();
		if (!allForms) { return; }

		const RE::BSReadLockGuard locker{ lock };
		for (auto& [id, form] : *allForms)
		{
			if (form && form->formType == RE::FormType::Faction)
			{
				const auto* faction = form->As<RE::TESFaction>();
				if (faction && (faction->IsVendor() || faction->OffersServices()))
				{
					if (const auto* chest = faction->vendorData.merchantContainer; chest)
					{
						ExcludedVendorChests.emplace(chest->GetFormID(), faction->GetFormID());
						//INFO("Excluded Vendor Chest - {} From: {}", std::format("{:08X}", chest->GetFormID()), chest->GetFile()->GetFilename());
					}
				}
			}
		}
		INFO("Excluded {} Vendor Chests", ExcludedVendorChests.size());
	}

	//---------------------------------------------------
	//-- Vendor Chest Functions -------------------------
	//---------------------------------------------------

	static bool IsOwnedByVendor(RE::TESObjectREFR* a_reference)
	{
		if (a_reference && a_reference->GetBaseObject() && a_reference->GetBaseObject()->GetFormType() == RE::FormType::Container)
		{
			const auto* faction = a_reference->GetFactionOwner();
			if (faction && (faction->IsVendor() || faction->OffersServices()) && !ExcludedVendorChests.contains(a_reference->GetFormID()))
			{
				const auto* vendorChest = faction->vendorData.merchantContainer;
				if (vendorChest && !ExcludedVendorChests.contains(vendorChest->GetFormID())) {
					ExcludedVendorChests.emplace(vendorChest->GetFormID(), faction->GetFormID());
					//INFO("Excluded Vendor Chest - {} From: {}", std::format("{:08X}", vendorChest->GetFormID()), vendorChest->GetFile()->GetFilename());
				}

				ExcludedVendorChests.emplace(a_reference->GetFormID(), faction->GetFormID());
				//INFO("Excluded Owned Chest - {} From: {}", std::format("{:08X}", a_reference->GetFormID()), a_reference->GetFile()->GetFilename());
				return true;
			}
		}
		return false;
	}

	//---------------------------------------------------
	//-- Vendor Chest Functions -------------------------
	//---------------------------------------------------

	bool CellScanner::IsModAddedReferenceExcluded(RE::FormID a_formID)
	{
		const auto& vector = CompletionistExclusions::ExclusionHandler::GetExcludedContainersVector();
		return std::ranges::find(vector, a_formID) != vector.end();
	}

	//---------------------------------------------------
	//-- Cell Scanner Function --------------------------
	//---------------------------------------------------

	bool CellScanner::isCellExcluded(RE::StaticFunctionTag*, RE::TESObjectCELL* a_cell) {

		for (auto& formID : CompletionistExclusions::ExclusionHandler::GetExcludedCellsVector())
		{
			if (a_cell && a_cell->GetFormID() == formID) {
				return true;
			}
		};
		return false;
	}

	//---------------------------------------------------
	//-- Vendor Chest Functions -------------------------
	//---------------------------------------------------

	bool CellScanner::IsReferenceExcluded(RE::TESObjectREFR* a_chest)
	{
		if (!a_chest) {
			return true;
		}

		return ExcludedVendorChests.contains(a_chest->GetFormID())
			|| CFramework_Master::ExcludedCellScannerRefs.HasReference(a_chest->GetFormID())
			|| IsModAddedReferenceExcluded(a_chest->GetFormID());
	}

	//---------------------------------------------------
	//-- Collectability Functions -----------------------
	//---------------------------------------------------

	// Nameless and non-playable items never show in inventories
	static bool ShownInInventory(RE::TESBoundObject* a_object)
	{
		const char* name = a_object ? a_object->GetName() : nullptr;
		if (!name || !*name) {
			return false;
		}
		if (const auto* weapon = a_object->As<RE::TESObjectWEAP>()) {
			return !weapon->weaponData.flags.any(RE::TESObjectWEAP::Data::Flag::kNonPlayable);
		}
		if (a_object->Is(RE::FormType::Armor) || a_object->Is(RE::FormType::Misc)) {
			return (a_object->GetFormFlags() & RE::TESObjectARMO::RecordFlags::kNonPlayable) == 0;
		}
		return true;
	}

	// Stones of Barenziah and patch "interactable objects" are tracked by reference, not base item
	static bool IsTrackedReference(RE::TESObjectREFR* a_ref)
	{
		const auto id = a_ref->GetFormID();
		if (FoundItemData_NoShow.HasForm(id)) {
			return false;
		}
		if (CFramework_Others::ItemDataSB.HasForm(id)) {
			return true;
		}
		for (const auto& [page, patch] : CExternalPatchHandler::Get()) {
			for (const auto& section : patch->type_sections) {
				if (section.enabled && section.type == CMiscPatchType::kInteractableObject && section.data.HasForm(id)) {
					return true;
				}
			}
		}
		return false;
	}

	// The item a plant gives, or the first one still needed from its levelled list
	static RE::TESBoundObject* HarvestedItem(RE::TESBoundObject* a_produce)
	{
		const auto* list = a_produce ? a_produce->As<RE::TESLevItem>() : nullptr;
		if (!list) {
			return a_produce;
		}
		for (const auto& entry : list->entries) {
			auto* form = entry.form ? entry.form->As<RE::TESBoundObject>() : nullptr;
			if (form && cmd::CheckIsCollectable(form->GetFormID()) && !FoundItemData.HasForm(form->GetFormID()) && !FoundItemData_NoShow.HasForm(form->GetFormID())) {
				return form;
			}
		}
		return nullptr;
	}

	bool CellScanner::ItemIsCollectable(RE::TESForm* a_form) {
		return cmd::CheckIsCollectable(a_form->GetFormID());
	}

	//---------------------------------------------------
	//-- Collectability Functions -----------------------
	//---------------------------------------------------

	bool CellScanner::ItemIsCollected(RE::TESForm* a_form) {
		return FoundItemData.HasForm(a_form->GetFormID()) || FoundItemData_NoShow.HasForm(a_form->GetFormID());
	}

	//---------------------------------------------------
	//-- Cell Scanner Function --------------------------
	//---------------------------------------------------

	std::vector<CellScanner::Result> CellScanner::Scan(RE::TESObjectCELL* a_cell, const std::vector<std::pair<RE::FormID, RE::FormID>>& a_tracked)
	{
		trackedTargets = a_tracked;
		ScanCell(a_cell);
		trackedTargets.clear();
		std::vector<Result> results;
		results.reserve(refsWithNames.size());
		for (const auto& [ref, name, type, form, place, stealing] : refsWithNames) {
			results.push_back({ ref, form, name, type, place, stealing });
		}
		return results;
	}

	bool CellScanner::IsItemPinnable(RE::StaticFunctionTag*, RE::TESForm* a_form) {
		return a_form && ItemIsCollectable(a_form) && !ItemIsCollected(a_form);
	};

	std::string CellScanner::GetPinnedReferenceName(RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		auto it = FindPinnedReferenceEntry(a_form);
		return it != refsWithNames.end() ? std::get<1>(*it) : "";
	};

	std::string CellScanner::GetPinnedReferenceType(RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		auto it = FindPinnedReferenceEntry(a_form);
		return it != refsWithNames.end() ? std::get<2>(*it) : "";
	};

	RE::TESObjectREFR* CellScanner::GetPinnedReferenceRefr(RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		auto it = FindPinnedReferenceEntry(a_form);
		return it != refsWithNames.end() ? GetReference(std::get<0>(*it)) : nullptr;
	};

	//---------------------------------------------------
	//-- Lambda to sort refs based on distance ----------
	//---------------------------------------------------

	static bool CompareByDistance(const RE::ObjectRefHandle& a, const RE::ObjectRefHandle& b) {
		const auto refA = a.get();
		const auto refB = b.get();
		if (!refA || !refB) {
			return refA != nullptr;
		}

		const auto playerPos = RE::PlayerCharacter::GetSingleton()->GetPosition();
		return refA->GetPosition().GetDistance(playerPos) < refB->GetPosition().GetDistance(playerPos);
	}

	//---------------------------------------------------
	//-- Cell Scanner Function --------------------------
	//---------------------------------------------------

	void CellScanner::ScanCell(RE::TESObjectCELL* cell)
	{
		if (!cell)
		{
			return;
		};

		// All loaded exterior cells count, not just the current one
		std::vector<RE::TESObjectCELL*> cells{ cell };
		if (!cell->IsInteriorCell()) {
			const auto* tes = RE::TES::GetSingleton();
			if (const auto* grid = tes ? tes->gridCells : nullptr) {
				for (std::uint32_t x = 0; x < grid->length; ++x) {
					for (std::uint32_t y = 0; y < grid->length; ++y) {
						if (auto* other = grid->GetCell(x, y); other && other != cell && other->IsAttached()) {
							cells.push_back(other);
						}
					}
				}
			}
		}

		refsWithNames.clear();

		auto* const player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			return;
		}
		const auto  playerPos = player->GetPosition();
		const auto  allowedDistance = CVariables::V_CellScanner_ScanRange / 0.01428;
		auto* const followerFaction = cmd::GetFullForm<RE::TESFaction>(0x05C84E, "Skyrim.esm");

		// The one place an entry is kept or dropped
		const auto add = [&](RE::TESObjectREFR* a_ref, RE::TESForm* a_item, std::string a_name, Place a_place, bool a_stealing) {
			if (CVariables::V_CellScanner_ExcludeOwned && a_stealing) {
				return;
			}
			refsWithNames.emplace_back(a_ref->GetHandle(), std::move(a_name), GetFormType(a_item), a_item, a_place, a_stealing);
		};
		const auto wanted = [](RE::TESForm* a_item) {
			return a_item && !IsExcludedFormType(a_item) && ItemIsCollectable(a_item) && !ItemIsCollected(a_item);
		};

		for (auto* scanCell : cells) {
			for (auto& handle : scanCell->GetRuntimeData().references) {
				auto* ref = handle.get();
				auto* base = ref ? ref->GetBaseObject() : nullptr;
				if (!base || ref == player || ref->IsDeleted() || ref->IsDisabled() || IsReferenceExcluded(ref) || IsOwnedByVendor(ref)) {
					continue;
				}
				if (!cell->IsInteriorCell() && ref->GetPosition().GetDistance(playerPos) > allowedDistance) {
					continue;
				}

				// Entries tracked by the placed reference, like the Stones of Barenziah
				if (IsTrackedReference(ref)) {
					const char* display = ref->GetDisplayFullName();
					add(ref, ref, display ? display : "", Place::kLoose, ref->IsCrimeToActivate());
					continue;
				}

				// Plants give their ingredient, unless already picked
				if (auto* produce = base->As<RE::TESProduceForm>(); produce && (base->Is(RE::FormType::Flora) || base->Is(RE::FormType::Tree))) {
					if ((ref->GetFormFlags() & RE::TESObjectREFR::RecordFlags::kHarvested) == 0) {
						if (auto* item = HarvestedItem(produce->produceItem); wanted(item)) {
							add(ref, item, item->GetName(), Place::kLoose, ref->IsCrimeToActivate());
						}
					}
					continue;
				}

				// Containers and people: whether taking it is theft is down to each item's owner
				auto* actor = ref->As<RE::Actor>();
				if (base->Is(RE::FormType::Container) || actor) {
					if (actor && followerFaction && actor->IsInFaction(followerFaction)) {
						continue;  // a follower's things are the player's
					}
					if (actor && !actor->IsDead() && CVariables::V_CellScanner_ExcludeLiving) {
						continue;
					}
					const bool freeByDefault = actor ? actor->IsDead() || actor->IsPlayerTeammate() : !ref->GetOwner() || ref->IsAnOwner(player, true, false);
					const auto place = actor ? Place::kCarried : Place::kContainer;
					for (auto& [item, data] : ref->GetInventory()) {
						if (data.first <= 0 || !data.second || !wanted(item)) {
							continue;
						}
						if (!ShownInInventory(item)) {
							static std::set<RE::FormID> logged;  // once per item, not every scan
							if (logged.insert(item->GetFormID()).second) {
								INFO("Cell scanner: skipped hidden item {:08X} \"{}\" in {:08X}", item->GetFormID(), item->GetName(), ref->GetFormID());
							}
							continue;
						}
						add(ref, item, item->GetName(), place, !data.second->IsOwnedBy(player, freeByDefault));
					}
					continue;
				}

				// Anything else lying around
				if (wanted(base)) {
					const char* display = ref->GetDisplayFullName();
					if (!display || !*display) {
						static std::unordered_set<RE::FormID> logged;
						if (logged.insert(base->GetFormID()).second) {
							INFO("Cell scanner: skipped nameless item {:08X} at {:08X}, it can't be picked up", base->GetFormID(), ref->GetFormID());
						}
						continue;
					}
					add(ref, base, base->GetName(), Place::kLoose, ref->IsCrimeToActivate());
				}
			}
		}

		// One row per item, the tracked reference first, then not stolen, then closest
		{
			const auto  rank = [&](const auto& a_entry) {
				const auto* ref = std::get<0>(a_entry).get().get();
				const auto* form = std::get<3>(a_entry);
				const bool  tracked = ref && form && std::ranges::find(trackedTargets, std::pair{ ref->GetFormID(), form->GetFormID() }) != trackedTargets.end();
				const float distance = ref && player ? ref->GetPosition().GetDistance(player->GetPosition()) : FLT_MAX;
				return std::tuple{ !tracked, std::get<5>(a_entry), distance };
			};
			std::unordered_map<RE::TESForm*, std::size_t> chosen;
			for (std::size_t i = 0; i < refsWithNames.size(); ++i) {
				const auto [it, added] = chosen.try_emplace(std::get<3>(refsWithNames[i]), i);
				if (!added && rank(refsWithNames[i]) < rank(refsWithNames[it->second])) {
					it->second = i;
				}
			}
			decltype(refsWithNames) kept;
			kept.reserve(chosen.size());
			for (std::size_t i = 0; i < refsWithNames.size(); ++i) {
				if (chosen.at(std::get<3>(refsWithNames[i])) == i) {
					kept.push_back(std::move(refsWithNames[i]));
				}
			}
			refsWithNames = std::move(kept);
		}

		// Sort the vector of pairs based on CompareByDistance
		std::sort(refsWithNames.begin(), refsWithNames.end(), [](const auto& a, const auto& b) {
			return CompareByDistance(std::get<0>(a), std::get<0>(b));
			});
	}

	bool CellScanner::IsExcludedFormType(RE::TESForm* a_form)
	{
		switch (a_form->GetFormType())
		{
		case RE::FormType::Armor: { return CVariables::V_CellScanner_ExcludeArmor; break;}
		case RE::FormType::Book: { return CVariables::V_CellScanner_ExcludeBooks; break;}
		case RE::FormType::Weapon: { return CVariables::V_CellScanner_ExcludeWeapons; break;}
		case RE::FormType::AlchemyItem: { return CVariables::V_CellScanner_ExcludeAlchemy; break;}
		case RE::FormType::Ingredient: { return CVariables::V_CellScanner_ExcludeIngredients; break;}
		case RE::FormType::Misc: { return CVariables::V_CellScanner_ExcludeMiscItems; break;}
		default: return false; break;
		}
		return false;
	}

	//---------------------------------------------------
	//-- Get Item Type ----------------------------------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Get Item Type ----------------------------------
	//---------------------------------------------------

	std::string CellScanner::GetFormType(RE::TESForm* a_form)
	{
		using loc = CLocalisation::LocalisationAPI;

		std::string type{};

		switch (a_form->GetFormType())
		{
		case RE::FormType::Scroll:
			type = loc::GetLocStringByKey("ItemType_Scroll");
			break;

		case RE::FormType::Armor:
			type = loc::GetLocStringByKey("ItemType_Armor");
			break;
		case RE::FormType::Book:
		{
			auto* book = static_cast<RE::TESObjectBOOK*>(a_form);
			if (book && book->GetSpell()) {
				type = loc::GetLocStringByKey("ItemType_SpellTome");
			}
			else if (book && book->TeachesSkill()) {
				type = loc::GetLocStringByKey("ItemType_SkillBook");
			}
			else {
				type = loc::GetLocStringByKey("ItemType_Book");
			}
		}
		break;

		case RE::FormType::Ingredient:
			type = loc::GetLocStringByKey("ItemType_Ingredient");
			break;
		case RE::FormType::Weapon:
			type = loc::GetLocStringByKey("ItemType_Weapon");
			break;
		case RE::FormType::KeyMaster:
			type = loc::GetLocStringByKey("ItemType_Key");
			break;
		case RE::FormType::AlchemyItem:
		{
			if (cmd::HasKeywordString(a_form, "VendorItemPotion")) {
				type = loc::GetLocStringByKey("ItemType_Potion");
				break;
			}

			if (cmd::HasKeywordString(a_form, "VendorItemPoison")) {
				type = loc::GetLocStringByKey("ItemType_Poison");
				break;
			}

			type = loc::GetLocStringByKey("ItemType_Item");
		}
		break;

		case RE::FormType::SoulGem:
			type = loc::GetLocStringByKey("ItemType_SoulGem");
			break;

		default:
			type = loc::GetLocStringByKey("ItemType_Item");
			break;
		}

		return fmt::format("{:s}"sv, type);
	};
};