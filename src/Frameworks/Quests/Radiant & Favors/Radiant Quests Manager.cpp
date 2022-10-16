#include "Radiant Quests Manager.hpp"
#include "Quests_Bounty.hpp"
#include "Quests_College.hpp"
#include "Quests_Companions.hpp"
#include "Quests_Dawnguard.hpp"
#include "Quests_Favors.hpp"
#include "Quests_Misc.hpp"
#include "Quests_SupportedMods.hpp"
#include "Quests_ThievesGuild.hpp"
#include "Quests_Beggars.hpp"

namespace Quest_Manager
{
	void RadiantQuestManager::ProcessFormID(std::uint32_t _ID, std::string questID) {

		if (questID == "CWSiegeObj" || questID == "CWFortSiegeFort") {
			auto papyrusVM = RE::BSScript::Internal::VirtualMachine::GetSingleton();
			RE::VMHandle questHandle;

			if (GetQuestHandle(papyrusVM, std::string("Completionist_Main"), questHandle)) {

				RE::BSFixedString AliasName = "Hold";
				if (questID == "CWSiegeObj") { AliasName = "City"; }

				RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> funcReturn;
				auto args = RE::MakeFunctionArguments(RE::BSFixedString(questID), RE::BSFixedString(AliasName));
				papyrusVM->DispatchMethodCall2(questHandle, "Completionist_QuestsScript", "ProcessQuest_CW", args, funcReturn);
				return;
			}
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (questID == "FavorJobsDrunks") {

			if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x0013390, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorDrunksCosnach")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x0013392, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorDrunksDegaine")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x003550B, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorDrunksEmbry")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x0013619, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorDrunksKarl")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001C186, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorDrunksRanmir")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x00368C8, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorDrunksRazelan")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001413F, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorDrunksTorbjorn")) {
					global->value += 1;
					return;
				}
			}
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (questID == "CR13") {

			if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001A692, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Companions_CR13Farkas")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001A694, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Companions_CR13Vilkas")) {
					global->value += 1;
					return;
				}
			}
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (questID == "Favor001") {

			if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001413C, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor001Adonato")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001360B, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor001Aeri")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x009A7A8, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor001Banning")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x00135EC, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor001Idgrod")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001366B, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor001Sondas")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x004E5E9, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor001Thadgeir")) {
					global->value += 1;
					return;
				}
			}
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (questID == "Favor013") {

			if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x0013B99, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor013CarlottaValentia")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001C184, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor013Haran")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x0013662, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor013Iddra")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001329D, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor013OctieveSan")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x00133AE, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor013Omluag")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001412E, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor013ScoutsManyMarshes")) {
					global->value += 1;
					return;
				}
			}
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (questID == "Favor017") {

			if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x00135E8, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor017Benor")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x0013B79, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor017Burguk")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x0019951, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor017Larak")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001B075, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor017Mauhulakh")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x0013390, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor017Cosnach")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x003EFE9, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor017Rolff")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x00918E2, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor017Uthgerd")) {
					global->value += 1;
					return;
				}
			}
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (questID == "Favor018") {

			if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001365A, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor018Dengeir")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001C182, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor018Malur")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x0014130, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor018StandInShallows")) {
					global->value += 1;
					return;
				}
			}
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (questID == "Favor019") {

			if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001413A, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor019Revyn")) {
					global->value += 1;
					return;
				}
			}
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (questID == "Favor104") {

			if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001325F, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor104Ahtar")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x0013666, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor104Annekke")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x0014149, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor104Brunwulf")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x4018FC5, "Dragonborn.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor104Fanari")) {
					global->value += 1;
					return;
				}
			}
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (questID == "Favor109") {

			if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x00132AA, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor109Sybille")) {
					global->value += 1;
					return;
				}
			}
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (questID == "Favor110") {

			if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x0041FB8, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor110Aldis")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x00135EF, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor110Lami")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001361E, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor110Rustleif")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x0013653, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor110Siddgeir")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001413F, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor110Torbjorn")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x0013BAB, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor110Ysolda")) {
					global->value += 1;
					return;
				}
			}
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (questID == "Favor151") {

			if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001339F, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor151Igmund")) {
					global->value += 1;
					return;
				}
			}
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (questID == "Favor153") {

			if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x0013620, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor153Skald")) {
					global->value += 1;
					return;
				}
			}
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (questID == "Favor154") {

			if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x0013653, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor154Siddgeir")) {
					global->value += 1;
					return;
				}
			}
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (questID == "Favor157") {

			if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001339F, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor157Igmund")) {
					global->value += 1;
					return;
				}
			}
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (questID == "Favor158") {

			if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001C188, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor158Korir")) {
					global->value += 1;
					return;
				}
			}
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (questID == "Favor204") {

			if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x0013BAA, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor204Amren")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001411A, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor204Shahvee")) {
					global->value += 1;
					return;
				}
			}
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (questID == "Favor205") {

			if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x0013614, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor205Frida")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001329C, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor205Noster")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x0014142, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor205Oengul")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001403F, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor205Roggi")) {
					global->value += 1;
					return;
				}
			}
			else if (_ID == RE::TESDataHandler::GetSingleton()->LookupFormID(0x001364D, "Skyrim.esm")) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor205Runil")) {
					global->value += 1;
					return;
				}
			}
			return;
		}
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	bool RadiantQuestManager::GetQuestHandle(RE::BSScript::IVirtualMachine* a_vm, std::string ScriptName, RE::VMHandle& handleOut) {

		auto handlePolicy = a_vm->GetObjectHandlePolicy();
		if (!handlePolicy) { return false; }

		auto quest = RE::TESForm::LookupByEditorID(ScriptName);
		if (!quest) { return false; }

		handleOut = handlePolicy->GetHandleForObject(static_cast<RE::VMTypeID>(quest->GetFormType()), quest);
		if (!handleOut) { return false; }

		return true;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void Install() {

		BeggarQuests::ScriptEventHandler::Register();
		BountyQuests::ScriptEventHandler::Register();
		CollegeQuests::ScriptEventHandler::Register();
		CompanionsQuests::ScriptEventHandler::Register();
		DawnguardQuests::ScriptEventHandler::Register();
		ThievesGuildQuests::ScriptEventHandler::Register();
		MiscQuests::ScriptEventHandler::Register();

		Favor001::ScriptEventHandler::Register();
		Favor013::ScriptEventHandler::Register();
		Favor017::ScriptEventHandler::Register();
		Favor018::ScriptEventHandler::Register();
		Favor019::ScriptEventHandler::Register();
		Favor104::ScriptEventHandler::Register();
		Favor109::ScriptEventHandler::Register();
		Favor110::ScriptEventHandler::Register();
		Favor151::ScriptEventHandler::Register();
		Favor153::ScriptEventHandler::Register();
		Favor154::ScriptEventHandler::Register();
		Favor157::ScriptEventHandler::Register();
		Favor158::ScriptEventHandler::Register();
		Favor204::ScriptEventHandler::Register();
		Favor205::ScriptEventHandler::Register();

		/*Supported Mods*/ LOTD::ScriptEventHandler::Register();
		/*Supported Mods*/ Vigilant::ScriptEventHandler::Register();
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------
}
