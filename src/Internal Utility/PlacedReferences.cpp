#include "Internal Utility/PlacedReferences.hpp"
#include "Internal Utility/PluginScan.hpp"

#include <chrono>
#include <cstdio>
#include <fstream>
#include <memory>
#include <set>
#include <unordered_map>

namespace PlacedReferences
{
	namespace
	{
		// A blank reference of our own: the game never sees it, so it is never saved or loaded,
		// and only its form ID, form type and base object are filled in
		struct alignas(RE::TESObjectREFR) StandIn
		{
			std::byte bytes[sizeof(RE::TESObjectREFR)]{};

			// Where the plugin places it
			RE::FormID   cell{ 0 };
			RE::FormID   location{ 0 };	// the cell's, as the plugins have it
			RE::FormID   worldspace{ 0 };
			RE::NiPoint3 position;

			mutable RE::ObjectRefHandle anchor;	// what it's found by outside, until its cell loads
		};

		std::unordered_map<RE::FormID, std::unique_ptr<StandIn>> standIns;

		//---------------------------------------------------
		//-- Placed References ( Owner Of ) -----------------
		//---------------------------------------------------

		// The file a form ID belongs to, and its ID inside that file
		const RE::TESFile* OwnerOf(RE::FormID a_formID, std::uint32_t& a_localID) {

			auto* handler = RE::TESDataHandler::GetSingleton();
			if (!handler) {
				return nullptr;
			}
			const auto index = a_formID >> 24;
			if (index == 0xFE && !REL::Module::IsVR()) {
				const auto light = (a_formID >> 12) & 0xFFF;
				a_localID = a_formID & 0xFFF;
				for (std::size_t i = 0; i < handler->GetLoadedLightModCount(); ++i) {
					const auto* file = handler->GetLoadedLightMods()[i];
					if (file && file->smallFileCompileIndex == light) {
						return file;
					}
				}
				return nullptr;
			}
			a_localID = a_formID & 0xFFFFFF;
			for (std::size_t i = 0; i < handler->GetLoadedModCount(); ++i) {
				const auto* file = handler->GetLoadedMods()[i];
				if (file && file->compileIndex == index) {
					return file;
				}
			}
			return nullptr;
		}

		//---------------------------------------------------
		//-- Placed References ( Key Of ) -------------------
		//---------------------------------------------------

		// As the plugin scanner names records: file#localid, lower case
		std::string KeyOf(const RE::TESFile* a_file, std::uint32_t a_localID) {

			std::string name(a_file->GetFilename());
			std::ranges::transform(name, name.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
			char local[16];
			std::snprintf(local, sizeof(local), "#%06x", a_localID);
			return name + local;
		}

		//---------------------------------------------------
		//-- Placed References ( Uses ) ---------------------
		//---------------------------------------------------

		bool Uses(const RE::TESFile* a_file, const RE::TESFile* a_master) {

			if (a_file == a_master) {
				return true;
			}
			for (std::uint32_t i = 0; i < a_file->masterCount; ++i) {
				if (a_file->masterPtrs && a_file->masterPtrs[i] == a_master) {
					return true;
				}
			}
			return false;
		}

		//---------------------------------------------------
		//-- Placed References ( Form Of Key ) --------------
		//---------------------------------------------------

		RE::FormID FormOfKey(const std::string& a_key) {

			std::string   plugin;
			std::uint32_t localID = 0;
			auto*         handler = RE::TESDataHandler::GetSingleton();
			return handler && !a_key.empty() && PluginScan::Scanner::SplitKey(a_key, plugin, localID) ? handler->LookupFormID(localID, plugin) : 0;
		}

		//---------------------------------------------------
		//-- Placed References ( Marked Location ) ----------
		//---------------------------------------------------

		// The place an interior belongs to, or the place around that, that has a marker
		const RE::BGSLocation* MarkedLocation(const StandIn& a_standIn) {

			for (const auto* location = RE::TESForm::LookupByID<RE::BGSLocation>(a_standIn.location); location; location = location->parentLoc) {
				if (location->worldLocMarker) {
					return location;
				}
			}
			return nullptr;
		}

		//---------------------------------------------------
		//-- Placed References ( Nearest Door ) -------------
		//---------------------------------------------------

		// The load door in its own cell nearest it. Load doors are persistent, so the game has them
		// while the cell isn't loaded, and quest markers find their way to them through other doors
		RE::TESObjectREFR* NearestDoor(const StandIn& a_standIn) {

			const auto* cell = RE::TESForm::LookupByID<RE::TESObjectCELL>(a_standIn.cell);
			if (!cell) {
				return nullptr;
			}
			RE::TESObjectREFR* nearest = nullptr;
			float              best = FLT_MAX;
			for (const auto& ref : cell->GetRuntimeData().references) {
				if (!ref || !ref->extraList.HasType<RE::ExtraTeleport>()) {
					continue;
				}
				if (const float distance = ref->GetPosition().GetDistance(a_standIn.position); distance < best) {
					best = distance;
					nearest = ref.get();
				}
			}
			return nearest;
		}

		//---------------------------------------------------
		//-- Placed References ( Inside Target ) ------------
		//---------------------------------------------------

		// An interior's: a door in its cell, else the marker of the place it belongs to
		RE::TESObjectREFR* InsideTarget(const StandIn& a_standIn) {

			if (auto* door = NearestDoor(a_standIn)) {
				return door;
			}
			const auto* location = MarkedLocation(a_standIn);
			return location ? location->worldLocMarker.get().get() : nullptr;
		}

		//---------------------------------------------------
		//-- Placed References ( Nearest Persistent ) -------
		//---------------------------------------------------

		// Outside, the persistent reference nearest it in its worldspace: a map marker, a door, a
		// shrine. The game always has these, NPCs aside, as they move. Found once, again if it goes
		RE::TESObjectREFR* NearestPersistent(const StandIn& a_standIn) {

			const auto usable = [](const RE::TESObjectREFR* a_ref) {
				return a_ref && a_ref->GetBaseObject() && !a_ref->IsDisabled() && !a_ref->IsDeleted() && !a_ref->Is(RE::FormType::ActorCharacter);
			};
			if (auto anchor = a_standIn.anchor.get(); usable(anchor.get())) {
				return anchor.get();
			}
			const auto* world = RE::TESForm::LookupByID<RE::TESWorldSpace>(a_standIn.worldspace);
			if (!world || !world->persistentCell) {
				return nullptr;
			}
			RE::TESObjectREFR* nearest = nullptr;
			float              best = FLT_MAX;
			for (const auto& ref : world->persistentCell->GetRuntimeData().references) {
				if (!usable(ref.get())) {
					continue;
				}
				if (const float distance = ref->GetPosition().GetDistance(a_standIn.position); distance < best) {
					best = distance;
					nearest = ref.get();
				}
			}
			a_standIn.anchor = nearest ? nearest->GetHandle() : RE::ObjectRefHandle{};
			return nearest;
		}

		//---------------------------------------------------
		//-- Placed References ( Make Stand In ) ------------
		//---------------------------------------------------

		RE::TESForm* MakeStandIn(RE::FormID a_formID, RE::TESBoundObject* a_base, const PluginScan::Reference& a_place, const PluginScan::Result& a_result) {

			static REL::Relocation<std::uintptr_t> vtable{ RE::VTABLE_TESObjectREFR[0] };

			auto  storage = std::make_unique<StandIn>();
			auto* ref = reinterpret_cast<RE::TESObjectREFR*>(storage->bytes);
			*reinterpret_cast<std::uintptr_t*>(ref) = vtable.address();	// so RTTI casts work
			ref->formID = a_formID;
			ref->formType = RE::FormType::Reference;
			ref->data.objectReference = a_base;
			ref->data.location = { a_place.position[0], a_place.position[1], a_place.position[2] };
			storage->cell = FormOfKey(a_place.cell);
			storage->worldspace = FormOfKey(a_place.worldspace);
			if (const auto cell = a_result.cells.find(a_place.cell); cell != a_result.cells.end()) {
				storage->location = FormOfKey(cell->second.location);
			}
			storage->position = ref->data.location;
			standIns.emplace(a_formID, std::move(storage));
			return ref;
		}
	}

	//---------------------------------------------------
	//-- Placed References ( Resolve ) ------------------
	//---------------------------------------------------

	void Resolve(const std::vector<RE::FormID>& a_formIDs) {

		auto* handler = RE::TESDataHandler::GetSingleton();
		if (!handler) {
			return;
		}

		const auto                                  start = std::chrono::steady_clock::now();
		std::set<std::string>                       wanted;
		std::unordered_map<std::string, RE::FormID> formIDs;
		std::vector<const RE::TESFile*>             owners;
		for (const auto formID : a_formIDs) {
			if (!formID || standIns.contains(formID) || RE::TESForm::LookupByID(formID)) {
				continue;
			}
			std::uint32_t localID = 0;
			const auto*   owner = OwnerOf(formID, localID);
			if (!owner) {
				continue;
			}
			const auto key = KeyOf(owner, localID);
			wanted.insert(key);
			formIDs.emplace(key, formID);
			if (std::ranges::find(owners, owner) == owners.end()) {
				owners.push_back(owner);
			}
		}
		if (wanted.empty()) {
			return;
		}

		// The files that hold them and every file that could override them, full plugins then light ones
		std::vector<const RE::TESFile*> files;
		const auto                      consider = [&](const RE::TESFile* a_file) {
			if (a_file && std::ranges::any_of(owners, [&](const RE::TESFile* a_owner) { return Uses(a_file, a_owner); })) {
				files.push_back(a_file);
			}
		};
		for (std::size_t i = 0; i < handler->GetLoadedModCount(); ++i) {
			consider(handler->GetLoadedMods()[i]);
		}
		for (std::size_t i = 0; i < handler->GetLoadedLightModCount(); ++i) {
			consider(handler->GetLoadedLightMods()[i]);
		}

		PluginScan::Scanner scanner;
		scanner.WantReferences(wanted);
		for (const auto* file : files) {
			const std::string name(file->GetFilename());
			std::ifstream     stream("Data/" + name, std::ios::binary | std::ios::ate);
			if (!stream) {
				INFO("Placed references: couldn't open {}", name);
				continue;
			}
			std::vector<std::byte> data(static_cast<std::size_t>(stream.tellg()));
			stream.seekg(0);
			stream.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(data.size()));
			scanner.AddFile(name, data);
		}
		const auto result = scanner.Finish();
		for (const auto& error : result.errors) {
			INFO("Placed references: {}", error);
		}

		std::size_t made = 0;
		for (const auto& [key, formID] : formIDs) {
			const auto it = result.references.find(key);
			if (it == result.references.end()) {
				INFO("Placed references: {:08X} isn't a placed reference in its plugin", formID);
				continue;
			}
			if (it->second.deleted) {
				INFO("Placed references: {:08X} is deleted by {}", formID, it->second.plugin);
				continue;
			}
			auto*         base = RE::TESForm::LookupByID(FormOfKey(it->second.base));
			auto*         bound = base ? base->As<RE::TESBoundObject>() : nullptr;
			if (!bound) {
				INFO("Placed references: {:08X} has no base object the game knows ({})", formID, it->second.base);
				continue;
			}
			MakeStandIn(formID, bound, it->second, result);
			++made;
		}

		const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
		INFO("Placed references: {} of {} temporary references read from {} plugins in {} ms", made, formIDs.size(), files.size(), ms);
	}

	//---------------------------------------------------
	//-- Placed References ( Find ) ---------------------
	//---------------------------------------------------

	RE::TESForm* Find(RE::FormID a_formID) {

		const auto it = standIns.find(a_formID);
		return it != standIns.end() ? reinterpret_cast<RE::TESObjectREFR*>(it->second->bytes) : nullptr;
	}

	//---------------------------------------------------
	//-- Placed References ( Lookup ) -------------------
	//---------------------------------------------------

	RE::TESForm* Lookup(RE::FormID a_formID) {

		auto* standIn = Find(a_formID);
		return standIn ? standIn : RE::TESForm::LookupByID(a_formID);
	}

	//---------------------------------------------------
	//-- Placed References ( Place Name ) ---------------
	//---------------------------------------------------

	std::string PlaceName(RE::FormID a_formID) {

		const auto  it = standIns.find(a_formID);
		const auto* location = it != standIns.end() ? RE::TESForm::LookupByID<RE::BGSLocation>(it->second->location) : nullptr;
		return location && location->GetName() ? location->GetName() : std::string{};
	}

	//---------------------------------------------------
	//-- Placed References ( Trackable ) ----------------
	//---------------------------------------------------

	bool Trackable(RE::FormID a_formID) {

		const auto it = standIns.find(a_formID);
		if (it == standIns.end()) {
			return false;
		}
		const auto& standIn = *it->second;
		return standIn.worldspace ? RE::TESForm::LookupByID<RE::TESWorldSpace>(standIn.worldspace) != nullptr : InsideTarget(standIn) != nullptr;
	}

	//---------------------------------------------------
	//-- Placed References ( Target ) -------------------
	//---------------------------------------------------

	RE::TESObjectREFR* Target(RE::FormID a_formID) {

		if (auto* ref = RE::TESForm::LookupByID<RE::TESObjectREFR>(a_formID)) {
			return ref;
		}
		const auto it = standIns.find(a_formID);
		if (it == standIns.end()) {
			return nullptr;
		}
		const auto& standIn = *it->second;
		if (standIn.worldspace) {
			return NearestPersistent(standIn);
		}
		return InsideTarget(standIn);
	}
}
