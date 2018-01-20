#include "stdafx.h"
#include "Computer.h"

Computer::Computer(const Configuration& configuration)
: m_configuration(configuration),
  m_board(configuration) {
}

void Computer::initialise() {
	m_board.initialise();
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