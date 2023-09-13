#pragma once

namespace CQFramework_Misc_SK
{	
	inline std::vector<std::string>		NameArray_S;
	inline std::vector<std::string>		IdenArray_S;
	inline std::vector<std::string>		TextArray_S;
	inline std::vector<std::string>		KeysArray_S;
	inline std::vector<std::int32_t>	RadiArray_S;
	inline std::vector<bool>			BoolArray_S;

	inline std::vector<std::string>		NameArray_R;
	inline std::vector<std::string>		IdenArray_R;
	inline std::vector<std::string>		TextArray_R;
	inline std::vector<std::string>		KeysArray_R;
	inline std::vector<std::int32_t>	RadiArray_R;
	inline std::vector<bool>			BoolArray_R;

	class CHandler
	{
	public:
		static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }
		static void InstallFramework();
	};
}