#pragma once

#include <SKSE\API.h>
#include "Serialization.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CPatch_Fishing_F {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_Fishing_I {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_Fishing_B {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_Fishing_A {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_Fishing_C {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_Fishing_L {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_Fishing_S {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_Fishing
{

	enum PatchID : std::int32_t
	{
		ksection_F = 245,
		ksection_I = 246,
		ksection_B = 247,
		ksection_A = 248,
		ksection_C = 249,
		ksection_L = 250,
		ksection_S = 251,
	};

	extern std::vector<std::string>		F_NameArray;
	extern std::vector<std::string>		F_TextArray;
	extern std::vector<RE::TESForm*>	F_FormArray;
	extern std::vector<bool>			F_BoolArray;
	extern std::int32_t					F_EntriesTotal;
	extern std::int32_t					F_EntriesFound;

	extern std::vector<std::string>		I_NameArray;
	extern std::vector<std::string>		I_TextArray;
	extern std::vector<RE::TESForm*>	I_FormArray;
	extern std::vector<bool>			I_BoolArray;
	extern std::int32_t					I_EntriesTotal;
	extern std::int32_t					I_EntriesFound;

	extern std::vector<std::string>		B_NameArray;
	extern std::vector<std::string>		B_TextArray;
	extern std::vector<RE::TESForm*>	B_FormArray;
	extern std::vector<bool>			B_BoolArray;
	extern std::int32_t					B_EntriesTotal;
	extern std::int32_t					B_EntriesFound;

	extern std::vector<std::string>		A_NameArray;
	extern std::vector<std::string>		A_TextArray;
	extern std::vector<RE::TESForm*>	A_FormArray;
	extern std::vector<bool>			A_BoolArray;
	extern std::int32_t					A_EntriesTotal;
	extern std::int32_t					A_EntriesFound;

	extern std::vector<std::string>		C_NameArray;
	extern std::vector<std::string>		C_TextArray;
	extern std::vector<RE::TESForm*>	C_FormArray;
	extern std::vector<bool>			C_BoolArray;
	extern std::int32_t					C_EntriesTotal;
	extern std::int32_t					C_EntriesFound;

	extern std::vector<std::string>		L_NameArray;
	extern std::vector<std::string>		L_TextArray;
	extern std::vector<RE::TESForm*>	L_FormArray;
	extern std::vector<bool>			L_BoolArray;
	extern std::int32_t					L_EntriesTotal;
	extern std::int32_t					L_EntriesFound;

	extern std::vector<std::string>		S_NameArray;
	extern std::vector<std::string>		S_TextArray;
	extern std::vector<RE::TESForm*>	S_FormArray;
	extern std::vector<bool>			S_BoolArray;
	extern std::int32_t					S_EntriesTotal;
	extern std::int32_t					S_EntriesFound;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :
		public RE::BSTEventSink<RE::TESContainerChangedEvent>,
		public RE::BSTEventSink<RE::BooksRead::Event>,
		public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
		public RE::BSTEventSink<RE::TESEquipEvent> {

		public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult			ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;
		  EventResult			ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>*) override;
		  EventResult			ProcessEvent(const RE::BooksRead::Event* a_event, RE::BSTEventSource<RE::BooksRead::Event>*) override;
		  EventResult			ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override;

		  static void			InstallFramework();
		  static void			UpdateFoundForms();
		  static void			SinkEvents();

		  static void			ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, PatchID a_patchID);
		  static bool			ProcessMapMarker(RE::TESForm* a_form);

		  static void			InjectAndCompileData();;
		  static void			BuildFishArrays(RE::TESForm* a_form, std::string a_rod, std::string a_loc);

		  inline static const std::vector<std::string> Null_S = {};
		  inline static const std::vector<RE::TESForm*> Null_F = {};
		  inline static const std::vector<bool> Null_B = {};

	public: [[nodiscard]] static std::int32_t ReturnEntriesInt(std::int32_t a_patchID, std::string a_section) {

		switch (a_patchID) {

		case ksection_F:
			if (a_section == "Total") { return F_EntriesTotal; }
			if (a_section == "Found") { return F_EntriesFound; }
			return -1;

		case ksection_I:
			if (a_section == "Total") { return I_EntriesTotal; }
			if (a_section == "Found") { return I_EntriesFound; }
			return -1;

		case ksection_B:
			if (a_section == "Total") { return B_EntriesTotal; }
			if (a_section == "Found") { return B_EntriesFound; }
			return -1;

		case ksection_A:
			if (a_section == "Total") { return A_EntriesTotal; }
			if (a_section == "Found") { return A_EntriesFound; }
			return -1;

		case ksection_C:
			if (a_section == "Total") { return C_EntriesTotal; }
			if (a_section == "Found") { return C_EntriesFound; }
			return -1;

		case ksection_L:
			if (a_section == "Total") { return L_EntriesTotal; }
			if (a_section == "Found") { return L_EntriesFound; }
			return -1;

		case ksection_S:
			if (a_section == "Total") { return S_EntriesTotal; }
			if (a_section == "Found") { return S_EntriesFound; }
			return -1;

		default:
			return -1;
		}
	}

	public: [[nodiscard]] static const std::vector<std::string>& ReturnEntriesString(std::int32_t a_patchID, std::string a_section) {

		switch (a_patchID) {

		case ksection_F:
			if (a_section == "Name") { return F_NameArray; }
			if (a_section == "Text") { return F_TextArray; }
			return Null_S;

		case ksection_I:
			if (a_section == "Name") { return I_NameArray; }
			if (a_section == "Text") { return I_TextArray; }
			return Null_S;

		case ksection_B:
			if (a_section == "Name") { return B_NameArray; }
			if (a_section == "Text") { return B_TextArray; }
			return Null_S;

		case ksection_A:
			if (a_section == "Name") { return A_NameArray; }
			if (a_section == "Text") { return A_TextArray; }
			return Null_S;

		case ksection_C:
			if (a_section == "Name") { return C_NameArray; }
			if (a_section == "Text") { return C_TextArray; }
			return Null_S;

		case ksection_L:
			if (a_section == "Name") { return L_NameArray; }
			if (a_section == "Text") { return L_TextArray; }
			return Null_S;

		case ksection_S:
			if (a_section == "Name") { return S_NameArray; }
			if (a_section == "Text") { return S_TextArray; }
			return Null_S;

		default:
			return Null_S;
		}
	}

	public: [[nodiscard]] static const std::vector<RE::TESForm*>& ReturnEntriesForm(std::int32_t a_patchID) {

		switch (a_patchID) {

		case ksection_F:
			return F_FormArray;

		case ksection_I:
			return I_FormArray;

		case ksection_B:
			return B_FormArray;

		case ksection_A:
			return A_FormArray;

		case ksection_C:
			return C_FormArray;

		case ksection_L:
			return L_FormArray;

		case ksection_S:
			return S_FormArray;

		default:
			return Null_F;
		}
	}

	public: [[nodiscard]] static const std::vector<bool>& ReturnEntriesBool(std::int32_t a_patchID) {

		switch (a_patchID) {

		case ksection_F:
			return F_BoolArray;

		case ksection_I:
			return I_BoolArray;

		case ksection_B:
			return B_BoolArray;

		case ksection_A:
			return A_BoolArray;

		case ksection_C:
			return C_BoolArray;

		case ksection_L:
			return L_BoolArray;

		case ksection_S:
			return S_BoolArray;

		default:
			return Null_B;
		}
	}

	public: [[nodiscard]] static std::int32_t IsOptionCompleted(std::int32_t a_patchID, std::string a_name) {

		switch (a_patchID) {

		case ksection_F:
			if (auto t_pos = std::ranges::find(F_NameArray, a_name); t_pos != F_NameArray.end()) {
				return std::int32_t(F_BoolArray[std::distance(F_NameArray.begin(), t_pos)]);
			}
			return -1;

		case ksection_I:
			if (auto t_pos = std::ranges::find(I_NameArray, a_name); t_pos != I_NameArray.end()) {
				return std::int32_t(I_BoolArray[std::distance(I_NameArray.begin(), t_pos)]);
			}
			return -1;

		case ksection_B:
			if (auto t_pos = std::ranges::find(B_NameArray, a_name); t_pos != B_NameArray.end()) {
				return std::int32_t(B_BoolArray[std::distance(B_NameArray.begin(), t_pos)]);
			}
			return -1;

		default:
			return -1;
		}
	}

	public: static void SetOptionCompleted(std::int32_t a_patchID, std::string a_name) {
		using namespace CFramework_Master;

		switch (a_patchID) {

		case ksection_F:
			if (auto t_pos = std::ranges::find(F_NameArray, a_name); t_pos != F_NameArray.end()) {
				auto b_pos = std::distance(F_NameArray.begin(), t_pos);

				if (F_BoolArray.at(b_pos)) {
					F_BoolArray.at(b_pos) = false;

					FoundItemData.RemoveForm(F_FormArray.at(b_pos)->GetFormID());
					for (auto var : CPatch_Fishing_F::Data.GetAllVariations()) {
						if (CPatch_Fishing_F::Data.GetBase(var) == F_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.RemoveForm(var);
						}
					}
				}
				else {
					F_BoolArray.at(b_pos) = true;
					FoundItemData.AddForm(F_FormArray.at(b_pos)->GetFormID());
					for (auto var : CPatch_Fishing_F::Data.GetAllVariations()) {
						if (CPatch_Fishing_F::Data.GetBase(var) == F_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.AddForm(var);
						}
					}
				}

				F_EntriesTotal = F_FormArray.size();
				F_EntriesFound = std::ranges::count(F_BoolArray, true);
			}
			break;

		case ksection_I:
			if (auto t_pos = std::ranges::find(I_NameArray, a_name); t_pos != I_NameArray.end()) {
				auto b_pos = std::distance(I_NameArray.begin(), t_pos);

				if (I_BoolArray.at(b_pos)) {
					I_BoolArray.at(b_pos) = false;

					FoundItemData.RemoveForm(I_FormArray.at(b_pos)->GetFormID());
					for (auto var : CPatch_Fishing_I::Data.GetAllVariations()) {
						if (CPatch_Fishing_I::Data.GetBase(var) == I_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.RemoveForm(var);
						}
					}
				}
				else {
					I_BoolArray.at(b_pos) = true;
					FoundItemData.AddForm(I_FormArray.at(b_pos)->GetFormID());
					for (auto var : CPatch_Fishing_I::Data.GetAllVariations()) {
						if (CPatch_Fishing_I::Data.GetBase(var) == I_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.AddForm(var);
						}
					}
				}

				I_EntriesTotal = I_FormArray.size();
				I_EntriesFound = std::ranges::count(I_BoolArray, true);
			}
			break;

		case ksection_B:
			if (auto t_pos = std::ranges::find(B_NameArray, a_name); t_pos != B_NameArray.end()) {
				auto b_pos = std::distance(B_NameArray.begin(), t_pos);

				if (B_BoolArray.at(b_pos)) {
					B_BoolArray.at(b_pos) = false;

					FoundItemData.RemoveForm(B_FormArray.at(b_pos)->GetFormID());
					for (auto var : CPatch_Fishing_B::Data.GetAllVariations()) {
						if (CPatch_Fishing_B::Data.GetBase(var) == B_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.RemoveForm(var);
						}
					}
				}
				else {
					B_BoolArray.at(b_pos) = true;
					FoundItemData.AddForm(B_FormArray.at(b_pos)->GetFormID());
					for (auto var : CPatch_Fishing_B::Data.GetAllVariations()) {
						if (CPatch_Fishing_B::Data.GetBase(var) == B_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.AddForm(var);
						}
					}
				}

				B_EntriesTotal = B_FormArray.size();
				B_EntriesFound = std::ranges::count(B_BoolArray, true);
			}
			break;

		default:
			break;
		}
	}
	};

}