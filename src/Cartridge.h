#pragma once

#include <string>
#include <memory>

#include "NesFile.h"
#include "Mapper.h"

class Cartridge final {
public:
	Cartridge(const std::string& path);

	uint8_t& reference(uint16_t address, bool& rom);

private:
	NesFile m_container;
	std::unique_ptr<Mapper> m_mapper;
};

