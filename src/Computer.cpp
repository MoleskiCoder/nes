#include "stdafx.h"
#include "Computer.h"

#include <iostream>

Computer::Computer(const Configuration& configuration)
: m_configuration(configuration),
  m_board(configuration) {
}

void Computer::initialise() {
	m_board.initialise();
	m_board.WrittenByte.connect(std::bind(&Computer::Board_WrittenByte, this, std::placeholders::_1));
}

void Computer::runLoop() {

	auto& cpu = m_board.CPU();

	cpu.powerOn();

	// Nestest...
	//cpu.PC().word = 0xC000;	// Hack!!
	//cpu.X() = 0x00;			// Hack!!
	//cpu.P() |= 0x04;		// Hack!!
	//cpu.S() = 0xFD;			// Hack!!

	auto cycles = 0;
	while (cpu.powered()) {
		cycles += cpu.step();
	}
}

void Computer::Board_WrittenByte(const uint16_t address) {
	if (address >= 0x6004) {
		const auto contents = m_board.peek(address);
		std::cout << (char)contents;
	}
}