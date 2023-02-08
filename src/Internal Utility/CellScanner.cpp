#include "Frameworks/FrameworkMaster.hpp"
#include "Serialization.hpp"
#include "CellScanner.hpp"
#include "Variables.hpp"

namespace CellScanner
{
	using namespace CFramework_Master;
	using namespace Serialization;
	using namespace CVariables;

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
	//-- Cell Scanner Function --------------------------
	//---------------------------------------------------

	void CellScanner::CHandler::CheckForReferences(RE::StaticFunctionTag*, RE::TESObjectCELL* cell, bool a_logging) {

		std::unordered_set<RE::TESForm*> refs;

		if (!cell || !cell->IsInteriorCell()) { return; }

		const auto& rtd = cell->GetRuntimeData();
		int32_t cont = 0, loos = 0, npcs = 0;
		refs.clear();

		for (auto& ref : rtd.references) {
			if (!ref || !ref.get() || !ref->GetBaseObject() || ref.get() == RE::PlayerCharacter::GetSingleton() || ref->IsDeleted() || ref->IsDisabled()) {
				continue;
			}

			switch (ref.get()->GetBaseObject()->GetFormType()) {
			case RE::FormType::Container:
			{
				if (V_CellScanner_CONT) {
					if (auto* container = ref.get()->GetBaseObject()->As<RE::TESContainer>()) {
						container->ForEachContainerObject([&](RE::ContainerObject& entry) {
							auto* obj = entry.obj->As<RE::TESForm>();
							if (ItemIsCollectable(obj) && !ItemIsCollected(obj) && !refs.contains(obj)) {
								if (a_logging) {
									INFO("Found {} In Container - [{}]", RE::TESForm::LookupByID(obj->GetFormID())->GetName(), ref->GetFormID());
								}
								refs.emplace(obj);
								cont++;
							}
							return true;
							});
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
									if (a_logging) {
										INFO("Found {} on - [{}]", RE::TESForm::LookupByID(obj->GetFormID())->GetName(), NPC->GetName());
									}
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
						if (a_logging) {
							INFO("Found {}", RE::TESForm::LookupByID(obj->GetFormID())->GetName());
						}
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