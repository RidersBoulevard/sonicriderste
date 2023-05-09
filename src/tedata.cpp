#include <array>
#include <cctype>
#include "main.hpp"

template<std::size_t size>
consteval std::array<char, size * 2> makeTe(const char (&str)[size]) {
	std::array<char, size * 2> buf{};
	int spaces = 0;
	for (std::size_t i = 0; i < size; i++) {
		const std::size_t &destIdx = (i - static_cast<size_t>(spaces)) * 2;
		char curChar = str[i];
		switch (curChar) {
			case 'A'...'Z':
				curChar = _tolower(curChar);
				[[fallthrough]];
			case 'a'...'z':
				buf[destIdx] = curChar - 'a';
				buf[destIdx + 1] = static_cast<char>(0xFC);
				continue;
			case ' ': // Got a space, go back and enable the space, then skip this character
				buf[destIdx - 1] = static_cast<char>(0x08);
				spaces++;
				continue;
			case '\0':
				buf[destIdx] = static_cast<char>(0xFF);
				buf[destIdx + 1] = static_cast<char>(0x00);
				return buf;
			default: // Skip character if unknown how to handle
				continue;
		}
	}
	return buf;
}

template<std::size_t size>
consteval std::size_t getTeLength(const std::array<char, size> &array) {
	std::size_t chars = 0;
	for (std::size_t i = 0; i < size; i += 2) {
		chars++;
		if (array[i] == 0xFF) break;
	}
	return chars;
}

constexpr auto silver_Psychokinetic = makeTe("psychokinetic");
constexpr auto silver_NoUse = makeTe("no use");
constexpr auto silver_Future = makeTe("future");
constexpr auto silver_Change = makeTe("change");

constexpr auto silver_PsychoBoost = makeTe("psycho boost");
constexpr auto silver_Levitation = makeTe("levitation");
constexpr auto silver_Timeline = makeTe("timeline");
constexpr auto silver_Courageous = makeTe("courageous");

constexpr auto silver_PsychicBurst = makeTe("psychic burst");
constexpr auto silver_Savior = makeTe("savior");
constexpr auto silver_Energized = makeTe("energized");
constexpr auto silver_ESP = makeTe("esp");

constexpr auto metalsonic_Assimilated = makeTe("assimilated");
constexpr auto metalsonic_Destructive = makeTe("destructive");
constexpr auto metalsonic_Speedbreak = makeTe("speedbreak");
constexpr auto metalsonic_Data = makeTe("data");

constexpr auto metalsonic_Unparalleled = makeTe("unparalleled");
constexpr auto metalsonic_Mechanical = makeTe("mechanical");
constexpr auto metalsonic_Strength = makeTe("strength");
constexpr auto metalsonic_MetalTriad = makeTe("metal triad");

constexpr auto metalsonic_Stardust = makeTe("stardust");
constexpr auto metalsonic_Speedway = makeTe("speedway");
constexpr auto metalsonic_Neo = makeTe("neo");
constexpr auto metalsonic_Metal = makeTe("metal");

constexpr auto emerl_Gizoid = makeTe("gizoid");
constexpr auto emerl_Indy = makeTe("indy");
constexpr auto emerl_Card = makeTe("card");
constexpr auto emerl_Immortal = makeTe("immortal");

constexpr auto emerl_Killer = makeTe("killer");
constexpr auto emerl_Orange = makeTe("orange");
constexpr auto emerl_Copycat = makeTe("copycat");
constexpr auto emerl_HiddenPotential = makeTe("hidden potential");

constexpr auto emerl_SkillShot = makeTe("skill shot");
constexpr auto emerl_BlankSlate = makeTe("blank slate");
constexpr auto emerl_Weaponized = makeTe("weaponized");
constexpr auto emerl_Nocturnus = makeTe("nocturnus");

constexpr auto blaze_Sol = makeTe("sol");
constexpr auto blaze_Ablaze = makeTe("ablaze");
constexpr auto blaze_Emerald = makeTe("emerald");
constexpr auto blaze_Pyrokinesis = makeTe("pyrokinesis");

constexpr auto blaze_Speedster = makeTe("speedster");
constexpr auto blaze_Rush = makeTe("rush");
constexpr auto blaze_Adventure = makeTe("adventure");
constexpr auto blaze_RadialBurst = makeTe("radial burst");

constexpr auto blaze_Royal = makeTe("royal");
constexpr auto blaze_Princess = makeTe("princess");
constexpr auto blaze_HotCat = makeTe("hot cat");
constexpr auto blaze_SoulSpin = makeTe("soul spin");

constexpr auto chaos_Hydrokinetic = makeTe("hydrokinetic");
constexpr auto chaos_Dive = makeTe("dive");
constexpr auto chaos_Chaotic = makeTe("chaotic");
constexpr auto chaos_SwiftSwim = makeTe("swift swim");

constexpr auto chaos_Bind = makeTe("bind");
constexpr auto chaos_Guardian = makeTe("guardian");
constexpr auto chaos_Mutation = makeTe("mutation");
constexpr auto chaos_WaterBend = makeTe("water bend");

constexpr auto chaos_Evolution = makeTe("evolution");
constexpr auto chaos_Elastic = makeTe("elastic");
constexpr auto chaos_Stretch = makeTe("stretch");
constexpr auto chaos_SpiralUpper = makeTe("spiral upper");

constexpr auto tikal_HeavenlyLight = makeTe("heavenly light");
constexpr auto tikal_Justice = makeTe("justice");
constexpr auto tikal_Tribal = makeTe("tribal");
constexpr auto tikal_Queen = makeTe("queen");

constexpr auto tikal_WrathOfGaia = makeTe("wrath of gaia");
constexpr auto tikal_Echidna = makeTe("echidna");
constexpr auto tikal_AltarServant = makeTe("altar servant");
constexpr auto tikal_EmeraldPrayer = makeTe("emerald prayer");

constexpr auto tikal_Ghostly = makeTe("ghostly");
constexpr auto tikal_PastTense = makeTe("past tense");
constexpr auto tikal_Harmony = makeTe("harmony");
constexpr auto tikal_Golden = makeTe("golden");


global {
	volatile constexpr char silver_lengths[] = {
			// skate trick text strings
            getTeLength(silver_PsychicBurst),
            getTeLength(silver_Savior),
            getTeLength(silver_Energized),
            getTeLength(silver_ESP),

			// bike trick text strings
			getTeLength(silver_PsychoBoost), getTeLength(silver_Levitation), getTeLength(silver_Timeline), getTeLength(silver_Courageous),

			// board trick text strings
			getTeLength(silver_Psychokinetic), getTeLength(silver_Future), getTeLength(silver_Change),
			getTeLength(silver_NoUse)
	};
	global constexpr const char *silver_stringptrs[] = {
			// skate trick text strings
            silver_PsychicBurst.data(),
            silver_Savior.data(),
            silver_Energized.data(),
            silver_ESP.data(),

			// bike trick text strings
			silver_PsychoBoost.data(), silver_Levitation.data(), silver_Timeline.data(), silver_Courageous.data(),

			// board trick text strings
			silver_Psychokinetic.data(), silver_Future.data(), silver_Change.data(), silver_NoUse.data()
	};

	volatile constexpr char tikal_lengths[] = {
			// skate trick text strings
            getTeLength(tikal_Ghostly),
            getTeLength(tikal_PastTense),
            getTeLength(tikal_Harmony),
            getTeLength(tikal_Golden),

			// bike trick text strings
			getTeLength(tikal_WrathOfGaia), getTeLength(tikal_Echidna), getTeLength(tikal_AltarServant), getTeLength(tikal_EmeraldPrayer),

			// board trick text strings
			getTeLength(tikal_HeavenlyLight), getTeLength(tikal_Justice), getTeLength(tikal_Tribal),
			getTeLength(tikal_Queen)
	};
	global constexpr const char *tikal_stringptrs[] = {
			// skate trick text strings
			tikal_Ghostly.data(),
            tikal_PastTense.data(),
            tikal_Harmony.data(),
            tikal_Golden.data(),

			// bike trick text strings
			tikal_WrathOfGaia.data(), tikal_Echidna.data(), tikal_AltarServant.data(), tikal_EmeraldPrayer.data(),

			// board trick text strings
			tikal_HeavenlyLight.data(), tikal_Justice.data(), tikal_Tribal.data(), tikal_Queen.data()
	};

	volatile constexpr char chaos_lengths[] = {
			// skate trick text strings
            getTeLength(chaos_Evolution),
            getTeLength(chaos_Elastic),
            getTeLength(chaos_Stretch),
            getTeLength(chaos_SpiralUpper),

			// bike trick text strings
			getTeLength(chaos_Bind), getTeLength(chaos_Guardian), getTeLength(chaos_Mutation), getTeLength(chaos_WaterBend),

			// board trick text strings
			getTeLength(chaos_Hydrokinetic), getTeLength(chaos_Dive), getTeLength(chaos_Chaotic),
			getTeLength(chaos_SwiftSwim)
	};
	global constexpr const char *chaos_stringptrs[] = {
			// skate trick text strings
			chaos_Evolution.data(),
            chaos_Elastic.data(),
            chaos_Stretch.data(),
            chaos_SpiralUpper.data(),

			// bike trick text strings
			chaos_Bind.data(), chaos_Guardian.data(), chaos_Mutation.data(), chaos_WaterBend.data(),

			// board trick text strings
			chaos_Hydrokinetic.data(), chaos_Dive.data(), chaos_Chaotic.data(), chaos_SwiftSwim.data()
	};

	volatile constexpr char blaze_lengths[] = {
			// skate trick text strings
            getTeLength(blaze_Royal),
            getTeLength(blaze_Princess),
            getTeLength(blaze_HotCat),
            getTeLength(blaze_SoulSpin),

			// bike trick text strings
			getTeLength(blaze_Speedster), getTeLength(blaze_Rush), getTeLength(blaze_Adventure), getTeLength(blaze_RadialBurst),

			// board trick text strings
			getTeLength(blaze_Sol), getTeLength(blaze_Ablaze), getTeLength(blaze_Emerald), getTeLength(blaze_Pyrokinesis)
	};
	global constexpr const char *blaze_stringptrs[] = {
			// skate trick text strings
			blaze_Royal.data(),
            blaze_Princess.data(),
            blaze_HotCat.data(),
            blaze_SoulSpin.data(),

			// bike trick text strings
			blaze_Speedster.data(), blaze_Rush.data(), blaze_Adventure.data(), blaze_RadialBurst.data(),

			// board trick text strings
			blaze_Sol.data(), blaze_Ablaze.data(), blaze_Emerald.data(), blaze_Pyrokinesis.data()
	};

	volatile constexpr char emerl_lengths[] = {
			// skate trick text strings
            getTeLength(emerl_SkillShot),
            getTeLength(emerl_BlankSlate),
            getTeLength(emerl_Weaponized),
            getTeLength(emerl_Nocturnus),

			// bike trick text strings
			getTeLength(emerl_Killer), getTeLength(emerl_Orange), getTeLength(emerl_Copycat), getTeLength(emerl_HiddenPotential),

			// board trick text strings
			getTeLength(emerl_Gizoid), getTeLength(emerl_Indy), getTeLength(emerl_Card), getTeLength(emerl_Immortal)
	};
	global constexpr const char *emerl_stringptrs[] = {
			// skate trick text strings
			emerl_SkillShot.data(),
            emerl_BlankSlate.data(),
            emerl_Weaponized.data(),
            emerl_Nocturnus.data(),

			// bike trick text strings
			emerl_Killer.data(), emerl_Orange.data(), emerl_Copycat.data(), emerl_HiddenPotential.data(),

			// board trick text strings
			emerl_Gizoid.data(), emerl_Indy.data(), emerl_Card.data(), emerl_Immortal.data()
	};

	volatile constexpr char metalsonic_lengths[] = {
			// skate trick text strings
            getTeLength(metalsonic_Stardust),
            getTeLength(metalsonic_Speedway),
            getTeLength(metalsonic_Neo),
            getTeLength(metalsonic_Metal),

			// bike trick text strings
			getTeLength(metalsonic_Unparalleled), getTeLength(metalsonic_Mechanical), getTeLength(metalsonic_Strength), getTeLength(metalsonic_MetalTriad),

			// board trick text strings
			getTeLength(metalsonic_Assimilated), getTeLength(metalsonic_Destructive), getTeLength(metalsonic_Speedbreak),
			getTeLength(metalsonic_Data)
	};
	global constexpr const char *metalsonic_stringptrs[] = {
			// skate trick text strings
			metalsonic_Stardust.data(),
            metalsonic_Speedway.data(),
            metalsonic_Neo.data(),
            metalsonic_Metal.data(),

			// bike trick text strings
			metalsonic_Unparalleled.data(), metalsonic_Mechanical.data(), metalsonic_Strength.data(), metalsonic_MetalTriad.data(),

			// board trick text strings
			metalsonic_Assimilated.data(), metalsonic_Destructive.data(), metalsonic_Speedbreak.data(),
			metalsonic_Data.data()
	};

	volatile constexpr signed char CharacterAFSVoiceLines[][9] = {
			// first 6 are never used, so they're -1
			// 7th: race start voiceline ID in AFS
			// 8th: race win voiceline ID in AFS
			// 9th: race lose voiceline ID in AFS
			{-1, -1, -1, -1, -1, -1, 0x1C, 0x1D, 0x1E},
			{-1, -1, -1, -1, -1, -1, 0x25, 0x26, 0x27},
			{-1, -1, -1, -1, -1, -1, 0x13, 0x14, 0x15},
			{-1, -1, -1, -1, -1, -1, 0x0,  0x1,  0x2},
			{-1, -1, -1, -1, -1, -1, 0x10, 0x11, 0x12},
			{-1, -1, -1, -1, -1, -1, 0x22, 0x23, 0x24},
			{-1, -1, -1, -1, -1, -1, 0x28, 0x29, 0x2A},
			{-1, -1, -1, -1, -1, -1, 0x6,  0x7,  0x8},
			{-1, -1, -1, -1, -1, -1, 0x3,  0x4,  0x5},
			{-1, -1, -1, -1, -1, -1, 0x16, 0x17, 0x18},
			{-1, -1, -1, -1, -1, -1, 0x19, 0x1A, 0x1B},
			{-1, -1, -1, -1, -1, -1, 0x1F, 0x20, 0x21},
			{-1, -1, -1, -1, -1, -1, -1,   -1,   -1},
			{-1, -1, -1, -1, -1, -1, 0x2B, 0x2C, 0x2D},
			{-1, -1, -1, -1, -1, -1, 0x2E, 0x2F, 0x30},
			{-1, -1, -1, -1, -1, -1, 0xA,  0xB,  0xC},
			{-1, -1, -1, -1, -1, -1, 0xD,  0xE,  0xF},

			{-1, -1, -1, -1, -1, -1, 49,   50,   51},
			{-1, -1, -1, -1, -1, -1, 52,   53,   54},
			{-1, -1, -1, -1, -1, -1, 55,   56,   57},
			{-1, -1, -1, -1, -1, -1, 58,   59,   60},
			{-1, -1, -1, -1, -1, -1, 61,   62,   63},
			{-1, -1, -1, -1, -1, -1, 64,   65,   66},
	};
}