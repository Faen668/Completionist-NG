#include "Frameworks/FrameworkMaster.hpp"
#include "Serialization.hpp"
#include "CellScanner.hpp"
#include "Variables.hpp"

constexpr std::array<std::pair<RE::FormID, const char*>, 1> ExcludedChests = {
	{{0x0009C0, "cceejsse003-hollow.esl"}, // Shadowfoot Sanctum Test Urn
} };

namespace CellScanner
{
	using namespace CFramework_Master;
	using namespace Serialization;
	using namespace CVariables;

	//---------------------------------------------------
	//-- Add Excluded CHests ----------------------------
	//---------------------------------------------------

	void CellScanner::CHandler::AddExcludedChests() {

		for (auto& [chest, file] : ExcludedChests) {
			auto* cont = CompletionistData::GetFullForm<RE::TESObjectREFR>(chest, file);
			if (cont) {
				INFO("Got Container");
				AddMerchantChest(cont);
			}
		}
	}

	//---------------------------------------------------
	//-- Build Merchant Chest List ----------------------
	//---------------------------------------------------

	void CellScanner::CHandler::BuildMerchantChestData(RE::TESObjectCELL* cell, const RE::TESObjectCELL::RUNTIME_DATA rtd)
	{	
		for (auto& ref : rtd.references) {
			if (!ref || !ref.get() || !ref->GetBaseObject() || ref.get()->GetBaseObject()->GetFormType() != RE::FormType::NPC || ref.get() == RE::PlayerCharacter::GetSingleton() || ref->IsDeleted() || ref->IsDisabled()) {
				continue;
			}

			auto* NPC = ref->As<RE::Actor>();
			if (NPC && NPC->GetVendorFaction() && (NPC->GetVendorFaction()->IsVendor() || NPC->GetVendorFaction()->OffersServices())) {
				if (auto* chest = NPC->GetVendorFaction()->vendorData.merchantContainer; chest != nullptr) {
					AddMerchantChest(chest);
				};
			}
		}
	}

	//---------------------------------------------------
	//-- Collectability Functions -----------------------
	//---------------------------------------------------

	bool CellScanner::CHandler::ItemIsCollectable(RE::TESForm* a_form) {
		return CompletionistData::CheckIsCollectable(a_form->GetFormID());
	}

	bool CellScanner::CHandler::ItemIsCollected(RE::TESForm* a_form) {
		return FoundItemData.HasForm(a_form->GetFormID()) || FoundItemData_NoShow.HasForm(a_form->GetFormID());
	}

	//---------------------------------------------------
	//-- Vendor Chest Functions -------------------------
	//---------------------------------------------------

	void CellScanner::CHandler::AddMerchantChest(RE::TESObjectREFR* a_chest) {

		if (std::find(MerchantChests.begin(), MerchantChests.end(), a_chest) == MerchantChests.end()) {
			MerchantChests.push_back(a_chest);
		}
	}

	bool CellScanner::CHandler::IsMerchantChest(RE::TESObjectREFR* a_chest) {
		return std::find(MerchantChests.begin(), MerchantChests.end(), a_chest) != MerchantChests.end();
	}

	//---------------------------------------------------
	//-- Cell Scanner Function --------------------------
	//---------------------------------------------------

	void CellScanner::CHandler::CheckForReferences(RE::StaticFunctionTag*, RE::TESObjectCELL* cell, bool a_logging) {

		std::unordered_set<RE::TESForm*> refs;

		if (!cell || !cell->IsInteriorCell()) { return; }

		const auto& rtd = cell->GetRuntimeData();
		AddExcludedChests();
		BuildMerchantChestData(cell, rtd);

		int32_t cont = 0, loos = 0, npcs = 0;
		refs.clear();

		for (auto& ref : rtd.references) {
			if (!ref || !ref.get() || !ref->GetBaseObject() || ref.get() == RE::PlayerCharacter::GetSingleton() || ref->IsDeleted() || ref->IsDisabled()) {
				continue;
			}

			switch (ref.get()->GetBaseObject()->GetFormType()) {
			case RE::FormType::Container:
			{
				if (V_CellScanner_CONT && !IsMerchantChest(ref.get())) {
					if (auto* container = ref.get()->GetBaseObject()->As<RE::TESContainer>()) {
						auto inv = ref.get()->GetInventory();

						for (auto& [obj, data] : inv) {
							if (data.first > 0 && data.second) {
								if (ItemIsCollectable(obj) && !ItemIsCollected(obj) && !refs.contains(obj)) {
									//RE::PlayerCharacter::GetSingleton()->MoveTo(ref.get());
									INFO("Found container item {} with formID {} ", obj->GetName(), obj->GetLocalFormID());
									refs.emplace(obj);
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

						auto* FollowerFaction = CompletionistData::GetFullForm<RE::TESFaction>(0x05C84E, "Skyrim.esm");

						if (FollowerFaction && NPC->IsInFaction(FollowerFaction)) {
							INFO("{} is in the follower faction", NPC->GetName());
							break;
						}

						auto inv = NPC->GetInventory();
						for (auto& [obj, data] : inv) {
							if (data.first > 0 && data.second) {
								if (ItemIsCollectable(obj) && !ItemIsCollected(obj) && !refs.contains(obj)) {
									INFO("Found NPC Item: {} on - [{}]", RE::TESForm::LookupByID(obj->GetFormID())->GetName(), NPC->GetName());
									refs.emplace(obj);
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
					if (ItemIsCollectable(obj) && !ItemIsCollected(obj) && !refs.contains(obj)) {
						INFO("Found loose item: {} with formID {} ", obj->GetName(), obj->GetLocalFormID());
						refs.emplace(ref->GetBaseObject());
						loos++;
					}
				}
				break;
			}
		}
		if (refs.size() > 0) {
			if (V_CellScanner_NUMB) {
				if (V_CellScanner_DETA) {

					auto msg = fmt::format("~~~ {:s} {:s} ~~~"sv, cell->GetFullName(), CVariables::V_notificationTextCellScan3);
					msg += loos > 0 ? fmt::format("\n~ {:s} {:s} ~"sv, std::to_string(loos), CVariables::V_notificationTextCellScan4) : "", "";
					msg += npcs > 0 ? fmt::format("\n~ {:s} {:s} ~"sv, std::to_string(npcs), CVariables::V_notificationTextCellScan5) : "", "";
					msg += cont > 0 ? fmt::format("\n~ {:s} {:s} ~"sv, std::to_string(cont), CVariables::V_notificationTextCellScan6) : "", "";

					RE::DebugMessageBox(msg.c_str());
					return;
				}
				auto msg = fmt::format("{:s}{:s}{:s}{:s}"sv, cell->GetFullName(), CVariables::V_notificationTextCellScan0, std::to_string(refs.size()), CVariables::V_notificationTextCellScan1);
				FrameworkAPI::SendNotification(msg, "NotifyMissable");
				return;
			}
			auto msg = fmt::format("{:s}{:s}"sv, cell->GetFullName(), CVariables::V_notificationTextCellScan2);
			FrameworkAPI::SendNotification(msg, "NotifyMissable");
		}
	}
};