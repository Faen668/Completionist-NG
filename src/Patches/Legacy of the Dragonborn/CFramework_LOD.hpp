#pragma once

namespace CPatch_LOD
{	
	inline std::vector<std::string>		Quest1_NameArray;
	inline std::vector<std::string>		Quest1_IdenArray;
	inline std::vector<std::string>		Quest1_TextArray;
	inline std::vector<std::string>		Quest1_KeysArray;
	inline std::vector<std::int32_t>	Quest1_RadiArray;
	inline std::vector<bool>			Quest1_BoolArray;
	inline std::vector<bool>			Quest1_StgeArray;

	inline std::vector<std::string>		Quest2_NameArray;
	inline std::vector<std::string>		Quest2_IdenArray;
	inline std::vector<std::string>		Quest2_TextArray;
	inline std::vector<std::string>		Quest2_KeysArray;
	inline std::vector<std::int32_t>	Quest2_RadiArray;
	inline std::vector<bool>			Quest2_BoolArray;
	inline std::vector<bool>			Quest2_StgeArray;

	inline std::vector<std::string>		Quest3_NameArray;
	inline std::vector<std::string>		Quest3_IdenArray;
	inline std::vector<std::string>		Quest3_TextArray;
	inline std::vector<std::string>		Quest3_KeysArray;
	inline std::vector<std::int32_t>	Quest3_RadiArray;
	inline std::vector<bool>			Quest3_BoolArray;
	inline std::vector<bool>			Quest3_StgeArray;

	inline std::vector<std::string>		Quest4_NameArray;
	inline std::vector<std::string>		Quest4_IdenArray;
	inline std::vector<std::string>		Quest4_TextArray;
	inline std::vector<std::string>		Quest4_KeysArray;
	inline std::vector<std::int32_t>	Quest4_RadiArray;
	inline std::vector<bool>			Quest4_BoolArray;
	inline std::vector<bool>			Quest4_StgeArray;

	inline std::vector<std::string>		Quest5_NameArray;
	inline std::vector<std::string>		Quest5_IdenArray;
	inline std::vector<std::string>		Quest5_TextArray;
	inline std::vector<std::string>		Quest5_KeysArray;
	inline std::vector<std::int32_t>	Quest5_RadiArray;
	inline std::vector<bool>			Quest5_BoolArray;
	inline std::vector<bool>			Quest5_StgeArray;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :
		public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
		public RE::BSTEventSink<RE::TESQuestStageEvent> {

		public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		EventResult			ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;
		EventResult			ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;

		static void			SinkEvents();

		static void			InstallFramework();
		static void			InstallQuestFramework();

		static void			UpdateQuestFramework();
	};
}