#pragma once

#include <SKSE\API.h>
#include "Serialization.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CPatch_AHD_Items {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_AHD
{
	enum PatchID : std::int32_t
	{
		kItems = 200,
	};

	extern std::vector<std::string> Items_NameArray;
	extern std::vector<std::string> Items_TextArray;
	extern std::vector<RE::TESForm*> Items_FormArray;
	extern std::vector<bool> Items_BoolArray;
	extern std::int32_t Items_EntriesTotal;
	extern std::int32_t Items_EntriesFound;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :

	public RE::BSTEventSink<RE::TESContainerChangedEvent> {

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

	EventResult			ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;

	static void			SinkEvents();
	static void			InstallFramework();
	static void			UpdateFoundForms();
	static void			InjectAndCompileData();

	static void			ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_curID, std::string a_variable);
	};
}