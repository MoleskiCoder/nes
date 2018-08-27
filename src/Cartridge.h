#pragma once

#include <string>
#include <memory>

#include <Bus.h>

#include "NesFile.h"
#include "Mapper.h"

class Cartridge final {
public:
	Cartridge(EightBit::Bus& bus, const std::string& path);

	uint8_t& reference(uint16_t address);

private:
	EightBit::Bus& m_bus;
	NesFile m_container;
	std::unique_ptr<Mapper> m_mapper;
};

