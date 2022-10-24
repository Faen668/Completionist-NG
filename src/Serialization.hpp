#include "DKUtil/Utility.hpp"
#pragma once

#undef GetForm
#undef AddForm

namespace Serialization
{
	enum : std::uint32_t
	{
		kHeader = 'COMP',
		kVersion = 1001,
	};

#define SetSerializableInfo(DATA) (DATA).SetAsSerializable(#DATA)

#define DEFAULT_VARIATION_MAX 12
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

			auto ESP = handler->LookupLoadedModByName(a_modname);
			auto ESL = handler->LookupLoadedLightModByName(a_modname);
			return ESP || ESL;
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

		//---------------------------------------------------
		//-- Utility Functions ( Get Skill Book String ) ----
		//---------------------------------------------------

		[[nodiscard]] auto GetBookSkill(RE::ActorValue a_val) noexcept
		{
			static DKUtil::enumeration<RE::ActorValue, std::uint32_t> actorValueNameTbl{};

			auto rawName = actorValueNameTbl.value_name(a_val);

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
			if (IsModInstalled(a_bfilename) && RE::TESDataHandler::GetSingleton()) {
				return;
			}

			auto base = RE::TESDataHandler::GetSingleton()->LookupFormID(a_base, a_bfilename);
			auto vari = RE::TESDataHandler::GetSingleton()->LookupFormID(a_vari, a_mfilename);

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
			return data.contains(a_form);
		}

		[[nodiscard]] bool HasForm(RE::TESForm* a_form) const noexcept
		{
			return data.contains(a_form->GetFormID());
		}

		//---------------------------------------------------
		//-- Completionist Data Functions ( Get Form ) ------
		//---------------------------------------------------

		[[nodiscard]] RE::TESForm* GetForm(RE::FormID a_form) noexcept
		{
			return HasForm(a_form) ? RE::TESForm::LookupByID(a_form) : nullptr;
		}

		template <typename T>
		[[nodiscard]] T* GetForm(RE::FormID a_form) noexcept
		{
			auto* form = GetForm(a_form);
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

		[[nodiscard]] std::string_view GetFileName(RE::FormID a_form) noexcept
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

			auto bases = GetAllBases() |
				std::views::filter([&](auto f) { return GetForm(f) && GetForm(f)->GetName(); }) |
				std::views::transform([&](auto f) {
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

			// assertions can be removed
			assert(a_names.size() == zipped.size());
			assert(a_forms.size() == zipped.size());
			assert(a_bools.size() == zipped.size());

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

			// assertions can be removed
			assert(a_names.size() == zipped.size());
			assert(a_forms.size() == zipped.size());
			assert(a_bools.size() == zipped.size());
			assert(a_texts.size() == zipped.size());
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
				ERROR("Lost data during loading co-save!\nExpected: {}\nWritten: {}", total, read);
			}

			INFO("Loaded {} from co-save with a size of - {}", a_name, total);
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
		void AddStage(std::string_view a_key, int a_stage) noexcept
		{
			if (HasStage(a_key, a_stage)) {
				return;
			}

			auto stage = fmt::format("|{}|", a_stage);
			data[a_key.data()] += stage;
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
				DKUtil::string::replace_all(data[a_key.data()], stage, {});
			}
		}

		// accessor
		[[nodiscard]] bool HasKey(std::string_view a_key) noexcept
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

		// utility
		void DumpToLog() noexcept
		{
			for (auto& [key, val] : data) {
				INFO("Quest: {} {}", key, val);
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
				std::size_t keySize, valSize;
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
				ERROR("Lost data during loading co-save!\nExpected: {}\nWritten: {}", total, read);
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
}

namespace CFramework_Master
{
	extern Serialization::CompletionistData FoundItemData;
}

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
				if (data) {
					data->Load(a_intfc, name);
				}
			}

			if (version < kVersion) {
				/*if somehow the serialized data structure changed, thus resulting a kVersion change
				  it must be able to patch/regress the backward compatibility or it will ERROR*/

				if (version == 1000) {
					// patch notes goes here
					/*
					 * 1000 -> 1001:
					 * extra records written to FoundItemsData
					 */
					INFO("Patching old data from version 1000");

					constexpr FormArray SK_BrokenForms = {
					0x10F570,0x07C932,0x109C11,0x0B144D,0x0D2844,0x05ABC3,0x0D2842,0x05ABC4,0x05A9E3,0x05A9DF,0x05DB86,0x0FCC0E,0x0FCC0F,
					0x0FCC0C,0x05DB85,0x0FCC0D,0x05DB87,0x0FCC10,0x0FCC11,0x05DB88,0x0FCC13,0x0FCC12,0x0D3AC3,0x0D3ACC,0x0D3AC2,0x0D3ACB,
					0x0D3AC4,0x0D3ACD,0x0D3AC5,0x0D3ACE,0x0AB702,0x0F5D1A,0x0F5D1B,0x0F5D1C,0x0F5D1D,0x0D37CE,0x0F5D2A,0x0F5D2B,0x0F5D2C,
					0x0F5D2D,0x0A5DEF,0x0F5D1F,0x0F5D20,0x0F5D21,0x0F5D22,0x03B0BF,0x03B0C0,0x03B0C1,0x03B0C2,0x03B0C3,0x03B0C4,0x03B0C5,
					0x03B0C6,0x03B0C7,0x03B0B6,0x03B0B9,0x03B0BC,0x03B0B7,0x03B0BA,0x03B0BD,0x03B0B8,0x03B0BB,0x03B0BE,0x0F1AC1,0x0F71CD,
					0x0F71CE,0x0F71CF,0x0F71D0,0x07A917,0x0F6524,0x0F6525,0x0F6526,0x0F6527,0x07E5C3,0x0F6529,0x0F652A,0x0F652B,0x0F652C,
					0x0F8313,0x0F8314,0x0F8315,0x0F8316,0x0F8317,0x0F8318,0x02AC6F,0x09B2B2,0x063B27,0x063B29,0x021EA3,0x10CC6A,0x03E6BB,
					0x03E6BC,0x08D770,0x09DFF5,0x0DA74D,0x02AC60,0x0F82FE,
					};

					constexpr FormArray DG_BrokenForms = {
					0x00C816,0x00CAD3,0x0191CB,0x002B29,
					};

					constexpr FormArray DB_BrokenForms = {
					0x039FA6,0x039FAC,0x039FAD,0x0397F6,0x039FB1,0x039FB4,0x039FA1,0x039FA2,0x039FA3,0x039D2B,0x039D2E,0x039D2F,
					};

					constexpr FormArray CC_BrokenForms = {
					0x000D62,0x000D63,
					};

					constexpr FormArray Fishing_BrokenForms = {
					0x07AED6,0x0009D9,0x04D05E,
					};

					constexpr FormArray MoonAndStar_BrokenForms = {
					0x00E285,0x00E284,
					};

					constexpr FormArray Oblivion_BrokenForms = {
					0x00453D,0x00453F,0x003A02,0x003469,0x07F8B4,0x08A088,0x00EEED,0x00EEEF,0x00454C,0x02046D,0x02046A,0x020464,
					0x020471,0x022C11,0x022C12,0x040A2D,0x03BAEE,0x000D6B,
					};

					constexpr FormArray WheelsOfLull_BrokenForms = {
					0x271EB0,0x271EB1,0x271EB2,0x271EB3,0x271EB4,0x271EB5,0x271EB6,
					};

					constexpr FormArray Vigilant_BrokenForms = {
					0x23007E,0x230080,0x23007C,0x23007D,0x23007F,0x230081,0x144CDD,0x144CDE,0x0BBF35,0x0BD352,0x0D428A,0x0D568D,
					0x323B2F,0x43CBAD,0x0C3DA4,0x0B828B,0x1AABC2,0x1AABC3,0x0C68B9,0x0C68B8,
					};

					auto* handler = RE::TESDataHandler::GetSingleton();

					for (auto formID : SK_BrokenForms) {
						if (auto* form = handler->LookupForm(formID, "Skyrim.esm"); form) {
							FoundItemData.RemoveForm(form->GetFormID());
							INFO("{} {} From List", !FoundItemData.HasForm(form) ? "Successfully removed" : "Failed to remove", form->GetName());
						}
					}

					for (auto formID : DG_BrokenForms) {
						if (auto* form = handler->LookupForm(formID, "Dawnguard.esm"); form) {
							FoundItemData.RemoveForm(form->GetFormID());
							INFO("{} {} From List", !FoundItemData.HasForm(form) ? "Successfully removed" : "Failed to remove", form->GetName());
						}
					}

					for (auto formID : DB_BrokenForms) {
						if (auto* form = handler->LookupForm(formID, "Dragonborn.esm"); form) {
							FoundItemData.RemoveForm(form->GetFormID());
							INFO("{} {} From List", !FoundItemData.HasForm(form) ? "Successfully removed" : "Failed to remove", form->GetName());
						}
					}

					for (auto formID : CC_BrokenForms) {
						if (auto* form = handler->LookupForm(formID, "ccbgssse018-shadowrend.esl"); form) {
							FoundItemData.RemoveForm(form->GetFormID());
							INFO("{} {} From List", !FoundItemData.HasForm(form) ? "Successfully removed" : "Failed to remove", form->GetName());
						}
					}

					for (auto formID : Fishing_BrokenForms) {
						if (auto* form = handler->LookupForm(formID, "ccbgssse001-fish.esm"); form) {
							FoundItemData.RemoveForm(form->GetFormID());
							INFO("{} {} From List", !FoundItemData.HasForm(form) ? "Successfully removed" : "Failed to remove", form->GetName());
						}
					}

					for (auto formID : MoonAndStar_BrokenForms) {
						if (auto* form = handler->LookupForm(formID, "MoonAndStar_MAS.esp"); form) {
							FoundItemData.RemoveForm(form->GetFormID());
							INFO("{} {} From List", !FoundItemData.HasForm(form) ? "Successfully removed" : "Failed to remove", form->GetName());
						}
					}

					for (auto formID : Oblivion_BrokenForms) {
						if (auto* form = handler->LookupForm(formID, "WZOblivionArtifacts.esp"); form) {
							FoundItemData.RemoveForm(form->GetFormID());
							INFO("{} {} From List", !FoundItemData.HasForm(form) ? "Successfully removed" : "Failed to remove", form->GetName());
						}
					}

					for (auto formID : WheelsOfLull_BrokenForms) {
						if (auto* form = handler->LookupForm(formID, "WheelsOfLull.esp"); form) {
							FoundItemData.RemoveForm(form->GetFormID());
							INFO("{} {} From List", !FoundItemData.HasForm(form) ? "Successfully removed" : "Failed to remove", form->GetName());
						}
					}

					for (auto formID : Vigilant_BrokenForms) {
						if (auto* form = handler->LookupForm(formID, "Vigilant.esm"); form) {
							FoundItemData.RemoveForm(form->GetFormID());
							INFO("{} {} From List", !FoundItemData.HasForm(form) ? "Successfully removed" : "Failed to remove", form->GetName());
						}
					}
				}
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