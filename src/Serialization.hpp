#include "Structs.hpp"
#include "DKUtil/Utility.hpp"
#include "Internal Utility/Localisation.hpp"
#include "Internal Utility/Variables.hpp"
#pragma once

#undef GetForm
#undef AddForm

namespace Serialization
{
	enum : std::uint32_t
	{
		kHeader = 'COMP',
		kVersion = 1018,
	};

#define SetSerializableInfo(DATA) (DATA).SetAsSerializable(#DATA)
constexpr auto DEFAULT_VARIATION_MAX = 13;

	using FormArray = RE::FormID[];
	using Variation = std::pair<RE::FormID, std::array<RE::FormID, DEFAULT_VARIATION_MAX>>;

	struct ISerializable
	{
		virtual void Save(SKSE::SerializationInterface*, std::string_view) noexcept = 0;
		virtual void Load(SKSE::SerializationInterface*, std::string_view) noexcept = 0;
		virtual void Revert([[maybe_unused]] SKSE::SerializationInterface*, std::string_view) noexcept = 0;

		void SetAsSerializable(std::string a_setName) noexcept
		{
			ManagedData.emplace_back(this, a_setName);
		}

		inline static std::vector<std::pair<ISerializable*, std::string>> ManagedData = {};
	};

	struct CompletionistData final : public ISerializable
	{
		//---------------------------------------------------
		//-- Utility Functions ( Is Mod Installed ) ---------
		//---------------------------------------------------

		[[nodiscard]] static bool IsModInstalled(std::string_view a_modname) noexcept
		{	
			auto* handler = RE::TESDataHandler::GetSingleton();
			if (!handler) {
				return false;
			}

			auto* file1 = handler->LookupModByName(a_modname);
			auto* file2 = handler->LookupLoadedModByName(a_modname);
			auto* file3 = handler->LookupLoadedLightModByName(a_modname);
			return file1 || file2 || file3;
		}

		//---------------------------------------------------
		//-- Utility Functions ( Is Mod Installed ) ---------
		//---------------------------------------------------

		[[nodiscard]] static bool IsModInstalled(std::string_view a_modname, RE::FormID a_formID) noexcept
		{
			auto* handler = RE::TESDataHandler::GetSingleton();
			if (!handler) {
				return false;
			}

			auto* form = RE::TESDataHandler::GetSingleton()->LookupForm(a_formID, a_modname);
			return form != nullptr;
		}

		//---------------------------------------------------
		//-- Utility Functions ( Is Map Marker ) ------------
		//---------------------------------------------------

		[[nodiscard]] static RE::ExtraMapMarker* GetMapMarkerInternal(RE::TESObjectREFR* a_marker)
		{
			if (!a_marker) {
				return nullptr;
			}

			if (!a_marker->extraList.HasType<RE::ExtraMapMarker>()) {
				return nullptr;
			}

			return a_marker->extraList.GetByType<RE::ExtraMapMarker>();
		}

		[[nodiscard]] static std::string GetMapMarkerName(RE::TESForm* a_form)
		{
			if (!a_form) {
				return "";
			}

			auto a_marker = static_cast<RE::TESObjectREFR*>(a_form);
			if (!a_marker->extraList.HasType<RE::ExtraMapMarker>()) {
				return "";
			}

			return a_marker->extraList.GetByType<RE::ExtraMapMarker>()->mapData->locationName.fullName.c_str();
		}

		//---------------------------------------------------
		//-- Utility Functions ( Get FormID Hex String ) ----
		//---------------------------------------------------

		[[nodiscard]] static auto IsFormExcludable(RE::TESObjectREFR* a_form) noexcept
		{
			return a_form && a_form->GetFile(0) && !GetFormIDHexString(a_form).starts_with("FF");
		}

		//---------------------------------------------------
		//-- Utility Functions ( Get FormID Hex String ) ----
		//---------------------------------------------------

		[[nodiscard]] static std::string GetFormIDHexString(RE::TESObjectREFR* a_form) noexcept
		{
			auto idx = GetModIndexFromForm(a_form);
			auto fID = fmt::format("{:x}", a_form->GetFormID());
			std::string pID = "";

			if (fID.contains(idx))
			{
				pID = fmt::format(" - [{:s}]"sv, fID.substr(fID.find(idx) + idx.length()));
			}

			return std::format("{:08X}{:s}", a_form->GetFormID(), pID);
		}

		//---------------------------------------------------
		//-- Utility Functions ( Get FormID Hex String ) ----
		//---------------------------------------------------

		[[nodiscard]] static std::string GetFormIDHexString(RE::TESForm* a_form) noexcept
		{
			return std::format("0x{:06X}", a_form->GetFormID());
		}

		[[nodiscard]] static std::string GetFormIDHexString(RE::FormID a_formID) noexcept
		{
			return std::format("0x{:06X}", a_formID);
		}

		//---------------------------------------------------
		//-- Utility Functions ( Get FormID Hex String ) ----
		//---------------------------------------------------

		[[nodiscard]] static std::string GetFormOwner(RE::TESObjectREFR* a_form) noexcept
		{
			if (!a_form || !a_form->GetFile(0))
			{
				return "";
			};

			auto idx = GetModIndexFromForm(a_form);
			return fmt::format("{:s}{:s}", a_form->GetFile(0)->GetFilename(), idx == "0" ? "" : fmt::format(" - [{:s}]",idx));
		}

		//---------------------------------------------------
		//-- Utility Functions ( Get FormID Hex String ) ----
		//---------------------------------------------------

		[[nodiscard]] static std::string GetModIndexFromForm(const RE::TESObjectREFR* a_form) noexcept
		{
			if (!a_form || !a_form->GetFile(0))
			{
				return "";
			};

			return fmt::format("{:x}", a_form->GetFile(0)->GetPartialIndex());
		}

		//---------------------------------------------------
		//-- Utility Functions ( Get FormID Hex String ) ----
		//---------------------------------------------------

		[[nodiscard]] static std::string GetModIndexFromForm(RE::TESForm* a_form) noexcept
		{
			if (!a_form || !a_form->GetFile(0))
			{
				return "";
			};

			return fmt::format("{:x}", a_form->GetFile(0)->GetPartialIndex());
		}

		//---------------------------------------------------
		//-- Utility Functions ( Has Keyword String ) -------
		//---------------------------------------------------

		[[nodiscard]] static auto HasKeywordString(RE::TESForm* a_form, const char* a_keyword) noexcept
		{
			const auto keywordForm = a_form->As<RE::BGSKeywordForm>();
			return keywordForm && keywordForm->HasKeywordString(a_keyword);
		}

		//---------------------------------------------------
		//-- Utility Functions ( Get Skill Book String ) ----
		//---------------------------------------------------

		[[nodiscard]] auto GetBookSkill(RE::ActorValue a_val) noexcept
		{
			static DKUtil::enumeration<RE::ActorValue, std::uint32_t> actorValueNameTbl{};

			auto rawName = actorValueNameTbl.to_string(a_val);

			for (auto i = 0; i < rawName.length(); ++i) {
				if (std::isupper(rawName[i])) {
					rawName.insert(i++, " ");
				}
			}

			return rawName.erase(0, 2);
		}

		//---------------------------------------------------
		//-- Utility Functions ( Check Colleced Forms ) -----
		//---------------------------------------------------

		//Normal Function
		[[nodiscard]] static bool CheckIsCollectable(RE::FormID a_form) noexcept
		{
			for (auto* data : CompletionistData::NewItemData) {
				if (data && data->HasForm(a_form)) {
					return true;
				}
			}
			return false;
		}

		//Overload To Pass Through TESForm
		[[nodiscard]] static bool CheckIsCollectable(RE::TESForm* a_form) noexcept
		{
			return a_form ? CheckIsCollectable(a_form->GetFormID()) : false;
		}

		//Overload To Pass Through TESObjectREFR (Only useful for SOB)
		[[nodiscard]] static bool CheckIsCollectable(RE::TESObjectREFR* a_form) noexcept
		{
			return a_form ? CheckIsCollectable(a_form->GetBaseObject()->GetFormID()) : false;
		}

		//---------------------------------------------------
		//-- Utility Functions ( Merges For Collectables ) --
		//---------------------------------------------------

		void MergeAsCollectable() noexcept
		{
			NewItemData.emplace_back(this);
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Add Form ) ------
		//---------------------------------------------------

		//Normal Function
		void AddForm(RE::FormID a_form) noexcept
		{
			if (!a_form) {
				return;
			}
			data.try_emplace(a_form);
		}

		//Overload To Pass Through TESForm
		void AddForm(RE::TESForm* a_form) noexcept
		{
			if (!a_form || !a_form->GetFormID()) {
				return;
			}
			data.try_emplace(a_form->GetFormID());
		}

		//Overload To Pass Through FormID With File Name
		void AddForm(RE::FormID a_form, std::string_view a_filename) noexcept
		{
			if (!IsModInstalled(a_filename)) {
				return;
			}
			if (auto form = RE::TESDataHandler::GetSingleton()->LookupFormID(a_form, a_filename)) {
				data.try_emplace(form);
			}
		}

		//Overload To Pass Through FormID With File Name and (1) Variation (Also used to add variations to existing base files)
		void AddForm(RE::FormID a_base, std::string_view a_filename, RE::FormID a_vari) noexcept
		{
			if (!IsModInstalled(a_filename)) {
				return;
			}

			auto base = RE::TESDataHandler::GetSingleton()->LookupFormID(a_base, a_filename);
			auto vari = RE::TESDataHandler::GetSingleton()->LookupFormID(a_vari, a_filename);

			if (!base || !vari || (HasForm(base) && HasForm(vari))) {
				return;
			}

			data.try_emplace(base);
			data.try_emplace(vari, base);
		}

		//Overload To Pass Through FormID With Base File And (1) Variation From A Seperate File (Also used to add variations to existing base files)
		void AddForm(RE::FormID a_base, std::string_view a_bfilename, RE::FormID a_vari, std::string a_mfilename) noexcept
		{
			auto* handler = RE::TESDataHandler::GetSingleton();

			if (!IsModInstalled(a_bfilename) || !handler) {
				INFO("Unable to Install {} from {} as {} is not installed.", a_vari, a_mfilename, a_bfilename);
				return;
			}

			if (!IsModInstalled(a_mfilename) || !handler) {
				INFO("Unable to Install {} from {} as {} is not installed.", a_vari, a_mfilename, a_mfilename);
				return;
			}

			auto base = handler->LookupFormID(a_base, a_bfilename);
			auto vari = handler->LookupFormID(a_vari, a_mfilename);

			if (!base || !vari || (HasForm(base) && HasForm(vari))) {
				return;
			}

			data.try_emplace(base);
			data.try_emplace(vari, base);
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Erase Form ) ----
		//---------------------------------------------------

		void RemoveForm(RE::FormID a_form) noexcept
		{
			if (!HasForm(a_form)) {
				return;
			}
			data.erase(a_form);
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Has Form ) ------
		//---------------------------------------------------

		[[nodiscard]] bool HasForm(RE::FormID a_form) const noexcept
		{
			return a_form && data.contains(a_form);
		}

		[[nodiscard]] bool HasForm(RE::TESForm* a_form) const noexcept
		{
			return a_form && data.contains(a_form->GetFormID());
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Player Hits ) ---
		//---------------------------------------------------

		void AddTarget(RE::FormID a_form, RE::FormID a_weapon) noexcept
		{
			if (!a_form) {
				return;
			}
			data.try_emplace(a_form, a_weapon);
		}

		void AddTarget(RE::TESForm* a_form, RE::TESForm* a_weapon) noexcept
		{
			if (!a_form || !a_form->GetFormID()) {
				return;
			}
			data.try_emplace(a_form->GetFormID(), a_weapon->GetFormID());
		}

		[[nodiscard]] bool HasTargetRegistered(RE::FormID a_form) const noexcept
		{
			return HasForm(a_form);
		}

		[[nodiscard]] bool HasTargetRegistered(RE::TESForm* a_form) const noexcept
		{
			return HasForm(a_form);
		}

		void UpdateTargetWeapon(RE::TESForm* a_form, RE::TESForm* a_weapon) noexcept
		{
			if (HasForm(a_form)) {
				data[a_form->GetFormID()] = a_weapon->GetFormID();
			};
		};

		void UpdateTargetWeapon(RE::FormID a_form, RE::FormID a_weapon) noexcept
		{
			if (HasForm(a_form)) {
				data[a_form] = a_weapon;
			};
		};

		void RemoveTarget(RE::FormID a_form) noexcept
		{
			RemoveForm(a_form);
		}

		[[nodiscard]] RE::TESForm* GetTargetWeapon(RE::TESForm* a_form) noexcept
		{
			return HasForm(a_form) ? RE::TESForm::LookupByID(data[a_form->GetFormID()]) : nullptr;
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Get Form ) ------
		//---------------------------------------------------

		[[nodiscard]] RE::TESForm* GetForm(RE::FormID a_form) const noexcept
		{
			return HasForm(a_form) ? RE::TESForm::LookupByID(a_form) : nullptr;
		}

		template <typename T>
		[[nodiscard]] T* GetForm(RE::FormID a_form) noexcept
		{
			auto* form = GetForm(a_form);
			return form ? form->As<T>() : nullptr;
		}

		template <typename T = RE::TESForm>
		[[nodiscard]] static T* GetFullForm(RE::FormID a_form, const char* a_filename) noexcept
		{
			auto* form = RE::TESDataHandler::GetSingleton()->LookupForm(a_form, a_filename);
			return form ? form->As<T>() : nullptr;
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Get Base ) ------
		//---------------------------------------------------

		[[nodiscard]] RE::FormID GetBase(RE::FormID a_variation) const noexcept
		{
			return HasForm(a_variation) ? data.at(a_variation) : 0;
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Is Variation ) --
		//---------------------------------------------------

		[[nodiscard]] bool IsVariation(RE::FormID a_form) const noexcept
		{
			return GetBase(a_form);
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Get File Name ) -
		//---------------------------------------------------

		[[nodiscard]] std::string_view GetFileName(RE::FormID a_form) const noexcept
		{
			return HasForm(a_form) ? GetForm(a_form)->GetFile()->GetFilename() : std::string_view{};
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Get All Forms ) -
		//---------------------------------------------------

		[[nodiscard]] auto GetAllForms() noexcept
		{
			return std::views::keys(data);
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Get All bases ) -
		//---------------------------------------------------

		[[nodiscard]] auto GetAllBases() noexcept
		{
			return std::views::keys(data) | std::views::filter([&](auto f) { return !IsVariation(f); });
		}

		[[nodiscard]] auto GetAllBases() const noexcept
		{
			return std::views::keys(data) | std::views::filter([&](auto f) { return !IsVariation(f); });
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Get All varia ) -
		//---------------------------------------------------

		[[nodiscard]] auto GetAllVariations() noexcept
		{
			return std::views::keys(data) | std::views::filter([&](auto f) { return IsVariation(f); });
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Get Data Set ) --
		//---------------------------------------------------

		[[nodiscard]] constexpr auto& get() noexcept
		{
			return data;
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Compile Forms ) -
		//---------------------------------------------------

		template <std::size_t N>
		void CompileFormArray(const RE::FormID(&a_array)[N], std::string_view a_filename) noexcept
		{
			auto* handler = RE::TESDataHandler::GetSingleton();
			if (!handler) {
				ERROR("Failed to obtain TESDataHandler*, maybe too early?");
			}

			for (auto idx = 0; idx < N; ++idx) {
				auto form = handler->LookupFormID(a_array[idx], a_filename);
				if (!form) {
					continue;
				}
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
			if (!handler) {
				ERROR("Failed to obtain TESDataHandler*, maybe too early?");
			}

			for (auto idx = 0; idx < N; ++idx) {
				if (a_variation[idx].second.empty()) {
					continue;
				}

				auto baseID = handler->LookupFormID(a_variation[idx].first, a_filename);
				if (!baseID) {
					continue;
				}

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
			bool a_nosort = false, int a_opttype = -1)  // a_opttype - book = 1, Loc = 2
		{
			a_names.clear();
			a_forms.clear();
			a_bools.clear();
			a_texts.clear();

			using zipped_t = std::pair<std::string, std::pair<RE::TESForm*, bool>>;

			auto bases = GetAllBases() 
				| std::views::filter([&](auto f) { return GetForm(f) && GetForm(f)->GetName(); }) 
				| std::views::transform([&](auto f) {
				std::string name = GetForm(f)->GetName();

				if (auto* marker = GetForm<RE::TESObjectREFR>(f); marker && a_opttype == 2) {
					if (auto extraMapMarker = GetMapMarkerInternal(GetForm<RE::TESObjectREFR>(f)); extraMapMarker && extraMapMarker->mapData) {
						name = extraMapMarker->mapData->locationName.fullName.c_str();
					}
				}

				name[0] = std::toupper(name[0]);  // in case some mod forms have lower case name e.g wraithguard / sunder etc...
				return std::make_pair(std::move(name), std::make_pair(GetForm(f), false));
					});

			std::vector<zipped_t> zipped = { std::ranges::begin(bases), std::ranges::end(bases) };

			if (!a_nosort) {
				std::ranges::sort(zipped);
			}

			for (auto& [name, data] : zipped) {
				auto& [form, status] = data;
				a_names.emplace_back(name);
				a_forms.emplace_back(form);
				a_bools.emplace_back(status);
			}

			switch (a_opttype) {
			case 1:
			{
				for (auto* form : a_forms) {
					auto* book = static_cast<RE::TESObjectBOOK*>(form);
					if (book && book->GetSpell()) {
						a_texts.push_back("$AddSpellTomeHighlight{" + std::string(form->GetName()) + "}{" + GetBookSkill(book->GetSpell()->GetAssociatedSkill()) + "}{" + book->GetSpell()->GetName() + "}");
					}
					else if (book && book->TeachesSkill()) {
						a_texts.push_back("$AddSkillBookHighlight{" + std::string(form->GetName()) + "}{" + GetBookSkill(book->GetSkill()) + "}");
					}
					else {
						a_texts.push_back("NO_HIGHLIGHT");
					}
				}
				break;
			}
			case 2:
			{
				for (auto& name : a_names) {
					a_texts.push_back("$AddLocationHighlight{" + name + "}");
				}
				break;
			}
			default:
			{
				a_texts = std::vector<std::string>(zipped.size(), "NO_HIGHLIGHT");
				break;
			}
			}
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Populate Array )-
		//---------------------------------------------------

		void PopulateSpellTomes(
			std::vector<std::string>& a_names, std::vector<RE::TESForm*>& a_forms, std::vector<bool>& a_bools, std::vector<std::string>& a_texts, RE::ActorValue a_actorval)
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

				name[0] = std::toupper(name[0]);  // in case some mod forms have lower case name e.g wraithguard / sunder etc...
				return std::make_pair(std::move(name), std::make_pair(GetForm(f), false));
					});

			std::vector<zipped_t> zipped = { std::ranges::begin(bases), std::ranges::end(bases) };
			std::ranges::sort(zipped);

			for (auto& [name, data] : zipped) {
				auto& [form, status] = data;

				auto* book = static_cast<RE::TESObjectBOOK*>(form);
				if (book && book->GetSpell()) {
					if (book->GetSpell()->GetAssociatedSkill() == a_actorval) {
						a_names.emplace_back(name);
						a_bools.emplace_back(status);
						a_forms.emplace_back(form);
						a_texts.emplace_back("$AddSpellTomeHighlight{" + std::string(form->GetName()) + "}{" + GetBookSkill(book->GetSpell()->GetAssociatedSkill()) + "}{" + book->GetSpell()->GetName() + "}");
					}
				}
			}
		}

		//---------------------------------------------------
		//-- Completionist Serialization ( SKSE APIs ) ------
		//---------------------------------------------------

		virtual void Save(SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept
		{
			std::size_t total = data.size();
			if (!a_intfc->WriteRecordData(&total, sizeof(total))) {
				ERROR("Failed to write serialized form data: size");
			}

			for (auto& m : data) {
				if (!a_intfc->WriteRecordData(&m, sizeof(m))) {
					ERROR("Failed to write serialized form data: pair");
				}
			}

			INFO("Saved {} to co-save with a size of - {}", a_name, total);
		}

		virtual void Load(SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept override
		{
			std::size_t total;
			if (!a_intfc->ReadRecordData(&total, sizeof(total))) {
				ERROR("Failed to read serialized form data: size");
			}

			auto read = 0;
			for (auto i = 0; i < total; ++i) {
				RE::FormID form, base;
				if (!a_intfc->ReadRecordData(&form, sizeof(form)) ||
					!a_intfc->ReadRecordData(&base, sizeof(base))) {
					continue;
				}

				if (!form || !a_intfc->ResolveFormID(form, form)) {
					continue;
				}

				if (base && !a_intfc->ResolveFormID(base, base)) {
					continue;
				}

				data.try_emplace(form, base);

				read++;
			}

			if (read != total) {
				INFO("Lost data while loading {} from co-save... Expected: {} Written: {}", a_name, total, read);
			}

			INFO("Loaded {} from co-save with a size of - {}", a_name, data.size());
		}

		virtual void Revert([[maybe_unused]] SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept
		{
			INFO("Reverting {} from co-save", a_name);
			data.clear();
		}

		//---------------------------------------------------
		//-- Completionist Serialization ( Members ) --------
		//---------------------------------------------------

		std::unordered_map<RE::FormID, RE::FormID> data;
		inline static std::vector<CompletionistData*> NewItemData = {};
	};

	// new polymorphed data structure designed for keys
	struct CompletionistKey final : public ISerializable
	{
		// modifier
		void AddKey(std::string_view a_key) noexcept
		{
			if (!data.contains(a_key.data())) {
				data.try_emplace(a_key.data());
			}
		}

		void AddStage(std::string_view a_editorID, int a_stage, std::string_view a_name) noexcept
		{
			if (HasStage(a_editorID, a_stage)) {
				return;
			}

			auto stage = fmt::format("|{}|", a_stage);
			data[a_editorID.data()] += stage;
			INFO("Stage Recorder Added Stage {} to '{}' Serialized Map.", a_stage, fmt::format("{:s} [{}]"sv, a_editorID, a_name));
		}

		void RemoveKey(std::string_view a_key) noexcept
		{
			if (HasKey(a_key)) {
				data.erase(a_key.data());
			}
		}

		void RemoveStage(std::string_view a_key, int a_stage) noexcept
		{
			if (HasKey(a_key)) {
				auto stage = fmt::format("|{}|", a_stage);
				data[a_key.data()] = DKUtil::string::replace_nth_occurrence(data[a_key.data()], 0, stage, {});
			}
		}

		// accessor
		[[nodiscard]] bool HasKey(std::string_view a_key) const noexcept
		{
			return data.contains(a_key.data());
		}

		[[nodiscard]] bool HasStage(std::string_view a_key, int a_stage) noexcept
		{
			if (!HasKey(a_key)) {
				return false;
			}

			auto stage = fmt::format("|{}|", a_stage);
			return DKUtil::string::icontains(data[a_key.data()], stage);
		}

		[[nodiscard]] std::vector<int> GetAllStages(std::string_view a_key) noexcept
		{
			if (!HasKey(a_key)) {
				return {};
			}

			std::vector<int> list;
			auto raw = DKUtil::string::split(data[a_key.data()], "|");
			for (auto& r : raw) {
				if (!r.empty()) {
					list.push_back(std::stoi(r));
				}
			}

			return list;
		}

		//---------------------------------------------------
		//-- Completionist Serialization ( SKSE APIs ) ------
		//---------------------------------------------------

		virtual void Save(SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept
		{
			std::size_t total = data.size();
			if (!a_intfc->WriteRecordData(&total, sizeof(total))) {
				ERROR("Failed to write serialized form data: size");
			}

			auto written = 0;
			for (auto& [key, val] : data) {
				std::size_t keySize{ key.size() }, valSize{ val.size() };
				if (!a_intfc->WriteRecordData(&keySize, sizeof(keySize)) ||
					!a_intfc->WriteRecordData(&valSize, sizeof(valSize))) {
					ERROR("Failed to write serialized form data: pair_size");
				}

				if (!a_intfc->WriteRecordData(key.data(), keySize) ||
					!a_intfc->WriteRecordData(val.data(), valSize)) {
					ERROR("Failed to write serialized form data: pair_data");
				}

				written++;
			}

			if (written != total) {
				ERROR("Lost data during saving co-save!\nExpected: {}\nWritten: {}", total, written);
			}

			INFO("Saved {} to co-save with a size of - {}", a_name, total);
		}

		virtual void Load(SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept override
		{
			std::size_t total;
			if (!a_intfc->ReadRecordData(&total, sizeof(total))) {
				ERROR("Failed to read serialized form data: size");
			}

			auto read = 0;
			for (auto i = 0; i < total; ++i) {
				static std::string key, val;
				std::size_t keySize, valSize{};
				if (!a_intfc->ReadRecordData(&keySize, sizeof(keySize)) ||
					!a_intfc->ReadRecordData(&valSize, sizeof(valSize))) {
					ERROR("Failed to read serialized form data: pair_size");
				}

				key.resize(keySize);
				val.resize(valSize);
				if (!a_intfc->ReadRecordData(key.data(), keySize) || (valSize && !a_intfc->ReadRecordData(val.data(), valSize))) {
					ERROR("Failed to read serialized form data: pair_data");
				}

				data.try_emplace(key, val);
				read++;
			}

			if (read != total) {
				INFO("Lost data while loading {} from co-save... Expected: {} Written: {}", a_name, total, read);
			}

			INFO("Loaded SKSE co-save {} with a size of - {}", a_name, data.size());
		}

		virtual void Revert([[maybe_unused]] SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept
		{
			INFO("Reverting {} from co-save", a_name);
			data.clear();
		}

		//---------------------------------------------------
		//-- Completionist Serialization ( Members ) --------
		//---------------------------------------------------

		std::unordered_map<std::string, std::string> data;
	};

	// new polymorphed data structure designed for Logging
	struct CompletionistLog final : public ISerializable
	{
		enum logType
		{
			kCollected,
			kDiscovered,
			kLearnt,
			kObtained,
			kBarenziah,
			kTamed,
			kShout,
			kWord,
			kFish,
			kBook,
			kTome,
			kQuestComplete,
		};

		// modifier
		void AddDate(std::string_view a_date) noexcept
		{
			if (!data.contains(a_date.data())) {
				data.try_emplace(a_date.data());
			}
		}

		void AddLoggedEvent(logType kType, std::string_view a_log) noexcept
		{
			time_t theTime = time(NULL);
			struct tm* aTime = localtime(&theTime);

			// Use fmt::format for consistent formatting and avoid manual length checks
			auto hr = fmt::format("{:02}", aTime->tm_hour); // Leading zero if necessary
			auto mi = fmt::format("{:02}", aTime->tm_min);
			auto da = fmt::format("{:02}", aTime->tm_mday);
			auto mo = fmt::format("{:02}", aTime->tm_mon + 1); // Month is 0-11, so +1 is needed
			auto yr = std::to_string(1900 + aTime->tm_year); // Year is already 4 digits

			auto date = fmt::format("{}/{}/{}", da, mo, yr);
			auto time = fmt::format("{}:{}", hr, mi);

			auto event = fmt::format("|{} ~ {}{}|", time, GetLogTypePrefx(kType), a_log);

			if (HasLoggedEvent(date, event)) {
				return;
			}

			AddDate(date);
			data[date.data()] += event;

			INFO("Log Recorder Added Log [{}] to '{}' Serialized Date Map.", a_log, date);
		}

		std::string GetLogTypePrefx(logType kType) {
			switch (kType)
			{
			case Serialization::CompletionistLog::kCollected:
				return GET_LOC_STRING_BY_KEY("LogPrefix_Col");
				break;
			case Serialization::CompletionistLog::kDiscovered:
				return GET_LOC_STRING_BY_KEY("LogPrefix_Dis");
				break;
			case Serialization::CompletionistLog::kLearnt:
				return GET_LOC_STRING_BY_KEY("LogPrefix_Lea");
				break;
			case Serialization::CompletionistLog::kShout:
				return GET_LOC_STRING_BY_KEY("LogPrefix_Sho");
				break;
			case Serialization::CompletionistLog::kWord:
				return GET_LOC_STRING_BY_KEY("LogPrefix_Wor");
				break;
			case Serialization::CompletionistLog::kObtained:
				return GET_LOC_STRING_BY_KEY("LogPrefix_Obt");
				break;
			case Serialization::CompletionistLog::kBarenziah:
				return GET_LOC_STRING_BY_KEY("LogPrefix_Bar");
				break;
			case Serialization::CompletionistLog::kTamed:
				return GET_LOC_STRING_BY_KEY("LogPrefix_Tam");
				break;
			case Serialization::CompletionistLog::kFish:
				return GET_LOC_STRING_BY_KEY("LogPrefix_Fis");
				break;
			case Serialization::CompletionistLog::kBook:
				return GET_LOC_STRING_BY_KEY("LogPrefix_Boo");
				break;
			case Serialization::CompletionistLog::kTome:
				return GET_LOC_STRING_BY_KEY("LogPrefix_Tom");
				break;
			case Serialization::CompletionistLog::kQuestComplete:
				return GET_LOC_STRING_BY_KEY("LogPrefix_Qst");
				break;
			default:
				break;
			}
			return std::string{};
		}

		std::string_view GetEntryTypeByLogPrefx(std::string_view entry, std::string_view qc, std::string_view ic, std::string_view bc, std::string_view sc) {
			using loc = CLocalisation::LocalisationAPI;

			if (entry.contains(loc::GetLocStringByKey("LogPrefix_Qst"))) {
				return qc;
			}

			if (entry.contains(loc::GetLocStringByKey("LogPrefix_Col"))) {
				return ic;
			}

			if (entry.contains(loc::GetLocStringByKey("LogPrefix_Boo")) || entry.contains(loc::GetLocStringByKey("LogPrefix_Tom"))) {
				return bc;
			}

			return sc;
		}

		// accessor
		[[nodiscard]] bool HasDate(std::string_view a_date) const noexcept
		{
			return data.contains(a_date.data());
		}

		[[nodiscard]] bool HasLoggedEvent(std::string_view a_date, std::string_view a_log) noexcept
		{
			if (!HasDate(a_date)) {
				return false;
			}

			auto event = fmt::format("|{}|", a_log);
			return DKUtil::string::icontains(data[a_date.data()], a_log);
		}

		[[nodiscard]] std::vector<std::string> GetAllLoggedDates() noexcept
		{		
			std::vector<std::string> list;

			for (auto& x : data)
			{
				list.push_back(x.first);
			}

			return list;
		}

		[[nodiscard]] std::vector<std::string> GetAllLoggedEvents() noexcept
		{
			std::vector<std::string> list;

			for (auto& x : data)
			{
				auto raw = DKUtil::string::split(x.second, "|");
				for (auto& r : raw) {
					if (!r.empty()) {
						list.push_back(fmt::format("{} {}", x.first, r));
					}
				}
			}
			return list;
		}

		[[nodiscard]] std::vector<std::string> GetAllLoggedEvents(std::string_view a_date, bool b_prefix, bool b_colour, std::string_view qc, std::string_view ic, std::string_view bc, std::string_view sc) noexcept
		{
			if (!HasDate(a_date)) {
				return {};
			}

			std::vector<std::string> list;
			auto raw = DKUtil::string::split(data[a_date.data()], "|");
			for (auto& r : raw) {
				if (!r.empty()) 
				{
					std::string x{};
					auto colourprefix = !b_colour ? "" : GetEntryTypeByLogPrefx(r, qc, ic, bc, sc);

					x = b_prefix ? fmt::format("{}", r) : fmt::format("{}", r.substr(r.find_first_of("~ ") + 2));

					if (b_colour)
					{
						x = fmt::format("<font color='{}'>{}{}", colourprefix.data(), x, "</font>");
					}

					list.push_back(x);
				}
			}
			return list;
		}

		//---------------------------------------------------
		//-- Completionist Serialization ( SKSE APIs ) ------
		//---------------------------------------------------

		virtual void Save(SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept
		{
			std::size_t total = data.size();
			if (!a_intfc->WriteRecordData(&total, sizeof(total))) {
				ERROR("Failed to write serialized form data: size");
			}

			auto written = 0;
			for (auto& [key, val] : data) {
				std::size_t keySize{ key.size() }, valSize{ val.size() };
				if (!a_intfc->WriteRecordData(&keySize, sizeof(keySize)) ||
					!a_intfc->WriteRecordData(&valSize, sizeof(valSize))) {
					ERROR("Failed to write serialized form data: pair_size");
				}

				if (!a_intfc->WriteRecordData(key.data(), keySize) ||
					!a_intfc->WriteRecordData(val.data(), valSize)) {
					ERROR("Failed to write serialized form data: pair_data");
				}

				written++;
			}

			if (written != total) {
				ERROR("Lost data during saving co-save!\nExpected: {}\nWritten: {}", total, written);
			}

			INFO("Saved {} to co-save with a size of - {}", a_name, total);
		}

		virtual void Load(SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept override
		{
			std::size_t total;
			if (!a_intfc->ReadRecordData(&total, sizeof(total))) {
				ERROR("Failed to read serialized form data: size");
			}

			auto read = 0;
			for (auto i = 0; i < total; ++i) {
				static std::string key, val;
				std::size_t keySize, valSize{};
				if (!a_intfc->ReadRecordData(&keySize, sizeof(keySize)) ||
					!a_intfc->ReadRecordData(&valSize, sizeof(valSize))) {
					ERROR("Failed to read serialized form data: pair_size");
				}

				key.resize(keySize);
				val.resize(valSize);
				if (!a_intfc->ReadRecordData(key.data(), keySize) ||
					(valSize && !a_intfc->ReadRecordData(val.data(), valSize))) {
					ERROR("Failed to read serialized form data: pair_data");
				}

				data.try_emplace(key, val);
				read++;
			}

			if (read != total) {
				INFO("Lost data while loading {} from co-save... Expected: {} Written: {}", a_name, total, read);
			}

			INFO("Loaded SKSE co-save {} with a size of - {}", a_name, data.size());
		}

		virtual void Revert([[maybe_unused]] SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept
		{
			INFO("Reverting {} from co-save", a_name);
			data.clear();
		}

		//---------------------------------------------------
		//-- Completionist Serialization ( Members ) --------
		//---------------------------------------------------

		std::unordered_map<std::string, std::string> data;
	};

	// new polymorphed data structure designed for Logging how many times quests have been completed
	struct CompletionistRadiantCounter final : public ISerializable
	{
		void AddKey(std::string_view a_key) noexcept
		{
			if (!HasKey(a_key)) {
				data.try_emplace(a_key.data());
			}
		}

		// accessor
		[[nodiscard]] bool HasKey(std::string_view a_key) const noexcept
		{
			return data.contains(a_key.data());
		}

		void RemoveKey(std::string_view a_key) noexcept
		{
			if (HasKey(a_key)) {
				data.erase(a_key.data());
			}
		}

		void IncreaseCount(std::string_view a_key, int32_t value) noexcept
		{
			if (!HasKey(a_key)) {
				AddKey(a_key);
			}

			data[a_key.data()]++;
			INFO("Radiant Quest Handler Incremented key {} to a value of: {}", a_key, data[a_key.data()]);
		}

		void SetCount(std::string_view a_key, int32_t value) noexcept
		{
			if (!HasKey(a_key)) {
				AddKey(a_key);
			}

			data[a_key.data()] = value;
			INFO("Radiant Quest Handler set key {} to a value of: {}", a_key, data[a_key.data()]);
		}

		void DecreaseCount(std::string_view a_key, int32_t value) noexcept
		{
			if (!HasKey(a_key)) {
				return;
			}

			if (data[a_key.data()] == 0) {
				return;
			}

			data[a_key.data()]--;
			INFO("Radiant Quest Handler Decreased key {} to a value of: {}", a_key, data[a_key.data()]);
		}

		void ResetCount(std::string_view a_key) noexcept
		{
			if (!HasKey(a_key)) {
				return;
			}

			data[a_key.data()] = 0;
			INFO("Radiant Quest Handler reset key {} to a value of: {}", a_key, data[a_key.data()]);
		}

		// accessor
		[[nodiscard]] int32_t GetCount(std::string_view a_key) noexcept
		{
			return HasKey(a_key.data()) ? data[a_key.data()] : 0;
		}

		//---------------------------------------------------
		//-- Completionist Serialization ( SKSE APIs ) ------
		//---------------------------------------------------

		virtual void Save(SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept
		{
			std::size_t total = data.size();
			if (!a_intfc->WriteRecordData(&total, sizeof(total))) {
				ERROR("Failed to write serialized form data: size");
			}

			auto written = 0;
			for (auto& [key, val] : data) {
				std::size_t keySize{ key.size() };

				if (!a_intfc->WriteRecordData(&keySize, sizeof(keySize))) {
					ERROR("Failed to write serialized data: keysize");
				}

				if (!a_intfc->WriteRecordData(key.data(), keySize) || !a_intfc->WriteRecordData(val)) {
					ERROR("Failed to write serialized form data: pair_data");
				}

				written++;
			}

			if (written != total) {
				ERROR("Lost data during saving co-save!\nExpected: {}\nWritten: {}", total, written);
			}

			INFO("Saved {} to co-save with a size of - {}", a_name, total);
		}

		virtual void Load(SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept override
		{
			std::size_t total;
			if (!a_intfc->ReadRecordData(&total, sizeof(total))) {
				ERROR("Failed to read serialized form data: size");
			}

			auto read = 0;
			for (auto i = 0; i < total; ++i) {
				static std::string key;
				static int32_t val;

				std::size_t keySize;
				if (!a_intfc->ReadRecordData(&keySize, sizeof(keySize))) {
					ERROR("Failed to read serialized form data: keysize");
				}

				key.resize(keySize);
				if (!a_intfc->ReadRecordData(key.data(), keySize) || (!a_intfc->ReadRecordData(val))) {
					ERROR("Failed to read serialized form data: pair_data");
				}

				data.try_emplace(key, val);
				read++;
			}

			if (read != total) {
				INFO("Lost data while loading {} from co-save... Expected: {} Written: {}", a_name, total, read);
			}

			INFO("Loaded SKSE co-save {} with a size of - {}", a_name, data.size());
		}

		virtual void Revert([[maybe_unused]] SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept
		{
			INFO("Reverting {} from co-save", a_name);
			data.clear();
		}

		//---------------------------------------------------
		//-- Completionist Serialization ( Members ) --------
		//---------------------------------------------------

		std::unordered_map<std::string, int32_t> data;
	};

	// new polymorphed data structure designed for holding excluded object references from the cell scanner.
	struct CompletionistExcludedReferences final : public ISerializable
	{

	void AddReference(RE::FormID a_formID, std::string_view a_name, RE::FormID a_cell) noexcept
	{
		if (!HasReference(a_formID)) {
			data.try_emplace(a_formID, std::make_pair(a_name.data(), a_cell));
		}
	}

	void AddReference(RE::TESObjectREFR* a_reference, std::string_view a_name, RE::FormID a_cell) noexcept
	{
		if (!HasReference(a_reference->GetFormID())) {
			data.try_emplace(a_reference->GetFormID(), std::make_pair(fmt::format("{:s}{:s}"s, a_name.data(), "."), a_cell));
			INFO("Excluded Chest For: {}", a_name.data());
		}
	}

	// accessor
	[[nodiscard]] bool HasReference(RE::FormID a_formID) const noexcept
	{
		return data.contains(a_formID);
	}

	void RemoveReference(RE::FormID a_formID) noexcept
	{
		if (HasReference(a_formID)) {
			data.erase(a_formID);
		}
	}

	[[nodiscard]] RE::TESObjectREFR* GetReference(RE::FormID a_formID) noexcept
	{
		return static_cast<RE::TESObjectREFR*>(RE::TESObjectREFR::LookupByID(a_formID));
	}

	// accessor
	[[nodiscard]] bool IsSameCell(RE::FormID a_loc1, RE::FormID a_loc2) noexcept
	{
		return a_loc1 == a_loc2;
	}

	// accessor
	[[nodiscard]] bool HasActorInSameCellAsForm(RE::TESObjectREFR* a_actor) noexcept
	{	
		const auto* cell = RE::PlayerCharacter::GetSingleton()->GetParentCell();
		if (!cell) {
			return false;
		}
		
		const auto& rtd = cell->GetRuntimeData();
		for (auto& ref : rtd.references) 
		{
			if (ref && ref.get() && ref.get()->GetFormID() == a_actor->GetFormID()) 
			{
				return true;
			}
		}  

		return false;
	}

	[[nodiscard]] bool ShouldInclude(RE::FormID a_formID, RE::FormID a_loc1, RE::FormID a_loc2) noexcept
	{
		auto* refr = GetReference(a_formID);
		if (!refr) { return false; }

		return refr->As<RE::Actor>() ? HasActorInSameCellAsForm(refr) : IsSameCell(a_loc1, a_loc2);
	}

	std::vector<std::string> GetReferenceFormIDs(RE::FormID a_cell)
	{
		std::vector<std::string> list{};

		for (auto& [formID, pair] : data)
		{
			if (ShouldInclude(formID, a_cell, pair.second))
			{
				list.push_back(std::format("{:08X}", formID));
			}
		}
		return list;
	}

	std::vector<std::string> GetReferenceNames(RE::FormID a_cell)
	{
		std::vector<std::string> list{};

		for (auto& [formID, pair] : data)
		{
			if (ShouldInclude(formID, a_cell, pair.second))
			{
				list.push_back(pair.first);
			}
		}
		return list;
	}

	std::vector<RE::TESObjectREFR*> GetReferenceForms(RE::FormID a_cell)
	{
		std::vector<RE::TESObjectREFR*> list{};

		for (auto& [formID, pair] : data)
		{
			if (ShouldInclude(formID, a_cell, pair.second))
			{
				auto* ref = static_cast<RE::TESObjectREFR*>(RE::TESObjectREFR::LookupByID(formID));

				if (ref)
				{
					list.push_back(ref);
				}
			}
		}
		return list;
	}

	//---------------------------------------------------
	//-- Completionist Serialization ( SKSE APIs ) ------
	//---------------------------------------------------

	virtual void Save(SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept
	{
		std::size_t total = data.size();
		if (!a_intfc->WriteRecordData(&total, sizeof(total))) {
			ERROR("Failed to write serialized form data: size");
		}

		auto written = 0;
		for (auto& [formID, pair] : data) {

			std::size_t refNameSize{ pair.first.size() }, locationSize{ sizeof(pair.second) };
			if (!a_intfc->WriteRecordData(&refNameSize, sizeof(refNameSize)) || !a_intfc->WriteRecordData(&locationSize, sizeof(locationSize))) {
				ERROR("Failed to write serialized form data: pair_size");
			}

			if (!a_intfc->WriteRecordData(pair.first.data(), refNameSize) || !a_intfc->WriteRecordData(&pair.second, locationSize)) {
				ERROR("Failed to write serialized form data: pair_data");
			}

			if (!a_intfc->WriteRecordData(&formID, sizeof(formID))) {
				ERROR("Failed to write serialized form data: formID");
			}

			written++;
		}

		if (written != total) {
			ERROR("Lost data during saving co-save!\nExpected: {}\nWritten: {}", total, written);
		}

		INFO("Saved {} to co-save with a size of - {}", a_name, total);
	}

	virtual void Load(SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept override
	{
		std::size_t total;
		if (!a_intfc->ReadRecordData(&total, sizeof(total))) {
			ERROR("Failed to read serialized form data: size");
		}

		auto read = 0;
		for (auto i = 0; i < total; ++i)
		{
			static RE::FormID formID;
			static RE::FormID cellID;
			static std::string refName;
			std::size_t refNameSize, locationSize;
			if (!a_intfc->ReadRecordData(&refNameSize, sizeof(refNameSize)) || !a_intfc->ReadRecordData(&locationSize, sizeof(locationSize))) {
				ERROR("Failed to read serialized form data: pair_size");
			}

			refName.resize(refNameSize);
			if (!a_intfc->ReadRecordData(refName.data(), refNameSize) || (!a_intfc->ReadRecordData(&cellID, locationSize))) {
				ERROR("Failed to read serialized form data: pair_data");
			}

			if (!a_intfc->ReadRecordData(&formID, sizeof(formID))) {
				ERROR("Failed to read serialized form data: formID");
			}

			if (!formID || !a_intfc->ResolveFormID(formID, formID)) {
				INFO("Failed to resolve formID {}", std::format("{:08X}", formID));
				continue;
			}

			if (!cellID || !a_intfc->ResolveFormID(cellID, cellID)) {
				INFO("Failed to resolve formID {}", std::format("{:08X}", cellID));
				continue;
			}

			data.try_emplace(formID, std::make_pair(refName, cellID));
			read++;
		}

		if (read != total) {
			INFO("Lost data while loading {} from co-save... Expected: {} Written: {}", a_name, total, read);
		}

		INFO("Loaded {} from co-save with a size of - {}", a_name, data.size());
	}

	virtual void Revert([[maybe_unused]] SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept
	{
		INFO("Reverting {} from co-save", a_name);
		data.clear();
	}

	//---------------------------------------------------
	//-- Completionist Serialization ( Members ) --------
	//---------------------------------------------------

	std::map<RE::FormID, std::pair<std::string, RE::FormID>> data;
	};
};

namespace CFramework_Master
{
	extern Serialization::CompletionistData FoundItemData;
	extern Serialization::CompletionistData ExcludedCellData;
	extern Serialization::CompletionistData FoundItemData_NoShow;
	extern Serialization::CompletionistKey CQuestKeys_Natural;
	extern Serialization::CompletionistKey CQuestKeys_Manual;
	extern Serialization::CompletionistKey CQuestKeys_Stages;
	extern Serialization::CompletionistLog LoggingData;
};

namespace Serialization
{
	//---------------------------------------------------
	//-- SKSE Callback Functions ( Save Callback ) ------
	//---------------------------------------------------

	static void SaveCallback([[maybe_unused]] SKSE::SerializationInterface* a_intfc) noexcept
	{
		if (!a_intfc->OpenRecord(kHeader, kVersion)) {
			ERROR("Failed to open record");
		}

		for (auto& [data, name] : ISerializable::ManagedData) {
			if (data) {
				data->Save(a_intfc, name);
			}
		}
	}

	//---------------------------------------------------
	//-- SKSE Callback Functions ( Load Callback ) ------
	//---------------------------------------------------

	static void LoadCallback([[maybe_unused]] SKSE::SerializationInterface* a_intfc) noexcept
	{
		using namespace CFramework_Master;

		std::uint32_t type, version, length;
		while (a_intfc->GetNextRecordInfo(type, version, length)) {
			if (type != kHeader) {
				break;
			}

			if (version > kVersion) {
				ERROR("New version of save file detected, possbily using an outdated plugin?");
			}

			for (auto& [data, name] : ISerializable::ManagedData) {

				if (data)
				{
					/*Add new serialised data sets here for first load... e.g if (version < kVersion && name == "EXAMPLE DATA NAME") { continue; }*/
					if (version < 1006 && DKUtil::string::iequals(name, "LoggingData")) { 
						INFO("Skipping loading of Logging Data.");
						continue; 
					}

					if (version < 1008 && DKUtil::string::iequals(name, "RadiantCountData")) {
						INFO("Skipping loading of Radiant Count Data.");
						continue;
					}

					if (version < 1011 && DKUtil::string::iequals(name, "ExcludedCellScannerRefs")) {
						INFO("Skipping loading of Excluded Cell Scanner Refs.");
						continue;
					}

					if (version < 1011 && DKUtil::string::iequals(name, "ExcludedMerchantContainers")) {
						INFO("Skipping loading of Excluded Merchant Containers.");
						continue;
					}

					if (version < 1013 && DKUtil::string::iequals(name, "PatchSettings")) {
						INFO("Skipping loading of Patch Page Settings.");
						continue;
					}

					if (version < 1016 && DKUtil::string::iequals(name, "PlayerHits")) {
						INFO("Skipping loading of Player hits.");
						continue;
					}

					if (version < 1017 && DKUtil::string::iequals(name, "PlayerKills")) {
						INFO("Skipping loading of Player Kills.");
						continue;
					}

					data->Load(a_intfc, name);

					if (version < 1007 && DKUtil::string::iequals(name, "LoggingData"))
					{
						int i = 0;
						int x = LoggingData.data.size();

						while (i < x) {
							std::unordered_map<const std::string, std::string>::iterator it = LoggingData.data.begin();

							// Iterating over the map using Iterator till map end.
							while (it != LoggingData.data.end()) {
								if (it->first.find("/08/2023") != std::string::npos) {
									INFO("Correcting Completion Log Date: {}", it->first);

									std::string newNode = it->first;
									newNode = DKUtil::string::replace_nth_occurrence(newNode, 0, "/08/2023", "/09/2023");

									auto node = LoggingData.data.extract(it->first);
									if (node) {
										node.key() = newNode;
										LoggingData.data.insert(std::move(node));
									}
								}

								INFO("Correcting Invalid Formats on event: {}", it->first);
								it->second = DKUtil::string::replace_nth_occurrence(it->second, 0, ":  ", ": ");
								it++;
							}
							i++;
						}
					}

					if (version < 1018 && DKUtil::string::iequals(name, "LoggingData"))
					{
						int i = 0;
						int x = LoggingData.data.size();

						while (i < x) {
							std::unordered_map<const std::string, std::string>::iterator it = LoggingData.data.begin();

							// Iterating over the map using Iterator till map end.
							while (it != LoggingData.data.end()) {
								if (it->first.find("/010/2024") != std::string::npos) {
									INFO("Correcting Completion Log Date: {}", it->first);

									std::string newNode = it->first;
									newNode = DKUtil::string::replace_nth_occurrence(newNode, 0, "/010/2024", "/10/2024");

									auto node = LoggingData.data.extract(it->first);
									if (node) {
										node.key() = newNode;
										LoggingData.data.insert(std::move(node));
									}
								}
								if (it->first.find("/011/2024") != std::string::npos) {
									INFO("Correcting Completion Log Date: {}", it->first);

									std::string newNode = it->first;
									newNode = DKUtil::string::replace_nth_occurrence(newNode, 0, "/011/2024", "/11/2024");

									auto node = LoggingData.data.extract(it->first);
									if (node) {
										node.key() = newNode;
										LoggingData.data.insert(std::move(node));
									}
								}
								it++;
							}
							i++;
						}
					}
				}
			}

			if (version < kVersion) {
				/*if somehow the serialized data structure changed, thus resulting a kVersion change
				  it must be able to patch/regress the backward compatibility or it will ERROR*/
			}
		}
	}

	//---------------------------------------------------
	//-- SKSE Callback Functions ( Clear Callback ) -----
	//---------------------------------------------------

	static void RevertCallback([[maybe_unused]] SKSE::SerializationInterface* a_intfc) noexcept
	{
		for (auto& [data, name] : ISerializable::ManagedData) {
			if (data) {
				data->Revert(a_intfc, name);
			}
		}
	}
}