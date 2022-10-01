#pragma once

#undef GetForm
#undef AddForm

namespace Serialization
{
	enum : std::uint32_t
	{
		kHeader = 'COMP',
		kVersion = 1000,
	};

#define DEFAULT_VARIATION_MAX 12
	using FormArray = RE::FormID[];
	using Variation = std::pair<RE::FormID, std::array<RE::FormID, DEFAULT_VARIATION_MAX>>;

	struct CompletionistData
	{
		//---------------------------------------------------
		//-- Utility Functions ( Is Mod Installed ) ---------
		//---------------------------------------------------

		[[nodiscard]] static bool IsModInstalled(std::string_view a_modname) noexcept
		{
			auto ESP = RE::TESDataHandler::GetSingleton()->LookupLoadedModByName(a_modname) != nullptr;
			auto ESL = RE::TESDataHandler::GetSingleton()->LookupLoadedLightModByName(a_modname) != nullptr;
			return ESP || ESL;
		}

		//---------------------------------------------------
		//-- Utility Functions ( Is Map Marker ) ------------
		//---------------------------------------------------

		[[nodiscard]] static RE::ExtraMapMarker* GetMapMarkerInternal(RE::TESObjectREFR* a_marker)
		{
			if (!a_marker) { return nullptr; }
			if (!a_marker->extraList.HasType<RE::ExtraMapMarker>()) { return nullptr; }

			return a_marker->extraList.GetByType<RE::ExtraMapMarker>();
		}

		//---------------------------------------------------
		//-- Utility Functions ( Check Colleced Forms ) -----
		//---------------------------------------------------

		//Normal Function
		[[nodiscard]] static bool CheckIsCollectable(RE::FormID a_form) noexcept {
			for (auto* data : CompletionistData::NewItemData) {
				if (data && data->HasForm(a_form)) { return true; }
			}
			return false;
		}

		//Overload To Pass Through TESForm
		[[nodiscard]] static bool CheckIsCollectable(RE::TESForm* a_form) noexcept {
			return a_form ? CheckIsCollectable(a_form->GetFormID()) : false;
		}

		//Overload To Pass Through TESObjectREFR (Only useful for SOB)
		[[nodiscard]] static bool CheckIsCollectable(RE::TESObjectREFR* a_form) noexcept {
			return a_form ? CheckIsCollectable(a_form->GetBaseObject()->GetFormID()) : false;
		}

		//---------------------------------------------------
		//-- Utility Functions ( Get Skill Book String ) ----
		//---------------------------------------------------

		[[nodiscard]] auto GetBookSkill(RE::ActorValue a_val) noexcept { 

			switch (a_val) {  // Maybe a better way of doing this???

			case RE::ActorValue::kOneHanded:
				return "One-Handed";
			case RE::ActorValue::kTwoHanded:
				return "Two-Handed";
			case RE::ActorValue::kArchery:
				return "Marksman";
			case RE::ActorValue::kBlock:
				return "Block";
			case RE::ActorValue::kSmithing:
				return "Smithing";
			case RE::ActorValue::kHeavyArmor:
				return "Heavy Armor";
			case RE::ActorValue::kLightArmor:
				return "Light Armor";
			case RE::ActorValue::kPickpocket:
				return "Pickpocket";
			case RE::ActorValue::kLockpicking:
				return "LockPicking";
			case RE::ActorValue::kSneak:
				return "Sneak";
			case RE::ActorValue::kAlchemy:
				return "Alchemy";
			case RE::ActorValue::kSpeech:
				return "SpeechCraft";
			case RE::ActorValue::kAlteration:
				return "Alteration";
			case RE::ActorValue::kConjuration:
				return "Conjuration";
			case RE::ActorValue::kDestruction:
				return "Destruction";
			case RE::ActorValue::kIllusion:
				return "Illusion";
			case RE::ActorValue::kRestoration:
				return "Restoration";
			case RE::ActorValue::kEnchanting:
				return "Enchanting";
			default:
				return "";
			}
		}

		//---------------------------------------------------
		//-- Utility Functions ( Sets the DataSet To Save ) -
		//---------------------------------------------------

		void SetAsSerializable() noexcept {
			CompletionistData::ManagedData.emplace_back(this);
		}

		//---------------------------------------------------
		//-- Utility Functions ( Merges For Collectables ) --
		//---------------------------------------------------

		void MergeAsCollectable() noexcept {
			CompletionistData::NewItemData.emplace_back(this);
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Add Form ) ------
		//---------------------------------------------------

		//Normal Function
		[[nodiscard]] void AddForm(RE::FormID a_form) noexcept {
			if (!a_form) { return; } 
			data.try_emplace(a_form);
		}

		//Overload To Pass Through TESForm
		[[nodiscard]] void AddForm(RE::TESForm* a_form) noexcept {
			if (!a_form || !a_form->GetFormID()) { return; } 
			data.try_emplace(a_form->GetFormID());
		}

		//Overload To Pass Through FormID With File Name
		[[nodiscard]] void AddForm(RE::FormID a_form, std::string_view a_filename) noexcept {
			if (auto valid = IsModInstalled(a_filename) && RE::TESDataHandler::GetSingleton() != nullptr; !valid) { return; }
			if (auto form = RE::TESDataHandler::GetSingleton()->LookupFormID(a_form, a_filename); form) {
				data.try_emplace(form);
			}
		}

		//Overload To Pass Through FormID With File Name and (1) Variation (Also used to add variations to existing base files)
		[[nodiscard]] void AddForm(RE::FormID a_base, std::string_view a_filename, RE::FormID a_vari) noexcept {
			if (auto valid = IsModInstalled(a_filename) && RE::TESDataHandler::GetSingleton() != nullptr; !valid) { return; }
			
			auto base = RE::TESDataHandler::GetSingleton()->LookupFormID(a_base, a_filename);
			auto vari = RE::TESDataHandler::GetSingleton()->LookupFormID(a_vari, a_filename);

			if (!base || !vari || (HasForm(base) && HasForm(vari))) { return; }

			data.try_emplace(base);
			data.try_emplace(vari, base);
		}

		//Overload To Pass Through FormID With Base File And (1) Variation From A Seperate File (Also used to add variations to existing base files)
		[[nodiscard]] void AddForm(RE::FormID a_base, std::string_view a_bfilename, RE::FormID a_vari, std::string a_mfilename) noexcept {
			if (auto valid = IsModInstalled(a_bfilename) && RE::TESDataHandler::GetSingleton() != nullptr; !valid) { return; }

			auto base = RE::TESDataHandler::GetSingleton()->LookupFormID(a_base, a_bfilename);
			auto vari = RE::TESDataHandler::GetSingleton()->LookupFormID(a_vari, a_mfilename);

			if (!base || !vari || (HasForm(base) && HasForm(vari))) { return; }

			data.try_emplace(base);
			data.try_emplace(vari, base);
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Erase Form ) ----
		//---------------------------------------------------

		[[nodiscard]] void RemoveForm(RE::FormID a_form) noexcept {
			if (!HasForm(a_form)) { return; }
			data.erase(a_form);
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Has Form ) ------
		//---------------------------------------------------

		[[nodiscard]] bool HasForm(RE::FormID a_form) const noexcept {
			return data.contains(a_form);
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Get Form ) ------
		//---------------------------------------------------

		[[nodiscard]] RE::TESForm* GetForm(RE::FormID a_form) noexcept {
			return HasForm(a_form) ? RE::TESForm::LookupByID(a_form) : nullptr;
		}

		template <typename T>
		[[nodiscard]] T* GetForm(RE::FormID a_form) noexcept {
			auto* form = GetForm(a_form);
			return form ? static_cast<T*>(form) : nullptr;
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Get Base ) ------
		//---------------------------------------------------

		[[nodiscard]] RE::FormID GetBase(RE::FormID a_variation) const noexcept {	
			return HasForm(a_variation) ? data.at(a_variation) : 0;
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Is Variation ) --
		//---------------------------------------------------

		[[nodiscard]] bool IsVariation(RE::FormID a_form) const noexcept {
			return GetBase(a_form);
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Get File Name ) -
		//---------------------------------------------------

		[[nodiscard]] std::string_view GetFileName(RE::FormID a_form) noexcept {
			return HasForm(a_form) ? GetForm(a_form)->GetFile()->GetFilename() : "";
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Get All Forms ) -
		//---------------------------------------------------

		[[nodiscard]] auto GetAllForms() noexcept {
			return std::views::keys(data);
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Get All bases ) -
		//---------------------------------------------------

		[[nodiscard]] auto GetAllBases() noexcept {
			return std::views::keys(data) | std::views::filter([&](auto f) { return !IsVariation(f); });
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Get All varia ) -
		//---------------------------------------------------

		[[nodiscard]] auto GetAllVariations() noexcept {
			return std::views::keys(data) | std::views::filter([&](auto f) { return IsVariation(f); });
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Get Data Set ) --
		//---------------------------------------------------

		[[nodiscard]] constexpr auto& get() noexcept { 
			return data; 
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Compile Forms ) -
		//---------------------------------------------------

		template <std::size_t N>
		void CompileFormArray(const RE::FormID(&a_array)[N], std::string_view a_filename) noexcept
		{
			auto* handler = RE::TESDataHandler::GetSingleton();
			if (!handler) { ERROR("Failed to obtain TESDataHandler*, maybe too early?"); }

			for (auto idx = 0; idx < N; ++idx) {
				auto form = handler->LookupFormID(a_array[idx], a_filename);
				if (!form) { continue; }
				data.try_emplace(form);
			}
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Compile Varia ) -
		//---------------------------------------------------

		template <std::size_t N>
		void CompileVariation(const Variation(&a_variation)[N], std::string_view a_filename) noexcept
		{
			auto* handler = RE::TESDataHandler::GetSingleton();
			if (!handler) { ERROR("Failed to obtain TESDataHandler*, maybe too early?"); }

			for (auto idx = 0; idx < N; ++idx) {
				if (a_variation[idx].second.empty()) { continue; }

				auto baseID = handler->LookupFormID(a_variation[idx].first, a_filename);
				if (!baseID) { continue; }

				data.try_emplace(baseID);
				for (auto var : a_variation[idx].second) {
					if (var) {
						auto varID = handler->LookupFormID(var, a_filename);
						data.try_emplace(varID, baseID);
					}
					else {
						break;
					}
				}
			}
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Populate Array )-
		//---------------------------------------------------

		void Populate(std::vector<std::string>& a_names, std::vector<RE::TESForm*>& a_forms, std::vector<bool>& a_bools, std::vector<std::string>& a_texts,
			bool a_nosort = false, int a_opttype = -1) // a_opttype - book = 1, Loc = 2*/
		{
			a_names.clear();
			a_forms.clear();
			a_bools.clear();
			a_texts.clear();

			using zipped_t = std::pair<std::string, std::pair<RE::TESForm*, bool>>;

			auto bases = GetAllBases() |
				std::views::filter([&](auto f) { return GetForm(f) && GetForm(f)->GetName(); }) |
				std::views::transform([&](auto f) {
				std::string name = GetForm(f)->GetName();

				if (auto marker = GetForm<RE::TESObjectREFR>(f); marker && a_opttype == 2) {
					if (auto extraMapMarker = GetMapMarkerInternal(GetForm<RE::TESObjectREFR>(f)); extraMapMarker && extraMapMarker->mapData) {
						name = extraMapMarker->mapData->locationName.fullName.c_str();
					}
				}

				name[0] = std::toupper(name[0]);  // in case some mod forms have lower case name e.g wraithguard / sunder etc...
				return std::make_pair(std::move(name), std::make_pair(GetForm(f), false));
					});

			std::vector<zipped_t> zipped = { std::ranges::begin(bases), std::ranges::end(bases) };

			if (!a_nosort) { std::ranges::sort(zipped); }

			for (auto& [name, data] : zipped) {
				auto& [form, status] = data;
				a_names.emplace_back(name);
				a_forms.emplace_back(form);
				a_bools.emplace_back(status);
			}

			// assertions can be removed
			assert(a_names.size() == zipped.size());
			assert(a_names.size() == a_forms.size());
			assert(a_bools.size() == a_forms.size());

			a_texts = std::vector<std::string>(a_names.size(), "NO_HIGHLIGHT");

			if (a_opttype == 1) { a_texts.clear();
				for (auto form : a_forms) { auto* book = static_cast<RE::TESObjectBOOK*>(form);
					if (book->GetSpell()) {
						a_texts.push_back("$AddSpellTomeHighlight{" + std::string(form->GetName()) + "}{" + book->GetSpell()->GetName() + "}");
					}
					else if (book->TeachesSkill()) {
						a_texts.push_back("$AddSkillBookHighlight{" + std::string(form->GetName()) + "}{" + GetBookSkill(book->GetSkill()) + "}");
					}
					else {
						a_texts.push_back("NO_HIGHLIGHT");
					}
				}
			}

			if (a_opttype == 2) { a_texts.clear();
				for (auto& name : a_names) { a_texts.push_back("$AddLocationHighlight{" + name + "}"); }
			}
		}

		//---------------------------------------------------
		//-- Completionist Serialization ( Save Data ) ------
		//---------------------------------------------------

		void Save([[maybe_unused]] SKSE::SerializationInterface* a_intfc) noexcept
		{
			std::size_t total = data.size();
			if (!a_intfc->WriteRecordData(&total, sizeof(total))) { ERROR("Failed to write serialized data"); }

			for (auto& m : data) {
				if (!a_intfc->WriteRecordData(&m, sizeof(m))) { ERROR("Failed to write serialized data"); }
			}

			INFO("Saved SKSE co-save with a size of - {}", total);
		}

		//---------------------------------------------------
		//-- Completionist Serialization ( Load Data ) ------
		//---------------------------------------------------

		void Load([[maybe_unused]] SKSE::SerializationInterface* a_intfc) noexcept
		{
			std::size_t total;
			if (!a_intfc->ReadRecordData(&total, sizeof(total))) { ERROR("Failed to read serialized data"); }

			for (auto i : std::views::iota(static_cast<std::size_t>(0), total)) {
				RE::FormID form, base;
				if (!a_intfc->ReadRecordData(&form, sizeof(form))) { continue; }

				if (!form || !a_intfc->ResolveFormID(form, form)) { continue; }

				if (!a_intfc->ReadRecordData(&base, sizeof(base))) { continue; }

				if (base && !a_intfc->ResolveFormID(base, base)) { continue; }

				data.try_emplace(form, base);
			}

			INFO("Loaded SKSE co-save with a size of - {}", data.size());
		}

		//---------------------------------------------------
		//-- Completionist Serialization ( Clear Data ) -----
		//---------------------------------------------------

		void Revert([[maybe_unused]] SKSE::SerializationInterface* a_intfc) noexcept
		{
			INFO("Reverting SKSE co-save");
			data.clear();
		}

		//---------------------------------------------------
		//-- Completionist Serialization ( Members ) --------
		//---------------------------------------------------

		std::unordered_map<RE::FormID, RE::FormID> data;
		inline static std::vector<CompletionistData*> ManagedData = {};
		inline static std::vector<CompletionistData*> NewItemData = {};
	};

	//---------------------------------------------------
	//-- SKSE Callback Functions ( Save Callback ) ------
	//---------------------------------------------------

	static void SaveCallback([[maybe_unused]] SKSE::SerializationInterface* a_intfc) noexcept
	{
		if (!a_intfc->OpenRecord(kHeader, kVersion)) { ERROR("Failed to open record"); }

		for (auto* data : CompletionistData::ManagedData) {
			if (data) { data->Save(a_intfc); }
		}
	}

	//---------------------------------------------------
	//-- SKSE Callback Functions ( Load Callback ) ------
	//---------------------------------------------------

	static void LoadCallback([[maybe_unused]] SKSE::SerializationInterface* a_intfc) noexcept
	{
		std::uint32_t type, version, length;
		while (a_intfc->GetNextRecordInfo(type, version, length)) {
			if (type != kHeader || version > kVersion) { break; }

			if (version < kVersion) {
				/*if somehow the serialized data structure changed, thus resulting a kVersion change
				  it must be able to patch/regress the backward compatibility or it will ERROR*/
			}

			for (auto* data : CompletionistData::ManagedData) {
				if (data) { data->Load(a_intfc); }
			}
		}
	}

	//---------------------------------------------------
	//-- SKSE Callback Functions ( Clear Callback ) -----
	//---------------------------------------------------

	static void RevertCallback([[maybe_unused]] SKSE::SerializationInterface* a_intfc) noexcept
	{
		for (auto* data : CompletionistData::ManagedData) {
			if (data) { data->Revert(a_intfc); }
		}
	}
}