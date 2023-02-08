#pragma once

namespace CQFramework_Misc_SK
{	
	inline std::vector<std::string>		NameArray_Side;
	inline std::vector<std::string>		IdenArray_Side;
	inline std::vector<std::string>		TextArray_Side;
	inline std::vector<std::string>		KeysArray_Side;
	inline std::vector<std::int32_t>	RadiArray_Side;
	inline std::vector<bool>			BoolArray_Side;

	inline std::vector<std::string>		NameArray_Radi;
	inline std::vector<std::string>		IdenArray_Radi;
	inline std::vector<std::string>		TextArray_Radi;
	inline std::vector<std::string>		KeysArray_Radi;
	inline std::vector<std::int32_t>	RadiArray_Radi;
	inline std::vector<bool>			BoolArray_Radi;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :
		public RE::BSTEventSink<RE::MenuOpenCloseEvent> {

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult			ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

		  static void			SinkEvents();
		  static void			InstallFramework();
		  static void			UpdateCompletion();
	};
}
