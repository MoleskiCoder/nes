#pragma once

#include <string>
#include <vector>
#include <cstdint>

class NesFile {
public:
	NesFile() = default;

	void load(const std::string& path);

	std::vector<uint8_t>& PRGRAM() { return m_prgRam; }
	std::vector<std::vector<uint8_t>>& PRG() { return m_prg; }
	std::vector<std::vector<uint8_t>>& CHR() { return m_chr; }

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
		int mapperLower : 4;	
		bool fourScreenVramLayout : 1;
		bool trainerPatch : 1;
		bool batteryBackedSram : 1;
		mirroring_t mirroring : 1;

		// Flags 7
		int mapperUpper : 4;
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
	};

	union header_t {
		uint8_t raw[16];
		decoded_t decoded;
	};

	header_t m_header;

	std::vector<std::vector<uint8_t>> m_prg;
	std::vector<std::vector<uint8_t>> m_chr;
	std::vector<uint8_t> m_prgRam;
};
