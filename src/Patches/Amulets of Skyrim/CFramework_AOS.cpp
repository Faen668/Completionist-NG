#include "Serialization.hpp"
#include "CFramework_AOS.hpp"
#include "Internal Utility/Events.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_AOS {
	using namespace CFramework_Master;

	// clang-format off

	// Misc Amulets = 45
	constexpr Serialization::FormArray ItmL1 = {
	0x00DABF,0x02C657,0x02C658,0x03FFCE,0x03EF5D,0x02C659,0x02C65A,0x02F527,
	0x03FFDC,0x041B33,0x040ADC,0x03FFD1,0x0471DE,0x0471DC,0x0471DD,0x03FFD0,
	0x0197C7,0x0471DB,0x0471D9,0x040ADB,0x0471DA,0x041B37,0x041B36,0x03FFD2,
	0x03FFD7,0x03FFDB,0x03FFD9,0x03FFD4,0x03FFD8,0x03FFD5,0x03FFD3,0x03FFD6,
	0x03FFDA,
	};

	// Misc Amulets = 45
	constexpr Serialization::Variation ItmL1V[] = { // Misc Amulets (V)
	{0x0388D7, { 0x0388D8 } },
	{0x017E90, { 0x046BB1 } },
	{0x002F9B, { 0x03BE9F } },
	{0x03EF5C, { 0x0492A1 } },
	{0x03EF5E, { 0x0492A2 } },
	{0x01942A, { 0x03BE9E } },
	{0x03A933, { 0x046C6E } },
	{0x01A292, { 0x01A293,0x040AEF,0x040AF0 } },
	{0x04A2DB, { 0x04A2E2,0x04A2E3,0x04A2E4,0x04A2E5,0x04A2E6,0x04A2E7 } },
	{0x047CE9, { 0x047CEA,0x047CEB,0x047CEC,0x047CED,0x047CEE,0x048258 } },
	{0x04A2DA, { 0x04A2DC,0x04A2DD,0x04A2DE,0x04A2DF,0x04A2E0,0x04A2E1 } },
	{0x047CE3, { 0x047CE4,0x047CE5,0x047CE6,0x047CE7,0x047CE8,0x048257 } },
	};

	// Misc Rings = 4
	constexpr Serialization::Variation ItmL2[] = {
	{0x03A932, {0x046C6D,0x040AED,0x048259} },
	{0x018CFB, {0x040AEE} },
	{0x00B4D6, {0x040AF2} },
	{0x018CFA, {0x0197C6} },
	};

	// Daedric Amulets = 24
	constexpr Serialization::FormArray ItmL3 = {
	0x011D47,0x04056F,0x03FFCF,
	};

	// Daedric Amulets = 24
	constexpr Serialization::Variation ItmL3V[] = {
	{0x02A0C5, {0x046C5B} },
	{0x029090, {0x046C5D} },
	{0x0275A1, {0x046C5E} },
	{0x02A0C7, {0x046C5F} },
	{0x02A62D, {0x046C60} },
	{0x029B5C, {0x048D3D} },
	{0x02A0C8, {0x046C61} },
	{0x024521, {0x046C62} },
	{0x029B5B, {0x048D3C} },
	{0x026ADB, {0x046C5C} },
	{0x02554B, {0x046C64} },
	{0x028B2C, {0x046C66} },
	{0x0295F6, {0x046C67} },
	{0x02A0C6, {0x046C5A} },
	{0x02AB93, {0x046C63} },
	{0x029092, {0x046C6A} },
	{0x024A85, {0x046C6B} },
	{0x02AB92, {0x046C65} },
	{0x02D6CF, {0x046C69} },
	{0x026574, {0x046C68} },
	{0x03B06C, {0x05B35B, 0x05E233} },
	};

	// Dragon Priest Amulets = 14
	constexpr Serialization::Variation ItmL4[] = {
	{0x0036CE, {0x05B367} },
	{0x0036CF, {0x05B368} },
	{0x01A80D, {0x0487BF} },
	{0x01A813, {0x0487C4} },
	{0x01A80A, {0x0487BC} },
	{0x01A80F, {0x0487C1} },
	{0x01A80C, {0x0487BE} },
	{0x01A80E, {0x0487C0} },
	{0x01A812, {0x0487C2} },
	{0x01A810, {0x0487C3} },
	{0x01A80B, {0x0487BD} },
	{0x01A811, {0x0487C5} },
	{0x0036CD, {0x05B366} },
	{0x0036D0, {0x0036D1, 0x0036D2, 0x0036D3, 0x0036D4, 0x0036D5, 0x05B369} },
	};

	// Dragon Priest Rings = 14
	constexpr Serialization::Variation ItmL5[] = {
	{0x00C345, {0x05B360,0x0526E3,0x05B363} },
	{0x00C346, {0x05B361,0x0526E4,0x05B364} },
	{0x01CE0C, {0x0487D3,0x041065,0x0487C9} },
	{0x01CE11, {0x0487D8,0x04106A,0x0487CE} },
	{0x01CE09, {0x0487D0,0x04106C,0x0487C6} },
	{0x01CE0E, {0x0487D5,0x041067,0x0487CB} },
	{0x01CE0B, {0x0487D2,0x041064,0x0487C8} },
	{0x01CE0D, {0x0487D4,0x041066,0x0487CA} },
	{0x01CE0F, {0x0487D6,0x041068,0x0487CC} },
	{0x01CE10, {0x0487D7,0x041069,0x0487CD} },
	{0x01CE0A, {0x0487D1,0x041063,0x0487C7} },
	{0x01CE12, {0x0487D9,0x04106B,0x0487CF} },
	{0x00C344, {0x05B35F,0x0526E2,0x05B362} },
	{0x00C347, {0x00C351,0x00C352,0x00C353,0x00C354,0x00C355,0x0610FD,0x0526E5,0x0526E6,0x0526E7,0x0526E8,0x0526E9,0x0526EA,0x05B365} },
	};


	// Faith Amulets = 7
	constexpr Serialization::FormArray ItmL6 = {
	0x0144E5,0x020B0F,0x020B10,
	};

	// Faith Amulets = 7
	constexpr Serialization::Variation ItmL6V[] = {
	{0x01AD5F, {0x046BB3} },
	{0x005737, {0x005738} },
	{0x01AD61, {0x046BB5} },
	{0x01AD60, {0x04310C,0x046BB4,0x0492A0 } },
	};

	// Doomstone Amulets = 13
	constexpr Serialization::Variation ItmL7[] = { // Doomstone Amulets
	{0x01D8F9, {0x01E94C} },
	{0x01D8FA, {0x01E94E} },
	{0x01D8FB, {0x01E94F} },
	{0x01D8FC, {0x01E950} },
	{0x01D8FD, {0x01E951} },
	{0x01D8FE, {0x01E952} },
	{0x01D8FF, {0x01E953} },
	{0x01D900, {0x01E954} },
	{0x01D901, {0x01E955} },
	{0x01D902, {0x01E956} },
	{0x01D903, {0x01E957} },
	{0x01D904, {0x01E958} },
	{0x01D905, {0x01E959} },
	};

	// Solder Amulets = 19
	constexpr Serialization::FormArray ItmL8 = {
	0x000D64,0x0012D7,0x0012D8,0x0012D9,0x0012DA,0x0012DB,0x0012DC,0x0012DD,
	0x0012DE,0x002313,0x0038A0,0x004367,0x005391,0x00B4DC,0x00B4DD,0x00DABE,
	0x014FC5,0x01720A,0x01DC2D
	};

	// Guild Amulets = 22
	constexpr Serialization::FormArray ItmL9 = {
	0x0110CE,0x0110CF,0x0110D1,0x0110D2,0x012104,0x012105,0x01210F,0x0136A8,
	0x0136AF,0x0136B2,0x013C1A,0x013C1D,0x01418C,0x01DC3D,0x03A92D,0x03A930,
	0x03A931,0x044139,0x02C64E,0x040ADA,
	};

	// Guild Amulets = 22
	constexpr Serialization::Variation ItmL9V[] = {
	{0x03A92E, {0x0136AD, 0x0136AB} },
	{0x03A92F, {0x046C6C} },
	};

	// Thane Amulets = 9
	constexpr Serialization::FormArray ItmL10 = {
	0x0063B8,0x0063B9,0x0063BA,0x0063BB,0x0063BC,0x0063BD,0x0063BE,0x0063BF,
	0x0063C0,
	};

	// Jarl Amulets = 9
	constexpr Serialization::FormArray ItmL11 = {
	0x0073EB,0x0073EC,0x0073ED,0x0073EE,0x0073EF,0x0073F0,0x0073F1,0x0073F2,
	0x0073F3,
	};

	// Hold Amulets = 42
	constexpr Serialization::FormArray ItmL12 = {
	0x009F33,0x009F34,0x009F35,0x009F36,0x009F37,0x009F38,0x009F39,0x009F3A,
	0x009F3B,0x009F3C,0x009F3D,0x009F3E,0x009F3F,0x009F40,0x009F41,0x009F42,
	0x009F43,0x009F44,0x01DC39,0x0420B9,0x0420BA,0x01DC3A,0x0420BE,0x01DC3B,
	0x01DC3C,0x020B11,0x020B16,0x020B17,0x0420B6,0x0420BB,0x0420BC,0x0420B8,
	0x0420BD,
	};

	// Hold Amulets = 42
	constexpr Serialization::Variation ItmL12V[] = {
	{0x00DAC0, {0x04209B} },
	{0x00DAC1, {0x0420AD} },
	{0x00DAC2, {0x0420AE} },
	{0x00DAC3, {0x0420AF} },
	{0x00DAC4, {0x0420B2} },
	{0x00DAC5, {0x0420B0} },
	{0x00DAC6, {0x0420B3} },
	{0x00DAC7, {0x0420B4} },
	{0x00DAC8, {0x0420B5} },
	};

	// clang-format on

	constexpr std::string_view modname = "SL01AmuletsSkyrim.esp";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		CHandler::InjectAndCompileData();
		CHandler::InstallSearchTerms();
		FrameworkAPI::AddUpdateFoundForms(CHandler::UpdateFoundForms);
		CEvents::EventHandler::RegisterForEvent_OnContainerChangedEvent(CHandler::OnContainerChangedEvent);
		PatchesInstalled += 1;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	void CHandler::OnContainerChangedEvent(RE::TESContainerChangedEvent const* a_event) {
		using log = Serialization::CompletionistLog::logType;

		if (a_event->newContainer != 0x00014) { return; }

		if (ItemData1.HasForm(a_event->baseObj)) {
			auto base = ItemData1.GetBase(a_event->baseObj) ? ItemData1.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData1, ItmL1_FormArray, &ItmL1_BoolArray, &ItmL1_EntriesFound, log::kCollected, "NotifyItems");
			return;
		}

		if (ItemData2.HasForm(a_event->baseObj)) {
			auto base = ItemData2.GetBase(a_event->baseObj) ? ItemData2.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData2, ItmL2_FormArray, &ItmL2_BoolArray, &ItmL2_EntriesFound, log::kCollected, "NotifyItems");
			return;
		}

		if (ItemData3.HasForm(a_event->baseObj)) {
			auto base = ItemData3.GetBase(a_event->baseObj) ? ItemData3.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData3, ItmL3_FormArray, &ItmL3_BoolArray, &ItmL3_EntriesFound, log::kCollected, "NotifyItems");
			return;
		}

		if (ItemData4.HasForm(a_event->baseObj)) {
			auto base = ItemData4.GetBase(a_event->baseObj) ? ItemData4.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData4, ItmL4_FormArray, &ItmL4_BoolArray, &ItmL4_EntriesFound, log::kCollected, "NotifyItems");
			return;
		}

		if (ItemData5.HasForm(a_event->baseObj)) {
			auto base = ItemData5.GetBase(a_event->baseObj) ? ItemData5.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData5, ItmL5_FormArray, &ItmL5_BoolArray, &ItmL5_EntriesFound, log::kCollected, "NotifyItems");
			return;
		}

		if (ItemData6.HasForm(a_event->baseObj)) {
			auto base = ItemData6.GetBase(a_event->baseObj) ? ItemData6.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData6, ItmL6_FormArray, &ItmL6_BoolArray, &ItmL6_EntriesFound, log::kCollected, "NotifyItems");
			return;
		}

		if (ItemData7.HasForm(a_event->baseObj)) {
			auto base = ItemData7.GetBase(a_event->baseObj) ? ItemData7.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData7, ItmL7_FormArray, &ItmL7_BoolArray, &ItmL7_EntriesFound, log::kCollected, "NotifyItems");
			return;
		}

		if (ItemData8.HasForm(a_event->baseObj)) {
			auto base = ItemData8.GetBase(a_event->baseObj) ? ItemData8.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData8, ItmL8_FormArray, &ItmL8_BoolArray, &ItmL8_EntriesFound, log::kCollected, "NotifyItems");
			return;
		}

		if (ItemData9.HasForm(a_event->baseObj)) {
			auto base = ItemData9.GetBase(a_event->baseObj) ? ItemData9.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData9, ItmL9_FormArray, &ItmL9_BoolArray, &ItmL9_EntriesFound, log::kCollected, "NotifyItems");
			return;
		}

		if (ItemData10.HasForm(a_event->baseObj)) {
			auto base = ItemData10.GetBase(a_event->baseObj) ? ItemData10.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData10, ItmL10_FormArray, &ItmL10_BoolArray, &ItmL10_EntriesFound, log::kCollected, "NotifyItems");
			return;
		}

		if (ItemData11.HasForm(a_event->baseObj)) {
			auto base = ItemData11.GetBase(a_event->baseObj) ? ItemData11.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData11, ItmL11_FormArray, &ItmL11_BoolArray, &ItmL11_EntriesFound, log::kCollected, "NotifyItems");
			return;
		}

		if (ItemData12.HasForm(a_event->baseObj)) {
			auto base = ItemData12.GetBase(a_event->baseObj) ? ItemData12.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData12, ItmL12_FormArray, &ItmL12_BoolArray, &ItmL12_EntriesFound, log::kCollected, "NotifyItems");
			return;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(ProcessFoundFormArgs, std::string a_section) {

		if (!FoundItemData.HasForm(a_eventID)) {
			auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, data.GetForm(a_eventID)->GetName());
			FrameworkAPI::SendNotification(msg, a_section);
			FrameworkAPI::AddNewEventToLog(eventHandle, data.GetForm(a_eventID)->GetName());
		}

		FoundItemData.AddForm(a_baseID);
		for (auto var : data.GetAllVariations()) {
			if (data.GetBase(var) == a_baseID) {
				FoundItemData.AddForm(var);
			}
		}

		bools->at(std::distance(forms.begin(), std::ranges::find(forms, data.GetForm(a_baseID)))) = true;
		*found = std::ranges::count(*bools, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		ItemData1.CompileFormArray(CPatch_AOS::ItmL1, modname);
		ItemData1.CompileVariation(CPatch_AOS::ItmL1V, modname);
		ItemData2.CompileVariation(CPatch_AOS::ItmL2, modname);
		ItemData3.CompileFormArray(CPatch_AOS::ItmL3, modname);
		ItemData3.CompileVariation(CPatch_AOS::ItmL3V, modname);
		ItemData4.CompileVariation(CPatch_AOS::ItmL4, modname);
		ItemData5.CompileVariation(CPatch_AOS::ItmL5, modname);
		ItemData6.CompileFormArray(CPatch_AOS::ItmL6, modname);
		ItemData6.CompileVariation(CPatch_AOS::ItmL6V, modname);
		ItemData7.CompileVariation(CPatch_AOS::ItmL7, modname);
		ItemData8.CompileFormArray(CPatch_AOS::ItmL8, modname);
		ItemData9.CompileFormArray(CPatch_AOS::ItmL9, modname);
		ItemData9.CompileVariation(CPatch_AOS::ItmL9V, modname);
		ItemData10.CompileFormArray(CPatch_AOS::ItmL10, modname);
		ItemData11.CompileFormArray(CPatch_AOS::ItmL11, modname);
		ItemData12.CompileFormArray(CPatch_AOS::ItmL12, modname);
		ItemData12.CompileVariation(CPatch_AOS::ItmL12V, modname);

		ItemData1.MergeAsCollectable();
		ItemData2.MergeAsCollectable();
		ItemData3.MergeAsCollectable();
		ItemData4.MergeAsCollectable();
		ItemData5.MergeAsCollectable();
		ItemData6.MergeAsCollectable();
		ItemData7.MergeAsCollectable();
		ItemData8.MergeAsCollectable();
		ItemData9.MergeAsCollectable();
		ItemData10.MergeAsCollectable();
		ItemData11.MergeAsCollectable();
		ItemData12.MergeAsCollectable();

		ItemData1.Populate(ItmL1_NameArray, ItmL1_FormArray, ItmL1_BoolArray, ItmL1_TextArray);
		ItemData2.Populate(ItmL2_NameArray, ItmL2_FormArray, ItmL2_BoolArray, ItmL2_TextArray);
		ItemData3.Populate(ItmL3_NameArray, ItmL3_FormArray, ItmL3_BoolArray, ItmL3_TextArray);
		ItemData4.Populate(ItmL4_NameArray, ItmL4_FormArray, ItmL4_BoolArray, ItmL4_TextArray);
		ItemData5.Populate(ItmL5_NameArray, ItmL5_FormArray, ItmL5_BoolArray, ItmL5_TextArray);
		ItemData6.Populate(ItmL6_NameArray, ItmL6_FormArray, ItmL6_BoolArray, ItmL6_TextArray);
		ItemData7.Populate(ItmL7_NameArray, ItmL7_FormArray, ItmL7_BoolArray, ItmL7_TextArray);
		ItemData8.Populate(ItmL8_NameArray, ItmL8_FormArray, ItmL8_BoolArray, ItmL8_TextArray);
		ItemData9.Populate(ItmL9_NameArray, ItmL9_FormArray, ItmL9_BoolArray, ItmL9_TextArray);
		ItemData10.Populate(ItmL10_NameArray, ItmL10_FormArray, ItmL10_BoolArray, ItmL10_TextArray);
		ItemData11.Populate(ItmL11_NameArray, ItmL11_FormArray, ItmL11_BoolArray, ItmL11_TextArray);
		ItemData12.Populate(ItmL12_NameArray, ItmL12_FormArray, ItmL12_BoolArray, ItmL12_TextArray);

		ItmL1_EntriesTotal = ItmL1_FormArray.size();
		ItmL1_EntriesFound = std::ranges::count(ItmL1_BoolArray, true);

		ItmL2_EntriesTotal = ItmL2_FormArray.size();
		ItmL2_EntriesFound = std::ranges::count(ItmL2_BoolArray, true);

		ItmL3_EntriesTotal = ItmL3_FormArray.size();
		ItmL3_EntriesFound = std::ranges::count(ItmL3_BoolArray, true);

		ItmL4_EntriesTotal = ItmL4_FormArray.size();
		ItmL4_EntriesFound = std::ranges::count(ItmL4_BoolArray, true);

		ItmL5_EntriesTotal = ItmL5_FormArray.size();
		ItmL5_EntriesFound = std::ranges::count(ItmL5_BoolArray, true);

		ItmL6_EntriesTotal = ItmL6_FormArray.size();
		ItmL6_EntriesFound = std::ranges::count(ItmL6_BoolArray, true);

		ItmL7_EntriesTotal = ItmL7_FormArray.size();
		ItmL7_EntriesFound = std::ranges::count(ItmL7_BoolArray, true);

		ItmL8_EntriesTotal = ItmL8_FormArray.size();
		ItmL8_EntriesFound = std::ranges::count(ItmL8_BoolArray, true);

		ItmL9_EntriesTotal = ItmL9_FormArray.size();
		ItmL9_EntriesFound = std::ranges::count(ItmL9_BoolArray, true);

		ItmL10_EntriesTotal = ItmL10_FormArray.size();
		ItmL10_EntriesFound = std::ranges::count(ItmL10_BoolArray, true);

		ItmL11_EntriesTotal = ItmL11_FormArray.size();
		ItmL11_EntriesFound = std::ranges::count(ItmL11_BoolArray, true);

		ItmL12_EntriesTotal = ItmL12_FormArray.size();
		ItmL12_EntriesFound = std::ranges::count(ItmL12_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Search Terms ) ---
	//---------------------------------------------------

	void CHandler::InstallSearchTerms()
	{
		for (auto i = 0; i < ItmL1_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL1_FormArray[i], ItmL1_NameArray[i], "$MCMPageAmuletsofSkyrim1", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL2_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL2_FormArray[i], ItmL2_NameArray[i], "$MCMPageAmuletsofSkyrim1", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL3_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL3_FormArray[i], ItmL3_NameArray[i], "$MCMPageAmuletsofSkyrim1", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL4_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL4_FormArray[i], ItmL4_NameArray[i], "$MCMPageAmuletsofSkyrim2", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL5_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL5_FormArray[i], ItmL5_NameArray[i], "$MCMPageAmuletsofSkyrim2", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL6_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL6_FormArray[i], ItmL6_NameArray[i], "$MCMPageAmuletsofSkyrim2", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL7_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL7_FormArray[i], ItmL7_NameArray[i], "$MCMPageAmuletsofSkyrim2", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL8_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL8_FormArray[i], ItmL8_NameArray[i], "$MCMPageAmuletsofSkyrim3", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL9_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL9_FormArray[i], ItmL9_NameArray[i], "$MCMPageAmuletsofSkyrim3", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL10_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL10_FormArray[i], ItmL10_NameArray[i], "$MCMPageAmuletsofSkyrim3", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL11_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL11_FormArray[i], ItmL11_NameArray[i], "$MCMPageAmuletsofSkyrim3", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL12_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL12_FormArray[i], ItmL12_NameArray[i], "$MCMPageAmuletsofSkyrim3", std::to_underlying(EntryCategory::kItem)));
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i = 0; i < ItmL1_FormArray.size(); i++) {
			ItmL1_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL1_FormArray[i], &ItemData1);
		}

		for (auto i = 0; i < ItmL2_FormArray.size(); i++) {
			ItmL2_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL2_FormArray[i], &ItemData2);
		}

		for (auto i = 0; i < ItmL3_FormArray.size(); i++) {
			ItmL3_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL3_FormArray[i], &ItemData3);
		}

		for (auto i = 0; i < ItmL4_FormArray.size(); i++) {
			ItmL4_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL4_FormArray[i], &ItemData4);
		}

		for (auto i = 0; i < ItmL5_FormArray.size(); i++) {
			ItmL5_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL5_FormArray[i], &ItemData5);
		}

		for (auto i = 0; i < ItmL6_FormArray.size(); i++) {
			ItmL6_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL6_FormArray[i], &ItemData6);
		}

		for (auto i = 0; i < ItmL7_FormArray.size(); i++) {
			ItmL7_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL7_FormArray[i], &ItemData7);
		}

		for (auto i = 0; i < ItmL8_FormArray.size(); i++) {
			ItmL8_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL8_FormArray[i], &ItemData8);
		}

		for (auto i = 0; i < ItmL9_FormArray.size(); i++) {
			ItmL9_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL9_FormArray[i], &ItemData9);
		}

		for (auto i = 0; i < ItmL10_FormArray.size(); i++) {
			ItmL10_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL10_FormArray[i], &ItemData10);
		}

		for (auto i = 0; i < ItmL11_FormArray.size(); i++) {
			ItmL11_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL11_FormArray[i], &ItemData11);
		}

		for (auto i = 0; i < ItmL12_FormArray.size(); i++) {
			ItmL12_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL12_FormArray[i], &ItemData12);
		}

		ItmL1_EntriesTotal = ItmL1_FormArray.size();
		ItmL1_EntriesFound = std::ranges::count(ItmL1_BoolArray, true);

		ItmL2_EntriesTotal = ItmL2_FormArray.size();
		ItmL2_EntriesFound = std::ranges::count(ItmL2_BoolArray, true);

		ItmL3_EntriesTotal = ItmL3_FormArray.size();
		ItmL3_EntriesFound = std::ranges::count(ItmL3_BoolArray, true);

		ItmL4_EntriesTotal = ItmL4_FormArray.size();
		ItmL4_EntriesFound = std::ranges::count(ItmL4_BoolArray, true);

		ItmL5_EntriesTotal = ItmL5_FormArray.size();
		ItmL5_EntriesFound = std::ranges::count(ItmL5_BoolArray, true);

		ItmL6_EntriesTotal = ItmL6_FormArray.size();
		ItmL6_EntriesFound = std::ranges::count(ItmL6_BoolArray, true);

		ItmL7_EntriesTotal = ItmL7_FormArray.size();
		ItmL7_EntriesFound = std::ranges::count(ItmL7_BoolArray, true);

		ItmL8_EntriesTotal = ItmL8_FormArray.size();
		ItmL8_EntriesFound = std::ranges::count(ItmL8_BoolArray, true);

		ItmL9_EntriesTotal = ItmL9_FormArray.size();
		ItmL9_EntriesFound = std::ranges::count(ItmL9_BoolArray, true);

		ItmL10_EntriesTotal = ItmL10_FormArray.size();
		ItmL10_EntriesFound = std::ranges::count(ItmL10_BoolArray, true);

		ItmL11_EntriesTotal = ItmL11_FormArray.size();
		ItmL11_EntriesFound = std::ranges::count(ItmL11_BoolArray, true);

		ItmL12_EntriesTotal = ItmL12_FormArray.size();
		ItmL12_EntriesFound = std::ranges::count(ItmL12_BoolArray, true);
	}
}