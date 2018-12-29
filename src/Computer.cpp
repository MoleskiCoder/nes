#include "stdafx.h"
#include "Computer.h"

#include <iostream>

Computer::Computer(const Configuration& configuration)
: m_configuration(configuration),
  m_board(configuration, m_colours) {
	m_board.WrittenByte.connect(std::bind(&Computer::Board_WrittenByte, this, std::placeholders::_1));
}

void Computer::plug(const std::string& path) {
	m_board.plug(path);
}

void Computer::initialise() {

	verifySDLCall(::SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC), "Failed to initialise SDL: ");

	m_window = ::SDL_CreateWindow(
		"GameBoy",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		ScreenWidth, ScreenHeight,
		SDL_WINDOW_SHOWN);

	if (m_window == nullptr)
		throwSDLException("Unable to create window: ");

	::SDL_DisplayMode mode;
	verifySDLCall(::SDL_GetWindowDisplayMode(m_window, &mode), "Unable to obtain window information");

	Uint32 rendererFlags = 0;
	if (m_vsync) {
		auto required = m_fps;
		if (required == mode.refresh_rate) {
			rendererFlags |= SDL_RENDERER_PRESENTVSYNC;
			::SDL_Log("Attempting to use SDL_RENDERER_PRESENTVSYNC");
		} else {
			m_vsync = false;
			::SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Display refresh rate is incompatible with required rate (%d)", required);
		}
	}
	m_renderer = ::SDL_CreateRenderer(m_window, -1, rendererFlags);
	if (m_renderer == nullptr)
		throwSDLException("Unable to create renderer: ");

	::SDL_Log("Available renderers:");
	dumpRendererInformation();

	::SDL_RendererInfo info;
	verifySDLCall(::SDL_GetRendererInfo(m_renderer, &info), "Unable to obtain renderer information");
	::SDL_Log("Using renderer:");
	dumpRendererInformation(info);

	if (m_vsync) {
		if ((info.flags & SDL_RENDERER_PRESENTVSYNC) == 0) {
			::SDL_LogWarn(::SDL_LOG_CATEGORY_APPLICATION, "Renderer does not support VSYNC, reverting to timed delay loop.");
			m_vsync = false;
		}
	}

	m_pixelFormat = ::SDL_AllocFormat(m_pixelType);
	if (m_pixelFormat == nullptr)
		throwSDLException("Unable to allocate pixel format: ");
	m_colours.load(m_pixelFormat);

	configureBackground();
	createBitmapTexture();
}

void Computer::configureBackground() const {
	Uint8 r, g, b;
	r = g = b = 0;
	verifySDLCall(::SDL_SetRenderDrawColor(m_renderer, r, g, b, SDL_ALPHA_OPAQUE), "Unable to set render draw colour");
}

void Computer::createBitmapTexture() {
	m_bitmapTexture = ::SDL_CreateTexture(m_renderer, m_pixelType, SDL_TEXTUREACCESS_STREAMING, Display::RasterWidth, Display::RasterHeight);
	if (m_bitmapTexture == nullptr)
		throwSDLException("Unable to create bitmap texture");
}

void Computer::run() {

	auto& cpu = m_board.CPU();

	cpu.powerOn();

	m_board.reset();

	// Nestest...
	cpu.raise(cpu.RESET());
	cpu.PC().word = 0xC000;	// Hack!!
	cpu.X() = 0x00;			// Hack!!
	cpu.P() |= 0x04;		// Hack!!
	cpu.S() = 0xFD;			// Hack!!

	auto cycles = 0;

	auto graphics = m_configuration.isDrawGraphics();

	while (cpu.powered()) {

		::SDL_Event e;
		while (::SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				cpu.powerOff();
				break;
			}
		}

		m_frameCycles = 0;

		cycles += Board::CyclesPerFrame;

		cycles -= m_board.runScanLinesRaster();

		if (graphics) {
			drawFrame();
			::SDL_RenderPresent(m_renderer);
			if (!m_vsync) {
				const auto elapsedTicks = ::SDL_GetTicks() - m_startTicks;
				const auto neededTicks = (++m_frames / (float)m_fps) * 1000.0;
				auto sleepNeeded = (int)(neededTicks - elapsedTicks);
				if (sleepNeeded > 0)
					::SDL_Delay(sleepNeeded);
			}
		}

		cycles -= m_board.runScanLinesVBlank();
	}
}

void Computer::drawFrame() {
	verifySDLCall(::SDL_UpdateTexture(m_bitmapTexture, NULL, &(m_board.PPU().pixels()[0]), Display::RasterWidth * sizeof(Uint32)), "Unable to update texture: ");
	verifySDLCall(
		::SDL_RenderCopy(m_renderer, m_bitmapTexture, nullptr, nullptr),
		"Unable to copy texture to renderer");
}

void Computer::dumpRendererInformation() {
	auto count = ::SDL_GetNumRenderDrivers();
	for (int i = 0; i < count; ++i) {
		::SDL_RendererInfo info;
		verifySDLCall(::SDL_GetRenderDriverInfo(i, &info), "Unable to obtain renderer information");
		dumpRendererInformation(info);
	}
}

void Computer::dumpRendererInformation(::SDL_RendererInfo info) {
	auto name = info.name;
	auto flags = info.flags;
	int software = (flags & SDL_RENDERER_SOFTWARE) != 0;
	int accelerated = (flags & SDL_RENDERER_ACCELERATED) != 0;
	int vsync = (flags & SDL_RENDERER_PRESENTVSYNC) != 0;
	int targetTexture = (flags & SDL_RENDERER_TARGETTEXTURE) != 0;
	::SDL_Log("%s: software=%d, accelerated=%d, vsync=%d, target texture=%d", name, software, accelerated, vsync, targetTexture);
}

void Computer::Board_WrittenByte(const EightBit::EventArgs& e) {
	// for Blargg tests
	const auto address = m_board.ADDRESS().word;
	if (address >= 0x6004) {
		const auto contents = m_board.peek(address);
		std::cout << (char)contents;
	}
}