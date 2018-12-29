#pragma once

#include <string>
#include <memory>

#include <Bus.h>
#include <Mapper.h>

#include "NesFile.h"

class Cartridge final : public EightBit::Mapper {
public:
	Cartridge(EightBit::Bus& bus, const std::string& path);

	virtual EightBit::MemoryMapping mapping(uint16_t address) final;

private:
	EightBit::Bus& m_bus;
	NesFile m_container;
	std::unique_ptr<EightBit::Mapper> m_mapper;
};

