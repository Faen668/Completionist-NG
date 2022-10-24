#pragma once

namespace CQFramework_SmallTowns
{	
	inline std::vector<std::string>		NameArray;
	inline std::vector<std::string>		IdenArray;
	inline std::vector<std::string>		TextArray;
	inline std::vector<std::string>		KeysArray;
	inline std::vector<std::int32_t>	RadiArray;
	inline std::vector<bool>			BoolArray;
	inline std::vector<bool>			StgeArray;


	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :
		public RE::BSTEventSink<RE::TESContainerChangedEvent>,
		public RE::BSTEventSink<RE::MenuOpenCloseEvent> {

		public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		EventResult			ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override;
		EventResult			ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;

		static void			SinkEvents();
		static void			InstallFramework();
		static void			UpdateCompletion();
	};
}
