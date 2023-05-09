#pragma once

#include "context.hpp"

ASMUsed void Player_checkGearIfUsesBlastGauge(Player *player);
ASMUsed void Player_blastGaugeHandler(Player *player);
ASMUsed void Player_resetGauge(Player *player);
//global void lbl_drawDynamicGauge();
//global void lbl_airGearDrawAirGauge();
global void lbl_drawAirGauge(); // allows drawing the air gauge for any gear if called
global void lbl_0002AD54(); // skips drawing the gauge