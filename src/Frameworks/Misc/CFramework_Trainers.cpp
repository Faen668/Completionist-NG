#include "CFramework_Trainers.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/Variables.hpp"

namespace CFramework_Trainers
{
	using namespace CFramework_Master;

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	// Any class that trains a skill, so mod added trainers come along. Lumberjack, Spellsword
	// and a few other classes carry a leftover 100 the game never offers, so a cap of 100
	// counts only for the trainer faction the vanilla training topics check
	void CHandler::InstallFramework() {

		Trainers.clear();
		auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) { return; }

		const auto* trainerFaction = dataHandler->LookupForm<RE::TESFaction>(0x0E3A49, "Skyrim.esm");
		const auto  inTrainerFaction = [&](const RE::TESNPC* a_npc) {
			return trainerFaction && std::ranges::any_of(a_npc->factions, [&](const RE::FACTION_RANK& a_rank) { return a_rank.faction == trainerFaction; });
		};

		for (auto* npc : dataHandler->GetFormArray<RE::TESNPC>()) {
			const auto* npcClass = npc ? npc->npcClass : nullptr;
			if (!npcClass || npcClass->data.maximumTrainingLevel == 0 || !npc->GetFullName() || !*npc->GetFullName()) { continue; }
			if (npcClass->data.maximumTrainingLevel >= 100 && !inTrainerFaction(npc)) { continue; }
			const auto skill = static_cast<std::int32_t>(npcClass->data.teaches.underlying());
			if (skill < 0 || skill > 17) { continue; }
			Trainers.push_back({ npc, {}, skill, npcClass->data.maximumTrainingLevel });
		}
		INFO("Trainers: {} NPCs have a training class", Trainers.size());

		if (auto* ui = RE::UI::GetSingleton()) {
			ui->AddEventSink<RE::MenuOpenCloseEvent>(GetSingleton());
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Find Placed Trainers ) ---
	//---------------------------------------------------

	// Trainers nobody placed in the world (unused or duplicate NPCs) are left off the page
	void CHandler::FindPlacedTrainers() {

		if (Trainers.empty()) { return; }

		std::unordered_map<RE::FormID, Trainer*> byBase;
		for (auto& trainer : Trainers) {
			trainer.ref.reset();
			byBase.emplace(trainer.npc->GetFormID(), std::addressof(trainer));
		}

		std::size_t placed = 0;
		const auto& [forms, lock] = RE::TESForm::GetAllForms();
		RE::BSReadLockGuard guard(lock);
		for (const auto& [formID, form] : *forms) {
			auto* actor = form && form->GetFormType() == RE::FormType::ActorCharacter ? form->As<RE::Actor>() : nullptr;
			if (!actor || actor->IsDeleted()) { continue; }
			const auto* base = actor->GetActorBase();
			if (!base) { continue; }
			if (auto it = byBase.find(base->GetFormID()); it != byBase.end() && !it->second->ref) {
				it->second->ref = actor->CreateRefHandle();
				++placed;
			}
		}
		INFO("Trainers: {} of {} are placed in the world", placed, Trainers.size());
	}

	//---------------------------------------------------
	//-- Framework Functions ( Is Trained ) -------------
	//---------------------------------------------------

	bool CHandler::IsTrained(const Trainer& a_trainer) {

		return a_trainer.npc && FoundItemData_NoShow.HasForm(a_trainer.npc->GetFormID());
	}

	//---------------------------------------------------
	//-- Framework Functions ( Skill Name ) -------------
	//---------------------------------------------------

	// The game's own name, so it's in the player's language
	std::string CHandler::SkillName(std::int32_t a_skill) {

		const auto* info = RE::ActorValueList::GetActorValueInfo(SkillValue(a_skill));
		const auto* name = info ? info->GetFullName() : nullptr;
		return name && *name ? name : std::string(RE::ActorValueToString(SkillValue(a_skill)));
	}

	//---------------------------------------------------
	//-- Framework Functions ( Mark Trained ) -----------
	//---------------------------------------------------

	void CHandler::MarkTrained(RE::TESNPC* a_npc) {

		if (!a_npc || std::ranges::none_of(Trainers, [&](const Trainer& a_trainer) { return a_trainer.npc == a_npc; })) { return; }
		if (FoundItemData_NoShow.HasForm(a_npc->GetFormID())) { return; }

		FoundItemData_NoShow.AddForm(a_npc->GetFormID());
		INFO("Trainers: trained with {} ({:08X})", a_npc->GetName(), a_npc->GetFormID());
		FrameworkAPI::SendNotification(fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, a_npc->GetName()), "NotifySpecial");
		FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kTrained, a_npc->GetName());
	}

	//---------------------------------------------------
	//-- Framework Events ( Training Menu ) -------------
	//---------------------------------------------------

	// A lesson counts once the skill goes up, or on opening if the setting says so
	RE::BSEventNotifyControl CHandler::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) {

		if (!a_event || a_event->menuName != RE::TrainingMenu::MENU_NAME) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->opening) {
			SKSE::GetTaskInterface()->AddTask([this]() {
				auto* ui = RE::UI::GetSingleton();
				auto  menu = ui ? ui->GetMenu<RE::TrainingMenu>() : nullptr;
				auto* player = RE::PlayerCharacter::GetSingleton();
				if (!menu || !player) { return; }

				auto& data = menu->GetRuntimeData();
				auto* trainer = data.trainer;
				auto* npc = trainer ? trainer->GetActorBase() : nullptr;
				_lessonTrainer = npc ? npc->GetFormID() : 0;
				_lessonSkill = data.skill;
				_lessonStart = player->AsActorValueOwner()->GetBaseActorValue(data.skill);
				if (npc && CVariables::V_Trainers_CountOnOffer) {
					MarkTrained(npc);
				}
			});
			return RE::BSEventNotifyControl::kContinue;
		}

		SKSE::GetTaskInterface()->AddTask([this]() {
			auto* player = RE::PlayerCharacter::GetSingleton();
			const auto trainer = std::exchange(_lessonTrainer, 0);
			if (!player || !trainer || _lessonSkill == RE::ActorValue::kNone) { return; }
			if (player->AsActorValueOwner()->GetBaseActorValue(_lessonSkill) > _lessonStart) {
				MarkTrained(RE::TESForm::LookupByID<RE::TESNPC>(trainer));
			}
		});
		return RE::BSEventNotifyControl::kContinue;
	}
}
