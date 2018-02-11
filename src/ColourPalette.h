#pragma once

#include <array>
#include <cstdint>

struct SDL_PixelFormat;

class ColourPalette final {
public:
	ColourPalette() = default;

	uint32_t getColour(size_t index) const;
	void load(SDL_PixelFormat* hardware);

private:
	std::array<uint32_t, 64> m_colours;
};
