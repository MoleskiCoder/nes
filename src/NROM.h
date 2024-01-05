#pragma once

#include <cstdint>
#include <vector>

#include <Mapper.h>
#include <Rom.h>
#include <UnusedMemory.h>

class NesFile;

// AKA Mapper 0
class NROM final : public EightBit::Mapper {
public:
	NROM(const NesFile& nesFile);

	EightBit::MemoryMapping mapping(uint16_t address) noexcept final;

private:
	auto& PRG() { return m_prg; }
	auto& CHR() { return m_chr; }

	EightBit::UnusedMemory m_unused8000 = { 0x8000, 0xff };
	std::vector<EightBit::Rom> m_prg;
	std::vector<EightBit::Rom> m_chr;
};
