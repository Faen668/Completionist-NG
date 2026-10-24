#pragma once

#include <cstddef>
#include <span>
#include <vector>

// zlib inflate from RFC 1950 / 1951, checksum not checked
namespace Inflate
{
	bool Zlib(std::span<const std::byte> a_in, std::vector<std::byte>& a_out);
}
