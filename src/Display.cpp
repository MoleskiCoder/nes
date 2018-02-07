#include "stdafx.h"
#include "Display.h"

Display::Display(EightBit::Bus& bus)
: m_bus(bus) {
	bus.WritingByte.connect(std::bind(&Display::Bus_WritingByte, this, std::placeholders::_1));
	bus.WrittenByte.connect(std::bind(&Display::Bus_WrittenByte, this, std::placeholders::_1));
	bus.ReadingByte.connect(std::bind(&Display::Bus_ReadingByte, this, std::placeholders::_1));
	bus.ReadByte.connect(std::bind(&Display::Bus_ReadByte, this, std::placeholders::_1));
}

bool Display::convertAddress(const uint16_t address, size_t& index, bool& writable, bool& readable) {

	if ((address >= PPU_START) && (address <= PPU_FINISH)) {
		index = address & 7;
	} else if (address == (0x4000 + OAMDMA)) {
		index = 8;
	} else {
		return false;
	}

	writable = readable = false;
	switch (index) {
	case PPUSTATUS:
		readable = true;
		break;
	case PPUDATA:
		readable = writable = true;
		break;
	case PPUCTRL:
	case PPUMASK:
	case OAMADDR:
	case OAMDATA:
	case PPUSCROLL:
	case PPUADDR:
	case 8:			// OAMDMA
		writable = true;
		break;
	default:
		UNREACHABLE;
	}
	return true;
}

uint8_t& Display::reference(const uint16_t address, bool& rom) {
	size_t index = 0;
	bool writable = false, readable = false;
	if (convertAddress(address, index, writable, readable)) {
		rom = !writable;
		return m_registers[index].raw;
	}
	UNREACHABLE;
}

void Display::Bus_WritingByte(const uint16_t address) {

	size_t index = 0;
	bool writable = false, readable = false;
	if (!convertAddress(address, index, writable, readable))
		return;

	const auto& data = BUS().DATA();

	switch (index) {
	case PPUCTRL:
		break;
	case PPUMASK:
		break;
	case PPUSTATUS:
		break;
	case OAMADDR:
		break;
	case OAMDATA:
		break;
	case PPUSCROLL:
		break;
	case PPUADDR:
		break;
	case PPUDATA:
		break;
	case 8:		// OAMDMA
		break;
	default:
		UNREACHABLE;
	}
}

void Display::Bus_WrittenByte(const uint16_t address) {

	size_t index = 0;
	bool writable = false, readable = false;
	if (!convertAddress(address, index, writable, readable))
		return;

	const auto& data = BUS().DATA();

	switch (index) {
	case PPUCTRL:
		break;
	case PPUMASK:
		break;
	case PPUSTATUS:
		break;
	case OAMADDR:
		break;
	case OAMDATA:
		break;
	case PPUSCROLL:
		break;
	case PPUADDR:
		break;
	case PPUDATA:
		break;
	case 8:		// OAMDMA
		break;
	default:
		UNREACHABLE;
	}
}

void Display::Bus_ReadingByte(const uint16_t address) {

	size_t index = 0;
	bool writable = false, readable = false;
	if (!convertAddress(address, index, writable, readable))
		return;

	switch (index) {
	case PPUCTRL:
		break;
	case PPUMASK:
		break;
	case PPUSTATUS:
		break;
	case OAMADDR:
		break;
	case OAMDATA:
		break;
	case PPUSCROLL:
		break;
	case PPUADDR:
		break;
	case PPUDATA:
		break;
	case 8:		// OAMDMA
		break;
	default:
		UNREACHABLE;
	}
}

void Display::Bus_ReadByte(const uint16_t address) {

	size_t index = 0;
	bool writable = false, readable = false;
	if (!convertAddress(address, index, writable, readable))
		return;

	switch (index) {
	case PPUCTRL:
		break;
	case PPUMASK:
		break;
	case PPUSTATUS:
		clearVBlank();
		m_ppuScrollLatch = m_ppuAddressLatch = 0;
		break;
	case OAMADDR:
		break;
	case OAMDATA:
		break;
	case PPUSCROLL:
		break;
	case PPUADDR:
		break;
	case PPUDATA:
		break;
	case 8:		// OAMDMA
		break;
	default:
		UNREACHABLE;
	}
}
