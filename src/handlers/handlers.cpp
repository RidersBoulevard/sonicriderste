#include "mechanics/magneticimpulse.hpp" // C24119A0
#include "handlers/player/deathplane.hpp" // C240F4F8
#include "mechanics/slipstream.hpp" // C240F4C4
#include "mechanics/fastfall.hpp" // C240F4EC
#include "cosmetics/player/hypersonic_lighting.hpp"
#include "handlers/player/timerhandler.hpp" // C240C9EC C2415B74 C240F4DC C24119A8 C24119CC
#include "gears/beginner.hpp" // C24119B0
#include "gears/coverf.hpp" // C24119D4
#include "gears/fastest.hpp" // C2411994
#include "gears/supertails.hpp" // C24119AC
#include "gears/supermetal.hpp"
#include "mechanics/dash/grindraildash.hpp"
#include "mechanics/dash/flyhoopdash.hpp"
#include "mechanics/lcanceling.hpp"
#include "mechanics/characters/gizoidreplication.hpp"
#include "gears/thecrazy.hpp"
#include "gears/hangon.hpp"
#include "gears/advantagef.hpp"
#include "gears/magiccarpet.hpp"
#include "gears/covers.hpp"
#include "gears/coverp.hpp"
#include "gears/powergear.hpp"
#include "gears/tweaks/ringgearlevels.hpp"
#include "gears/stardustspeederII.hpp"
#include "tweaks/player/archetype/boostarchetypejcbc.hpp"
#include "gears/speedbalancer.hpp"
#include "handlers/customcodehandler.hpp"
#include "gears/slidebooster.hpp"
#include "handlers/player/zbutton.hpp"
#include "mechanics/airoutbutton.hpp"
#include "gears/turbostar.hpp"
#include "gears/faster.hpp"
#include "mechanics/autopilot.hpp"
#include "gears/hypersonic.hpp"
#include "mechanics/archetype/afterburner.hpp"
#include "handlers/player/specialflagtweaks.hpp"
#include "gears/blastGaugeGears.hpp"
//#include "gears/faster.hpp"
//#include "gears/hyperhangon.hpp"
//#include "gears/slidebooster.hpp"
//#include "gears/turbostar.hpp"
#include "gears/airtank.hpp"
#include <gears/berserker.hpp>

#include "gears/advantagep.hpp"
#include "gears/gambler.hpp"
#include "gears/omnipotence.hpp"
#include "handlers/menu/debugmenu/debugmenu_handlers.hpp"
#include "gears/darkness.hpp"
//#include "tweaks/stage/itemrngchange.hpp"
#include "mechanics/speed_shoes.hpp"

#include "gears/advantagep.hpp"
#include "gears/gambler.hpp"

// ASMDefined void Player_Autopilot(Player *player); // _Main/text.s ; C240F4D8

ASMUsed void PlayerHandler(Player &player) {
    // this is just a main function that calls all the handlers

    if (player.state != PlayerState::Fly) {
	    player.splashCanyonFlyRoute = false;
    }

    lbl_Slipstream(player);
    lbl_FastFall(player);
    // HyperSonic_RainbowLighting(player);
    Player_DeathPlane(player);
    Player_TimerHandler(player);
    Player_Autopilot(player);
    Player_MagneticImpulseTimer(player);
    Player_BeginnerOmegaBoost(player);
    Player_CoverF(player);
    Player_Fastest(player);
    Player_SuperTailsTransformation(player);
    Player_SuperMetalTransformation(player);
    Player_CheckBeginnerAirOut(player);
    Player_GrindRailDashHandler(player);
    Player_FlyHoopDashHandler(player);
    Player_LCancelHandler(player);
    Player_GRHandler(player);
    // Player_DriftTips(player);
    Player_LimiterCut(player);
    Player_CrazyStop(player);
    Player_HangOn(player);
    Player_AdvantageF(player);
    Player_MagicCarpet(player);
    Player_CoverS(player);
    Player_CoverP(player);
    Player_PowerGear(player);
    Player_RingGearLevels(player);
    Player_StardustSpeederII(player);
    Player_SSIIRingBoost(player);
    Player_BoostArchetypeJCBC(player);
    Player_SpeedBalancerResetBoostSpeed(player);
    CustomCodehandler_Player(player);
    Player_HyperdriveHandler2(&player);
    Player_HyperSonicUpdatePlayerStats(player);
    Player_HyperSonicTotalLinkCalc(player);
    Player_HyperSonicRingStream(player);
    Player_Faster(player);
    Player_SlideBooster(player);
    Player_TurboStar(player);
    Player_AirTank(player);
    Player_IgnoreTurbulenceHandler(player);
    Player_AirOutButton(player);
    Player_Afterburner(player);
    Player_lightBoardEffect(player);
    Player_moneyCrisis(player);
    Player_storeFlagInfo(player);
    Player_BlastGaugeUpdateStatus(player);
    DebugMenuHandler_InfiniteAir(player);
    DebugMenuHandler_InfiniteRings(player);
    Player_SuperShadow(player);
    Player_Omnipotence(player);
    Player_AdvantageP(player);
    Player_GamblerBoost(player);
	Player_BerserkerSpeedCheck(&player);
	Player_SuperMetalIII(player);
	Player_Darkness(player);
    Player_cancelAttackHack(player);
	// Player_HandleSpeedShoes(player); // tap to speed shoe
}