#include "Internal Utility/WordsOfPower.hpp"
#include "Internal Utility/PluginScan.hpp"

#include <fstream>

namespace WordsOfPower
{
	namespace
	{
		std::mutex                                           lock;
		std::unordered_map<RE::FormID, std::vector<Source>> sources;
		std::atomic<bool>                                    started{ false };
		std::atomic<bool>                                    finished{ false };

		template <class T>

		//---------------------------------------------------
		//-- Words Of Power ( Lookup ) ----------------------
		//---------------------------------------------------

		T* Lookup(const std::string& a_key) {

			std::string   plugin;
			std::uint32_t localID = 0;
			if (a_key.empty() || !PluginScan::Scanner::SplitKey(a_key, plugin, localID)) {
				return nullptr;
			}
			auto* handler = RE::TESDataHandler::GetSingleton();
			auto* form = handler ? handler->LookupForm(localID, plugin) : nullptr;
			return form ? form->As<T>() : nullptr;
		}

		//---------------------------------------------------
		//-- Words Of Power ( Name Or ) ---------------------
		//---------------------------------------------------

		std::string NameOr(const RE::TESForm* a_form, std::string_view a_fallback) {

			const char* name = a_form ? a_form->GetName() : nullptr;
			return name && *name ? std::string(name) : std::string(a_fallback);
		}

		//---------------------------------------------------
		//-- Words Of Power ( Hold Of ) ---------------------
		//---------------------------------------------------

		const RE::BGSLocation* HoldOf(const RE::BGSLocation* a_location) {

			for (auto* location = a_location ? a_location->parentLoc : nullptr; location; location = location->parentLoc) {
				if (location->HasKeywordString("LocTypeHold")) {
					return location;
				}
			}
			return nullptr;
		}

		//---------------------------------------------------
		//-- Words Of Power ( Place Of ) --------------------
		//---------------------------------------------------

		// No place for storage and test cells
		Source PlaceOf(const PluginScan::Result& a_result, const PluginScan::WordSource& a_source) {

			const auto cell = a_result.cells.find(a_source.cell);
			if (cell == a_result.cells.end()) {
				return {};
			}
			const auto& info = cell->second;
			if (PluginScan::Scanner::IsDeveloperCell(info)) {
				return {};
			}

			auto locationKey = !a_source.location.empty() ? a_source.location : info.location;
			if (locationKey.empty()) {
				locationKey = PluginScan::Scanner::NamedLocation(a_result, info);
			}

			Source where;
			if (const auto* location = Lookup<RE::BGSLocation>(locationKey)) {
				where.place = NameOr(location, info.editorID);
				where.location = location->GetFormID();
				if (const auto* hold = HoldOf(location)) {
					where.region = NameOr(hold, {});
				}
			} else if (!info.interior) {
				const auto world = a_result.worldspaces.find(info.worldspace);
				where.place = NameOr(Lookup<RE::TESWorldSpace>(info.worldspace), world != a_result.worldspaces.end() ? world->second : info.worldspace);
			}
			return where;
		}

		//---------------------------------------------------
		//-- Words Of Power ( Scan ) ------------------------
		//---------------------------------------------------

		void Scan() {

			const auto start = std::chrono::steady_clock::now();
			auto*      handler = RE::TESDataHandler::GetSingleton();
			if (!handler) {
				return;
			}

			// Full plugins in load order, then light ones
			std::vector<const RE::TESFile*> files;
			for (std::size_t i = 0; i < handler->GetLoadedModCount(); ++i) {
				files.push_back(handler->GetLoadedMods()[i]);
			}
			for (std::size_t i = 0; i < handler->GetLoadedLightModCount(); ++i) {
				files.push_back(handler->GetLoadedLightMods()[i]);
			}

			PluginScan::Scanner scanner;
			std::size_t         plugins = 0;
			for (const auto* file : files) {
				if (!file) {
					continue;
				}
				const std::string name(file->GetFilename());
				std::ifstream     stream("Data/" + name, std::ios::binary | std::ios::ate);
				if (!stream) {
					INFO("Words of power: couldn't open {}", name);
					continue;
				}
				std::vector<std::byte> data(static_cast<std::size_t>(stream.tellg()));
				stream.seekg(0);
				stream.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(data.size()));
				scanner.AddFile(name, data);
				++plugins;
			}
			const auto result = scanner.Finish();
			for (const auto& error : result.errors) {
				INFO("Words of power: {}", error);
			}

			std::unordered_map<RE::TESWordOfPower*, std::pair<RE::TESShout*, int>> shoutOf;
			for (auto* shout : handler->GetFormArray<RE::TESShout>()) {
				for (int i = 0; shout && i < 3; ++i) {
					if (auto* word = shout->variations[i].word) {
						shoutOf.try_emplace(word, shout, i + 1);
					}
				}
			}

			std::unordered_map<RE::FormID, std::vector<Source>> found;
			for (const auto& [key, editorID] : result.words) {
				auto* word = Lookup<RE::TESWordOfPower>(key);
				if (!word) {
					continue;
				}
				const auto shout = shoutOf.find(word);
				if (shout == shoutOf.end()) {
					continue;	// test and creature words
				}

				auto&      list = found[word->GetFormID()];
				const auto it = result.sources.find(key);
				for (const auto& source : it != result.sources.end() ? it->second : std::vector<PluginScan::WordSource>{}) {
					bool   keep = false;
					Source entry;
					if (source.kind == PluginScan::WordSource::Kind::kReference) {
						entry = PlaceOf(result, source);
						keep = !entry.place.empty();
					} else if (source.kind == PluginScan::WordSource::Kind::kQuest) {
						entry.place = NameOr(Lookup<RE::TESQuest>(source.record), {});
						entry.quest = true;
						keep = true;
					}
					const auto same = [&](const Source& a_other) { return a_other.place == entry.place && a_other.region == entry.region && a_other.quest == entry.quest; };
					if (keep && std::ranges::none_of(list, same)) {
						list.push_back(std::move(entry));
					}
				}

				std::ranges::stable_partition(list, [](const Source& a_source) { return !a_source.quest; });
			}

			const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
			INFO("Words of power: scanned {} plugins ({} records) in {} ms; {} shout words, {} with a place or quest", plugins, result.records, ms, found.size(),
				std::ranges::count_if(found, [](const auto& a_entry) { return !a_entry.second.empty(); }));
			if (result.compressedSkipped > 0) {
				INFO("Words of power: {} compressed records couldn't be read", result.compressedSkipped);
			}

			{
				std::scoped_lock guard(lock);
				sources = std::move(found);
			}
			finished = true;
		}
	}

	//---------------------------------------------------
	//-- Words Of Power ( Start Scan ) ------------------
	//---------------------------------------------------

	void StartScan() {

		if (started.exchange(true)) {
			return;
		}
		std::thread([] {
			try {
				Scan();
			} catch (const std::exception& e) {
				INFO("Words of power: scan failed: {}", e.what());
			}
		}).detach();
	}

	//---------------------------------------------------
	//-- Words Of Power ( Ready ) -----------------------
	//---------------------------------------------------

	bool Ready() noexcept {

		return finished.load();
	}

	//---------------------------------------------------
	//-- Words Of Power ( Sources For ) -----------------
	//---------------------------------------------------

	std::vector<Source> SourcesFor(RE::FormID a_word) {

		std::scoped_lock guard(lock);
		const auto       it = sources.find(a_word);
		return it != sources.end() ? it->second : std::vector<Source>{};
	}
}
