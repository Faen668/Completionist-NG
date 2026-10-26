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

		enum class Place : std::uint8_t
		{
			kLoose,
			kContainer,
			kCarried,
		};

		struct Result
		{
			RE::ObjectRefHandle ref;	// item, or the container / NPC holding it
			RE::TESForm*        item{ nullptr };
			std::string         name;
			std::string         type;
			Place               place{ Place::kLoose };
			bool                stealing{ false };
		};

		// Game thread, closest first
		static std::vector<Result> Scan(RE::TESObjectCELL* a_cell, const std::vector<std::pair<RE::FormID, RE::FormID>>& a_tracked = {});

		static void ExcludeReference(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESObjectCELL* cell);
		static void RemoveExcludedReference(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref);

		static std::vector<RE::TESObjectREFR*> GetObjectReferences(RE::StaticFunctionTag*, RE::TESObjectCELL* cell);
		static std::vector<std::string> GetReferenceFormIDs(RE::StaticFunctionTag*, RE::TESObjectCELL* cell);
		static std::vector<std::string> GetReferenceNames(RE::StaticFunctionTag*, RE::TESObjectCELL* cell);

		static bool IsItemPinnable(RE::StaticFunctionTag*, RE::TESForm* a_form);

		static std::string	GetPinnedReferenceType(RE::StaticFunctionTag*, RE::TESForm* a_form);
		static std::string	GetPinnedReferenceName(RE::StaticFunctionTag*, RE::TESForm* a_form);
		static RE::TESObjectREFR* GetPinnedReferenceRefr(RE::StaticFunctionTag*, RE::TESForm* a_form);


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

		static void ScanCell(RE::TESObjectCELL* cell);
	};
}
