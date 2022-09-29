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

	inline static const std::vector<std::string> Null_S = {};
	inline static const std::vector<RE::TESForm*> Null_F = {};
	inline static const std::vector<bool> Null_B = {};

	public: [[nodiscard]] static std::int32_t ReturnEntriesInt(std::int32_t a_patchID, std::string a_section) {

		switch (a_patchID) {

		case kItems:
			if (a_section == "Total") { return Items_EntriesTotal; }
			if (a_section == "Found") { return Items_EntriesFound; }
			return -1;

		default:
			return -1;
		}
	}

	public: [[nodiscard]] static const std::vector<RE::TESForm*>& ReturnEntriesForm(std::int32_t a_patchID) {

		switch (a_patchID) {

		case kItems:
			return Items_FormArray;

		default:
			return Null_F;
		}
	}

	public: [[nodiscard]] static std::vector<std::string> ReturnEntriesString(std::int32_t a_patchID, std::string a_section) {

		switch (a_patchID) {

		case kItems:
			if (a_section == "Name") { return Items_NameArray; }
			if (a_section == "Text") { return Items_TextArray; }
			return Null_S;

		default:
			return Null_S;
		}
	}

	public: [[nodiscard]] static std::vector<bool> ReturnEntriesBool(std::int32_t a_patchID) {

		switch (a_patchID) {

		case kItems:
			return Items_BoolArray;

		default:
			return Null_B;
		}
	}

	public: [[nodiscard]] static std::int32_t IsOptionCompleted(std::int32_t a_patchID, std::string a_name) {

		switch (a_patchID) {

		case kItems:
			if (auto t_pos = std::ranges::find(Items_NameArray, a_name); t_pos != Items_NameArray.end()) {
				return std::int32_t(Items_BoolArray[std::distance(Items_NameArray.begin(), t_pos)]);
			}
			return -1;

		default:
			return -1;
		}
	}
 
	public: static void SetOptionCompleted(std::int32_t a_patchID, std::string a_name) {
		using namespace CFramework_Master;

		switch (a_patchID) {

		case kItems:
			if (auto t_pos = std::ranges::find(Items_NameArray, a_name); t_pos != Items_NameArray.end()) {
				auto b_pos = std::distance(Items_NameArray.begin(), t_pos);

				if (Items_BoolArray.at(b_pos)) {
					Items_BoolArray.at(b_pos) = false;

					FoundItemData.RemoveForm(Items_FormArray.at(b_pos)->GetFormID());
					for (auto var : CPatch_AHD_Items::Data.GetAllVariations()) {
						if (CPatch_AHD_Items::Data.GetBase(var) == Items_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.RemoveForm(var);
						}
					}
				}
				else {
					Items_BoolArray.at(b_pos) = true;
					FoundItemData.AddForm(Items_FormArray.at(b_pos)->GetFormID());
					for (auto var : CPatch_AHD_Items::Data.GetAllVariations()) {
						if (CPatch_AHD_Items::Data.GetBase(var) == Items_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.AddForm(var);
						}
					}
				}

				Items_EntriesTotal = Items_FormArray.size();
				Items_EntriesFound = std::ranges::count(Items_BoolArray, true);
			}
			break;

		default:
			break;
		}
	}
	};
}