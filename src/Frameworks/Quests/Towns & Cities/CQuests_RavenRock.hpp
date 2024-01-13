#pragma once

namespace CQFramework_RavenRock
{
	class CHandler
	{
	public:
		static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }
		static void InstallFramework();
	};
}