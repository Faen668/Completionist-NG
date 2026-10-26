#include "FormUtil.hpp"

auto FormUtil::GetIdentifierFromForm(RE::TESForm* a_form) -> std::string
{
	if (!a_form) {
		return ""s;
	}

	auto file = a_form->GetFile();
	auto plugin = file ? file->fileName : "";

	RE::FormID formID = a_form->GetFormID();
	RE::FormID relativeID = formID & 0x00FFFFFF;

	std::ostringstream ss;
	ss << plugin << "|" << std::hex << relativeID;
	return ss.str();
}

auto FormUtil::GetIdentifierFromForm(RE::BGSRefAlias* a_form) -> std::string
{
	if (!a_form || !a_form->owningQuest) {
		return ""s;
	}

	auto file = a_form->owningQuest->GetFile();
	auto plugin = file ? file->fileName : "";

	RE::FormID formID = a_form->owningQuest->GetFormID();
	RE::FormID relativeID = formID & 0x00FFFFFF;

	std::ostringstream ss;
	ss << plugin << "|" << std::hex << relativeID;
	return ss.str();
}
