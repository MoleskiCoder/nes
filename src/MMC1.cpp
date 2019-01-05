#include "stdafx.h"
#include "MMC1.h"

#include <cassert>
#include <functional>

#include "NesFile.h"

MMC1::MMC1(EightBit::Bus& bus, const NesFile& nesFile)
: m_bus(bus),
  m_shiftRegister(0) {

	const auto& content = nesFile.content();

	const auto sizePRGROM = nesFile.getSizePRGROM();
	PRG().resize(sizePRGROM);
	for (int i = 0; i != sizePRGROM; ++i)
		PRG()[i].load(content, 0, 16 + 0x4000 * i, 0x4000);

	const auto sizeCHRROM = nesFile.getSizeCHRROM();
	CHR().resize(sizeCHRROM);
	for (int i = 0; i != sizeCHRROM; ++i)
		CHR()[i].load(content, 0, 16 + 0x4000 * sizePRGROM + 0x2000 * i, 0x2000);

	resetRegisters();

	bus.WritingByte.connect(std::bind(&MMC1::Bus_WritingByte, this, std::placeholders::_1));
}

void MMC1::resetRegisters() {
	m_controls[0].decoded0.reset();
	m_controls[1].decoded1.reset();
	m_controls[2].decoded2.reset();
	m_controls[3].decoded3.reset();
}

EightBit::MemoryMapping MMC1::mapping(const uint16_t address) {

	if (address < 0x6000)
		return { m_unused6000, 0, 0xffff, EightBit::MemoryMapping::AccessLevel::ReadOnly };

	if (address < 0x8000)
		return { PRGRAM(), 0x6000, 0xffff, EightBit::MemoryMapping::AccessLevel::ReadWrite };

	const auto register0 = m_controls[0].decoded0;
	const auto prgRomBankMode = (register0_t::prg_rom_bank_mode_t)(register0.prgRomBankMode);
	const auto register3 = m_controls[3].decoded3;
	const auto prgRomBank = register3.prgRomBank;

	if (address < 0xC000) {
		const size_t lowBank = prgRomBankMode == register0_t::BankLowSixteenK ? prgRomBank : 0;
		return { PRG()[lowBank], 0x8000, 0xffff, EightBit::MemoryMapping::AccessLevel::ReadOnly };
	}

	const auto roms = PRG().size();
	const size_t highBank = prgRomBankMode == register0_t::BankHighSixteenK ? prgRomBank : roms - 1;
	return { PRG()[highBank], 0xc000, 0xffff, EightBit::MemoryMapping::AccessLevel::ReadOnly };
}

void MMC1::Bus_WritingByte(const EightBit::EventArgs& e) {

	const auto address = m_bus.ADDRESS().word;
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
			m_shiftRegister = 0;
		}
	}
}
