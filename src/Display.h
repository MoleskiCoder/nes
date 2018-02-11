#pragma once

#include <cstdint>
#include <array>
#include <Bus.h>
#include <Ram.h>

#include "Mapper.h"

class Display final : public Mapper {
public:
	enum {
		PPU_START = 0x2000,
		PPU_FINISH = 0x3fff,
		idxPPUCTRL = 0,
		idxPPUMASK,
		idxPPUSTATUS,
		idxOAMADDR,
		idxOAMDATA,
		idxPPUSCROLL,
		idxPPUADDR,
		idxPPUDATA,
	};

	Display(EightBit::Bus& bus);

	virtual uint8_t& reference(uint16_t address, bool& rom) final;

	bool nmi() const { return m_registers[idxPPUCTRL].decodedPPUCTRL.nmi(); }

	void setVBlank() { vblankOccurring(true); }
	void clearVBlank() { vblankOccurring(false); }

	void triggerOAMDMA(uint8_t page);
	bool stepOAMDMA();	// true, if taken

private:
	static bool convertAddress(uint16_t address, size_t& index, bool& writable, bool& readable);

	void Bus_WritingByte(uint16_t address);
	void Bus_WrittenByte(uint16_t address);
	void Bus_ReadingByte(uint16_t address);
	void Bus_ReadByte(uint16_t address);

	EightBit::Bus& BUS() { return m_bus; }
	EightBit::Ram& VRAM() { return m_vram; }
	EightBit::Ram& OAMRAM() { return m_oamram; }

	uint8_t& OAMDATA() { return m_registers[idxOAMDATA].raw; }
	uint8_t& OAMADDR() { return m_registers[idxOAMADDR].raw; }

	uint8_t& ppuScrollX() { return m_ppuScrollPosition[0]; }
	uint8_t& ppuScrollY() { return m_ppuScrollPosition[1]; }
	uint16_t ppuAddress() const { return (m_ppuAddress[0] << 8) | m_ppuAddress[1]; }

	void incrementPPUAddress() {
		EightBit::register16_t address;
		address.word = ppuAddress() + addressIncrement();
		m_ppuAddress[0] = address.high;
		m_ppuAddress[1] = address.low;
	}

	EightBit::Bus& m_bus;

	// Control register 1 wrappers:
	uint16_t nameTableAddress() const { return m_registers[idxPPUCTRL].decodedPPUCTRL.nameTableAddress(); }
	int addressIncrement() const { return m_registers[idxPPUCTRL].decodedPPUCTRL.addressIncrement(); }
	uint16_t spritePatternTableAddress() const { return m_registers[idxPPUCTRL].decodedPPUCTRL.spritePatternTableAddress(); }
	uint16_t backgroundPatternTableAddress() const { return m_registers[idxPPUCTRL].decodedPPUCTRL.backgroundPatternTableAddress(); }
	int spriteHeight() const { return m_registers[idxPPUCTRL].decodedPPUCTRL.spriteHeight(); }

	// Control register 2 wrappers:
	bool colour() const { return m_registers[idxPPUMASK].decodedPPUMASK.colour(); }
	bool clipBackground() const { return m_registers[idxPPUMASK].decodedPPUMASK.clipBackground(); }
	bool clipSprites() const { return m_registers[idxPPUMASK].decodedPPUMASK.clipSprites(); }
	bool showBackground() const { return m_registers[idxPPUMASK].decodedPPUMASK.showBackground(); }
	bool showSprites() const { return m_registers[idxPPUMASK].decodedPPUMASK.showSprites(); }
	int backgroundIntensity() const { return m_registers[idxPPUMASK].decodedPPUMASK.backgroundIntensity(); }

	// Status register wrappers
	void ignoreVramWrites(bool value) { m_registers[idxPPUSTATUS].decodedPPUSTATUS.ignoreVramWrites(value); }
	void excessiveScanlineSprites(bool value) { m_registers[idxPPUSTATUS].decodedPPUSTATUS.excessiveScanlineSprites(value); }
	void spriteZeroHit(bool value) { m_registers[idxPPUSTATUS].decodedPPUSTATUS.spriteZeroHit(value); }
	void vblankOccurring(bool value) { m_registers[idxPPUSTATUS].decodedPPUSTATUS.vblankOccurring(value); }

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

	std::array<register_t, 8> m_registers;

	// PPUSCROLL
	size_t m_ppuScrollLatch : 1;
	std::array<uint8_t, 2> m_ppuScrollPosition;	// X,Y

	// PPUADDR
	size_t m_ppuAddressLatch : 1;
	std::array<uint8_t, 2> m_ppuAddress;	// high, low

	EightBit::Ram m_vram = 0x4000;
	EightBit::Ram m_oamram = 0x100;

	bool m_oamdmaActive = false;
	EightBit::register16_t m_oamdmaAddress = { { 0 } };
};
