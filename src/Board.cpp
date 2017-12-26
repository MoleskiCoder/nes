#include "stdafx.h"
#include "Board.h"
#include "NesFile.h"

#include <functional>
#include <iostream>
#include <iomanip>

Board::Board(const Configuration& configuration)
: m_cpu(*this),
  m_symbols(""),
  m_disassembler(m_cpu, m_symbols),
  m_configuration(configuration) {
	if (m_configuration.isDebugMode())
		CPU().ExecutingInstruction.connect(std::bind(&Board::Cpu_ExecutingInstruction_Debug, this, std::placeholders::_1));
}

void Board::initialise() {
	const auto romDirectory = m_configuration.getRomDirectory();
	loadRom(romDirectory + "/nestest.nes");
	reset();
}

void Board::reset() {
	EightBit::Ricoh2A03::lower(CPU().RESET());
}

uint8_t& Board::reference(uint16_t address, bool& rom) {
	rom = false;
	return RAM().reference(address);
}

void Board::loadRom(const std::string& path) {
	NesFile cartridge;
	cartridge.load(path);
	RAM().load(cartridge.PRG()[0], 0xC000);
}

void Board::Cpu_ExecutingInstruction_Debug(const EightBit::MOS6502& cpu) {

	auto address = CPU().PC().word;
	auto cell = peek(address);

	std::cout << std::hex;
	std::cout << "PC=" << std::setw(4) << std::setfill('0') << address << ":";
	std::cout << "P=" << m_disassembler.dump_Flags(CPU().P()) << ", ";
	std::cout << std::setw(2) << std::setfill('0');
	std::cout << "A=" << (int)CPU().A() << ", ";
	std::cout << "X=" << (int)CPU().X() << ", ";
	std::cout << "Y=" << (int)CPU().Y() << ", ";
	std::cout << "S=" << (int)CPU().S() << "\t";

	std::cout << m_disassembler.disassemble(address);

	std::cout << "\n";
}
