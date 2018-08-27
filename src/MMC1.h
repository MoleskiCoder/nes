#pragma once

#include <cstdint>
#include <vector>
#include <array>

#include <Bus.h>

#include "Mapper.h"
#include "NesFile.h"

// AKA Mapper 1
class MMC1 final : public Mapper {
public:
	MMC1(EightBit::Bus& bus, const NesFile& nesFile);

	virtual uint8_t& reference(uint16_t address) final;

private:
	void Bus_WrittenByte(const EightBit::EventArgs& e);

	void resetRegisters();

	std::vector<uint8_t>& PRGRAM() { return m_prgRam; }
	std::vector<std::vector<uint8_t>>& PRG() { return m_prg; }
	std::vector<std::vector<uint8_t>>& CHR() { return m_chr; }

	std::vector<uint8_t> m_prgRam;
	std::vector<std::vector<uint8_t>> m_prg;
	std::vector<std::vector<uint8_t>> m_chr;

	EightBit::Bus& m_bus;

	// AKA Control
	struct register0_t {

		enum mirror_t {
			OneScreenLowerBank,
			OneScreenUpperBank,
			Vertical,
			Horizontal
		};
		uint8_t mirroring : 2;

		enum prg_rom_bank_mode_t {
			ThirtyTwoK_0,
			ThirtyTwoK_1,
			BankHighSixteenK,
			BankLowSixteenK,
		};
		uint8_t prgRomBankMode : 2;

		enum chr_rom_bank_mode_t {
			SingleBankEightK,
			TwoBanksFourK
		};
		uint8_t chrRomBankMode : 1;

		void reset() {
			mirroring = OneScreenLowerBank;
			prgRomBankMode = BankLowSixteenK;
			chrRomBankMode = SingleBankEightK;
		}
	};

	// CHR bank 0
	struct register1_t {
		// 4K or 8K CHR bank at PPU $0000 (low bit ignored in 8K mode)
		uint8_t chrRomBank : 5;

		void reset() {
			chrRomBank = 0;
		}
	};

	// CHR bank 1
	struct register2_t {
		// 4K CHR bank at PPU $1000 (ignored in 8K mode)
		uint8_t chrRomBank : 5;

		void reset() {
			chrRomBank = 0;
		}
	};

	// PRG bank
	struct register3_t {
		// Select 16K PRG ROM bank (low bit ignored in 32K mode)
		uint8_t prgRomBank : 4;

		// 0: Enabled, 1: Disabled
		uint8_t prgRamEnable : 1;

		void reset() {
			prgRomBank = 0;
			prgRamEnable = 0;
		}
	};

	union register_t {
		uint8_t raw;
		register0_t decoded0;
		register1_t decoded1;
		register2_t decoded2;
		register3_t decoded3;
	};

	int m_shiftCounter = 0;
	uint8_t m_shiftRegister : 5;

	std::array<register_t, 4> m_controls;

	size_t m_lowPRGBank;
	size_t m_highPRGBank;
};
