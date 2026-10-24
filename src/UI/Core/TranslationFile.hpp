#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace CompletionistUI
{
	// CompletionistData/Translations file (UTF-8)
	[[nodiscard]] std::unordered_map<std::string, std::string> ParseModTranslations(std::string_view a_rawUtf8);

	// $Key{arg1}{arg2}, fills each {} and turns \n into a line break
	[[nodiscard]] std::string FormatInterfaceString(const std::string& a_key, const std::function<std::string(const std::string&)>& a_lookup);
}
