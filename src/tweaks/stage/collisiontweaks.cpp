#include "context.hpp"

ASMUsed void CustomCollisionTweaks(Collision *collision) {
    switch (CurrentStage) {
        case NightChase:
            // trick truck
            if (collision->bound_scaleUL == 0x4103c082) {
                collision->properties = ManualRampCollision + 1;
            }
            break;

        case SegaIllusion:
            // ramp skip fix
            if (collision->bound_scaleUL == 0x430d1270) {
                collision->properties = ForwardFlipRampCollision;
            } else if (collision->bound_scaleUL == 0x437499bb) {
                collision->properties = BackFlipRampCollision;
            }
            break;

        case GreenCave:
            if (collision->bound_scaleUL == 0x43a5e9d3) {
                collision->properties = ForwardFlipRampCollision;
            }
            break;
		default:
			break;
    }
}