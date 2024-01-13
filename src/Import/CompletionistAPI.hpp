#pragma once

struct APIQuestData
{
	const char* unique_identifier{};

	//Morphs into Completionist CFlagType.
	int	quest_type{};

	//Morphs into Completionist CCompType.
	int	completion_type{};

	//Quest editor ID as per CreationKit.
	const char* editor_id{};
};

class CompletionistAPI
{
public:
	virtual std::string GetHTMLFormattedName(RE::FormID a_formID) {};
};

typedef void (*GetHTMLFormattedName_t)(std::unique_ptr<CompletionistAPI> api);