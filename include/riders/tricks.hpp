#pragma once

#include "../types.hpp"

/**
 * The trick ranks in the game
 * @note Rank will never be set to TenPlus in vanilla code, it is only used in custom code
 */
enum TrickRanks : u8 {
	CRank, BRank, ARank, APlusRank, SRank, SPlusRank, XRank, TenPlus
};