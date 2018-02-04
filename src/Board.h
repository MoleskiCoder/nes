#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

#include <Rom.h>
#include <Ram.h>
#include <Bus.h>
#include <Register.h>

#include <Ricoh2A03.h>
#include <Disassembly.h>

#include "Configuration.h"
#include "Cartridge.h"
#include "Display.h"

class Board : public EightBit::Bus {
public:
	enum {
		MasterClockSpeed = 21477272,
		CPUClockSpeed = 1789773,		// ~ MasterClockSpeed / 12
		PPUClockSpeed = 5369319,		// ~ CPUClockSpeed * 3

		PPUCyclesPerCPUCycle = 3,
		PPUCyclesPerScanLine = 341,

		RasterWidth = 256,
		RasterHeight = 224,

		ScanLinesVBlankLatency = 2,
		ScanLinesVBlank = 20,
		ScanLinesTopBorder = 8,
		ScanLinesBottomBorder = ScanLinesTopBorder,
		ScanLinesPerFrame = ScanLinesTopBorder + RasterHeight + ScanLinesBottomBorder + ScanLinesVBlankLatency + ScanLinesVBlank,

		FramesPerSecond = 60,
		CyclesPerFrame = CPUClockSpeed / FramesPerSecond,
		CyclesPerLine = CyclesPerFrame / RasterHeight,
	};

	Board(const Configuration& configuration);

	EightBit::Ricoh2A03& CPU() { return m_cpu; }
	EightBit::Ram& RAM() { return m_ram; }
	Display& PPU() { return m_ppu; }
	EightBit::Ram& APU() { return m_apu; }
	Cartridge& cartridge() { return *m_cartridge; }

	void plug(const std::string& path);
	void reset();

	int run(int limit);
	int runScanLine();
	int runScanLines(int lines);

	int runScanLinesTopBorder();
	int runScanLinesRaster();
	int runScanLinesBottomBorder();
	int runScanLinesVBlankLatency();
	int runScanLinesVBlank();

protected:
	virtual uint8_t& reference(uint16_t address, bool& rom);

private:
	EightBit::Ricoh2A03 m_cpu;

	EightBit::Ram m_ram = 0x800;	// 0000h-07FFh (mirrored to 800h-1FFFh)
	Display m_ppu;
	EightBit::Ram m_apu = 0x16;
	EightBit::Ram m_joysticks = 2;

	std::unique_ptr<Cartridge> m_cartridge;

	EightBit::Symbols m_symbols;
	EightBit::Disassembly m_disassembler;

	int m_totalCPUCycles = 0;

	const Configuration& m_configuration;

	void Cpu_ExecutingInstruction_Debug(const EightBit::MOS6502& cpu);
	void Cpu_ExecutedInstruction(const EightBit::MOS6502& cpu);
};
