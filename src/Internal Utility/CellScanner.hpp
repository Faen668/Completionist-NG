#pragma once

namespace Completionist
{
	class CellScanner
	{
	public:
		CellScanner(CellScanner const&) = delete;
		CellScanner(CellScanner const&&) = delete;
		CellScanner operator=(CellScanner&) = delete;
		CellScanner operator=(CellScanner&&) = delete;

		static void init();

		static void CheckForReferences(RE::StaticFunctionTag*, RE::TESObjectCELL* cell, bool b_logging, bool b_notify);

		static std::string GetQuestMarkerReferenceFormID(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref);
		static std::string GetQuestMarkerReferenceOwner(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref);
		static std::string GetQuestMarkerReferenceIndex(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref);
		static void ExcludeReference(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESObjectCELL* cell);
		static void RemoveExcludedReference(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref);

		static std::vector<RE::TESObjectREFR*> GetObjectReferences(RE::StaticFunctionTag*, RE::TESObjectCELL* cell);
		static std::vector<std::string> GetReferenceFormIDs(RE::StaticFunctionTag*, RE::TESObjectCELL* cell);
		static std::vector<std::string> GetReferenceNames(RE::StaticFunctionTag*, RE::TESObjectCELL* cell);

		static bool HasPinnedFormInCell(RE::StaticFunctionTag*, RE::TESObjectCELL* cell, RE::TESForm* a_form);
		static bool IsItemPinnable(RE::StaticFunctionTag*, RE::TESForm* a_form);

		static std::string	GetPinnedReferenceType(RE::StaticFunctionTag*, RE::TESForm* a_form);
		static std::string	GetPinnedReferenceName(RE::StaticFunctionTag*, RE::TESForm* a_form);
		static RE::TESObjectREFR* GetPinnedReferenceRefr(RE::StaticFunctionTag*, RE::TESForm* a_form);

		static RE::TESForm* GetTargetReferenceForm(RE::StaticFunctionTag*);
		static std::string	GetTargetReferenceType(RE::StaticFunctionTag*);
		static std::string	GetTargetReferenceName(RE::StaticFunctionTag*);
		static RE::TESObjectREFR* GetTargetReferenceRefr(RE::StaticFunctionTag*, RE::TESObjectCELL* a_cell, RE::TESForm* a_lastForm);

		static bool isCellExcluded(RE::StaticFunctionTag*, RE::TESObjectCELL* cell);
		static bool IsModAddedReferenceExcluded(RE::FormID a_formID);

		static void ExcludeAllVendorChests();
		
	private:
		CellScanner() = default;
		~CellScanner() = default;

		static bool ItemIsCollectable(RE::TESForm* a_form);
		static bool ItemIsCollected(RE::TESForm* a_form);

		static std::string GetFormType(RE::TESForm* a_form);

		static bool IsExcludedFormType(RE::TESForm* a_form);
		static bool IsReferenceExcluded(RE::TESObjectREFR* a_chest);

		static void ScanCell(RE::TESObjectCELL* cell, bool b_logging, bool b_notify);

		static int GetRandomIndex(const int32_t size)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dist(0, (size - 1));
			return dist(gen);
		}
	};
}
