#include "Serialization.hpp"
#include "CFramework_Armor.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"

#undef AddForm

namespace CFramework_Armor {

	using namespace CFramework_Master;

	// clang-format off
	constexpr Serialization::FormArray SkyrimForms = {
		0x0F494E,0x0EAFD0,0x0EAFD3,0x0EAFD2,0x0EAFD1,0x0E1F15,
		0x0E1F14,0x0E1F16,0x0E1F17,0x0E35D7,0x0E35D6,0x0E35D8,
		0x0E35D9,0x108544,0x108543,0x108545,0x108546,0x0D2845,
		0x0D2843,0x0F9904,0x052794,0x086990,0x0C3CE6,0x05CBFE,
		0x0E84C1,0x10A06A,0x0940D5,0x0295F3,0x0E1643,0x096D9B,
		0x06FE72,0x0A4DCD,0x02AC61,0x0DA750,0x0EE5C0,0x0C0165,
		0x0F5981,0x06230B,0x064B71,0x06901D,0x10EB65,0x0E41D8,
		0x045F96,0x0FC5BF,0x06492C,0x06492A,0x06492D,0x06492E,
		0x0CEE6E,0x0CEE70,0x0CEE74,0x0CEE72,0x0CF8A0,0x0CF8A9,
		0x0CF89F,0x0CF8A1,0x0B1460,0x0B145B,0x0EAD49,0x015516,
		0x0CEE76,0x026C3B,0x088952,0x088954,0x0E0DD0,0x0E0DD2,
		0x0C7CBB,0x062303,0x062311,0x088956,0x088958,0x08895A,
	};

	constexpr Serialization::FormArray DawnguardForms = {
		0x00C815,0x00C817,0x00C814,0x00575A,0x005759,0x00284D,
		0x011BAF,0x0047DA,0x014C03
	};

	constexpr Serialization::FormArray DragonbornForms = {
		0x02AD33,0x02AD34,0x02AD32,0x02AD31,0x038ADD,
	};

	constexpr Serialization::Variation SkyrimVariation[] = {
		{ 0x10F570, { 0x07C932 } },
		{ 0x109C11, { 0x0B144D } },
		{ 0x0D2844, { 0x05ABC3 } },
		{ 0x0D2842, { 0x05ABC4 } },
		{ 0x05A9E3, { 0x05A9DF } },
		{ 0x05DB86, { 0x0FCC0E, 0x0FCC0F } },
		{ 0x0FCC0C, { 0x05DB85, 0x0FCC0D } },
		{ 0x05DB87, { 0x0FCC10, 0x0FCC11 } },
		{ 0x05DB88, { 0x0FCC13, 0x0FCC12 } },
		{ 0x0D3AC3, { 0x0D3ACC } },
		{ 0x0D3AC2, { 0x0D3ACB } },
		{ 0x0D3AC4, { 0x0D3ACD } },
		{ 0x0D3AC5, { 0x0D3ACE } },
	};

	constexpr Serialization::Variation DawnguardVariation[] = {
		{ 0x00C816, { 0x00CAD3 } },
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

		auto t_pos = std::find(FormArray.begin(), FormArray.end(), Data.GetForm(a_baseID));
		auto b_pos = std::distance(FormArray.begin(), t_pos);
		BoolArray[b_pos] = true;

		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		auto handler = RE::TESDataHandler::GetSingleton();
		MCMScript = ScriptObject::FromForm(static_cast<RE::TESForm*>(handler->LookupForm(0x00800, "Completionist.esp")), "Completionist_MCMScript");

		if (const auto* Mod = handler->LookupLoadedModByName("Cutting Room Floor.esp"); Mod) {
			Data.AddForm(0x0F9649, "Skyrim.esm"); // 
		}

		Data.CompileFormArray(CFramework_Armor::SkyrimForms, "Skyrim.esm");
		Data.CompileFormArray(CFramework_Armor::DawnguardForms, "Dawnguard.esm");
		Data.CompileFormArray(CFramework_Armor::DragonbornForms, "Dragonborn.esm");

		Data.CompileVariation(CFramework_Armor::SkyrimVariation, "Skyrim.esm");
		Data.CompileVariation(CFramework_Armor::DawnguardVariation, "Dawnguard.esm");
		Data.MergeAsCollectable();

		CFramework_Armor::Data.Populate(NameArray, FormArray, BoolArray, TextArray);
		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);

		//using namespace FrameworkHandler;
		//RegisterAs<FrameworkID::kArmor>(&NameArray, &FormArray, &BoolArray, &TextArray);
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