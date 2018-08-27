#pragma once

#include <cstdint>
#include <vector>

#include "Mapper.h"
#include "NesFile.h"

// AKA Mapper 0
class NROM final : public Mapper {
public:
	NROM(const NesFile& nesFile);

	virtual uint8_t& reference(uint16_t address) final;

private:
	std::vector<uint8_t>& PRGRAM() { return m_prgRam; }
	std::vector<std::vector<uint8_t>>& PRG() { return m_prg; }
	std::vector<std::vector<uint8_t>>& CHR() { return m_chr; }

	std::vector<uint8_t> m_prgRam;
	std::vector<std::vector<uint8_t>> m_prg;
	std::vector<std::vector<uint8_t>> m_chr;
};
