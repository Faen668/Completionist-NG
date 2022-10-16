#pragma once

#include <SKSE\API.h>

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor001
{
	using EventResult = RE::BSEventNotifyControl;

	class ScriptEventHandler final : 
		public RE::BSTEventSink<RE::TESQuestStageEvent> {
	
	public: 
		[[nodiscard]] static ScriptEventHandler* GetSingleton() {
			static ScriptEventHandler singleton;
			return &singleton;
		}

		static void Register() {
			register_event<RE::TESQuestStageEvent>();
		}
		
		EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;
	
		static bool IsValidQuest(std::string questID, std::string checkID);

	private:
		ScriptEventHandler() = default;
		ScriptEventHandler(const ScriptEventHandler&) = delete;
		ScriptEventHandler(ScriptEventHandler&&) = delete;

		~ScriptEventHandler() override = default;

		ScriptEventHandler& operator=(const ScriptEventHandler&) = delete;
		ScriptEventHandler& operator=(ScriptEventHandler&&) = delete;

		template <class T>
		static void register_event()
		{
			if (const auto scripts = RE::ScriptEventSourceHolder::GetSingleton(); scripts) {
				scripts->AddEventSink<T>(GetSingleton());
			}
		}
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor013
{
	using EventResult = RE::BSEventNotifyControl;

	class ScriptEventHandler final :
		public RE::BSTEventSink<RE::TESQuestStageEvent> {

	public:
		[[nodiscard]] static ScriptEventHandler* GetSingleton() {
			static ScriptEventHandler singleton;
			return &singleton;
		}

		static void Register() {
			register_event<RE::TESQuestStageEvent>();
		}

		EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;

		static bool IsValidQuest(std::string questID, std::string checkID);

	private:
		ScriptEventHandler() = default;
		ScriptEventHandler(const ScriptEventHandler&) = delete;
		ScriptEventHandler(ScriptEventHandler&&) = delete;

		~ScriptEventHandler() override = default;

		ScriptEventHandler& operator=(const ScriptEventHandler&) = delete;
		ScriptEventHandler& operator=(ScriptEventHandler&&) = delete;

		template <class T>
		static void register_event()
		{
			if (const auto scripts = RE::ScriptEventSourceHolder::GetSingleton(); scripts) {
				scripts->AddEventSink<T>(GetSingleton());
			}
		}
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor017
{
	using EventResult = RE::BSEventNotifyControl;

	class ScriptEventHandler final :
		public RE::BSTEventSink<RE::TESQuestStageEvent> {

	public:
		[[nodiscard]] static ScriptEventHandler* GetSingleton() {
			static ScriptEventHandler singleton;
			return &singleton;
		}

		static void Register() {
			register_event<RE::TESQuestStageEvent>();
		}

		EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;

	private:
		ScriptEventHandler() = default;
		ScriptEventHandler(const ScriptEventHandler&) = delete;
		ScriptEventHandler(ScriptEventHandler&&) = delete;

		~ScriptEventHandler() override = default;

		ScriptEventHandler& operator=(const ScriptEventHandler&) = delete;
		ScriptEventHandler& operator=(ScriptEventHandler&&) = delete;

		template <class T>
		static void register_event()
		{
			if (const auto scripts = RE::ScriptEventSourceHolder::GetSingleton(); scripts) {
				scripts->AddEventSink<T>(GetSingleton());
			}
		}
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor018
{
	using EventResult = RE::BSEventNotifyControl;

	class ScriptEventHandler final :
		public RE::BSTEventSink<RE::TESQuestStageEvent> {

	public:
		[[nodiscard]] static ScriptEventHandler* GetSingleton() {
			static ScriptEventHandler singleton;
			return &singleton;
		}

		static void Register() {
			register_event<RE::TESQuestStageEvent>();
		}

		EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;

		static bool IsValidQuest(std::string questID, std::string checkID);

	private:
		ScriptEventHandler() = default;
		ScriptEventHandler(const ScriptEventHandler&) = delete;
		ScriptEventHandler(ScriptEventHandler&&) = delete;

		~ScriptEventHandler() override = default;

		ScriptEventHandler& operator=(const ScriptEventHandler&) = delete;
		ScriptEventHandler& operator=(ScriptEventHandler&&) = delete;

		template <class T>
		static void register_event()
		{
			if (const auto scripts = RE::ScriptEventSourceHolder::GetSingleton(); scripts) {
				scripts->AddEventSink<T>(GetSingleton());
			}
		}
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor019
{
	using EventResult = RE::BSEventNotifyControl;

	class ScriptEventHandler final :
		public RE::BSTEventSink<RE::TESQuestStageEvent> {

	public:
		[[nodiscard]] static ScriptEventHandler* GetSingleton() {
			static ScriptEventHandler singleton;
			return &singleton;
		}

		static void Register() {
			register_event<RE::TESQuestStageEvent>();
		}

		EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;

	private:
		ScriptEventHandler() = default;
		ScriptEventHandler(const ScriptEventHandler&) = delete;
		ScriptEventHandler(ScriptEventHandler&&) = delete;

		~ScriptEventHandler() override = default;

		ScriptEventHandler& operator=(const ScriptEventHandler&) = delete;
		ScriptEventHandler& operator=(ScriptEventHandler&&) = delete;

		template <class T>
		static void register_event()
		{
			if (const auto scripts = RE::ScriptEventSourceHolder::GetSingleton(); scripts) {
				scripts->AddEventSink<T>(GetSingleton());
			}
		}
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor104
{
	using EventResult = RE::BSEventNotifyControl;

	class ScriptEventHandler final :
		public RE::BSTEventSink<RE::TESQuestStageEvent> {

	public:
		[[nodiscard]] static ScriptEventHandler* GetSingleton() {
			static ScriptEventHandler singleton;
			return &singleton;
		}

		static void Register() {
			register_event<RE::TESQuestStageEvent>();
		}

		EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;

		static bool IsValidQuest(std::string questID, std::string checkID);

	private:
		ScriptEventHandler() = default;
		ScriptEventHandler(const ScriptEventHandler&) = delete;
		ScriptEventHandler(ScriptEventHandler&&) = delete;

		~ScriptEventHandler() override = default;

		ScriptEventHandler& operator=(const ScriptEventHandler&) = delete;
		ScriptEventHandler& operator=(ScriptEventHandler&&) = delete;

		template <class T>
		static void register_event()
		{
			if (const auto scripts = RE::ScriptEventSourceHolder::GetSingleton(); scripts) {
				scripts->AddEventSink<T>(GetSingleton());
			}
		}
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor109
{
	using EventResult = RE::BSEventNotifyControl;

	class ScriptEventHandler final :
		public RE::BSTEventSink<RE::TESQuestStageEvent> {

	public:
		[[nodiscard]] static ScriptEventHandler* GetSingleton() {
			static ScriptEventHandler singleton;
			return &singleton;
		}

		static void Register() {
			register_event<RE::TESQuestStageEvent>();
		}

		EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;

	private:
		ScriptEventHandler() = default;
		ScriptEventHandler(const ScriptEventHandler&) = delete;
		ScriptEventHandler(ScriptEventHandler&&) = delete;

		~ScriptEventHandler() override = default;

		ScriptEventHandler& operator=(const ScriptEventHandler&) = delete;
		ScriptEventHandler& operator=(ScriptEventHandler&&) = delete;

		template <class T>
		static void register_event()
		{
			if (const auto scripts = RE::ScriptEventSourceHolder::GetSingleton(); scripts) {
				scripts->AddEventSink<T>(GetSingleton());
			}
		}
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor110
{
	using EventResult = RE::BSEventNotifyControl;

	class ScriptEventHandler final :
		public RE::BSTEventSink<RE::TESQuestStageEvent> {

	public:
		[[nodiscard]] static ScriptEventHandler* GetSingleton() {
			static ScriptEventHandler singleton;
			return &singleton;
		}

		static void Register() {
			register_event<RE::TESQuestStageEvent>();
		}

		EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;

		static bool IsValidQuest(std::string questID, std::string checkID);

	private:
		ScriptEventHandler() = default;
		ScriptEventHandler(const ScriptEventHandler&) = delete;
		ScriptEventHandler(ScriptEventHandler&&) = delete;

		~ScriptEventHandler() override = default;

		ScriptEventHandler& operator=(const ScriptEventHandler&) = delete;
		ScriptEventHandler& operator=(ScriptEventHandler&&) = delete;

		template <class T>
		static void register_event()
		{
			if (const auto scripts = RE::ScriptEventSourceHolder::GetSingleton(); scripts) {
				scripts->AddEventSink<T>(GetSingleton());
			}
		}
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor151
{
	using EventResult = RE::BSEventNotifyControl;

	class ScriptEventHandler final :
		public RE::BSTEventSink<RE::TESQuestStageEvent> {

	public:
		[[nodiscard]] static ScriptEventHandler* GetSingleton() {
			static ScriptEventHandler singleton;
			return &singleton;
		}

		static void Register() {
			register_event<RE::TESQuestStageEvent>();
		}

		EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;

	private:
		ScriptEventHandler() = default;
		ScriptEventHandler(const ScriptEventHandler&) = delete;
		ScriptEventHandler(ScriptEventHandler&&) = delete;

		~ScriptEventHandler() override = default;

		ScriptEventHandler& operator=(const ScriptEventHandler&) = delete;
		ScriptEventHandler& operator=(ScriptEventHandler&&) = delete;

		template <class T>
		static void register_event()
		{
			if (const auto scripts = RE::ScriptEventSourceHolder::GetSingleton(); scripts) {
				scripts->AddEventSink<T>(GetSingleton());
			}
		}
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor153
{
	using EventResult = RE::BSEventNotifyControl;

	class ScriptEventHandler final :
		public RE::BSTEventSink<RE::TESQuestStageEvent> {

	public:
		[[nodiscard]] static ScriptEventHandler* GetSingleton() {
			static ScriptEventHandler singleton;
			return &singleton;
		}

		static void Register() {
			register_event<RE::TESQuestStageEvent>();
		}

		EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;

	private:
		ScriptEventHandler() = default;
		ScriptEventHandler(const ScriptEventHandler&) = delete;
		ScriptEventHandler(ScriptEventHandler&&) = delete;

		~ScriptEventHandler() override = default;

		ScriptEventHandler& operator=(const ScriptEventHandler&) = delete;
		ScriptEventHandler& operator=(ScriptEventHandler&&) = delete;

		template <class T>
		static void register_event()
		{
			if (const auto scripts = RE::ScriptEventSourceHolder::GetSingleton(); scripts) {
				scripts->AddEventSink<T>(GetSingleton());
			}
		}
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor154
{
	using EventResult = RE::BSEventNotifyControl;

	class ScriptEventHandler final :
		public RE::BSTEventSink<RE::TESQuestStageEvent> {

	public:
		[[nodiscard]] static ScriptEventHandler* GetSingleton() {
			static ScriptEventHandler singleton;
			return &singleton;
		}

		static void Register() {
			register_event<RE::TESQuestStageEvent>();
		}

		EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;

	private:
		ScriptEventHandler() = default;
		ScriptEventHandler(const ScriptEventHandler&) = delete;
		ScriptEventHandler(ScriptEventHandler&&) = delete;

		~ScriptEventHandler() override = default;

		ScriptEventHandler& operator=(const ScriptEventHandler&) = delete;
		ScriptEventHandler& operator=(ScriptEventHandler&&) = delete;

		template <class T>
		static void register_event()
		{
			if (const auto scripts = RE::ScriptEventSourceHolder::GetSingleton(); scripts) {
				scripts->AddEventSink<T>(GetSingleton());
			}
		}
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor157
{
	using EventResult = RE::BSEventNotifyControl;

	class ScriptEventHandler final :
		public RE::BSTEventSink<RE::TESQuestStageEvent> {

	public:
		[[nodiscard]] static ScriptEventHandler* GetSingleton() {
			static ScriptEventHandler singleton;
			return &singleton;
		}

		static void Register() {
			register_event<RE::TESQuestStageEvent>();
		}

		EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;

	private:
		ScriptEventHandler() = default;
		ScriptEventHandler(const ScriptEventHandler&) = delete;
		ScriptEventHandler(ScriptEventHandler&&) = delete;

		~ScriptEventHandler() override = default;

		ScriptEventHandler& operator=(const ScriptEventHandler&) = delete;
		ScriptEventHandler& operator=(ScriptEventHandler&&) = delete;

		template <class T>
		static void register_event()
		{
			if (const auto scripts = RE::ScriptEventSourceHolder::GetSingleton(); scripts) {
				scripts->AddEventSink<T>(GetSingleton());
			}
		}
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor158
{
	using EventResult = RE::BSEventNotifyControl;

	class ScriptEventHandler final :
		public RE::BSTEventSink<RE::TESQuestStageEvent> {

	public:
		[[nodiscard]] static ScriptEventHandler* GetSingleton() {
			static ScriptEventHandler singleton;
			return &singleton;
		}

		static void Register() {
			register_event<RE::TESQuestStageEvent>();
		}

		EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;

	private:
		ScriptEventHandler() = default;
		ScriptEventHandler(const ScriptEventHandler&) = delete;
		ScriptEventHandler(ScriptEventHandler&&) = delete;

		~ScriptEventHandler() override = default;

		ScriptEventHandler& operator=(const ScriptEventHandler&) = delete;
		ScriptEventHandler& operator=(ScriptEventHandler&&) = delete;

		template <class T>
		static void register_event()
		{
			if (const auto scripts = RE::ScriptEventSourceHolder::GetSingleton(); scripts) {
				scripts->AddEventSink<T>(GetSingleton());
			}
		}
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor204
{
	using EventResult = RE::BSEventNotifyControl;

	class ScriptEventHandler final :
		public RE::BSTEventSink<RE::TESQuestStageEvent> {

	public:
		[[nodiscard]] static ScriptEventHandler* GetSingleton() {
			static ScriptEventHandler singleton;
			return &singleton;
		}

		static void Register() {
			register_event<RE::TESQuestStageEvent>();
		}

		EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;

		static bool IsValidQuest(std::string questID, std::string checkID);

	private:
		ScriptEventHandler() = default;
		ScriptEventHandler(const ScriptEventHandler&) = delete;
		ScriptEventHandler(ScriptEventHandler&&) = delete;

		~ScriptEventHandler() override = default;

		ScriptEventHandler& operator=(const ScriptEventHandler&) = delete;
		ScriptEventHandler& operator=(ScriptEventHandler&&) = delete;

		template <class T>
		static void register_event()
		{
			if (const auto scripts = RE::ScriptEventSourceHolder::GetSingleton(); scripts) {
				scripts->AddEventSink<T>(GetSingleton());
			}
		}
	};
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor205
{
	using EventResult = RE::BSEventNotifyControl;

	class ScriptEventHandler final :
		public RE::BSTEventSink<RE::TESQuestStageEvent> {

	public:
		[[nodiscard]] static ScriptEventHandler* GetSingleton() {
			static ScriptEventHandler singleton;
			return &singleton;
		}

		static void Register() {
			register_event<RE::TESQuestStageEvent>();
		}

		EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;

		static bool IsValidQuest(std::string questID, std::string checkID);

	private:
		ScriptEventHandler() = default;
		ScriptEventHandler(const ScriptEventHandler&) = delete;
		ScriptEventHandler(ScriptEventHandler&&) = delete;

		~ScriptEventHandler() override = default;

		ScriptEventHandler& operator=(const ScriptEventHandler&) = delete;
		ScriptEventHandler& operator=(ScriptEventHandler&&) = delete;

		template <class T>
		static void register_event()
		{
			if (const auto scripts = RE::ScriptEventSourceHolder::GetSingleton(); scripts) {
				scripts->AddEventSink<T>(GetSingleton());
			}
		}
	};
}
