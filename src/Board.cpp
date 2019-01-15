#include "stdafx.h"
#include "Board.h"
#include "NesFile.h"

#include <functional>
#include <iostream>
#include <iomanip>

Board::Board(const Configuration& configuration, const ColourPalette& colours)
: m_cpu(*this),
  m_ppu(*this, colours),
  m_symbols(""),
  m_disassembler(*this, m_cpu, m_symbols),
  m_configuration(configuration) {
	if (m_configuration.isDebugMode())
		CPU().ExecutingInstruction.connect(std::bind(&Board::Cpu_ExecutingInstruction_Debug, this, std::placeholders::_1));
	CPU().ExecutedInstruction.connect(std::bind(&Board::Cpu_ExecutedInstruction, this, std::placeholders::_1));
	WrittenByte.connect(std::bind(&Board::Bus_WrittenByte, this, std::placeholders::_1));
}

void Board::plug(const std::string& path) {
	m_cartridge.reset(new Cartridge(*this, path));
}

void Board::raisePOWER() {
	PPU().initialise();
	CPU().raisePOWER();
	CPU().lowerRESET();
	CPU().raiseINT();
	CPU().raiseNMI();
	CPU().raiseSO();
	CPU().raiseRDY();
}

void Board::lowerPOWER() {
	CPU().lowerPOWER();
}

void Board::initialise() {}

EightBit::MemoryMapping Board::mapping(const uint16_t address) {

	if (address < Display::PPU_START)
		return { RAM(), 0x0000, 0x7ff, EightBit::MemoryMapping::AccessLevel::ReadWrite };

	if (address < Display::PPU_END)
		return PPU().mapping(address);

	if (address < 0x4020)
		return { IO(), Display::PPU_END, 0xffff, EightBit::MemoryMapping::AccessLevel::ReadWrite };

	return cartridge().mapping(address);
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

	const bool reset = EightBit::Chip::lowered(CPU().RESET());
	const bool irq = EightBit::Chip::lowered(CPU().INT());
	const bool nmi = EightBit::Chip::lowered(CPU().NMI());

	std::cout << "\t";
	if (reset)
		std::cout << "RESET ";
	if (irq)
		std::cout << "IRQ ";
	if (nmi)
		std::cout << "NMI ";

	std::cout << std::endl;
}

void Board::Cpu_ExecutedInstruction(const EightBit::MOS6502& cpu) {
	m_totalCPUCycles += CPU().cycles();
}

int Board::run(const int limit) {
	int current = 0;
	while (LIKELY(CPU().powered()) && current < limit) {
		if (UNLIKELY(PPU().stepOAMDMA())) {
			current += 2;
		} else {
			current += CPU().step();
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
	for (int line = 0; line < lines; ++line) {
		count += runScanLine();
		PPU().render();
	}
	return count;
}

int Board::runScanLinesRaster() {
	auto returned = 0;
	returned += runScanLines(1);
	PPU().startRender();
	returned += runScanLines(Display::RasterHeight);
	PPU().finishRender();
	returned += runScanLines(1);
	return returned;
}

int Board::runScanLinesVBlank() {
	auto returned = 0;
	PPU().setVBlank();
	if (PPU().nmi())
		EightBit::Processor::lower(CPU().NMI());
	returned += runScanLines(ScanLinesVBlank);
	PPU().clearVBlank();
	returned += runScanLines(ScanLinesVBlankLatency);
	return returned;
}

void Board::Bus_WrittenByte(const EightBit::EventArgs& e) {
	if (UNLIKELY(ADDRESS().word == 0x4014)) {	// OAMDMA
		PPU().triggerOAMDMA(DATA());
		m_oamdmaActive = true;
	}
}
