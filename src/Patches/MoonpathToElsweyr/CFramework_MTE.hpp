#pragma once

#include <SKSE\API.h>
#include "Serialization.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CPatch_MTE_Items {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_MTE_Books {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_MTE 
{	
	extern std::vector<std::string> Items_NameArray;
	extern std::vector<std::string> Items_TextArray;
	extern std::vector<RE::TESForm*> Items_FormArray;
	extern std::vector<bool> Items_BoolArray;
	extern std::int32_t Items_EntriesTotal;
	extern std::int32_t Items_EntriesFound;

	extern std::vector<std::string> Books_NameArray;
	extern std::vector<std::string> Books_TextArray;
	extern std::vector<RE::TESForm*> Books_FormArray;
	extern std::vector<bool> Books_BoolArray;
	extern std::int32_t Books_EntriesTotal;
	extern std::int32_t Books_EntriesFound;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :

	public RE::BSTEventSink<RE::TESContainerChangedEvent>,
	public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
	public RE::BSTEventSink<RE::BooksRead::Event> {

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

	EventResult			ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;
	EventResult			ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) override;
	EventResult			ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

	static void			SinkEvents();
	static void			InstallFramework();
	static void			UpdateFoundForms();
	static void			InjectAndCompileData();

	static void			ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_curID, std::string a_variable);
	};
}