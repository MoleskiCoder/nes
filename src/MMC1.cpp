#include "stdafx.h"
#include "MMC1.h"

#include <cassert>
#include <functional>

MMC1::MMC1(EightBit::Bus& bus, const NesFile& nesFile)
: m_bus(bus),
  m_shiftRegister(0) {

	const auto& content = nesFile.content();

	const auto sizePRGROM = nesFile.getSizePRGROM();
	PRG().resize(sizePRGROM);
	for (size_t i = 0; i != sizePRGROM; ++i) {
		auto& rom = PRG()[i];
		rom.resize(0x4000);
		std::copy_n(
			content.cbegin() + 16 + 0x4000 * i,
			0x4000,
			rom.begin());
	}

	const auto sizeCHRROM = nesFile.getSizeCHRROM();
	CHR().resize(sizeCHRROM);
	for (size_t i = 0; i != sizeCHRROM; ++i) {
		auto& rom = CHR()[i];
		rom.resize(0x2000);
		std::copy_n(
			content.cbegin() + 16 + 0x4000 * sizePRGROM + 0x2000 * i,
			0x2000,
			rom.begin());
	}

	PRGRAM().resize(0x2000);

	resetRegisters();

	bus.WrittenByte.connect(std::bind(&MMC1::Bus_WrittenByte, this, std::placeholders::_1));
}

void MMC1::resetRegisters() {
	m_controls[0].decoded0.reset();
	m_controls[1].decoded1.reset();
	m_controls[2].decoded2.reset();
	m_controls[3].decoded3.reset();
}

uint8_t& MMC1::reference(uint16_t address, bool& rom) {

	rom = false;
	if (address < 0x8000)
		return PRGRAM()[address - 0x6000];

	const auto register0 = m_controls[0].decoded0;
	const auto prgRomBankMode = (register0_t::prg_rom_bank_mode_t)(register0.prgRomBankMode);
	const auto register3 = m_controls[3].decoded3;
	const auto prgRomBank = register3.prgRomBank;

	rom = true;
	if (address < 0xC000) {
		const size_t lowBank = prgRomBankMode == register0_t::BankLowSixteenK ? prgRomBank : 0;
		return PRG()[lowBank][address - 0x8000];
	}

	const auto roms = PRG().size();
	const size_t highBank = prgRomBankMode == register0_t::BankHighSixteenK ? prgRomBank : roms - 1;
	return PRG()[highBank][address - 0xc000];
}

void MMC1::Bus_WrittenByte(const uint16_t address) {

	if (LIKELY(address < 0x8000))
		return;

	const auto contents = m_bus.DATA();
	const auto reset = !!(contents & EightBit::Processor::Bit7);

	m_shiftCounter = ++m_shiftCounter % 5;
	assert(m_shiftCounter < 5);

	if (UNLIKELY(reset)) {

		m_shiftCounter = 0;

	} else {

		const auto addressMSB = m_bus.ADDRESS().high;
		const auto mapperRegister = (addressMSB & 0x60) >> 5;

		const uint8_t data = contents & EightBit::Processor::Bit0;
		m_shiftRegister = (m_shiftRegister >> 1) | (data << 4);
		assert(m_shiftRegister < 32);

		if (m_shiftCounter == 0) {

			m_controls[mapperRegister].raw = m_shiftRegister;
			//registerUpdated(mapperRegister);

			m_shiftRegister = 0;
		}
	}
}

//void MMC1::registerUpdated(const size_t index) {
//
//	std::cout << std::endl;
//	std::cout << "** Register " << index << ", raw: " << (int)(m_controls[index].raw) << std::endl;
//
//	switch (index) {
//
//	case 0b00: {
//			const register0_t& control = m_controls[index].decoded0;
//			std::cout << "** Mirroring: " << (int)(control.mirroring) << std::endl;
//			std::cout << "** PRG-ROM bank mode: " << (int)(control.prgRomBankMode) << std::endl;
//			std::cout << "** CHR-ROM bank mode: " << (int)(control.chrRomBankMode) << std::endl;
//		}
//		break;
//
//	case 0b01: {
//			const register1_t& control = m_controls[index].decoded1;
//			std::cout << "** CHR-ROM bank 0: " << (int)(control.chrRomBank) << std::endl;
//		}
//		break;
//
//	case 0b10: {
//			const register2_t& control = m_controls[index].decoded2;
//			std::cout << "** CHR-ROM bank 1: " << (int)(control.chrRomBank) << std::endl;
//		}
//		break;
//
//	case 0b11: {
//			const register3_t& control = m_controls[index].decoded3;
//			std::cout << "\n** PRG-ROM bank: " << (int)(control.prgRomBank) << std::endl;
//			std::cout << "\n** PRG-RAM enable mode: " << (int)(control.prgRamEnable) << std::endl;
//		}
//		break;
//
//	default:
//		UNREACHABLE;
//	}
//}