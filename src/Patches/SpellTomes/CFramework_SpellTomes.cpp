#include "Serialization.hpp"
#include "Internal Utility/Events.hpp"
#include "CFramework_Spelltomes.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_SpellTomes {
	using namespace CFramework_Master;

	constexpr std::string_view filename_Apocalypse = "Apocalypse - Magic of Skyrim.esp";
	constexpr std::string_view filename_Odin = "Odin - Skyrim Magic Overhaul.esp";
	constexpr std::string_view filename_Mysticism = "MysticismMagic.esp";
	constexpr std::string_view filename_ForgottenMagic = "ForgottenMagic_Redone.esp";
	constexpr std::string_view filename_Triumvirate = "Triumvirate - Mage Archetypes.esp";

	// clang-format off

	constexpr Serialization::FormArray Apocalypse_Tomes = {
	0x03C52B,0x03C517,0x03E095,0x03D043,0x089706,0x03E601,0x03D5B7,0x05F257,0x03EB8A,0x03E608,
	0x047343,0x03DB27,0x03E094,0x03EB7B,0x03EB8F,0x03EB89,0x03EB90,0x03EB7C,0x03EB83,0x03E606,
	0x03D041,0x089708,0x048910,0x03D048,0x03D5BB,0x03D046,0x03D5B8,0x03D5B1,0x03D045,0x03D5B2,
	0x03D5B6,0x03D049,0x03D5B3,0x089707,0x03D04A,0x03D5B9,0x03D5BA,0x05E224,0x03D5B4,0x03D04B,
	0x03D042,0x089704,0x03D04D,0x03DB1F,0x03DB22,0x03E5FF,0x03E099,0x03D04C,0x03EB8E,0x03C525,
	0x03C527,0x03E5FD,0x03DB21,0x03C51A,0x03EB92,0x03DB28,0x03E60C,0x03E60E,0x03F65B,0x03E605,
	0x03C528,0x03E607,0x03EB84,0x03E096,0x03E097,0x05E219,0x03DB23,0x03DB29,0x03E098,0x03E5FE,
	0x03DB20,0x03E613,0x03EB8D,0x03C52F,0x047341,0x03EB85,0x08970A,0x03DB2A,0x03DB2A,0x03E609,
	0x03DB2B,0x03DB24,0x03EB93,0x03EB94,0x03C529,0x03EB7E,0x03E616,0x03EB7F,0x089711,0x03E092,
	0x03C522,0x03C518,0x03E611,0x03C52C,0x05E218,0x03C52E,0x03E60B,0x03E60D,0x03E603,0x03E612,
	0x03EB80,0x03EB8C,0x03EB81,0x03D5BC,0x03D5B5,0x03C51C,0x08970C,0x03C51B,0x03D047,0x089703,
	0x08970D,0x03C519,0x03EB8B,0x03C51D,0x03DB26,0x03EB87,0x03DB2C,0x03E08F,0x03E614,0x03EB88,
	0x03E60F,0x03E60A,0x03E600,0x03E091,0x03E093,0x03E615,0x03EB82,0x03E090,0x089705,0x03D044,
	0x08970E,0x03C523,0x08970B,0x03C51F,0x03D04E,0x047E25,0x03C524,0x03E604,0x03C530,0x03DB25,
	0x03C521,0x03EB86,0x03C526,0x03E09A,0x089709,0x03C520,0x03E610,0x047345,0x03EB7D,0x08970F,
	0x03C52D,0x03C51E,0x03EB91,0x05E225,0x03E602,0x089710,
	};

	constexpr Serialization::FormArray ForgottenMagic_Tomes = {
	0x01EB94,0x02AF53,0x057D41,0x01BFF9,0x01BFED,0x02BA42,0x01C563,0x03E866,0x02BA43,0x022CF2,
	0x022CF0,0x01BFEF,0x0221FB,0x051C1B,0x01BFEC,0x01EB95,0x01EB92,0x04B575,0x022CF3,0x0216F8,
	0x022CF1,0x01DB49,0x0258A7,0x01BFEE,0x01C561,0x03E868,0x04EB77,0x02BA44,0x03FE2C,0x03D2D3,
	0x0216FA,0x0258AA,0x04E613,0x0258A9,0x0577CE,0x0258A4,0x03A204,0x01C562,0x0221FD,0x04B572,
	0x03ACF4,
	};

	constexpr Serialization::FormArray Mysticism_Tomes = {
	0x7D5825,0x3C7589,0x1901E6,0x3E0AAF,0x3E0AB1,0x36C2C5,0x36C2E0,0xDC99E2,0xE01501,0xDC48DD,
	0xDD3BEE,0xDED0F8,0x3C7584,0xC857B4,0x1A96F7,0x3E5C10,0x3E5C0D,0x2508D0,0x1AE803,0x4B0684,
	0xF3637E,0xF21F4B,0xB1DFBB,0x3E0AB4,0x515B98,0x3E5C0F,0x3E5C17,0x4E3146,0x4E3133,0x4E313D,
	0x4E3145,0xE7FEC7,0x00FFFF,0x010000,0x4E3139,0x4E312C,0x4E3134,0xE7FED1,0xE7FEDA,0x4E313C,
	0x4E312A,0x4E312D,0x4E3142,0x4E3131,0x4E313A,0x4E3137,0x4E312B,0x4E314F,0x4E312F,0x4E3138,
	0xE7FECC,0x01F2F8,0x01F2F9,0x3E5C14,0x3C7577,0x3E0AB8,0x9F8520,0x209A7E,0x9AC5D8,0x209A89,
	0x3E5C1D,0x8C3768,0x4F758F,0x7DA929,0xC671AD,0x3E5C1C,0x478AB3,0x478AB4,0x478AAE,0x3C758B,
	0x48CF0E,0x9F851F,0xAC7E8C,0xA9A576,0x3E5C16,0x3C7571,0x1D7038,0x3A3E10,0x3C7573,0x3C7585,
	0x46469C,0xC4DC38,0x9F851E,0xB1DFBE,0xF3635E,0xF3636C,0x2A19B1,0x3C7574,0xB4B8F8,0x3E5C1E,
	0x478AB8,0x4B0633,0x4B063F,0x3C7586,0xCDB97E,0x176CD1,0x1DC13A,0x478AAC,0x478AB7,0x36C2E8,
	0x3C7587,0x45536F,0x9F8521,0xB18EB0,0x3E5C13,0x3E5C0C,0x3C756E,0x3C756D,0x3C756F,0x3C756C,
	0x3C7583,0x4B0632,0x3E5C18,0x4E314C,0x209A87,0x209A88,0x3C7576,0x9AC5D6,0x209A85,0x209A83,
	0x3E5C1B,0x78E9F8,0x9AC5D5,0xD45F29,0xDBA6D4,0x455371,0x2508CF,0x3E0AA9,0x3E5C1F,0x478AAD,
	0x478AAB,0x9F851D,0x3C7579,0x2508CC,0x4B067E,0x3C7575,0x1AE805,0x3E5C12,0xB467E4,0x3E5C15,
	0x478AB5,0x3C7582,0x25FC02,0x4B063D,0x3E0AB9,0xCF9FB0,0x482CE5,0x46469D,0xCCC63C,0x2508CA,
	0x8AF367,0x9E9214,0xFF19E1,0x4B0649,0x2E8871,0x1D7037,0xA30050,0x4E314D,0x478AAA,0x1B8A16,
	0x36C2DD,0x36C2DE,0x1B8A13,0x705EB3,0x3C7588,0x48CF05,0xD278E4,0x209A84,0x209A86,0x9AC5DA,
	0x9E4105,0x3C757C,0x3C757D,0x3C757F,0x3C757E,0x4B0683,0x4B0635,0x3E0AB5,0x3E0AAB,0x36C2DC,
	0x1D7033,0x3A3E0B,0x1D7035,0x3A3E0C,0x1D7036,
	};

	constexpr Serialization::FormArray Odin_Tomes = {
	0x03DB25,0x03DB2C,0x03E090,0x03E615,0x03EB87,0x0D4351,0x0DD9E9,0x1AFD16,0x1AFD17,0x1AFD18,
	0x1AFD19,0x1AFD1A,0x1AFD1B,0x1AFD1C,0x1AFD1D,0x1AFD1E,0x1AFD1F,0x1AFD21,0x1AFD22,0x1BF053,
	0x20AFD0,0x2100DB,0x2100E3,0x21A2F7,0x21A2F8,0x21F415,0x2C67B9,0x2CB8C4,0x2CB8CA,0x2D5ADB,
	0x21F416,0x21F417,0x22452F,0x22E73B,0x233856,0x233857,0x233858,0x23385F,0x23386C,0x23897E,
	0x23897F,0x238980,0x2611EF,0x2611F9,0x2611FA,0x2611FF,0x261202,0x261203,0x261204,0x261205,
	0x261206,0x261207,0x26120E,0x261211,0x261214,0x261215,0x261216,0x261217,0x26B445,0x26B446,
	0x26B447,0x26B448,0x26B449,0x26B44A,0x26B44B,0x26B44C,0x26B44D,0x26B44E,0x26B44F,0x26B450,
	0x26B451,0x26B452,0x26B453,0x27568B,0x27568C,0x27568F,0x275692,0x275696,0x27569E,0x27A7AB,
	0x27F8B9,0x27F8CE,0x2849D9,0x2849DF,0x293D12,0x293D18,0x293D19,0x293D2A,0x298E2B,0x298E2C,
	0x298E2D,0x298E3E,0x2A3053,0x2A3054,0x2A3055,0x2C16A3,
	};

	//Only used if Apocalypse is not installed.
	constexpr Serialization::FormArray Odin_Crossover_Tomes = {
	0x03C518,0x03C517,0x03C51D,0x03C51C,0x03C525,0x03C528,0x03C52E,0x089704,0x03D047,0x1AFD20,
	0x03E610,0x03EB82,0x03EB90,0x089707,
	};

	constexpr Serialization::FormArray Triumvirate_Tomes = {
	0x41EC71,0x41EC72,0x41EC74,0x41EC73,0x419B21,0x419B54,0x41EC88,0x41EC82,0x41EC84,0x41EC81,
	0x41EC80,0x41EC83,0x41EC7F,0x419B56,0x419B4E,0x419B52,0x419B51,0x419B50,0x419B4F,0x41EC6E,
	0x414A08,0x414A06,0x414A05,0x419B26,0x419B5C,0x419B24,0x419B23,0x419B20,0x41EC7B,0x419B53,
	0x41EC77,0x41EC7A,0x414A0A,0x414A0C,0x41EC7C,0x419B59,0x419B25,0x41EC6D,0x41EC6F,0x419B57,
	0x41EC7E,0x419B5A,0x419B5B,0x41EC75,0x419B1F,0x419B22,0x419B29,0x41EC79,0x41EC86,0x419B28,
	0x419B1E,0x419B27,0x414A0E,0x419B1D,0x419B1B,0x414A04,0x41EC6C,0x414A00,0x41EC78,0x414A01,
	0x41EC87,0x41EC85,0x41EC89,0x414A0D,0x414A0B,0x419B1C,0x41EC70,0x41EC76,0x414A09,0x414A07,
	0x414A02,0x414A03,0x419B58,0x41EC7D,0x419B55,
	};

	// clang-format on

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		CHandler::InjectAndCompileData();
		CHandler::InstallSearchTerms();
		FrameworkAPI::AddUpdateFoundForms(CHandler::UpdateFoundForms);
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	void CHandler::OnBooksReadEvent(RE::BooksRead::Event const* a_event) {
		using log = Serialization::CompletionistLog::logType;

		if (BookDataA.HasForm(a_event->book->GetFormID())) {
			auto base = BookDataA.GetBase(a_event->book->GetFormID()) ? BookDataA.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
			CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), patchID::kpatch_Apocalypse);
			return;
		}

		if (BookDataF.HasForm(a_event->book->GetFormID())) {
			auto base = BookDataF.GetBase(a_event->book->GetFormID()) ? BookDataF.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
			CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), patchID::kpatch_ForgottenMagic);
			return;
		}

		if (BookDataM.HasForm(a_event->book->GetFormID())) {
			auto base = BookDataM.GetBase(a_event->book->GetFormID()) ? BookDataM.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
			CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), patchID::kpatch_Mysticism);
			return;
		}

		if (BookDataO.HasForm(a_event->book->GetFormID())) {
			auto base = BookDataO.GetBase(a_event->book->GetFormID()) ? BookDataO.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
			CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), patchID::kpatch_Odin);
			return;
		}

		if (BookDataT.HasForm(a_event->book->GetFormID())) {
			auto base = BookDataT.GetBase(a_event->book->GetFormID()) ? BookDataT.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
			CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), patchID::kpatch_Triumvirate);
			return;
		}

		return;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	void CHandler::OnMenuOpenCloseEvent(RE::MenuOpenCloseEvent const* a_event) {
		using log = Serialization::CompletionistLog::logType;

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening) {
	
			if (BookDataA.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
				if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
					auto base = BookDataA.GetBase(target) ? BookDataA.GetBase(target) : target;
					CHandler::ProcessFoundForm(base, target, patchID::kpatch_Apocalypse);
					return;
				}
			}

			if (BookDataF.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
				if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
					auto base = BookDataF.GetBase(target) ? BookDataF.GetBase(target) : target;
					CHandler::ProcessFoundForm(base, target, patchID::kpatch_ForgottenMagic);
					return;
				}
			}

			if (BookDataM.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
				if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
					auto base = BookDataM.GetBase(target) ? BookDataM.GetBase(target) : target;
					CHandler::ProcessFoundForm(base, target, patchID::kpatch_Mysticism);
					return;
				}
			}

			if (BookDataO.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
				if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
					auto base = BookDataO.GetBase(target) ? BookDataO.GetBase(target) : target;
					CHandler::ProcessFoundForm(base, target, patchID::kpatch_Odin);
					return;
				}
			}

			if (BookDataT.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
				if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
					auto base = BookDataT.GetBase(target) ? BookDataT.GetBase(target) : target;
					CHandler::ProcessFoundForm(base, target, patchID::kpatch_Triumvirate);
					return;
				}
			}
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, patchID a_patch) {

		switch (a_patch)
		{
		case CPatch_SpellTomes::patchID::kpatch_Apocalypse: {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, BookDataA.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyBooks");
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kTome, BookDataA.GetForm(a_eventID)->GetName());
			}

			FoundItemData.AddForm(a_baseID);

			auto t_pos = std::ranges::find(Apocalypse_A_FormArray, BookDataA.GetForm(a_baseID));
			if (t_pos != Apocalypse_A_FormArray.end()) {
				auto b_pos = std::distance(Apocalypse_A_FormArray.begin(), t_pos);
				Apocalypse_A_BoolArray[b_pos] = true;
				Apocalypse_A_EntriesFound = std::ranges::count(Apocalypse_A_BoolArray, true);
				break;
			}

			t_pos = std::ranges::find(Apocalypse_C_FormArray, BookDataA.GetForm(a_baseID));
			if (t_pos != Apocalypse_C_FormArray.end()) {
				auto b_pos = std::distance(Apocalypse_C_FormArray.begin(), t_pos);
				Apocalypse_C_BoolArray[b_pos] = true;
				Apocalypse_C_EntriesFound = std::ranges::count(Apocalypse_C_BoolArray, true);
				break;
			}

			t_pos = std::ranges::find(Apocalypse_D_FormArray, BookDataA.GetForm(a_baseID));
			if (t_pos != Apocalypse_D_FormArray.end()) {
				auto b_pos = std::distance(Apocalypse_D_FormArray.begin(), t_pos);
				Apocalypse_D_BoolArray[b_pos] = true;
				Apocalypse_D_EntriesFound = std::ranges::count(Apocalypse_D_BoolArray, true);
				break;
			}

			t_pos = std::ranges::find(Apocalypse_I_FormArray, BookDataA.GetForm(a_baseID));
			if (t_pos != Apocalypse_I_FormArray.end()) {
				auto b_pos = std::distance(Apocalypse_I_FormArray.begin(), t_pos);
				Apocalypse_I_BoolArray[b_pos] = true;
				Apocalypse_I_EntriesFound = std::ranges::count(Apocalypse_I_BoolArray, true);
				break;
			}

			t_pos = std::ranges::find(Apocalypse_R_FormArray, BookDataA.GetForm(a_baseID));
			if (t_pos != Apocalypse_R_FormArray.end()) {
				auto b_pos = std::distance(Apocalypse_R_FormArray.begin(), t_pos);
				Apocalypse_R_BoolArray[b_pos] = true;
				Apocalypse_R_EntriesFound = std::ranges::count(Apocalypse_R_BoolArray, true);
				break;
			}

			break;
		}

		case CPatch_SpellTomes::patchID::kpatch_ForgottenMagic: {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, BookDataF.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyBooks");
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kTome, BookDataF.GetForm(a_eventID)->GetName());
			}

			FoundItemData.AddForm(a_baseID);

			auto t_pos = std::ranges::find(ForgottenMagic_A_FormArray, BookDataF.GetForm(a_baseID));
			if (t_pos != ForgottenMagic_A_FormArray.end()) {
				auto b_pos = std::distance(ForgottenMagic_A_FormArray.begin(), t_pos);
				ForgottenMagic_A_BoolArray[b_pos] = true;
				ForgottenMagic_A_EntriesFound = std::ranges::count(ForgottenMagic_A_BoolArray, true);
				break;
			}

			t_pos = std::ranges::find(ForgottenMagic_C_FormArray, BookDataF.GetForm(a_baseID));
			if (t_pos != ForgottenMagic_C_FormArray.end()) {
				auto b_pos = std::distance(ForgottenMagic_C_FormArray.begin(), t_pos);
				ForgottenMagic_C_BoolArray[b_pos] = true;
				ForgottenMagic_C_EntriesFound = std::ranges::count(ForgottenMagic_C_BoolArray, true);
				break;
			}

			t_pos = std::ranges::find(ForgottenMagic_D_FormArray, BookDataF.GetForm(a_baseID));
			if (t_pos != ForgottenMagic_D_FormArray.end()) {
				auto b_pos = std::distance(ForgottenMagic_D_FormArray.begin(), t_pos);
				ForgottenMagic_D_BoolArray[b_pos] = true;
				ForgottenMagic_D_EntriesFound = std::ranges::count(ForgottenMagic_D_BoolArray, true);
				break;
			}

			t_pos = std::ranges::find(ForgottenMagic_I_FormArray, BookDataF.GetForm(a_baseID));
			if (t_pos != ForgottenMagic_I_FormArray.end()) {
				auto b_pos = std::distance(ForgottenMagic_I_FormArray.begin(), t_pos);
				ForgottenMagic_I_BoolArray[b_pos] = true;
				ForgottenMagic_I_EntriesFound = std::ranges::count(ForgottenMagic_I_BoolArray, true);
				break;
			}

			t_pos = std::ranges::find(ForgottenMagic_R_FormArray, BookDataF.GetForm(a_baseID));
			if (t_pos != ForgottenMagic_R_FormArray.end()) {
				auto b_pos = std::distance(ForgottenMagic_R_FormArray.begin(), t_pos);
				ForgottenMagic_R_BoolArray[b_pos] = true;
				ForgottenMagic_R_EntriesFound = std::ranges::count(ForgottenMagic_R_BoolArray, true);
				break;
			}
			break;
		}

		case CPatch_SpellTomes::patchID::kpatch_Mysticism: {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, BookDataM.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyBooks");
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kTome, BookDataM.GetForm(a_eventID)->GetName());
			}

			FoundItemData.AddForm(a_baseID);

			auto t_pos = std::ranges::find(Mysticism_A_FormArray, BookDataM.GetForm(a_baseID));
			if (t_pos != Mysticism_A_FormArray.end()) {
				auto b_pos = std::distance(Mysticism_A_FormArray.begin(), t_pos);
				Mysticism_A_BoolArray[b_pos] = true;
				Mysticism_A_EntriesFound = std::ranges::count(Mysticism_A_BoolArray, true);
				break;
			}

			t_pos = std::ranges::find(Mysticism_C_FormArray, BookDataM.GetForm(a_baseID));
			if (t_pos != Mysticism_C_FormArray.end()) {
				auto b_pos = std::distance(Mysticism_C_FormArray.begin(), t_pos);
				Mysticism_C_BoolArray[b_pos] = true;
				Mysticism_C_EntriesFound = std::ranges::count(Mysticism_C_BoolArray, true);
				break;
			}

			t_pos = std::ranges::find(Mysticism_D_FormArray, BookDataM.GetForm(a_baseID));
			if (t_pos != Mysticism_D_FormArray.end()) {
				auto b_pos = std::distance(Mysticism_D_FormArray.begin(), t_pos);
				Mysticism_D_BoolArray[b_pos] = true;
				Mysticism_D_EntriesFound = std::ranges::count(Mysticism_D_BoolArray, true);
				break;
			}

			t_pos = std::ranges::find(Mysticism_I_FormArray, BookDataM.GetForm(a_baseID));
			if (t_pos != Mysticism_I_FormArray.end()) {
				auto b_pos = std::distance(Mysticism_I_FormArray.begin(), t_pos);
				Mysticism_I_BoolArray[b_pos] = true;
				Mysticism_I_EntriesFound = std::ranges::count(Mysticism_I_BoolArray, true);
				break;
			}

			t_pos = std::ranges::find(Mysticism_R_FormArray, BookDataM.GetForm(a_baseID));
			if (t_pos != Mysticism_R_FormArray.end()) {
				auto b_pos = std::distance(Mysticism_R_FormArray.begin(), t_pos);
				Mysticism_R_BoolArray[b_pos] = true;
				Mysticism_R_EntriesFound = std::ranges::count(Mysticism_R_BoolArray, true);
				break;
			}
			break;
		}

		case CPatch_SpellTomes::patchID::kpatch_Odin: {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, BookDataO.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyBooks");
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kTome, BookDataO.GetForm(a_eventID)->GetName());
			}

			FoundItemData.AddForm(a_baseID);

			auto t_pos = std::ranges::find(Odin_A_FormArray, BookDataO.GetForm(a_baseID));
			if (t_pos != Odin_A_FormArray.end()) {
				auto b_pos = std::distance(Odin_A_FormArray.begin(), t_pos);
				Odin_A_BoolArray[b_pos] = true;
				Odin_A_EntriesFound = std::ranges::count(Odin_A_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Odin_C_FormArray, BookDataO.GetForm(a_baseID));
			if (t_pos != Odin_C_FormArray.end()) {
				auto b_pos = std::distance(Odin_C_FormArray.begin(), t_pos);
				Odin_C_BoolArray[b_pos] = true;
				Odin_C_EntriesFound = std::ranges::count(Odin_C_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Odin_D_FormArray, BookDataO.GetForm(a_baseID));
			if (t_pos != Odin_D_FormArray.end()) {
				auto b_pos = std::distance(Odin_D_FormArray.begin(), t_pos);
				Odin_D_BoolArray[b_pos] = true;
				Odin_D_EntriesFound = std::ranges::count(Odin_D_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Odin_I_FormArray, BookDataO.GetForm(a_baseID));
			if (t_pos != Odin_I_FormArray.end()) {
				auto b_pos = std::distance(Odin_I_FormArray.begin(), t_pos);
				Odin_I_BoolArray[b_pos] = true;
				Odin_I_EntriesFound = std::ranges::count(Odin_I_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Odin_R_FormArray, BookDataO.GetForm(a_baseID));
			if (t_pos != Odin_R_FormArray.end()) {
				auto b_pos = std::distance(Odin_R_FormArray.begin(), t_pos);
				Odin_R_BoolArray[b_pos] = true;
				Odin_R_EntriesFound = std::ranges::count(Odin_R_BoolArray, true);
				return;
			}
			break;
		}

		case CPatch_SpellTomes::patchID::kpatch_Triumvirate: {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, BookDataT.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyBooks");
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kTome, BookDataT.GetForm(a_eventID)->GetName());
			}

			FoundItemData.AddForm(a_baseID);

			auto t_pos = std::ranges::find(Triumvirate_A_FormArray, BookDataT.GetForm(a_baseID));
			if (t_pos != Triumvirate_A_FormArray.end()) {
				auto b_pos = std::distance(Triumvirate_A_FormArray.begin(), t_pos);
				Triumvirate_A_BoolArray[b_pos] = true;
				Triumvirate_A_EntriesFound = std::ranges::count(Triumvirate_A_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Triumvirate_C_FormArray, BookDataT.GetForm(a_baseID));
			if (t_pos != Triumvirate_C_FormArray.end()) {
				auto b_pos = std::distance(Triumvirate_C_FormArray.begin(), t_pos);
				Triumvirate_C_BoolArray[b_pos] = true;
				Triumvirate_C_EntriesFound = std::ranges::count(Triumvirate_C_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Triumvirate_D_FormArray, BookDataT.GetForm(a_baseID));
			if (t_pos != Triumvirate_D_FormArray.end()) {
				auto b_pos = std::distance(Triumvirate_D_FormArray.begin(), t_pos);
				Triumvirate_D_BoolArray[b_pos] = true;
				Triumvirate_D_EntriesFound = std::ranges::count(Triumvirate_D_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Triumvirate_I_FormArray, BookDataT.GetForm(a_baseID));
			if (t_pos != Triumvirate_I_FormArray.end()) {
				auto b_pos = std::distance(Triumvirate_I_FormArray.begin(), t_pos);
				Triumvirate_I_BoolArray[b_pos] = true;
				Triumvirate_I_EntriesFound = std::ranges::count(Triumvirate_I_BoolArray, true);
				return;
			}

			t_pos = std::ranges::find(Triumvirate_R_FormArray, BookDataT.GetForm(a_baseID));
			if (t_pos != Triumvirate_R_FormArray.end()) {
				auto b_pos = std::distance(Triumvirate_R_FormArray.begin(), t_pos);
				Triumvirate_R_BoolArray[b_pos] = true;
				Triumvirate_R_EntriesFound = std::ranges::count(Triumvirate_R_BoolArray, true);
				return;
			}
			break;
		}

		default:
			break;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		if (Serialization::CompletionistData::IsModInstalled(filename_Apocalypse)) {

			BookDataA.CompileFormArray(CPatch_SpellTomes::Apocalypse_Tomes, filename_Apocalypse);
			BookDataA.MergeAsCollectable();
			BookDataA.PopulateSpellTomes(Apocalypse_A_NameArray, Apocalypse_A_FormArray, Apocalypse_A_BoolArray, Apocalypse_A_TextArray, RE::ActorValue::kAlteration);
			BookDataA.PopulateSpellTomes(Apocalypse_C_NameArray, Apocalypse_C_FormArray, Apocalypse_C_BoolArray, Apocalypse_C_TextArray, RE::ActorValue::kConjuration);
			BookDataA.PopulateSpellTomes(Apocalypse_D_NameArray, Apocalypse_D_FormArray, Apocalypse_D_BoolArray, Apocalypse_D_TextArray, RE::ActorValue::kDestruction);
			BookDataA.PopulateSpellTomes(Apocalypse_I_NameArray, Apocalypse_I_FormArray, Apocalypse_I_BoolArray, Apocalypse_I_TextArray, RE::ActorValue::kIllusion);
			BookDataA.PopulateSpellTomes(Apocalypse_R_NameArray, Apocalypse_R_FormArray, Apocalypse_R_BoolArray, Apocalypse_R_TextArray, RE::ActorValue::kRestoration);

			Apocalypse_A_EntriesTotal = Apocalypse_A_FormArray.size();
			Apocalypse_C_EntriesTotal = Apocalypse_C_FormArray.size();
			Apocalypse_D_EntriesTotal = Apocalypse_D_FormArray.size();
			Apocalypse_I_EntriesTotal = Apocalypse_I_FormArray.size();
			Apocalypse_R_EntriesTotal = Apocalypse_R_FormArray.size();

			Apocalypse_A_EntriesFound = std::ranges::count(Apocalypse_A_BoolArray, true);
			Apocalypse_C_EntriesFound = std::ranges::count(Apocalypse_C_BoolArray, true);
			Apocalypse_D_EntriesFound = std::ranges::count(Apocalypse_D_BoolArray, true);
			Apocalypse_I_EntriesFound = std::ranges::count(Apocalypse_I_BoolArray, true);
			Apocalypse_R_EntriesFound = std::ranges::count(Apocalypse_R_BoolArray, true);

			CFramework_Master::TomesInstalled += 1;
		}

		if (Serialization::CompletionistData::IsModInstalled(filename_ForgottenMagic)) {

			BookDataF.CompileFormArray(CPatch_SpellTomes::ForgottenMagic_Tomes, filename_ForgottenMagic);
			BookDataF.MergeAsCollectable();
			BookDataF.PopulateSpellTomes(ForgottenMagic_A_NameArray, ForgottenMagic_A_FormArray, ForgottenMagic_A_BoolArray, ForgottenMagic_A_TextArray, RE::ActorValue::kAlteration);
			BookDataF.PopulateSpellTomes(ForgottenMagic_C_NameArray, ForgottenMagic_C_FormArray, ForgottenMagic_C_BoolArray, ForgottenMagic_C_TextArray, RE::ActorValue::kConjuration);
			BookDataF.PopulateSpellTomes(ForgottenMagic_D_NameArray, ForgottenMagic_D_FormArray, ForgottenMagic_D_BoolArray, ForgottenMagic_D_TextArray, RE::ActorValue::kDestruction);
			BookDataF.PopulateSpellTomes(ForgottenMagic_I_NameArray, ForgottenMagic_I_FormArray, ForgottenMagic_I_BoolArray, ForgottenMagic_I_TextArray, RE::ActorValue::kIllusion);
			BookDataF.PopulateSpellTomes(ForgottenMagic_R_NameArray, ForgottenMagic_R_FormArray, ForgottenMagic_R_BoolArray, ForgottenMagic_R_TextArray, RE::ActorValue::kRestoration);

			ForgottenMagic_A_EntriesTotal = ForgottenMagic_A_FormArray.size();
			ForgottenMagic_C_EntriesTotal = ForgottenMagic_C_FormArray.size();
			ForgottenMagic_D_EntriesTotal = ForgottenMagic_D_FormArray.size();
			ForgottenMagic_I_EntriesTotal = ForgottenMagic_I_FormArray.size();
			ForgottenMagic_R_EntriesTotal = ForgottenMagic_R_FormArray.size();

			ForgottenMagic_A_EntriesFound = std::ranges::count(ForgottenMagic_A_BoolArray, true);
			ForgottenMagic_C_EntriesFound = std::ranges::count(ForgottenMagic_C_BoolArray, true);
			ForgottenMagic_D_EntriesFound = std::ranges::count(ForgottenMagic_D_BoolArray, true);
			ForgottenMagic_I_EntriesFound = std::ranges::count(ForgottenMagic_I_BoolArray, true);
			ForgottenMagic_R_EntriesFound = std::ranges::count(ForgottenMagic_R_BoolArray, true);

			CFramework_Master::TomesInstalled += 1;
		}

		if (Serialization::CompletionistData::IsModInstalled(filename_Mysticism)) {

			BookDataM.CompileFormArray(CPatch_SpellTomes::Mysticism_Tomes, filename_Mysticism);
			BookDataM.MergeAsCollectable();
			BookDataM.PopulateSpellTomes(Mysticism_A_NameArray, Mysticism_A_FormArray, Mysticism_A_BoolArray, Mysticism_A_TextArray, RE::ActorValue::kAlteration);
			BookDataM.PopulateSpellTomes(Mysticism_C_NameArray, Mysticism_C_FormArray, Mysticism_C_BoolArray, Mysticism_C_TextArray, RE::ActorValue::kConjuration);
			BookDataM.PopulateSpellTomes(Mysticism_D_NameArray, Mysticism_D_FormArray, Mysticism_D_BoolArray, Mysticism_D_TextArray, RE::ActorValue::kDestruction);
			BookDataM.PopulateSpellTomes(Mysticism_I_NameArray, Mysticism_I_FormArray, Mysticism_I_BoolArray, Mysticism_I_TextArray, RE::ActorValue::kIllusion);
			BookDataM.PopulateSpellTomes(Mysticism_R_NameArray, Mysticism_R_FormArray, Mysticism_R_BoolArray, Mysticism_R_TextArray, RE::ActorValue::kRestoration);

			Mysticism_A_EntriesTotal = Mysticism_A_FormArray.size();
			Mysticism_C_EntriesTotal = Mysticism_C_FormArray.size();
			Mysticism_D_EntriesTotal = Mysticism_D_FormArray.size();
			Mysticism_I_EntriesTotal = Mysticism_I_FormArray.size();
			Mysticism_R_EntriesTotal = Mysticism_R_FormArray.size();

			Mysticism_A_EntriesFound = std::ranges::count(Mysticism_A_BoolArray, true);
			Mysticism_C_EntriesFound = std::ranges::count(Mysticism_C_BoolArray, true);
			Mysticism_D_EntriesFound = std::ranges::count(Mysticism_D_BoolArray, true);
			Mysticism_I_EntriesFound = std::ranges::count(Mysticism_I_BoolArray, true);
			Mysticism_R_EntriesFound = std::ranges::count(Mysticism_R_BoolArray, true);

			CFramework_Master::TomesInstalled += 1;
		}

		if (Serialization::CompletionistData::IsModInstalled(filename_Odin)) {

			if (!Serialization::CompletionistData::IsModInstalled(filename_Apocalypse)) {
				for (auto& formid : Odin_Crossover_Tomes) {
					BookDataO.AddForm(formid, filename_Odin);
				}
			}

			BookDataO.CompileFormArray(CPatch_SpellTomes::Odin_Tomes, filename_Odin);
			BookDataO.MergeAsCollectable();
			BookDataO.PopulateSpellTomes(Odin_A_NameArray, Odin_A_FormArray, Odin_A_BoolArray, Odin_A_TextArray, RE::ActorValue::kAlteration);
			BookDataO.PopulateSpellTomes(Odin_C_NameArray, Odin_C_FormArray, Odin_C_BoolArray, Odin_C_TextArray, RE::ActorValue::kConjuration);
			BookDataO.PopulateSpellTomes(Odin_D_NameArray, Odin_D_FormArray, Odin_D_BoolArray, Odin_D_TextArray, RE::ActorValue::kDestruction);
			BookDataO.PopulateSpellTomes(Odin_I_NameArray, Odin_I_FormArray, Odin_I_BoolArray, Odin_I_TextArray, RE::ActorValue::kIllusion);
			BookDataO.PopulateSpellTomes(Odin_R_NameArray, Odin_R_FormArray, Odin_R_BoolArray, Odin_R_TextArray, RE::ActorValue::kRestoration);

			Odin_A_EntriesTotal = Odin_A_FormArray.size();
			Odin_C_EntriesTotal = Odin_C_FormArray.size();
			Odin_D_EntriesTotal = Odin_D_FormArray.size();
			Odin_I_EntriesTotal = Odin_I_FormArray.size();
			Odin_R_EntriesTotal = Odin_R_FormArray.size();

			Odin_A_EntriesFound = std::ranges::count(Odin_A_BoolArray, true);
			Odin_C_EntriesFound = std::ranges::count(Odin_C_BoolArray, true);
			Odin_D_EntriesFound = std::ranges::count(Odin_D_BoolArray, true);
			Odin_I_EntriesFound = std::ranges::count(Odin_I_BoolArray, true);
			Odin_R_EntriesFound = std::ranges::count(Odin_R_BoolArray, true);

			CFramework_Master::TomesInstalled += 1;
		}

		if (Serialization::CompletionistData::IsModInstalled(filename_Triumvirate)) {

			BookDataT.CompileFormArray(CPatch_SpellTomes::Triumvirate_Tomes, filename_Triumvirate);
			BookDataT.MergeAsCollectable();
			BookDataT.PopulateSpellTomes(Triumvirate_A_NameArray, Triumvirate_A_FormArray, Triumvirate_A_BoolArray, Triumvirate_A_TextArray, RE::ActorValue::kAlteration);
			BookDataT.PopulateSpellTomes(Triumvirate_C_NameArray, Triumvirate_C_FormArray, Triumvirate_C_BoolArray, Triumvirate_C_TextArray, RE::ActorValue::kConjuration);
			BookDataT.PopulateSpellTomes(Triumvirate_D_NameArray, Triumvirate_D_FormArray, Triumvirate_D_BoolArray, Triumvirate_D_TextArray, RE::ActorValue::kDestruction);
			BookDataT.PopulateSpellTomes(Triumvirate_I_NameArray, Triumvirate_I_FormArray, Triumvirate_I_BoolArray, Triumvirate_I_TextArray, RE::ActorValue::kIllusion);
			BookDataT.PopulateSpellTomes(Triumvirate_R_NameArray, Triumvirate_R_FormArray, Triumvirate_R_BoolArray, Triumvirate_R_TextArray, RE::ActorValue::kRestoration);

			Triumvirate_A_EntriesTotal = Triumvirate_A_FormArray.size();
			Triumvirate_C_EntriesTotal = Triumvirate_C_FormArray.size();
			Triumvirate_D_EntriesTotal = Triumvirate_D_FormArray.size();
			Triumvirate_I_EntriesTotal = Triumvirate_I_FormArray.size();
			Triumvirate_R_EntriesTotal = Triumvirate_R_FormArray.size();

			Triumvirate_A_EntriesFound = std::ranges::count(Triumvirate_A_BoolArray, true);
			Triumvirate_C_EntriesFound = std::ranges::count(Triumvirate_C_BoolArray, true);
			Triumvirate_D_EntriesFound = std::ranges::count(Triumvirate_D_BoolArray, true);
			Triumvirate_I_EntriesFound = std::ranges::count(Triumvirate_I_BoolArray, true);
			Triumvirate_R_EntriesFound = std::ranges::count(Triumvirate_R_BoolArray, true);

			CFramework_Master::TomesInstalled += 1;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Search Terms ) ---
	//---------------------------------------------------

	void CHandler::InstallSearchTerms()
	{
		for (auto i = 0; i < Odin_A_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Odin_A_FormArray[i], Odin_A_NameArray[i], "$MCMPageTomes_Odin1", FrameworkAPI::GetBookCategoryType(Odin_A_FormArray[i])));
		}
		for (auto i = 0; i < Odin_C_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Odin_C_FormArray[i], Odin_C_NameArray[i], "$MCMPageTomes_Odin1", FrameworkAPI::GetBookCategoryType(Odin_C_FormArray[i])));
		}
		for (auto i = 0; i < Odin_D_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Odin_D_FormArray[i], Odin_D_NameArray[i], "$MCMPageTomes_Odin2", FrameworkAPI::GetBookCategoryType(Odin_D_FormArray[i])));
		}
		for (auto i = 0; i < Odin_I_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Odin_I_FormArray[i], Odin_I_NameArray[i], "$MCMPageTomes_Odin2", FrameworkAPI::GetBookCategoryType(Odin_I_FormArray[i])));
		}
		for (auto i = 0; i < Odin_R_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Odin_R_FormArray[i], Odin_R_NameArray[i], "$MCMPageTomes_Odin2", FrameworkAPI::GetBookCategoryType(Odin_R_FormArray[i])));
		}
		for (auto i = 0; i < ForgottenMagic_A_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ForgottenMagic_A_FormArray[i], ForgottenMagic_A_NameArray[i], "$MCMPageTomes_ForgottenMagic1", FrameworkAPI::GetBookCategoryType(ForgottenMagic_A_FormArray[i])));
		}
		for (auto i = 0; i < ForgottenMagic_C_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ForgottenMagic_C_FormArray[i], ForgottenMagic_C_NameArray[i], "$MCMPageTomes_ForgottenMagic1", FrameworkAPI::GetBookCategoryType(ForgottenMagic_C_FormArray[i])));
		}
		for (auto i = 0; i < ForgottenMagic_D_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ForgottenMagic_D_FormArray[i], ForgottenMagic_D_NameArray[i], "$MCMPageTomes_ForgottenMagic2", FrameworkAPI::GetBookCategoryType(ForgottenMagic_D_FormArray[i])));
		}
		for (auto i = 0; i < ForgottenMagic_I_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ForgottenMagic_I_FormArray[i], ForgottenMagic_I_NameArray[i], "$MCMPageTomes_ForgottenMagic2", FrameworkAPI::GetBookCategoryType(ForgottenMagic_I_FormArray[i])));
		}
		for (auto i = 0; i < ForgottenMagic_R_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ForgottenMagic_R_FormArray[i], ForgottenMagic_R_NameArray[i], "$MCMPageTomes_ForgottenMagic2", FrameworkAPI::GetBookCategoryType(ForgottenMagic_R_FormArray[i])));
		}
		for (auto i = 0; i < Apocalypse_A_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Apocalypse_A_FormArray[i], Apocalypse_A_NameArray[i], "$MCMPageTomes_Apocalypse1", FrameworkAPI::GetBookCategoryType(Apocalypse_A_FormArray[i])));
		}
		for (auto i = 0; i < Apocalypse_C_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Apocalypse_C_FormArray[i], Apocalypse_C_NameArray[i], "$MCMPageTomes_Apocalypse1", FrameworkAPI::GetBookCategoryType(Apocalypse_C_FormArray[i])));
		}
		for (auto i = 0; i < Apocalypse_D_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Apocalypse_D_FormArray[i], Apocalypse_D_NameArray[i], "$MCMPageTomes_Apocalypse2", FrameworkAPI::GetBookCategoryType(Apocalypse_D_FormArray[i])));
		}
		for (auto i = 0; i < Apocalypse_I_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Apocalypse_I_FormArray[i], Apocalypse_I_NameArray[i], "$MCMPageTomes_Apocalypse2", FrameworkAPI::GetBookCategoryType(Apocalypse_I_FormArray[i])));
		}
		for (auto i = 0; i < Apocalypse_R_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Apocalypse_R_FormArray[i], Apocalypse_R_NameArray[i], "$MCMPageTomes_Apocalypse2", FrameworkAPI::GetBookCategoryType(Apocalypse_R_FormArray[i])));
		}
		for (auto i = 0; i < Mysticism_A_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Mysticism_A_FormArray[i], Mysticism_A_NameArray[i], "$MCMPageTomes_Mysticism1", FrameworkAPI::GetBookCategoryType(Mysticism_A_FormArray[i])));
		}
		for (auto i = 0; i < Mysticism_C_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Mysticism_C_FormArray[i], Mysticism_C_NameArray[i], "$MCMPageTomes_Mysticism1", FrameworkAPI::GetBookCategoryType(Mysticism_C_FormArray[i])));
		}
		for (auto i = 0; i < Mysticism_D_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Mysticism_D_FormArray[i], Mysticism_D_NameArray[i], "$MCMPageTomes_Mysticism2", FrameworkAPI::GetBookCategoryType(Mysticism_D_FormArray[i])));
		}
		for (auto i = 0; i < Mysticism_I_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Mysticism_I_FormArray[i], Mysticism_I_NameArray[i], "$MCMPageTomes_Mysticism2", FrameworkAPI::GetBookCategoryType(Mysticism_I_FormArray[i])));
		}
		for (auto i = 0; i < Mysticism_R_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Mysticism_R_FormArray[i], Mysticism_R_NameArray[i], "$MCMPageTomes_Mysticism2", FrameworkAPI::GetBookCategoryType(Mysticism_R_FormArray[i])));
		}
		for (auto i = 0; i < Triumvirate_A_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Triumvirate_A_FormArray[i], Triumvirate_A_NameArray[i], "$MCMPageTomes_Triumvirate1", FrameworkAPI::GetBookCategoryType(Triumvirate_A_FormArray[i])));
		}
		for (auto i = 0; i < Triumvirate_C_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Triumvirate_C_FormArray[i], Triumvirate_C_NameArray[i], "$MCMPageTomes_Triumvirate1", FrameworkAPI::GetBookCategoryType(Triumvirate_C_FormArray[i])));
		}
		for (auto i = 0; i < Triumvirate_D_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Triumvirate_D_FormArray[i], Triumvirate_D_NameArray[i], "$MCMPageTomes_Triumvirate2", FrameworkAPI::GetBookCategoryType(Triumvirate_D_FormArray[i])));
		}
		for (auto i = 0; i < Triumvirate_I_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Triumvirate_I_FormArray[i], Triumvirate_I_NameArray[i], "$MCMPageTomes_Triumvirate2", FrameworkAPI::GetBookCategoryType(Triumvirate_I_FormArray[i])));
		}
		for (auto i = 0; i < Triumvirate_R_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Triumvirate_R_FormArray[i], Triumvirate_R_NameArray[i], "$MCMPageTomes_Triumvirate2", FrameworkAPI::GetBookCategoryType(Triumvirate_R_FormArray[i])));
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (Serialization::CompletionistData::IsModInstalled(filename_Apocalypse)) {
			for (auto i = 0; i < Apocalypse_A_FormArray.size(); i++) {
				Apocalypse_A_BoolArray[i] = FrameworkAPI::IsBookKnown(Apocalypse_A_FormArray[i]);
			}

			for (auto i = 0; i < Apocalypse_C_FormArray.size(); i++) {
				Apocalypse_C_BoolArray[i] = FrameworkAPI::IsBookKnown(Apocalypse_C_FormArray[i]);
			}

			for (auto i = 0; i < Apocalypse_D_FormArray.size(); i++) {
				Apocalypse_D_BoolArray[i] = FrameworkAPI::IsBookKnown(Apocalypse_D_FormArray[i]);
			}

			for (auto i = 0; i < Apocalypse_I_FormArray.size(); i++) {
				Apocalypse_I_BoolArray[i] = FrameworkAPI::IsBookKnown(Apocalypse_I_FormArray[i]);
			}

			for (auto i = 0; i < Apocalypse_R_FormArray.size(); i++) {
				Apocalypse_R_BoolArray[i] = FrameworkAPI::IsBookKnown(Apocalypse_R_FormArray[i]);
			}

			Apocalypse_A_EntriesTotal = Apocalypse_A_FormArray.size();
			Apocalypse_C_EntriesTotal = Apocalypse_C_FormArray.size();
			Apocalypse_D_EntriesTotal = Apocalypse_D_FormArray.size();
			Apocalypse_I_EntriesTotal = Apocalypse_I_FormArray.size();
			Apocalypse_R_EntriesTotal = Apocalypse_R_FormArray.size();

			Apocalypse_A_EntriesFound = std::ranges::count(Apocalypse_A_BoolArray, true);
			Apocalypse_C_EntriesFound = std::ranges::count(Apocalypse_C_BoolArray, true);
			Apocalypse_D_EntriesFound = std::ranges::count(Apocalypse_D_BoolArray, true);
			Apocalypse_I_EntriesFound = std::ranges::count(Apocalypse_I_BoolArray, true);
			Apocalypse_R_EntriesFound = std::ranges::count(Apocalypse_R_BoolArray, true);
		}

		if (Serialization::CompletionistData::IsModInstalled(filename_ForgottenMagic)) {
			for (auto i = 0; i < ForgottenMagic_A_FormArray.size(); i++) {
				ForgottenMagic_A_BoolArray[i] = FrameworkAPI::IsBookKnown(ForgottenMagic_A_FormArray[i]);
			}

			for (auto i = 0; i < ForgottenMagic_C_FormArray.size(); i++) {
				ForgottenMagic_C_BoolArray[i] = FrameworkAPI::IsBookKnown(ForgottenMagic_C_FormArray[i]);
			}

			for (auto i = 0; i < ForgottenMagic_D_FormArray.size(); i++) {
				ForgottenMagic_D_BoolArray[i] = FrameworkAPI::IsBookKnown(ForgottenMagic_D_FormArray[i]);
			}

			for (auto i = 0; i < ForgottenMagic_I_FormArray.size(); i++) {
				ForgottenMagic_I_BoolArray[i] = FrameworkAPI::IsBookKnown(ForgottenMagic_I_FormArray[i]);
			}

			for (auto i = 0; i < ForgottenMagic_R_FormArray.size(); i++) {
				ForgottenMagic_R_BoolArray[i] = FrameworkAPI::IsBookKnown(ForgottenMagic_R_FormArray[i]);
			}

			ForgottenMagic_A_EntriesTotal = ForgottenMagic_A_FormArray.size();
			ForgottenMagic_C_EntriesTotal = ForgottenMagic_C_FormArray.size();
			ForgottenMagic_D_EntriesTotal = ForgottenMagic_D_FormArray.size();
			ForgottenMagic_I_EntriesTotal = ForgottenMagic_I_FormArray.size();
			ForgottenMagic_R_EntriesTotal = ForgottenMagic_R_FormArray.size();

			ForgottenMagic_A_EntriesFound = std::ranges::count(ForgottenMagic_A_BoolArray, true);
			ForgottenMagic_C_EntriesFound = std::ranges::count(ForgottenMagic_C_BoolArray, true);
			ForgottenMagic_D_EntriesFound = std::ranges::count(ForgottenMagic_D_BoolArray, true);
			ForgottenMagic_I_EntriesFound = std::ranges::count(ForgottenMagic_I_BoolArray, true);
			ForgottenMagic_R_EntriesFound = std::ranges::count(ForgottenMagic_R_BoolArray, true);
		}

		if (Serialization::CompletionistData::IsModInstalled(filename_Mysticism)) {
			for (auto i = 0; i < Mysticism_A_FormArray.size(); i++) {
				Mysticism_A_BoolArray[i] = FrameworkAPI::IsBookKnown(Mysticism_A_FormArray[i]);
			}

			for (auto i = 0; i < Mysticism_C_FormArray.size(); i++) {
				Mysticism_C_BoolArray[i] = FrameworkAPI::IsBookKnown(Mysticism_C_FormArray[i]);
			}

			for (auto i = 0; i < Mysticism_D_FormArray.size(); i++) {
				Mysticism_D_BoolArray[i] = FrameworkAPI::IsBookKnown(Mysticism_D_FormArray[i]);
			}

			for (auto i = 0; i < Mysticism_I_FormArray.size(); i++) {
				Mysticism_I_BoolArray[i] = FrameworkAPI::IsBookKnown(Mysticism_I_FormArray[i]);
			}

			for (auto i = 0; i < Mysticism_R_FormArray.size(); i++) {
				Mysticism_R_BoolArray[i] = FrameworkAPI::IsBookKnown(Mysticism_R_FormArray[i]);
			}

			Mysticism_A_EntriesTotal = Mysticism_A_FormArray.size();
			Mysticism_C_EntriesTotal = Mysticism_C_FormArray.size();
			Mysticism_D_EntriesTotal = Mysticism_D_FormArray.size();
			Mysticism_I_EntriesTotal = Mysticism_I_FormArray.size();
			Mysticism_R_EntriesTotal = Mysticism_R_FormArray.size();

			Mysticism_A_EntriesFound = std::ranges::count(Mysticism_A_BoolArray, true);
			Mysticism_C_EntriesFound = std::ranges::count(Mysticism_C_BoolArray, true);
			Mysticism_D_EntriesFound = std::ranges::count(Mysticism_D_BoolArray, true);
			Mysticism_I_EntriesFound = std::ranges::count(Mysticism_I_BoolArray, true);
			Mysticism_R_EntriesFound = std::ranges::count(Mysticism_R_BoolArray, true);
		}

		if (Serialization::CompletionistData::IsModInstalled(filename_Odin)) {
			for (auto i = 0; i < Odin_A_FormArray.size(); i++) {
				Odin_A_BoolArray[i] = FrameworkAPI::IsBookKnown(Odin_A_FormArray[i]);
			}

			for (auto i = 0; i < Odin_C_FormArray.size(); i++) {
				Odin_C_BoolArray[i] = FrameworkAPI::IsBookKnown(Odin_C_FormArray[i]);
			}

			for (auto i = 0; i < Odin_D_FormArray.size(); i++) {
				Odin_D_BoolArray[i] = FrameworkAPI::IsBookKnown(Odin_D_FormArray[i]);
			}

			for (auto i = 0; i < Odin_I_FormArray.size(); i++) {
				Odin_I_BoolArray[i] = FrameworkAPI::IsBookKnown(Odin_I_FormArray[i]);
			}

			for (auto i = 0; i < Odin_R_FormArray.size(); i++) {
				Odin_R_BoolArray[i] = FrameworkAPI::IsBookKnown(Odin_R_FormArray[i]);
			}

			Odin_A_EntriesTotal = Odin_A_FormArray.size();
			Odin_C_EntriesTotal = Odin_C_FormArray.size();
			Odin_D_EntriesTotal = Odin_D_FormArray.size();
			Odin_I_EntriesTotal = Odin_I_FormArray.size();
			Odin_R_EntriesTotal = Odin_R_FormArray.size();

			Odin_A_EntriesFound = std::ranges::count(Odin_A_BoolArray, true);
			Odin_C_EntriesFound = std::ranges::count(Odin_C_BoolArray, true);
			Odin_D_EntriesFound = std::ranges::count(Odin_D_BoolArray, true);
			Odin_I_EntriesFound = std::ranges::count(Odin_I_BoolArray, true);
			Odin_R_EntriesFound = std::ranges::count(Odin_R_BoolArray, true);
		}

		if (Serialization::CompletionistData::IsModInstalled(filename_Triumvirate)) {
			for (auto i = 0; i < Triumvirate_A_FormArray.size(); i++) {
				Triumvirate_A_BoolArray[i] = FrameworkAPI::IsBookKnown(Triumvirate_A_FormArray[i]);
			}

			for (auto i = 0; i < Triumvirate_C_FormArray.size(); i++) {
				Triumvirate_C_BoolArray[i] = FrameworkAPI::IsBookKnown(Triumvirate_C_FormArray[i]);
			}

			for (auto i = 0; i < Triumvirate_D_FormArray.size(); i++) {
				Triumvirate_D_BoolArray[i] = FrameworkAPI::IsBookKnown(Triumvirate_D_FormArray[i]);
			}

			for (auto i = 0; i < Triumvirate_I_FormArray.size(); i++) {
				Triumvirate_I_BoolArray[i] = FrameworkAPI::IsBookKnown(Triumvirate_I_FormArray[i]);
			}

			for (auto i = 0; i < Triumvirate_R_FormArray.size(); i++) {
				Triumvirate_R_BoolArray[i] = FrameworkAPI::IsBookKnown(Triumvirate_R_FormArray[i]);
			}

			Triumvirate_A_EntriesTotal = Triumvirate_A_FormArray.size();
			Triumvirate_C_EntriesTotal = Triumvirate_C_FormArray.size();
			Triumvirate_D_EntriesTotal = Triumvirate_D_FormArray.size();
			Triumvirate_I_EntriesTotal = Triumvirate_I_FormArray.size();
			Triumvirate_R_EntriesTotal = Triumvirate_R_FormArray.size();

			Triumvirate_A_EntriesFound = std::ranges::count(Triumvirate_A_BoolArray, true);
			Triumvirate_C_EntriesFound = std::ranges::count(Triumvirate_C_BoolArray, true);
			Triumvirate_D_EntriesFound = std::ranges::count(Triumvirate_D_BoolArray, true);
			Triumvirate_I_EntriesFound = std::ranges::count(Triumvirate_I_BoolArray, true);
			Triumvirate_R_EntriesFound = std::ranges::count(Triumvirate_R_BoolArray, true);
		}
	}
}