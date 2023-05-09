#include "chuchurocket_dashpanel.hpp"


USED f32 Player_SegaIllusion_ChuChuRocketDashPanelSpeed(Player *player, ChuChuRocketDashPanel *object) {
    u32 id = object->properties->id;
    f32 dashPanelSpeed = object->speed + ChuChuDashPanelSpeeds[id-1];
    dashPanelSpeed += (pSpeed(10) * static_cast<f32>(player->placement));
    return dashPanelSpeed;
}