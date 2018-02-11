#include "stdafx.h"
#include "ColourPalette.h"

#include <SDL.h>

uint32_t ColourPalette::getColour(const size_t index) const {
	return m_colours[index];
}

void ColourPalette::load(SDL_PixelFormat* hardware) {
	m_colours[0] = ::SDL_MapRGBA(hardware, 0x7C, 0x7C, 0x7C, SDL_ALPHA_OPAQUE);
	m_colours[1] = ::SDL_MapRGBA(hardware, 0x00, 0x00, 0xFC, SDL_ALPHA_OPAQUE);
	m_colours[2] = ::SDL_MapRGBA(hardware, 0x00, 0x00, 0xBC, SDL_ALPHA_OPAQUE);
	m_colours[3] = ::SDL_MapRGBA(hardware, 0x44, 0x28, 0xBC, SDL_ALPHA_OPAQUE);
	m_colours[4] = ::SDL_MapRGBA(hardware, 0x94, 0x00, 0x84, SDL_ALPHA_OPAQUE);
	m_colours[5] = ::SDL_MapRGBA(hardware, 0xA8, 0x00, 0x20, SDL_ALPHA_OPAQUE);
	m_colours[6] = ::SDL_MapRGBA(hardware, 0xA8, 0x10, 0x00, SDL_ALPHA_OPAQUE);
	m_colours[7] = ::SDL_MapRGBA(hardware, 0x88, 0x14, 0x00, SDL_ALPHA_OPAQUE);
	m_colours[8] = ::SDL_MapRGBA(hardware, 0x50, 0x30, 0x00, SDL_ALPHA_OPAQUE);
	m_colours[9] = ::SDL_MapRGBA(hardware, 0x00, 0x78, 0x00, SDL_ALPHA_OPAQUE);
	m_colours[10] = ::SDL_MapRGBA(hardware, 0x00, 0x68, 0x00, SDL_ALPHA_OPAQUE);
	m_colours[11] = ::SDL_MapRGBA(hardware, 0x00, 0x58, 0x00, SDL_ALPHA_OPAQUE);
	m_colours[12] = ::SDL_MapRGBA(hardware, 0x00, 0x40, 0x58, SDL_ALPHA_OPAQUE);
	m_colours[13] = ::SDL_MapRGBA(hardware, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	m_colours[14] = ::SDL_MapRGBA(hardware, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	m_colours[15] = ::SDL_MapRGBA(hardware, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	m_colours[16] = ::SDL_MapRGBA(hardware, 0xBC, 0xBC, 0xBC, SDL_ALPHA_OPAQUE);
	m_colours[17] = ::SDL_MapRGBA(hardware, 0x00, 0x78, 0xF8, SDL_ALPHA_OPAQUE);
	m_colours[18] = ::SDL_MapRGBA(hardware, 0x00, 0x58, 0xF8, SDL_ALPHA_OPAQUE);
	m_colours[19] = ::SDL_MapRGBA(hardware, 0x68, 0x44, 0xFC, SDL_ALPHA_OPAQUE);
	m_colours[20] = ::SDL_MapRGBA(hardware, 0xD8, 0x00, 0xCC, SDL_ALPHA_OPAQUE);
	m_colours[21] = ::SDL_MapRGBA(hardware, 0xE4, 0x00, 0x58, SDL_ALPHA_OPAQUE);
	m_colours[22] = ::SDL_MapRGBA(hardware, 0xF8, 0x38, 0x00, SDL_ALPHA_OPAQUE);
	m_colours[23] = ::SDL_MapRGBA(hardware, 0xE4, 0x5C, 0x10, SDL_ALPHA_OPAQUE);
	m_colours[24] = ::SDL_MapRGBA(hardware, 0xAC, 0x7C, 0x00, SDL_ALPHA_OPAQUE);
	m_colours[25] = ::SDL_MapRGBA(hardware, 0x00, 0xB8, 0x00, SDL_ALPHA_OPAQUE);
	m_colours[26] = ::SDL_MapRGBA(hardware, 0x00, 0xA8, 0x00, SDL_ALPHA_OPAQUE);
	m_colours[27] = ::SDL_MapRGBA(hardware, 0x00, 0xA8, 0x44, SDL_ALPHA_OPAQUE);
	m_colours[28] = ::SDL_MapRGBA(hardware, 0x00, 0x88, 0x88, SDL_ALPHA_OPAQUE);
	m_colours[29] = ::SDL_MapRGBA(hardware, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	m_colours[30] = ::SDL_MapRGBA(hardware, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	m_colours[31] = ::SDL_MapRGBA(hardware, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	m_colours[32] = ::SDL_MapRGBA(hardware, 0xF8, 0xF8, 0xF8, SDL_ALPHA_OPAQUE);
	m_colours[33] = ::SDL_MapRGBA(hardware, 0x3C, 0xBC, 0xFC, SDL_ALPHA_OPAQUE);
	m_colours[34] = ::SDL_MapRGBA(hardware, 0x68, 0x88, 0xFC, SDL_ALPHA_OPAQUE);
	m_colours[35] = ::SDL_MapRGBA(hardware, 0x98, 0x78, 0xF8, SDL_ALPHA_OPAQUE);
	m_colours[36] = ::SDL_MapRGBA(hardware, 0xF8, 0x78, 0xF8, SDL_ALPHA_OPAQUE);
	m_colours[37] = ::SDL_MapRGBA(hardware, 0xF8, 0x58, 0x98, SDL_ALPHA_OPAQUE);
	m_colours[38] = ::SDL_MapRGBA(hardware, 0xF8, 0x78, 0x58, SDL_ALPHA_OPAQUE);
	m_colours[39] = ::SDL_MapRGBA(hardware, 0xFC, 0xA0, 0x44, SDL_ALPHA_OPAQUE);
	m_colours[40] = ::SDL_MapRGBA(hardware, 0xF8, 0xB8, 0x00, SDL_ALPHA_OPAQUE);
	m_colours[41] = ::SDL_MapRGBA(hardware, 0xB8, 0xF8, 0x18, SDL_ALPHA_OPAQUE);
	m_colours[42] = ::SDL_MapRGBA(hardware, 0x58, 0xD8, 0x54, SDL_ALPHA_OPAQUE);
	m_colours[43] = ::SDL_MapRGBA(hardware, 0x58, 0xF8, 0x98, SDL_ALPHA_OPAQUE);
	m_colours[44] = ::SDL_MapRGBA(hardware, 0x00, 0xE8, 0xD8, SDL_ALPHA_OPAQUE);
	m_colours[45] = ::SDL_MapRGBA(hardware, 0x78, 0x78, 0x78, SDL_ALPHA_OPAQUE);
	m_colours[46] = ::SDL_MapRGBA(hardware, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	m_colours[47] = ::SDL_MapRGBA(hardware, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	m_colours[48] = ::SDL_MapRGBA(hardware, 0xFC, 0xFC, 0xFC, SDL_ALPHA_OPAQUE);
	m_colours[49] = ::SDL_MapRGBA(hardware, 0xA4, 0xE4, 0xFC, SDL_ALPHA_OPAQUE);
	m_colours[50] = ::SDL_MapRGBA(hardware, 0xB8, 0xB8, 0xF8, SDL_ALPHA_OPAQUE);
	m_colours[51] = ::SDL_MapRGBA(hardware, 0xD8, 0xB8, 0xF8, SDL_ALPHA_OPAQUE);
	m_colours[52] = ::SDL_MapRGBA(hardware, 0xF8, 0xB8, 0xF8, SDL_ALPHA_OPAQUE);
	m_colours[53] = ::SDL_MapRGBA(hardware, 0xF8, 0xA4, 0xC0, SDL_ALPHA_OPAQUE);
	m_colours[54] = ::SDL_MapRGBA(hardware, 0xF0, 0xD0, 0xB0, SDL_ALPHA_OPAQUE);
	m_colours[55] = ::SDL_MapRGBA(hardware, 0xFC, 0xE0, 0xA8, SDL_ALPHA_OPAQUE);
	m_colours[56] = ::SDL_MapRGBA(hardware, 0xF8, 0xD8, 0x78, SDL_ALPHA_OPAQUE);
	m_colours[57] = ::SDL_MapRGBA(hardware, 0xD8, 0xF8, 0x78, SDL_ALPHA_OPAQUE);
	m_colours[58] = ::SDL_MapRGBA(hardware, 0xB8, 0xF8, 0xB8, SDL_ALPHA_OPAQUE);
	m_colours[59] = ::SDL_MapRGBA(hardware, 0xB8, 0xF8, 0xD8, SDL_ALPHA_OPAQUE);
	m_colours[60] = ::SDL_MapRGBA(hardware, 0x00, 0xFC, 0xFC, SDL_ALPHA_OPAQUE);
	m_colours[61] = ::SDL_MapRGBA(hardware, 0xF8, 0xD8, 0xF8, SDL_ALPHA_OPAQUE);
	m_colours[62] = ::SDL_MapRGBA(hardware, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	m_colours[63] = ::SDL_MapRGBA(hardware, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
}
