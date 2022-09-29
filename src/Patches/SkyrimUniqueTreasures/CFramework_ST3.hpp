#pragma once

#include <SKSE\API.h>
#include "Serialization.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CPatch_ST3_ItmL1 {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_ST3_ItmL2 {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_ST3_ItmL3 {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_ST3_ItmL4 {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_ST3
{
	enum PatchID : std::int32_t
	{
		kItmL1 = 241,
		kItmL2 = 242,
		kItmL3 = 243,
		kItmL4 = 244,
	};

	extern std::vector<std::string> ItmL1_NameArray;
	extern std::vector<std::string> ItmL1_TextArray;
	extern std::vector<RE::TESForm*> ItmL1_FormArray;
	extern std::vector<bool> ItmL1_BoolArray;
	extern std::int32_t ItmL1_EntriesTotal;
	extern std::int32_t ItmL1_EntriesFound;

	extern std::vector<std::string> ItmL2_NameArray;
	extern std::vector<std::string> ItmL2_TextArray;
	extern std::vector<RE::TESForm*> ItmL2_FormArray;
	extern std::vector<bool> ItmL2_BoolArray;
	extern std::int32_t ItmL2_EntriesTotal;
	extern std::int32_t ItmL2_EntriesFound;

	extern std::vector<std::string> ItmL3_NameArray;
	extern std::vector<std::string> ItmL3_TextArray;
	extern std::vector<RE::TESForm*> ItmL3_FormArray;
	extern std::vector<bool> ItmL3_BoolArray;
	extern std::int32_t ItmL3_EntriesTotal;
	extern std::int32_t ItmL3_EntriesFound;

	extern std::vector<std::string> ItmL4_NameArray;
	extern std::vector<std::string> ItmL4_TextArray;
	extern std::vector<RE::TESForm*> ItmL4_FormArray;
	extern std::vector<bool> ItmL4_BoolArray;
	extern std::int32_t ItmL4_EntriesTotal;
	extern std::int32_t ItmL4_EntriesFound;

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

		case kItmL1:
			if (a_section == "Total") { return ItmL1_EntriesTotal; }
			if (a_section == "Found") { return ItmL1_EntriesFound; }
			return -1;

		case kItmL2:
			if (a_section == "Total") { return ItmL2_EntriesTotal; }
			if (a_section == "Found") { return ItmL2_EntriesFound; }
			return -1;

		case kItmL3:
			if (a_section == "Total") { return ItmL3_EntriesTotal; }
			if (a_section == "Found") { return ItmL3_EntriesFound; }
			return -1;

		case kItmL4:
			if (a_section == "Total") { return ItmL4_EntriesTotal; }
			if (a_section == "Found") { return ItmL4_EntriesFound; }
			return -1;

		default:
			return -1;
		}
	}

	public: [[nodiscard]] static const std::vector<RE::TESForm*>& ReturnEntriesForm(std::int32_t a_patchID) {

		switch (a_patchID) {

		case kItmL1:
			return ItmL1_FormArray;

		case kItmL2:
			return ItmL2_FormArray;

		case kItmL3:
			return ItmL3_FormArray;

		case kItmL4:
			return ItmL4_FormArray;

		default:
			return Null_F;
		}
	}

	public: [[nodiscard]] static const std::vector<std::string>& ReturnEntriesString(std::int32_t a_patchID, std::string a_section) {

		switch (a_patchID) {

		case kItmL1:
			if (a_section == "Name") { return ItmL1_NameArray; }
			if (a_section == "Text") { return ItmL1_TextArray; }
			return Null_S;

		case kItmL2:
			if (a_section == "Name") { return ItmL2_NameArray; }
			if (a_section == "Text") { return ItmL2_TextArray; }
			return Null_S;

		case kItmL3:
			if (a_section == "Name") { return ItmL3_NameArray; }
			if (a_section == "Text") { return ItmL3_TextArray; }
			return Null_S;

		case kItmL4:
			if (a_section == "Name") { return ItmL4_NameArray; }
			if (a_section == "Text") { return ItmL4_TextArray; }
			return Null_S;


		default:
			return Null_S;
		}
	}

	public: [[nodiscard]] static const std::vector<bool>& ReturnEntriesBool(std::int32_t a_patchID) {

		switch (a_patchID) {

		case kItmL1:
			return ItmL1_BoolArray;

		case kItmL2:
			return ItmL2_BoolArray;

		case kItmL3:
			return ItmL3_BoolArray;

		case kItmL4:
			return ItmL4_BoolArray;

		default:
			return Null_B;
		}
	}

	public: [[nodiscard]] static std::int32_t IsOptionCompleted(std::int32_t a_patchID, std::string a_name) {

		switch (a_patchID) {

		case kItmL1:
			if (auto t_pos = std::ranges::find(ItmL1_NameArray, a_name); t_pos != ItmL1_NameArray.end()) {
				return std::int32_t(ItmL1_BoolArray[std::distance(ItmL1_NameArray.begin(), t_pos)]);
			}
			return -1;

		case kItmL2:
			if (auto t_pos = std::ranges::find(ItmL2_NameArray, a_name); t_pos != ItmL2_NameArray.end()) {
				return std::int32_t(ItmL2_BoolArray[std::distance(ItmL2_NameArray.begin(), t_pos)]);
			}
			return -1;

		case kItmL3:
			if (auto t_pos = std::ranges::find(ItmL3_NameArray, a_name); t_pos != ItmL3_NameArray.end()) {
				return std::int32_t(ItmL3_BoolArray[std::distance(ItmL3_NameArray.begin(), t_pos)]);
			}
			return -1;

		case kItmL4:
			if (auto t_pos = std::ranges::find(ItmL4_NameArray, a_name); t_pos != ItmL4_NameArray.end()) {
				return std::int32_t(ItmL4_BoolArray[std::distance(ItmL4_NameArray.begin(), t_pos)]);
			}
			return -1;

		default:
			return -1;
		}
	}

	public: static void SetOptionCompleted(std::int32_t a_patchID, std::string a_name) {
		using namespace CFramework_Master;

		switch (a_patchID) {

		case kItmL1:
			if (auto t_pos = std::ranges::find(ItmL1_NameArray, a_name); t_pos != ItmL1_NameArray.end()) {
				auto b_pos = std::distance(ItmL1_NameArray.begin(), t_pos);

				if (ItmL1_BoolArray.at(b_pos)) {
					ItmL1_BoolArray.at(b_pos) = false;

					FoundItemData.RemoveForm(ItmL1_FormArray.at(b_pos)->GetFormID());
					for (auto var : CPatch_ST3_ItmL1::Data.GetAllVariations()) {
						if (CPatch_ST3_ItmL1::Data.GetBase(var) == ItmL1_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.RemoveForm(var);
						}
					}
				}
				else {
					ItmL1_BoolArray.at(b_pos) = true;
					FoundItemData.AddForm(ItmL1_FormArray.at(b_pos)->GetFormID());
					for (auto var : CPatch_ST3_ItmL1::Data.GetAllVariations()) {
						if (CPatch_ST3_ItmL1::Data.GetBase(var) == ItmL1_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.AddForm(var);
						}
					}
				}

				ItmL1_EntriesTotal = ItmL1_FormArray.size();
				ItmL1_EntriesFound = std::ranges::count(ItmL1_BoolArray, true);
			}
			break;

		case kItmL2:
			if (auto t_pos = std::ranges::find(ItmL2_NameArray, a_name); t_pos != ItmL2_NameArray.end()) {
				auto b_pos = std::distance(ItmL2_NameArray.begin(), t_pos);

				if (ItmL2_BoolArray.at(b_pos)) {
					ItmL2_BoolArray.at(b_pos) = false;

					FoundItemData.RemoveForm(ItmL2_FormArray.at(b_pos)->GetFormID());
					for (auto var : CPatch_ST3_ItmL2::Data.GetAllVariations()) {
						if (CPatch_ST3_ItmL2::Data.GetBase(var) == ItmL2_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.RemoveForm(var);
						}
					}
				}
				else {
					ItmL2_BoolArray.at(b_pos) = true;
					FoundItemData.AddForm(ItmL2_FormArray.at(b_pos)->GetFormID());
					for (auto var : CPatch_ST3_ItmL2::Data.GetAllVariations()) {
						if (CPatch_ST3_ItmL2::Data.GetBase(var) == ItmL2_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.AddForm(var);
						}
					}
				}

				ItmL2_EntriesTotal = ItmL2_FormArray.size();
				ItmL2_EntriesFound = std::ranges::count(ItmL2_BoolArray, true);
			}
			break;

		case kItmL3:
			if (auto t_pos = std::ranges::find(ItmL3_NameArray, a_name); t_pos != ItmL3_NameArray.end()) {
				auto b_pos = std::distance(ItmL3_NameArray.begin(), t_pos);

				if (ItmL3_BoolArray.at(b_pos)) {
					ItmL3_BoolArray.at(b_pos) = false;

					FoundItemData.RemoveForm(ItmL3_FormArray.at(b_pos)->GetFormID());
					for (auto var : CPatch_ST3_ItmL3::Data.GetAllVariations()) {
						if (CPatch_ST3_ItmL3::Data.GetBase(var) == ItmL3_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.RemoveForm(var);
						}
					}
				}
				else {
					ItmL3_BoolArray.at(b_pos) = true;
					FoundItemData.AddForm(ItmL3_FormArray.at(b_pos)->GetFormID());
					for (auto var : CPatch_ST3_ItmL3::Data.GetAllVariations()) {
						if (CPatch_ST3_ItmL3::Data.GetBase(var) == ItmL3_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.AddForm(var);
						}
					}
				}

				ItmL3_EntriesTotal = ItmL3_FormArray.size();
				ItmL3_EntriesFound = std::ranges::count(ItmL3_BoolArray, true);
			}
			break;

		case kItmL4:
			if (auto t_pos = std::ranges::find(ItmL4_NameArray, a_name); t_pos != ItmL4_NameArray.end()) {
				auto b_pos = std::distance(ItmL4_NameArray.begin(), t_pos);

				if (ItmL4_BoolArray.at(b_pos)) {
					ItmL4_BoolArray.at(b_pos) = false;

					FoundItemData.RemoveForm(ItmL4_FormArray.at(b_pos)->GetFormID());
					for (auto var : CPatch_ST3_ItmL4::Data.GetAllVariations()) {
						if (CPatch_ST3_ItmL4::Data.GetBase(var) == ItmL4_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.RemoveForm(var);
						}
					}
				}
				else {
					ItmL4_BoolArray.at(b_pos) = true;
					FoundItemData.AddForm(ItmL4_FormArray.at(b_pos)->GetFormID());
					for (auto var : CPatch_ST3_ItmL4::Data.GetAllVariations()) {
						if (CPatch_ST3_ItmL4::Data.GetBase(var) == ItmL4_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.AddForm(var);
						}
					}
				}

				ItmL4_EntriesTotal = ItmL4_FormArray.size();
				ItmL4_EntriesFound = std::ranges::count(ItmL4_BoolArray, true);
			}
			break;

		default:
			break;
		}
	}
	};
}