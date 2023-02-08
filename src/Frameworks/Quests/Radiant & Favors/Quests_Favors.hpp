#pragma once

#include "Serialization.hpp";

struct FavorQuestData
{
	struct Data
	{
		RE::FormID ID;
		const char* FN;
	};

	Data base;
	Data vari;
	Data actr;

	std::int32_t Stage;
	std::int32_t Value;

	const char* Global;
};

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace FavorProcessor
{
	template <std::size_t N>
	static void ProcessQuest(const RE::FormID a_formID, const FavorQuestData(&a_array)[N], std::uint16_t a_stage);

	auto Register() -> void;
}		

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor001
{
	using EventResult = RE::BSEventNotifyControl;

	class CHandler: public
		RE::BSTEventSink<RE::TESQuestStageEvent>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;
		  static void Sink();
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor013
{
	using EventResult = RE::BSEventNotifyControl;

	class CHandler final : public
		RE::BSTEventSink<RE::TESQuestStageEvent>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;
		  static void Sink();
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor017
{
	using EventResult = RE::BSEventNotifyControl;

	class CHandler final : public
		RE::BSTEventSink<RE::TESQuestStageEvent>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;
		  static void Sink();
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor018
{
	using EventResult = RE::BSEventNotifyControl;

	class CHandler final : public
		RE::BSTEventSink<RE::TESQuestStageEvent>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;
		  static void Sink();
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor019
{
	using EventResult = RE::BSEventNotifyControl;

	class CHandler final : public
		RE::BSTEventSink<RE::TESQuestStageEvent>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;
		  static void Sink();
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor104
{
	using EventResult = RE::BSEventNotifyControl;

	class CHandler final : public
		RE::BSTEventSink<RE::TESQuestStageEvent>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;
		  static void Sink();
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor109
{
	using EventResult = RE::BSEventNotifyControl;

	class CHandler final : public
		RE::BSTEventSink<RE::TESQuestStageEvent>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;
		  static void Sink();
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor110
{
	using EventResult = RE::BSEventNotifyControl;

	class CHandler final : public
		RE::BSTEventSink<RE::TESQuestStageEvent>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;
		  static void Sink();
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor151
{
	using EventResult = RE::BSEventNotifyControl;

	class CHandler final : public
		RE::BSTEventSink<RE::TESQuestStageEvent>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;
		  static void Sink();
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor153
{
	using EventResult = RE::BSEventNotifyControl;

	class CHandler final : public
		RE::BSTEventSink<RE::TESQuestStageEvent>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;
		  static void Sink();
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor154
{
	using EventResult = RE::BSEventNotifyControl;

	class CHandler final : public
		RE::BSTEventSink<RE::TESQuestStageEvent>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;
		  static void Sink();
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor157
{
	using EventResult = RE::BSEventNotifyControl;

	class CHandler final : public
		RE::BSTEventSink<RE::TESQuestStageEvent>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;
		  static void Sink();
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor158
{
	using EventResult = RE::BSEventNotifyControl;

	class CHandler final : public
		RE::BSTEventSink<RE::TESQuestStageEvent>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;
		  static void Sink();
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor204
{
	using EventResult = RE::BSEventNotifyControl;

	class CHandler final : public
		RE::BSTEventSink<RE::TESQuestStageEvent>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;
		  static void Sink();
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor205
{
	using EventResult = RE::BSEventNotifyControl;

	class CHandler final : public
		RE::BSTEventSink<RE::TESQuestStageEvent>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;
		  static void Sink();
	};
}
