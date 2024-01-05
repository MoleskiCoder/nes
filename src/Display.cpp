#include "stdafx.h"
#include "Display.h"
#include "PatternDefinition.h"

Display::Display(EightBit::Bus& bus, const ColourPalette& palette)
: m_bus(bus),
  m_palette(palette) {
	bus.WritingByte.connect(std::bind(&Display::Bus_WritingByte, this, std::placeholders::_1));
	bus.WrittenByte.connect(std::bind(&Display::Bus_WrittenByte, this, std::placeholders::_1));
	bus.ReadingByte.connect(std::bind(&Display::Bus_ReadingByte, this, std::placeholders::_1));
	bus.ReadByte.connect(std::bind(&Display::Bus_ReadByte, this, std::placeholders::_1));
}

const std::vector<uint32_t>& Display::pixels() const {
	return m_pixels;
}

void Display::initialise() {
	m_pixels.resize(RasterWidth * RasterHeight);
}

void Display::render() {
	if (m_currentScanLine >= 0) {
		if (showBackground()) {

			const auto nameTable = nameTableAddress();
			const auto attributeTable = attributeTableAddress();

			const auto y = m_currentScanLine++;

			const auto row = y >> 3;
			const auto line = y & EightBit::Processor::Mask3;

			for (int column = 0; column < 32; ++column) {

				const int selectedPalette = selectPalette(row, column);
				const auto palette = buildBackgroundPalette(selectedPalette);

				const int pattern = VRAM().peek(nameTable + (row << 5) + column);
				const PatternDefinition definition(VRAM(), spritePatternTableAddress(), pattern);
				const auto rowPattern = definition.get(line);

				for (size_t x = 0; x < 8; ++x)
					m_pixels[y * RasterWidth + (column << 3) + x] = m_palette.getColour(palette[rowPattern[x]]);
			}
		}
	}
}

std::array<uint8_t, 4> Display::buildBackgroundPalette(const int selected) {
	std::array<uint8_t, 4> palette;
	palette[0] = VRAM().peek(BasePaletteAddress);
	palette[1] = VRAM().peek(BasePaletteAddress + (selected << 2) + 1);
	palette[2] = VRAM().peek(BasePaletteAddress + (selected << 2) + 2);
	palette[3] = VRAM().peek(BasePaletteAddress + (selected << 2) + 3);
	return palette;
}

int Display::selectPalette(const int row, const int column) {

	const auto attributeRow = row >> 2;
	const auto attributeColumn = column >> 2;

	const auto attribute = VRAM().peek((attributeRow << 3) + attributeColumn);

	const auto top = row & 1;
	const auto left = column & 1;

	const auto bottom = !top;
	const auto right = !left;

	if (top && left)
		return attribute && 0b11;
	if (top && right)
		return attribute && 0b1100;
	if (bottom && left)
		return attribute && 0b110000;
	if (bottom && right)
		return attribute && 0b11000000;

	UNREACHABLE;
}

bool Display::validAddress(const uint16_t address) {
	return (address >= PPU_START) && (address < PPU_END);
}

bool Display::invalidAddress(const uint16_t address) {
	return !validAddress(address);
}

EightBit::MemoryMapping::AccessLevel Display::convertAddress(const uint16_t address) {
	switch (maskAddress(address)) {
	case idxPPUSTATUS:
		return EightBit::MemoryMapping::AccessLevel::ReadOnly;
	case idxOAMDATA:
	case idxPPUDATA:
		return EightBit::MemoryMapping::AccessLevel::ReadWrite;
	case idxPPUCTRL:
	case idxPPUMASK:
	case idxOAMADDR:
	case idxPPUSCROLL:
	case idxPPUADDR:
		return EightBit::MemoryMapping::AccessLevel::WriteOnly;
	default:
		return EightBit::MemoryMapping::AccessLevel::Unknown;
	}
}

EightBit::MemoryMapping Display::mapping(const uint16_t address) noexcept {
	return { m_registers, 0, addressMask(), convertAddress(address) };
}

void Display::Bus_WritingByte(const EightBit::EventArgs& e) {

	const auto address = BUS().ADDRESS().word;
	if (invalidAddress(address))
		return;

	const auto index = maskAddress(address);
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

void Display::Bus_WrittenByte(const EightBit::EventArgs& e) {

	const auto address = BUS().ADDRESS().word;
	if (invalidAddress(address))
		return;

	const auto& data = BUS().DATA();

	const auto index = maskAddress(address);
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

void Display::Bus_ReadingByte(const EightBit::EventArgs& e) {

	const auto address = BUS().ADDRESS().word;
	if (invalidAddress(address))
		return;

	const auto index = maskAddress(address);
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

void Display::Bus_ReadByte(const EightBit::EventArgs& e) {

	const auto address = BUS().ADDRESS().word;
	if (invalidAddress(address))
		return;

	const auto index = maskAddress(address);
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
