#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

// Reads plugin files directly for word of power script properties and placed references
namespace PluginScan
{
	struct Cell
	{
		std::string  editorID;
		std::string  location;
		std::string  worldspace;
		bool         interior{ false };
	};

	struct Location
	{
		std::string editorID;
		std::string parent;
	};

	struct WordSource
	{
		enum class Kind : std::uint8_t
		{
			kReference,
			kQuest,
			kOther,
		};

		Kind        kind{ Kind::kOther };
		std::string record;
		std::string recordType;
		std::string editorID;
		std::string base;
		std::string cell;
		std::string location;	// XLRL, outranks the cell's
		std::string script;
		std::string plugin;
	};

	// A placed reference as the last file to touch it has it
	struct Reference
	{
		std::string          base;
		std::string          cell;
		std::string          worldspace;	// exterior only
		std::array<float, 3> position{};
		std::string          plugin;
		bool                 deleted{ false };
	};

	struct Result
	{
		std::map<std::string, std::string>              words;
		std::map<std::string, std::vector<WordSource>>  sources;
		std::map<std::string, Cell>                     cells;
		std::map<std::string, Location>                 locations;
		std::map<std::string, std::string>              worldspaces;
		std::map<std::string, std::string>              editorIDs;
		std::map<std::string, Reference>                references;
		std::size_t                                     records{ 0 };
		std::size_t                                     compressedSkipped{ 0 };
		std::vector<std::string>                        errors;
	};

	class Scanner
	{
	public:
		void AddFile(std::string_view a_name, std::span<const std::byte> a_data);

		// Only these placed references are read, and the cells they could be in
		void WantReferences(std::set<std::string> a_keys) { _wanted = std::move(a_keys); }

		[[nodiscard]] Result Finish();

		static bool SplitKey(std::string_view a_key, std::string& a_plugin, std::uint32_t& a_localID);

		// Storage, QA and test cells
		static bool IsDeveloperCell(const Cell& a_cell);

		// YsgramorsTombExterior02 -> YsgramorsTombLocation
		static std::string NamedLocation(const Result& a_result, const Cell& a_cell);

	private:
		struct ObjectProperty
		{
			std::string script;
			std::string target;
		};

		struct ScriptedRecord
		{
			std::string                 type;
			std::string                 editorID;
			std::string                 base;
			std::string                 cell;
			std::string                 location;
			std::string                 plugin;
			std::vector<ObjectProperty> properties;
		};

		std::map<std::string, ScriptedRecord>           _scripted;
		std::map<std::string, std::vector<std::string>> _formLists;
		std::set<std::string>                           _wanted;
		Result                                _result;
	};
}
