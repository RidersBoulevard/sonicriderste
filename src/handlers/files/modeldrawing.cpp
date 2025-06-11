#include "modeldrawing.hpp"
#include "packman_handlers.hpp"
#include "cosmetics/player/exloads.hpp"
#include "riders/gamemode.hpp"

std::array<f32, MaxPlayerCount> Player_MatMotFrameCounter;
ASMDefined f32 gf32Motion_1Frame;

/**
 * Draws all players in a race. Behaves exactly like the vanilla function, with added functionality of drawing material motions on player models, if applicable.
 *
 * @param player The player whose model we're drawing.
 * @param nnsObject The object to draw.
 * @param nnsMtxPal The matrix palette to use.
 * @param nnsNodeStat The node status to use.
 */
ASMUsed void DrawPlayerWithMatMot(const Player &player, NNS_Object *nnsObject, NNS_Matrix *nnsMtxPal, NNS_NodeStatus *nnsNodeStat) {
    const u32 mirrorFlag = static_cast<const u32>(((player.unkBAC & 0x100) ? 1 : 0) * 0x40);// NOLINT(readability-implicit-bool-conversion)

    if (gpsaMatMotObj_Player[player.index] != nullptr) {
        // player model has material animations applied

        NNS_MatMotion *matMotion = gpsaMatMotion_Player[player.index];
        f32 frame = Player_MatMotFrameCounter[player.index] + gf32Motion_1Frame;
        if (frame > matMotion->endFrame) {
            frame = 0.0f;
        }
        Player_MatMotFrameCounter[player.index] = frame;

        nnCalcMaterialMotion(gpsaMatMotObj_Player[player.index], nnsObject, matMotion, frame);
        nnDrawMaterialMotionObject(gpsaMatMotObj_Player[player.index], nnsMtxPal, nnsNodeStat, 1U << 31U, mirrorFlag);
    } else {
        // player model doesn't have material animations applied
        if (static_cast<bool>(mirrorFlag)) {
            nnDrawObjectExt(nnsObject, nnsMtxPal, nnsNodeStat, 1U << 31U, mirrorFlag);
        } else {
            nnDrawObjectLtd(nnsObject, nnsMtxPal, nnsNodeStat, 1U << 31U, mirrorFlag);
        }
    }
}