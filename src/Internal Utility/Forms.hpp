#pragma once

namespace CForms
{
	// Completionist.esp records by form ID
	inline constexpr std::string_view plugin = "Completionist.esp";

	// Tracker slots are made from this template
	inline constexpr RE::FormID   TrackerTemplate = 0x802;  // Completionist_Tracker
	inline constexpr std::int32_t TrackerSlots = 32;
	inline constexpr RE::FormID ExcludedCells = 0x821;    // Completionist_ExcludedCells (FLST)

	// The old single location marker, only hidden on older saves
	inline constexpr RE::FormID OldMarkerQuest = 0x8AB;  // Completionist_Marker

	// Index 0 is no sound
	inline constexpr RE::FormID PinnedItemShader = 0x810;  // Completionist_PinnedItemShader (EFSH)
	inline constexpr RE::FormID PinnedItemSounds = 0x838;  // Completionist_PinnedItemSoundList (FLST)

	template <class T>
	T* Get(RE::FormID a_id)
	{
		auto* handler = RE::TESDataHandler::GetSingleton();
		return handler ? handler->LookupForm<T>(a_id, plugin) : nullptr;
	}

	inline RE::BSScript::IVirtualMachine* VM()
	{
		const auto* skyrimVM = RE::SkyrimVM::GetSingleton();
		return skyrimVM ? skyrimVM->GetVMRuntimeData().impl.get() : nullptr;
	}

	inline RE::VMHandle HandleFor(const RE::TESForm* a_form)
	{
		auto* vm = VM();
		return vm && a_form ? vm->GetObjectHandlePolicy()->GetHandleForObject(a_form->GetFormType(), a_form) : 0;
	}

	inline RE::VMHandle HandleFor(const RE::BGSRefAlias* a_alias)
	{
		auto* vm = VM();
		return vm && a_alias ? vm->GetObjectHandlePolicy()->GetHandleForObject(RE::BGSRefAlias::VMTYPEID, a_alias) : 0;
	}

	inline ScriptObjectPtr AttachedScript(RE::VMHandle a_handle, const char* a_script)
	{
		ScriptObjectPtr object;
		if (auto* vm = VM(); vm && a_handle) {
			vm->FindBoundObject(a_handle, a_script, object);
		}
		return object;
	}

	inline ScriptObjectPtr NativeObject(RE::VMHandle a_handle, const char* a_type)
	{
		auto*           vm = VM();
		ScriptObjectPtr object;
		if (!vm || !a_handle) {
			return object;
		}
		if (!vm->FindBoundObject(a_handle, a_type, object) && vm->CreateObject(a_type, object)) {
			vm->GetObjectBindPolicy()->BindObject(object, a_handle);
		}
		return object;
	}

	template <class... Args>
	void Call(ScriptObjectPtr a_object, std::string_view a_function, Args... a_args)
	{
		auto* vm = VM();
		if (!vm || !a_object) {
			return;
		}
		auto*             args = RE::MakeFunctionArguments(std::move(a_args)...);
		ScriptCallbackPtr callback;
		vm->DispatchMethodCall(a_object, a_function, args, callback);
		delete args;
	}

	inline void ShowObjective(RE::FormID a_quest, bool a_displayed, bool a_force, std::int32_t a_index = 10)
	{
		Call(NativeObject(HandleFor(Get<RE::TESQuest>(a_quest)), "Quest"), "SetObjectiveDisplayed"sv, a_index, a_displayed, a_force);
	}

}
