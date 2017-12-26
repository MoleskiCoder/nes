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

class Board : public EightBit::Bus {
public:
	enum {
		CyclesPerSecond = 4 * 1024 * 1024,
		FramesPerSecond = 60,
		CyclesPerFrame = CyclesPerSecond / FramesPerSecond,
		TotalLineCount = 154,
		CyclesPerLine = CyclesPerFrame / TotalLineCount,
	};

	Board(const Configuration& configuration);

	EightBit::Ricoh2A03& CPU() { return m_cpu; }
	EightBit::Ram& RAM() { return m_ram; }

	void initialise();
	void reset();

protected:
	virtual uint8_t& reference(uint16_t address, bool& rom);

private:
	EightBit::Ricoh2A03 m_cpu;
	EightBit::Ram m_ram = 0x10000;

	EightBit::Symbols m_symbols;
	EightBit::Disassembly m_disassembler;

	const Configuration& m_configuration;

	void loadRom(const std::string& path);

	void Cpu_ExecutingInstruction_Debug(const EightBit::MOS6502& cpu);
};
