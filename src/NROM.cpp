#include "stdafx.h"
#include "NROM.h"

#include <cassert>

#include "NesFile.h"

NROM::NROM(const NesFile& nesFile) {

	const auto& content = nesFile.content();

	const auto sizePRGROM = nesFile.getSizePRGROM();
	PRG().resize(sizePRGROM);
	for (size_t i = 0; i != sizePRGROM; ++i)
		PRG()[i].load(content, 0, 16 + 0x4000 * i, 0x4000);

	const auto sizeCHRROM = nesFile.getSizeCHRROM();
	CHR().resize(sizeCHRROM);
	for (size_t i = 0; i != sizeCHRROM; ++i)
		CHR()[i].load(content, 0, 16 + 0x4000 * sizePRGROM + 0x2000 * i, 0x2000);
}

EightBit::MemoryMapping NROM::mapping(const uint16_t address) {

	if (address < 0x8000)
		return { m_unused8000, 0, 0xffff, EightBit::MemoryMapping::AccessLevel::ReadOnly };

	if (address < 0xC000)
		return { PRG()[0], 0x8000, 0xffff, EightBit::MemoryMapping::AccessLevel::ReadOnly };

	return { PRG()[PRG().size() - 1], 0xc000, 0xffff, EightBit::MemoryMapping::AccessLevel::ReadOnly };
}