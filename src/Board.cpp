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
	CPU().ExecutedInstruction.connect(std::bind(&Board::Cpu_ExecutedInstruction, this, std::placeholders::_1));
}

void Board::initialise() {
	const auto romDirectory = m_configuration.getRomDirectory();

	// Mapper 0

	//loadRom(romDirectory + "/nestest.nes");

	loadRom(romDirectory + "/instr_test-v5/rom_singles/01-basics.nes");		// pass
	//loadRom(romDirectory + "/instr_test-v5/rom_singles/02-implied.nes");		// pass
	//loadRom(romDirectory + "/instr_test-v5/rom_singles/03-immediate.nes");	// illegal instruction failures
	//loadRom(romDirectory + "/instr_test-v5/rom_singles/04-zero_page.nes");	// pass
	//loadRom(romDirectory + "/instr_test-v5/rom_singles/05-zp_xy.nes");		// pass
	//loadRom(romDirectory + "/instr_test-v5/rom_singles/06-absolute.nes");		// pass
	//loadRom(romDirectory + "/instr_test-v5/rom_singles/07-abs_xy.nes");		// unimplemented exception
	//loadRom(romDirectory + "/instr_test-v5/rom_singles/08-ind_x.nes");		// pass
	//loadRom(romDirectory + "/instr_test-v5/rom_singles/09-ind_y.nes");		// pass
	//loadRom(romDirectory + "/instr_test-v5/rom_singles/10-branches.nes");		// pass
	//loadRom(romDirectory + "/instr_test-v5/rom_singles/11-stack.nes");		// pass
	//loadRom(romDirectory + "/instr_test-v5/rom_singles/12-jmp_jsr.nes");		// pass
	//loadRom(romDirectory + "/instr_test-v5/rom_singles/13-rts.nes");			// pass
	//loadRom(romDirectory + "/instr_test-v5/rom_singles/14-rti.nes");			// pass
	//loadRom(romDirectory + "/instr_test-v5/rom_singles/15-brk.nes");			// pass
	//loadRom(romDirectory + "/instr_test-v5/rom_singles/16-special.nes");		// pass

	// Mapper 1?

	//loadRom(romDirectory + "/instr_test-v5/official_only.nes");
	//loadRom(romDirectory + "/instr_test-v5/all_instrs.nes");

	reset();
}

void Board::reset() {
	EightBit::Ricoh2A03::lower(CPU().RESET());
}

// Mapper 0 (AKA NROM)
uint8_t& Board::reference(uint16_t address, bool& rom) {

	rom = false;
	if (address < 0x6000)
		return RAM().reference(address);
	if (address < 0x8000)
		return cartridge().PRGRAM()[address - 0x6000];

	rom = true;
	if (address < 0xC000)
		return cartridge().PRG()[0][address - 0x8000];

	const auto roms = cartridge().PRG().size();
	return cartridge().PRG()[roms - 1][address - 0xc000];
}

void Board::loadRom(const std::string& path) {
	m_cartridge.load(path);
}

void Board::Cpu_ExecutingInstruction_Debug(const EightBit::MOS6502& cpu) {

	auto address = CPU().PC().word;
	auto cell = peek(address);

	std::cout << m_disassembler.dump_WordValue(address);
	std::cout << "  ";
	std::cout << m_disassembler.disassemble(address);

	std::cout << "\t\t";

	std::cout << "A:" << m_disassembler.dump_ByteValue(CPU().A()) << " ";
	std::cout << "X:" << m_disassembler.dump_ByteValue(CPU().X()) << " ";
	std::cout << "Y:" << m_disassembler.dump_ByteValue(CPU().Y()) << " ";
	std::cout << "P:" << m_disassembler.dump_ByteValue(CPU().P()) << " ";
	std::cout << "SP:" << m_disassembler.dump_ByteValue(CPU().S()) << " ";

	std::cout << "CYC:" << std::setw(3) << std::setfill(' ') << (m_totalCPUCycles * PPUCyclesPerCPUCycle) % PPUCyclesPerScanLine;

	std::cout << std::endl;
}

void Board::Cpu_ExecutedInstruction(const EightBit::MOS6502& cpu) {
	m_totalCPUCycles += CPU().clockCycles();
}
