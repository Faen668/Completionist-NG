#pragma once

namespace CSettings
{
	// [Settings] in Completionist.ini, names match the old MCM properties
	class SettingsAPI final
	{
	public:
		static void Load();

		[[nodiscard]] static RE::BSScript::Variable* Get(std::string_view a_name);
		[[nodiscard]] static bool                    Has(std::string_view a_name);

		static void SetBool(std::string_view a_name, bool a_value);
		static void SetInt(std::string_view a_name, std::int32_t a_value);
		static void SetString(std::string_view a_name, std::string_view a_value);

		static void Save();
	};
}
