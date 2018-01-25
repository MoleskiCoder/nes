#pragma once

#include <string>
#include <vector>
#include <cstdint>

class NesFile {
public:
	NesFile() = default;

	void load(const std::string& path);
	void dumpInformation() const;

	uint8_t getMapper() const { return m_header.decoded.buildMapperType(); }
	uint8_t getSizePRGROM() const { return m_header.decoded.prgRomSize; }
	uint8_t getSizeCHRROM() const { return m_header.decoded.chrRomSize; }

	const std::vector<uint8_t>& content() const { return m_content; }

private:
	enum mirroring_t {
		Horizontal = 0,
		Vertical = 1
	};

	enum tv_system_t {
		NTSC = 0,
		PAL = 1
	};

	struct decoded_t {

		uint8_t identifier[4];		// Constant $4E $45 $53 $1A ("NES" followed by MS-DOS end-of-file)

		uint8_t prgRomSize;			// Size of PRG ROM in 16 KB units
		uint8_t chrRomSize;			// Size of CHR ROM in 8 KB units (Value 0 means the board uses CHR RAM)

		// Flags 6
		uint8_t mapperLower : 4;	
		bool fourScreenVramLayout : 1;
		bool trainerPatch : 1;
		bool batteryBackedSram : 1;
		mirroring_t mirroring : 1;

		// Flags 7
		uint8_t mapperUpper : 4;
		int reserved7 : 2;
		bool pc10 : 1;
		bool vsUnisystem : 1;

		uint8_t prgRamSize;			// Size of PRG RAM in 8 KB units (Value 0 infers 8 KB for compatibility; see PRG RAM circuit)

		// Flags 9
		int reserved9 : 7;			// Reserved, set to zero
		tv_system_t tv : 1;			// TV system(0: NTSC; 1: PAL)

		// Flags 10
		uint8_t flag10;

		uint8_t zeroed[5];			// Zero filled

		uint8_t buildMapperType() const {
			return mapperLower | (mapperUpper << 4);
		}

		int buildPrgRamSize() const {
			return prgRamSize == 0 ? 1 : prgRamSize;
		}
	};

	union header_t {
		uint8_t raw[16];
		decoded_t decoded;
	};

	header_t m_header;
	std::vector<uint8_t> m_content;
	std::string m_path;
};
