#pragma once

namespace CQFramework_CC3
{
	inline uint32_t QuestsInstalled{};

	class CHandler
	{
	public:
		static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }
		static void InstallFramework();
	};
}
