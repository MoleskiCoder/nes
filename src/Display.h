#pragma once

#include <cstdint>
#include <array>
#include <Bus.h>
#include "Mapper.h"

class Display final : public Mapper {
public:
	enum {
		PPU_START = 0x2000,
		PPU_FINISH = 0x3fff,
		PPUCTRL = 0,
		PPUMASK,
		PPUSTATUS,
		OAMADDR,
		OAMDATA,
		PPUSCROLL,
		PPUADDR,
		PPUDATA,
		OAMDMA = 14
	};

	Display(EightBit::Bus& bus);

	virtual uint8_t& reference(uint16_t address, bool& rom) final;

	void setVBlank() { vblankOccurring(true); }
	void clearVBlank() { vblankOccurring(false); }

private:
	static bool convertAddress(uint16_t address, size_t& index, bool& writable, bool& readable);

	void Bus_WritingByte(uint16_t address);
	void Bus_WrittenByte(uint16_t address);
	void Bus_ReadingByte(uint16_t address);
	void Bus_ReadByte(uint16_t address);

	EightBit::Bus& BUS() { return m_bus; }

	EightBit::Bus& m_bus;

	// Control register 1 wrappers:
	uint16_t nameTableAddress() const { return m_registers[PPUCTRL].decodedPPUCTRL.nameTableAddress(); }
	int addressIncrement() const { return m_registers[PPUCTRL].decodedPPUCTRL.addressIncrement(); }
	uint16_t spritePatternTableAddress() const { return m_registers[PPUCTRL].decodedPPUCTRL.spritePatternTableAddress(); }
	uint16_t backgroundPatternTableAddress() const { return m_registers[PPUCTRL].decodedPPUCTRL.backgroundPatternTableAddress(); }
	int spriteHeight() const { return m_registers[PPUCTRL].decodedPPUCTRL.spriteHeight(); }
	bool nmi() const { return m_registers[PPUCTRL].decodedPPUCTRL.nmi(); }

	// Control register 2 wrappers:
	bool colour() const { return m_registers[PPUMASK].decodedPPUMASK.colour(); }
	bool clipBackground() const { return m_registers[PPUMASK].decodedPPUMASK.clipBackground(); }
	bool clipSprites() const { return m_registers[PPUMASK].decodedPPUMASK.clipSprites(); }
	bool hideBackground() const { return m_registers[PPUMASK].decodedPPUMASK.hideBackground(); }
	bool hideSprites() const { return m_registers[PPUMASK].decodedPPUMASK.hideSprites(); }
	int backgroundIntensity() const { return m_registers[PPUMASK].decodedPPUMASK.backgroundIntensity(); }

	// Status register wrappers
	void ignoreVramWrites(bool value) { m_registers[PPUSTATUS].decodedPPUSTATUS.ignoreVramWrites(value); }
	void excessiveScanlineSprites(bool value) { m_registers[PPUSTATUS].decodedPPUSTATUS.excessiveScanlineSprites(value); }
	void spriteZeroHit(bool value) { m_registers[PPUSTATUS].decodedPPUSTATUS.spriteZeroHit(value); }
	void vblankOccurring(bool value) { m_registers[PPUSTATUS].decodedPPUSTATUS.vblankOccurring(value); }

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
		uint8_t _hideBackground : 1;
		uint8_t _hideSprites : 1;
		uint8_t _backgroundIntensity : 3;

		bool colour() const { return !!_colour; }
		bool clipBackground() const { return !!_clipBackground; }
		bool clipSprites() const { return !!_clipSprites; }
		bool hideBackground() const { return !!_hideBackground; }
		bool hideSprites() const { return !!_hideSprites; }
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

	// OAMADDR
	struct spr_ram_address_register_t {
		uint8_t address;
	};

	// OAMDATA
	struct spr_ram_io_register_t {
		uint8_t value;
	};

	// PPUSCROLL
	struct vram_address_register_1_t {
		uint8_t address;
	};

	// PPUADDR
	struct vram_address_register_2_t {
		uint8_t address;
	};

	// PPUDATA
	struct vram_io_register_t {
		uint8_t value;
	};

	// OAMDMA
	struct spr_dma_register_t {
		uint8_t value;
	};

	union register_t {
		uint8_t raw;
		control_register_1_t decodedPPUCTRL;
		control_register_2_t decodedPPUMASK;
		status_register_t decodedPPUSTATUS;
		spr_ram_address_register_t decodedOAMADDR;
		spr_ram_io_register_t decodedOAMDATA;
		vram_address_register_1_t decodedPPUSCROLL;
		vram_address_register_2_t decodedPPUADDR;
		vram_io_register_t decodedPPUDATA;
		spr_dma_register_t decodedOAMDMA;
	};

	std::array<register_t, 9> m_registers;

	// PPUSCROLL
	size_t m_ppuScrollLatch : 1;
	std::array<uint8_t, 2> m_ppuScrollPosition;	// X,Y

	// PPUADDR
	size_t m_ppuAddressLatch : 1;
	std::array<uint8_t, 2> m_ppuAddress;
};
