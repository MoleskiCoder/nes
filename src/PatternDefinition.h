#pragma once

#include <array>
#include <cstdint>
#include <Ram.h>

class PatternDefinition final {
public:
	PatternDefinition(const EightBit::Ram& vram, uint16_t base, int pattern);

	std::array<int, 8> get(int row) const;

private:
	const EightBit::Ram& VRAM() const { return m_vram; }

	const EightBit::Ram& m_vram;
	const int m_pattern;
	const uint16_t m_base;
	const uint16_t m_address;
};

