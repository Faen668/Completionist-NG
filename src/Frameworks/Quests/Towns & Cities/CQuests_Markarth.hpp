#pragma once

namespace CQFramework_Markarth
{
	class CHandler
	{
	private:
		CHandler() = default;
		~CHandler() = default;

	public:
		CHandler(CHandler const&) = delete;
		CHandler(CHandler const&&) = delete;
		CHandler operator=(CHandler&) = delete;
		CHandler operator=(CHandler&&) = delete;

		static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }
		static void InstallFramework();
	};
}
