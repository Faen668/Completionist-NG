#pragma once
#include "Serialization.hpp"

namespace CFramework_Trainers
{
	// Anyone whose class trains a skill, vanilla or mod added
	struct Trainer
	{
		RE::TESNPC*         npc{ nullptr };
		RE::ObjectRefHandle ref;
		std::int32_t        skill{ 0 };	// CLASS_DATA::Skill, 0-17
		std::int32_t        maxLevel{ 0 };
	};

	inline std::vector<Trainer> Trainers;

	// Marking by hand, clear of the MCM's framework IDs
	inline constexpr std::int32_t FrameworkID = 100;

	class CHandler final : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
	{
	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override;

		static void InstallFramework();
		static void FindPlacedTrainers();
		static void MarkTrained(RE::TESNPC* a_npc);

		[[nodiscard]] static bool               IsTrained(const Trainer& a_trainer);
		[[nodiscard]] static RE::ActorValue     SkillValue(std::int32_t a_skill) { return static_cast<RE::ActorValue>(std::to_underlying(RE::ActorValue::kOneHanded) + a_skill); }
		[[nodiscard]] static std::string        SkillName(std::int32_t a_skill);

	private:
		RE::FormID   _lessonTrainer{ 0 };
		RE::ActorValue _lessonSkill{ RE::ActorValue::kNone };
		float        _lessonStart{ 0.0f };
	};
}
