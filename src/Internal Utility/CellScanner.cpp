#include "Ini File Management/Cell Scanner Exclusions.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "CellScanner.hpp"
#include "Variables.hpp"

namespace Completionist
{
	using namespace CFramework_Master;
	using namespace Serialization;
	using namespace CVariables;

	static std::vector<RE::TESObjectREFR*> refs;
	static std::vector<std::string> names;
	static std::vector<std::string> types;
	static std::vector<RE::FormID> formIDs;
	static std::vector<std::tuple<RE::ObjectRefHandle, std::string, std::string, RE::TESForm*, CellScanner::Place, bool>> refsWithNames;
	static std::unordered_map<RE::FormID, RE::FormID> ExcludedVendorChests;
	static int32_t currentReferencePosition = -1;
	static std::vector<std::pair<RE::FormID, RE::FormID>> trackedTargets;

	using cmd = Serialization::CompletionistData;

	static RE::TESObjectREFR* GetReference(const RE::ObjectRefHandle& a_handle)
	{
		return a_handle.get().get();
	}

	static auto* GetCurrentReferenceEntry()
	{
		const bool valid = currentReferencePosition >= 0 && currentReferencePosition < static_cast<int32_t>(refsWithNames.size());
		return valid ? &refsWithNames[currentReferencePosition] : nullptr;
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

	std::string CellScanner::GetQuestMarkerReferenceFormID(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) { return "-"; }
		return cmd::GetFormIDHexString(a_ref);
	}

	std::string CellScanner::GetQuestMarkerReferenceOwner(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) { return "-"; }
		return cmd::GetFormOwner(a_ref);
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

	void CellScanner::CheckForReferences(RE::StaticFunctionTag*, RE::TESObjectCELL* cell, bool a_logging, bool b_notify)
	{
		ScanCell(cell, a_logging, b_notify);
	}

	std::vector<CellScanner::Result> CellScanner::Scan(RE::TESObjectCELL* a_cell, const std::vector<std::pair<RE::FormID, RE::FormID>>& a_tracked)
	{
		trackedTargets = a_tracked;
		ScanCell(a_cell, false, false);
		trackedTargets.clear();
		std::vector<Result> results;
		results.reserve(refsWithNames.size());
		for (const auto& [ref, name, type, form, place, stealing] : refsWithNames) {
			results.push_back({ ref, form, name, type, place, stealing });
		}
		return results;
	}

	bool CellScanner::HasPinnedFormInCell(RE::StaticFunctionTag*, RE::TESObjectCELL* cell, RE::TESForm* a_form) {
		return a_form && FindPinnedReferenceEntry(a_form) != refsWithNames.end();
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

	void CellScanner::ScanCell(RE::TESObjectCELL* cell, bool a_logging, bool b_notify)
	{
		std::unordered_set<RE::TESForm*> forms;

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

		forms.clear();
		refs.clear();
		names.clear();
		types.clear();
		formIDs.clear();
		refsWithNames.clear();
		currentReferencePosition = -1;


		// Exclude Owned Items
		auto* const player = RE::PlayerCharacter::GetSingleton();
		const bool  excludeOwned = CVariables::V_CellScanner_ExcludeOwned && player;

		for (auto* scanCell : cells) {
			for (auto& ref : scanCell->GetRuntimeData().references) {
				if (!ref || !ref.get() || !ref->GetBaseObject() || ref.get() == RE::PlayerCharacter::GetSingleton() || ref->IsDeleted() || ref->IsDisabled()) {
					continue;
				}

				//Is an excluded reference.
				if (IsReferenceExcluded(ref.get())) {
					//INFO("Reference - {} - Is Excluded And Cannot Be Accessed - {}", std::format("{:08X}", ref.get()->GetFormID()), ref.get()->GetFile()->GetFilename());
					continue;
				}

				//Is a vendor chest.
				if (IsOwnedByVendor(ref.get())) {
					//INFO("Reference - {} - Is Owned By A Vendor And Cannot Be Accessed - {}", std::format("{:08X}", ref.get()->GetFormID()), ref.get()->GetFile()->GetFilename());
					continue;
				}

				auto allowed_distance = (CVariables::V_CellScanner_ScanRange / 0.01428);

				switch (ref.get()->GetBaseObject()->GetFormType()) {
				case RE::FormType::Container:
				{
					if (V_CellScanner_CONT) {
						if (auto* container = ref.get()->GetBaseObject()->As<RE::TESContainer>()) {
							auto inv = ref.get()->GetInventory();
							const bool containerIsPlayers = !ref.get()->GetOwner() || ref.get()->IsAnOwner(player, true, false);

							for (auto& [obj, data] : inv) {
								if (data.first > 0 && data.second) {
									// Taking it would be stealing.
									const bool stealing = player && !data.second->IsOwnedBy(player, containerIsPlayers);
									if (excludeOwned && stealing) {
										continue;
									}

									//Is a reference of an excluded type in the MCM.
									if (IsExcludedFormType(obj)) {
										continue;
									}

									if (!ShownInInventory(obj)) {
										static std::set<RE::FormID> logged;  // once per item, not every scan
										if (ItemIsCollectable(obj) && !ItemIsCollected(obj) && logged.insert(obj->GetFormID()).second) {
											INFO("Cell scanner: skipped hidden item {:08X} \"{}\" in {:08X}", obj->GetFormID(), obj->GetName(), ref.get()->GetFormID());
										}
										continue;
									}

									if (ItemIsCollectable(obj) && !ItemIsCollected(obj)) {
										//RE::PlayerCharacter::GetSingleton()->MoveTo(ref.get());
										//INFO("Found container item: {}", obj->GetName());

										if (!cell->IsInteriorCell()) {
											auto distance = ref.get()->GetPosition().GetDistance(RE::PlayerCharacter::GetSingleton()->GetPosition());

											if (distance > allowed_distance) {
												continue;
											}
										}

										forms.emplace(obj);

										refsWithNames.emplace_back(std::make_tuple(ref->GetHandle(), obj->GetName(), GetFormType(obj), obj, CellScanner::Place::kContainer, stealing));
									}
								}

							}
						}
					}
					break;
				}

				case RE::FormType::NPC:
				{
					if (V_CellScanner_NPCS) {
						if (auto* NPC = ref->As<RE::Actor>()) {

							auto* FollowerFaction = cmd::GetFullForm<RE::TESFaction>(0x05C84E, "Skyrim.esm");

							if (FollowerFaction && NPC->IsInFaction(FollowerFaction)) {
								//INFO("{} is in the follower faction", NPC->GetName());
								break;
							}

							// Pickpocketing, the dead and teammates are fine
							const bool pickpocketing = !NPC->IsDead() && !NPC->IsPlayerTeammate();
							if (excludeOwned && pickpocketing) {
								break;
							}

							auto inv = NPC->GetInventory();
							for (auto& [obj, data] : inv) {
								if (data.first > 0 && data.second) {

									//Is a reference of an excluded type in the MCM.
									if (IsExcludedFormType(obj)) {
										continue;
									}

									if (!ShownInInventory(obj)) {
										static std::set<RE::FormID> logged;  // once per item, not every scan
										if (ItemIsCollectable(obj) && !ItemIsCollected(obj) && logged.insert(obj->GetFormID()).second) {
											INFO("Cell scanner: skipped hidden item {:08X} \"{}\" in {:08X}", obj->GetFormID(), obj->GetName(), ref.get()->GetFormID());
										}
										continue;
									}

									if (ItemIsCollectable(obj) && !ItemIsCollected(obj)) {
										//INFO("Found NPC Item: {} on - [{}]", obj->GetName(), NPC->GetName());

										if (!cell->IsInteriorCell()) {
											auto distance = ref.get()->GetPosition().GetDistance(RE::PlayerCharacter::GetSingleton()->GetPosition());

											if (distance > allowed_distance) {
												continue;
											}
										}
										refsWithNames.emplace_back(std::make_tuple(ref->GetHandle(), obj->GetName(), GetFormType(obj), obj, CellScanner::Place::kCarried, pickpocketing));
										forms.emplace(obj);
									}
								}

							}
						}
					}
					break;
				}

				default:
					if (V_CellScanner_REFS) {
						auto* obj = ref->GetBaseObject();

						//Is a reference of an excluded type in the MCM.
						if (IsExcludedFormType(obj)) {
							continue;
						}

						// Taking it would be stealing.
						const bool stealing = ref.get()->IsCrimeToActivate();
						if (excludeOwned && stealing) {
							continue;
						}

						if (ItemIsCollectable(obj) && !ItemIsCollected(obj)) {
							//INFO("Found loose item: {}", obj->GetName());

							if (!cell->IsInteriorCell()) {
								auto distance = ref.get()->GetPosition().GetDistance(RE::PlayerCharacter::GetSingleton()->GetPosition());

								if (distance > allowed_distance) {
									continue;
								}
							}

							// Nameless ones can't be picked up
							const char* display = ref.get()->GetDisplayFullName();
							if (!display || !*display) {
								static std::unordered_set<RE::FormID> logged;
								if (logged.insert(obj->GetFormID()).second) {
									INFO("Cell scanner: skipped nameless item {:08X} at {:08X}, it can't be picked up", obj->GetFormID(), ref.get()->GetFormID());
								}
								continue;
							}

							forms.emplace(ref->GetBaseObject());
							refsWithNames.emplace_back(std::make_tuple(ref->GetHandle(), obj->GetName(), GetFormType(obj), obj, CellScanner::Place::kLoose, stealing));
						}
					}
					break;
				}
			}
		}

		// One row per item, the tracked reference first, then not stolen, then closest
		{
			const auto* player = RE::PlayerCharacter::GetSingleton();
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

		auto name = cell->IsInteriorCell() ? cell->GetFullName() : GET_LOC_STRING_BY_KEY("CellScanner_ExteriorCellPrefix");
		if (forms.size() > 0)
		{
			if (b_notify) 
			{
				if (V_CellScanner_NUMB) 
				{
					// The detail (what's where) is in the In Current Cell window now, not a message box.
					//numbers
					auto msg = fmt::format("{:s}{:s}{:s}{:s}"sv, name, CVariables::V_notificationTextCellScan0, std::to_string(forms.size()), CVariables::V_notificationTextCellScan1);
					FrameworkAPI::SendNotification(msg, "NotifyMissable");
					return;
				}

				//simple
				auto msg = fmt::format("{:s}{:s}"sv, name, CVariables::V_notificationTextCellScan2);
				FrameworkAPI::SendNotification(msg, "NotifyMissable");
			}
		}
		else
		{
			//none
			auto msg = fmt::format("{:s}{:s}"sv, name, GET_LOC_STRING_BY_KEY("CellScanner_NoCollectables"));
			FrameworkAPI::SendNotification(msg, "NotifyMissable");
		}
	}

	//---------------------------------------------------
	//-- Get Item Type ----------------------------------
	//---------------------------------------------------

	bool CellScanner::IsExcludedFormType(RE::TESForm* a_form)
	{
		switch (a_form->GetFormType())
		{
		case RE::FormType::Armor: { return CVariables::V_CellScanner_ExcludeArmor; break;}
		case RE::FormType::Book: { return CVariables::V_CellScanner_ExcludeBooks; break;}
		case RE::FormType::Weapon: { return CVariables::V_CellScanner_ExcludeWeapons; break;}
		case RE::FormType::AlchemyItem: { return CVariables::V_CellScanner_ExcludeAlchemy; break;}
		case RE::FormType::Misc: { return CVariables::V_CellScanner_ExcludeMiscItems; break;}
		default: return false; break;
		}
		return false;
	}

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