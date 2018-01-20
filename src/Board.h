#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include <Rom.h>
#include <Ram.h>
#include <Bus.h>
#include <Register.h>

#include <Ricoh2A03.h>
#include <Disassembly.h>

#include "Configuration.h"
#include "NesFile.h"

class Board : public EightBit::Bus {
public:
	enum {
		MasterClockSpeed = 21477272,
		CPUClockSpeed = 1789773,		// ~ MasterClockSpeed / 12
		PPUClockSpeed = 5369319,		// ~ CPUClockSpeed * 3

		PPUCyclesPerCPUCycle = 3,
		PPUCyclesPerScanLine = 341,
	};

	Board(const Configuration& configuration);

	EightBit::Ricoh2A03& CPU() { return m_cpu; }
	EightBit::Ram& RAM() { return m_ram; }
	NesFile& cartridge() { return m_cartridge; }

	void initialise();
	void reset();

protected:
	virtual uint8_t& reference(uint16_t address, bool& rom);

private:
	EightBit::Ricoh2A03 m_cpu;
	EightBit::Ram m_ram = 0x6000;
	NesFile m_cartridge;

	EightBit::Symbols m_symbols;
	EightBit::Disassembly m_disassembler;

	int m_totalCPUCycles = 0;

	const Configuration& m_configuration;

	void loadRom(const std::string& path);

	void Cpu_ExecutingInstruction_Debug(const EightBit::MOS6502& cpu);
	void Cpu_ExecutedInstruction(const EightBit::MOS6502& cpu);
};
