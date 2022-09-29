#pragma once

#include <SKSE\API.h>
#include "Serialization.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CFramework_VanillaPets {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_PetsOfSkyrim {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_SaintsSeducers {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_MiscellaneousPets {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Pets
{
	enum PatchID : std::int32_t
	{
		kPet_1 = 46,
		kPet_2 = 47,
		kPet_3 = 48,
		kPet_4 = 49,
	};

	extern std::vector<std::string> Pet_1_NameArray;
	extern std::vector<std::string> Pet_1_TextArray;
	extern std::vector<RE::TESForm*> Pet_1_FormArray;
	extern std::vector<bool> Pet_1_BoolArray;
	extern std::int32_t Pet_1_EntriesTotal;
	extern std::int32_t Pet_1_EntriesFound;

	extern std::vector<std::string> Pet_2_NameArray;
	extern std::vector<std::string> Pet_2_TextArray;
	extern std::vector<RE::TESForm*> Pet_2_FormArray;
	extern std::vector<bool> Pet_2_BoolArray;
	extern std::int32_t Pet_2_EntriesTotal;
	extern std::int32_t Pet_2_EntriesFound;

	extern std::vector<std::string> Pet_3_NameArray;
	extern std::vector<std::string> Pet_3_TextArray;
	extern std::vector<RE::TESForm*> Pet_3_FormArray;
	extern std::vector<bool> Pet_3_BoolArray;
	extern std::int32_t Pet_3_EntriesTotal;
	extern std::int32_t Pet_3_EntriesFound;

	extern std::vector<std::string> Pet_4_NameArray;
	extern std::vector<std::string> Pet_4_TextArray;
	extern std::vector<RE::TESForm*> Pet_4_FormArray;
	extern std::vector<bool> Pet_4_BoolArray;
	extern std::int32_t Pet_4_EntriesTotal;
	extern std::int32_t Pet_4_EntriesFound;

	class CHandler {

		using VM = RE::BSScript::Internal::VirtualMachine;
		using StackID = RE::VMStackID;
		using Severity = RE::BSScript::ErrorLogger::Severity;

	public:

		static void			InstallFramework();
		static void			UpdateFoundForms();
		static void			InjectAndCompileData();

		static void			Framework_UpdatePetOwnership(RE::StaticFunctionTag*, std::string a_name);

		inline static const std::vector<std::string> Null_S = {};
		inline static const std::vector<RE::TESForm*> Null_F = {};
		inline static const std::vector<bool> Null_B = {};

	public: [[nodiscard]] static std::int32_t ReturnEntriesInt(std::int32_t a_patchID, std::string a_section) {

		switch (a_patchID) {

		case kPet_1:
			if (a_section == "Total") { return Pet_1_EntriesTotal; }
			if (a_section == "Found") { return Pet_1_EntriesFound; }
			return -1;

		case kPet_2:
			if (a_section == "Total") { return Pet_2_EntriesTotal; }
			if (a_section == "Found") { return Pet_2_EntriesFound; }
			return -1;

		case kPet_3:
			if (a_section == "Total") { return Pet_3_EntriesTotal; }
			if (a_section == "Found") { return Pet_3_EntriesFound; }
			return -1;

		case kPet_4:
			if (a_section == "Total") { return Pet_4_EntriesTotal; }
			if (a_section == "Found") { return Pet_4_EntriesFound; }
			return -1;

		default:
			return -1;
		}
	}

	public: [[nodiscard]] static const std::vector<RE::TESForm*>& ReturnEntriesForm(std::int32_t a_patchID) {

		switch (a_patchID) {

		case kPet_1:
			return Pet_1_FormArray;

		case kPet_2:
			return Pet_2_FormArray;

		case kPet_3:
			return Pet_3_FormArray;

		case kPet_4:
			return Pet_4_FormArray;

		default:
			return Null_F;
		}
	}

	public: [[nodiscard]] static const std::vector<std::string>& ReturnEntriesString(std::int32_t a_patchID, std::string a_section) {

		switch (a_patchID) {

		case kPet_1:
			if (a_section == "Name") { return Pet_1_NameArray; }
			if (a_section == "Text") { return Pet_1_TextArray; }
			return Null_S;

		case kPet_2:
			if (a_section == "Name") { return Pet_2_NameArray; }
			if (a_section == "Text") { return Pet_2_TextArray; }
			return Null_S;

		case kPet_3:
			if (a_section == "Name") { return Pet_3_NameArray; }
			if (a_section == "Text") { return Pet_3_TextArray; }
			return Null_S;

		case kPet_4:
			if (a_section == "Name") { return Pet_4_NameArray; }
			if (a_section == "Text") { return Pet_4_TextArray; }
			return Null_S;

		default:
			return Null_S;
		}
	}

	public: [[nodiscard]] static const std::vector<bool>& ReturnEntriesBool(std::int32_t a_patchID) {

		switch (a_patchID) {

		case kPet_1:
			return Pet_1_BoolArray;

		case kPet_2:
			return Pet_2_BoolArray;

		case kPet_3:
			return Pet_3_BoolArray;

		case kPet_4:
			return Pet_4_BoolArray;

		default:
			return Null_B;
		}
	}

	public: [[nodiscard]] static std::int32_t IsOptionCompleted(std::int32_t a_patchID, std::string a_name) {

		switch (a_patchID) {

		case kPet_1:
			if (auto t_pos = std::ranges::find(Pet_1_NameArray, a_name); t_pos != Pet_1_NameArray.end()) {
				return std::int32_t(Pet_1_BoolArray[std::distance(Pet_1_NameArray.begin(), t_pos)]);
			}
			return -1;

		case kPet_2:
			if (auto t_pos = std::ranges::find(Pet_2_NameArray, a_name); t_pos != Pet_2_NameArray.end()) {
				return std::int32_t(Pet_2_BoolArray[std::distance(Pet_2_NameArray.begin(), t_pos)]);
			}
			return -1;

		case kPet_3:
			if (auto t_pos = std::ranges::find(Pet_3_NameArray, a_name); t_pos != Pet_3_NameArray.end()) {
				return std::int32_t(Pet_3_BoolArray[std::distance(Pet_3_NameArray.begin(), t_pos)]);
			}
			return -1;

		case kPet_4:
			if (auto t_pos = std::ranges::find(Pet_4_NameArray, a_name); t_pos != Pet_4_NameArray.end()) {
				return std::int32_t(Pet_4_BoolArray[std::distance(Pet_4_NameArray.begin(), t_pos)]);
			}
			return -1;

		default:
			return -1;
		}
	}

	public: static void SetOptionCompleted(std::int32_t a_patchID, std::string a_name) {
		using namespace CFramework_Master;

		switch (a_patchID) {

		case kPet_1:
			if (auto t_pos = std::ranges::find(Pet_1_NameArray, a_name); t_pos != Pet_1_NameArray.end()) {
				auto b_pos = std::distance(Pet_1_NameArray.begin(), t_pos);

				if (Pet_1_BoolArray.at(b_pos)) {
					Pet_1_BoolArray.at(b_pos) = false;

					FoundItemData.RemoveForm(Pet_1_FormArray.at(b_pos)->GetFormID());
					for (auto var : CFramework_VanillaPets::Data.GetAllVariations()) {
						if (CFramework_VanillaPets::Data.GetBase(var) == Pet_1_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.RemoveForm(var);
						}
					}
				}
				else {
					Pet_1_BoolArray.at(b_pos) = true;
					FoundItemData.AddForm(Pet_1_FormArray.at(b_pos)->GetFormID());
					for (auto var : CFramework_VanillaPets::Data.GetAllVariations()) {
						if (CFramework_VanillaPets::Data.GetBase(var) == Pet_1_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.AddForm(var);
						}
					}
				}

				Pet_1_EntriesTotal = Pet_1_FormArray.size();
				Pet_1_EntriesFound = std::ranges::count(Pet_1_BoolArray, true);
			}
			break;

		case kPet_2:
			if (auto t_pos = std::ranges::find(Pet_2_NameArray, a_name); t_pos != Pet_2_NameArray.end()) {
				auto b_pos = std::distance(Pet_2_NameArray.begin(), t_pos);

				if (Pet_2_BoolArray.at(b_pos)) {
					Pet_2_BoolArray.at(b_pos) = false;

					FoundItemData.RemoveForm(Pet_2_FormArray.at(b_pos)->GetFormID());
					for (auto var : CFramework_VanillaPets::Data.GetAllVariations()) {
						if (CFramework_VanillaPets::Data.GetBase(var) == Pet_2_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.RemoveForm(var);
						}
					}
				}
				else {
					Pet_2_BoolArray.at(b_pos) = true;
					FoundItemData.AddForm(Pet_2_FormArray.at(b_pos)->GetFormID());
					for (auto var : CFramework_VanillaPets::Data.GetAllVariations()) {
						if (CFramework_VanillaPets::Data.GetBase(var) == Pet_2_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.AddForm(var);
						}
					}
				}

				Pet_2_EntriesTotal = Pet_2_FormArray.size();
				Pet_2_EntriesFound = std::ranges::count(Pet_2_BoolArray, true);
			}
			break;

		case kPet_3:
			if (auto t_pos = std::ranges::find(Pet_3_NameArray, a_name); t_pos != Pet_3_NameArray.end()) {
				auto b_pos = std::distance(Pet_3_NameArray.begin(), t_pos);

				if (Pet_3_BoolArray.at(b_pos)) {
					Pet_3_BoolArray.at(b_pos) = false;

					FoundItemData.RemoveForm(Pet_3_FormArray.at(b_pos)->GetFormID());
					for (auto var : CFramework_VanillaPets::Data.GetAllVariations()) {
						if (CFramework_VanillaPets::Data.GetBase(var) == Pet_3_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.RemoveForm(var);
						}
					}
				}
				else {
					Pet_3_BoolArray.at(b_pos) = true;
					FoundItemData.AddForm(Pet_3_FormArray.at(b_pos)->GetFormID());
					for (auto var : CFramework_VanillaPets::Data.GetAllVariations()) {
						if (CFramework_VanillaPets::Data.GetBase(var) == Pet_3_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.AddForm(var);
						}
					}
				}

				Pet_3_EntriesTotal = Pet_3_FormArray.size();
				Pet_3_EntriesFound = std::ranges::count(Pet_3_BoolArray, true);
			}
			break;

		case kPet_4:
			if (auto t_pos = std::ranges::find(Pet_4_NameArray, a_name); t_pos != Pet_4_NameArray.end()) {
				auto b_pos = std::distance(Pet_4_NameArray.begin(), t_pos);

				if (Pet_4_BoolArray.at(b_pos)) {
					Pet_4_BoolArray.at(b_pos) = false;

					FoundItemData.RemoveForm(Pet_4_FormArray.at(b_pos)->GetFormID());
					for (auto var : CFramework_VanillaPets::Data.GetAllVariations()) {
						if (CFramework_VanillaPets::Data.GetBase(var) == Pet_4_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.RemoveForm(var);
						}
					}
				}
				else {
					Pet_4_BoolArray.at(b_pos) = true;
					FoundItemData.AddForm(Pet_4_FormArray.at(b_pos)->GetFormID());
					for (auto var : CFramework_VanillaPets::Data.GetAllVariations()) {
						if (CFramework_VanillaPets::Data.GetBase(var) == Pet_4_FormArray.at(b_pos)->GetFormID()) {
							FoundItemData.AddForm(var);
						}
					}
				}

				Pet_4_EntriesTotal = Pet_4_FormArray.size();
				Pet_4_EntriesFound = std::ranges::count(Pet_4_BoolArray, true);
			}
			break;

		default:
			break;
		}
	}
	};
}