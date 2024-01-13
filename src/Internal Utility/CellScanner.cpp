#include "Frameworks/FrameworkMaster.hpp"
#include "Serialization.hpp"
#include "CellScanner.hpp"
#include "Variables.hpp"

static std::vector<RE::TESObjectREFR*> refs{};
static std::vector<std::string> names{};
static std::vector<std::string> types{};
static RE::BGSListForm* ExcludedChestsFormlist{};

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

	std::vector<std::string> CellScanner::CHandler::GetValidItemReferenceNames(RE::StaticFunctionTag*)
	{
		return names;
	};

	std::vector<std::string> CellScanner::CHandler::GetValidItemReferenceTypes(RE::StaticFunctionTag*)
	{
		return types;
	};

	std::vector<RE::TESObjectREFR*> CellScanner::CHandler::GetValidItemReferences(RE::StaticFunctionTag*, RE::TESObjectCELL* cell)
	{
		if (!cell)
		{
			return std::vector<RE::TESObjectREFR*>{};
		}

		CheckForReferences(nullptr, cell, false, true);
		return refs;
	};

	//---------------------------------------------------
	//-- Add Excluded Chests ----------------------------
	//---------------------------------------------------

	void CellScanner::CHandler::AddExcludedReferencesFromMods() {

		for (auto& [reference, file, name] : ExcludedModAddedReferences)
		{
			auto* refr = cmd::GetFullForm<RE::TESObjectREFR>(reference, file);
			if (refr)
			{
				CFramework_Master::ExcludedMerchantContainers.AddReference(refr, name, refr->GetParentCell()->GetFormID());
			};
		};
	}; 

	//---------------------------------------------------
	//-- Vendor Chest Functions -------------------------
	//---------------------------------------------------

	void CellScanner::CHandler::ExcludeMerchantChests(RE::TESObjectCELL* cell, const RE::TESObjectCELL::RUNTIME_DATA rtd)
	{	
		for (auto& ref : rtd.references) 
		{
			auto* NPC = ref->As<RE::Actor>();
			if (NPC && NPC->GetVendorFaction() && (NPC->GetVendorFaction()->IsVendor() || NPC->GetVendorFaction()->OffersServices())) 
			{
				if (auto* chest = NPC->GetVendorFaction()->vendorData.merchantContainer; chest != nullptr) {
					CFramework_Master::ExcludedMerchantContainers.AddReference(chest, ref->GetDisplayFullName(), cell->GetFormID());
				};
			}
		}
	}

	//---------------------------------------------------
	//-- Vendor Chest Functions -------------------------
	//---------------------------------------------------

	bool CellScanner::CHandler::IsReferenceExcluded(RE::TESObjectREFR* a_chest)
	{
		return CFramework_Master::ExcludedMerchantContainers.HasReference(a_chest->GetFormID())
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
		ScanCell(cell, false, false);
		ScanCell(cell, a_logging, b_notify);
	}

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

		AddExcludedReferencesFromMods();
		ExcludeMerchantChests(cell, rtd);

		int32_t cont = 0, loos = 0, npcs = 0;
		forms.clear();

		refs.clear();
		names.clear();
		types.clear();

		for (auto& ref : rtd.references) {
			if (!ref || !ref.get() || !ref->GetBaseObject() || ref.get() == RE::PlayerCharacter::GetSingleton() || ref->IsDeleted() || ref->IsDisabled()) {
				continue;
			}

			//Is an excluded reference.
			if (IsReferenceExcluded(ref.get())) {
				continue;
			}

			//Is a reference of an excluded container.
			ExcludedChestsFormlist = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSListForm>(0x00081D, "Completionist.esp");
			if (ExcludedChestsFormlist->HasForm(ref.get()->GetBaseObject())) {
				CFramework_Master::ExcludedMerchantContainers.AddReference(ref.get(), "Formlist Merchant Chest", cell->GetFormID());
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
									refs.push_back(ref.get());
									names.push_back(obj->GetName());
									types.push_back(GetFormType(obj));
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

									forms.emplace(obj);
									refs.push_back(ref.get());
									names.push_back(obj->GetName());
									types.push_back(GetFormType(obj));
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
						refs.push_back(ref.get());
						names.push_back(obj->GetName());
						types.push_back(GetFormType(obj));
						loos++;
					}
				}
				break;
			}
		}
		if (forms.size() > 0) {
			if (b_notify) {
				auto name = cell->IsInteriorCell() ? cell->GetFullName() : CLocalisation::LocalisationAPI::GetLocStringByKey("CellScanner_ExteriorCellPrefix");
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