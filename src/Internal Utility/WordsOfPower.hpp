#pragma once

// Where each word of power is taught, from PluginScan
namespace WordsOfPower
{
	struct Source
	{
		std::string place;
		std::string region;
		RE::FormID  location{ 0 };
		bool        quest{ false };
	};

	// Once, at kDataLoaded
	void StartScan();

	[[nodiscard]] bool Ready() noexcept;

	[[nodiscard]] std::vector<Source> SourcesFor(RE::FormID a_word);
}
