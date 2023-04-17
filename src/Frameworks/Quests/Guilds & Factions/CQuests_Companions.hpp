#pragma once

namespace CQFramework_Companions
{
	inline std::vector<std::string>		NameArray;
	inline std::vector<std::string>		IdenArray;
	inline std::vector<std::string>		TextArray;
	inline std::vector<std::int32_t>	RadiArray;
	inline std::vector<bool>			BoolArray;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :
		public RE::BSTEventSink<RE::TESQuestStageEvent>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>*) override;
		  static void InstallFramework();
	};
}
