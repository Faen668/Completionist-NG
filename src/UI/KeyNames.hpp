#pragma once

namespace CompletionistUI
{
	inline std::string KeyName(std::uint32_t a_scanCode)
	{
		// 0x80 and up are extended keys
		const LONG lParam = static_cast<LONG>(((a_scanCode & 0x7F) << 16) | ((a_scanCode & 0x80) ? (1 << 24) : 0));
		wchar_t    name[64]{};
		if (GetKeyNameTextW(lParam, name, 64) <= 0) {
			return std::to_string(a_scanCode);
		}

		const int   size = WideCharToMultiByte(CP_UTF8, 0, name, -1, nullptr, 0, nullptr, nullptr);
		std::string result(static_cast<std::size_t>(std::max(size - 1, 0)), '\0');
		WideCharToMultiByte(CP_UTF8, 0, name, -1, result.data(), size, nullptr, nullptr);
		return result;
	}
}
