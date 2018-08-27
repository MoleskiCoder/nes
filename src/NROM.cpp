#include "stdafx.h"
#include "NROM.h"

#include <cassert>

NROM::NROM(const NesFile& nesFile) {

	const auto& content = nesFile.content();

	const auto sizePRGROM = nesFile.getSizePRGROM();
	PRG().resize(sizePRGROM);
	for (size_t i = 0; i != sizePRGROM; ++i) {
		auto& rom = PRG()[i];
		rom.resize(0x4000);
		std::copy_n(
			content.cbegin() + 16 + 0x4000 * i,
			0x4000,
			rom.begin());
	}

	const auto sizeCHRROM = nesFile.getSizeCHRROM();
	CHR().resize(sizeCHRROM);
	for (size_t i = 0; i != sizeCHRROM; ++i) {
		auto& rom = CHR()[i];
		rom.resize(0x2000);
		std::copy_n(
			content.cbegin() + 16 + 0x4000 * sizePRGROM + 0x2000 * i,
			0x2000,
			rom.begin());
	}

	PRGRAM().resize(0x2000);
}

uint8_t& NROM::reference(uint16_t address) {

	if (address < 0x8000)
		return PRGRAM()[address - 0x6000];

	if (address < 0xC000)
		return m_temporary = PRG()[0][address - 0x8000];

	const auto roms = PRG().size();
	return m_temporary = PRG()[roms - 1][address - 0xc000];
}