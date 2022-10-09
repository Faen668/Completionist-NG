#pragma once

#include "Serialization.hpp"

namespace CQFramework_DB
{

	extern std::vector<RE::TESForm*>	FormArray;
	extern std::vector<std::string>		NameArray;
	extern std::vector<std::string>		IdenArray;
	extern std::vector<std::string>		TextArray;
	extern std::vector<std::string>		KeysArray;
	extern std::vector<std::int32_t>	RadiArray;
	extern std::vector<bool>			BoolArray;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final : public RE::BSTEventSink<RE::MenuOpenCloseEvent> {

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult			ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

		  static void			SinkEvents();
		  static void			InstallFramework();
		  static void			UpdateCompletion();

		  static void			AddQuest(std::string k, std::string n, std::int32_t t, std::string h, std::string g);
	};
}