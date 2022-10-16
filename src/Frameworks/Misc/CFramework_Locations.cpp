#include "CFramework_Locations.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CFramework_MapMa {
	using namespace Serialization;
	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray MapMa_AG = {
	0x00ECF49,0x005254D,0x00D3931,0x0015D4A,0x0016231,0x00162D6,0x0015D51,
	0x00CF27D,0x00D9531,0x0015D56,0x00F5FEF,0x001620C,0x0016233,0x001620D,
	0x004781E,0x001620E,0x0016332,0x0016275,0x00A7CE5,0x0016232,0x001620F,
	0x0016211,0x00BB47C,0x0016213,0x0016214,0x0016215,0x0016EB2,0x00C44AB,
	0x00FDA48,0x00162B6,0x00F52D8,0x0016339,0x002C652,0x0016218,0x0016219,
	0x00ECF86,0x00B77C3,0x001621A,0x0046BAB,0x001621B,0x001621C,0x001621D,
	0x001633C,0x0016220,0x0016222,0x0016223,0x00B238B,0x0046BDF,0x00E66A7,
	0x001625B,0x00B23A1,0x0016225,0x0016226,0x0016227,0x0016228,0x0016229,
	0x007BC2D,0x001C3B0,0x001622A,0x00B6046,0x0017732,0x001622C,0x001773A,
	0x005E0A6,0x001622D,0x001622E,0x00B238F,0x001622F,0x0017753,0x00B2393,
	0x00C44AD,0x0016230,0x00E66AD,0x001623B,0x001623C,0x001623E,0x001623F,
	0x00F5B8D,0x004FEF2,0x00162B7,0x0016238,0x00B6CDF,0x0016241,0x0016242,
	0x0017760,0x004788C,0x0016243,0x0016244,0x0016246,0x0016247,0x0016248,
	0x0016249,0x001624B,0x001624C,0x001624D,0x001624A,0x003F510,0x001624F,
	0x0016250,0x0016251,0x0016252,0x00B23A7,0x00D392D,0x0016253,0x00162C2,
	0x0016254,0x0016255,0x00D28B7,0x00DED8E,0x0016257,0x0016349,0x0016258,
	0x001A710,0x0016259,0x00F81DB,0x00F81DA,0x00F81D9,0x0016269,0x001625A,
	0x001625C,
	};

	constexpr Serialization::FormArray MapMa_HR = {
	0x001625D,0x001625E,0x001625F,0x0016370,0x00C342A,0x0016260,0x0016261,
	0x00EE79C,0x0016262,0x001634F,0x00162AC,0x0017780,0x0016263,0x0016352,
	0x00162CB,0x0016355,0x0016266,0x0016358,0x0016267,0x0038593,0x00F52B2,
	0x001626A,0x001626B,0x00EF543,0x003F076,0x0017791,0x0016245,0x00EF549,
	0x0080E56,0x001626C,0x00F6698,0x001779A,0x001635E,0x00C2EEF,0x008E03C,
	0x002051D,0x0016224,0x00177A1,0x001626E,0x001626F,0x0016364,0x0016270,
	0x0016367,0x00AF89B,0x0016273,0x00D3938,0x0016235,0x00EAA63,0x0016274,
	0x00F4FE7,0x00E66A3,0x00ECF51,0x001C38A,0x00E962F,0x00F6F11,0x00268FF,
	0x00C44B1,0x0016286,0x001636D,0x0016287,0x00177B0,0x0016288,0x0016289,
	0x001628A,0x0016256,0x001628B,0x001628C,0x001628D,0x0017082,0x0017785,
	0x0047D25,0x001628F,0x0016265,0x00F5EB1,0x00A0E46,0x004B6EB,0x0016290,
	0x00FDBDE,0x0016234,0x00177C3,0x0016291,0x0016292,0x00162B0,0x00C44B3,
	0x0047880,0x0016373,0x00162AD,0x00EF0BA,0x0016293,0x0016294,0x0016295,
	0x0032875,0x001629A,0x0090595,0x001629B,0x001629C,0x001629D,0x010B2C8,
	0x001629E,0x00B23A3,0x00B629F,0x001623D,0x00162A0,0x001629F,0x00EF570,
	0x00D3930,0x001C390,0x0096A46,0x00162A1,0x00ECF4F,0x00162A4,0x0047875,
	0x00162A7,0x00177CC,0x00D393C,0x00D3939,
	};

	constexpr Serialization::FormArray MapMa_SZ = {
	0x00162A5,0x004D8DA,0x0016376,0x0016379,0x00162A6,0x002D511,0x00162CC,
	0x00162A8,0x0016237,0x00162A9,0x00177D7,0x00D3934,0x0092496,0x00246A5,
	0x00AD066,0x006CCC8,0x00162B1,0x00162B3,0x00162B4,0x00162B5,0x001637C,
	0x0016239,0x00EF0AE,0x00162B8,0x0068094,0x002DDD1,0x001634C,0x00C2EF6,
	0x004D0F4,0x00C016D,0x0016382,0x00162BB,0x0082213,0x00F5EAF,0x00B83C8,
	0x00479BB,0x0016217,0x001626D,0x001623A,0x00162BF,0x0108A58,0x00162C0,
	0x00162C1,0x00177E4,0x0080F13,0x00B6295,0x00F5EA8,0x0015D4D,0x00162C3,
	0x0033E45,0x001BAB9,0x00E0F4C,0x001BABD,0x00C4D20,0x00DED90,0x00E0F47,
	0x001BABB,0x00DD9D5,0x00E0F69,0x00D3935,0x001BAC0,0x00E0ED5,0x0105F3A,
	0x00162C4,0x00162C5,0x00D30DA,0x00C2EE9,0x00162C6,0x00162C7,0x0016216,
	0x00C44AF,0x001621F,0x00162C8,0x00162C9,0x008D5D6,0x00162CA,0x0016264,
	0x00C2EFA,0x00DB889,0x00BC0AA,0x0016385,0x00162CD,0x00162CE,0x0072879,
	0x00DD99A,0x00EF0A7,0x0038436,0x009846F,0x00C2EEC,0x00177EF,0x00E1B80,
	0x00162D1,0x00162AB,0x00162AE,0x00162AA,0x00162AF,0x0016271,0x00162D3,
	0x00C33FF,0x00162D2,
	};

	constexpr Serialization::FormArray MapMa_DG = {
	0x004BA5,0x008257,0x00BA4D,0x00D165,0x004C0F,0x002F6F,
	0x003CFE,0x0088E0,0x00FC6C,0x008E41,0x00D0AE,0x00777E,
	0x007B23,
	};

	constexpr Serialization::FormArray MapMa_DB = {
	0x017B68,0x0143BD,0x0143D8,0x0143E3,0x0143BE,0x0143BF,
	0x01BC48,0x0143C0,0x0143D4,0x0143D0,0x018ED8,0x0143C1,
	0x0296DE,0x0143E4,0x0143C2,0x0143C4,0x0143C5,0x0143D1,
	0x03A2CB,0x0143D7,0x0143C6,0x0143D5,0x039F96,0x0143D9,
	0x0143C7,0x0143D6,0x017BB0,0x0365F1,0x0143C3,0x0143CC,
	0x02C49B,0x0143CE,0x03A2C9,0x03A2C7,0x0143C9,0x0143DA,
	0x01A562,0x034EE4,0x0143DC,0x019D1C,0x0143E2,0x0247C8,
	0x0296E0,0x0143E5,0x0143E8,0x0143CA,0x0143E9,0x0143CB,
	0x0143E6,0x0143CD,0x0143E7,0x0143CF,
	};

	constexpr Serialization::FormArray MapMa_CC = {
	0,
	};

	// clang-format on

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() {

		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event || a_event->menuName != RE::MapMenu::MENU_NAME) { return RE::BSEventNotifyControl::kContinue; }

		for (auto i = 0; i < MapMa_AG_FormArray.size(); i++) {
			CHandler::ProcessMapMarker(MapMa_AG_FormArray[i], i, MapMa_Sec::kMapMa_AG);
		}

		for (auto i = 0; i < MapMa_HR_FormArray.size(); i++) {
			CHandler::ProcessMapMarker(MapMa_HR_FormArray[i], i, MapMa_Sec::kMapMa_HR);
		}

		for (auto i = 0; i < MapMa_SZ_FormArray.size(); i++) {
			CHandler::ProcessMapMarker(MapMa_SZ_FormArray[i], i, MapMa_Sec::kMapMa_SZ);
		}

		for (auto i = 0; i < MapMa_DG_FormArray.size(); i++) {
			CHandler::ProcessMapMarker(MapMa_DG_FormArray[i], i, MapMa_Sec::kMapMa_DG);
		}

		for (auto i = 0; i < MapMa_DB_FormArray.size(); i++) {
			CHandler::ProcessMapMarker(MapMa_DB_FormArray[i], i, MapMa_Sec::kMapMa_DB);
		}

		for (auto i = 0; i < MapMa_CC_FormArray.size(); i++) {
			CHandler::ProcessMapMarker(MapMa_CC_FormArray[i], i, MapMa_Sec::kMapMa_CC);
		}

		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Map Marker ) -----
	//---------------------------------------------------

	void CHandler::ProcessMapMarker(RE::TESForm* a_form, std::int32_t a_pos, MapMa_Sec a_section) {

		auto* a_marker = static_cast<RE::TESObjectREFR*>(a_form);
		if (!a_marker || !MarkerIsValid(a_marker)) { return; }

		FoundItemData_NoShow.AddForm(a_marker);

		switch (a_section) {

		case MapMa_Sec::kMapMa_AG:
			MapMa_AG_BoolArray[a_pos] = true;
			MapMa_AG_EntriesFound = std::ranges::count(MapMa_AG_BoolArray, true);
			break;

		case MapMa_Sec::kMapMa_HR:
			MapMa_HR_BoolArray[a_pos] = true;
			MapMa_HR_EntriesFound = std::ranges::count(MapMa_HR_BoolArray, true);
			break;

		case MapMa_Sec::kMapMa_SZ:
			MapMa_SZ_BoolArray[a_pos] = true;
			MapMa_SZ_EntriesFound = std::ranges::count(MapMa_SZ_BoolArray, true);
			break;

		case MapMa_Sec::kMapMa_DG:
			MapMa_DG_BoolArray[a_pos] = true;
			MapMa_DG_EntriesFound = std::ranges::count(MapMa_DG_BoolArray, true);
			break;

		case MapMa_Sec::kMapMa_DB:
			MapMa_DB_BoolArray[a_pos] = true;
			MapMa_DB_EntriesFound = std::ranges::count(MapMa_DB_BoolArray, true);
			break;

		case MapMa_Sec::kMapMa_CC:
			MapMa_CC_BoolArray[a_pos] = true;
			MapMa_CC_EntriesFound = std::ranges::count(MapMa_CC_BoolArray, true);
			break;

		default:
			break;
		}	
	}

	//---------------------------------------------------
	//-- Framework Functions ( Marker Validity Check ) --
	//---------------------------------------------------

	bool CHandler::MarkerIsValid(RE::TESObjectREFR* a_marker) {

		auto* mm = CompletionistData::GetMapMarkerInternal(a_marker);
		if (!mm || !mm->mapData) { return false; }

		return mm->mapData->flags.all(RE::MapMarkerData::Flag::kVisible, RE::MapMarkerData::Flag::kCanTravelTo) && !a_marker->IsDisabled();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		CHandler::Install_CCM();

		CFramework_MapMa_AG::Data.AddForm(0x012BF7, "Cutting Room Floor.esp");
		CFramework_MapMa_AG::Data.AddForm(0x199843, "Cutting Room Floor.esp");
		CFramework_MapMa_HR::Data.AddForm(0x14378F, "Cutting Room Floor.esp");

		CFramework_MapMa_AG::Data.CompileFormArray(CFramework_MapMa::MapMa_AG, "Skyrim.esm");
		CFramework_MapMa_HR::Data.CompileFormArray(CFramework_MapMa::MapMa_HR, "Skyrim.esm");
		CFramework_MapMa_SZ::Data.CompileFormArray(CFramework_MapMa::MapMa_SZ, "Skyrim.esm");
		
		CFramework_MapMa_DG::Data.CompileFormArray(CFramework_MapMa::MapMa_DG, "Dawnguard.esm");
		CFramework_MapMa_DB::Data.CompileFormArray(CFramework_MapMa::MapMa_DB, "Dragonborn.esm");
		CFramework_MapMa_CC::Data.CompileFormArray(CFramework_MapMa::MapMa_CC, "");

		CFramework_MapMa_AG::Data.Populate(MapMa_AG_NameArray, MapMa_AG_FormArray, MapMa_AG_BoolArray, MapMa_AG_TextArray, false, 2);
		CFramework_MapMa_HR::Data.Populate(MapMa_HR_NameArray, MapMa_HR_FormArray, MapMa_HR_BoolArray, MapMa_HR_TextArray, false, 2);
		CFramework_MapMa_SZ::Data.Populate(MapMa_SZ_NameArray, MapMa_SZ_FormArray, MapMa_SZ_BoolArray, MapMa_SZ_TextArray, false, 2);
		CFramework_MapMa_DG::Data.Populate(MapMa_DG_NameArray, MapMa_DG_FormArray, MapMa_DG_BoolArray, MapMa_DG_TextArray, false, 2);
		CFramework_MapMa_DB::Data.Populate(MapMa_DB_NameArray, MapMa_DB_FormArray, MapMa_DB_BoolArray, MapMa_DB_TextArray, false, 2);
		CFramework_MapMa_CC::Data.Populate(MapMa_CC_NameArray, MapMa_CC_FormArray, MapMa_CC_BoolArray, MapMa_CC_TextArray, false, 2);

		MapMa_AG_EntriesTotal = MapMa_AG_FormArray.size();
		MapMa_AG_EntriesFound = std::ranges::count(MapMa_AG_BoolArray, true);

		MapMa_HR_EntriesTotal = MapMa_HR_FormArray.size();
		MapMa_HR_EntriesFound = std::ranges::count(MapMa_HR_BoolArray, true);

		MapMa_SZ_EntriesTotal = MapMa_SZ_FormArray.size();
		MapMa_SZ_EntriesFound = std::ranges::count(MapMa_SZ_BoolArray, true);

		MapMa_DG_EntriesTotal = MapMa_DG_FormArray.size();
		MapMa_DG_EntriesFound = std::ranges::count(MapMa_DG_BoolArray, true);

		MapMa_DB_EntriesTotal = MapMa_DB_FormArray.size();
		MapMa_DB_EntriesFound = std::ranges::count(MapMa_DB_BoolArray, true);

		MapMa_CC_EntriesTotal = MapMa_CC_FormArray.size();
		MapMa_CC_EntriesFound = std::ranges::count(MapMa_CC_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		for (auto i = 0; i < MapMa_AG_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(MapMa_AG_FormArray[i]->GetFormID())) {
				MapMa_AG_BoolArray[i] = true;
			}
		}

		for (auto i = 0; i < MapMa_HR_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(MapMa_HR_FormArray[i]->GetFormID())) {
				MapMa_HR_BoolArray[i] = true;
			}
		}

		for (auto i = 0; i < MapMa_SZ_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(MapMa_SZ_FormArray[i]->GetFormID())) {
				MapMa_SZ_BoolArray[i] = true;
			}
		}

		for (auto i = 0; i < MapMa_DG_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(MapMa_DG_FormArray[i]->GetFormID())) {
				MapMa_DG_BoolArray[i] = true;
			}
		}

		for (auto i = 0; i < MapMa_DB_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(MapMa_DB_FormArray[i]->GetFormID())) {
				MapMa_DB_BoolArray[i] = true;
			}
		}

		for (auto i = 0; i < MapMa_CC_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(MapMa_CC_FormArray[i]->GetFormID())) {
				MapMa_CC_BoolArray[i] = true;
			}
		}

		MapMa_AG_EntriesTotal = MapMa_AG_FormArray.size();
		MapMa_AG_EntriesFound = std::ranges::count(MapMa_AG_BoolArray, true);

		MapMa_HR_EntriesTotal = MapMa_HR_FormArray.size();
		MapMa_HR_EntriesFound = std::ranges::count(MapMa_HR_BoolArray, true);

		MapMa_SZ_EntriesTotal = MapMa_SZ_FormArray.size();
		MapMa_SZ_EntriesFound = std::ranges::count(MapMa_SZ_BoolArray, true);

		MapMa_DG_EntriesTotal = MapMa_DG_FormArray.size();
		MapMa_DG_EntriesFound = std::ranges::count(MapMa_DG_BoolArray, true);

		MapMa_DB_EntriesTotal = MapMa_DB_FormArray.size();
		MapMa_DB_EntriesFound = std::ranges::count(MapMa_DB_BoolArray, true);

		MapMa_CC_EntriesTotal = MapMa_CC_FormArray.size();
		MapMa_CC_EntriesFound = std::ranges::count(MapMa_CC_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install CC Locations ) ---
	//---------------------------------------------------

	void CHandler::Install_CCM() {

		CFramework_MapMa_CC::Data.AddForm(0x00085C, "ccbgssse059-ba_dragonplate.esl");
		CFramework_MapMa_CC::Data.AddForm(0x127259, "cceejsse005-cave.esm");
		CFramework_MapMa_CC::Data.AddForm(0x000F74, "ccbgssse031-advcyrus.esm");
		CFramework_MapMa_CC::Data.AddForm(0x000B63, "ccvsvsse004-beafarmer.esl");
		CFramework_MapMa_CC::Data.AddForm(0x0038D6, "cctwbsse001-puzzledungeon.esm");
		CFramework_MapMa_CC::Data.AddForm(0x0BC826, "ccasvsse001-almsivi.esm");
		CFramework_MapMa_CC::Data.AddForm(0x000B86, "ccbgssse005-goldbrand.esl");
		CFramework_MapMa_CC::Data.AddForm(0x000AFF, "cceejsse004-hall.esl");
		CFramework_MapMa_CC::Data.AddForm(0x000FA2, "cceejsse002-tower.esl");
		CFramework_MapMa_CC::Data.AddForm(0x070E0A, "ccafdsse001-dwesanctuary.esm");
		CFramework_MapMa_CC::Data.AddForm(0x000F72, "cceejsse003-hollow.esl");
		CFramework_MapMa_CC::Data.AddForm(0x1D67E3, "ccbgssse067-daedinv.esm");
		CFramework_MapMa_CC::Data.AddForm(0x1D67E4, "ccbgssse067-daedinv.esm");
		CFramework_MapMa_CC::Data.AddForm(0x1D67E7, "ccbgssse067-daedinv.esm");
		CFramework_MapMa_CC::Data.AddForm(0x000B8A, "cceejsse001-hstead.esm");
		CFramework_MapMa_CC::Data.AddForm(0x00CD93, "ccbgssse016-umbra.esm");
	}
}