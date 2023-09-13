#include "Serialization.hpp"
#include "CFramework_Books.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CFramework_Books_AG {

	bool ProcessForm(RE::FormID a_formID) {
		if (CFramework_Books_AG::Data.HasForm(a_formID)) {
			CFramework_Books::CHandler::ProcessFoundForm(a_formID, CFramework_Books_AG::Data.GetForm(a_formID), "CFramework_Books_AG");
			return true;
		}
		return false;
	}
}

namespace CFramework_Books_HS {

	bool ProcessForm(RE::FormID a_formID) {
		if (CFramework_Books_HS::Data.HasForm(a_formID)) {
			CFramework_Books::CHandler::ProcessFoundForm(a_formID, CFramework_Books_HS::Data.GetForm(a_formID), "CFramework_Books_HS");
			return true;
		}
		return false;
	}
}

namespace CFramework_Books_TY {

	bool ProcessForm(RE::FormID a_formID) {
		if (CFramework_Books_TY::Data.HasForm(a_formID)) {
			CFramework_Books::CHandler::ProcessFoundForm(a_formID, CFramework_Books_TY::Data.GetForm(a_formID), "CFramework_Books_TY");
			return true;
		}
		return false;
	}
}

namespace CFramework_Skill_SK {

	bool ProcessForm(RE::FormID a_formID) {
		if (CFramework_Skill_SK::Data.HasForm(a_formID)) {
			CFramework_Books::CHandler::ProcessFoundForm(a_formID, CFramework_Skill_SK::Data.GetForm(a_formID), "CFramework_Skill_SK");
			return true;
		}
		return false;
	}
}

namespace CFramework_Tomes_SK {

	bool ProcessForm(RE::FormID a_formID) {
		if (CFramework_Tomes_SK::Data.HasForm(a_formID)) {
			CFramework_Books::CHandler::ProcessFoundForm(a_formID, CFramework_Tomes_SK::Data.GetForm(a_formID), "CFramework_Tomes_SK");
			return true;
		}
		return false;
	}
}

namespace CFramework_Books_DG {

	bool ProcessForm(RE::FormID a_formID) {
		if (CFramework_Books_DG::Data.HasForm(a_formID)) {
			CFramework_Books::CHandler::ProcessFoundForm(a_formID, CFramework_Books_DG::Data.GetForm(a_formID), "CFramework_Books_DG");
			return true;
		}
		return false;
	}
}

namespace CFramework_Tomes_DG {

	bool ProcessForm(RE::FormID a_formID) {
		if (CFramework_Tomes_DG::Data.HasForm(a_formID)) {
			CFramework_Books::CHandler::ProcessFoundForm(a_formID, CFramework_Tomes_DG::Data.GetForm(a_formID), "CFramework_Tomes_DG");
			return true;
		}
		return false;
	}
}

namespace CFramework_Books_DB {

	bool ProcessForm(RE::FormID a_formID) {
		if (CFramework_Books_DB::Data.HasForm(a_formID)) {
			CFramework_Books::CHandler::ProcessFoundForm(a_formID, CFramework_Books_DB::Data.GetForm(a_formID), "CFramework_Books_DB");
			return true;
		}
		return false;
	}
}

namespace CFramework_Tomes_DB {

	bool ProcessForm(RE::FormID a_formID) {
		if (CFramework_Tomes_DB::Data.HasForm(a_formID)) {
			CFramework_Books::CHandler::ProcessFoundForm(a_formID, CFramework_Tomes_DB::Data.GetForm(a_formID), "CFramework_Tomes_DB");
			return true;
		}
		return false;
	}
}

namespace CFramework_TMaps_SK {

	bool ProcessForm(RE::FormID a_formID) {
		if (CFramework_TMaps_SK::Data.HasForm(a_formID)) {
			CFramework_Books::CHandler::ProcessFoundForm(a_formID, CFramework_TMaps_SK::Data.GetForm(a_formID), "CFramework_TMaps_SK");
			return true;
		}
		return false;
	}
}

namespace CFramework_TMaps_NT {

	bool ProcessForm(RE::FormID a_formID) {
		if (CFramework_TMaps_NT::Data.HasForm(a_formID)) {
			CFramework_Books::CHandler::ProcessFoundForm(a_formID, CFramework_TMaps_NT::Data.GetForm(a_formID), "CFramework_TMaps_NT");
			return true;
		}
		return false;
	}
}

namespace CFramework_TMaps_TH {

	bool ProcessForm(RE::FormID a_formID) {
		if (CFramework_TMaps_TH::Data.HasForm(a_formID)) {
			CFramework_Books::CHandler::ProcessFoundForm(a_formID, CFramework_TMaps_TH::Data.GetForm(a_formID), "CFramework_TMaps_TH");
			return true;
		}
		return false;
	}
}

namespace CFramework_Books_CC {

	bool ProcessForm(RE::FormID a_formID) {
		if (CFramework_Books_CC::Data.HasForm(a_formID)) {
			CFramework_Books::CHandler::ProcessFoundForm(a_formID, CFramework_Books_CC::Data.GetForm(a_formID), "CFramework_Books_CC");
			return true;
		}
		return false;
	}
}

namespace CFramework_Tomes_CC {

	bool ProcessForm(RE::FormID a_formID) {
		if (CFramework_Tomes_CC::Data.HasForm(a_formID)) {
			CFramework_Books::CHandler::ProcessFoundForm(a_formID, CFramework_Tomes_CC::Data.GetForm(a_formID), "CFramework_Tomes_CC");
			return true;
		}
		return false;
	}
}

namespace CFramework_Books {
	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray Books_AG = {
	0x001AFB2,0x001AFB3,0x001AFB1,0x001ACE5,0x001ACE3,0x001ACE4,0x001ACDF,0x00ED047,
	0x001AFC0,0x001AFD4,0x001AFC1,0x001B006,0x00ED02F,0x00ED02E,0x00A0322,0x00F1AB3,
	0x00F6843,0x00403AF,0x001B22C,0x001AFF3,0x00EA5B0,0x00ED03F,0x00ED03A,0x0083B38,
	0x001B22D,0x003636A,0x001B22F,0x001ACFE,0x001AFD7,0x001ACE7,0x001AFFC,0x001ACE8,
	0x0080D63,0x0080D64,0x0080D65,0x0080D66,0x00FF227,0x001ACE9,0x001ACC7,0x001ACB5,
	0x001ACB6,0x001ACB7,0x001B233,0x0032E72,0x001ACB9,0x001ACBA,0x001ACBB,0x001ACBC,
	0x001ACFF,0x00ED605,0x001B237,0x001B238,0x001B239,0x001B23A,0x001B23B,0x00ED032,
	0x001B23C,0x001AD04,0x001ACC9,0x00ED040,0x00EF2C0,0x00E2FC5,0x0083168,0x0083169,
	0x008316A,0x001ACEA,0x00E7F31,0x00E7F33,0x00E7F34,0x003010B,0x002D513,0x0048782,
	0x00ED033,0x001ACF7,0x001ACEB,0x001ACEC,0x001ACED,0x001B267,0x001AD05,0x00ED04E,
	0x00E3E69,0x0072841,0x001ACCB,0x001ACCD,0x00ED031,0x001AD07,0x001ACD5,0x001B00c,
	0x001AD03,
	};

	constexpr Serialization::FormArray Books_HS = {
	0x001AD08,0x00ED603,0x00ED60C,0x00ED60B,0x00D6F0B,0x00FBA57,0x00ED604,
	0x001AFF1,0x00EDA90,0x001B245,0x00EDD35,0x00AE324,0x00EF53E,0x001AD09,
	0x001ACDC,0x00ED608,0x00ED042,0x001ACF1,0x0028ADC,0x00F1445,0x001ACD1,
	0x001ACD4,0x001AFC3,0x001ACF5,0x0086EF8,0x0086EF9,0x0086EFA,0x0086EFB,
	0x001ACB8,0x001AD0E,0x001AD0D,0x004D249,0x00ED039,0x00ED161,0x00E0D66,
	0x0063A0F,0x00B6426,0x00ED607,0x00ED035,0x00B64B1,0x001A332,0x00EB090,
	0x001ACF3,0x00ED037,0x00EDA8E,0x001AF40,0x001AFFE,0x001ACF4,0x003010A,
	0x00ED041,0x00E7EF0,0x00F37D0,0x001AD14,0x001ACC6,0x00F11B7,0x00ED5F4,
	0x00ED5F5,0x00ED5F6,0x00ED5F7,0x001ACDB,0x0032785,0x00ED03B,0x00F84A1,
	0x001AF93,0x001B22B,0x00ED04C,0x00F1447,0x00E7F37,0x00ED062,0x00F11D5,
	0x00ED046,0x00ED045,0x00ED03D,0x00ED048,0x00ED044,0x001AD0C,0x00E2FC6,
	0x001B25A,0x001AD15,
	};

	constexpr Serialization::FormArray Books_TY = {
	0x007EB03,0x001AF94,0x00ED04B,0x001ACE1,0x00EDA8F,0x001B25E,0x007EB9E,
	0x001AFC2,0x001ACC8,0x0105A52,0x00F86FE,0x00EF638,0x001B262,0x007EBC2,
	0x007EBC9,0x001ACF2,0x001AFCA,0x00EDDD5,0x009DE3D,0x00E0D68,0x001ACD2,
	0x001ACD3,0x0068B5A,0x00F456D,0x00ED030,0x001B26A,0x0072840,0x00C1771,
	0x001AD0A,0x001ACEF,0x00F699D,0x001ACF0,0x001B26E,0x00E0D67,0x00F68AC,
	0x00F68AD,0x010BEDF,0x0037DEA,0x001AD0F,0x001AD11,0x001AD12,0x001AFD3,
	0x001ACD6,0x001ACD7,0x001ACD8,0x001ACD9,0x001ACDA,0x001AD13,0x00F4530,
	0x00DB0C7,0x001ACFA,0x001AF8A,0x001AF8B,0x001AF8C,0x001AF8D,0x001AF8E,
	0x001AF8F,0x001AF90,0x001AF91,0x00F03E3,0x00ED04D,0x001ACFB,0x00ED04F,
	0x00F683F,0x001AD16,0x001AD17,0x001AD18,0x00ED03C,0x0083B3B,0x001AFF2,
	0x001AFFB,0x001B00B,0x001B024,0x001B01E,0x001B026,0x001ACFD,0x00ED601,
	0x00ED5F8,0x001ACDD,0x001ADB4,0x001ACE2,0x00ED606,0x00E1640,0x009F267,
	0x006851B,0x001ACFC,0x001AFBA,0x00ED061,0x001B272,0x00ED63F,0x00F11B6,
	0x00F1446,0x001B27D,0x001B273,0x001B274,0x002A563,0x001B27A,
	};

	constexpr Serialization::FormArray Skill_SK = {
	0x001AFEA,0x001AFE9,0x001B01F,0x001AFF7,0x001AFD9,0x001B017,0x001B025,0x001B010,
	0x001AFDF,0x001B00D,0x001B00E,0x001AFC4,0x001AFEE,0x002F838,0x001B01C,0x001ACE6,
	0x002F83C,0x001AFDC,0x001B012,0x001AFD6,0x001B023,0x001B236,0x002F83B,0x002F83A,
	0x001AFD1,0x001AFF8,0x001AFC9,0x001AFC7,0x001AFDD,0x002F837,0x001B008,0x001AFDA,
	0x002F836,0x001AFF6,0x001AFD2,0x001AFC8,0x001AFEC,0x001B001,0x001B00F,0x001B002,
	0x001AFE5,0x001AFCF,0x001B021,0x001AFD0,0x001AFE8,0x001AFE6,0x001AFC5,0x001B018,
	0x001AFF0,0x001B013,0x001AFE4,0x001AFF9,0x001B01B,0x001B022,0x001B015,0x001AFCC,
	0x001AFED,0x001B004,0x001B020,0x001AFCB,0x001AFDB,0x001AFC6,0x001B01D,0x001AFCE,
	0x001AFEF,0x001B009,0x001B011,0x001B00A,0x001AFE7,0x001B016,0x001B005,0x001AFE3,
	0x001AFFA,0x001AFE2,0x001B019,0x001AFCD,0x001B26D,0x001AFDE,0x001B000,0x001AFD5,
	0x001B003,0x001AFEB,0x001B01A,0x001AFBF,0x001B276,0x002F839,0x001B007,0x001AFE0,
	0x001B014,0x001AFD8,
	};

	constexpr Serialization::FormArray Tomes_SK = {
	0x00DD647,0x00A26EE,0x00A270D,0x00A26ED,0x00A26F1,0x009E2A9,0x00A271B,0x00A2711,
	0x009E2A7,0x00A2708,0x00A2728,0x00FF7D1,0x010F64D,0x00A26F6,0x010FD60,0x009E2AB,
	0x00A26EC,0x00B45F7,0x00A26EF,0x00A26F0,0x009E2AD,0x00A26F9,0x00A26E7,0x00A26EA,
	0x00D2B4E,0x00A26F7,0x00A26E9,0x00F4997,0x00A26F8,0x00A271D,0x00A2712,0x00A2706,
	0x00A26FD,0x00A2700,0x00A270C,0x00A2703,0x009CD51,0x00A26FA,0x00A2714,0x009CD52,
	0x00A2704,0x00A2701,0x00A26FB,0x009E2AC,0x00DD643,0x00A2722,0x00FDE7B,0x00A271A,
	0x009E2AF,0x00A271E,0x00A2727,0x00A271C,0x00A26FE,0x00A2707,0x010F7F3,0x010F7F4,
	0x00A2715,0x00A26E4,0x009E2AE,0x00A26FF,0x00A2705,0x00A270E,0x00A26E2,0x00DD646,
	0x00A2719,0x00A270F,0x009E2A8,0x00A2717,0x00A26E8,0x009E2AA,0x00A2713,0x00A26EB,
	0x00A2725,0x00A2726,0x00A26F2,0x00A2718,0x00A2702,0x009CD54,0x009CD53,
	0x00A2720,0x00A26E3,0x00A26FC,0x00A26E5,0x010F7F5,0x0109112,0x00A2729,0x00A271F,
	0x00A2721,0x00A2709,0x00A270A,0x00A270B,0x00A26E6,
	};

	constexpr Serialization::FormArray Books_DG = {
	0x016692,0x01A3E7,0x014010,0x014011,0x014012,0x014013,0x014014,0x014015,
	0x014016,0x014017,0x014018,0x014019,0x01A3E6,0x00D070,0x003F79,0x004D5B,
	0x01A3E5,0x00FAC2,0x006925,0x01A3E8,0x01A3E0,0x01A3E1,0x01A3E2,0x01A3E3,
	};

	constexpr Serialization::FormArray Tomes_DG = {
	0x0045B0,0x0045B1,0x0045B2,0x00E8D6,0x00E8D5,0x003F4E,0x003F51,0x003F4D,
	};

	constexpr Serialization::FormArray Books_DB = {
	0x033BD8,0x016E2C,0x01E99E,0x01E99F,0x01E99D,0x01E99C,0x016E2D,0x016E22,
	0x028261,0x028262,0x028263,0x03ABCC,0x028264,0x03661A,0x03AF1B,0x03AF1C,
	0x03AF1D,0x028269,0x02BAC7,0x02826A,0x02826B,0x03A35F,0x03A360,0x03A361,
	0x03A362,0x02826C,0x02826D,0x028265,0x028267,0x03ABCD,0x028268,0x02826E,
	0x02826F,0x028270,0x028271,0x028272,0x028273,0x028274,0x03B052,0x03B063,
	0x03B064,0x028275,0x03B3A5,0x029102,0x028276,0x028266,0x01D8D0,0x028277,
	};

	constexpr Serialization::FormArray Tomes_DB = {
	0x0177B5,0x0177AC,0x01CE07,0x01AABA,0x01E2B1,0x033C67,0x0177BD,0x020685,
	0x0177AD,
	};

	constexpr Serialization::FormArray TMaps_SK = {
	0x0EF07A,0x0F33CE,0x0F33CF,0x0F33D1,0x0F33CD,0x0F33D4,0x0F33D0,0x0F33D5,
	0x0F33D3,0x0F33E0,0x0F33D2,0x0DDEFB,
	};

	constexpr Serialization::FormArray TMaps_NT = {
	0x000D62,0x000D63,0x000803,0x000804,0x000809,0x000805,0x000806,0x000807,
	0x000808,0x00080B,0x00080C,0x00080D,0x00080E,
	};

	constexpr Serialization::FormArray TMaps_TH = {
	0x000D63,0x006DA9,0x00787E,0x00834C,0x00834F,0x0095BA,0x021824,
	};

	constexpr Serialization::FormArray Books_CC = {
	0,
	};

	constexpr Serialization::FormArray Tomes_CC = {
	0,
	};

	// clang-format on

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
		CHandler::InstallSearchTerms();
		FrameworkAPI::AddUpdateFoundForms(CHandler::UpdateFoundForms);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() 
	{
		RE::BooksRead::GetEventSource()->AddEventSink(CHandler::GetSingleton());

		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (CFramework_Books_AG::ProcessForm(a_event->book->GetFormID())) { return EventResult::kContinue; };
		if (CFramework_Books_HS::ProcessForm(a_event->book->GetFormID())) { return EventResult::kContinue; };
		if (CFramework_Books_TY::ProcessForm(a_event->book->GetFormID())) { return EventResult::kContinue; };
		if (CFramework_Skill_SK::ProcessForm(a_event->book->GetFormID())) { return EventResult::kContinue; };
		if (CFramework_Tomes_SK::ProcessForm(a_event->book->GetFormID())) { return EventResult::kContinue; };
		if (CFramework_Books_DG::ProcessForm(a_event->book->GetFormID())) { return EventResult::kContinue; };
		if (CFramework_Tomes_DG::ProcessForm(a_event->book->GetFormID())) { return EventResult::kContinue; };
		if (CFramework_Books_DB::ProcessForm(a_event->book->GetFormID())) { return EventResult::kContinue; };
		if (CFramework_Tomes_DB::ProcessForm(a_event->book->GetFormID())) { return EventResult::kContinue; };
		if (CFramework_TMaps_SK::ProcessForm(a_event->book->GetFormID())) { return EventResult::kContinue; };
		if (CFramework_TMaps_NT::ProcessForm(a_event->book->GetFormID())) { return EventResult::kContinue; };
		if (CFramework_TMaps_TH::ProcessForm(a_event->book->GetFormID())) { return EventResult::kContinue; };
		if (CFramework_Books_CC::ProcessForm(a_event->book->GetFormID())) { return EventResult::kContinue; };
		if (CFramework_Tomes_CC::ProcessForm(a_event->book->GetFormID())) { return EventResult::kContinue; };
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				if (CFramework_Books_AG::ProcessForm(target)) { return EventResult::kContinue; };
				if (CFramework_Books_HS::ProcessForm(target)) { return EventResult::kContinue; };
				if (CFramework_Books_TY::ProcessForm(target)) { return EventResult::kContinue; };
				if (CFramework_Skill_SK::ProcessForm(target)) { return EventResult::kContinue; };
				if (CFramework_Tomes_SK::ProcessForm(target)) { return EventResult::kContinue; };
				if (CFramework_Books_DG::ProcessForm(target)) { return EventResult::kContinue; };
				if (CFramework_Tomes_DG::ProcessForm(target)) { return EventResult::kContinue; };
				if (CFramework_Books_DB::ProcessForm(target)) { return EventResult::kContinue; };
				if (CFramework_Tomes_DB::ProcessForm(target)) { return EventResult::kContinue; };
				if (CFramework_TMaps_SK::ProcessForm(target)) { return EventResult::kContinue; };
				if (CFramework_TMaps_NT::ProcessForm(target)) { return EventResult::kContinue; };
				if (CFramework_TMaps_TH::ProcessForm(target)) { return EventResult::kContinue; };
				if (CFramework_Books_CC::ProcessForm(target)) { return EventResult::kContinue; };
				if (CFramework_Tomes_CC::ProcessForm(target)) { return EventResult::kContinue; };
			}
			return EventResult::kContinue;
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_formID, RE::TESForm* a_base, std::string a_section) {

		if (!FoundItemData.HasForm(a_formID)) {
			auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, a_base->GetName());
			FrameworkAPI::SendNotification(msg, "NotifyBooks");
		}

		FoundItemData.AddForm(a_formID);

		if (a_section == "CFramework_Books_AG") {
			FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kBook, a_base->GetName());

			auto t_pos = std::ranges::find(Books_AG_FormArray, a_base);
			auto b_pos = std::distance(Books_AG_FormArray.begin(), t_pos);

			Books_AG_BoolArray[b_pos] = true;
			Books_AG_EntriesFound = std::ranges::count(Books_AG_BoolArray, true);
			return;
		}

		if (a_section == "CFramework_Books_HS") {
			FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kBook, a_base->GetName());

			auto t_pos = std::ranges::find(Books_HS_FormArray, a_base);
			auto b_pos = std::distance(Books_HS_FormArray.begin(), t_pos);

			Books_HS_BoolArray[b_pos] = true;
			Books_HS_EntriesFound = std::ranges::count(Books_HS_BoolArray, true);
			return;
		}

		if (a_section == "CFramework_Books_TY") {
			FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kBook, a_base->GetName());

			auto t_pos = std::ranges::find(Books_TY_FormArray, a_base);
			auto b_pos = std::distance(Books_TY_FormArray.begin(), t_pos);

			Books_TY_BoolArray[b_pos] = true;
			Books_TY_EntriesFound = std::ranges::count(Books_TY_BoolArray, true);
			return;
		}

		if (a_section == "CFramework_Skill_SK") {
			FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kBook, a_base->GetName());

			auto t_pos = std::ranges::find(Skill_SK_FormArray, a_base);
			auto b_pos = std::distance(Skill_SK_FormArray.begin(), t_pos);

			Skill_SK_BoolArray[b_pos] = true;
			Skill_SK_EntriesFound = std::ranges::count(Skill_SK_BoolArray, true);
			return;
		}

		if (a_section == "CFramework_Tomes_SK") {
			FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kTome, a_base->GetName());

			auto t_pos = std::ranges::find(Tomes_SK_A_FormArray, a_base);
			if (t_pos != Tomes_SK_A_FormArray.end()) {
				auto b_pos = std::distance(Tomes_SK_A_FormArray.begin(), t_pos);
				Tomes_SK_A_BoolArray[b_pos] = true;
				Tomes_SK_A_EntriesFound = std::ranges::count(Tomes_SK_A_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Tomes_SK_C_FormArray, a_base);
			if (t_pos != Tomes_SK_C_FormArray.end()) {
				auto b_pos = std::distance(Tomes_SK_C_FormArray.begin(), t_pos);
				Tomes_SK_C_BoolArray[b_pos] = true;
				Tomes_SK_C_EntriesFound = std::ranges::count(Tomes_SK_C_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Tomes_SK_D_FormArray, a_base);
			if (t_pos != Tomes_SK_D_FormArray.end()) {
				auto b_pos = std::distance(Tomes_SK_D_FormArray.begin(), t_pos);
				Tomes_SK_D_BoolArray[b_pos] = true;
				Tomes_SK_D_EntriesFound = std::ranges::count(Tomes_SK_D_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Tomes_SK_I_FormArray, a_base);
			if (t_pos != Tomes_SK_I_FormArray.end()) {
				auto b_pos = std::distance(Tomes_SK_I_FormArray.begin(), t_pos);
				Tomes_SK_I_BoolArray[b_pos] = true;
				Tomes_SK_I_EntriesFound = std::ranges::count(Tomes_SK_I_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Tomes_SK_R_FormArray, a_base);
			if (t_pos != Tomes_SK_R_FormArray.end()) {
				auto b_pos = std::distance(Tomes_SK_R_FormArray.begin(), t_pos);
				Tomes_SK_R_BoolArray[b_pos] = true;
				Tomes_SK_R_EntriesFound = std::ranges::count(Tomes_SK_R_BoolArray, true);
				return;
			}
		}

		if (a_section == "CFramework_Books_DG") {
			FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kBook, a_base->GetName());

			auto t_pos = std::ranges::find(Books_DG_FormArray, a_base);
			auto b_pos = std::distance(Books_DG_FormArray.begin(), t_pos);

			Books_DG_BoolArray[b_pos] = true;
			Books_DG_EntriesFound = std::ranges::count(Books_DG_BoolArray, true);
			return;
		}

		if (a_section == "CFramework_Tomes_DG") {
			FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kTome, a_base->GetName());

			auto t_pos = std::ranges::find(Tomes_DG_A_FormArray, a_base);
			if (t_pos != Tomes_DG_A_FormArray.end()) {
				auto b_pos = std::distance(Tomes_DG_A_FormArray.begin(), t_pos);
				Tomes_DG_A_BoolArray[b_pos] = true;
				Tomes_DG_A_EntriesFound = std::ranges::count(Tomes_DG_A_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Tomes_DG_C_FormArray, a_base);
			if (t_pos != Tomes_DG_C_FormArray.end()) {
				auto b_pos = std::distance(Tomes_DG_C_FormArray.begin(), t_pos);
				Tomes_DG_C_BoolArray[b_pos] = true;
				Tomes_DG_C_EntriesFound = std::ranges::count(Tomes_DG_C_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Tomes_DG_D_FormArray, a_base);
			if (t_pos != Tomes_DG_D_FormArray.end()) {
				auto b_pos = std::distance(Tomes_DG_D_FormArray.begin(), t_pos);
				Tomes_DG_D_BoolArray[b_pos] = true;
				Tomes_DG_D_EntriesFound = std::ranges::count(Tomes_DG_D_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Tomes_DG_I_FormArray, a_base);
			if (t_pos != Tomes_DG_I_FormArray.end()) {
				auto b_pos = std::distance(Tomes_DG_I_FormArray.begin(), t_pos);
				Tomes_DG_I_BoolArray[b_pos] = true;
				Tomes_DG_I_EntriesFound = std::ranges::count(Tomes_DG_I_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Tomes_DG_R_FormArray, a_base);
			if (t_pos != Tomes_DG_R_FormArray.end()) {
				auto b_pos = std::distance(Tomes_DG_R_FormArray.begin(), t_pos);
				Tomes_DG_R_BoolArray[b_pos] = true;
				Tomes_DG_R_EntriesFound = std::ranges::count(Tomes_DG_R_BoolArray, true);
				return;
			}
		}

		if (a_section == "CFramework_Books_DB") {
			FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kBook, a_base->GetName());

			auto t_pos = std::ranges::find(Books_DB_FormArray, a_base);
			auto b_pos = std::distance(Books_DB_FormArray.begin(), t_pos);

			Books_DB_BoolArray[b_pos] = true;
			Books_DB_EntriesFound = std::ranges::count(Books_DB_BoolArray, true);
			return;
		}

		if (a_section == "CFramework_Tomes_DB") {
			FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kTome, a_base->GetName());

			auto t_pos = std::ranges::find(Tomes_DB_A_FormArray, a_base);
			if (t_pos != Tomes_DB_A_FormArray.end()) {
				auto b_pos = std::distance(Tomes_DB_A_FormArray.begin(), t_pos);
				Tomes_DB_A_BoolArray[b_pos] = true;
				Tomes_DB_A_EntriesFound = std::ranges::count(Tomes_DB_A_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Tomes_DB_C_FormArray, a_base);
			if (t_pos != Tomes_DB_C_FormArray.end()) {
				auto b_pos = std::distance(Tomes_DB_C_FormArray.begin(), t_pos);
				Tomes_DB_C_BoolArray[b_pos] = true;
				Tomes_DB_C_EntriesFound = std::ranges::count(Tomes_DB_C_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Tomes_DB_D_FormArray, a_base);
			if (t_pos != Tomes_DB_D_FormArray.end()) {
				auto b_pos = std::distance(Tomes_DB_D_FormArray.begin(), t_pos);
				Tomes_DB_D_BoolArray[b_pos] = true;
				Tomes_DB_D_EntriesFound = std::ranges::count(Tomes_DB_D_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Tomes_DB_I_FormArray, a_base);
			if (t_pos != Tomes_DB_I_FormArray.end()) {
				auto b_pos = std::distance(Tomes_DB_I_FormArray.begin(), t_pos);
				Tomes_DB_I_BoolArray[b_pos] = true;
				Tomes_DB_I_EntriesFound = std::ranges::count(Tomes_DB_I_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Tomes_DB_R_FormArray, a_base);
			if (t_pos != Tomes_DB_R_FormArray.end()) {
				auto b_pos = std::distance(Tomes_DB_R_FormArray.begin(), t_pos);
				Tomes_DB_R_BoolArray[b_pos] = true;
				Tomes_DB_R_EntriesFound = std::ranges::count(Tomes_DB_R_BoolArray, true);
				return;
			}
		}

		if (a_section == "CFramework_TMaps_SK") {
			FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kBook, a_base->GetName());

			auto t_pos = std::ranges::find(TMaps_SK_FormArray, a_base);
			auto b_pos = std::distance(TMaps_SK_FormArray.begin(), t_pos);

			TMaps_SK_BoolArray[b_pos] = true;
			TMaps_SK_EntriesFound = std::ranges::count(TMaps_SK_BoolArray, true);
			return;
		}

		if (a_section == "CFramework_TMaps_NT") {
			FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kBook, a_base->GetName());

			auto t_pos = std::ranges::find(TMaps_NT_FormArray, a_base);
			auto b_pos = std::distance(TMaps_NT_FormArray.begin(), t_pos);

			TMaps_NT_BoolArray[b_pos] = true;
			TMaps_NT_EntriesFound = std::ranges::count(TMaps_NT_BoolArray, true);
			return;
		}

		if (a_section == "CFramework_TMaps_TH") {
			FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kBook, a_base->GetName());

			auto t_pos = std::ranges::find(TMaps_TH_FormArray, a_base);
			auto b_pos = std::distance(TMaps_TH_FormArray.begin(), t_pos);

			TMaps_TH_BoolArray[b_pos] = true;
			TMaps_TH_EntriesFound = std::ranges::count(TMaps_TH_BoolArray, true);
			return;
		}

		if (a_section == "CFramework_Books_CC") {
			FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kBook, a_base->GetName());

			auto t_pos = std::ranges::find(Books_CC_FormArray, a_base);
			auto b_pos = std::distance(Books_CC_FormArray.begin(), t_pos);

			Books_CC_BoolArray[b_pos] = true;
			Books_CC_EntriesFound = std::ranges::count(Books_CC_BoolArray, true);
			return;
		}

		if (a_section == "CFramework_Tomes_CC") {
			FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kTome, a_base->GetName());

			auto t_pos = std::ranges::find(Tomes_CC_A_FormArray, a_base);
			if (t_pos != Tomes_CC_A_FormArray.end()) {
				auto b_pos = std::distance(Tomes_CC_A_FormArray.begin(), t_pos);
				Tomes_CC_A_BoolArray[b_pos] = true;
				Tomes_CC_A_EntriesFound = std::ranges::count(Tomes_CC_A_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Tomes_CC_C_FormArray, a_base);
			if (t_pos != Tomes_CC_C_FormArray.end()) {
				auto b_pos = std::distance(Tomes_CC_C_FormArray.begin(), t_pos);
				Tomes_CC_C_BoolArray[b_pos] = true;
				Tomes_CC_C_EntriesFound = std::ranges::count(Tomes_CC_C_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Tomes_CC_D_FormArray, a_base);
			if (t_pos != Tomes_CC_D_FormArray.end()) {
				auto b_pos = std::distance(Tomes_CC_D_FormArray.begin(), t_pos);
				Tomes_CC_D_BoolArray[b_pos] = true;
				Tomes_CC_D_EntriesFound = std::ranges::count(Tomes_CC_D_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Tomes_CC_I_FormArray, a_base);
			if (t_pos != Tomes_CC_I_FormArray.end()) {
				auto b_pos = std::distance(Tomes_CC_I_FormArray.begin(), t_pos);
				Tomes_CC_I_BoolArray[b_pos] = true;
				Tomes_CC_I_EntriesFound = std::ranges::count(Tomes_CC_I_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Tomes_CC_R_FormArray, a_base);
			if (t_pos != Tomes_CC_R_FormArray.end()) {
				auto b_pos = std::distance(Tomes_CC_R_FormArray.begin(), t_pos);
				Tomes_CC_R_BoolArray[b_pos] = true;
				Tomes_CC_R_EntriesFound = std::ranges::count(Tomes_CC_R_BoolArray, true);
				return;
			}
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		//Cutting Room Floor
		if (Serialization::CompletionistData::IsModInstalled("Cutting Room Floor.esp")) {
			CFramework_Tomes_SK::Data.AddForm(0x00B3165, "Skyrim.esm");
		}

		CHandler::Install_CCB();
		CHandler::Install_CCT();

		//Spell Tomes (Skyrim)
		CFramework_Tomes_SK::Data.CompileFormArray(CFramework_Books::Tomes_SK, "Skyrim.esm");
		CFramework_Tomes_SK::Data.MergeAsCollectable();
		CFramework_Tomes_SK::Data.PopulateSpellTomes(Tomes_SK_A_NameArray, Tomes_SK_A_FormArray, Tomes_SK_A_BoolArray, Tomes_SK_A_TextArray, RE::ActorValue::kAlteration);
		CFramework_Tomes_SK::Data.PopulateSpellTomes(Tomes_SK_C_NameArray, Tomes_SK_C_FormArray, Tomes_SK_C_BoolArray, Tomes_SK_C_TextArray, RE::ActorValue::kConjuration);
		CFramework_Tomes_SK::Data.PopulateSpellTomes(Tomes_SK_D_NameArray, Tomes_SK_D_FormArray, Tomes_SK_D_BoolArray, Tomes_SK_D_TextArray, RE::ActorValue::kDestruction);
		CFramework_Tomes_SK::Data.PopulateSpellTomes(Tomes_SK_I_NameArray, Tomes_SK_I_FormArray, Tomes_SK_I_BoolArray, Tomes_SK_I_TextArray, RE::ActorValue::kIllusion);
		CFramework_Tomes_SK::Data.PopulateSpellTomes(Tomes_SK_R_NameArray, Tomes_SK_R_FormArray, Tomes_SK_R_BoolArray, Tomes_SK_R_TextArray, RE::ActorValue::kRestoration);

		Tomes_SK_A_EntriesTotal = Tomes_SK_A_FormArray.size();
		Tomes_SK_C_EntriesTotal = Tomes_SK_C_FormArray.size();
		Tomes_SK_D_EntriesTotal = Tomes_SK_D_FormArray.size();
		Tomes_SK_I_EntriesTotal = Tomes_SK_I_FormArray.size();
		Tomes_SK_R_EntriesTotal = Tomes_SK_R_FormArray.size();

		Tomes_SK_A_EntriesFound = std::ranges::count(Tomes_SK_A_BoolArray, true);
		Tomes_SK_C_EntriesFound = std::ranges::count(Tomes_SK_C_BoolArray, true);
		Tomes_SK_D_EntriesFound = std::ranges::count(Tomes_SK_D_BoolArray, true);
		Tomes_SK_I_EntriesFound = std::ranges::count(Tomes_SK_I_BoolArray, true);
		Tomes_SK_R_EntriesFound = std::ranges::count(Tomes_SK_R_BoolArray, true);

		//Spell Tomes (Dawnguard)
		CFramework_Tomes_DG::Data.CompileFormArray(CFramework_Books::Tomes_DG, "Dawnguard.esm");
		CFramework_Tomes_DG::Data.MergeAsCollectable();
		CFramework_Tomes_DG::Data.PopulateSpellTomes(Tomes_DG_A_NameArray, Tomes_DG_A_FormArray, Tomes_DG_A_BoolArray, Tomes_DG_A_TextArray, RE::ActorValue::kAlteration);
		CFramework_Tomes_DG::Data.PopulateSpellTomes(Tomes_DG_C_NameArray, Tomes_DG_C_FormArray, Tomes_DG_C_BoolArray, Tomes_DG_C_TextArray, RE::ActorValue::kConjuration);
		CFramework_Tomes_DG::Data.PopulateSpellTomes(Tomes_DG_D_NameArray, Tomes_DG_D_FormArray, Tomes_DG_D_BoolArray, Tomes_DG_D_TextArray, RE::ActorValue::kDestruction);
		CFramework_Tomes_DG::Data.PopulateSpellTomes(Tomes_DG_I_NameArray, Tomes_DG_I_FormArray, Tomes_DG_I_BoolArray, Tomes_DG_I_TextArray, RE::ActorValue::kIllusion);
		CFramework_Tomes_DG::Data.PopulateSpellTomes(Tomes_DG_R_NameArray, Tomes_DG_R_FormArray, Tomes_DG_R_BoolArray, Tomes_DG_R_TextArray, RE::ActorValue::kRestoration);

		Tomes_DG_A_EntriesTotal = Tomes_DG_A_FormArray.size();
		Tomes_DG_C_EntriesTotal = Tomes_DG_C_FormArray.size();
		Tomes_DG_D_EntriesTotal = Tomes_DG_D_FormArray.size();
		Tomes_DG_I_EntriesTotal = Tomes_DG_I_FormArray.size();
		Tomes_DG_R_EntriesTotal = Tomes_DG_R_FormArray.size();

		Tomes_DG_A_EntriesFound = std::ranges::count(Tomes_DG_A_BoolArray, true);
		Tomes_DG_C_EntriesFound = std::ranges::count(Tomes_DG_C_BoolArray, true);
		Tomes_DG_D_EntriesFound = std::ranges::count(Tomes_DG_D_BoolArray, true);
		Tomes_DG_I_EntriesFound = std::ranges::count(Tomes_DG_I_BoolArray, true);
		Tomes_DG_R_EntriesFound = std::ranges::count(Tomes_DG_R_BoolArray, true);

		//Spell Tomes (Dragonborn)
		CFramework_Tomes_DB::Data.CompileFormArray(CFramework_Books::Tomes_DB, "Dragonborn.esm");
		CFramework_Tomes_DB::Data.MergeAsCollectable();
		CFramework_Tomes_DB::Data.PopulateSpellTomes(Tomes_DB_A_NameArray, Tomes_DB_A_FormArray, Tomes_DB_A_BoolArray, Tomes_DB_A_TextArray, RE::ActorValue::kAlteration);
		CFramework_Tomes_DB::Data.PopulateSpellTomes(Tomes_DB_C_NameArray, Tomes_DB_C_FormArray, Tomes_DB_C_BoolArray, Tomes_DB_C_TextArray, RE::ActorValue::kConjuration);
		CFramework_Tomes_DB::Data.PopulateSpellTomes(Tomes_DB_D_NameArray, Tomes_DB_D_FormArray, Tomes_DB_D_BoolArray, Tomes_DB_D_TextArray, RE::ActorValue::kDestruction);
		CFramework_Tomes_DB::Data.PopulateSpellTomes(Tomes_DB_I_NameArray, Tomes_DB_I_FormArray, Tomes_DB_I_BoolArray, Tomes_DB_I_TextArray, RE::ActorValue::kIllusion);
		CFramework_Tomes_DB::Data.PopulateSpellTomes(Tomes_DB_R_NameArray, Tomes_DB_R_FormArray, Tomes_DB_R_BoolArray, Tomes_DB_R_TextArray, RE::ActorValue::kRestoration);

		Tomes_DB_A_EntriesTotal = Tomes_DB_A_FormArray.size();
		Tomes_DB_C_EntriesTotal = Tomes_DB_C_FormArray.size();
		Tomes_DB_D_EntriesTotal = Tomes_DB_D_FormArray.size();
		Tomes_DB_I_EntriesTotal = Tomes_DB_I_FormArray.size();
		Tomes_DB_R_EntriesTotal = Tomes_DB_R_FormArray.size();

		Tomes_DB_A_EntriesFound = std::ranges::count(Tomes_DB_A_BoolArray, true);
		Tomes_DB_C_EntriesFound = std::ranges::count(Tomes_DB_C_BoolArray, true);
		Tomes_DB_D_EntriesFound = std::ranges::count(Tomes_DB_D_BoolArray, true);
		Tomes_DB_I_EntriesFound = std::ranges::count(Tomes_DB_I_BoolArray, true);
		Tomes_DB_R_EntriesFound = std::ranges::count(Tomes_DB_R_BoolArray, true);

		//Spell Tomes (Creation Club)
		CFramework_Tomes_CC::Data.CompileFormArray(CFramework_Books::Tomes_CC, "");
		CFramework_Tomes_CC::Data.MergeAsCollectable();
		CFramework_Tomes_CC::Data.PopulateSpellTomes(Tomes_CC_A_NameArray, Tomes_CC_A_FormArray, Tomes_CC_A_BoolArray, Tomes_CC_A_TextArray, RE::ActorValue::kAlteration);
		CFramework_Tomes_CC::Data.PopulateSpellTomes(Tomes_CC_C_NameArray, Tomes_CC_C_FormArray, Tomes_CC_C_BoolArray, Tomes_CC_C_TextArray, RE::ActorValue::kConjuration);
		CFramework_Tomes_CC::Data.PopulateSpellTomes(Tomes_CC_D_NameArray, Tomes_CC_D_FormArray, Tomes_CC_D_BoolArray, Tomes_CC_D_TextArray, RE::ActorValue::kDestruction);
		CFramework_Tomes_CC::Data.PopulateSpellTomes(Tomes_CC_I_NameArray, Tomes_CC_I_FormArray, Tomes_CC_I_BoolArray, Tomes_CC_I_TextArray, RE::ActorValue::kIllusion);
		CFramework_Tomes_CC::Data.PopulateSpellTomes(Tomes_CC_R_NameArray, Tomes_CC_R_FormArray, Tomes_CC_R_BoolArray, Tomes_CC_R_TextArray, RE::ActorValue::kRestoration);

		Tomes_CC_A_EntriesTotal = Tomes_CC_A_FormArray.size();
		Tomes_CC_C_EntriesTotal = Tomes_CC_C_FormArray.size();
		Tomes_CC_D_EntriesTotal = Tomes_CC_D_FormArray.size();
		Tomes_CC_I_EntriesTotal = Tomes_CC_I_FormArray.size();
		Tomes_CC_R_EntriesTotal = Tomes_CC_R_FormArray.size();

		Tomes_CC_A_EntriesFound = std::ranges::count(Tomes_CC_A_BoolArray, true);
		Tomes_CC_C_EntriesFound = std::ranges::count(Tomes_CC_C_BoolArray, true);
		Tomes_CC_D_EntriesFound = std::ranges::count(Tomes_CC_D_BoolArray, true);
		Tomes_CC_I_EntriesFound = std::ranges::count(Tomes_CC_I_BoolArray, true);
		Tomes_CC_R_EntriesFound = std::ranges::count(Tomes_CC_R_BoolArray, true);

		//Books
		CFramework_Books_AG::Data.CompileFormArray(CFramework_Books::Books_AG, "Skyrim.esm");
		CFramework_Books_HS::Data.CompileFormArray(CFramework_Books::Books_HS, "Skyrim.esm");
		CFramework_Books_TY::Data.CompileFormArray(CFramework_Books::Books_TY, "Skyrim.esm");
		CFramework_Skill_SK::Data.CompileFormArray(CFramework_Books::Skill_SK, "Skyrim.esm");
		CFramework_Books_DG::Data.CompileFormArray(CFramework_Books::Books_DG, "Dawnguard.esm");
		CFramework_Books_DB::Data.CompileFormArray(CFramework_Books::Books_DB, "Dragonborn.esm");
		CFramework_TMaps_SK::Data.CompileFormArray(CFramework_Books::TMaps_SK, "Skyrim.esm");
		CFramework_Books_CC::Data.CompileFormArray(CFramework_Books::Books_CC, "");

		CFramework_Books_AG::Data.MergeAsCollectable();
		CFramework_Books_HS::Data.MergeAsCollectable();
		CFramework_Books_TY::Data.MergeAsCollectable();
		CFramework_Skill_SK::Data.MergeAsCollectable();
		CFramework_Books_DG::Data.MergeAsCollectable();
		CFramework_Books_DB::Data.MergeAsCollectable();
		CFramework_TMaps_SK::Data.MergeAsCollectable();
		CFramework_Books_CC::Data.MergeAsCollectable();

		CFramework_Books_AG::Data.Populate(Books_AG_NameArray, Books_AG_FormArray, Books_AG_BoolArray, Books_AG_TextArray, false, 1);
		CFramework_Books_HS::Data.Populate(Books_HS_NameArray, Books_HS_FormArray, Books_HS_BoolArray, Books_HS_TextArray, false, 1);
		CFramework_Books_TY::Data.Populate(Books_TY_NameArray, Books_TY_FormArray, Books_TY_BoolArray, Books_TY_TextArray, false, 1);
		CFramework_Skill_SK::Data.Populate(Skill_SK_NameArray, Skill_SK_FormArray, Skill_SK_BoolArray, Skill_SK_TextArray, false, 1);
		CFramework_Books_DG::Data.Populate(Books_DG_NameArray, Books_DG_FormArray, Books_DG_BoolArray, Books_DG_TextArray, false, 1);
		CFramework_Books_DB::Data.Populate(Books_DB_NameArray, Books_DB_FormArray, Books_DB_BoolArray, Books_DB_TextArray, false, 1);
		CFramework_TMaps_SK::Data.Populate(TMaps_SK_NameArray, TMaps_SK_FormArray, TMaps_SK_BoolArray, TMaps_SK_TextArray, false, 1);
		CFramework_Books_CC::Data.Populate(Books_CC_NameArray, Books_CC_FormArray, Books_CC_BoolArray, Books_CC_TextArray, false, 1);

		if (Serialization::CompletionistData::IsModInstalled("treasure_hunt.esp"))
		{
			CFramework_TMaps_NT::Data.CompileFormArray(CFramework_Books::TMaps_NT, "treasure_hunt.esp");
			CFramework_TMaps_NT::Data.MergeAsCollectable();
			CFramework_TMaps_NT::Data.Populate(TMaps_NT_NameArray, TMaps_NT_FormArray, TMaps_NT_BoolArray, TMaps_NT_TextArray, false, 1);

			TMaps_NT_EntriesTotal = TMaps_NT_FormArray.size();
			TMaps_NT_EntriesFound = std::ranges::count(TMaps_NT_BoolArray, true);
		}

		if (Serialization::CompletionistData::IsModInstalled("TreasureHunter.esp"))
		{
			CFramework_TMaps_TH::Data.CompileFormArray(CFramework_Books::TMaps_TH, "TreasureHunter.esp");
			CFramework_TMaps_TH::Data.MergeAsCollectable();
			CFramework_TMaps_TH::Data.Populate(TMaps_TH_NameArray, TMaps_TH_FormArray, TMaps_TH_BoolArray, TMaps_TH_TextArray, false, 1);

			TMaps_TH_EntriesTotal = TMaps_TH_FormArray.size();
			TMaps_TH_EntriesFound = std::ranges::count(TMaps_TH_BoolArray, true);
		}

		Books_AG_EntriesTotal = Books_AG_FormArray.size();
		Books_AG_EntriesFound = std::ranges::count(Books_AG_BoolArray, true);

		Books_HS_EntriesTotal = Books_HS_FormArray.size();
		Books_HS_EntriesFound = std::ranges::count(Books_HS_BoolArray, true);

		Books_TY_EntriesTotal = Books_TY_FormArray.size();
		Books_TY_EntriesFound = std::ranges::count(Books_TY_BoolArray, true);

		Skill_SK_EntriesTotal = Skill_SK_FormArray.size();
		Skill_SK_EntriesFound = std::ranges::count(Skill_SK_BoolArray, true);

		Books_DG_EntriesTotal = Books_DG_FormArray.size();
		Books_DG_EntriesFound = std::ranges::count(Books_DG_BoolArray, true);

		Books_DB_EntriesTotal = Books_DB_FormArray.size();
		Books_DB_EntriesFound = std::ranges::count(Books_DB_BoolArray, true);

		TMaps_SK_EntriesTotal = TMaps_SK_FormArray.size();
		TMaps_SK_EntriesFound = std::ranges::count(TMaps_SK_BoolArray, true);

		Books_CC_EntriesTotal = Books_CC_FormArray.size();
		Books_CC_EntriesFound = std::ranges::count(Books_CC_BoolArray, true);
	}

	void CHandler::InstallSearchTerms()
	{
		for (auto i = 0; i < Books_AG_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Books_AG_NameArray[i], "$MCMPageBooks1", FrameworkAPI::GetBookCategoryType(Books_AG_FormArray[i])));
		}

		for (auto i = 0; i < Books_HS_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Books_HS_NameArray[i], "$MCMPageBooks2", FrameworkAPI::GetBookCategoryType(Books_HS_FormArray[i])));
		}

		for (auto i = 0; i < Books_TY_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Books_TY_NameArray[i], "$MCMPageBooks3", FrameworkAPI::GetBookCategoryType(Books_TY_FormArray[i])));
		}

		for (auto i = 0; i < Skill_SK_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Skill_SK_NameArray[i], "$MCMPageBooks5", FrameworkAPI::GetBookCategoryType(Skill_SK_FormArray[i])));
		}

		for (auto i = 0; i < Tomes_SK_A_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_SK_A_NameArray[i], "$MCMPageBooks6", FrameworkAPI::GetBookCategoryType(Tomes_SK_A_FormArray[i])));
		}

		for (auto i = 0; i < Tomes_SK_C_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_SK_C_NameArray[i], "$MCMPageBooks6", FrameworkAPI::GetBookCategoryType(Tomes_SK_C_FormArray[i])));
		}

		for (auto i = 0; i < Tomes_SK_D_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_SK_D_NameArray[i], "$MCMPageBooks6", FrameworkAPI::GetBookCategoryType(Tomes_SK_D_FormArray[i])));
		}

		for (auto i = 0; i < Tomes_SK_I_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_SK_I_NameArray[i], "$MCMPageBooks6", FrameworkAPI::GetBookCategoryType(Tomes_SK_I_FormArray[i])));
		}

		for (auto i = 0; i < Tomes_SK_R_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_SK_R_NameArray[i], "$MCMPageBooks6", FrameworkAPI::GetBookCategoryType(Tomes_SK_R_FormArray[i])));
		}

		for (auto i = 0; i < Books_DG_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Books_DG_NameArray[i], "$MCMPageBooks7", FrameworkAPI::GetBookCategoryType(Books_DG_FormArray[i])));
		}

		for (auto i = 0; i < Tomes_DG_A_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_DG_A_NameArray[i], "$MCMPageBooks7", FrameworkAPI::GetBookCategoryType(Tomes_DG_A_FormArray[i])));
		}

		for (auto i = 0; i < Tomes_DG_C_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_DG_C_NameArray[i], "$MCMPageBooks7", FrameworkAPI::GetBookCategoryType(Tomes_DG_C_FormArray[i])));
		}

		for (auto i = 0; i < Tomes_DG_D_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_DG_D_NameArray[i], "$MCMPageBooks7", FrameworkAPI::GetBookCategoryType(Tomes_DG_D_FormArray[i])));
		}

		for (auto i = 0; i < Tomes_DG_I_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_DG_I_NameArray[i], "$MCMPageBooks7", FrameworkAPI::GetBookCategoryType(Tomes_DG_I_FormArray[i])));
		}

		for (auto i = 0; i < Tomes_DG_R_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_DG_R_NameArray[i], "$MCMPageBooks7", FrameworkAPI::GetBookCategoryType(Tomes_DG_R_FormArray[i])));
		}

		for (auto i = 0; i < Books_DB_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Books_DB_NameArray[i], "$MCMPageBooks8", FrameworkAPI::GetBookCategoryType(Books_DB_FormArray[i])));
		}

		for (auto i = 0; i < Tomes_DB_A_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_DB_A_NameArray[i], "$MCMPageBooks8", FrameworkAPI::GetBookCategoryType(Tomes_DB_A_FormArray[i])));
		}

		for (auto i = 0; i < Tomes_DB_C_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_DB_C_NameArray[i], "$MCMPageBooks8", FrameworkAPI::GetBookCategoryType(Tomes_DB_C_FormArray[i])));
		}

		for (auto i = 0; i < Tomes_DB_D_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_DB_D_NameArray[i], "$MCMPageBooks8", FrameworkAPI::GetBookCategoryType(Tomes_DB_D_FormArray[i])));
		}

		for (auto i = 0; i < Tomes_DB_I_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_DB_I_NameArray[i], "$MCMPageBooks8", FrameworkAPI::GetBookCategoryType(Tomes_DB_I_FormArray[i])));
		}

		for (auto i = 0; i < Tomes_DB_R_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_DB_R_NameArray[i], "$MCMPageBooks8", FrameworkAPI::GetBookCategoryType(Tomes_DB_R_FormArray[i])));
		}

		for (auto i = 0; i < TMaps_SK_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(TMaps_SK_NameArray[i], "$MCMPageBooks4", FrameworkAPI::GetBookCategoryType(TMaps_SK_FormArray[i])));
		}

		for (auto i = 0; i < TMaps_NT_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(TMaps_NT_NameArray[i], "$MCMPageBooks4", FrameworkAPI::GetBookCategoryType(TMaps_NT_FormArray[i])));
		}

		for (auto i = 0; i < TMaps_TH_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(TMaps_TH_NameArray[i], "$MCMPageBooks4", FrameworkAPI::GetBookCategoryType(TMaps_TH_FormArray[i])));
		}

		for (auto i = 0; i < Books_CC_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Books_CC_NameArray[i], "$MCMPageBooksCC", FrameworkAPI::GetBookCategoryType(Books_CC_FormArray[i])));
		}

		for (auto i = 0; i < Tomes_CC_A_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_CC_A_NameArray[i], "$MCMPageBooksCC", FrameworkAPI::GetBookCategoryType(Tomes_CC_A_FormArray[i])));
		}

		for (auto i = 0; i < Tomes_CC_C_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_CC_C_NameArray[i], "$MCMPageBooksCC", FrameworkAPI::GetBookCategoryType(Tomes_CC_C_FormArray[i])));
		}

		for (auto i = 0; i < Tomes_CC_D_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_CC_D_NameArray[i], "$MCMPageBooksCC", FrameworkAPI::GetBookCategoryType(Tomes_CC_D_FormArray[i])));
		}

		for (auto i = 0; i < Tomes_CC_I_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_CC_I_NameArray[i], "$MCMPageBooksCC", FrameworkAPI::GetBookCategoryType(Tomes_CC_I_FormArray[i])));
		}

		for (auto i = 0; i < Tomes_CC_R_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Tomes_CC_R_NameArray[i], "$MCMPageBooksCC", FrameworkAPI::GetBookCategoryType(Tomes_CC_R_FormArray[i])));
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		//Spell Tomes (Skyrim)
		for (auto i = 0; i < Tomes_SK_A_FormArray.size(); i++) {
			Tomes_SK_A_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_SK_A_FormArray[i]);
		}

		for (auto i = 0; i < Tomes_SK_C_FormArray.size(); i++) {
			Tomes_SK_C_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_SK_C_FormArray[i]);
		}

		for (auto i = 0; i < Tomes_SK_D_FormArray.size(); i++) {
			Tomes_SK_D_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_SK_D_FormArray[i]);
		}

		for (auto i = 0; i < Tomes_SK_I_FormArray.size(); i++) {
			Tomes_SK_I_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_SK_I_FormArray[i]);
		}

		for (auto i = 0; i < Tomes_SK_R_FormArray.size(); i++) {
			Tomes_SK_R_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_SK_R_FormArray[i]);
		}

		Tomes_SK_A_EntriesTotal = Tomes_SK_A_FormArray.size();
		Tomes_SK_C_EntriesTotal = Tomes_SK_C_FormArray.size();
		Tomes_SK_D_EntriesTotal = Tomes_SK_D_FormArray.size();
		Tomes_SK_I_EntriesTotal = Tomes_SK_I_FormArray.size();
		Tomes_SK_R_EntriesTotal = Tomes_SK_R_FormArray.size();

		Tomes_SK_A_EntriesFound = std::ranges::count(Tomes_SK_A_BoolArray, true);
		Tomes_SK_C_EntriesFound = std::ranges::count(Tomes_SK_C_BoolArray, true);
		Tomes_SK_D_EntriesFound = std::ranges::count(Tomes_SK_D_BoolArray, true);
		Tomes_SK_I_EntriesFound = std::ranges::count(Tomes_SK_I_BoolArray, true);
		Tomes_SK_R_EntriesFound = std::ranges::count(Tomes_SK_R_BoolArray, true);

		//Spell Tomes (Dawnguard)
		for (auto i = 0; i < Tomes_DG_A_FormArray.size(); i++) {
			Tomes_DG_A_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_DG_A_FormArray[i]);
		}

		for (auto i = 0; i < Tomes_DG_C_FormArray.size(); i++) {
			Tomes_DG_C_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_DG_C_FormArray[i]);
		}

		for (auto i = 0; i < Tomes_DG_D_FormArray.size(); i++) {
			Tomes_DG_D_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_DG_D_FormArray[i]);
		}

		for (auto i = 0; i < Tomes_DG_I_FormArray.size(); i++) {
			Tomes_DG_I_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_DG_I_FormArray[i]);
		}

		for (auto i = 0; i < Tomes_DG_R_FormArray.size(); i++) {
			Tomes_DG_R_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_DG_R_FormArray[i]);
		}

		Tomes_DG_A_EntriesTotal = Tomes_DG_A_FormArray.size();
		Tomes_DG_C_EntriesTotal = Tomes_DG_C_FormArray.size();
		Tomes_DG_D_EntriesTotal = Tomes_DG_D_FormArray.size();
		Tomes_DG_I_EntriesTotal = Tomes_DG_I_FormArray.size();
		Tomes_DG_R_EntriesTotal = Tomes_DG_R_FormArray.size();

		Tomes_DG_A_EntriesFound = std::ranges::count(Tomes_DG_A_BoolArray, true);
		Tomes_DG_C_EntriesFound = std::ranges::count(Tomes_DG_C_BoolArray, true);
		Tomes_DG_D_EntriesFound = std::ranges::count(Tomes_DG_D_BoolArray, true);
		Tomes_DG_I_EntriesFound = std::ranges::count(Tomes_DG_I_BoolArray, true);
		Tomes_DG_R_EntriesFound = std::ranges::count(Tomes_DG_R_BoolArray, true);

		//Spell Tomes (Dragonborn)
		for (auto i = 0; i < Tomes_DB_A_FormArray.size(); i++) {
			Tomes_DB_A_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_DB_A_FormArray[i]);
		}

		for (auto i = 0; i < Tomes_DB_C_FormArray.size(); i++) {
			Tomes_DB_C_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_DB_C_FormArray[i]);
		}

		for (auto i = 0; i < Tomes_DB_D_FormArray.size(); i++) {
			Tomes_DB_D_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_DB_D_FormArray[i]);
		}

		for (auto i = 0; i < Tomes_DB_I_FormArray.size(); i++) {
			Tomes_DB_I_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_DB_I_FormArray[i]);
		}

		for (auto i = 0; i < Tomes_DB_R_FormArray.size(); i++) {
			Tomes_DB_R_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_DB_R_FormArray[i]);
		}

		Tomes_DB_A_EntriesTotal = Tomes_DB_A_FormArray.size();
		Tomes_DB_C_EntriesTotal = Tomes_DB_C_FormArray.size();
		Tomes_DB_D_EntriesTotal = Tomes_DB_D_FormArray.size();
		Tomes_DB_I_EntriesTotal = Tomes_DB_I_FormArray.size();
		Tomes_DB_R_EntriesTotal = Tomes_DB_R_FormArray.size();

		Tomes_DB_A_EntriesFound = std::ranges::count(Tomes_DB_A_BoolArray, true);
		Tomes_DB_C_EntriesFound = std::ranges::count(Tomes_DB_C_BoolArray, true);
		Tomes_DB_D_EntriesFound = std::ranges::count(Tomes_DB_D_BoolArray, true);
		Tomes_DB_I_EntriesFound = std::ranges::count(Tomes_DB_I_BoolArray, true);
		Tomes_DB_R_EntriesFound = std::ranges::count(Tomes_DB_R_BoolArray, true);

		//Spell Tomes (Creation Club)
		for (auto i = 0; i < Tomes_CC_A_FormArray.size(); i++) {
			Tomes_CC_A_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_CC_A_FormArray[i]);
		}

		for (auto i = 0; i < Tomes_CC_C_FormArray.size(); i++) {
			Tomes_CC_C_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_CC_C_FormArray[i]);
		}

		for (auto i = 0; i < Tomes_CC_D_FormArray.size(); i++) {
			Tomes_CC_D_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_CC_D_FormArray[i]);
		}

		for (auto i = 0; i < Tomes_CC_I_FormArray.size(); i++) {
			Tomes_CC_I_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_CC_I_FormArray[i]);
		}

		for (auto i = 0; i < Tomes_CC_R_FormArray.size(); i++) {
			Tomes_CC_R_BoolArray[i] = FrameworkAPI::IsBookKnown(Tomes_CC_R_FormArray[i]);
		}

		Tomes_CC_A_EntriesTotal = Tomes_CC_A_FormArray.size();
		Tomes_CC_C_EntriesTotal = Tomes_CC_C_FormArray.size();
		Tomes_CC_D_EntriesTotal = Tomes_CC_D_FormArray.size();
		Tomes_CC_I_EntriesTotal = Tomes_CC_I_FormArray.size();
		Tomes_CC_R_EntriesTotal = Tomes_CC_R_FormArray.size();

		Tomes_CC_A_EntriesFound = std::ranges::count(Tomes_CC_A_BoolArray, true);
		Tomes_CC_C_EntriesFound = std::ranges::count(Tomes_CC_C_BoolArray, true);
		Tomes_CC_D_EntriesFound = std::ranges::count(Tomes_CC_D_BoolArray, true);
		Tomes_CC_I_EntriesFound = std::ranges::count(Tomes_CC_I_BoolArray, true);
		Tomes_CC_R_EntriesFound = std::ranges::count(Tomes_CC_R_BoolArray, true);

		//Books
		for (auto i = 0; i < Books_AG_FormArray.size(); i++) {
			Books_AG_BoolArray[i] = FrameworkAPI::IsBookKnown(Books_AG_FormArray[i]);
		}

		for (auto i = 0; i < Books_HS_FormArray.size(); i++) {
			Books_HS_BoolArray[i] = FrameworkAPI::IsBookKnown(Books_HS_FormArray[i]);
		}

		for (auto i = 0; i < Books_TY_FormArray.size(); i++) {
			Books_TY_BoolArray[i] = FrameworkAPI::IsBookKnown(Books_TY_FormArray[i]);
		}

		for (auto i = 0; i < Skill_SK_FormArray.size(); i++) {
			Skill_SK_BoolArray[i] = FrameworkAPI::IsBookKnown(Skill_SK_FormArray[i]);
		}

		for (auto i = 0; i < Books_DG_FormArray.size(); i++) {
			Books_DG_BoolArray[i] = FrameworkAPI::IsBookKnown(Books_DG_FormArray[i]);
		}

		for (auto i = 0; i < Books_DB_FormArray.size(); i++) {
			Books_DB_BoolArray[i] = FrameworkAPI::IsBookKnown(Books_DB_FormArray[i]);
		}

		for (auto i = 0; i < TMaps_SK_FormArray.size(); i++) {
			TMaps_SK_BoolArray[i] = FrameworkAPI::IsBookKnown(TMaps_SK_FormArray[i]);
		}

		for (auto i = 0; i < TMaps_NT_FormArray.size(); i++) {
			TMaps_NT_BoolArray[i] = FrameworkAPI::IsBookKnown(TMaps_NT_FormArray[i]);
		}

		for (auto i = 0; i < TMaps_TH_FormArray.size(); i++) {
			TMaps_TH_BoolArray[i] = FrameworkAPI::IsBookKnown(TMaps_TH_FormArray[i]);
		}

		for (auto i = 0; i < Books_CC_FormArray.size(); i++) {
			Books_CC_BoolArray[i] = FrameworkAPI::IsBookKnown(Books_CC_FormArray[i]);
		}

		Books_AG_EntriesTotal = Books_AG_FormArray.size();
		Books_AG_EntriesFound = std::ranges::count(Books_AG_BoolArray, true);

		Books_HS_EntriesTotal = Books_HS_FormArray.size();
		Books_HS_EntriesFound = std::ranges::count(Books_HS_BoolArray, true);

		Books_TY_EntriesTotal = Books_TY_FormArray.size();
		Books_TY_EntriesFound = std::ranges::count(Books_TY_BoolArray, true);

		Skill_SK_EntriesTotal = Skill_SK_FormArray.size();
		Skill_SK_EntriesFound = std::ranges::count(Skill_SK_BoolArray, true);

		Books_DG_EntriesTotal = Books_DG_FormArray.size();
		Books_DG_EntriesFound = std::ranges::count(Books_DG_BoolArray, true);

		Books_DB_EntriesTotal = Books_DB_FormArray.size();
		Books_DB_EntriesFound = std::ranges::count(Books_DB_BoolArray, true);

		TMaps_SK_EntriesTotal = TMaps_SK_FormArray.size();
		TMaps_SK_EntriesFound = std::ranges::count(TMaps_SK_BoolArray, true);

		TMaps_NT_EntriesTotal = TMaps_NT_FormArray.size();
		TMaps_NT_EntriesFound = std::ranges::count(TMaps_NT_BoolArray, true);

		TMaps_TH_EntriesTotal = TMaps_TH_FormArray.size();
		TMaps_TH_EntriesFound = std::ranges::count(TMaps_TH_BoolArray, true);

		Books_CC_EntriesTotal = Books_CC_FormArray.size();
		Books_CC_EntriesFound = std::ranges::count(Books_CC_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install CC Books ) -------
	//---------------------------------------------------

	void CHandler::Install_CCB() {

		CFramework_Books_CC::Data.AddForm(0x00085B, "ccbgssse063-ba_ebony.esl");			// The Crimson Dirks, v1
		CFramework_Books_CC::Data.AddForm(0x000824, "ccbgssse056-ba_silver.esl");			// The Crimson Dirks, v2
		CFramework_Books_CC::Data.AddForm(0x00081A, "ccbgssse050-ba_daedric.esl");			// The Crimson Dirks, v3
		CFramework_Books_CC::Data.AddForm(0x000819, "ccbgssse050-ba_daedric.esl");			// Death of a Crimson Dirk
		CFramework_Books_CC::Data.AddForm(0x000819, "ccbgssse060-ba_dragonscale.esl");		// The Crimson Dirks, v4
		CFramework_Books_CC::Data.AddForm(0x000847, "ccbgssse051-ba_daedricmail.esl");		// The Crimson Dirks, v5
		CFramework_Books_CC::Data.AddForm(0x000839, "ccbgssse055-ba_orcishscaled.esl");		// The Crimson Dirks, v6
		CFramework_Books_CC::Data.AddForm(0x000828, "ccbgssse064-ba_elven.esl");			// The Crimson Dirks, v7
		CFramework_Books_CC::Data.AddForm(0x000821, "ccbgssse062-ba_dwarvenmail.esl");		// The Crimson Dirks, v8
		CFramework_Books_CC::Data.AddForm(0x00081A, "ccbgssse061-ba_dwarven.esl");			// The Crimson Dirks, v9
		CFramework_Books_CC::Data.AddForm(0x000809, "ccbgssse031-advcyrus.esm");			// The Restless
		CFramework_Books_CC::Data.AddForm(0x0008A2, "cctwbsse001-puzzledungeon.esm");		// Forgotten Seasons, v1
		CFramework_Books_CC::Data.AddForm(0x00083D, "ccedhsse003-redguard.esl");			// The Alik'r
		CFramework_Books_CC::Data.AddForm(0x0008D3, "ccedhsse003-redguard.esl");			// The Ebon Arm
		CFramework_Books_CC::Data.AddForm(0x000AEC, "ccbgssse025-advdsgs.esm");				// Heretical Thoughts
		CFramework_Books_CC::Data.AddForm(0x000AEE, "ccbgssse025-advdsgs.esm");				// Saints and Seducers
		CFramework_Books_CC::Data.AddForm(0x000B97, "ccbgssse025-advdsgs.esm");				// The Blessings of Sheogorath
		CFramework_Books_CC::Data.AddForm(0x000804, "ccbgssse045-hasedoki.esl");			// The Light and the Dark
		CFramework_Books_CC::Data.AddForm(0x000805, "ccbgssse045-hasedoki.esl");			// Arkay the Enemy
		CFramework_Books_CC::Data.AddForm(0x000839, "ccbgssse045-hasedoki.esl");			// To Raise the Living
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install CC Tomes ) -------
	//---------------------------------------------------

	void CHandler::Install_CCT() {

		CFramework_Tomes_CC::Data.AddForm(0x000815, "ccbgssse014-spellpack01.esl");		// Spell Tome: Unbounded Flames
		CFramework_Tomes_CC::Data.AddForm(0x000816, "ccbgssse014-spellpack01.esl");		// Spell Tome: Unbounded Freezing
		CFramework_Tomes_CC::Data.AddForm(0x000817, "ccbgssse014-spellpack01.esl");		// Spell Tome: Unbounded Storms
		CFramework_Tomes_CC::Data.AddForm(0x000818, "ccbgssse014-spellpack01.esl");		// Spell Tome: Paralysis Rune
		CFramework_Tomes_CC::Data.AddForm(0x000861, "ccbgssse014-spellpack01.esl");		// Spell Tome: Pride of Hirstaang
		CFramework_Tomes_CC::Data.AddForm(0x000862, "ccbgssse014-spellpack01.esl");		// Spell Tome: Elemental Flare
		CFramework_Tomes_CC::Data.AddForm(0x000863, "ccbgssse014-spellpack01.esl");		// Spell Tome: Elemental Bolt
		CFramework_Tomes_CC::Data.AddForm(0x000864, "ccbgssse014-spellpack01.esl");		// Spell Tome: Elemental Burst
		CFramework_Tomes_CC::Data.AddForm(0x000865, "ccbgssse014-spellpack01.esl");		// Spell Tome: Elemental Blast
		CFramework_Tomes_CC::Data.AddForm(0x000866, "ccbgssse014-spellpack01.esl");		// Spell Tome: Orum's Aquatic Escape
		CFramework_Tomes_CC::Data.AddForm(0x000867, "ccbgssse014-spellpack01.esl");		// Spell Tome: Fenrik's Welcome
		CFramework_Tomes_CC::Data.AddForm(0x000868, "ccbgssse014-spellpack01.esl");		// Spell Tome: Mara's Wrath
		CFramework_Tomes_CC::Data.AddForm(0x000869, "ccbgssse014-spellpack01.esl");		// Spell Tome: Choking Grasp
		CFramework_Tomes_CC::Data.AddForm(0x00086A, "ccbgssse014-spellpack01.esl");		// Spell Tome: Strangulation
		CFramework_Tomes_CC::Data.AddForm(0x00086B, "ccbgssse014-spellpack01.esl");		// Spell Tome: Hangman's Noose
		CFramework_Tomes_CC::Data.AddForm(0x00086C, "ccbgssse014-spellpack01.esl");		// Spell Tome: Touch of Death
		CFramework_Tomes_CC::Data.AddForm(0x000816, "ccbgssse002-exoticarrows.esl");	// Spell Tome: Bound Quiver
		CFramework_Tomes_CC::Data.AddForm(0x00082B, "ccbgssse002-exoticarrows.esl");	// Spell Tome: Telekinesis Arrows
		CFramework_Tomes_CC::Data.AddForm(0x000889, "ccvsvsse003-necroarts.esl");		// Spell Tome: Banish Undead
		CFramework_Tomes_CC::Data.AddForm(0x00088A, "ccvsvsse003-necroarts.esl");		// Spell Tome: Conjure Undying Ghost
		CFramework_Tomes_CC::Data.AddForm(0x00088B, "ccvsvsse003-necroarts.esl");		// Spell Tome: Conjure Ancient Deathpriest
		CFramework_Tomes_CC::Data.AddForm(0x00088C, "ccvsvsse003-necroarts.esl");		// Spell Tome: Conjure Skeleton Minion
		CFramework_Tomes_CC::Data.AddForm(0x00088D, "ccvsvsse003-necroarts.esl");		// Spell Tome: Necromancer's Ritual
		CFramework_Tomes_CC::Data.AddForm(0x00088E, "ccvsvsse003-necroarts.esl");		// Spell Tome: Conjure Cursed Spectre
		CFramework_Tomes_CC::Data.AddForm(0x00088F, "ccvsvsse003-necroarts.esl");		// Spell Tome: Conjure Haunting Spirit
		CFramework_Tomes_CC::Data.AddForm(0x000890, "ccvsvsse003-necroarts.esl");		// Spell Tome: Conjure Tomb Guardian
		CFramework_Tomes_CC::Data.AddForm(0x000891, "ccvsvsse003-necroarts.esl");		// Spell Tome: Conjure Bone Colossus
		CFramework_Tomes_CC::Data.AddForm(0x000892, "ccvsvsse003-necroarts.esl");		// Spell Tome: Conjure Skeleton Warlock
		CFramework_Tomes_CC::Data.AddForm(0x000893, "ccvsvsse003-necroarts.esl");		// Spell Tome: Conjure Skeleton Champion
		CFramework_Tomes_CC::Data.AddForm(0x000894, "ccvsvsse003-necroarts.esl");		// Spell Tome: Conjure Skeleton Marksman
		CFramework_Tomes_CC::Data.AddForm(0x000896, "ccvsvsse003-necroarts.esl");		// Spell Tome: Soul Split
		CFramework_Tomes_CC::Data.AddForm(0x00086E, "ccbgssse003-zombies.esl");			// Spell Tome: Conjure Zombie
		CFramework_Tomes_CC::Data.AddForm(0x00086F, "ccbgssse003-zombies.esl");			// Spell Tome: Conjure Foul Zombie
		CFramework_Tomes_CC::Data.AddForm(0x000870, "ccbgssse003-zombies.esl");			// Spell Tome: Conjure Putrid Zombie
		CFramework_Tomes_CC::Data.AddForm(0x000871, "ccbgssse003-zombies.esl");			// Spell Tome: Conjure Malignant Zombie
		CFramework_Tomes_CC::Data.AddForm(0x000AF0, "ccbgssse025-advdsgs.esm");			// Spell Tome: Conjure Golden Saint Archer
		CFramework_Tomes_CC::Data.AddForm(0x000AFD, "ccbgssse025-advdsgs.esm");			// Spell Tome: Conjure Golden Saint Warrior
		CFramework_Tomes_CC::Data.AddForm(0x000B0C, "ccbgssse025-advdsgs.esm");			// Spell Tome: Conjure Dark Seducer Archer
		CFramework_Tomes_CC::Data.AddForm(0x000B63, "ccbgssse025-advdsgs.esm");			// Spell Tome: Conjure Dark Seducer Warrior
		CFramework_Tomes_CC::Data.AddForm(0x19D3F9, "ccbgssse025-advdsgs.esm");			// Spell Tome: Conjure Staada
		CFramework_Tomes_CC::Data.AddForm(0x000B64, "ccbgssse067-daedinv.esm");			// Spell Tome: Conjure Daedric Horse
		CFramework_Tomes_CC::Data.AddForm(0x147D93, "ccbgssse067-daedinv.esm");			// Spell Tome: Conjure Ayleid Lich
	}
}