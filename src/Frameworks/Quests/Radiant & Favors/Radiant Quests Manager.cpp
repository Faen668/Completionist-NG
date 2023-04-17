#include "Radiant Quests Manager.hpp"
#include "Quests_Bounty.hpp"
#include "Quests_Misc.hpp"
#include "Quests_Beggars.hpp"

namespace Quest_Manager
{
	void RadiantQuestManager::ProcessFormID(std::string questID) {

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

		BeggarQuests::CHandler::Register();
		MiscQuests::CHandler::Register();
		BountyProcessor::CHandler::Sink();
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------
}
