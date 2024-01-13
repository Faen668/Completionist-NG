#pragma once

namespace CQFramework_Morthal
{
	class CHandler
	{
	public:
		static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }
		static void InstallFramework();
	};
}