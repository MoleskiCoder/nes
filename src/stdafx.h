#pragma once

#include <stdexcept>
#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <cassert>

#include <Memory.h>
#include <Ram.h>
#include <Bus.h>
#include <Register.h>
#include <Disassembly.h>

#include <Ricoh2A03.h>

#include <SDL.h>

#ifdef _MSC_VER
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#endif

//#define NESTEST