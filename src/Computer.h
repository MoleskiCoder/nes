#pragma once

#include "Board.h"

class Configuration;

class Computer {
public:
	Computer(const Configuration& configuration);

	void plug(const std::string& path);
	void run();

private:
	void Board_WrittenByte(uint16_t address);

	enum {
		DisplayScale = 2,
		RasterWidth = 256,
		RasterHeight = 224,
		ScreenWidth = RasterWidth * DisplayScale,
		ScreenHeight = RasterHeight * DisplayScale,
	};

	const Configuration& m_configuration;
	mutable Board m_board;
};
