#include "stdafx.h"

#include "Configuration.h"
#include "Computer.h"

int main(int, char*[])
{
	Configuration configuration;

#ifdef _DEBUG
	configuration.setDebugMode(true);
	configuration.setProfileMode(true);
#endif

	Computer computer(configuration);

	computer.initialise();

	const std::string romDirectory = "roms/";

	// Mapper 0
	
	computer.plug(romDirectory + "/nestest.nes");	// pass
	
	//computer.plug(romDirectory + "/instr_test-v5/rom_singles/01-basics.nes");		// pass
	//computer.plug(romDirectory + "/instr_test-v5/rom_singles/02-implied.nes");	// pass
	//computer.plug(romDirectory + "/instr_test-v5/rom_singles/03-immediate.nes");	// illegal instruction failures
	//computer.plug(romDirectory + "/instr_test-v5/rom_singles/04-zero_page.nes");	// pass
	//computer.plug(romDirectory + "/instr_test-v5/rom_singles/05-zp_xy.nes");		// pass
	//computer.plug(romDirectory + "/instr_test-v5/rom_singles/06-absolute.nes");	// pass
	//computer.plug(romDirectory + "/instr_test-v5/rom_singles/07-abs_xy.nes");		// unimplemented exception
	//computer.plug(romDirectory + "/instr_test-v5/rom_singles/08-ind_x.nes");		// pass
	//computer.plug(romDirectory + "/instr_test-v5/rom_singles/09-ind_y.nes");		// pass
	//computer.plug(romDirectory + "/instr_test-v5/rom_singles/10-branches.nes");	// pass
	//computer.plug(romDirectory + "/instr_test-v5/rom_singles/11-stack.nes");		// pass
	//computer.plug(romDirectory + "/instr_test-v5/rom_singles/12-jmp_jsr.nes");	// pass
	//computer.plug(romDirectory + "/instr_test-v5/rom_singles/13-rts.nes");		// pass
	//computer.plug(romDirectory + "/instr_test-v5/rom_singles/14-rti.nes");		// pass
	//computer.plug(romDirectory + "/instr_test-v5/rom_singles/15-brk.nes");		// pass
	//computer.plug(romDirectory + "/instr_test-v5/rom_singles/16-special.nes");	// pass
	
	// Mapper 1?

	//computer.plug(romDirectory + "/instr_test-v5/official_only.nes");				// pass
	//computer.plug(romDirectory + "/instr_test-v5/all_instrs.nes");				// fail

	// Unknown
	//computer.plug(romDirectory + "/blargg_ppu_tests_2005.09.15b/palette_ram.nes");		// pass
	//computer.plug(romDirectory + "/blargg_ppu_tests_2005.09.15b/power_up_palette.nes");	// fail
	//computer.plug(romDirectory + "/blargg_ppu_tests_2005.09.15b/sprite_ram.nes");			// pass
	//computer.plug(romDirectory + "/blargg_ppu_tests_2005.09.15b/vbl_clear_time.nes");		// fail: $02 (VBL flag cleared too soon)
	//computer.plug(romDirectory + "/blargg_ppu_tests_2005.09.15b/vram_access.nes");		// fail $02 (VRAM reads should be delayed in a buffer)

	//computer.plug(romDirectory + "/full_palette/full_palette.nes");
	//computer.plug(romDirectory + "/color_test.nes");
	//computer.plug(romDirectory + "/oam_read/oam_read.nes");				// Pass
	//computer.plug(romDirectory + "/oam_stress/oam_stress.nes");				// fail

	// Games
	//computer.plug(romDirectory + "/Pac-Mania (USA) (Unl).nes");
	//computer.plug(romDirectory + "/Super Mario Bros (E).nes");
	//computer.plug(romDirectory + "/Donkey Kong Classics (U).nes");

	try {
		computer.run();
	} catch (const std::exception& error) {
		::SDL_LogError(::SDL_LOG_CATEGORY_APPLICATION, "%s", error.what());
		return 2;
	}

	return 0;
}