#include "stdafx.h"
#include "Board.h"
#include "NesFile.h"

#include <functional>
#include <iostream>
#include <iomanip>

Board::Board(const Configuration& configuration)
: m_cpu(*this),
  m_ppu(*this),
  m_symbols(""),
  m_disassembler(m_cpu, m_symbols),
  m_configuration(configuration) {
	if (m_configuration.isDebugMode())
		CPU().ExecutingInstruction.connect(std::bind(&Board::Cpu_ExecutingInstruction_Debug, this, std::placeholders::_1));
	CPU().ExecutedInstruction.connect(std::bind(&Board::Cpu_ExecutedInstruction, this, std::placeholders::_1));
	WrittenByte.connect(std::bind(&Board::Bus_WrittenByte, this, std::placeholders::_1));
}

void Board::plug(const std::string& path) {
	m_cartridge.reset(new Cartridge(*this, path));
}

void Board::reset() {
	EightBit::Ricoh2A03::lower(CPU().RESET());
}

uint8_t& Board::reference(uint16_t address, bool& rom) {

	rom = false;

	if (address < Display::PPU_START)
		return RAM().reference(address & 0x7ff);

	if (address < Display::PPU_END)
		return PPU().reference(address, rom);

	if (address < 0x4020)
		return IO().reference(address - Display::PPU_END);

	return cartridge().reference(address, rom);
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

int Board::run(const int limit) {
	int current = 0;
	while (LIKELY(CPU().powered()) && current < limit) {
		if (UNLIKELY(PPU().stepOAMDMA())) {
			current += 2;
		} else {
			current += CPU().singleStep();
			if (UNLIKELY(m_oamdmaActive))
				++current;
		}
	}
	return current;
}

int Board::runScanLine() {
	return run(CyclesPerLine);
}

int Board::runScanLines(const int lines) {
	int count = 0;
	for (int line = 0; line < lines; ++line)
		count += runScanLine();
	return count;
}

int Board::runScanLinesTopBorder() {
	return runScanLines(ScanLinesTopBorder);
}

int Board::runScanLinesBottomBorder() {
	return runScanLines(ScanLinesBottomBorder);
}

int Board::runScanLinesRaster() {
	return runScanLines(RasterHeight);
}

int Board::runScanLinesVBlankLatency() {
	return runScanLines(ScanLinesVBlankLatency);
}

int Board::runScanLinesVBlank() {
	PPU().setVBlank();
	if (PPU().nmi())
		EightBit::Processor::lower(CPU().NMI());
	const auto returned = runScanLines(ScanLinesVBlank);
	PPU().clearVBlank();
	return returned;
}

void Board::Bus_WrittenByte(const uint16_t address) {
	if (UNLIKELY(address == 0x4014)) {	// OAMDMA
		PPU().triggerOAMDMA(DATA());
		m_oamdmaActive = true;
	}
}
