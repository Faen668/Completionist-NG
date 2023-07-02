#include "Settings.hpp"

constexpr auto filepath = L"Data/SKSE/Plugins/Completionist.ini";

namespace Settings 
{
	void Main::Register() {
		SKSE::GetPapyrusInterface()->Register(RegisterFunctions);
	};

	auto Main::RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm) -> bool
	{
		a_vm->RegisterFunction("IsValid", "Completionist_Native", IsValid);

		a_vm->RegisterFunction("SaveB", "Completionist_Native", SaveBool);
		a_vm->RegisterFunction("SaveI", "Completionist_Native", SaveInt);
		a_vm->RegisterFunction("SaveF", "Completionist_Native", SaveFloat);
		a_vm->RegisterFunction("SaveS", "Completionist_Native", SaveStr);

		a_vm->RegisterFunction("LoadB", "Completionist_Native", LoadBool);
		a_vm->RegisterFunction("LoadI", "Completionist_Native", LoadInt);
		a_vm->RegisterFunction("LoadF", "Completionist_Native", LoadFloat);
		a_vm->RegisterFunction("LoadS", "Completionist_Native", LoadStr);
		return true;
	}

	bool Main::IsValid(RE::StaticFunctionTag*)
	{
		CSimpleIniA ini;
		ini.SetUnicode();

		SI_Error rc = ini.LoadFile(filepath);

		return (rc >= 0 && !ini.IsEmpty());
	}

	void Main::SaveBool(RE::StaticFunctionTag*, RE::BSFixedString section, RE::BSFixedString key, bool value) {
		
		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(filepath);

		SI_Error rc = ini.SetValue(section.data(), key.data(), value ? "true" : "false");
		if (rc < 0) { INFO("ERROR"); /* handle error */ };

		(void)ini.SaveFile(filepath);
	}

	void Main::SaveInt(RE::StaticFunctionTag*, RE::BSFixedString section, RE::BSFixedString key, int32_t value) {

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(filepath);

		SI_Error rc = ini.SetValue(section.data(), key.data(), std::to_string(value).c_str());
		if (rc < 0) { INFO("ERROR"); /* handle error */ };

		(void)ini.SaveFile(filepath);
	}

	void Main::SaveFloat(RE::StaticFunctionTag*, RE::BSFixedString section, RE::BSFixedString key, float value) {

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(filepath);

		SI_Error rc = ini.SetValue(section.data(), key.data(), std::to_string(value).c_str());
		if (rc < 0) { INFO("ERROR"); /* handle error */ };

		(void)ini.SaveFile(filepath);
	}

	void Main::SaveStr(RE::StaticFunctionTag*, RE::BSFixedString section, RE::BSFixedString key, RE::BSFixedString value) {

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(filepath);

		SI_Error rc = ini.SetValue(section.data(), key.data(), value.data());
		if (rc < 0) { INFO("ERROR"); /* handle error */ };

		(void)ini.SaveFile(filepath);
	}

	bool Main::LoadBool(RE::StaticFunctionTag*, RE::BSFixedString section, RE::BSFixedString key, RE::BSFixedString value) {

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(filepath);

		const char* pv;
		pv = ini.GetValue(section.data(), key.data(), "Error");

		return strcmp(pv, "true") == 0 ? true : false;
	}

	int32_t Main::LoadInt(RE::StaticFunctionTag*, RE::BSFixedString section, RE::BSFixedString key, RE::BSFixedString value) {

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(filepath);

		const char* pv;
		pv = ini.GetValue(section.data(), key.data(), "ERROR");

		return strcmp(pv, "ERROR") == 0 ? -1 : std::stoi(pv);
	}

	float Main::LoadFloat(RE::StaticFunctionTag*, RE::BSFixedString section, RE::BSFixedString key, RE::BSFixedString value) {

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(filepath);

		const char* pv;
		pv = ini.GetValue(section.data(), key.data(), "ERROR");

		return strcmp(pv, "ERROR") == 0 ? -1 : std::stof(pv);
	}

	RE::BSFixedString Main::LoadStr(RE::StaticFunctionTag*, RE::BSFixedString section, RE::BSFixedString key, RE::BSFixedString value) {

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(filepath);

		const char* pv;
		pv = ini.GetValue(section.data(), key.data(), "ERROR");

		return RE::BSFixedString(pv);
	}
}