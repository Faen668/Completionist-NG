#include "Internal Utility/Inflate.hpp"

#include <array>
#include <cstdint>

namespace Inflate
{
	namespace
	{
		constexpr int maxBits = 15;

		// LSB first (RFC 1951 3.1.1)
		class BitReader
		{
		public:
			explicit BitReader(std::span<const std::byte> a_data) :
				_data(a_data)
			{}

			[[nodiscard]] bool Ok() const noexcept { return _ok; }

			std::uint32_t Bits(int a_count) {

				std::uint32_t value = 0;
				for (int i = 0; i < a_count; ++i) {
					if (_pos >= _data.size()) {
						_ok = false;
						return 0;
					}
					const auto bit = (static_cast<std::uint32_t>(_data[_pos]) >> _bit) & 1u;
					value |= bit << i;
					if (++_bit == 8) {
						_bit = 0;
						++_pos;
					}
				}
				return value;
			}

			// Stored blocks start on a byte boundary.
			void AlignToByte() {

				if (_bit != 0) {
					_bit = 0;
					++_pos;
				}
			}

			bool Copy(std::size_t a_count, std::vector<std::byte>& a_out) {

				if (_pos + a_count > _data.size()) {
					_ok = false;
					return false;
				}
				a_out.insert(a_out.end(), _data.begin() + static_cast<std::ptrdiff_t>(_pos), _data.begin() + static_cast<std::ptrdiff_t>(_pos + a_count));
				_pos += a_count;
				return true;
			}

		private:
			std::span<const std::byte> _data;
			std::size_t                _pos{ 0 };
			int                        _bit{ 0 };
			bool                       _ok{ true };
		};

		// Canonical Huffman (3.2.2)
		struct Huffman
		{
			std::array<std::uint16_t, maxBits + 1> counts{};
			std::vector<std::uint16_t>             symbols;

			bool Build(std::span<const std::uint8_t> a_lengths) {

				counts.fill(0);
				for (const auto length : a_lengths) {
					++counts[length];
				}
				counts[0] = 0;

				int left = 1;
				for (int bits = 1; bits <= maxBits; ++bits) {
					left = (left << 1) - counts[bits];
					if (left < 0) {
						return false;
					}
				}

				std::array<std::uint16_t, maxBits + 2> offsets{};
				for (int bits = 1; bits <= maxBits; ++bits) {
					offsets[bits + 1] = static_cast<std::uint16_t>(offsets[bits] + counts[bits]);
				}
				symbols.assign(a_lengths.size(), 0);
				for (std::size_t symbol = 0; symbol < a_lengths.size(); ++symbol) {
					if (a_lengths[symbol] != 0) {
						symbols[offsets[a_lengths[symbol]]++] = static_cast<std::uint16_t>(symbol);
					}
				}
				return true;
			}

			int Decode(BitReader& a_reader) const {

				int code = 0, first = 0, index = 0;
				for (int bits = 1; bits <= maxBits; ++bits) {
					code |= static_cast<int>(a_reader.Bits(1));
					const int count = counts[bits];
					if (code - first < count) {
						return symbols[static_cast<std::size_t>(index + code - first)];
					}
					index += count;
					first = (first + count) << 1;
					code <<= 1;
					if (!a_reader.Ok()) {
						break;
					}
				}
				return -1;
			}
		};

		// 3.2.5
		constexpr std::array<std::uint16_t, 29> lengthBase{ 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258 };
		constexpr std::array<std::uint8_t, 29>  lengthExtra{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0 };
		constexpr std::array<std::uint16_t, 30> distanceBase{ 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577 };
		constexpr std::array<std::uint8_t, 30>  distanceExtra{ 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };

		//---------------------------------------------------
		//-- Inflate ( Codes ) ------------------------------
		//---------------------------------------------------

		bool Codes(BitReader& a_reader, const Huffman& a_literals, const Huffman& a_distances, std::vector<std::byte>& a_out, std::size_t a_start) {

			for (;;) {
				const int symbol = a_literals.Decode(a_reader);
				if (symbol < 0) {
					return false;
				}
				if (symbol < 256) {
					a_out.push_back(static_cast<std::byte>(symbol));
					continue;
				}
				if (symbol == 256) {
					return true;
				}

				const int lengthIndex = symbol - 257;
				if (lengthIndex >= static_cast<int>(lengthBase.size())) {
					return false;
				}
				const std::size_t length = lengthBase[lengthIndex] + a_reader.Bits(lengthExtra[lengthIndex]);
				const int         distanceSymbol = a_distances.Decode(a_reader);
				if (distanceSymbol < 0 || distanceSymbol >= static_cast<int>(distanceBase.size())) {
					return false;
				}
				const std::size_t distance = distanceBase[distanceSymbol] + a_reader.Bits(distanceExtra[distanceSymbol]);
				if (!a_reader.Ok() || distance > a_out.size() - a_start) {
					return false;
				}
				// Copy may overlap
				const std::size_t from = a_out.size() - distance;
				for (std::size_t i = 0; i < length; ++i) {
					a_out.push_back(a_out[from + i]);
				}
			}
		}
	}

	//---------------------------------------------------
	//-- Inflate ( Zlib ) -------------------------------
	//---------------------------------------------------

	bool Zlib(std::span<const std::byte> a_in, std::vector<std::byte>& a_out) {

		if (a_in.size() < 2 || (static_cast<std::uint8_t>(a_in[0]) & 0x0F) != 8 || (static_cast<std::uint8_t>(a_in[1]) & 0x20) != 0) {
			return false;
		}

		BitReader  reader(a_in.subspan(2));
		const auto start = a_out.size();
		bool       last = false;
		while (!last) {
			last = reader.Bits(1) != 0;
			const auto type = reader.Bits(2);
			if (!reader.Ok()) {
				return false;
			}

			if (type == 0) {
				reader.AlignToByte();
				const auto length = reader.Bits(16);
				const auto check = reader.Bits(16);
				if (!reader.Ok() || length != (~check & 0xFFFF) || !reader.Copy(length, a_out)) {
					return false;
				}
			} else if (type == 1) {
				// Fixed codes (3.2.6)
				static const auto fixed = [] {
					std::array<std::uint8_t, 288> lengths{};
					for (int i = 0; i < 144; ++i) lengths[i] = 8;
					for (int i = 144; i < 256; ++i) lengths[i] = 9;
					for (int i = 256; i < 280; ++i) lengths[i] = 7;
					for (int i = 280; i < 288; ++i) lengths[i] = 8;
					std::array<std::uint8_t, 30> distances{};
					distances.fill(5);
					std::pair<Huffman, Huffman> codes;
					codes.first.Build(lengths);
					codes.second.Build(distances);
					return codes;
				}();
				if (!Codes(reader, fixed.first, fixed.second, a_out, start)) {
					return false;
				}
			} else if (type == 2) {
				// Dynamic codes (3.2.7)
				const auto literalCount = reader.Bits(5) + 257;
				const auto distanceCount = reader.Bits(5) + 1;
				const auto lengthCount = reader.Bits(4) + 4;
				if (!reader.Ok() || literalCount > 286 || distanceCount > 30) {
					return false;
				}

				constexpr std::array<std::uint8_t, 19> order{ 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
				std::array<std::uint8_t, 19>           codeLengths{};
				for (std::uint32_t i = 0; i < lengthCount; ++i) {
					codeLengths[order[i]] = static_cast<std::uint8_t>(reader.Bits(3));
				}
				Huffman lengthCode;
				if (!lengthCode.Build(codeLengths)) {
					return false;
				}

				std::vector<std::uint8_t> lengths;
				lengths.reserve(literalCount + distanceCount);
				while (lengths.size() < literalCount + distanceCount) {
					const int symbol = lengthCode.Decode(reader);
					if (symbol < 0) {
						return false;
					}
					if (symbol < 16) {
						lengths.push_back(static_cast<std::uint8_t>(symbol));
						continue;
					}
					std::uint8_t value = 0;
					std::size_t  repeat = 0;
					if (symbol == 16) {
						if (lengths.empty()) {
							return false;
						}
						value = lengths.back();
						repeat = 3 + reader.Bits(2);
					} else if (symbol == 17) {
						repeat = 3 + reader.Bits(3);
					} else {
						repeat = 11 + reader.Bits(7);
					}
					if (lengths.size() + repeat > literalCount + distanceCount) {
						return false;
					}
					lengths.insert(lengths.end(), repeat, value);
				}
				if (lengths[256] == 0) {
					return false;  // no end-of-block code
				}

				Huffman literals, distances;
				if (!literals.Build(std::span(lengths).first(literalCount)) || !distances.Build(std::span(lengths).subspan(literalCount))) {
					return false;
				}
				if (!Codes(reader, literals, distances, a_out, start)) {
					return false;
				}
			} else {
				return false;
			}
		}
		return true;
	}
}
