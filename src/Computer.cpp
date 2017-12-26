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

	cpu.PC().word = 0xC000;	// Hack!!

	auto cycles = 0;
	while (cpu.powered()) {
		cycles += cpu.step();
	}
}