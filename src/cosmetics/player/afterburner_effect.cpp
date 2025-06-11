#include "globalDefs.hpp"
#include "riders/object.hpp"
#include "handlers/files/packman_handlers.hpp"
#include "ninjanext.hpp"
#include "lib/lib.hpp"
#include "lib/stdlib.hpp"
#include "containers/particle_details.hpp"
#include "mechanics/magneticimpulse.hpp"

constexpr auto MAX_MTXSTACK_COUNT = 2;

ASMDefined ObjectNode *gpsCurrentTask;
ASMDefined f32 gf32Motion_1Frame;
ASMDefined void *gsNnMtxStack;
ASMDefined Matrix3x3F gaNnViewMtx;
ASMDefined ParticleDetails tsAirCharge_Par_DefData;
ASMDefined void func_Particle_Task();
ASMDefined NNS_TexList *gpsTexList_EffectAirChargeParticle;
ASMDefined u32 *gpsaNodeStat_Player[];

struct AfterburnerEffectObject1 {
    f32 motionFrame;
    Matrix3x3F *matrixStack;
};
static_assert(sizeof(AfterburnerEffectObject1) < 0x20);

struct AirChargeDef {
    u32 bone;
    f32 trans_x;
    f32 trans_y;
    f32 trans_z;
    s32 rot_x;
    s32 rot_y;
    s32 rot_z;
};
static_assert(sizeof(AirChargeDef) == 0x1C);

/*
 * Index 0: For boards
 * Index 1: For bikes
 * Index 2: For right foot skate
 * Index 3: For left foot skate
 */
ASMDefined std::array<AirChargeDef, 4> tasAirChargeOfsData;

namespace {
    constexpr ParticleDetails AfterburnerParticleParams = {
            .particleType = static_cast<u8>(ParticleDraw::POLYGON),
            .particleCount = static_cast<u8>(ParticleType::SPIRAL),
            .rangeOpacity = ParticleCtrl::ALWAYS_FOLLOW_ME | ParticleCtrl::ROLL | ParticleCtrl::COL_RETURN,
            .unk4 = 0,
            .textureID = 0,
            .objectGroupID = ObjectGroups::Particle,
            .textureWidth = 0.1f,
            .textureHeight = 0.1f,
            .unk10 = 0,
            .mainParticleCount = 30,
            .subParticleCount = 3,
            .upwardsVelocity = 0.5f,
            .outwardsVelocity = 0.0078125f,
            .distributionVelocity = 0.0078125f,
            .downwardsVelocity = 0.0001f,
            .lifetime = 30,
            .frequency = 1,
            .mainParticleColor = {255, 255, 255, 255},
            .subParticleColor = {255, 0, 0, 255},
            .extraHorizontalDistribution = 0.0f,
            .extraVerticalDistribution = 0.0f,
            .unk54 = 0,
            .unk58 = 0,
            .unk5C = 0,
            .unk60 = 0,
            .unk64 = 0,
            .unk68 = 0,
            .unk6C = 0,
            .unk70 = 0.0f,
    };

    constexpr ParticleDetails AfterburnerLineParticleParams = {
            .particleType = static_cast<u8>(ParticleDraw::RADILINE),
            .particleCount = static_cast<u8>(ParticleType::RANDOM),
            .rangeOpacity = ParticleCtrl::ALWAYS_FOLLOW_ME | ParticleCtrl::ROLL | ParticleCtrl::COL_RETURN,
            .unk4 = 0,
            .textureID = 0x0,
            .objectGroupID = ObjectGroups::Particle,
            .textureWidth = 0.12f,
            .textureHeight = 0.12f,
            .unk10 = 0,
            .mainParticleCount = 7,
            .subParticleCount = 1,
            .upwardsVelocity = 0.16f,
            .outwardsVelocity = 0.001f,
            .distributionVelocity = 0.f,
            .downwardsVelocity = 0.0125f,
            .lifetime = 4,
            .frequency = 2,
            .mainParticleColor = {255, 255, 255, 255},
            .subParticleColor = {255, 0, 0, 255},
            .extraHorizontalDistribution = 0.06f,
            .extraVerticalDistribution = 0.06f,
            .unk54 = 0,
            .unk58 = 0,
            .unk5C = 0,
            .unk60 = 0,
            .unk64 = 0,
            .unk68 = 0,
            .unk6C = 0,
            .unk70 = 0.0f,
    };

    constexpr ParticleDetails AfterburnerSkateLineParticleParams = {
            .particleType = static_cast<u8>(ParticleDraw::RADILINE),
            .particleCount = static_cast<u8>(ParticleType::RANDOM),
            .rangeOpacity = ParticleCtrl::ALWAYS_FOLLOW_ME | ParticleCtrl::ROLL | ParticleCtrl::COL_RETURN,
            .unk4 = 0,
            .textureID = 0x0,
            .objectGroupID = ObjectGroups::Particle,
            .textureWidth = 0.06f, // controls the length of the line
            .textureHeight = 0.06f,
            .unk10 = 0,
            .mainParticleCount = 5,
            .subParticleCount = 1,
            .upwardsVelocity = 0.16f,
            .outwardsVelocity = 0.001f,
            .distributionVelocity = 0.f,
            .downwardsVelocity = 0.0125f,
            .lifetime = 3,
            .frequency = 2,
            .mainParticleColor = {255, 255, 255, 255},
            .subParticleColor = {255, 0, 0, 255},
            .extraHorizontalDistribution = 0.06f,
            .extraVerticalDistribution = 0.06f,
            .unk54 = 0,
            .unk58 = 0,
            .unk5C = 0,
            .unk60 = 0,
            .unk64 = 0,
            .unk68 = 0,
            .unk6C = 0,
            .unk70 = 0.0f,
    };

    inline ParticleTaskObject1 *MakeParticleTask() {
        return static_cast<ParticleTaskObject1 *>(SetTask(func_Particle_Task, ObjectGroups::Particle, 2)->object);
    }

    /**
     * Takes care of updating the matrix that the particles (and possibly an animated model) will use.
     *
     * @param obj1 Afterburner effect data.
     * @param player The player to apply the update on.
     * @param airChargeDef Defining data that determines how the matrix is formed.
     * @param mtxIdx Which matrix to update.
     */
    inline void UpdateParticleMatrix(AfterburnerEffectObject1 *obj1, Player &player, const AirChargeDef &airChargeDef, const u32 mtxIdx) {
        auto *plyMtxList = gpsaMtxList_Player[player.index];
        Matrix3x3F rotationMat;
        nnMakeRotateXYZMatrix(&rotationMat, airChargeDef.rot_x, airChargeDef.rot_y, airChargeDef.rot_z);

        Matrix3x3F boneMat;
        nnTranslateMatrix(&boneMat, plyMtxList + airChargeDef.bone, airChargeDef.trans_x, airChargeDef.trans_y,
                          airChargeDef.trans_z);

        Matrix3x3F rotBoneMat;
        nnMultiplyMatrix(&boneMat, &rotationMat, rotBoneMat);

        nnMultiplyMatrix(&player.unkC4, &rotBoneMat, obj1->matrixStack[mtxIdx]);
        obj1->matrixStack[MAX_MTXSTACK_COUNT + mtxIdx] = obj1->matrixStack[mtxIdx]; // this copy will be used for line particles
    }

    /**
     * Gets the starting index for some defining data on which bone and where exactly should the particles position themselves.
     *
     * @param player The player to obtain this data from.
     * @return The proper index
     */
    [[nodiscard]] inline u8 GetAirChargeDefStartIdx(const Player &player) {
        u8 idx = 2; // skates
        if (player.extremeGear < ExtremeGear::ERider) idx = 0; // board
        else if (player.extremeGear < ExtremeGear::Darkness) idx = 1; // bike
        return idx;
    }

    inline void UpdateParticleMatrixAll(AfterburnerEffectObject1 *obj1, Player &player) {
        const u8 defStartIdx = GetAirChargeDefStartIdx(player);

        UpdateParticleMatrix(obj1, player, tasAirChargeOfsData[defStartIdx], 0);
        if (player.gearType == GearType::Skates) {
            UpdateParticleMatrix(obj1, player, tasAirChargeOfsData[defStartIdx + 1], 1);
        }
    }

    /**
     * Draws an animated model alongside all the particles. These are set up from the TSGC file.
     * Particle matrices must be up to date via UpdateParticleMatrix() before using this function.
     * This will most likely stay unused for now.
     *
     * @note This function probably doesn't work with drawing multiple models, like on skates!
     * @param obj1 Afterburner effect data.
     * @param mtxIdx Which matrix to use.
     */
    [[maybe_unused]] inline void DrawAnimatedParticleModel(AfterburnerEffectObject1 *obj1, const u32 mtxIdx) {
        auto *obj = AfterburnerEffect_Object;
        auto *matMotObj = AfterburnerEffect_MatMotObj;
        auto *matMotion = AfterburnerEffect_MatMotion;
        auto *nodeStat = AfterburnerEffect_NodeStat;
        auto *mtxPal = AfterburnerEffect_MtxPal;
        auto motionFrame = obj1->motionFrame;

        nnCalcMaterialMotion(matMotObj, obj, matMotion, motionFrame);
        nnSetTextureList(AfterburnerEffect_TexList);
        nnSetUpNodeStatusList(nodeStat, obj->nNode, 0);

        Matrix3x3F finalMat;
        nnMultiplyMatrix(&gaNnViewMtx, &obj1->matrixStack[mtxIdx], finalMat);

        nnCalcMatrixPaletteMotion(mtxPal, nodeStat, obj, AfterburnerEffect_Motion, motionFrame, &finalMat,
                                  &gsNnMtxStack, 0);
        nnDrawMaterialMotionObject(matMotObj, mtxPal, nodeStat, 1U << 31U, 0);

        const f32 incrementedFrame = motionFrame + gf32Motion_1Frame;
        if (incrementedFrame > matMotion->endFrame) {
            obj1->motionFrame = 0.0f;
        } else {
            obj1->motionFrame = incrementedFrame;
        }
    }

    /**
     * Makes the main "dot" particles that stay in the exhaust of the board/skate.
     * Particle matrices must be up to date via UpdateParticleMatrix() before using this function.
     *
     * @param obj1 Afterburner effect data.
     * @param player The player to apply it to.
     * @param mtxIdx Which matrix to use.
     */
    inline void MakeMainParticles(AfterburnerEffectObject1 *obj1, Player &player, const u32 mtxIdx) {
        if (gu32GameCnt % 30 == 0) {
            auto *particles = MakeParticleTask();
            particles->unk72 = player.index;
            particles->x = 0.0f;
            particles->y = 0.0f;
            particles->z = 0.0f;
            particles->unk30 = 0.0f;
            particles->unk34 = 0.0006f;
            particles->unk38 = 0.0f;
            particles->unk3C = 1.0f;
            particles->unk40 = 0.0f;
            particles->unk44 = 0.0f;
            particles->unk48 = &gcosNnSystemVecZeroFast;
            particles->unk10 = 0.0f;
            particles->unk14 = 0.01f;
            particles->unk18 = 0.005f;
            particles->particleParams = &AfterburnerParticleParams;
            particles->texList = gpsTexList_EffectAirChargeParticle;
            particles->baseModelMatrix = &obj1->matrixStack[mtxIdx];
            particles->unk73 = static_cast<u8>(-1);
            particles->unk74 = 0;
        }

        nnRotateXMatrix(&obj1->matrixStack[mtxIdx], &obj1->matrixStack[mtxIdx], nn::FloatToBAM(-90.0f));
    }

    /**
     * Makes the line particles that shoot out the back of the board/skate.
     * Particle matrices must be up to date via UpdateParticleMatrix() before using this function.
     *
     * @param obj1 Afterburner effect data.
     * @param player The player to apply it to.
     * @param mtxIdx Which matrix to use.
     */
    inline void MakeLineParticles(AfterburnerEffectObject1 *obj1, Player &player, const u32 mtxIdx) {
        auto *lineParticles = MakeParticleTask();
        lineParticles->unk72 = player.index;
        lineParticles->x = 0.0f;
        lineParticles->y = 0.0f;
        lineParticles->z = 0.0f;
        lineParticles->unk30 = 0.0f;
        lineParticles->unk34 = 1.0f;
        lineParticles->unk38 = 0.0f;
        lineParticles->unk3C = 0.3f;
        lineParticles->unk40 = -0.6f;
        lineParticles->unk44 = 0.3f;
        lineParticles->unk10 = 0.0f;
        lineParticles->unk14 = 0.3f;
        lineParticles->unk18 = -0.6f;
        lineParticles->unk48 = &gcosNnSystemVecZeroFast;
        const auto *params = player.gearType == GearType::Skates ? &AfterburnerSkateLineParticleParams : &AfterburnerLineParticleParams;
        lineParticles->particleParams = params;
        lineParticles->texList = gpsTexList_Particle;
        lineParticles->baseModelMatrix = &obj1->matrixStack[MAX_MTXSTACK_COUNT + mtxIdx];
        lineParticles->unk73 = static_cast<u8>(-1);
        lineParticles->unk74 = 0;
    }
}

/// Task that handles rendering the visual effects for afterburner. This task runs independently for each player.
ASMUsed void Player_AfterburnerEffectTask() {
    auto *taskObj = gpsCurrentTask;
    auto *obj1 = static_cast<AfterburnerEffectObject1 *>(taskObj->object);

    switch (taskObj->state) {
        case 2: {
            // make space for 4 matrices, max 2 matrices for particles + 2 copies
            obj1->matrixStack = static_cast<Matrix3x3F *>(gNp_MallocHi(4, sizeof(Matrix3x3F) * (MAX_MTXSTACK_COUNT * 2)));
            taskObj->state++;
            [[fallthrough]];
        }
        case 3: {
            if (gu32Game_PauseFlag != 0) break;

            const u8 playerIndex = taskObj->userDataU8;
            Player &player = players[playerIndex];

            if (!player.isRealPlayer()) break;

            UpdateParticleMatrixAll(obj1, player);

            if (MI::impulseData[playerIndex].afterburnerTimer == 0) break;

            u32 particleNum = 1;
            if (player.gearType == GearType::Skates) particleNum++;
            const u8 defStartIdx = GetAirChargeDefStartIdx(player);

            for (u32 i = 0; i < particleNum; i++) {
                const auto &airChargeDef = tasAirChargeOfsData[defStartIdx + i];

                // if the board/skate is hidden via animation, don't render particles for it
                if (gpsaNodeStat_Player[playerIndex][airChargeDef.bone] != 0) continue;

//                DrawAnimatedParticleModel(obj1, i);
                MakeMainParticles(obj1, player, i);
                MakeLineParticles(obj1, player, i);
            }

            break;
        }
        case 0 ... 1:
            free_Hi(obj1->matrixStack);
            gNp_DeadTask();
            break;
        default:
            break;
    }
}