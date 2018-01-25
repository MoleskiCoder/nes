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

	EightBit::Memory::load(file, m_content);
	std::copy_n(m_content.cbegin(), sizeof(m_header.raw), m_header.raw);

	file.close();
}
