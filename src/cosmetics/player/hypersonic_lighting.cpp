#include "hypersonic_lighting.hpp"
#include "exloads.hpp"

std::array<HyperSonic_ColorState, MaxPlayerCount> HyperSonic_ColorState;


inline RGBModule TransitionColorValues(RGBModule *fromValues, RGBModule *toValues) {
	// calculate differences
	return RGBModule{
			(toValues->R - fromValues->R) * 0.25F,
			(toValues->G - fromValues->G) * 0.25F,
			(toValues->B - fromValues->B) * 0.25F
	};
}


void HyperSonic_RainbowLighting(Player *player) {
	void **modeldataptr = &lbl_1000DFE4 + player->index;
	void *lightingptr = static_cast<u32*>(*modeldataptr) + 5;
	void **temp2 = static_cast<void **>(lightingptr);
	void *generallightingptr = static_cast<u32 *>(*temp2) + 7;
	void **temp = static_cast<void **>(generallightingptr);
	auto *lighting =  static_cast<Lighting *>(*temp);
	RGBModule originalValues{}, toValues{}, finalValues;
	const u8 &index = player->index;
	const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*player);

	if (exLoads.gearExLoadID != HyperSonicEXLoad) { return; }

	if (!player->hyperSonicInit) {
		lighting->red1 = 0.5647058823529412f;
		//lighting->red2 = 1;
		lighting->green1 = 0;
		//lighting->green2 = 0;
		lighting->blue1 = 1;
		//lighting->blue2 = 0;
		player->hyperSonicInit = 1;
		HyperSonic_ColorState[index].state = 0;
		HyperSonic_ColorState[index].transitionFrames = 0;
	}

	if (gu32GameCnt % 8 == 0) { // every 8 frames
		HyperSonic_ColorState[index].state += 1;
		HyperSonic_ColorState[index].transitionFrames = 0;
		if (HyperSonic_ColorState[index].state >= 6) {
			HyperSonic_ColorState[index].state = 0;
		}
	}

	switch (HyperSonic_ColorState[index].state) {
		case 0:
			if (HyperSonic_ColorState[index].transitionFrames >= 4) {
				break;
			}
			HyperSonic_ColorState[index].transitionFrames += 1;

			originalValues.R = 0.5647058823529412f;
			originalValues.G = 0;
			originalValues.B = 1;
			// red
			toValues.R = 1;
			toValues.G = 0;
			toValues.B = 0;
			finalValues = TransitionColorValues(&originalValues, &toValues);

			lighting->red1 += finalValues.R;
			lighting->green1 += finalValues.G;
			lighting->blue1 += finalValues.B;

			//lighting->red1 = 1;
			//lighting->red2 = 1;
			//lighting->green1 = 0;
			//lighting->green2 = 0;
			//lighting->blue1 = 0;
			//lighting->blue2 = 0;
			break;
		case 1:
			if (HyperSonic_ColorState[index].transitionFrames >= 4) {
				break;
			}
			HyperSonic_ColorState[index].transitionFrames += 1;

			originalValues.R = 1;
			originalValues.G = 0;
			originalValues.B = 0;
			// blue
			toValues.R = 0;
			toValues.G = 0;
			toValues.B = 1;
			finalValues = TransitionColorValues(&originalValues, &toValues);

			lighting->red1 += finalValues.R;
			lighting->green1 += finalValues.G;
			lighting->blue1 += finalValues.B;

			//lighting->red1 = 0;
			//lighting->red2 = 0;
			//lighting->green1 = 0;
			//lighting->green2 = 0;
			//lighting->blue1 = 1;
			//lighting->blue2 = 1;
			break;
		case 2:
			if (HyperSonic_ColorState[index].transitionFrames >= 4) {
				break;
			}
			HyperSonic_ColorState[index].transitionFrames += 1;

			originalValues.R = 0;
			originalValues.G = 0;
			originalValues.B = 1;
			// green
			toValues.R = 0;
			toValues.G = 1;
			toValues.B = 0;
			finalValues = TransitionColorValues(&originalValues, &toValues);

			lighting->red1 += finalValues.R;
			lighting->green1 += finalValues.G;
			lighting->blue1 += finalValues.B;

			//lighting->red1 = 0;
			//lighting->red2 = 0;
			//lighting->green1 = 1;
			//lighting->green2 = 1;
			//lighting->blue1 = 0;
			//lighting->blue2 = 0;
			break;
		case 3:
			if (HyperSonic_ColorState[index].transitionFrames >= 4) {
				break;
			}
			HyperSonic_ColorState[index].transitionFrames += 1;

			originalValues.R = 0;
			originalValues.G = 1;
			originalValues.B = 0;
			// yellow
			toValues.R = 1;
			toValues.G = 1;
			toValues.B = 0;
			finalValues = TransitionColorValues(&originalValues, &toValues);

			lighting->red1 += finalValues.R;
			lighting->green1 += finalValues.G;
			lighting->blue1 += finalValues.B;

			//lighting->red1 = 1;
			//lighting->red2 = 1;
			//lighting->green1 = 1;
			//lighting->green2 = 1;
			//lighting->blue1 = 0;
			//lighting->blue2 = 0;
			break;
		case 4:
			if (HyperSonic_ColorState[index].transitionFrames >= 4) {
				break;
			}
			HyperSonic_ColorState[index].transitionFrames += 1;

			originalValues.R = 1;
			originalValues.G = 1;
			originalValues.B = 0;
			// white
			toValues.R = 1;
			toValues.G = 1;
			toValues.B = 1;
			finalValues = TransitionColorValues(&originalValues, &toValues);

			lighting->red1 += finalValues.R;
			lighting->green1 += finalValues.G;
			lighting->blue1 += finalValues.B;

			//lighting->red1 = 1;
			//lighting->red2 = 1;
			//lighting->green1 = 1;
			//lighting->green2 = 1;
			//lighting->blue1 = 1;
			//lighting->blue2 = 1;
			break;
		case 5:
			if (HyperSonic_ColorState[index].transitionFrames >= 4) {
				break;
			}
			HyperSonic_ColorState[index].transitionFrames += 1;

			originalValues.R = 1;
			originalValues.G = 1;
			originalValues.B = 1;
			// violet
			toValues.R = 0.5647058823529412f;
			toValues.G = 0;
			toValues.B = 1;
			finalValues = TransitionColorValues(&originalValues, &toValues);

			lighting->red1 += finalValues.R;
			lighting->green1 += finalValues.G;
			lighting->blue1 += finalValues.B;

			//lighting->red1 = 0.5647058823529412;
			//lighting->red2 = 0.5647058823529412;
			//lighting->green1 = 0;
			//lighting->green2 = 0;
			//lighting->blue1 = 1;
			//lighting->blue2 = 1;
			break;
		default:
			break;
	}
}