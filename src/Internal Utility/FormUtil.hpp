#pragma once

namespace FormUtil
{

	auto GetIdentifierFromForm(RE::TESForm* a_form)->std::string;
	auto GetIdentifierFromForm(RE::BGSRefAlias* a_form)->std::string;

}
