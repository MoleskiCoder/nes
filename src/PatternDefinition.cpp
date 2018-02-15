#include "stdafx.h"
#include "PatternDefinition.h"

PatternDefinition::PatternDefinition(const EightBit::Ram& vram, const uint16_t base, const int pattern)
: m_vram(vram),
  m_pattern(pattern),
  m_base(base),
  m_address(base + pattern * 0x10) {
}

std::array<int, 8> PatternDefinition::get(const int row) const {

	std::array<int, 8> returned;

	const auto planeAddress = m_address + row;

	const auto planeLow = VRAM().peek(planeAddress);
	const auto planeHigh = VRAM().peek(planeAddress + 8);

	for (int bit = 0; bit < 8; ++bit) {

		const auto mask = 1 << bit;

		const auto bitLow = planeLow & mask ? 1 : 0;
		const auto bitHigh = planeHigh & mask ? 0b10 : 0;

		const auto colour = bitHigh | bitLow;

		returned[7 - bit] = colour;
	}

	return returned;
}
