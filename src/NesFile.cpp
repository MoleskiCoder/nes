#include "stdafx.h"
#include "NesFile.h"

#include <fstream>
#include <vector>

#include <Memory.h>

void NesFile::load(const std::string& path) {

	std::ifstream file;
	file.exceptions(std::ios::failbit | std::ios::badbit);

	file.open(path, std::ios::binary | std::ios::ate);

	std::vector<uint8_t> header;
	EightBit::Memory::load(file, header, 0, 0, 16, -1);
	std::copy(header.cbegin(), header.cend(), m_header.raw);

	m_prg.resize(m_header.decoded.prgRomSize);
	for (int i = 0; i < m_header.decoded.prgRomSize; ++i)
		EightBit::Memory::load(file, m_prg[i], 0, 16 + 0x4000 * i, 0x4000, -1);

	m_chr.resize(m_header.decoded.chrRomSize);
	for (int i = 0; i < m_header.decoded.chrRomSize; ++i)
		EightBit::Memory::load(file, m_chr[i], 0, 16 + 0x2000 * i, 0x2000, -1);

	m_prgRam.resize(0x2000);

	file.close();
}
