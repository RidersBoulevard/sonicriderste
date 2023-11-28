cmake_minimum_required(VERSION 3.13)
project(SonicRidersTE CXX ASM)

set(CXX_STANDARD 23)
set(WARNING_FLAGS
		#-Werror    # Turns all warnings into errors # Would not recommend enabling unless you're planning to fix *every* warning
		-Wall   # Enable most warning messages.
		-Wextra # Print extra (possibly unwanted) warnings.
		#-Wpedantic # Issue warnings needed for strict compliance to the standard. # We'll probably never need this
		-Wcast-align    # Warn about pointer casts which increase alignment. For example, warn if a char * is cast to an int * on machines where integers can only be accessed at two- or four-byte boundaries.
		-Wcast-qual # Warn about casts which discard qualifiers. For example, warn if a const char * is cast to an ordinary char *
		-Wctor-dtor-privacy # Warn when all constructors and destructors are private.
		-Wdisabled-optimization # Warn when an optimization pass is disabled.
		-Wformat=2  # Warn about printf/scanf/strftime/strfmon format string anomalies.
		-Winit-self # Warn about variables which are initialized to themselves.
		-Wlogical-op    # Warn when a logical operator is suspiciously always evaluating to true or false.
		#-Wmissing-declarations # Warn about global functions without previous declarations.
		-Wmissing-include-dirs # Warn about user-specified include directories that do not exist.
		-Wnoexcept  # Warn when a noexcept expression evaluates to false even though the expression can't actually throw.
		-Wold-style-cast    # Warn if a C-style cast is used in a program.
		-Woverloaded-virtual    # Warn about overloaded virtual function names.
		-Wredundant-decls   # Warn about multiple declarations of the same object.
		-Wshadow    # Warn when one variable shadows another # Might disable this one
		-Wsign-conversion   # Warn for implicit type conversions between signed and unsigned integers.
		-Wsign-promo    # Warn when overload promotes from unsigned to signed.
		-Wstrict-null-sentinel  # Warn about uncasted NULL used as sentinel.
		-Wstrict-overflow=5 # Warn about optimizations that assume that signed overflow is undefined.
		-Wswitch-default    # Warn about enumerated switches missing a "default:" statement.
		-Wundef # Warn if an undefined macro is used in an #if directive.
		-Wzero-as-null-pointer-constant # Warn when a literal '0' is used as null pointer.
		-Wuseless-cast  # Warn about useless casts.
		-Wpadded # Warn when padding is required to align structure members.

		-Wno-unknown-pragmas # Disables warning about unkown pragmas since both clion and clang-tidy use their own
		) # May need to add or remove some warnings from this list depending on if they actually apply to our system or not
#set(WARNING_FLAGS -w) # Disable all warnings

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)

set(DOL_SOURCES     # main
		asm/init.S
		asm/text.S
		asm/lib.S
		# src/containers/vector3.cpp
		src/tweaks/stage/stage_changes.cpp
		asm/ctors.S
		asm/dtors.S
		asm/rodata.S
		asm/data.S
		asm/bss.S
		asm/sdata.S
		asm/sbss.S
		asm/sdata2.S
		asm/sbss2.S
		asm/_Main/text.S
		asm/_Main/rodata.S
		asm/_Main/data.S
		asm/_Main/bss.S
		src/tedata.cpp
		src/lib/operators.cpp
		src/handlers/files/filehandler_loadingscreen.cpp
		src/mechanics/slipstream.cpp
		src/mechanics/fastfall.cpp
		src/handlers/music/custom_music.cpp
		src/handlers/files/filehandler_dat.cpp
		src/handlers/prolog.cpp
		src/handlers/stage/customobjects.cpp
		src/handlers/menu/tournamentrace/stageban_handler.cpp
		src/cosmetics/player/hypersonic_lighting.cpp
		src/tweaks/typeshortcuts/flyhoopspeeds.cpp
		src/tweaks/player/archetype/character_archetype.cpp
		src/tweaks/player/archetype/customboostduration.cpp
		src/handlers/player/quillmovement.cpp
		src/handlers/player/SetPlayerState.cpp
		src/gears/tweaks/ringgear_ringsfromairboxes.cpp
		src/tweaks/stage/itemrngchange.cpp
		src/tweaks/typeshortcuts/typeshortcut_modifications.cpp
		src/tweaks/stage/icefactory/qteflyhoop.cpp
		src/handlers/player/deathplane.cpp
		src/tweaks/stage/greencave/miniqte.cpp
		src/handlers/player/dynamic_itemboxcooldown.cpp
		src/tweaks/player/tricks/turbulencetrickspeed.cpp
		src/handlers/player/timerhandler.cpp
		src/handlers/player/zignoretornado.cpp
		src/tweaks/stage/segaillusion/chuchurocket_dashpanel.cpp
		src/tweaks/stage/custompowerobjecttimer.cpp
        src/tweaks/player/weight.cpp
		src/handlers/player/antiitemcamp.cpp
		src/tweaks/player/jumpinrun.cpp
		src/cosmetics/player/jumpchargeparticlevisibility.cpp
		src/tweaks/player/driftcap.cpp
		src/tweaks/stage/signgrinding.cpp
		src/handlers/files/LoadCharacterDATFile.cpp
		src/cosmetics/hud/holdmorerings.cpp
		src/cosmetics/player/exloads.cpp
		src/cosmetics/stage/fogdistance.cpp
		src/mechanics/magneticimpulse.cpp
		src/tweaks/typeshortcuts/grindrailspeed.cpp
		src/tweaks/stage/rail_fastfall.cpp
		src/handlers/player/zbutton.cpp
		src/tweaks/stage/additive_dashpanels.cpp
		src/cosmetics/japanesevoice.cpp
		src/handlers/ingame/customtext.cpp
		src/tweaks/stage/rampdistance.cpp
		src/tweaks/bugfixes/fixsegaillusionfastqte.cpp
		src/handlers/files/texturehotswap_handler.cpp
		src/handlers/files/filehandler_mainmenu.cpp
		src/tweaks/sfx.cpp
		src/tweaks/bugfixes/eggfacqtefix.cpp
		src/lib/lib.cpp
		src/cosmetics/menu/stageicon.cpp
		src/gears/berserker.cpp
		src/gears/beginner.cpp
		src/cosmetics/player/exhaustTrail.cpp
		src/tweaks/player/tricks/trickofgreed.cpp
		src/gears/coverf.cpp
		src/gears/tweaks/nolevelup.cpp
		src/gears/fastest.cpp
		src/gears/tweaks/stickyfingersrequiem.cpp
		src/handlers/player/initgeardata.cpp
		src/gears/supertails.cpp
		src/gears/supermetal.cpp
		src/mechanics/boostcontrol.cpp
		src/gears/gambler.cpp
		src/tweaks/player/boostchainmultiplier.cpp
		src/handlers/files/separatemodelloading.cpp
		src/mechanics/dash/grindraildash.cpp
		src/handlers/handlers.cpp
		src/mechanics/dash/flyhoopdash.cpp
		src/mechanics/lcanceling.cpp
		src/handlers/files/packman_handlers.cpp
		src/handlers/menu/graphicalobject_handlers.cpp
		src/lib/calculate_date.cpp
		src/aprilfools/aprilfools.cpp
		src/gears/slidebooster.cpp
		src/mechanics/archetype/superleap.cpp
		src/mechanics/characters/gizoidreplication.cpp
		src/tweaks/player/archetype/boostarchetypejcbc.cpp
		src/handlers/customcodehandler.cpp
		src/handlers/customcodehandler_func.cpp
		src/gears/blastGaugeGears.cpp
		src/gears/hypersonic.cpp
		src/gears/faster.cpp
		src/cosmetics/menu/menucolors.cpp
		src/gears/magiccarpet.cpp
		src/cosmetics/player/jumpchargeparticlecolor.cpp
		src/mechanics/airoutbutton.cpp
		src/gears/turbostar.cpp
		src/gears/powergear.cpp
		src/handlers/menu/debugmenu/debugmenu.cpp
		src/mechanics/autopilot.cpp
		src/handlers/menu/debugmenu/debugmenu_handlers.cpp
		src/gears/speedbalancer.cpp
		src/gears/thecrazy.cpp
		src/gears/hangon.cpp
		src/gears/advantagef.cpp
		src/gears/covers.cpp
		src/gears/tweaks/ringgearlevels.cpp
		src/gears/stardustspeederII.cpp
		src/gears/coverp.cpp
		src/handlers/ingame/customhud.cpp
		src/mechanics/archetype/afterburner.cpp
        src/handlers/player/voicelines.cpp
		src/handlers/ingame/customcss.cpp
		src/cosmetics/player/supersonicaura.cpp
		src/tweaks/player/runspeed.cpp
		src/handlers/menu/randomstage.cpp
		src/handlers/menu/randomstage.hpp
		src/cosmetics/player/skinsystem.cpp
		# src/lib/sound.cpp
		src/lib/thread.cpp src/lib/thread.hpp
		src/handlers/player/specialflagtweaks.cpp
        src/handlers/files/load_deduplicated.cpp) # main

set(STANDALONE_HEADERS # This is only here to make sure that headers actually include the things it needs, by "compiling" them standalone.
		include/compare_many.hpp
		include/flags.hpp
		include/globalDefs.hpp
		include/macros.h
		include/main.hpp
		include/types.hpp
		include/riders/character.hpp
		include/riders/collision.hpp
		include/riders/controller.hpp
		include/riders/gear.hpp
		include/riders/player.hpp
		include/riders/general_flags.hpp
		include/riders/stage.hpp
		include/riders/tricks.hpp
		include/riders/gamemode.hpp
		include/riders/object.hpp
		include/riders/hud.hpp
        include/endian.hpp)

#====================================================================================
# Extra plumbing stuff. Hopefully nothing in here will need to be changed
#====================================================================================

#Set default build type to be release. add `-DCMAKE_BUILD_TYPE=debug` to cmake command to configure a debug build
set(release_build_type "Release")
set(debug_build_type "Debug")
# Set a default build type if none was specified
set(default_build_type ${release_build_type})

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	#message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
	set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
			STRING "Choose the type of build." FORCE)
	# Set the possible values of build type for cmake-gui
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
			"Debug" "Release")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(DEVKITPRO $ENV{DEVKITPRO} CACHE PATH "Path to the main DevKitPro tools directory")

set(CMAKE_SYSTEM_NAME NintendoGameCube)
include(${DEVKITPRO}/cmake/GameCube.cmake)
include(${DEVKITPRO}/cmake/Platform/NintendoGameCube.cmake)

set(CMAKE_CXX_STANDARD ${CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)     # Enforces the standard we set instead of silently using an older standard if not available
set(CMAKE_CXX_EXTENSIONS ON)            # Enables the use of compiler-specific extensions

add_executable(main ${DOL_SOURCES} ${STANDALONE_HEADERS})

#unset(CMAKE_CXX_STANDARD_LIBRARIES)
#set(CMAKE_CXX_STANDARD_LIBRARIES "")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT}")

#set(OPTIMIZATION_FLAGS -O3)
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Ofast -DDEBUG -save-temps -fverbose-asm")
set(CMAKE_CXX_FLAGS_RELEASE "-g0 -Ofast -DNDEBUG")

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS_INIT}")
set(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

target_compile_options(main PUBLIC ${WARNING_FLAGS} -memb)

get_filename_component(ASMINCLUDE asm ABSOLUTE)
get_filename_component(DOLINCLUDE dolinclude ABSOLUTE) # For any binary files included directly into the dol
get_filename_component(CINCLUDE include ABSOLUTE)
target_compile_options(main PUBLIC -Wa,-D,-I${ASMINCLUDE},-I${CINCLUDE},-I${DOLINCLUDE}) # For inline asm

target_link_directories(main PUBLIC ${DEVKITPRO}/libogc/lib/cube)
target_include_directories(main PUBLIC ${DEVKITPRO}/libogc/include/)

target_include_directories(main PUBLIC src include)

if (${FORCE_COLORED_OUTPUT})
	#if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	target_compile_options(main PUBLIC -fdiagnostics-color=always)
	#elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	#	add_compile_options (-fcolor-diagnostics)
	#endif ()
endif ()

get_filename_component(STATICLCF static.lcf ABSOLUTE)
get_filename_component(PARSEMAPPY tools/parse_map.py ABSOLUTE)

target_link_options(main PRIVATE -T ${STATICLCF} -Wl,-Map=main.map,-d,--wrap=malloc)

if("${CMAKE_BUILD_TYPE}" STREQUAL "${debug_build_type}")
	target_link_libraries(main PUBLIC db)
endif ()
target_link_libraries(main PUBLIC ogc m)

find_program(ELF2DOL NAMES ELF2DOL elf2dol HINTS "tools/build")
if (NOT ELF2DOL)
	message(FATAL_ERROR "Could not find elf2dol: try running makeTools.sh")
endif ()

function(srte_create_dol target)
	__dkp_target_derive_name(DOL_OUTPUT ${target} ".dol")
	add_custom_command(TARGET ${target} POST_BUILD
			COMMAND "${ELF2DOL}" "$<TARGET_FILE:${target}>" "${DOL_OUTPUT}"
			BYPRODUCTS "${DOL_OUTPUT}"
			COMMENT "Converting ${target} to .dol format"
			VERBATIM
			)
	dkp_set_target_file(${target} "${DOL_OUTPUT}")
endfunction()

srte_create_dol(main)

add_custom_target(
		mapfile ALL
		DEPENDS GXSRTE.map
)

find_package(Python REQUIRED)

add_custom_command(
		OUTPUT GXSRTE.map
		COMMAND ${Python_EXECUTABLE} ${PARSEMAPPY} ${PROJECT_BINARY_DIR}
		COMMENT "Creating Dolphin map file"
		DEPENDS main
		VERBATIM
)

#[[
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

install(TARGETS main
		RUNTIME DESTINATION data/sys)

install(TARGETS _Main
		RUNTIME DESTINATION data/files)
]]

# Uncomment to list all variables in the project
#[[
get_cmake_property(_variableNames VARIABLES)
list(REMOVE_DUPLICATES _variableNames)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
	message(STATUS "${_variableName}=${${_variableName}}")
endforeach()
]]