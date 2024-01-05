#pragma once

#include <cstdint>
#include <array>
#include <vector>

#include <Bus.h>
#include <Ram.h>
#include <Mapper.h>

#include "ColourPalette.h"

class Display final : public EightBit::Mapper {
public:
	enum {

		BasePaletteAddress = 0x3f00,

		RasterWidth = 256,
		RasterHeight = 240,

		PPU_START = 0x2000,
		PPU_END = 0x4000,

		idxPPUCTRL = 0,
		idxPPUMASK,
		idxPPUSTATUS,
		idxOAMADDR,
		idxOAMDATA,
		idxPPUSCROLL,
		idxPPUADDR,
		idxPPUDATA,
	};

	Display(EightBit::Bus& bus, const ColourPalette& palette);

	EightBit::MemoryMapping mapping(uint16_t address) noexcept final;

	bool nmi() const { return PPUCTRL().nmi(); }

	void setVBlank() { vblankOccurring(true); }
	void clearVBlank() { vblankOccurring(false); }

	void triggerOAMDMA(uint8_t page);
	bool stepOAMDMA();	// true, if taken

	const std::vector<uint32_t>& pixels() const;

	void initialise();
	void render();

	void startRender() { m_currentScanLine = 0; }
	void finishRender() { m_currentScanLine = -1; }

private:
	static uint16_t addressMask() { return 7;  }
	static size_t maskAddress(const uint16_t address) { return address & addressMask(); }

	static bool validAddress(uint16_t address);
	static bool invalidAddress(uint16_t address);
	static EightBit::MemoryMapping::AccessLevel convertAddress(uint16_t address);

	void Bus_WritingByte(const EightBit::EventArgs& e);
	void Bus_WrittenByte(const EightBit::EventArgs& e);
	void Bus_ReadingByte(const EightBit::EventArgs& e);
	void Bus_ReadByte(const EightBit::EventArgs& e);

	EightBit::Bus& BUS() { return m_bus; }
	EightBit::Ram& VRAM() { return m_vram; }
	EightBit::Ram& OAMRAM() { return m_oamram; }

	uint8_t& OAMDATA() { return access(idxOAMDATA).raw; }
	uint8_t& OAMADDR() { return access(idxOAMADDR).raw; }

	uint8_t& ppuScrollX() { return m_ppuScrollPosition[0]; }
	uint8_t& ppuScrollY() { return m_ppuScrollPosition[1]; }
	uint16_t ppuAddress() const { return (m_ppuAddress[0] << 8) | m_ppuAddress[1]; }

	int selectPalette(int row, int column);
	std::array<uint8_t, 4> buildBackgroundPalette(int selected);

	void incrementPPUAddress() {
		EightBit::register16_t address;
		address.word = ppuAddress() + addressIncrement();
		m_ppuAddress[0] = address.high;
		m_ppuAddress[1] = address.low;
	}

	EightBit::Bus& m_bus;

	// Control register 1 wrappers:
	uint16_t nameTableAddress() const { return PPUCTRL().nameTableAddress(); }
	uint16_t attributeTableAddress() const { return nameTableAddress() + (0x400 - 64); }
	int addressIncrement() const { return PPUCTRL().addressIncrement(); }
	uint16_t spritePatternTableAddress() const { return PPUCTRL().spritePatternTableAddress(); }
	uint16_t backgroundPatternTableAddress() const { return PPUCTRL().backgroundPatternTableAddress(); }
	int spriteHeight() const { return PPUCTRL().spriteHeight(); }

	// Control register 2 wrappers:
	bool colour() const { return PPUMASK().colour(); }
	bool clipBackground() const { return PPUMASK().clipBackground(); }
	bool clipSprites() const { return PPUMASK().clipSprites(); }
	bool showBackground() const { return PPUMASK().showBackground(); }
	bool showSprites() const { return PPUMASK().showSprites(); }
	int backgroundIntensity() const { return PPUMASK().backgroundIntensity(); }

	// Status register wrappers
	void ignoreVramWrites(bool value) { PPUSTATUS().ignoreVramWrites(value); }
	void excessiveScanlineSprites(bool value) { PPUSTATUS().excessiveScanlineSprites(value); }
	void spriteZeroHit(bool value) { PPUSTATUS().spriteZeroHit(value); }
	void vblankOccurring(bool value) { PPUSTATUS().vblankOccurring(value); }

	// PPUCTRL
	struct control_register_1_t {
		uint8_t _nameTable : 2;
		uint8_t _addressIncrement : 1;
		uint8_t _spritePatternTable : 1;
		uint8_t _backgroundPatternTable : 1;
		uint8_t _spriteHeight : 1;
		uint8_t _mode : 1;	// Master or slave
		uint8_t _nmi : 1;

		uint16_t nameTableAddress() const { return 0x2000 + _nameTable * 0x400; }
		int addressIncrement() const { return _addressIncrement ? 32 : 1; }
		uint16_t spritePatternTableAddress() const { return _spritePatternTable * 0x1000; }
		uint16_t backgroundPatternTableAddress() const { return _backgroundPatternTable * 0x1000; }
		int spriteHeight() const { return _spriteHeight * 8 + 8; }
		bool nmi() const { return !!_nmi; }
	};

	// PPUMASK
	struct control_register_2_t {
		uint8_t _colour : 1;
		uint8_t _clipBackground : 1;
		uint8_t _clipSprites : 1;
		uint8_t _showBackground : 1;
		uint8_t _showSprites : 1;
		uint8_t _backgroundIntensity : 3;

		bool colour() const { return !!_colour; }
		bool clipBackground() const { return !!_clipBackground; }
		bool clipSprites() const { return !!_clipSprites; }
		bool showBackground() const { return !!_showBackground; }
		bool showSprites() const { return !!_showSprites; }
		int backgroundIntensity() const { return _backgroundIntensity; }
	};

	// PPUSTATUS
	struct status_register_t {
		uint8_t _unused : 4;
		uint8_t _ignoreVramWrites : 1;
		uint8_t _excessiveScanlineSprites : 1;
		uint8_t _spriteZeroHit : 1;
		uint8_t _vblankOccurring : 1;

		void ignoreVramWrites(bool value) { _ignoreVramWrites = value; }
		void excessiveScanlineSprites(bool value) { _excessiveScanlineSprites = value; }
		void spriteZeroHit(bool value) { _spriteZeroHit = value; }
		void vblankOccurring(bool value) { _vblankOccurring = value; }
	};

	union register_t {
		uint8_t raw;
		control_register_1_t decodedPPUCTRL;
		control_register_2_t decodedPPUMASK;
		status_register_t decodedPPUSTATUS;
	};

	register_t access(const uint16_t i) const { return { m_registers.peek(i) }; }
	register_t& access(const uint16_t i) { return (register_t&)m_registers.reference(i); }

	control_register_1_t PPUCTRL() const { return access(idxPPUCTRL).decodedPPUCTRL; }
	control_register_1_t& PPUCTRL() { return access(idxPPUCTRL).decodedPPUCTRL; }

	control_register_2_t PPUMASK() const { return access(idxPPUMASK).decodedPPUMASK; }
	control_register_2_t& PPUMASK() { return access(idxPPUMASK).decodedPPUMASK; }

	status_register_t PPUSTATUS() const { return access(idxPPUSTATUS).decodedPPUSTATUS; }
	status_register_t& PPUSTATUS() { return access(idxPPUSTATUS).decodedPPUSTATUS; }

	EightBit::Ram m_registers = 8;

	// PPUSCROLL
	size_t m_ppuScrollLatch : 1;
	std::array<uint8_t, 2> m_ppuScrollPosition;	// X,Y

	// PPUADDR
	size_t m_ppuAddressLatch : 1;
	std::array<uint8_t, 2> m_ppuAddress;	// high, low

	EightBit::Ram m_vram = 0x4000;
	EightBit::Ram m_oamram = 0x100;

	bool m_oamdmaActive = false;
	EightBit::register16_t m_oamdmaAddress = { 0, 0 };

	std::vector<uint32_t> m_pixels;
	const ColourPalette& m_palette;

	int m_currentScanLine = 0;
};
