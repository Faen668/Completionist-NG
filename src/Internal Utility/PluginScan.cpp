#include "Internal Utility/PluginScan.hpp"
#include "Internal Utility/Inflate.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdio>
#include <cstring>

// Record layout (TES4 format, Skyrim SE), as the files store it:
//   group:     "GRUP", u32 size (header included), u32 label, i32 group type, 8 bytes stamps
//   record:    char[4] type, u32 data size, u32 flags, u32 form ID, 8 bytes stamps, then the data
//   subrecord: char[4] type, u16 size, data ("XXXX" gives the next subrecord's size as a u32)
// A form ID's top byte indexes the file's master list; one past the end means the file itself.
namespace PluginScan
{
	namespace
	{
		constexpr std::uint32_t recordDeleted = 0x20;
		constexpr std::uint32_t recordCompressed = 0x40000;

		// Group types whose label is the cell their records belong to.
		constexpr std::int32_t cellChildren = 6;
		constexpr std::int32_t cellPersistent = 8;
		constexpr std::int32_t cellTemporary = 9;
		constexpr std::int32_t cellDistant = 10;
		constexpr std::int32_t worldChildren = 1;

		//---------------------------------------------------
		//-- Plugin Scan ( Lower ) --------------------------
		//---------------------------------------------------

		std::string Lower(std::string_view a_text) {

			std::string out(a_text);
			std::ranges::transform(out, out.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
			return out;
		}

		//---------------------------------------------------
		//-- Plugin Scan ( Is Placed ) ----------------------
		//---------------------------------------------------

		bool IsPlaced(std::string_view a_type) {

			constexpr std::array<std::string_view, 10> placed{ "REFR", "ACHR", "PGRE", "PMIS", "PHZD", "PARW", "PBAR", "PBEA", "PCON", "PFLA" };
			return std::ranges::find(placed, a_type) != placed.end();
		}

		//---------------------------------------------------
		//-- Plugin Scan ( Keeps Editor ID ) ----------------
		//---------------------------------------------------

		bool KeepsEditorID(std::string_view a_type) {

			constexpr std::array<std::string_view, 9> kept{ "ACTI", "BOOK", "CONT", "FLOR", "FURN", "MISC", "NPC_", "SPEL", "TACT" };
			return std::ranges::find(kept, a_type) != kept.end();
		}

		class Reader
		{
		public:
			explicit Reader(std::span<const std::byte> a_data) :
				_data(a_data)
			{}

			[[nodiscard]] bool        Ok() const noexcept { return _ok; }
			[[nodiscard]] bool        AtEnd() const noexcept { return _pos >= _data.size(); }
			[[nodiscard]] std::size_t Remaining() const noexcept { return _pos < _data.size() ? _data.size() - _pos : 0; }

			template <class T>
			T Read() {

				T value{};
				if (!_ok || Remaining() < sizeof(T)) {
					_ok = false;
					return value;
				}
				std::memcpy(&value, _data.data() + _pos, sizeof(T));
				_pos += sizeof(T);
				return value;
			}

			void Skip(std::size_t a_bytes) {

				if (!_ok || Remaining() < a_bytes) {
					_ok = false;
					return;
				}
				_pos += a_bytes;
			}

			std::string ReadWString() {

				const auto length = Read<std::uint16_t>();
				if (!_ok || Remaining() < length) {
					_ok = false;
					return {};
				}
				std::string text(reinterpret_cast<const char*>(_data.data() + _pos), length);
				_pos += length;
				return text;
			}

		private:
			std::span<const std::byte> _data;
			std::size_t                _pos{ 0 };
			bool                       _ok{ true };
		};

		//---------------------------------------------------
		//-- Plugin Scan ( Four CC ) ------------------------
		//---------------------------------------------------

		std::string FourCC(const std::byte* a_data) {

			return std::string(reinterpret_cast<const char*>(a_data), 4);
		}

		//---------------------------------------------------
		//-- Plugin Scan ( U32 ) ----------------------------
		//---------------------------------------------------

		std::uint32_t U32(const std::byte* a_data) {

			std::uint32_t value;
			std::memcpy(&value, a_data, sizeof(value));
			return value;
		}

		//---------------------------------------------------
		//-- Plugin Scan ( Z String ) -----------------------
		//---------------------------------------------------

		std::string ZString(std::span<const std::byte> a_data) {

			const auto* text = reinterpret_cast<const char*>(a_data.data());
			return std::string(text, strnlen(text, a_data.size()));
		}

		//---------------------------------------------------
		//-- Plugin Scan ( Read Object ) --------------------
		//---------------------------------------------------

		std::uint32_t ReadObject(Reader& a_reader, std::int16_t a_objectFormat) {

			if (a_objectFormat == 1) {
				const auto formID = a_reader.Read<std::uint32_t>();
				a_reader.Skip(4);  // alias, unused
				return formID;
			}
			a_reader.Skip(4);  // unused, alias
			return a_reader.Read<std::uint32_t>();
		}
	}

	//---------------------------------------------------
	//-- Plugin Scan ( Split Key ) ----------------------
	//---------------------------------------------------

	bool Scanner::SplitKey(std::string_view a_key, std::string& a_plugin, std::uint32_t& a_localID) {

		const auto hash = a_key.rfind('#');
		if (hash == std::string_view::npos) {
			return false;
		}
		a_plugin = std::string(a_key.substr(0, hash));
		a_localID = static_cast<std::uint32_t>(std::strtoul(std::string(a_key.substr(hash + 1)).c_str(), nullptr, 16));
		return true;
	}

	//---------------------------------------------------
	//-- Plugin Scan ( Is Developer Cell ) --------------
	//---------------------------------------------------

	bool Scanner::IsDeveloperCell(const Cell& a_cell) {

		if (a_cell.interior && a_cell.location.empty()) {
			return true;
		}
		const auto id = Lower(a_cell.editorID);
		return id.find("warehouse") != std::string::npos || id.starts_with("e3demo") || id.starts_with("qa") || id.starts_with("test") || id.starts_with("zz");
	}

	//---------------------------------------------------
	//-- Plugin Scan ( Named Location ) -----------------
	//---------------------------------------------------

	std::string Scanner::NamedLocation(const Result& a_result, const Cell& a_cell) {

		// Editor ID up to Exterior / Interior, trailing digits dropped
		auto       stem = Lower(a_cell.editorID);
		const auto cut = std::min(stem.find("exterior"), stem.find("interior"));
		if (cut != std::string::npos) {
			stem.erase(cut);
		}
		while (!stem.empty() && std::isdigit(static_cast<unsigned char>(stem.back()))) {
			stem.pop_back();
		}
		if (stem.size() < 4) {
			return {};
		}
		const auto wanted = stem + "location";
		for (const auto& [key, location] : a_result.locations) {
			if (Lower(location.editorID) == wanted) {
				return key;
			}
		}
		return {};
	}

	//---------------------------------------------------
	//-- Plugin Scan ( Add File ) -----------------------
	//---------------------------------------------------

	void Scanner::AddFile(std::string_view a_name, std::span<const std::byte> a_data) {

		const auto fileName = Lower(a_name);
		if (a_data.size() < 24 || FourCC(a_data.data()) != "TES4") {
			_result.errors.push_back(std::string(a_name) + ": not a plugin file");
			return;
		}

		std::vector<std::string> masters;
		const auto               headerSize = U32(a_data.data() + 4);
		if (24 + static_cast<std::size_t>(headerSize) > a_data.size()) {
			_result.errors.push_back(std::string(a_name) + ": header runs past the end of the file");
			return;
		}
		{
			Reader header(a_data.subspan(24, headerSize));
			while (header.Ok() && header.Remaining() >= 6) {
				const auto type = header.Read<std::array<char, 4>>();
				const auto size = header.Read<std::uint16_t>();
				if (std::string_view(type.data(), 4) == "MAST" && header.Remaining() >= size) {
					std::string name(size, '\0');
					for (auto& c : name) {
						c = header.Read<char>();
					}
					masters.push_back(Lower(name.c_str()));
				} else {
					header.Skip(size);
				}
			}
		}

		const auto keyFor = [&](std::uint32_t a_raw) -> std::string {
			if (a_raw == 0) {
				return {};
			}
			const auto  index = a_raw >> 24;
			const auto& owner = index < masters.size() ? masters[index] : fileName;
			char        local[16];
			std::snprintf(local, sizeof(local), "#%06x", a_raw & 0xFFFFFF);
			return owner + local;
		};

		struct Group
		{
			std::int32_t  type;
			std::uint32_t label;
		};
		std::vector<Group> groups;

		const auto enclosing = [&](std::initializer_list<std::int32_t> a_types) -> std::uint32_t {
			for (auto it = groups.rbegin(); it != groups.rend(); ++it) {
				if (std::ranges::find(a_types, it->type) != a_types.end()) {
					return it->label;
				}
			}
			return 0;
		};

		const auto readScripts = [&](std::span<const std::byte> a_vmad, bool a_quest) {
			std::vector<ObjectProperty> properties;
			Reader                      reader(a_vmad);

			const auto readScriptList = [&](std::int16_t a_version, std::int16_t a_objectFormat, const std::string& a_prefix) {
				const auto scriptCount = reader.Read<std::uint16_t>();
				for (std::uint16_t s = 0; s < scriptCount && reader.Ok(); ++s) {
					const auto script = reader.ReadWString();
					if (a_version >= 4) {
						reader.Skip(1);  // status
					}
					const auto propertyCount = reader.Read<std::uint16_t>();
					for (std::uint16_t p = 0; p < propertyCount && reader.Ok(); ++p) {
						const auto name = reader.ReadWString();
						const auto type = reader.Read<std::uint8_t>();
						if (a_version >= 4) {
							reader.Skip(1);  // status
						}
						const auto label = a_prefix + script + '.' + name;
						switch (type) {
						case 1:  // object
							if (const auto formID = ReadObject(reader, a_objectFormat)) {
								properties.push_back({ label, keyFor(formID) });
							}
							break;
						case 2:  // string
							reader.ReadWString();
							break;
						case 3:  // int
						case 4:  // float
							reader.Skip(4);
							break;
						case 5:  // bool
							reader.Skip(1);
							break;
						case 11:  // object array
							{
								const auto count = reader.Read<std::uint32_t>();
								for (std::uint32_t i = 0; i < count && reader.Ok(); ++i) {
									if (const auto formID = ReadObject(reader, a_objectFormat)) {
										properties.push_back({ label + '[' + std::to_string(i) + ']', keyFor(formID) });
									}
								}
							}
							break;
						case 12:  // string array
							{
								const auto count = reader.Read<std::uint32_t>();
								for (std::uint32_t i = 0; i < count && reader.Ok(); ++i) {
									reader.ReadWString();
								}
							}
							break;
						case 13:  // int array
						case 14:  // float array
							reader.Skip(static_cast<std::size_t>(reader.Read<std::uint32_t>()) * 4);
							break;
						case 15:  // bool array
							reader.Skip(reader.Read<std::uint32_t>());
							break;
						default:	// unknown type, stop
							reader.Skip(reader.Remaining() + 1);
							break;
						}
					}
				}
			};

			const auto version = reader.Read<std::int16_t>();
			const auto objectFormat = reader.Read<std::int16_t>();
			readScriptList(version, objectFormat, {});

			// Quests: fragments, then alias scripts
			if (a_quest && reader.Ok() && !reader.AtEnd()) {
				reader.Skip(1);  // unknown
				const auto fragmentCount = reader.Read<std::uint16_t>();
				reader.ReadWString();  // fragment script file
				for (std::uint16_t i = 0; i < fragmentCount && reader.Ok(); ++i) {
					reader.Skip(2 + 2 + 4 + 1);  // index, unknown, log entry, unknown
					reader.ReadWString();        // script
					reader.ReadWString();        // fragment
				}
				const auto aliasCount = reader.Read<std::uint16_t>();
				for (std::uint16_t i = 0; i < aliasCount && reader.Ok(); ++i) {
					reader.Skip(8);  // the alias object
					const auto aliasVersion = reader.Read<std::int16_t>();
					const auto aliasFormat = reader.Read<std::int16_t>();
					readScriptList(aliasVersion, aliasFormat, "alias:");
				}
			}
			return properties;
		};

		const auto handleRecord = [&](const std::string& a_type, std::uint32_t a_flags, std::uint32_t a_formID, std::span<const std::byte> a_data) {
			++_result.records;
			const auto key = keyFor(a_formID);

			// Compressed: u32 size then zlib, navmesh and land skipped
			std::vector<std::byte> inflated;
			if (a_flags & recordCompressed) {
				if (a_type == "NAVM" || a_type == "LAND" || a_type == "NAVI") {
					return;
				}
				if (a_data.size() < 4) {
					++_result.compressedSkipped;
					return;
				}
				inflated.reserve(U32(a_data.data()));
				if (!Inflate::Zlib(a_data.subspan(4), inflated)) {
					++_result.compressedSkipped;
					return;
				}
				a_data = inflated;
			}

			std::string                editorID;
			std::uint32_t              name = 0, location = 0, parent = 0, refLocation = 0;
			std::uint8_t               cellFlags = 0;
			std::span<const std::byte> vmad;
			std::vector<std::string>   listEntries;

			std::size_t   pos = 0;
			std::uint32_t bigSize = 0;
			while (pos + 6 <= a_data.size()) {
				const auto     type = FourCC(a_data.data() + pos);
				std::uint16_t  shortSize;
				std::memcpy(&shortSize, a_data.data() + pos + 4, sizeof(shortSize));
				const std::size_t size = bigSize ? bigSize : shortSize;
				pos += 6;
				if (pos + size > a_data.size()) {
					break;
				}
				const auto body = a_data.subspan(pos, size);
				pos += size;

				if (type == "XXXX") {
					bigSize = size >= 4 ? U32(body.data()) : 0;
					continue;
				}
				bigSize = 0;

				if (type == "EDID") {
					editorID = ZString(body);
				} else if (type == "VMAD") {
					vmad = body;
				} else if (type == "NAME" && size >= 4) {
					name = U32(body.data());
				} else if (type == "XLCN" && size >= 4) {
					location = U32(body.data());
				} else if (type == "XLRL" && size >= 4) {
					refLocation = U32(body.data());
				} else if (type == "LNAM" && size >= 4 && a_type == "FLST") {
					listEntries.push_back(keyFor(U32(body.data())));
				} else if (type == "PNAM" && size >= 4 && a_type == "LCTN") {
					parent = U32(body.data());
				} else if (type == "DATA" && size >= 1 && a_type == "CELL") {
					cellFlags = static_cast<std::uint8_t>(body[0]);
				}
			}

			if (a_type == "WOOP") {
				_result.words[key] = editorID;
			} else if (a_type == "CELL") {
				const auto world = enclosing({ worldChildren });
				_result.cells[key] = { editorID, keyFor(location), keyFor(world), (cellFlags & 1) != 0 };
			} else if (a_type == "WRLD") {
				_result.worldspaces[key] = editorID;
			} else if (a_type == "LCTN") {
				_result.locations[key] = { editorID, keyFor(parent) };
			} else if (a_type == "FLST") {
				// Form lists followed, shout properties aren't
				if (listEntries.empty() || (a_flags & recordDeleted)) {
					_formLists.erase(key);
				} else {
					_formLists[key] = std::move(listEntries);
				}
			} else if (KeepsEditorID(a_type) || a_type == "QUST") {
				_result.editorIDs[key] = editorID;
			}

			std::vector<ObjectProperty> properties;
			if (!vmad.empty() && !(a_flags & recordDeleted)) {
				properties = readScripts(vmad, a_type == "QUST");
			}
			if (properties.empty()) {
				_scripted.erase(key);
				return;
			}
			auto& record = _scripted[key];
			record.type = a_type;
			record.editorID = editorID;
			record.plugin = std::string(a_name);
			record.base = IsPlaced(a_type) ? keyFor(name) : std::string{};
			record.cell = IsPlaced(a_type) ? keyFor(enclosing({ cellChildren, cellPersistent, cellTemporary, cellDistant })) : std::string{};
			record.location = IsPlaced(a_type) ? keyFor(refLocation) : std::string{};
			record.properties = std::move(properties);
		};

		const auto walk = [&](auto&& a_self, std::size_t a_begin, std::size_t a_end) -> void {
			std::size_t pos = a_begin;
			while (pos + 24 <= a_end) {
				const auto* header = a_data.data() + pos;
				const auto  type = FourCC(header);
				const auto  size = U32(header + 4);
				if (type == "GRUP") {
					if (size < 24 || pos + size > a_end) {
						_result.errors.push_back(std::string(a_name) + ": bad group size");
						return;
					}
					std::int32_t groupType;
					std::memcpy(&groupType, header + 12, sizeof(groupType));
					groups.push_back({ groupType, U32(header + 8) });
					a_self(a_self, pos + 24, pos + size);
					groups.pop_back();
					pos += size;
				} else {
					if (pos + 24 + size > a_end) {
						_result.errors.push_back(std::string(a_name) + ": record runs past its group");
						return;
					}
					handleRecord(type, U32(header + 8), U32(header + 12), a_data.subspan(pos + 24, size));
					pos += 24 + static_cast<std::size_t>(size);
				}
			}
		};
		walk(walk, 24 + static_cast<std::size_t>(headerSize), a_data.size());
	}

	//---------------------------------------------------
	//-- Plugin Scan ( Finish ) -------------------------
	//---------------------------------------------------

	Result Scanner::Finish() {

		for (const auto& [key, record] : _scripted) {
			for (const auto& property : record.properties) {
				std::vector<std::pair<std::string, std::string>> words;
				if (_result.words.contains(property.target)) {
					words.emplace_back(property.target, property.script);
				} else if (const auto list = _formLists.find(property.target); list != _formLists.end()) {
					for (const auto& entry : list->second) {
						if (_result.words.contains(entry)) {
							words.emplace_back(entry, property.script + " (list)");
						}
					}
				}

				for (const auto& [word, script] : words) {
					WordSource source;
					source.kind = IsPlaced(record.type) ? WordSource::Kind::kReference : record.type == "QUST" ? WordSource::Kind::kQuest : WordSource::Kind::kOther;
					source.record = key;
					source.recordType = record.type;
					source.editorID = record.editorID;
					source.base = record.base;
					source.cell = record.cell;
					source.location = record.location;
					source.script = script;
					source.plugin = record.plugin;
					_result.sources[word].push_back(std::move(source));
				}
			}
		}
		_scripted.clear();
		_formLists.clear();
		return std::move(_result);
	}
}
