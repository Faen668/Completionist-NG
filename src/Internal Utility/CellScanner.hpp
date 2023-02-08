#pragma once

namespace CellScanner
{
	class CHandler
	{
	public:
		static void CheckForReferences(RE::StaticFunctionTag*, RE::TESObjectCELL* cell, bool a_logging);

	private:
		static bool ItemIsCollectable(RE::TESForm* a_form);
		static bool ItemIsCollected(RE::TESForm* a_form);
	};
}
