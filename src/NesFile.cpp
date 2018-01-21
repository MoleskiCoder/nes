#include "stdafx.h"
#include "NesFile.h"

#include <fstream>
#include <vector>

#include <Memory.h>

void NesFile::dumpInformation() const {

	std::cout << "Loaded cartridge: " << m_path << std::endl;

	std::cout << "PRG ROM size (16KiB units): " << (int)m_header.decoded.prgRomSize << std::endl;
	std::cout << "CHR ROM size (8KiB units): " << (int)m_header.decoded.chrRomSize << std::endl;
	std::cout << "Mapper type: " << (int)m_header.decoded.buildMapperType() << std::endl;
	std::cout << "PC10: " << m_header.decoded.pc10 << std::endl;
	std::cout << "VS Unisystem: " << m_header.decoded.pc10 << std::endl;
	std::cout << "PRG RAM size (8KiB units): " << m_header.decoded.buildPrgRamSize() << std::endl;
	std::cout << "TV system: " << m_header.decoded.tv << std::endl;
}

void NesFile::load(const std::string& path) {

	m_path = path;

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
