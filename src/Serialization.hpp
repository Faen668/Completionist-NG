#include "DKUtil/Utility.hpp"
#include "Internal Utility/Variables.hpp"
#pragma once

#undef GetForm
#undef AddForm

namespace Serialization
{
	class tme
	{
	public:
		tme(std::string& a)
			:day{ a.substr(0,3),a.substr(8,2) }, month{ a.substr(4,3) }, year{ a.substr(20,4) }
		{
			tie = a.substr(11, 8);
		}

		std::string day[2]{}; std::string month{}; std::string year{};
		std::string tie{};
	};

	enum : std::uint32_t
	{
		kHeader = 'COMP',
		kVersion = 1004,
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
			for (auto& file : RE::TESDataHandler::GetSingleton()->compiledFileCollection.files)
			{
				if (file && DKUtil::string::iequals(file->fileName, a_modname.data())) {
					return true;
				}	
			}

			for (auto& file : RE::TESDataHandler::GetSingleton()->compiledFileCollection.smallFiles)
			{
				if (file && DKUtil::string::iequals(file->fileName, a_modname.data())) {
					return true;
				}
			}

			return false;
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
			if (!IsModInstalled(a_bfilename) || !RE::TESDataHandler::GetSingleton()) {
				INFO("Unable to Install {} from {} as {} is not installed.", a_vari, a_mfilename, a_bfilename);
				return;
			}

			if (!IsModInstalled(a_mfilename) || !RE::TESDataHandler::GetSingleton()) {
				INFO("Unable to Install {} from {} as {} is not installed.", a_vari, a_mfilename, a_mfilename);
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
		// modifier
		void AddDate(std::string_view a_date) noexcept
		{
			if (!data.contains(a_date.data())) {
				data.try_emplace(a_date.data());
			}
		}

		void AddLoggedEvent(std::string_view a_log) noexcept
		{
			auto date = GetCurrentDate();

			if (HasLoggedEvent(date, a_log)) {
				return;
			}

			AddDate(GetCurrentDate());

			auto event = fmt::format("|{}|", a_log);
			data[date.data()] += event;

			INFO("Log Recorder Added Log [{}] to '{}' Serialized Date Map.", a_log, date);
		}

		void RemoveDate(std::string_view a_date) noexcept
		{
			if (HasDate(a_date)) {
				data.erase(a_date.data());
			}
		}

		void RemoveLoggedEvent(std::string_view a_date, std::string_view a_log) noexcept
		{
			if (HasDate(a_date)) {
				auto event = fmt::format("|{}|", a_log);
				DKUtil::string::replace_all(data[a_date.data()], event, {});
			}
		}

		// accessor
		[[nodiscard]] std::string GetCurrentDate() noexcept
		{
			auto rp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			std::string h(ctime(&rp));
			tme curtime(h);

			return fmt::format("{:s} {:s} {:s}", curtime.month, curtime.day[1], curtime.year);
		}

		[[nodiscard]] bool HasDate(std::string_view a_date) noexcept
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

		[[nodiscard]] std::vector<std::string> GetAllLoggedEvents(std::string_view a_date) noexcept
		{
			if (!HasDate(a_date)) {
				return {};
			}

			std::vector<std::string> list;
			auto raw = DKUtil::string::split(data[a_date.data()], "|");
			for (auto& r : raw) {
				if (!r.empty()) {
					list.push_back(fmt::format("{} {}", a_date, r));
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
}

namespace CFramework_Master
{
	extern Serialization::CompletionistData FoundItemData;
	extern Serialization::CompletionistData ExcludedCellData;
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
				if (data)
				{
					/*Add new serialised data sets here for first load... e.g if (version < kVersion && name == "EXAMPLE DATA NAME") { continue; }*/
					if (version < 1004 && DKUtil::string::iequals(name, "LoggingData")) { continue; }
					
					data->Load(a_intfc, name);
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