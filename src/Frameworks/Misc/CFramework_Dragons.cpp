#include "CFramework_Dragons.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CFramework_Dragons
{
	using namespace CFramework_Master;

	namespace
	{
		// The named dragons of the game and its DLC, from their records, in story order. Kruziikrel
		// and Relonikiv are two placed leveled dragons that At the Summit of Apocrypha's aliases
		// name with a message each, so their message and their reference
		struct Known
		{
			const char* key;
			const char* plugin;
			RE::FormID  id;
		};

		constexpr Known known[] = {
			{ "Alduin", "Skyrim.esm", 0x032B94 },
			{ "Alduin", "Skyrim.esm", 0x032D9D },
			{ "Alduin", "Skyrim.esm", 0x04377F },
			{ "Alduin", "Skyrim.esm", 0x04424A },
			{ "Alduin", "Skyrim.esm", 0x04E9BC },
			{ "Alduin", "Skyrim.esm", 0x08E4F1 },
			{ "Mirmulnir", "Skyrim.esm", 0x01CA05 },
			{ "Sahloknir", "Skyrim.esm", 0x032D9B },
			{ "Vuljotnaak", "Skyrim.esm", 0x0FE430 },
			{ "Nahagliiv", "Skyrim.esm", 0x0FE431 },
			{ "Viinturuth", "Skyrim.esm", 0x0FE432 },
			{ "Paarthurnax", "Skyrim.esm", 0x03C57C },
			{ "Odahviing", "Skyrim.esm", 0x045920 },
			{ "Odahviing", "Skyrim.esm", 0x0F6850 },
			{ "Vulthuryol", "Skyrim.esm", 0x07EAC7 },
			{ "SkeletalDragon", "Skyrim.esm", 0x09192C },
			{ "Durnehviir", "Dawnguard.esm", 0x0030D8 },
			{ "Durnehviir", "Dawnguard.esm", 0x00C71A },
			{ "Voslaarum", "Dawnguard.esm", 0x010E99 },
			{ "Naaslaarum", "Dawnguard.esm", 0x010E9C },
			{ "Krosulhah", "Dragonborn.esm", 0x017F87 },
			{ "Sahrotaar", "Dragonborn.esm", 0x023F7B },
			{ "Sahrotaar", "Dragonborn.esm", 0x031CA5 },
			{ "Sahrotaar", "Dragonborn.esm", 0x039B6B },
			{ "Kruziikrel", "Dragonborn.esm", 0x02A75A },
			{ "Kruziikrel", "Dragonborn.esm", 0x01D573 },
			{ "Relonikiv", "Dragonborn.esm", 0x02A75B },
			{ "Relonikiv", "Dragonborn.esm", 0x01D572 },
		};

		//---------------------------------------------------
		//-- Framework Functions ( Race Of ) ----------------
		//---------------------------------------------------

		// Through the templates, as the game does: most story dragons take theirs from a list
		RE::TESRace* RaceOf(RE::TESForm* a_form, int a_depth = 0) {

			if (!a_form || a_depth > 8) { return nullptr; }
			if (auto* npc = a_form->As<RE::TESNPC>()) {
				if (npc->baseTemplateForm && npc->actorData.templateUseFlags.any(RE::ACTOR_BASE_DATA::TEMPLATE_USE_FLAG::kTraits)) {
					return RaceOf(npc->baseTemplateForm, a_depth + 1);
				}
				return npc->race;
			}
			if (auto* list = a_form->As<RE::TESLevCharacter>()) {
				for (auto& entry : list->entries) {
					if (auto* race = RaceOf(entry.form, a_depth + 1)) { return race; }
				}
			}
			return nullptr;
		}

		bool IsDragonRace(const RE::TESRace* a_race) { return a_race && a_race->HasKeywordString("ActorTypeDragon"); }

		//---------------------------------------------------
		//-- Framework Functions ( Name Of ) ----------------
		//---------------------------------------------------

		std::string NameOf(RE::TESNPC* a_npc, int a_depth = 0) {

			if (!a_npc || a_depth > 8) { return {}; }
			if (const auto* name = a_npc->GetFullName(); name && *name) { return name; }
			return NameOf(a_npc->baseTemplateForm ? a_npc->baseTemplateForm->As<RE::TESNPC>() : nullptr, a_depth + 1);
		}

		// Kinds of dragon: names a leveled list hands out
		void AddListNames(RE::TESForm* a_form, std::unordered_set<std::string>& a_names, int a_depth = 0) {

			if (!a_form || a_depth > 8) { return; }
			if (auto* npc = a_form->As<RE::TESNPC>()) {
				if (IsDragonRace(RaceOf(npc))) {
					if (auto name = NameOf(npc); !name.empty()) { a_names.insert(std::move(name)); }
				}
			} else if (auto* list = a_form->As<RE::TESLevCharacter>()) {
				for (auto& entry : list->entries) { AddListNames(entry.form, a_names, a_depth + 1); }
			}
		}

		bool QuestDone(const char* a_plugin, RE::FormID a_id) {

			const auto* quest = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESQuest>(a_id, a_plugin);
			return quest && quest->IsCompleted();
		}

		// Reached a stage of the story; running alone isn't enough, some run from the start of the game
		bool QuestReached(const char* a_plugin, RE::FormID a_id, std::uint16_t a_stage) {

			const auto* quest = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESQuest>(a_id, a_plugin);
			return quest && (quest->IsCompleted() || quest->GetCurrentStageID() >= a_stage);
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	// The list above, then any dragon with a name of its own: one no leveled list gives out,
	// that isn't a kind of dragon (Skeletal Dragon), so mod-added dragons come along
	void CHandler::InstallFramework() {

		Dragons.clear();
		auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) { return; }

		const auto dragonFor = [&](const std::string& a_key, const std::string& a_name) -> Dragon& {
			const auto it = std::ranges::find_if(Dragons, [&](const Dragon& a_dragon) { return a_key.empty() ? a_dragon.key.empty() && a_dragon.name == a_name : a_dragon.key == a_key; });
			return it != Dragons.end() ? *it : Dragons.emplace_back(Dragon{ a_key, a_name });
		};

		std::unordered_set<RE::TESNPC*> listed;
		for (const auto& [key, plugin, id] : known) {
			auto* form = dataHandler->LookupForm(id, plugin);
			if (!form) { continue; }
			if (auto* message = form->As<RE::BGSMessage>()) {
				const auto* name = message->GetFullName();
				auto&       dragon = dragonFor(key, name && *name ? name : key);
				dragon.plugin = plugin;
				dragon.formID = dragon.formID ? dragon.formID : form->GetFormID();
				dragon.known = true;
			} else if (auto* npc = form->As<RE::TESNPC>()) {
				auto& dragon = dragonFor(key, NameOf(npc));
				dragon.plugin = plugin;
				dragon.formID = dragon.formID ? dragon.formID : npc->GetFormID();
				dragon.npcs.push_back(npc);
				dragon.known = true;
				listed.insert(npc);
			}
		}

		std::unordered_set<std::string> kinds;
		for (auto* list : dataHandler->GetFormArray<RE::TESLevCharacter>()) {
			AddListNames(list, kinds);
		}
		// The plain word, "Dragon" in English, is in every kind's name
		std::string plain;
		for (const auto& kind : kinds) {
			if (plain.empty() || kind.size() < plain.size()) { plain = kind; }
		}

		std::size_t found = 0;
		for (auto* npc : dataHandler->GetFormArray<RE::TESNPC>()) {
			if (!npc || listed.contains(npc) || !IsDragonRace(RaceOf(npc))) { continue; }
			const auto name = NameOf(npc);
			if (name.empty() || kinds.contains(name) || (!plain.empty() && name.find(plain) != std::string::npos)) { continue; }
			// The known dragons' spare records, under a name the list already has
			const auto it = std::ranges::find(Dragons, name, &Dragon::name);
			auto&      dragon = it != Dragons.end() ? *it : dragonFor({}, name);
			if (dragon.plugin.empty()) {
				const auto* file = npc->GetFile(0);
				dragon.plugin = file ? std::string(file->GetFilename()) : std::string{};
				dragon.formID = npc->GetFormID();
				++found;
			}
			dragon.npcs.push_back(npc);
		}
		INFO("Dragons: {} named dragons, {} of them found outside the known list", Dragons.size(), found);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Find Placed Dragons ) ----
	//---------------------------------------------------

	// Their references tell who's dead; found names nobody placed (summons, test records) are dropped
	void CHandler::FindPlacedDragons() {

		std::unordered_map<RE::FormID, Dragon*> byBase;
		for (auto& dragon : Dragons) {
			dragon.refs.clear();
			for (auto* npc : dragon.npcs) {
				byBase.emplace(npc->GetFormID(), std::addressof(dragon));
			}
		}

		{
			const auto& [forms, lock] = RE::TESForm::GetAllForms();
			RE::BSReadLockGuard guard(lock);
			for (const auto& [formID, form] : *forms) {
				auto* actor = form && form->GetFormType() == RE::FormType::ActorCharacter ? form->As<RE::Actor>() : nullptr;
				if (!actor || actor->IsDeleted()) { continue; }
				const RE::TESForm* base = actor->GetActorBase();
				if (const auto* leveled = actor->extraList.GetByType<RE::ExtraLeveledCreature>(); leveled && leveled->originalBase) {
					base = leveled->originalBase;
				}
				if (const auto it = base ? byBase.find(base->GetFormID()) : byBase.end(); it != byBase.end()) {
					it->second->refs.push_back(actor->CreateRefHandle());
				}
			}
		}

		// The ones only an alias names, by their own reference
		auto* dataHandler = RE::TESDataHandler::GetSingleton();
		for (const auto& [key, plugin, id] : known) {
			auto*      form = dataHandler ? dataHandler->LookupForm(id, plugin) : nullptr;
			auto*      actor = form ? form->As<RE::Actor>() : nullptr;
			const auto it = std::ranges::find(Dragons, std::string_view(key), &Dragon::key);
			if (actor && it != Dragons.end()) {
				it->refs.push_back(actor->CreateRefHandle());
			}
		}

		std::erase_if(Dragons, [](const Dragon& a_dragon) { return !a_dragon.known && a_dragon.refs.empty(); });
		INFO("Dragons: {} named dragons tracked", Dragons.size());
	}

	//---------------------------------------------------
	//-- Framework Functions ( Fate Of ) ----------------
	//---------------------------------------------------

	// From their bodies, the dragon kills and the quests that settle them
	Fate CHandler::FateOf(const Dragon& a_dragon) {

		const auto& key = a_dragon.key;
		const bool  dead = std::ranges::any_of(a_dragon.refs, [](const RE::ObjectRefHandle& a_ref) {
			const auto  ref = a_ref.get();
			const auto* actor = ref ? ref->As<RE::Actor>() : nullptr;
			return actor && actor->IsDead();
		}) || [&] {
			const auto it = PlayerDragonKills.data.find(a_dragon.name);
			return it != PlayerDragonKills.data.end() && it->second.killCount + it->second.assistCount > 0;
		}();

		if (key == "Alduin") {
			// Dragonslayer; he's first seen attacking Helgen in Unbound, stage 150
			return QuestDone("Skyrim.esm", 0x046EF2) ? Fate::kSlain : QuestReached("Skyrim.esm", 0x03372B, 150) ? Fate::kMet : Fate::kNotMet;
		}
		if (key == "Paarthurnax") {
			// The Throat of the World
			return dead ? Fate::kSlain : QuestDone("Skyrim.esm", 0x02D515) ? Fate::kMet : Fate::kNotMet;
		}
		if (key == "Odahviing") {
			// The Fallen: trapped in Dragonsreach at stage 200, agrees to fly you to Skuldafn at 220
			return dead ? Fate::kSlain : QuestReached("Skyrim.esm", 0x044C56, 220) ? Fate::kTamed : QuestReached("Skyrim.esm", 0x044C56, 200) ? Fate::kMet : Fate::kNotMet;
		}
		if (key == "Durnehviir") {
			// Summon Durnehviir, the pact he makes once he's beaten; he attacks in Beyond Death at stage 80
			auto*      player = RE::PlayerCharacter::GetSingleton();
			auto*      shout = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESShout>(0x0030D2, "Dawnguard.esm");
			const bool ally = (player && shout && player->HasShout(shout)) || QuestReached("Dawnguard.esm", 0x00C73A, 30);
			return ally ? Fate::kAlly : dead ? Fate::kSlain : QuestReached("Dawnguard.esm", 0x002850, 80) ? Fate::kMet : Fate::kNotMet;
		}
		if (key == "Sahrotaar") {
			// Tamed with Bend Will at stage 400, then Miraak takes his soul
			return dead || QuestDone("Dragonborn.esm", 0x0179D7) ? Fate::kSlain : QuestReached("Dragonborn.esm", 0x0179D7, 400) ? Fate::kTamed : Fate::kNotMet;
		}
		if (key == "Kruziikrel" || key == "Relonikiv") {
			// Miraak calls them at the summit and takes their souls
			return dead || QuestDone("Dragonborn.esm", 0x0179D7) ? Fate::kSlain : Fate::kNotMet;
		}
		return dead ? Fate::kSlain : Fate::kNotMet;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Is Done ) ----------------
	//---------------------------------------------------

	// Their story's over: Paarthurnax can be spared, the rest fall, are tamed or join you
	bool CHandler::IsDone(const Dragon& a_dragon, Fate a_fate) {

		if (FoundItemData_NoShow.HasForm(a_dragon.formID)) { return true; }
		return a_dragon.key == "Paarthurnax" ? a_fate != Fate::kNotMet : a_fate >= Fate::kTamed;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Is Dragon ) --------------
	//---------------------------------------------------

	bool CHandler::IsDragon(const RE::Actor* a_actor) {

		return a_actor && IsDragonRace(a_actor->GetRace());
	}

	//---------------------------------------------------
	//-- Framework Functions ( Fate Key ) ---------------
	//---------------------------------------------------

	const char* CHandler::FateKey(Fate a_fate) {

		switch (a_fate) {
		case Fate::kMet:   return "UI_Dragons_Met";
		case Fate::kTamed: return "UI_Dragons_Tamed";
		case Fate::kAlly:  return "UI_Dragons_Ally";
		case Fate::kSlain: return "UI_Dragons_Slain";
		default:           return "UI_Dragons_NotMet";
		}
	}
}
