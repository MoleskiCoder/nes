#pragma once

#include <cstdint>

class Mapper {
public:
	virtual ~Mapper() = default;
	virtual uint8_t& reference(uint16_t address) = 0;

protected:
	uint8_t m_temporary = EightBit::Processor::Mask8;
};
