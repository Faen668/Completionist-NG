#pragma once

#include <vector>

// Temporary references only exist while their cell is loaded. These stand in for them
// with the base object and form ID read from the plugin files, so patches can list them
// without a plugin that makes them persistent
namespace PlacedReferences
{
	// Reads the references the game hasn't loaded from the files that hold them
	void Resolve(const std::vector<RE::FormID>& a_formIDs);

	// The stand-in for this reference, if it has one
	RE::TESForm* Find(RE::FormID a_formID);

	// The stand-in, or the form the game has
	RE::TESForm* Lookup(RE::FormID a_formID);

	// The name of the place its cell belongs to, as the plugins have it
	std::string PlaceName(RE::FormID a_formID);

	// Whether a stand-in has somewhere the tracker can point to
	bool Trackable(RE::FormID a_formID);

	// What the tracker points to: the reference once its cell is loaded, before that the
	// nearest map marker outside, or the marker of the place an interior belongs to
	RE::TESObjectREFR* Target(RE::FormID a_formID);
}
