#include "UI/Core/TranslationFile.hpp"

namespace CompletionistUI
{
	//---------------------------------------------------
	//-- Translation File ( Parse Mod Translations ) ----
	//---------------------------------------------------

	std::unordered_map<std::string, std::string> ParseModTranslations(std::string_view a_rawUtf8) {

		std::unordered_map<std::string, std::string> strings;
		while (!a_rawUtf8.empty()) {
			const auto lineEnd = a_rawUtf8.find('\n');
			auto       line = a_rawUtf8.substr(0, lineEnd);
			a_rawUtf8 = lineEnd == std::string_view::npos ? std::string_view{} : a_rawUtf8.substr(lineEnd + 1);

			if (line.ends_with('\r')) {
				line.remove_suffix(1);
			}
			const auto tab = line.find('\t');
			const auto pipe = line.find('|');
			if (!line.starts_with('-') && tab != std::string_view::npos && pipe != std::string_view::npos) {
				strings.emplace(line.substr(0, tab), line.substr(pipe + 1));
			}
		}
		return strings;
	}

	//---------------------------------------------------
	//-- Translation File ( Format Interface String ) ---
	//---------------------------------------------------

	std::string FormatInterfaceString(const std::string& a_key, const std::function<std::string(const std::string&)>& a_lookup) {

		std::vector<std::string> args;
		const auto               brace = a_key.find('{');
		for (auto pos = brace; pos != std::string::npos;) {
			const auto end = a_key.find('}', pos);
			if (end == std::string::npos) {
				break;
			}
			args.push_back(a_key.substr(pos + 1, end - pos - 1));
			pos = a_key.find('{', end);
		}

		std::string lookupKey = a_key.substr(0, brace);
		for (std::size_t i = 0; i < args.size(); ++i) {
			lookupKey += "{}";
		}

		std::string result = a_lookup(lookupKey);
		if (result == lookupKey) {
			result = a_key;
			args.clear();
		}
		for (auto pos = result.find("\\n"); pos != std::string::npos; pos = result.find("\\n", pos + 1)) {
			result.replace(pos, 2, "\n");
		}

		std::size_t slot = 0;
		for (const auto& arg : args) {
			slot = result.find("{}", slot);
			if (slot == std::string::npos) {
				break;
			}
			const auto value = arg.starts_with('$') ? a_lookup(arg) : arg;
			result.replace(slot, 2, value);
			slot += value.size();
		}
		return result;
	}
}
