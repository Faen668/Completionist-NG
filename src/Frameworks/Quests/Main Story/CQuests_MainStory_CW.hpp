#pragma once

namespace CQFramework_CW
{	
	inline std::vector<std::string>		NameArray;
	inline std::vector<std::string>		IdenArray;
	inline std::vector<std::string>		TextArray;
	inline std::vector<std::string>		KeysArray;
	inline std::vector<std::int32_t>	RadiArray;
	inline std::vector<bool>			BoolArray;

	class CHandler
	{
	public:
		static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }
		static void InstallFramework();
	};
}
