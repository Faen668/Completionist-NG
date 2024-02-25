#include "Frameworks/FrameworkMaster.hpp"
#include "Serialization.hpp"
#include "CellScanner.hpp"
#include "Variables.hpp"

static std::vector<RE::TESObjectREFR*> refs;
static std::vector<std::string> names;
static std::vector<std::string> types;
static std::vector<RE::FormID> formIDs;
static std::vector<std::tuple<RE::TESObjectREFR*, std::string, std::string, RE::TESForm*>> refsWithNames;
static std::unordered_map<RE::FormID, RE::FormID> ExcludedVendorChests{};

static int32_t currentReferencePosition;

namespace CellScanner
{
	using cmd = Serialization::CompletionistData;
	using namespace CFramework_Master;
	using namespace Serialization;
	using namespace CVariables;

	//---------------------------------------------------
	//-- Serialisation Interface ------------------------
	//---------------------------------------------------

	void CellScanner::CHandler::ExcludeReference(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESObjectCELL* a_cell)
	{
		CFramework_Master::ExcludedCellScannerRefs.AddReference(a_ref->GetFormID(), a_ref->GetDisplayFullName(), a_cell->GetFormID());
	};

	void CellScanner::CHandler::RemoveExcludedReference(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		CFramework_Master::ExcludedCellScannerRefs.RemoveReference(a_ref->GetFormID());
	};

	std::vector<std::string> CellScanner::CHandler::GetReferenceFormIDs(RE::StaticFunctionTag*, RE::TESObjectCELL* a_cell)
	{
		return CFramework_Master::ExcludedCellScannerRefs.GetReferenceFormIDs(a_cell->GetFormID());
	}

	std::vector<std::string> CellScanner::CHandler::GetReferenceNames(RE::StaticFunctionTag*, RE::TESObjectCELL* a_cell)
	{
		return CFramework_Master::ExcludedCellScannerRefs.GetReferenceNames(a_cell->GetFormID());
	}

	std::vector<RE::TESObjectREFR*> CellScanner::CHandler::GetObjectReferences(RE::StaticFunctionTag*, RE::TESObjectCELL* a_cell)
	{
		return CFramework_Master::ExcludedCellScannerRefs.GetReferenceForms(a_cell->GetFormID());
	}

	std::string CellScanner::CHandler::GetQuestMarkerReferenceFormID(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) { return "-"; }
		return cmd::GetFormIDHexString(a_ref);
	}

	std::string CellScanner::CHandler::GetQuestMarkerReferenceOwner(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) { return "-"; }
		return cmd::GetFormOwner(a_ref);
	}

	std::string CellScanner::CHandler::GetQuestMarkerReferenceIndex(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) { return "-"; }
		return cmd::GetModIndexFromForm(a_ref);
	}

	//---------------------------------------------------
	//-- Add Excluded Chests ----------------------------
	//---------------------------------------------------

	void CellScanner::CHandler::AddExcludedReferencesFromMods() {

		for (auto& [reference, file, name] : ExcludedModAddedReferences)
		{
			auto* refr = cmd::GetFullForm<RE::TESObjectREFR>(reference, file);
			if (refr && refr->GetParentCell())
			{
				CFramework_Master::ExcludedMerchantContainers.AddReference(refr, name, refr->GetParentCell()->GetFormID());
			};
		};
	}; 

	//---------------------------------------------------
	//-- Vendor Chest Functions -------------------------
	//---------------------------------------------------

	void CellScanner::CHandler::ExcludeAllVendorChests() {

		const auto& [allForms, lock] = RE::TESForm::GetAllForms();
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

	bool CellScanner::CHandler::IsReferenceExcluded(RE::TESObjectREFR* a_chest)
	{
		if (!a_chest) {
			return true;
		}

		return ExcludedVendorChests.contains(a_chest->GetFormID()) 
			|| CFramework_Master::ExcludedMerchantContainers.HasReference(a_chest->GetFormID())
			|| CFramework_Master::ExcludedCellScannerRefs.HasReference(a_chest->GetFormID());
	}
	 
	//---------------------------------------------------
	//-- Collectability Functions -----------------------
	//---------------------------------------------------

	bool CellScanner::CHandler::ItemIsCollectable(RE::TESForm* a_form) {
		return cmd::CheckIsCollectable(a_form->GetFormID());
	}

	//---------------------------------------------------
	//-- Collectability Functions -----------------------
	//---------------------------------------------------

	bool CellScanner::CHandler::ItemIsCollected(RE::TESForm* a_form) {
		return FoundItemData.HasForm(a_form->GetFormID()) || FoundItemData_NoShow.HasForm(a_form->GetFormID());
	}

	//---------------------------------------------------
	//-- Cell Scanner Function --------------------------
	//---------------------------------------------------

	void CellScanner::CHandler::CheckForReferences(RE::StaticFunctionTag*, RE::TESObjectCELL* cell, bool a_logging, bool b_notify)
	{
		ScanCell(cell, a_logging, b_notify);
	}

	//---------------------------------------------------
	//-- Cell Scanner Function --------------------------
	//---------------------------------------------------

	RE::TESObjectREFR* CellScanner::CHandler::GetTargetReferenceRefr(RE::StaticFunctionTag*, RE::TESObjectCELL* a_cell, RE::TESForm* a_lastForm)
	{
		int32_t pos = 0;

		if (!a_cell) {
			return nullptr;
		}

		CHandler::CheckForReferences(nullptr, a_cell, false, true);

		//If the list is empty return nothing.
		if (refsWithNames.size() < 1) {
			currentReferencePosition = -1;
			return nullptr;
		}

		//If the list only has 1 entry or no last reference is provided then return the first reference.
		if (refsWithNames.size() == 1 || !a_lastForm) {
			currentReferencePosition = 0;
			return std::get<0>(refsWithNames[pos]) ? std::get<0>(refsWithNames[pos]) : nullptr;
		}

		//Using closest reference with a list size of 2 or more
		//Traverse the list until we come to the closest reference that does not match the last target.
		if (V_CellScanner_Closest)
		{
			for (pos = 0; pos < refsWithNames.size(); pos++) {
				auto* foundForm = std::get<3>(refsWithNames[pos]);
				auto* reference = std::get<0>(refsWithNames[pos]);

				if (!reference || !foundForm || foundForm->GetFormID() == a_lastForm->GetFormID()) {
					continue;
				}

				currentReferencePosition = pos;
				return reference;
			}
			return nullptr;
		}

		//Using Random Target
		else
		{
			//Get a random integer and traverse until we find a random ref that was not used previously.
			pos = CHandler::GetRandomIndex(refsWithNames.size());
			while (!std::get<0>(refsWithNames[pos]) || !std::get<3>(refsWithNames[pos]) || std::get<3>(refsWithNames[pos])->GetFormID() == a_lastForm->GetFormID()) {
				pos = CHandler::GetRandomIndex(refsWithNames.size());
			};
			currentReferencePosition = pos;
			return std::get<0>(refsWithNames[pos]);
		}

		return nullptr;
	}

	std::string CellScanner::CHandler::GetTargetReferenceName(RE::StaticFunctionTag*)
	{
		return currentReferencePosition != -1 ? std::get<1>(refsWithNames[currentReferencePosition]) : "";
	};

	std::string CellScanner::CHandler::GetTargetReferenceType(RE::StaticFunctionTag*)
	{
		return currentReferencePosition != -1 ? std::get<2>(refsWithNames[currentReferencePosition]) : "";
	};

	RE::TESForm* CellScanner::CHandler::GetTargetReferenceForm(RE::StaticFunctionTag*)
	{
		return currentReferencePosition != -1 ? std::get<3>(refsWithNames[currentReferencePosition]) : nullptr;
	};

	bool CellScanner::CHandler::HasPinnedFormInCell(RE::StaticFunctionTag*, RE::TESObjectCELL* cell, RE::TESForm* a_form) {
		
		if (!a_form || refsWithNames.size() == 0) {
			return false;
		}

		auto it = std::find_if(refsWithNames.begin(), refsWithNames.end(),
			[a_form](const auto& tuple) {
				return std::get<3>(tuple)->GetFormID() == a_form->GetFormID();
			}
		);
		return it != refsWithNames.end();
	}

	bool CellScanner::CHandler::IsItemPinnable(RE::StaticFunctionTag*, RE::TESForm* a_form) {
		return a_form && ItemIsCollectable(a_form) && !ItemIsCollected(a_form);
	};

	std::string CellScanner::CHandler::GetPinnedReferenceName(RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		auto it = std::find_if(refsWithNames.begin(), refsWithNames.end(),
			[a_form](const auto& tuple) {
				return std::get<3>(tuple)->GetFormID() == a_form->GetFormID();
			}
		);

		if (it != refsWithNames.end()) {
			return std::get<1>(*it);
		}
		return "";
	};

	std::string CellScanner::CHandler::GetPinnedReferenceType(RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		auto it = std::find_if(refsWithNames.begin(), refsWithNames.end(),
			[a_form](const auto& tuple) {
				return std::get<3>(tuple)->GetFormID() == a_form->GetFormID();
			}
		);

		if (it != refsWithNames.end()) {
			return std::get<2>(*it);
		}
		return "";
	};

	RE::TESObjectREFR* CellScanner::CHandler::GetPinnedReferenceRefr(RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		auto it = std::find_if(refsWithNames.begin(), refsWithNames.end(),
			[a_form](const auto& tuple) {
				return std::get<3>(tuple)->GetFormID() == a_form->GetFormID();
			}
		);

		if (it != refsWithNames.end()) {
			return std::get<0>(*it);
		}
		return nullptr;
	};

	//---------------------------------------------------
	//-- Cell Scanner Function --------------------------
	//---------------------------------------------------

	bool CellScanner::CHandler::isCellExcluded(RE::StaticFunctionTag*, RE::TESObjectCELL* a_cell) {

		auto* DH = RE::TESDataHandler::GetSingleton();

		for (auto& [formID, modName, Text] : ModAddedCells)
		{
			auto* cell = DH->LookupForm<RE::TESObjectCELL>(formID, modName);

			if (cell && cell->GetFormID() == a_cell->GetFormID()) {
				return true;
			};
		};
		return false;
	}

	//---------------------------------------------------
	//-- Lambda to sort refs based on distance ----------
	//---------------------------------------------------

	static bool CompareByDistance(const RE::TESObjectREFR* a, const RE::TESObjectREFR* b) {
		auto distanceA = a->GetPosition().GetDistance(RE::PlayerCharacter::GetSingleton()->GetPosition());
		auto distanceB = b->GetPosition().GetDistance(RE::PlayerCharacter::GetSingleton()->GetPosition());
		return distanceA < distanceB;
	}

	//---------------------------------------------------
	//-- Cell Scanner Function --------------------------
	//---------------------------------------------------

	void CellScanner::CHandler::ScanCell(RE::TESObjectCELL* cell, bool a_logging, bool b_notify) 
	{
		std::unordered_set<RE::TESForm*> forms;

		if (!cell)
		{
			return;
		};

		const auto& rtd = cell->GetRuntimeData();
		int32_t cont = 0, loos = 0, npcs = 0;

		forms.clear();
		refs.clear();
		names.clear();
		types.clear();
		formIDs.clear();
		refsWithNames.clear();


		for (auto& ref : rtd.references) {
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

						for (auto& [obj, data] : inv) {
							if (data.first > 0 && data.second) {

								//Is a reference of an excluded type in the MCM.
								if (IsExcludedFormType(obj)) {
									continue;
								}

								if (ItemIsCollectable(obj) && !ItemIsCollected(obj) && !forms.contains(obj)) {
									//RE::PlayerCharacter::GetSingleton()->MoveTo(ref.get());
									//INFO("Found container item {} with formID {} ", obj->GetName(), obj->GetLocalFormID());

									if (!cell->IsInteriorCell()) {
										auto distance = ref.get()->GetPosition().GetDistance(RE::PlayerCharacter::GetSingleton()->GetPosition());

										if (distance > allowed_distance) {
											continue;
										}
									}

									forms.emplace(obj);

									refsWithNames.emplace_back(std::make_tuple(ref.get(), obj->GetName(), GetFormType(obj), obj));
									cont++;
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

						auto inv = NPC->GetInventory();
						for (auto& [obj, data] : inv) {
							if (data.first > 0 && data.second) {

								//Is a reference of an excluded type in the MCM.
								if (IsExcludedFormType(obj)) {
									continue;
								}

								if (ItemIsCollectable(obj) && !ItemIsCollected(obj) && !forms.contains(obj)) {
									//INFO("Found NPC Item: {} on - [{}]", RE::TESForm::LookupByID(obj->GetFormID())->GetName(), NPC->GetName());
									
									if (!cell->IsInteriorCell()) {
										auto distance = ref.get()->GetPosition().GetDistance(RE::PlayerCharacter::GetSingleton()->GetPosition());

										if (distance > allowed_distance) {
											continue;
										}
									}
									refsWithNames.emplace_back(std::make_tuple(ref.get(), obj->GetName(), GetFormType(obj), obj));
									forms.emplace(obj);
									npcs++;
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

					if (ItemIsCollectable(obj) && !ItemIsCollected(obj) && !forms.contains(obj)) {
						//INFO("Found loose item: {} with formID {} ", obj->GetName(), obj->GetLocalFormID());

						if (!cell->IsInteriorCell()) {
							auto distance = ref.get()->GetPosition().GetDistance(RE::PlayerCharacter::GetSingleton()->GetPosition());

							if (distance > allowed_distance) {
								continue;
							}
						}

						forms.emplace(ref->GetBaseObject());
						refsWithNames.emplace_back(std::make_tuple(ref.get(), obj->GetName(), GetFormType(obj), obj));
						loos++;
					}
				}
				break;
			}
		}

		// Sort the vector of pairs based on CompareByDistance
		std::sort(refsWithNames.begin(), refsWithNames.end(), [](const auto& a, const auto& b) {
			return CompareByDistance(std::get<0>(a), std::get<0>(b));
			});

		if (forms.size() > 0) 
		{
			if (b_notify) {
				auto name = cell->IsInteriorCell() ? cell->GetFullName() : GET_LOC_STRING_BY_KEY("CellScanner_ExteriorCellPrefix");
				if (V_CellScanner_NUMB) {
					if (V_CellScanner_DETA) 
					{
						auto msg = fmt::format("~~~ {:s} {:s} ~~~"sv, name, CVariables::V_notificationTextCellScan3);
						msg += loos > 0 ? fmt::format("\n~ {:s} {:s} ~"sv, std::to_string(loos), CVariables::V_notificationTextCellScan4) : "", "";
						msg += npcs > 0 ? fmt::format("\n~ {:s} {:s} ~"sv, std::to_string(npcs), CVariables::V_notificationTextCellScan5) : "", "";
						msg += cont > 0 ? fmt::format("\n~ {:s} {:s} ~"sv, std::to_string(cont), CVariables::V_notificationTextCellScan6) : "", "";

						RE::DebugMessageBox(msg.c_str());
						return;
					}
					auto msg = fmt::format("{:s}{:s}{:s}{:s}"sv, name, CVariables::V_notificationTextCellScan0, std::to_string(forms.size()), CVariables::V_notificationTextCellScan1);
					FrameworkAPI::SendNotification(msg, "NotifyMissable");
					return;
				}
				auto msg = fmt::format("{:s}{:s}"sv, name, CVariables::V_notificationTextCellScan2);
				FrameworkAPI::SendNotification(msg, "NotifyMissable");
			}
		}
	}

	//---------------------------------------------------
	//-- Get Item Type ----------------------------------
	//---------------------------------------------------

	bool CellScanner::CHandler::IsExcludedFormType(RE::TESForm* a_form)
	{
		switch (a_form->GetFormType())
		{
		case RE::FormType::Armor:		{ return CVariables::V_CellScanner_ExcludeArmor; break;}
		case RE::FormType::Book:		{ return CVariables::V_CellScanner_ExcludeBooks; break;}
		case RE::FormType::Weapon:		{ return CVariables::V_CellScanner_ExcludeWeapons; break;}
		case RE::FormType::AlchemyItem: { return CVariables::V_CellScanner_ExcludeAlchemy; break;}
		case RE::FormType::Misc:		{ return CVariables::V_CellScanner_ExcludeMiscItems; break;}
		default: return false; break;
		}
		return false;
	}

	//---------------------------------------------------
	//-- Get Item Type ----------------------------------
	//---------------------------------------------------

	std::string CellScanner::CHandler::GetFormType(RE::TESForm* a_form)
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