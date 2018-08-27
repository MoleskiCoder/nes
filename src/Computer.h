#pragma once

#include "Board.h"
#include "Display.h"
#include "ColourPalette.h"

class Configuration;

class Computer {
public:
	static void throwSDLException(std::string failure) {
		throw std::runtime_error(failure + ::SDL_GetError());
	}

	static void verifySDLCall(int returned, std::string failure) {
		if (returned < 0) {
			throwSDLException(failure);
		}
	}

	Computer(const Configuration& configuration);

	void initialise();
	void plug(const std::string& path);
	void run();

private:
	void Board_WrittenByte(const EightBit::EventArgs& e);

	enum {
		DisplayScale = 2,
		ScreenWidth = Display::RasterWidth * DisplayScale,
		ScreenHeight = Display::RasterHeight * DisplayScale,
	};

	const Configuration& m_configuration;
	ColourPalette m_colours;
	mutable Board m_board;

	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;

	SDL_Texture* m_bitmapTexture = nullptr;
	Uint32 m_pixelType = SDL_PIXELFORMAT_ARGB8888;
	SDL_PixelFormat* m_pixelFormat = nullptr;

	int m_frameCycles = 0;

	int m_fps = Board::FramesPerSecond;
	Uint32 m_startTicks = 0;
	Uint32 m_frames = 0;
	bool m_vsync = true;

	void drawFrame();

	void configureBackground() const;
	void createBitmapTexture();

	static void dumpRendererInformation();
	static void dumpRendererInformation(::SDL_RendererInfo info);
};
