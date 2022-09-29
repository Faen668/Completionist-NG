#include "Serialization.hpp"
#include "CFramework_Weapons.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"

#undef AddForm

namespace CFramework_Weapons {

	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray SK_Forms = {
	0x00D2AFE, 0x0043E1E, 0x00647AC, 0x00CC392, 0x00946FC, 0x0079B1D, 0x009CCDC, 0x00A4DCE,
	0x00C1989, 0x00ECD54, 0x00AB705, 0x008ADFB, 0x008ADFC, 0x004E4EE, 0x00F82FA, 0x00F71DD,
	0x00F82FC, 0x007E5BB, 0x006B9AD, 0x004A38F, 0x008FFDF, 0x00B3DFA, 0x0017059, 0x00C0186,
	0x0046E0B, 0x0094A2B, 0x0053379, 0x00AB704, 0x00BE25E, 0x010076D, 0x0068976, 0x00D0758,
	0x00DEED8, 0x00233E3, 0x00240D2, 0x001C492, 0x01019D4, 0x008FFDE, 0x00AE086, 0x00AB85D,
	0x00AB703, 0x009FD50, 0x001C4E6, 0x00ECD53, 0x001CB36, 0x00223FB, 0x0035066, 0x00E57F0,
	0x00940D8, 0x007A4FB, 0x00E5F43, 0x0035369, 0x0040002, 0x00AE085, 0x0022265, 0x006AF63,
	0x005BF18, 0x00B994E, 0x002ACD2, 0x006EA8B, 0x00956B5,
	};

	constexpr Serialization::Variation SK_Variations[]{
	{ 0x00AB702, { 0x00F5D1A, 0x00F5D1B, 0x00F5D1C, 0x00F5D1D } },
	{ 0x00D37CE, { 0x00F5D2A, 0x00F5D2B, 0x00F5D2C, 0x00F5D2D } },
	{ 0x00A5DEF, { 0x00F5D1F, 0x00F5D20, 0x00F5D21, 0x00F5D22 } },
	{ 0x003B0BF, { 0x003B0C0, 0x003B0C1 } },
	{ 0x003B0C2, { 0x003B0C3, 0x003B0C4 } },
	{ 0x003B0C5, { 0x003B0C6, 0x003B0C7 } },
	{ 0x003B0B6, { 0x003B0B9, 0x003B0BC } },
	{ 0x003B0B7, { 0x003B0BA, 0x003B0BD } },
	{ 0x003B0B8, { 0x003B0BB, 0x003B0BE } },
	{ 0x00F1AC1, { 0x00F71CD, 0x00F71CE, 0x00F71CF, 0x00F71D0 } },
	{ 0x007A917, { 0x00F6524, 0x00F6525, 0x00F6526, 0x00F6527 } },
	{ 0x007E5C3, { 0x00F6529, 0x00F652A, 0x00F652B, 0x00F652C } },
	{ 0x00F8313, { 0x00F8314, 0x00F8315, 0x00F8316, 0x00F8317, 0x00F8318 } },
	{ 0x002AC6F, { 0x009B2B2 } },
	};

	constexpr Serialization::FormArray DG_Forms = {
	0x00575E, 0x014555, 0x000800, 0x01668C, 0x011BAD, 0x0067CF, 0x01681E, 0x00CFB6,
	};


	constexpr Serialization::FormArray DB_Forms = {
	0x018DDD, 0x01AEA4, 0x01A578, 0x02C01A, 0x018ED5, 0x0179C9, 0x026490, 0x036689,
	0x026B0B,
	};

	constexpr Serialization::Variation DB_Variations[]{
	{ 0x039FA6, { 0x039FAC, 0x039FAD } },
	{ 0x0397F6, { 0x039FB1, 0x039FB4 } },
	};

	// clang-format on

	Serialization::CompletionistData Data;

	std::vector<std::string> NameArray{};
	std::vector<std::string> TextArray{};
	std::vector<RE::TESForm*> FormArray{};
	std::vector<bool> BoolArray{};

	inline ScriptObjectPtr  MCMScript;

	inline uint32_t EntriesTotal;
	inline uint32_t EntriesFound;

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		CHandler::RegisterEvents();
		CHandler::InjectAndCompileData();
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {

		if (!a_event || a_event->newContainer != 0x00014 || !Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = Data.GetBase(a_event->baseObj) ? Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj);
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID) {

		if (auto t_pos = std::find(FormArray.begin(), FormArray.end(), Data.GetForm(a_baseID)); t_pos != FormArray.end()) {
			auto b_pos = std::distance(FormArray.begin(), t_pos);
			BoolArray.at(b_pos) = true;

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);

			for (auto var : Data.GetAllVariations()) {
				if (Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}

			EntriesFound = std::ranges::count(BoolArray, true);
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		auto handler = RE::TESDataHandler::GetSingleton();
		MCMScript = ScriptObject::FromForm(static_cast<RE::TESForm*>(handler->LookupForm(0x00800, "Completionist.esp")), "Completionist_MCMScript");

		if (const auto* Mod = handler->LookupLoadedModByName("Cutting Room Floor.esp"); Mod) {
			Data.AddForm(0x0F9648, "Skyrim.esm");
			Data.AddForm(0x0A88C9, "Skyrim.esm");
			Data.AddForm(0x0AE087, "Skyrim.esm");
		}

		Data.CompileFormArray(CFramework_Weapons::SK_Forms, "Skyrim.esm");
		Data.CompileFormArray(CFramework_Weapons::DG_Forms, "Dawnguard.esm");
		Data.CompileFormArray(CFramework_Weapons::DB_Forms, "Dragonborn.esm");

		Data.CompileVariation(CFramework_Weapons::SK_Variations, "Skyrim.esm");
		Data.CompileVariation(CFramework_Weapons::DB_Variations, "Dragonborn.esm");
		Data.MergeAsCollectable();

		CFramework_Weapons::Data.Populate(NameArray, FormArray, BoolArray, TextArray);
		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {
			if (FoundItemData.HasForm(FormArray[i]->GetFormID())) {
				BoolArray[i] = true;
			}
		}

		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM is Entry Complete ) --
	//---------------------------------------------------

	uint32_t CHandler::IsOptionCompleted(std::string a_name) {

		if (auto t_pos = std::find(NameArray.begin(), NameArray.end(), a_name); t_pos != NameArray.end()) {
			return uint32_t(BoolArray[std::distance(NameArray.begin(), t_pos)]);
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Set Entry Complete ) -
	//---------------------------------------------------

	void CHandler::SetOptionCompleted(std::string a_name) {

		if (auto t_pos = std::find(NameArray.begin(), NameArray.end(), a_name); t_pos != NameArray.end()) {
			auto b_pos = std::distance(NameArray.begin(), t_pos);

			if (BoolArray.at(b_pos)) {
				BoolArray.at(b_pos) = false;

				FoundItemData.RemoveForm(FormArray.at(b_pos)->GetFormID());
				for (auto var : Data.GetAllVariations()) {
					if (Data.GetBase(var) == FormArray.at(b_pos)->GetFormID()) {
						FoundItemData.RemoveForm(var);
					}
				}
			}
			else {
				BoolArray.at(b_pos) = true;
				FoundItemData.AddForm(FormArray.at(b_pos)->GetFormID());
				for (auto var : Data.GetAllVariations()) {
					if (Data.GetBase(var) == FormArray.at(b_pos)->GetFormID()) {
						FoundItemData.AddForm(var);
					}
				}
			}

			EntriesTotal = FormArray.size();
			EntriesFound = std::ranges::count(BoolArray, true);
		}
	}
}