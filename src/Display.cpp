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

	if ((address < PPU_START) || (address > PPU_FINISH))
		return false;

	index = address & 7;

	writable = readable = false;
	switch (index) {
	case idxPPUSTATUS:
		readable = true;
		break;
	case idxOAMDATA:
	case idxPPUDATA:
		readable = writable = true;
		break;
	case idxPPUCTRL:
	case idxPPUMASK:
	case idxOAMADDR:
	case idxPPUSCROLL:
	case idxPPUADDR:
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

	switch (index) {
	case idxPPUCTRL:
		break;
	case idxPPUMASK:
		break;
	case idxPPUSTATUS:
		break;
	case idxOAMADDR:
		break;
	case idxOAMDATA:
		break;
	case idxPPUSCROLL:
		break;
	case idxPPUADDR:
		break;
	case idxPPUDATA:
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
	case idxPPUCTRL:
		break;
	case idxPPUMASK:
		break;
	case idxPPUSTATUS:
		break;
	case idxOAMADDR:
		OAMADDR() = data;
		break;
	case idxOAMDATA:
		OAMRAM().poke(OAMADDR()++, data);
		break;
	case idxPPUSCROLL:
		break;
	case idxPPUADDR:
		m_ppuAddress[m_ppuAddressLatch++] = data;
		break;
	case idxPPUDATA:
		VRAM().poke(ppuAddress() & 0x3fff, data);
		incrementPPUAddress();
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
	case idxPPUCTRL:
		break;
	case idxPPUMASK:
		break;
	case idxPPUSTATUS:
		break;
	case idxOAMADDR:
		break;
	case idxOAMDATA:
		OAMDATA() = OAMRAM().peek(OAMADDR());
		break;
	case idxPPUSCROLL:
		break;
	case idxPPUADDR:
		break;
	case idxPPUDATA:
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
	case idxPPUCTRL:
		break;
	case idxPPUMASK:
		break;
	case idxPPUSTATUS:
		clearVBlank();
		m_ppuScrollLatch = m_ppuAddressLatch = 0;
		break;
	case idxOAMADDR:
		break;
	case idxOAMDATA:
		break;
	case idxPPUSCROLL:
		break;
	case idxPPUADDR:
		break;
	case idxPPUDATA:
		break;
	default:
		UNREACHABLE;
	}
}

void Display::triggerOAMDMA(const uint8_t page) {
	m_oamdmaAddress.high = page;
	m_oamdmaAddress.low = 0;
	m_oamdmaActive = true;
}

bool Display::stepOAMDMA() {
	const bool taken = m_oamdmaActive;
	if (m_oamdmaActive) {
		const uint8_t datum = BUS().peek(m_oamdmaAddress.word);
		OAMRAM().poke(OAMADDR()++, datum);
		m_oamdmaActive = !!(++m_oamdmaAddress.low);	// i.e. DMA finishes when the low part of the address has wrapped
	}
	return taken;
}
