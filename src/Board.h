#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

#include <Ram.h>
#include <Bus.h>
#include <Register.h>

#include <Ricoh2A03.h>
#include <Disassembly.h>
#include <Symbols.h>

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

		ScanLinesVBlankLatency = 2,
		ScanLinesVBlank = 20,
		ScanLinesPerFrame = 1 + Display::RasterHeight + 1 + ScanLinesVBlankLatency + ScanLinesVBlank,

		FramesPerSecond = 60,
		CyclesPerFrame = CPUClockSpeed / FramesPerSecond,
		CyclesPerLine = CyclesPerFrame / ScanLinesPerFrame,
	};

	Board(const Configuration& configuration, const ColourPalette& colours);

	EightBit::Ricoh2A03& CPU() { return m_cpu; }
	EightBit::Ram& RAM() { return m_ram; }
	Display& PPU() { return m_ppu; }
	EightBit::Ram& IO() { return m_io; }
	Cartridge& cartridge() { return *m_cartridge; }

	void plug(const std::string& path);

	void raisePOWER() noexcept override;
	void lowerPOWER() noexcept override;

	int runScanLinesRaster();
	int runScanLinesVBlank();

	void initialise() final;
	EightBit::MemoryMapping mapping(uint16_t address) noexcept final;

private:
	EightBit::Ricoh2A03 m_cpu;

	EightBit::Ram m_ram = 0x800;	// 0000h-07FFh (mirrored to 800h-1FFFh)
	EightBit::Ram m_io = 0x20;

	Display m_ppu;

	std::unique_ptr<Cartridge> m_cartridge;

	EightBit::Symbols m_symbols;
	EightBit::Disassembly m_disassembler;

	int m_totalCPUCycles = 0;

	const Configuration& m_configuration;

	bool m_oamdmaActive = false;

	int run(int limit);
	int runScanLine();
	int runScanLines(int lines);
};
