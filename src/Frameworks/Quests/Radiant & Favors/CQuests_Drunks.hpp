#pragma once

namespace CQFramework_FavorQuests
{
	class DrunkHandler final
	{
	public:
		  static void InstallFramework();
		  static void OnContainerChanged(RE::TESContainerChangedEvent const* a_event);
		  static void AddDrunkData(const std::string& Quest_Key, const RE::FormID a_listID, const RE::FormID a_baseID, const std::string& listFileName, const std::string& baseFileName);

		  DrunkHandler(DrunkHandler const&) = delete;
		  DrunkHandler(DrunkHandler const&&) = delete;
		  DrunkHandler operator=(DrunkHandler&) = delete;
		  DrunkHandler operator=(DrunkHandler&&) = delete;

	private:
		DrunkHandler() = default;
		~DrunkHandler() = default;
	};
}