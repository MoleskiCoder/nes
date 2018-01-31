#include "stdafx.h"
#include "Cartridge.h"
#include "NROM.h"
#include "MMC1.h"

Cartridge::Cartridge(EightBit::Bus& bus, const std::string& path)
: m_bus(bus) {

	m_container.load(path);
	m_container.dumpInformation();

	const auto mapper = m_container.getMapper();

	switch (mapper) {
	case 0:
		m_mapper.reset(new NROM(m_container));
		break;
	case 1:
		m_mapper.reset(new MMC1(m_bus, m_container));
		break;
	default:
		throw std::exception("Unhandled cartridge mapper");
	}
}

uint8_t& Cartridge::reference(uint16_t address, bool& rom) {
	if (address < 0x4018)
		throw new std::domain_error("Address is too low. i.e. not a cartridge address");
	return m_mapper->reference(address, rom);
}