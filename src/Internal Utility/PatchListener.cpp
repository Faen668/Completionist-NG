#include "Structs.hpp"
#include "PatchListener.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"
#include "Import/SimpleIni.h"
#include "Internal Utility/MCMHandler.hpp"

namespace CExternalPatchHandler
{
	//---------------------------------------------------
	//-- Patch API Functions-----------------------------
	//---------------------------------------------------

	int32_t CHandler::GetRandomID()
	{
		std::random_device rd;     // Only used once to initialise (seed) engine
		std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> uni(500, 999); // Guaranteed unbiased
		int32_t x = uni(rng);

		while (std::ranges::find(usedIdentifiers, x) != usedIdentifiers.end())
		{
			x++;
		}
		return usedIdentifiers.emplace_back(x);
	}

	//---------------------------------------------------
	//-- Patch API Functions-----------------------------
	//---------------------------------------------------

	void CHandler::Register()
	{
		using namespace CHCMHandler;

		std::vector<std::string> files;
		using rdi = std::filesystem::recursive_directory_iterator;

		const std::filesystem::path path = L"Data/SKSE/Plugins/CompletionistAddons";

		if (!std::filesystem::exists(path) || std::filesystem::is_empty(path))
		{
			INFO("Patch Folder Is Empty Or Does Not Exist.");
			return;
		}

		for (const auto& dirEntry : rdi(path))
		{
			auto file = dirEntry.path().string();

			if (!file.ends_with(".ini"))
			{
				INFO("Unable to load useable data from file: {} : [{}]", file, "Not an ini Format.");
				continue;
			};

			files.push_back(file);
		};

		if (files.size() == 0)
		{
			INFO("No Patch Files Found With Usable Data.");
			return;
		};

		for (auto& file : files)
		{
			SI_Error rc{};

			ini.Reset();
			ini.SetUnicode();
			rc = ini.LoadFile(file.c_str());
			if (rc < 0)
			{
				INFO("Unable to load useable data from file: {} : [{}]", file, "File Corrupt Or Wrong Format");
				continue;
			};

			if (!GetEnabled()) {
				INFO("Unable to load useable data from file: {} : [{}]", file, "Patch Disabled");
				continue;
			};

			if (DKUtil::string::iequals(GetPatchType(), "Quest"))
			{
				AddQuestSupport(file);
				continue;
			}

			if (DKUtil::string::iequals(GetPatchType(), "Misc"))
			{
				AddMiscSupport(file);
				continue;
			}
		};

		if (miscPagedefs.size() > 0) {
			std::sort(miscPagedefs.begin(), miscPagedefs.end());
			MCMAPI::AddMiscPatchedPageDefinitions(miscPagedefs);
		};

		if (mainPagedefs.size() > 0) {
			std::sort(mainPagedefs.begin(), mainPagedefs.end());
			MCMAPI::AddMainPatchedPageDefinitions(mainPagedefs);
		};
	};

	//---------------------------------------------------
	//-- Framework Functions ( Add Quest Support ) ------
	//---------------------------------------------------

	void CHandler::AddQuestSupport(std::string file)
	{
		using namespace CHCMHandler;

		INFO("Adding Patched Quest Support From File: {}", file);

		SetMainPatchID(GetRandomID(), file);
		CSimpleIniA::TNamesDepend sections;
		ini.GetAllSections(sections);

		for (auto& section : sections)
		{
			auto sec = section.pItem;

			//Skip Patch Data.
			if (strcmp(section.pItem, "Completionist Patch Data") == 0)
			{
				continue;
			};

			//Skip If Quest Disabled.
			if (!GetEnabled(section.pItem))
			{
				INFO("Unable to load useable data from quest: {} : [{}]", section.pItem, "Quest Disabled");
				continue;
			};

			//Skip if mod not installed.
			std::string PluginFileName = GetPluginFileName(sec);
			if (!Serialization::CompletionistData::IsModInstalled(PluginFileName)) {
				INFO("Unable to load useable data from quest: {} : [{}]", section.pItem, "Mod Not Found");
				continue;
			};

			int PatchID = GetPatchID();
			CQuestData* quest_data = GetNewQuestData();
			CStageData* stage_data = GetNewStageData();
			CRadiantData* radiant_data = GetNewRadiantData();

			//Set Quest Data.
			quest_data->unique_identifier = GetUUID(sec);
			quest_data->init()
				->set_editorID(GetEditorID(sec))
				->set_quest_type(GetQuestType(sec))
				->set_completion_type(GetCompletionType(sec))
				->set_name(GetQuestName(sec))
				->set_highlight(GetHighlightText(sec));

			//Set Stage Data.
			if (quest_data->completion_type == CCompEnum::kStage) {
				stage_data->link = quest_data->GetKey();
				stage_data->type = GetStageType(sec);
				stage_data->stage = GetStage(sec);
				stage_data->optional_stage = GetOptionalStage(sec);
				quest_data->initPatchStageData(stage_data);
			};

			//Set Radiant Data.
			if (quest_data->completion_type == CCompEnum::kGlobl) {
				radiant_data->link = quest_data->GetKey();
				radiant_data->baseID = GetBaseFormID(sec);
				radiant_data->variID = 0;
				radiant_data->stage = GetRadiantStage(sec);
				radiant_data->value = GetTimesRequired(sec);
				quest_data->initPatchRadiantData(radiant_data);
			};

			//Set MCM Page.
			bool createMCMPage = GetRequiresCustomMCMPage(sec);
			if (createMCMPage)
			{
				auto page = GetMCMPageName(sec);
				if (!IsQuestPageRegistered(page)) {
					mainPagedefs.push_back(std::make_tuple(page, PluginFileName, PatchID));
				};
			};

			//Finalise and Create Data Struct.
			CQuestMaster::QuestAPI::AddQuestData(quest_data, quest_data->GetName(), createMCMPage ? PatchID : GetPatchIDOverride(sec), quest_data->GetKey());
		};
	};

	//---------------------------------------------------
	//-- Framework Functions ( Add Misc Support ) -------
	//---------------------------------------------------

	void CHandler::AddMiscSupport(std::string file)
	{
		using namespace CHCMHandler;

		INFO("Adding Patched Misc Support From File: {}", file);

		SetMiscPatchIDs(GetRandomID(), GetRandomID(), GetRandomID(), file);
		CSimpleIniA::TNamesDepend sections;
		ini.GetAllSections(sections);

		//Skip if mod not installed.
		std::string PluginFileName = GetPluginFileName();
		if (!Serialization::CompletionistData::IsModInstalled(PluginFileName)) {
			INFO("Unable to load useable data from file: {} : [{}]", file, "Mod Not Found");
			return;
		};

		auto* data = GetNewitemsData();
		int iPatchID = GetItemsPatchID();
		int bPatchID = GetBooksPatchID();
		int mPatchID = GetMapMaPatchID();

		data->ItemsID = iPatchID;
		data->BooksID = bPatchID;
		data->MapMaID = mPatchID;
		data->modname = PluginFileName;
		data->mcmpage = GetMCMPageName();

		for (auto& section : sections)
		{
			auto sec = section.pItem;

			//Skip Patch Data.
			if (strcmp(sec, "Completionist Patch Data") == 0)
			{
				continue;
			};

			//Skip If entry Disabled.
			if (!GetEnabled(sec))
			{
				INFO("Unable to load useable data from entry: {} : [{}]", sec, "Entry Disabled.");
				continue;
			};

			auto formID = GetItemFormID(sec);

			auto* form = RE::TESDataHandler::GetSingleton()->LookupForm(formID, PluginFileName);
			if (!form) 
			{
				INFO("Unable to load useable data from entry: {} : [{}]", sec, "Incorrect FormID or Form not found.");
				continue; 
			}

			auto iType = GetItemType(sec);

			if (iType == 0) {
				INFO("Adding Support for Item: {}", form->GetName());
				data->ItemData.AddForm(formID, PluginFileName);
				data->itemsSecReq = true;
			}

			if (iType == 1) {
				INFO("Adding Support for Book: {}", form->GetName());
				data->BookData.AddForm(formID, PluginFileName);
				data->booksSecReq = true;
			}

			if (iType == 2) {
				INFO("Adding Support for Location: {}", Serialization::CompletionistData::GetMapMarkerName(form));
				data->MapsData.AddForm(formID, PluginFileName);
				data->mapmaSecReq = true;
			}
		};

		if (!IsMiscPageRegistered(data->mcmpage))
		{
			CustomItemsPatches.push_back(data);
			miscPagedefs.push_back(std::make_tuple(data->mcmpage, PluginFileName, iPatchID, bPatchID, mPatchID, data->itemsSecReq, data->booksSecReq, data->mapmaSecReq));
		};
	};
};