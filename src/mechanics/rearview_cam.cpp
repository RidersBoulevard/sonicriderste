#include <riders/stage.hpp>

#include "containers/vector3.hpp"
#include "globalDefs.hpp"
#include "nn/struct/camera.hpp"
#include "riders/gamemode.hpp"
#include "riders/player.hpp"
#include "types.hpp"

ASMDefined void PSVECScale(const Vector3F &src, Vector3F &dst, f32 scale);
ASMDefined void PSVECSubtract(const Vector3F &a, const Vector3F &b, Vector3F &ab);
ASMDefined void PSVECNormalize(const Vector3F &src, Vector3F &unit);

ASMUsed void PlayerApplyRearviewCamera(const Player &player, NNS_CameraTargetUpVector &camera) {
    constexpr auto CAMERA_DISTANCE = 5.f;
    const auto raceState = getRaceState();

    if (raceState != RaceState::Countdown && raceState != RaceState::Active) return;
    if (player.input->rightStickVertical > -80) return;

    switch (CurrentStage) {
        case SplashCanyon:
        case RedCanyon:
            // Waterfall & windmill
            if (player.state == PlayerState::QTE
                || player.state == PlayerState::QTE2
                || (player.state == PlayerState::Fall && player.previousState == PlayerState::QTE)) return;
                break;
        case WhiteCave:
            // Final web && following tunnel
                if (player.state == PlayerState::QTE2
                    || (player.state == PlayerState::QTE && player.previousState == PlayerState::BackflipRamp)) return;
        break;
        case SandRuins:
            // Arrow QTE
            if (player.state == PlayerState::QTE
                || player.state == PlayerState::QTE2
                || (player.state == PlayerState::Fall && player.previousState == PlayerState::Cruise && player.qteState == PlayerState::QTE)) return;
            break;
        case DigitalDimension:
            // Falling from first QTE
            if (player.state == PlayerState::Fall && player.previousState == PlayerState::Cruise && player.qteState == PlayerState::QTE2) return;
        default: break;
    }

    Vector3F playerPos{player.x, player.y, player.z};

    Vector3F dir;
    PSVECSubtract(playerPos, camera.Target, dir);
    PSVECNormalize(dir, dir);

    PSVECScale(dir, dir, CAMERA_DISTANCE);

    Vector3F newCamPos;
    PSVECSubtract(playerPos, dir, newCamPos);
    newCamPos.y += 2.0f;
    playerPos.y += 2.0f;

    camera.Target = playerPos;
    camera.Position = newCamPos;
    camera.Fov = nn::DegToA32(60.f);
}