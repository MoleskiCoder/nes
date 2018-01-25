#pragma once

#include <cstdint>

class Mapper {
public:
	virtual ~Mapper() = default;
	virtual uint8_t& reference(uint16_t address, bool& rom) = 0;
};
